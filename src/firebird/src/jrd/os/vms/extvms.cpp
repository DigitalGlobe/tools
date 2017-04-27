/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		extvms.cpp
 *	DESCRIPTION:	External file access
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
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "../jrd/rse.h"
#include "../jrd/ext.h"
#include "gen/iberror.h"
#include "../jrd/tra.h"
#include "../jrd/ods.h"
#include "../jrd/btr.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/ext_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/opt_proto.h"
#include "../jrd/vio_proto.h"
#include "../common/utils_proto.h"

#include rms

const char* DEFAULT_FILE	= ".dat";
const int MAX_KEYS	= 20;

static struct FAB fab;
static struct RAB rab;

static bool check_sort(const jrd_nod*, const index_desc*, USHORT);
static bool compare(const UCHAR*, const UCHAR*, USHORT);
static int compare_segment(jrd_nod*, UCHAR *, DSC *);
static int connect(ExternalFile*, USHORT);
static void disconnect(ExternalFile*);
static void expand_format(Format*, const Format*);
static SLONG find_field(const Format*, USHORT, USHORT, USHORT);
static bool get_dbkey(RecordSource*);
static bool get_indexed(RecordSource*);
static USHORT get_key_segment(jrd_nod*, UCHAR *, DSC *);
static bool get_sequential(RecordSource*);
static bool match_index(const Format*, struct XABKEY *, index_desc*);
static int open_indexed(RecordSource*);
static int open_sequential(RecordSource*);
static void position_by_rfa(ExternalFile*, USHORT *);
static void set_flags(jrd_rel*, Record*);
static void set_missing(jrd_rel*, Record*);


