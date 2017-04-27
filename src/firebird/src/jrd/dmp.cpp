/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dmp.cpp
 *	DESCRIPTION:	Logical page dumper
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "memory_routines.h"

#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "../jrd/ods.h"
#include "../jrd/cch.h"
#include "../jrd/pag.h"
#include "../jrd/val.h"
#include "../jrd/btr.h"
#include "../common/classes/timestamp.h"
#include "../jrd/tra.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dmp_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sqz_proto.h"


void (*dbg_block) (const BufferDesc*);

void (*dmp_active) () = DMP_active;
void (*dmp_dirty) () = DMP_dirty;
void (*dmp_page) (SLONG, USHORT) = DMP_page;

extern FILE* dbg_file;

static void btc_printer(SLONG, const BufferDesc*, SCHAR*);
static void btc_printer_errors(SLONG, const BufferDesc*, SCHAR*);
static void complement_key(UCHAR*, int);
static double decompress(const SCHAR*);
static void dmp_blob(const blob_page*);
static void dmp_data(const data_page*);
static void dmp_header(const header_page*);
static void dmp_index(const btree_page*, USHORT);
static void dmp_pip(const page_inv_page*, ULONG);
static void dmp_pointer(const pointer_page*);
static void dmp_root(const index_root_page*);
static void dmp_transactions(const tx_inv_page*, ULONG);

static int dmp_descending = 0;

// Why is this necessary? I see the same macros in tra.h and this file includes tra.h
// Commented them out.
//#define TRANS_SHIFT(number)	(((number) & TRA_MASK) << 1)
//#define TRANS_OFFSET(number)	((number) >> TRA_SHIFT)


void DMP_active()
{
/**************************************
 *
 *	D M P _ a c t i v e
 *
 **************************************
 *
 * Functional description
 *	Dump all buffers that are active.
 *
 **************************************/
	const Database* dbb = GET_DBB();

	const BufferControl* bcb = dbb->dbb_bcb;
	for (USHORT i = 0; i < bcb->bcb_count; i++)
	{
		const BufferDesc* bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (bdb->bdb_use_count)
		{
			if (*dbg_block != NULL)
			{
				(*dbg_block)(bdb);
			}
			DMP_page(bdb->bdb_page, 0);
		}
	}
}


void DMP_btc()
{
/**************************************
 *
 *	D M P _ b t c
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree.
 *
 **************************************/
	SCHAR buffer[250];

	const Database* dbb = GET_DBB();

	SLONG level = 0;
	const BufferDesc* bdb = dbb->dbb_bcb->bcb_btree;

	memset(buffer, ' ', sizeof(buffer));
	buffer[249] = 0;

	if (bdb)
		btc_printer(level, bdb, buffer);
	fprintf(dbg_file, "%s\n", buffer);
}


void DMP_btc_errors()
{
/**************************************
 *
 *	D M P _ b t c _ e r r o r s
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree.
 *
 **************************************/
	SCHAR buffer[250];

	const Database* dbb = GET_DBB();

	SLONG level = 0;
	const BufferDesc* bdb = dbb->dbb_bcb->bcb_btree;
	if (bdb)
		btc_printer_errors(level, bdb, buffer);
}


void DMP_btc_ordered()
{
/**************************************
 *
 *	D M P _ b t c _ o r d e r e d
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree.
 *
 **************************************/
	Database* dbb = GET_DBB();

/* Pick starting place at leftmost node */

	fprintf(dbg_file,
			   "\nDirty Page Binary Tree -- Page (Transaction) { Dirty | Clean }\n");

	SLONG max_seen = -3;
	const BufferDesc* next;
	for (next = dbb->dbb_bcb->bcb_btree; next && next->bdb_left;
		 next = next->bdb_left);

	int i = 0;

	const BufferDesc* bdb;
	while (bdb = next) {
		if (!bdb->bdb_parent && bdb != dbb->dbb_bcb->bcb_btree) {
			for (bdb = dbb->dbb_bcb->bcb_btree; bdb;)
				if (bdb->bdb_left && max_seen < bdb->bdb_page)
					bdb = bdb->bdb_left;
				else if (bdb->bdb_right && max_seen > bdb->bdb_page)
					bdb = bdb->bdb_right;
				else
					break;
			if (!bdb)
				break;
		}

		/* Decide where to go next.  The options are (right, then down to the left)
		   or up */
		i++;
		if (bdb->bdb_right && max_seen < bdb->bdb_right->bdb_page)
			for (next = bdb->bdb_right; next->bdb_left;
				 next = next->bdb_left);
		else
			next = bdb->bdb_parent;

		if (max_seen >= bdb->bdb_page)
			continue;

		max_seen = bdb->bdb_page;

		fprintf(dbg_file, "\t%ld (%ld) %s%s", bdb->bdb_page,
				   bdb->bdb_transactions,
				   (bdb->bdb_flags & BDB_dirty) ? "D" : "C",
				   (i % 10) ? "," : "\n");

	}
	fprintf(dbg_file, "\n");
}