void EXT_close(RecordSource* rsb)
{
/**************************************
 *
 *	E X T _ c l o s e
 *
 **************************************
 *
 * Functional description
 *	Close a record stream for an external file.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	jrd_rel* relation = rsb->rsb_relation;
	ExternalFile* file = relation->rel_file;
	jrd_req* request = tdbb->getRequest();
	record_param* rpb = &request->req_rpb[rsb->rsb_stream];

	if ((rab.rab$w_isi = rpb->rpb_ext_isi) && rpb->rpb_ext_isi != file->ext_isi)
	{
		disconnect(file);
		rpb->rpb_ext_isi = 0;
	}
}


void EXT_erase(record_param* rpb, jrd_tra *transaction)
{
/**************************************
 *
 *	E X T _ e r a s e
 *
 **************************************
 *
 * Functional description
 *	Update an external file.
 *
 **************************************/
	if (rpb->rpb_stream_flags & RPB_s_refetch)
		IBERROR(180);			/* msg 180 can't reposition for update after sort for RMS */

	jrd_rel* relation = rpb->rpb_relation;
	ExternalFile* file = relation->rel_file;
	rab.rab$w_isi = rpb->rpb_ext_isi;
	rab.rab$l_rop = 0;

	position_by_rfa(file, &rpb->rpb_ext_dbkey);
	const int status = sys$delete(&rab);

	if (!(status & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$delete",
				 isc_arg_string, file->ext_filename,
				 isc_arg_gds, isc_io_delete_err, isc_arg_vms, status, 0);
}


ExternalFile* EXT_file(jrd_rel* relation, const TEXT* file_name); //, bid* description)
{
/**************************************
 *
 *	E X T _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Create a file block for external file access.
 *
 **************************************/
	UCHAR index_buffer[MAX_KEYS * sizeof(index_desc)];

	thread_db* tdbb = JRD_get_thread_data();
	Database* dbb = tdbb->getDatabase();

/* Allocate and fill out an external file block.  Get the
   external format and flesh it out using the internal
   format. */

	const USHORT l = strlen(file_name);
	ExternalFile* file = FB_NEW_RPT(dbb->dbb_permanent, l) ExternalFile();
	relation->rel_file = file;
	strcpy(file->ext_filename, file_name);
	Format* format = file->ext_format = MET_format(tdbb, relation, 0);
	expand_format(format, MET_current(tdbb, relation));

/* Groan -- time to check in with RMS.  We're going to start
   by formatting and openning a FAB (file access block).  Rather
   than cart the full FAB around, just save the "ifi" (internal
   FAB identifier) for later use.  To avoid an extra DISPLAY
   operation, allocate and link in a summary XAB (extended
   attribute block) to determine the number of indices for an
   indexed file. */

	fab = cc$rms_fab;
	fab.fab$l_dna = DEFAULT_FILE;
	fab.fab$b_dns = sizeof(DEFAULT_FILE) - 1;
	fab.fab$l_fna = file->ext_filename;
	fab.fab$b_fns = l;

/* this was put in for Healthdyne;   if the logical name is defined and
   if it says "READONLY", then we open the file read only.  if the
   logical name is defined and it is not READONLY, then let there be
   an error. */

	Firebird::string lognam;
	if (fb_utils::readenv("GDS_RMSACCESS", lognam))
	{
		if (lognam == "READONLY")
			fab.fab$b_fac = FAB$M_GET;
	}
	else
		fab.fab$b_fac = FAB$M_DEL | FAB$M_GET | FAB$M_PUT | FAB$M_UPD;

	struct XABSUM summary;
	fab.fab$b_shr = FAB$M_MSE | FAB$M_SHRGET | FAB$M_SHRPUT | FAB$M_SHRDEL | FAB$M_SHRUPD;
	fab.fab$l_xab = &summary;
	summary = cc$rms_xabsum;

	int status = sys$open(&fab);
	fab.fab$l_xab = NULL;

	if (!(status & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$open",
				 isc_arg_string, file->ext_filename,
				 isc_arg_gds, isc_io_open_err, isc_arg_vms, status, 0);

/* If this is an indexed file, process keys.  For each known key,
   allocate a format a temporary key definition XAB.  Link the
   whole herd of XABs to the FAB and do an RMS DISPLAY operation
   to get key definitions.  For each key, try to match it against
   the external format by datatype, offset, and length.  If we can't
   match the key, ignore it */

	struct XABKEY keys[MAX_KEYS];

	if (fab.fab$b_org == FAB$C_IDX)
	{
		struct XABKEY *key, *end;
		struct XABKEY** ptr = (struct XABKEY**) &fab.fab$l_xab;
		for (key = keys, end = key + summary.xab$b_nok; key < end; key++) {
			*key = cc$rms_xabkey;
			key->xab$b_ref = key - keys;
			*ptr = key;
			ptr = &key->xab$l_nxt;
		}
		status = sys$display(&fab);
		fab.fab$l_xab = NULL;
		if (!(status & 1))
			ERR_post(isc_io_error,
					 isc_arg_string, "sys$display",
					 isc_arg_string, file->ext_filename,
					 isc_arg_gds, isc_io_access_err, isc_arg_vms, status, 0);
		index_desc* index = (index_desc*) index_buffer;
		for (key = keys; key < end; key++)
			if (match_index(format, key, index)) {
				++file->ext_index_count;
				index = (index_desc*) (index->idx_rpt + index->idx_count);
			}
		const ptrdiff_t l2 = (UCHAR *) index - index_buffer;
		if (l2) {
			str* string = FB_NEW_RPT(tdbb->getDefaultPool(), l2) str();
			memcpy(string->str_data, index_buffer, l2);
			file->ext_indices = string->str_data;
		}
	}

/* Set up an internal stream for the file.  Since RMS supports
   multiple streams for ISAM files, each indexed stream will
   allocate separate connections for their stream. */

	rab = cc$rms_rab;
	rab.rab$l_fab = &fab;
	file->ext_isi = connect(file, 0);
	file->ext_ifi = fab.fab$w_ifi;
	file->ext_file_type = fab.fab$b_org;

	return file;
}


void EXT_fini(jrd_rel* relation)
{
/**************************************
 *
 *	E X T _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Close the file associated with a relation.
 *
 **************************************/
	ExternalFile* file = relation->rel_file;

	if (fab.fab$w_ifi = file->ext_ifi) {
		const int status = sys$close(&fab);
		if (!(status & 1))
			ERR_post(isc_io_error,
					 isc_arg_string, "sys$close",
					 isc_arg_string, file->ext_filename,
					 isc_arg_gds, isc_io_close_err, isc_arg_vms, status, 0);
	}

	file->ext_ifi = 0;
}


bool EXT_get(RecordSource* rsb)
{
/**************************************
 *
 *	E X T _ g e t
 *
 **************************************
 *
 * Functional description
 *	Get a record from an external file.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	if (tdbb->getRequest()->req_flags & req_abort)
		return false;

	switch (rsb->rsb_type)
	{
		case rsb_ext_sequential:
			return get_sequential(rsb);

		case rsb_ext_indexed:
			return get_indexed(rsb);

		case rsb_ext_dbkey:
			return get_dbkey(rsb);

		default:
			IBERROR(181);			/* msg 181 external access type not implemented */
	}
}


void EXT_modify(record_param* old_rpb, record_param* new_rpb, jrd_tra *transaction)
{
/**************************************
 *
 *	E X T _ m o d i f y
 *
 **************************************
 *
 * Functional description
 *	Update an external file.
 *
 **************************************/
	if (old_rpb->rpb_stream_flags & RPB_s_refetch)
		IBERROR(180);			/* msg 180 cannot reposition for update after sort for RMS */

	Record* record = new_rpb->rpb_record;
	jrd_rel* relation = new_rpb->rpb_relation;
	ExternalFile* file = relation->rel_file;
	const Format* format = record->rec_format;
	set_missing(old_rpb->rpb_relation, record);
	const int offset = FLAG_BYTES(format->fmt_count);

	rab.rab$w_isi = old_rpb->rpb_ext_isi;
	rab.rab$l_rop = 0;

	position_by_rfa(file, &old_rpb->rpb_ext_dbkey);
	rab.rab$l_rbf = record->rec_data + offset;
	rab.rab$w_rsz = record->rec_length - offset;

	const int status = sys$update(&rab);
	if (!(status & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$update",
				 isc_arg_string, file->ext_filename,
				 isc_arg_gds, isc_io_write_err, isc_arg_vms, status, 0);
}


EXT_open(RecordSource* rsb)
{
/**************************************
 *
 *	E X T _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a record stream for an external file.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	jrd_rel* relation = rsb->rsb_relation;
	jrd_req* request = tdbb->getRequest();
	record_param* rpb = &request->req_rpb[rsb->rsb_stream];

	const Format* format;
	Record* record = rpb->rpb_record;
	if (!record || !(format = record->rec_format)) {
		format = MET_current(tdbb, relation);
		record = VIO_record(tdbb, rpb, format, request->req_pool);
	}

	switch (rsb->rsb_type)
	{
	case rsb_ext_sequential:
		return open_sequential(rsb);

	case rsb_ext_indexed:
		return open_indexed(rsb);

	case rsb_ext_dbkey:
		{
			const ExternalFile* file = relation->rel_file;
			rpb->rpb_ext_isi = file->ext_isi;
			return;
		}

	default:
		IBERROR(181);			/* msg 181 external access type not implemented */
	}
}


RecordSource* EXT_optimize(OptimizerBlk* opt, SSHORT stream, jrd_nod** sort_ptr)
{
/**************************************
 *
 *	E X T _ o p t i m i z e
 *
 **************************************
 *
 * Functional description
 *	Compile and optimize a record selection expression into a
 *	set of record source blocks (rsb's).
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

/* Start by chasing around finding pointers to the various
   data structures */

	CompilerScratch* csb = opt->opt_csb;
	CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[stream];
	jrd_rel* relation = csb_tail->csb_relation;
	ExternalFile* file = relation->rel_file;
	jrd_nod* dbkey = NULL;
	jrd_nod* inversion = NULL;

/* Check for a dbkey retrieval.  If we find it, ignore everything
   else */

	OptimizerBlk::opt_repeat* tail = opt->opt_rpt;
	for (const OptimizerBlk::opt_repeat* const opt_end = tail + opt->opt_count;
		 tail < opt_end; tail++)
	{
		if (!(tail->opt_flags & opt_used) &&
			(dbkey = OPT_make_dbkey(opt, tail->opt_conjunct, stream)))
			break;
	}

/* If there are booleans availables and indices to optimize
   against, go at it */

	index_desc* idx;
	if (!dbkey && opt->opt_count && (idx = file->ext_indices))
	{
		for (SSHORT i = 0; i < file->ext_index_count; i++) {
			if (OPT_match_index(opt, stream, idx) && opt->opt_rpt[0].opt_lower)
			{
				inversion = OPT_make_index(tdbb, opt, relation, idx);
				if (check_sort(*sort_ptr, idx, stream))
					*sort_ptr = NULL;
				break;
			}
			idx = idx->idx_rpt + idx->idx_count;
		}
	}

/* Depending on whether or not an index was selected build either
   and external indexed or an external sequential record stream
   block */

	RecordSource* rsb;
	SSHORT size; // better size_t
	if (dbkey) {
		rsb = FB_NEW_RPT(tdbb->getDefaultPool(), 1) RecordSource();
		rsb->rsb_type = rsb_ext_dbkey;
		rsb->rsb_count = 1;
		size = sizeof(struct irsb_index);
		rsb->rsb_arg[0] = (RecordSource*) dbkey;
	}
	else if (inversion) {
		rsb = FB_NEW_RPT(tdbb->getDefaultPool(), 1) RecordSource();
		rsb->rsb_type = rsb_ext_indexed;
		rsb->rsb_count = 1;
		size = sizeof(struct irsb_index);
		rsb->rsb_arg[0] = (RecordSource*) inversion;
	}
	else {
		rsb = FB_NEW(tdbb->getDefaultPool()) RecordSource();
		rsb->rsb_type = rsb_ext_sequential;
		size = sizeof(struct irsb);
	}

/* Finish filling out the record stream block and allocate some
   space in the impure area of the request for stream state information */

	rsb->rsb_stream = stream;
	rsb->rsb_relation = relation;
	rsb->rsb_impure = CMP_impure(csb, size);

	return rsb;
}


void EXT_ready(jrd_rel* relation)
{
/**************************************
 *
 *	E X T _ r e a d y
 *
 **************************************
 *
 * Functional description
 *	Open an external file.
 *
 **************************************/
}


void EXT_store(Jrd::thread_db*, record_param* rpb)
{
/**************************************
 *
 *	E X T _ s t o r e
 *
 **************************************
 *
 * Functional description
 *	Update an external  *
 **************************************/
	jrd_rel* relation = rpb->rpb_relation;
	ExternalFile* file = relation->rel_file;
	Record* record = rpb->rpb_record;
	const Format* format = record->rec_format;
	set_missing(relation, record);
	const USHORT offset = FLAG_BYTES(format->fmt_count);

	rab.rab$w_isi = file->ext_isi;
	rab.rab$l_rbf = record->rec_data + offset;
	rab.rab$w_rsz = record->rec_length - offset;
	rab.rab$l_rop = 0;

	if (file->ext_file_type == FAB$C_IDX)
		rab.rab$b_rac = RAB$C_KEY;
	else if (!(file->ext_flags & EXT_eof))
	{
		disconnect(file);
		rab.rab$l_rop |= RAB$M_EOF;
		file->ext_isi = connect(file, 0);
		rab.rab$b_rac = RAB$C_SEQ;
		file->ext_flags |= EXT_eof;
	}

	const int status = sys$put(&rab);
	if (!(status & 1))
	{
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$put",
				 isc_arg_string, file->ext_filename,
				 isc_arg_gds, isc_io_write_err, isc_arg_vms, status, 0);
	}
}


void EXT_trans_commit(jrd_tra* transaction)
{
/**************************************
 *
 *	E X T _ t r a n s _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Checkin at transaction commit time.
 *
 **************************************/
}


void EXT_trans_prepare(jrd_tra* transaction)
{
/**************************************
 *
 *	E X T _ t r a n s _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Checkin at transaction prepare time.
 *
 **************************************/
}


void EXT_trans_rollback(jrd_tra* transaction)
{
/**************************************
 *
 *	E X T _ t r a n s _ r o l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Checkin at transaction rollback time.
 *
 **************************************/
}


void EXT_trans_start(jrd_tra* transaction)
{
/**************************************
 *
 *	E X T _ t r a n s _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Checkin at start transaction time.
 *
 **************************************/
}


static bool check_sort(const jrd_nod* sort, const index_desc* index, USHORT stream)
{
/**************************************
 *
 *	c h e c k _ s o r t
 *
 **************************************
 *
 * Functional description
 *	Check a sort node against an index to see if the
 *	sort is totally redundant.
 *
 **************************************/

/* If there is no sort, or there are more keys than
   index segments, we obviously can optimize anything */

	if (!sort || sort->nod_count > index->idx_count)
		return false;

/* For each sort key, make sure the key matches the index segment,
   and that the sort is ascending */

	const index_desc::idx_repeat* tail = index->idx_rpt;
	const jrd_nod* const* ptr = sort->nod_arg;
	for (const jrd_nod* const* const end = ptr + sort->nod_count; ptr < end; ptr++, tail++)
	{
		const jrd_nod* field = *ptr;
		if (field->nod_type != nod_field ||
			(USHORT) field->nod_arg[e_fld_stream] != stream ||
			(USHORT) field->nod_arg[e_fld_id] != tail->idx_field ||
			ptr[sort->nod_count])
		{
			return false;
		}
	}

	return true;
}