void DMP_dirty()
{
/**************************************
 *
 *	D M P _ d i r t y
 *
 **************************************
 *
 * Functional description
 *	Dump all buffers that are dirty.
 *
 **************************************/
	Database* dbb = GET_DBB();

	const BufferControl* bcb = dbb->dbb_bcb;
	for (USHORT i = 0; i < bcb->bcb_count; i++)
	{
		const BufferDesc* bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (bdb->bdb_flags & BDB_dirty)
		{
			if (*dbg_block != NULL)
			{
				(*dbg_block)(bdb);
			}
			DMP_page(bdb->bdb_page, 0);
		}
	}
}


void DMP_fetched_page(const pag* page,
					  ULONG		number,
					  ULONG		sequence,
					  USHORT page_size)
{
/**************************************
 *
 *	D M P _ f e t c h e d _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Dump a database page.  Actually, just case on type
 *	and disptach.  The sequence number is provided by
 *	the rebuild utility and passed as zero by the standard
 *	dump code.
 *
 **************************************/

	fprintf(dbg_file, "\n%ld\t", number);

	switch (page->pag_type)
	{
	case pag_header:
		dmp_header((const header_page*) page);
		break;

	case pag_pages:
		dmp_pip((const page_inv_page*) page, sequence);
		break;

	case pag_transactions:
		dmp_transactions((const tx_inv_page*) page, sequence);
		break;

	case pag_pointer:
		dmp_pointer((const pointer_page*) page);
		break;

	case pag_data:
		dmp_data((const data_page*) page);
		break;

	case pag_root:
		dmp_root((const index_root_page*) page);
		break;

	case pag_index:
		dmp_index((const btree_page*) page, page_size);
		break;

	case pag_blob:
		dmp_blob((const blob_page*) page);
		break;

	case pag_ids:
		fprintf(dbg_file, "GEN-IDS PAGE\n");
		break;

	case pag_log:
		fprintf(dbg_file, "WRITE-AHEAD LOG INFO PAGE\n");
		break;

	default:
		fprintf(dbg_file, "*** Page %ld (type %d) is undefined ***",
				   number, page->pag_type);
	}

	fprintf(dbg_file, "\n");
}


void DMP_page(SLONG number, USHORT page_size)
{
/**************************************
 *
 *	D M P _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Dump a database page.  Actually, just case on type
 *	and disptach.
 *
 **************************************/
	WIN window(number);
	const pag* page = CCH_FETCH(NULL, &window, LCK_read, 0);
	DMP_fetched_page(page, number, 0, page_size);
	CCH_RELEASE(NULL, &window);
}


static void btc_printer(SLONG level, const BufferDesc* bdb, SCHAR* buffer)
{
/**************************************
 *
 * 	b t c _ p r i n t e r
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree recursively
 *
 **************************************/

	if (level >= 48) {
		fprintf(dbg_file, "overflow\n");
		return;
	}

	sprintf((buffer + 5 * level + 1), "%.4ld", bdb->bdb_page);

	if (bdb->bdb_left) {
		buffer[5 * (level + 1)] = 'L';
		btc_printer(level + 1, bdb->bdb_left, buffer);
	}
	else
		fprintf(dbg_file, "%s\n", buffer);

	memset(buffer, ' ', 250);
	buffer[249] = 0;

	if (bdb->bdb_right) {
		buffer[5 * (level + 1)] = 'R';
		btc_printer(level + 1, bdb->bdb_right, buffer);
	}
}


static void btc_printer_errors(SLONG level, const BufferDesc* bdb, SCHAR* buffer)
{
/**************************************
 *
 * 	b t c _ p r i n t e r _ e r r o r s
 *
 **************************************
 *
 * Functional description
 *	Dump the dirty page b-tree recursively
 *
 **************************************/

	if (((bdb->bdb_left) && (bdb->bdb_left->bdb_page > bdb->bdb_page)) ||
		((bdb->bdb_right) && (bdb->bdb_right->bdb_page < bdb->bdb_page)))
	{
		fprintf(dbg_file, "Whoops! Parent %ld, Left %ld, Right %ld\n",
				   bdb->bdb_page,
				   (bdb->bdb_left) ? bdb->bdb_left->bdb_page : 0,
				   (bdb->bdb_right) ? bdb->bdb_right->bdb_page : 0);
	}

	if (bdb->bdb_left)
		btc_printer_errors(level + 1, bdb->bdb_left, buffer);
	if (bdb->bdb_right) {
		btc_printer_errors(level + 1, bdb->bdb_right, buffer);
	}
}


static void complement_key(UCHAR* p, int length)
{
/**************************************
 *
 *	c o m p l e m e n t _ k e y
 *
 **************************************
 *
 * Functional description
 *	Negate a key for descending index.
 *
 **************************************/
	for (const UCHAR* end = p + length; p < end; p++)
		*p ^= -1;
}


static double decompress(const SCHAR* value)
{
/**************************************
 *
 *	d e c o m p r e s s
 *
 **************************************
 *
 * Functional description
 *	Re-form a double precision number out of a compressed
 *	value string (assume string has been null padded to 8
 *	bytes).
 *
 **************************************/
	double dbl;

	char* p = (SCHAR *) & dbl;

	if (*value & (1 << 7)) {
		*p++ = static_cast<SCHAR>(*value++ ^ (1 << 7));
		int l = 7;
		do {
			*p++ = *value++;
		} while (--l);
	}
	else {
		int l = 8;
		do {
			*p++ = -*value++ - 1;
		} while (--l);
	}

	return dbl;
}