static bool compare(const UCHAR* string1, const UCHAR* string2, USHORT length)
{
/**************************************
 *
 *	c o m p a r e
 *
 **************************************
 *
 * Functional description
 *	Compare two strings for equality. Return true if they are different.
 *
 **************************************/
	return memcmp(string1, string2, length) != 0;
}


static int compare_segment(jrd_nod* node, UCHAR * buffer, DSC * target)
{
/**************************************
 *
 *	c o m p a r e _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Compare field in record against upper bound.  Return
 *	comparison.
 *
 **************************************/
	const dsc* source = EVL_expr(tdbb, node);
	dsc desc = *target;
	desc.dsc_address = (ULONG) desc.dsc_address + buffer;

	return MOV_compare(&desc, source);
}


static int connect(ExternalFile* file, USHORT key)
{
/**************************************
 *
 *	c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Connect RAB and report any errors.
 *
 **************************************/
	rab.rab$w_isi = 0;
	rab.rab$b_krf = key;
	const int status = sys$connect(&rab);

	if (!(status & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$connect",
				 isc_arg_string, file->ext_filename,
				 isc_arg_gds, isc_io_access_err, isc_arg_vms, status, 0);

	return rab.rab$w_isi;
}


static void disconnect(ExternalFile* file)
{
/**************************************
 *
 *	d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Disconnect rab and check status.
 *
 **************************************/
	const int status = sys$disconnect(&rab);

	if (!(status & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$disconnect",
				 isc_arg_string, file->ext_filename,
				 isc_arg_gds, isc_io_close_err, isc_arg_vms, status, 0);
}


static void expand_format(Format* external, const Format* internal)
{
/**************************************
 *
 *	e x p a n d _ f o r m a t
 *
 **************************************
 *
 * Functional description
 *	Merge internal and external formats -- if external info
 *	is missing, default from internal.
 *
 *	Note: this doesn't yet handle datatypes supported in external
 *	files but not internally.
 *
 **************************************/

/* For now, more or less ignore external type declarations */

	SLONG offset = 0;

	const dsc* idesc = internal->fmt_desc;
	dsc* edesc = external->fmt_desc;
	for (const dsc* const end = edesc + external->fmt_count; edesc < end; edesc++, idesc++)
	{
		*edesc = *idesc;
		edesc->dsc_address = (UCHAR *) offset;
		offset += edesc->dsc_length;
	}

	external->fmt_length = offset;
}


static SLONG find_field(const Format* format, USHORT type, USHORT offset, USHORT length)
{
/**************************************
 *
 *	f i n d _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Given a format block and the offset and length of a
 *	field, find the field id.  If we can't identify the
 *	field, return -1.
 *
 **************************************/
	USHORT n = 0;
	const dsc* desc = format->fmt_desc;
	for (const dsc* const end = desc + format->fmt_count; desc < end; desc++, n++)
		if ((int) desc->dsc_address == offset && desc->dsc_length == length)
			switch (type)
			{
			case XAB$C_IN2:
				if (desc->dsc_dtype == dtype_short)
					return n;
				break;

			case XAB$C_IN4:
				if (desc->dsc_dtype == dtype_long)
					return n;
				break;

			case XAB$C_IN8:
				if (desc->dsc_dtype == dtype_timestamp || desc->dsc_dtype == dtype_quad)
				{
					return n;
				}
				break;

			case XAB$C_BN8:
				if (desc->dsc_dtype == dtype_timestamp)
					return n;
				break;

			case XAB$C_STG:
				if (desc->dsc_dtype == dtype_text)
					return n;
				break;
			}

	return -1;
}


static bool get_dbkey(RecordSource* rsb)
{
/**************************************
 *
 *	g e t _ d b k e y
 *
 **************************************
 *
 * Functional description
 *	Get a record from an external file.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

/* Chase down misc pointers */

	jrd_rel* relation = rsb->rsb_relation;
	ExternalFile* file = relation->rel_file;
	jrd_req* request = tdbb->getRequest();

	irsb_ext* impure = (irsb_ext*) ((UCHAR *) request + rsb->rsb_impure);
	record_param* rpb = &request->req_rpb[rsb->rsb_stream];
	Record* record = rpb->rpb_record;
	const Format* format = record->rec_format;

/* If this isn't the first time thru its the last time thru */

	if (!(impure->irsb_flags & irsb_first))
		return false;

/* Evaluate expression */

	jrd_nod* node = (jrd_nod*) rsb->rsb_arg[0];
	const dsc* desc = EVL_expr(tdbb, node->nod_arg[0]);

	const SSHORT offset = FLAG_BYTES(format->fmt_count);
	rab.rab$w_isi = rpb->rpb_ext_isi;
	rab.rab$l_ubf = record->rec_data + offset;
	rab.rab$w_usz = record->rec_length - offset;

/* If the rab is shared and has been moved, reposition it */

	position_by_rfa(file, desc->dsc_address);
	impure->irsb_flags &= ~irsb_first;
	file->ext_flags &= ~EXT_eof;

	int status;    /// Uhhh???? No place above sets status. Some sys$ call is missing.
	if (status == RMS$_EOF)
		return false;

	sys$free(&rab);
	set_flags(relation, record);
	memcpy(&rpb->rpb_ext_dbkey, rab.rab$w_rfa, sizeof(rab.rab$w_rfa));
	memcpy(file->ext_dbkey, rab.rab$w_rfa, sizeof(rab.rab$w_rfa));

	return true;
}


static bool get_indexed(RecordSource* rsb)
{
/**************************************
 *
 *	g e t _ i n d e x e d
 *
 **************************************
 *
 * Functional description
 *	Get a record from an external file.
 *
 **************************************/
	UCHAR key_buffer[256];

	thread_db* tdbb = JRD_get_thread_data();

/* Start by finding the signficant data structures for the stream.  These
   are mainly used to initialize the stream at some particular key value */

	jrd_rel* relation = rsb->rsb_relation;
	ExternalFile* file = relation->rel_file;
	const Format* format = file->ext_format;

	jrd_req* request = tdbb->getRequest();
	record_param* rpb = &request->req_rpb[rsb->rsb_stream];
	irsb_ext* impure = (irsb_ext*) ((UCHAR *) request + rsb->rsb_impure);

	jrd_nod* node = rsb->rsb_arg[0];
	IndexRetrieval* retrieval = (IndexRetrieval*) node->nod_arg[e_idx_retrieval];
	index_desc* index = &retrieval->irb_desc;

/* If this is the first time through on this stream, and a lower
   value is present, position the RMS stream with a keyed access.
   In theory, this could have been done in "open_indexed", but
   it is a little more convenient to do it here */

	if (impure->irsb_flags & irsb_first)
		if (retrieval->irb_lower_count)
		{
			UCHAR* p = key_buffer;
			index_desc::idx_repeat* segment = index->idx_rpt;
			jrd_nod** ptr = retrieval->irb_value;
			for (const jrd_nod* const* const end = ptr + retrieval->irb_lower_count;
				ptr < end; ptr++, segment++)
			{
				p += get_key_segment(*ptr, p, format->fmt_desc + segment->idx_field);
			}
			rab.rab$b_krf = index->idx_id;
			rab.rab$l_kbf = key_buffer;
			rab.rab$b_ksz = p - key_buffer;
			rab.rab$b_rac = RAB$C_KEY;
			rab.rab$l_rop = (retrieval->irb_generic & irb_equality) ? 0 : RAB$M_KGE;
			const int status = sys$find(&rab);
			file->ext_flags &= ~EXT_eof;
			if (status == RMS$_EOF || status == RMS$_RNF)
				return false;
			if (!(status & 1))
				ERR_post(isc_io_error,
						 isc_arg_string, "sys$find",
						 isc_arg_string, file->ext_filename,
						 isc_arg_gds, isc_io_read_err,
						 isc_arg_vms, status, 0);
		}

/* Do an RMS sequential get on the stream.  If it comes back
   EOF, we're done */

	if (!get_sequential(rsb))
		return false;

/* Check record against upper bound.  If we pass it, we're done.
   Note: this code ignores issues of inclusive/exclusive upper
   bound.  Since the full boolean will be applied anyway, the only
   danger is processing a few extraneous records. */

	index_desc::idx_repeat* segment = index->idx_rpt;

	jrd_nod** ptr = retrieval->irb_value + index->idx_count;
	for (const jrd_nod* const* const end = ptr + retrieval->irb_upper_count;
		ptr < end; ptr++, segment++)
	{
		const int result = compare_segment(*ptr, rab.rab$l_ubf, format->fmt_desc + segment->idx_field);
		if (result < 0)
			break;
		if (result > 0)
			return false;
	}

	return true;
}


static USHORT get_key_segment(jrd_nod* node, UCHAR * buffer, DSC * target)
{
/**************************************
 *
 *	g e t _ k e y _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Given a data descriptor, key buffer and description,
 *	build an RMS key segment.  This is basically trivial
 *	as SLONG as the RMS data type matches the internal data
 *	type.
 *
 **************************************/
	const dsc* source = EVL_expr(tdbb, node);
	dsc desc = *target;
	desc.dsc_address = buffer;
	MOV_move(source, &desc);

	return desc.dsc_length;
}


static bool get_sequential(RecordSource* rsb)
{
/**************************************
 *
 *	g e t _ s e q u e n t i a l
 *
 **************************************
 *
 * Functional description
 *	Get a record from an external file.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	jrd_rel* relation = rsb->rsb_relation;
	ExternalFile* file = relation->rel_file;
	jrd_req* request = tdbb->getRequest();

	irsb_ext* impure = (irsb_ext*) ((UCHAR *) request + rsb->rsb_impure);
	record_param* rpb = &request->req_rpb[rsb->rsb_stream];
	Record* record = rpb->rpb_record;
	const Format* format = record->rec_format;

	const SSHORT offset = FLAG_BYTES(format->fmt_count);
	rab.rab$w_isi = rpb->rpb_ext_isi;
	rab.rab$l_ubf = record->rec_data + offset;
	rab.rab$w_usz = record->rec_length - offset;

/* If the rab is shared and has been moved, reposition it */

	if (rpb->rpb_ext_isi == file->ext_isi && !(impure->irsb_flags & irsb_first) &&
		compare(file->ext_dbkey, &rpb->rpb_ext_dbkey, sizeof(rab.rab$w_rfa)))
	{
		position_by_rfa(file, &rpb->rpb_ext_dbkey);
	}

	rab.rab$b_rac = RAB$C_SEQ;
	const int status = sys$get(&rab);
	impure->irsb_flags &= ~irsb_first;
	file->ext_flags &= ~EXT_eof;

	if (status == RMS$_EOF)
		return false;

	if (!(status & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$get",
				 isc_arg_string, file->ext_filename,
				 isc_arg_gds, isc_io_read_err, isc_arg_vms, status, 0);

	sys$free(&rab);
	set_flags(relation, record);
	memcpy(&rpb->rpb_ext_dbkey, rab.rab$w_rfa, sizeof(rab.rab$w_rfa));

/* If we're using the shared stream, save the current dbkey to
   determine whether repositioning will be required next time
   thru */

	if (rpb->rpb_ext_isi == file->ext_isi)
		memcpy(file->ext_dbkey, rab.rab$w_rfa, sizeof(rab.rab$w_rfa));

	return true;
}


static bool match_index(const Format* format, struct XABKEY *xab, index_desc* idx)
{
/**************************************
 *
 *	m a t c h _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Try to match RMS key against fields.  If success, build
 *	internal index description and return true.  Otherwise
 *	return false.
 *
 **************************************/
	const UCHAR* size = &xab->xab$b_siz;
	const UCHAR* const end = size + xab->xab$b_nsg;
	const USHORT* position = &xab->xab$w_pos;
	index_desc::idx_repeat* tail = idx->idx_rpt;

	for (; size < end; size++, position++, tail++) {
		const int n = find_field(format, xab->xab$b_dtp, *position, *size);
		if (n < 0)
			return false;
		tail->idx_field = n;
		tail->idx_itype = xab->xab$b_dtp;
	}

	idx->idx_count = xab->xab$b_nsg;
	idx->idx_selectivity = .01;
	idx->idx_flags = 0;
	idx->idx_id = xab->xab$b_ref;

	return true;
}


static open_indexed(RecordSource* rsb)
{
/**************************************
 *
 *	o p e n _ i n d e x e d
 *
 **************************************
 *
 * Functional description
 *	Open a record stream for an external file.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	jrd_rel* relation = rsb->rsb_relation;
	ExternalFile* file = relation->rel_file;
	jrd_req* request = tdbb->getRequest();
	record_param* rpb = &request->req_rpb[rsb->rsb_stream];
	jrd_nod* node = (jrd_nod*) rsb->rsb_arg[0];
	IndexRetrieval* retrieval = (IndexRetrieval*) node->nod_arg[e_idx_retrieval];
	index_desc* index = &retrieval->irb_desc;

/* Create internal RMS rab (irab) for stream.  Connect
   temporary shared rab, then save internal rab number */

	fab.fab$w_ifi = file->ext_ifi;
	rpb->rpb_ext_isi = connect(file, index->idx_id);
}


static open_sequential(RecordSource* rsb)
{
/**************************************
 *
 *	o p e n _ s e q u e n t i a l
 *
 **************************************
 *
 * Functional description
 *	Open a record stream for an external file.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	jrd_rel* relation = rsb->rsb_relation;
	ExternalFile* file = relation->rel_file;
	jrd_req* request = tdbb->getRequest();
	record_param* rpb = &request->req_rpb[rsb->rsb_stream];

	if (file->ext_file_type == FAB$C_IDX) {
		fab.fab$w_ifi = file->ext_ifi;
		rpb->rpb_ext_isi = connect(file, 0);
	}
	else {
		file->ext_flags &= ~EXT_eof;
		rpb->rpb_ext_isi = rab.rab$w_isi = file->ext_isi;
		const int status = sys$rewind(&rab);
		if (!(status & 1))
			ERR_post(isc_io_error,
					 isc_arg_string, "sys$rewind",
					 isc_arg_string, file->ext_filename,
					 isc_arg_gds, isc_io_access_err, isc_arg_vms, status, 0);
	}
}


static void position_by_rfa(ExternalFile* file, USHORT * rfa)
{
/**************************************
 *
 *	p o s i t i o n _ b y _ r f a
 *
 **************************************
 *
 * Functional description
 *	Position file to specific RFA.
 *
 **************************************/
	memcpy(rab.rab$w_rfa, rfa, sizeof(rab.rab$w_rfa));
	rab.rab$b_rac = RAB$C_RFA;
	file->ext_flags &= ~EXT_eof;
	const int status = sys$get(&rab);

	if (!(status & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$get by RFA",
				 isc_arg_string, file->ext_filename,
				 isc_arg_gds, isc_io_access_err, isc_arg_vms, status, 0);
}


static void set_flags(jrd_rel* relation, Record* record)
{
/**************************************
 *
 *	s e t _ f l a g s
 *
 **************************************
 *
 * Functional description
 *	Set missing flags for a record.
 *
 **************************************/
	DSC desc;

	vec<jrd_fld*>* vector = relation->rel_fields;
	if (!vector)
		return;

	jrd_fld** field_ptr = &vector[0];
	const Format* format = record->rec_format;
	dsc* desc_ptr = format->fmt_desc;

	for (USHORT i = 0; i < format->fmt_count; i++, field_ptr++, desc_ptr++) {
		SET_NULL(record, i);
		jrd_fld* field;
		if (!desc_ptr->dsc_length || !(field = *field_ptr))
			continue;
		Literal* literal = field->fld_missing_value;
		if (literal) {
			desc = *desc_ptr;
			desc.dsc_address = record->rec_data + (int) desc.dsc_address;
			if (!MOV_compare(&literal->lit_desc, &desc))
				continue;
		}
		CLEAR_NULL(record, i);
	}
}


static void set_missing(jrd_rel* relation, Record* record)
{
/**************************************
 *
 *	s e t _ m i s s i n g
 *
 **************************************
 *
 * Functional description
 *	Set missing values for MODIFY/STORE.
 *
 **************************************/
	DSC desc;

	vec<jrd_fld*>* vector = relation->rel_fields;
	if (!vector)
		return;

	jrd_fld** field_ptr = &vector[0];
	const Format* format = record->rec_format;
	dsc* desc_ptr = format->fmt_desc;

	for (USHORT i = 0; i < format->fmt_count; i++, field_ptr++, desc_ptr++)
	{
		USHORT l;
		jrd_fld* field = *field_ptr;
		if ((field) && !field->fld_computation &&
			(l = desc_ptr->dsc_length) && TEST_NULL(record, i))
		{
			UCHAR* p = record->rec_data + (int) desc_ptr->dsc_address;
			Literal* literal = field->fld_missing_value;
			if (literal) {
				desc = *desc_ptr;
				desc.dsc_address = p;
				MOV_move(&literal->lit_desc, &desc);
			}
			else {
				const UCHAR pad = (desc_ptr->dsc_dtype == dtype_text) ? ' ' : 0;
				memset(p, pad, l);
			}
		}
	}
}