static void dmp_blob(const blob_page* page)
{
/**************************************
 *
 *	d m p _ b l o b
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fprintf(dbg_file,
			   "BLOB PAGE\t checksum %d\t generation %ld\n\tFlags: %x, lead page: %d, sequence: %d, length: %d\n\t",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   ((PAG) page)->pag_flags, page->blp_lead_page,
			   page->blp_sequence, page->blp_length);

	if (((PAG) page)->pag_flags & blp_pointers) {
		const int n = page->blp_length >> SHIFTLONG;
		const ULONG* ptr = (ULONG *) page->blp_page;
		for (int i = 0; i < n; i++, ptr++)
			fprintf(dbg_file, "%d,", *ptr);
	}

	fprintf(dbg_file, "\n");
}


static void dmp_data(const data_page* page)
{
/**************************************
 *
 *	d m p _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Dump a data page in a semi-readable format.
 *
 **************************************/
	SCHAR	buffer[8096 + 1];

	fprintf(dbg_file,
			   "DATA PAGE\t checksum %d\t generation %ld\n\tRelation: %d, Sequence: %d, Count: %d, Flags: %x\n",
			   ((PAG) page)->pag_checksum,
			   ((PAG) page)->pag_generation,
			   page->dpg_relation,
			   page->dpg_sequence,
			   page->dpg_count,
			   ((PAG) page)->pag_flags);

	int i = 0;
	for (const data_page::dpg_repeat* index = page->dpg_rpt; i < page->dpg_count;
		i++, index++)
	{
		if (index->dpg_length == 0)
		{
			fprintf(dbg_file, "\n\t%d - (empty)\n", i);
			continue;
		}
		const rhd* header = (rhd*) ((SCHAR *) page + index->dpg_offset);
		const rhdf* fragment = (rhdf*) header;
		if (header->rhd_flags & rhd_blob)
		{
			const blh* blob = (blh*) header;
			fprintf(dbg_file,
					   "\n\t%d - (blob) offset: %d, length: %d, flags: %x\n",
					   i, index->dpg_offset, index->dpg_length,
					   header->rhd_flags);
			fprintf(dbg_file,
					   "\tlevel: %d, lead page: %d, length: %d, count %d\n",
					   blob->blh_level, blob->blh_lead_page, blob->blh_length,
					   blob->blh_count);
			fprintf(dbg_file,
					   "\tmaxseq: %d, maxseg: %d, flags: %X, sub_type %d\n",
					   blob->blh_max_sequence, blob->blh_max_segment,
					   blob->blh_flags, blob->blh_sub_type);
		}
		else
		{
			SSHORT expanded_length = 0;
			SSHORT length;
			const char* p;
			if (index->dpg_offset)
			{
				if (header->rhd_flags & rhd_incomplete)
				{
					length = index->dpg_length - OFFSETA(rhdf*, rhdf_data);
					p = (SCHAR *) ((rhdf*) header)->rhdf_data;
				}
				else
				{
					length = index->dpg_length - OFFSETA(rhd*, rhd_data);
					p = (SCHAR *) header->rhd_data;
				}
				const char* q = p;
				for (const char* const end = p + length; q < end;)
				{
					if (*q > 0) {
						expanded_length += *q;
						q += *q + 1;
					}
					else {
						expanded_length += -(*q);
						q += 2;
					}
				}
			}
			fprintf(dbg_file,
					   "\n\t%d - offset: %d, length: %d, expanded data length: %d\n\t",
					   i, index->dpg_offset, index->dpg_length,
					   expanded_length);
			fprintf(dbg_file, "trans: %d, format: %d, flags: %#x\n\t",
					   header->rhd_transaction, (int) header->rhd_format,
					   header->rhd_flags);
			if (header->rhd_b_page)
			{
				fprintf(dbg_file, "back page: %d, line: %d\n\t",
						   header->rhd_b_page, header->rhd_b_line);
			}
			if (header->rhd_flags & rhd_incomplete)
			{
				fprintf(dbg_file, "frag page: %d, line: %d\n\t",
						   fragment->rhdf_f_page, fragment->rhdf_f_line);
			}
			if (index->dpg_offset)
			{
				if (length < 0)
				{
					fprintf(dbg_file, "*** invalid record length ***");
				}
				else if (length)
				{
					const char* const p_save = p;
					const int length_save = length;
					//int compress_value = 0;
					int cnt = 0;
					fprintf(dbg_file,
							   "Raw Compressed format: (length %d)\n\t",
							   length);
					do {
						if (cnt++ >= 16) {
							fprintf(dbg_file, "\n\t");
							cnt = 1;
						}

						fprintf(dbg_file, "%3d ", *p++);
					} while (--length);

					fprintf(dbg_file, "\n\t");

					buffer[0] = 0;
					const char* const end =
						SQZ_decompress((const UCHAR*) p_save, length_save, &buffer[1],
										 &buffer[sizeof(buffer)]);
					cnt = 0;
					p = &buffer[1];
					fprintf(dbg_file,
							   "Decompressed format: (length %d)\n\t",
							   end - p);
					do {
						if (cnt++ >= 20) {
							fprintf(dbg_file, "\n\t");
							cnt = 1;
						}

						if (isprint(*p) &&
							(isprint(*(p + 1)) || isprint(*(p - 1))))
						{
							fprintf(dbg_file, "%2c ", *p++);
						}
						else
						{
							fprintf(dbg_file, "%02x ", (UCHAR) *p++);
						}
					} while (p < end);

					fprintf(dbg_file, "\n");
				}
			}
		}
	}
}


static void dmp_header(const header_page* page)
{
/**************************************
 *
 *	d m p _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	const USHORT minor_version = page->hdr_ods_minor;

	fprintf(dbg_file,
			   "HEADER PAGE\t checksum %d\t generation %ld\n\tPage size: %d, version: %d.%d(%d), pages: %ld\n",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   page->hdr_page_size, page->hdr_ods_version & ~ODS_FIREBIRD_FLAG,
			   minor_version, page->hdr_ods_minor_original, page->hdr_PAGES);

	const Firebird::TimeStamp ts(*((GDS_TIMESTAMP *) page->hdr_creation_date));

	struct tm time;
	ts.decode(&time);

	fprintf(dbg_file, "\tCreation date:\t%s %d, %d %d:%02d:%02d\n",
			   FB_SHORT_MONTHS[time.tm_mon], time.tm_mday, time.tm_year + 1900,
			   time.tm_hour, time.tm_min, time.tm_sec);

	fprintf(dbg_file,
			   "\tOldest trans %ld, oldest_active %ld, oldest_snapshot %ld, next trans %ld, bumped trans %ld\n",
			   page->hdr_oldest_transaction, page->hdr_oldest_active,
			   page->hdr_oldest_snapshot, page->hdr_next_transaction,
			   page->hdr_bumped_transaction);

	fprintf(dbg_file,
			   "\tfile sequence # %d, flags %d, attachment %ld\n",
			   page->hdr_sequence, page->hdr_flags, page->hdr_attachment_id);


	fprintf(dbg_file,
			   "\timplementation %ld, shadow count %ld\n",
			   page->hdr_implementation, page->hdr_shadow_count);


	fprintf(dbg_file, "\n    Variable header data:\n");

	SLONG number;

	const char* p = (SCHAR *) page->hdr_data;
	for (const char* const end = p + page->hdr_page_size;
		 p < end && *p != HDR_end; p += 2 + p[1])
	{
		switch (*p)
		{
		case HDR_root_file_name:
			printf("\tRoot file name: %*s\n", p[1], p + 2);
			break;
/*
		case HDR_journal_server:
			printf("\tJournal server: %*s\n", p[1], p + 2);
			break;
*/
		case HDR_file:
			printf("\tContinuation file: %*s\n", p[1], p + 2);
			break;

		case HDR_last_page:
			memcpy(&number, p + 2, sizeof(number));
			printf("\tLast logical page: %ld\n", number);
			break;
/*
		case HDR_unlicensed:
			memcpy(&number, p + 2, sizeof(number));
			printf("\tUnlicensed accesses: %ld\n", number);
			break;
*/
		case HDR_sweep_interval:
			memcpy(&number, p + 2, sizeof(number));
			printf("\tSweep interval: %ld\n", number);
			break;

		case HDR_log_name:
			printf("\tLog file name: %*s\n", p[1], p + 2);
			break;
/*
		case HDR_journal_file:
			printf("\tJournal file: %*s\n", p[1], p + 2);
			break;
*/
		case HDR_password_file_key:
			printf("\tPassword file key: (can't print)\n");
			break;
/*
		case HDR_backup_info:
			printf("\tBackup info: (can't print)\n");
			break;

		case HDR_cache_file:
			printf("\tShared cache file: %*s\n", p[1], p + 2);
			break;
*/
		default:
			printf("\tUnrecognized option %d, length %d\n", p[0], p[1]);
		}
	}

	printf("\t*END*\n");
}


static void dmp_index(const btree_page* page, USHORT page_size)
{
/**************************************
 *
 *	d m p _ i n d e x
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fprintf(dbg_file,
			   "B-TREE PAGE\t checksum %d\t generation %ld\n"
			   "\tRelation: %d, Sibling: %ld, Backward Sibling: %ld, Level = %d, Length = %d, Flags = %d\n",
			   ((PAG) page)->pag_checksum,
			   ((PAG) page)->pag_generation,
			   page->btr_relation,
			   page->btr_sibling,
			   page->btr_left_sibling,
			   page->btr_level,
			   page->btr_length,
			   ((PAG) page)->pag_flags);
/* Compute the number of data pages per pointer page.  Each data page
   requires a 32 bit pointer and a 2 bit control field. */
	const USHORT dp_per_pp =
			(USHORT)((ULONG) ((page_size - OFFSETA(pointer_page*, ppg_page)) * 8) /
						(BITS_PER_LONG + 2));
	const USHORT max_records = (page_size - sizeof(data_page)) /
								(sizeof(data_page::dpg_repeat) + OFFSETA(rhd*, rhd_data));

	const btree_nod* const end  = (btree_nod*) ((UCHAR *) page + page->btr_length);
	btree_nod* node = (btree_nod*) page->btr_nodes;

	UCHAR	value[256];
	UCHAR	print[256];

	while (node < end)
	{
		const ULONG number = get_long(node->btn_number);

		/* compute running value */

		UCHAR* p = value + node->btn_prefix;
		const UCHAR* q = node->btn_data;
		SSHORT l = node->btn_length;
		if (l)
		{
			do {
				*p++ = *q++;
			} while (--l);
		}
		while (p < &value[8])
		{
			*p++ = 0;
		}

		/* format value as number */

		if (dmp_descending || (page->pag_flags & btr_descending))
			complement_key(value, node->btn_prefix + node->btn_length);

		double n;
		if ((node->btn_prefix == 0 && node->btn_length == 0) ||
			node->btn_prefix + node->btn_length > 8)
		{
			n = 0;
		}
		else
		{
			n = decompress((SCHAR *) value);
		}

		/* format value as string for printing */

		p = print;
		q = value;
		if (l = node->btn_prefix)
		{
			do {
				const UCHAR c = *q++;
				*p++ = (c >= ' ' && c <= '~') ? c : '.';
			} while (--l);
		}
		*p++ = '|';
		if (l = node->btn_length)
		{
			do {
				const UCHAR c = *q++;
				*p++ = (c >= ' ' && c <= '~') ? c : '.';
			} while (--l);
		}
		*p = 0;

		/* print formatted node */

		fprintf(dbg_file, "\t+%x Prefix: %d, length: %d, ",
				   (SCHAR *) node - (SCHAR *) page, node->btn_prefix,
				   node->btn_length);
		if (page->btr_level)
			fprintf(dbg_file, "page number: %ld", number);
		else
			fprintf(dbg_file, "number: %ld", number);
		if (page_size && !page->btr_level) {
			const int line = number % max_records;
			const int slot = (number / max_records) % dp_per_pp;
			const int pp = (number / max_records) / dp_per_pp;
			fprintf(dbg_file, " (pp=%d,slot=%d,line=%d)", pp, slot, line);
		}
		fprintf(dbg_file, ",\t(%s) [%g]\n", print, n);

		if (dmp_descending || (page->pag_flags & btr_descending))
			complement_key(value, node->btn_prefix + node->btn_length);

		node = NEXT_NODE(node);
	}
}


static void dmp_pip(const page_inv_page* page, ULONG sequence)
{
/**************************************
 *
 *	d m p _ p i p
 *
 **************************************
 *
 * Functional description
 *	Print a page inventory page.
 *
 **************************************/
	Database* dbb = GET_DBB();

	PageControl* control = dbb->dbb_pcontrol;
	fprintf(dbg_file,
			   "PAGE INVENTORY PAGE\t checksum %d\t generation %ld\n\tMin page: %ld\n\tFree pages:\n\t",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   page->pip_min);

#define BIT(n) (page->pip_bits [n >> 3] & (1 << (n & 7)))

	for (int n = 0; n < control->pgc_ppp;) {
		while (n < control->pgc_ppp)
		{
			if (BIT(n))
				break;
			n++;
		}
		fprintf(dbg_file, "%d - ", n);
		while (n < control->pgc_ppp)
		{
			if (!BIT(n))
				break;
			n++;
		}
		fprintf(dbg_file, "%d, ", n - 1);
	}

	fprintf(dbg_file, "\n");
}


static void dmp_pointer(const pointer_page* page)
{
/**************************************
 *
 *	d m p _ p o i n t e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	Database* dbb = GET_DBB();

	fprintf(dbg_file,
			   "POINTER PAGE\t checksum %d\t generation %ld\n\tRelation: %d, Flags: %x, Sequence: %ld, Next: %ld, Count: %d\n",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   page->ppg_relation, ((PAG) page)->pag_flags,
			   page->ppg_sequence, page->ppg_next, page->ppg_count);
	fprintf(dbg_file, "\tMin space slot: %d, Max space slot: %d\n",
			   page->ppg_min_space, page->ppg_max_space);

	const UCHAR* bytes = (UCHAR *) & page->ppg_page[dbb->dbb_dp_per_pp];

	for (USHORT i = 0; i < page->ppg_count; i++) {
		if (i % 20 == 0)
			/* fprintf (dbg_file, "\n\t%d: ", bytes [i / 4]); */
			fprintf(dbg_file, "\n\t");
		fprintf(dbg_file, "%ld ", page->ppg_page[i]);
	}

	fprintf(dbg_file, "\n");
}


static void dmp_root(const index_root_page* page)
{
/**************************************
 *
 *	d m p _ r o o t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	fprintf(dbg_file,
			   "INDEX ROOT PAGE\t checksum %d\t generation %ld\n\tRelation: %d, Count: %d\n",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation,
			   page->irt_relation, page->irt_count);
	const bool ods11plus =
		(JRD_get_thread_data()->getDatabase()->dbb_ods_version >= ODS_VERSION11);
	USHORT i = 0;
	for (const index_root_page::irt_repeat* desc = page->irt_rpt;
		i < page->irt_count; i++, desc++)
	{
		fprintf(dbg_file,
				   "\t%d -- root: %ld, number of keys: %d, flags: %x\n", i,
				   desc->irt_root, desc->irt_keys, desc->irt_flags);
		fprintf(dbg_file, "\t     keys (field, type): ");
		const SCHAR* ptr = (SCHAR *) page + desc->irt_desc;
		for (USHORT j = 0; j < desc->irt_keys; j++) {
			if (ods11plus) then
				ptr += sizeof(irtd);
			else
				ptr += sizeof(irtd_ods10);
			const irtd* stuff = (irtd*)ptr;
			fprintf(dbg_file, "(%d, %d),", stuff->irtd_field,
					   stuff->irtd_itype);
		}
		fprintf(dbg_file, "\n");
	}
}


static void dmp_transactions(const tx_inv_page* page, ULONG sequence)
{
/**************************************
 *
 *	d m p _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	Database* dbb = tdbb->getDatabase();

	const ULONG transactions_per_tip = dbb->dbb_pcontrol->pgc_tpt;

	fprintf(dbg_file,
			   "Transaction Inventory Page\t checksum %d\t generation %ld\n",
			   ((PAG) page)->pag_checksum, ((PAG) page)->pag_generation);
	if (tdbb->getTransaction())
	{
		fprintf(dbg_file, "\tCurrent transaction %d",
				   tdbb->getTransaction()->tra_number);
	}
	else
		fprintf(dbg_file, "\tCurrent transaction (NULL)");
	if (sequence)
		fprintf(dbg_file, "\t first transaction on page %ld",
				   transactions_per_tip * (sequence - 1));
	else
		fprintf(dbg_file, "\t Transactions per TIP %ld",
				   transactions_per_tip);
	fprintf(dbg_file, "\tnext TIP page %d\n\n", page->tip_next);
	fprintf(dbg_file,
			   "\t          1         2         3         4         5         6         7         8         9\n");
	fprintf(dbg_file,
			   "\t0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n\n");

	UCHAR s[101];
	const UCHAR* const end = s + sizeof(s) - 1;
	UCHAR* p = s;

	ULONG number = 0;
	for (USHORT hundreds = 0; number < transactions_per_tip; number++)
	{
		const ULONG trans_offset = TRANS_OFFSET(number);
		const UCHAR* byte = page->tip_transactions + trans_offset;
		const USHORT shift = TRANS_SHIFT(number);
		const USHORT state = (*byte >> shift) & TRA_MASK;
		*p++ = (state == tra_active) ? 'A' :
			(state == tra_limbo) ? 'L' : (state == tra_dead) ? 'D' : 'C';
		if (p >= end) {
			*p = 0;
			fprintf(dbg_file, "  %3d\t%s\n", hundreds++, s);
			p = s;
		}
	}
	while (p < end)
		*p++ = ' ';
	*p = 0;
	fprintf(dbg_file, "  %3d\t%s\n", hundreds, s);
}


