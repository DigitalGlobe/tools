/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dbg.cpp
 *	DESCRIPTION:	Debugging routines
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
#include <string.h>
#include <signal.h>

#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "../jrd/ods.h"
#include "../jrd/cch.h"
#include "../jrd/dbg.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"
#include "../jrd/rse.h"
#include "../jrd/btr.h"
#include "../jrd/sort.h"
#include "../jrd/que.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dbg_proto.h"
#include "../jrd/err_proto.h"

#ifdef SUPERSERVER
#include "../jrd/err_proto.h"
#endif


using namespace Jrd;

/* Given pointer a field in the block, find the block */
#define BLOCK(fld_ptr, type, fld) (type)((SCHAR*) fld_ptr - OFFSET (type, fld))


#ifndef DEBUG
int debug;
#endif

extern int *dbt_blocks[], dbt_window[], dbt_record_param[];
extern SLONG gds_delta_alloc, gds_max_alloc;

typedef int (*DBG_PFN_V) ();
typedef int (*DBG_PFN_I) (int);

int (*dbg_all) () = DBG_all;
int (*dbg_analyze) (int) = DBG_analyze;
int (*dbg_block) (blk *) = DBG_block;
int (*dbg_eval) (int) = DBG_eval;
int (*dbg_open) () = DBG_open;
int (*dbg_close) () = DBG_close;
int (*dbg_pool) (MemoryPool*) = DBG_pool;
int (*dbg_pretty) (const jrd_nod*, int) = DBG_pretty;
int (*dbg_window) (int *) = DBG_window;
int (*dbg_rpb) (record_param*) = DBG_rpb;
static int (*dbg_bdbs) () = DBG_bdbs;
int (*dbg_examine) (int *) = DBG_examine;
int (*dbg_check) (int) = DBG_check;


#ifdef LINUX
FILE *dbg_file = NULL;
#else
FILE *dbg_file = stdout;
#endif
int DBG_supervisor(int);
int *prior_frame;
static SLONG perm_pool_mem;
static SLONG req_pool_mem;
static SLONG trans_pool_mem;
static SLONG other_pool_mem;

static void go_column(int);
static void prt_dsc(const DSC*, int);
static int prt_fields(const SCHAR*, const int*);
static int prt_que(const SCHAR*, const que*);
static int rsb_pretty(const RecordSource*, int);

/* Pick up node names */

#define NODE(type, name, keyword) "name",

static const TEXT* node_names[] = {
#include "../jrd/nod.h"
	0
};
#undef NODE

/* rsb types */

static const TEXT* rsb_names[] =
{
	"boolean",
	"cross",
	"dbkey",
	"first",
	"indexed",
	"merge",
	"multiple",
	"project",
	"sequential",
	"sort",
	"union",
	"aggregate",
	"ext_sequential",			/* External sequential access */
	"ext_indexed",				/* External indexed access */
	"ext_dbkey",
	"navigation",
	"bit_sieve",
	"left_cross",
	"procedure"
};


int DBG_all()
{
/**************************************
 *
 *	D B G _ a l l
 *
 **************************************
 *
 * Functional description
 *	Print all known blocks.
 *
 **************************************/
	Database* dbb = GET_DBB();

	if (!dbg_file) {
		dbg_file = fopen("tt:", "w");
	}
//	if (!dbb || !(vector = dbb->dbb_pools))
	if (!dbb || !dbb->dbb_pools.getCount())
	{
		return TRUE;
	}
	Database::pool_vec_type::iterator itr;
	Database::pool_vec_type::iterator end = dbb->dbb_pools.end();
	for (itr = dbb->dbb_pools.begin(); itr < end; ++itr) {
		DBG_pool(*itr);
	}
	return TRUE;
}


int DBG_analyze(int pool_id)
{
/**************************************
 *
 *	D B G _ a n a l y z e
 *
 **************************************
 *
 * Functional description
 *	Analyze pool by block type and sub-type.
 *
 **************************************/
	SLONG total_length = 0;

	struct {
		int sum_count;
		SLONG sum_length;
	} blocks[type_MAX], nodes[nod_MAX], *p, *end;

	Database* dbb = GET_DBB();

	if (!dbb || !dbb->dbb_pools.getCount())
		return TRUE;

	Database::pool_vec_type* vector = &dbb->dbb_pools;

	for (p = blocks, end = p + (int) type_MAX; p < end; p++) {
		p->sum_count = 0;
		p->sum_length = 0;
	}

	for (p = nodes, end = p + (int) nod_MAX; p < end; p++) {
		p->sum_count = 0;
		p->sum_length = 0;
	}

	Database::pool_ptr pool = (*vector)[pool_id];
	/*
	if (pool) {
		SLONG length = 0;
		for (hunk = pool->plb_hunks; hunk; hunk = hunk->hnk_next) {
			const char* hunk_end = ((char*)hunk->hnk_address) + hunk->hnk_length;
			for (const blk* block = (blk*) hunk->hnk_address; block != (const blk*) hunk_end;
				 block = (blk*) ((SCHAR *) block + length))
			{
				const SSHORT type = block->blk_type;
				length = block->blk_length << SHIFT;
				total_length += length;
				if (type <= (SSHORT) type_MIN || type >= (SSHORT) type_MAX) {
					fprintf(dbg_file, "***punt***\n");
					break;
				}
				p = blocks + type;
				p->sum_count++;
				p->sum_length += length;
				if (type == (SSHORT) type_nod) {
					p = nodes + (int) ((jrd_nod*) block)->nod_type;
					p->sum_count++;
					p->sum_length += length;
				}
			}
		}
	}


	int pool_type = 0;
	if (pool->plb_pool_id == 0) {
		pool_type = 1;
		perm_pool_mem = total_length / 1024;
		fprintf(dbg_file, "\nPool %d (Permanent pool)\n",
				   pool->plb_pool_id);
	}
	else {
		SSHORT type = 0;
		for (p = blocks, end = p + (int) type_MAX; p < end;
			 p++, type++)
		{
			if (p->sum_count)
				TEXT** fields = reinterpret_cast<char**>(dbt_blocks[type]);
			if (!strcmp(*fields, "TRANSACTION") && p->sum_count) {
				pool_type = 2;
				trans_pool_mem += (total_length / 1024);
				fprintf(dbg_file, "\nPool %d (Transaction pool)\n",
						   pool->plb_pool_id);
				break;
			}
			if (!strcmp(*fields, "REQUEST") && p->sum_count) {
				pool_type = 3;
				req_pool_mem += (total_length / 1024);
				fprintf(dbg_file, "\nPool %d (Request pool)\n",
						   pool->plb_pool_id);
				break;
			}
		}
	}
	if (!pool_type) {
		other_pool_mem += (total_length / 1024);
		fprintf(dbg_file, "\nPool %d\n", pool->plb_pool_id);
	}
	fprintf(dbg_file,
			   "Summary by block types: (total length of pool = %ldk)\n",
			   total_length / 1024);

	SSHORT type;
	for (p = blocks, end = p + (int) type_MAX, type = 0; p < end; p++, type++)
		if (p->sum_count) {
			int i;
			TEXT** fields = reinterpret_cast<char**>(dbt_blocks[type]);
			TEXT name_padded[MAX_SQL_IDENTIFIER_SIZE];
			for (i = 0; i < MAX_SQL_IDENTIFIER_LEN; name_padded[i++] = ' ');
			name_padded[i] = '\0';
			for (i = 0; (*fields)[i]; i++)
				name_padded[i] = (*fields)[i];
			fprintf(dbg_file, "\t%s\t%d\t%ld\n", name_padded, p->sum_count,
					   p->sum_length);
		}

	fprintf(dbg_file, "Summary by node types:\n");

	for (p = nodes, end = p + (int) nod_MAX, type = 0; p < end; p++, type++)
		if (p->sum_count) {
			int i;
			const TEXT* node_name = node_names[type];
			TEXT name_padded[MAX_SQL_IDENTIFIER_SIZE];
			for (i = 0; i < 31; name_padded[i++] = ' ');
			name_padded[i] = '\0';
			for (i = 0; node_name[i]; i++)
				name_padded[i] = node_name[i];
			fprintf(dbg_file, "\t%s\t%d\t%ld\n", name_padded, p->sum_count,
					   p->sum_length);
		}

	return pool_type;
	*/

	return 0;
}


int DBG_bdbs()
{
/**************************************
 *
 *	D B G _ b d b s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	Database* dbb = GET_DBB();

	BufferControl* bcb = dbb->dbb_bcb;
	for (unsigned int i = 0; i < bcb->bcb_count; i++)
		DBG_block(bcb->bcb_rpt[i].bcb_bdb);

	return TRUE;
}


int DBG_precedence()
{
/**************************************
 *
 *	D B G _ p r e c e d e n c e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	QUE que_inst;
	Precedence* precedence;
	BufferDesc* hi_bdb;
	BufferDesc* lo_bdb;

	Database* dbb = GET_DBB();

	BufferControl* bcb = dbb->dbb_bcb;
	for (unsigned int i = 0; i < bcb->bcb_count; i++) {
		const BufferDesc* bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (bdb->bdb_flags || bdb->bdb_ast_flags) {
			fprintf(dbg_file, "BufferDesc %d:\tpage %"SLONGFORMAT"", i, bdb->bdb_page);
			if (bdb->bdb_flags & BDB_dirty)
				fprintf(dbg_file, ", dirty");
			if (bdb->bdb_ast_flags & BDB_blocking)
				fprintf(dbg_file, ", blocking");
			if (bdb->bdb_flags & BDB_writer)
				fprintf(dbg_file, ", writer");
			if (bdb->bdb_flags & BDB_marked)
				fprintf(dbg_file, ", marked");
			if (bdb->bdb_flags & BDB_must_write)
				fprintf(dbg_file, ", must_write");
			if (bdb->bdb_flags & BDB_faked)
				fprintf(dbg_file, ", faked");
			if (bdb->bdb_flags & BDB_system_dirty)
				fprintf(dbg_file, ", system_dirty");
			if (bdb->bdb_flags & BDB_io_error)
				fprintf(dbg_file, ", io_error");
			if (bdb->bdb_flags & BDB_read_pending)
				fprintf(dbg_file, ", read_pending");
			if (bdb->bdb_flags & BDB_free_pending)
				fprintf(dbg_file, ", free_pending");
			if (bdb->bdb_flags & BDB_not_valid)
				fprintf(dbg_file, ", not_valid");
			if (bdb->bdb_flags & BDB_db_dirty)
				fprintf(dbg_file, ", db_dirty");
			if (bdb->bdb_flags & BDB_checkpoint)
				fprintf(dbg_file, ", checkpoint");
			fprintf(dbg_file, "\n");
			if (QUE_NOT_EMPTY(bdb->bdb_higher)) {
				fprintf(dbg_file, "\tdirect higher precedence pages:");
				for (que_inst = bdb->bdb_higher.que_forward;
					 que_inst != &bdb->bdb_higher; que_inst = que_inst->que_forward)
				{
					precedence = BLOCK(que_inst, Precedence*, pre_higher);
					hi_bdb = precedence->pre_hi;
					fprintf(dbg_file, " %"SLONGFORMAT"", hi_bdb->bdb_page);
					if (precedence->pre_flags & PRE_cleared)
						fprintf(dbg_file, "(cleared)");
				}
				fprintf(dbg_file, "\n");
			}
			if (QUE_NOT_EMPTY(bdb->bdb_lower)) {
				fprintf(dbg_file, "\tdirect lower precedence pages:");
				for (que_inst = bdb->bdb_lower.que_forward; que_inst != &bdb->bdb_lower;
					 que_inst = que_inst->que_forward)
				{
					precedence = BLOCK(que_inst, Precedence*, pre_lower);
					lo_bdb = precedence->pre_low;
					fprintf(dbg_file, " %"SLONGFORMAT"", lo_bdb->bdb_page);
					if (precedence->pre_flags & PRE_cleared)
						fprintf(dbg_file, "(cleared)");
				}
				fprintf(dbg_file, "\n");
			}
		}
	}

	return TRUE;
}


int DBG_block(BLK block)
{
/**************************************
 *
 *	D B G _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Print a formatted block
 *
 **************************************/
	int i;
	SCHAR s[10], string[100], *p;
	DSC *desc;

#undef BLOCK
#define BLOCK(struct)	((struct) block)
	if (!block) {
		fprintf(dbg_file, "*** NULL ***\n");
		return FALSE;
	}

	/*
	if (block->blk_type <= (SCHAR) type_MIN || block->blk_type >= (SCHAR) type_MAX)
	{
		fprintf(dbg_file, "%X\t*** BAD BLOCK (%d) ***\n", block,
				   block->blk_type);
		return FALSE;
	}
	*/

	if (!block->blk_length) {
		fprintf(dbg_file, "%X\t*** BAD BLOCK LENGTH (%d) ***\n", block, block->blk_length);
		return FALSE;
	}

	const int* fields = dbt_blocks[block->blk_type];
	fprintf(dbg_file, "\n%X\t%s (%d)", block, *fields++, block->blk_length);
	/*
	if (block->blk_type == (SCHAR) type_nod)
		fprintf(dbg_file, " -- %s", node_names[(int) ((jrd_nod*) block)->nod_type]);
	*/

	prt_fields(reinterpret_cast<char*>(block), fields);

	switch ((enum blk_t) block->blk_type)
	{
	case type_vec:
		fprintf(dbg_file, "\t");
		p = string;
		*p = 0;
		for (i = 0; i < ((vec<void**>*) block)->count(); i++) {
			sprintf(p, "%X, ", (*((vec<void**>*) block))[i]);
			if (strlen(string) > 60) {
				fprintf(dbg_file, "%s\n", string);
				strcpy(string, "\t\t");
			}
			p = string + strlen(string);
		}
		fprintf(dbg_file, "%s\n", string);
		break;

	case type_vcl:
		fprintf(dbg_file, "\t");
		p = string;
		*p = 0;
		for (i = 0; i < ((vcl*) block)->count(); i++) {
			sprintf(p, "%X, ", (*(vcl*) block)[i]);
			if (strlen(string) > 60) {
				fprintf(dbg_file, "%s\n", string);
				strcpy(string, "\t\t");
			}
			p = string + strlen(string);
		}
		fprintf(dbg_file, "%s\n", string);
		break;

	case type_bcb:
		prt_que("Empty", &BLOCK(BufferControl*)->bcb_empty);
		for (i = 0; i < BLOCK(BufferControl*)->bcb_count; i++) {
			sprintf(s, "mod %d", i);
			prt_que(s, &BLOCK(BufferControl*)->bcb_rpt[i].bcb_page_mod);
		}
		break;

	case type_bdb:
		fprintf(dbg_file,
				   "\tUse count: %d, page: %d, flags: %x, ast flags: %x\n",
				   ((BufferDesc*) block)->bdb_use_count, ((BufferDesc*) block)->bdb_page,
				   ((BufferDesc*) block)->bdb_flags, ((BufferDesc*) block)->bdb_ast_flags);
#ifdef DIRTY_TREE
		fprintf(dbg_file,
				   "\tParent: %X, left: %X, right: %X, dirty mask: %X\n",
				   ((BufferDesc*) block)->bdb_parent, ((BufferDesc*) block)->bdb_left,
				   ((BufferDesc*) block)->bdb_right, ((BufferDesc*) block)->bdb_transactions);
#else
		fprintf(dbg_file,
				   "\tdirty mask: %X\n",
				   ((BufferDesc*) block)->bdb_transactions);

#endif
		prt_que("Que", &BLOCK(BufferDesc*)->bdb_que);
		prt_que("Higher", &BLOCK(BufferDesc*)->bdb_higher);
		prt_que("Lower", &BLOCK(BufferDesc*)->bdb_lower);
		break;

	case type_pre:
		prt_que("Higher", &BLOCK(Precedence*)->pre_higher);
		prt_que("Lower", &BLOCK(Precedence*)->pre_lower);
		break;

	case type_fmt:
		fprintf(dbg_file, "\t");
		for (i = 0, desc = BLOCK(Format*)->fmt_desc.begin();
			 i < BLOCK(Format*)->fmt_count; desc++, i++)
		{
			prt_dsc(desc, (i % 4) * 20);
			if (i % 4 == 3)
				fprintf(dbg_file, "\n\t");
		}
		fprintf(dbg_file, "\n");
		break;
    default:    /* Shut up compiler warnings */
		break;
	}

	return TRUE;
}


int DBG_check(int pool_id)
{
/**************************************
 *
 *	D B G _ c h e c k
 *
 **************************************
 *
 * Functional description
 *	Check pool for integrity.
 *
 **************************************/
	Database* dbb = GET_DBB();

	int corrupt = 0;

	if (!dbb || !dbb->dbb_pools.getCount())
		return corrupt;

	Database::pool_vec_type* vector = &dbb->dbb_pools;

	Database::pool_ptr pool = (*vector)[pool_id];
	/*
	if (pool) {
		for (HNK hunk = pool->plb_hunks; hunk; hunk = hunk->hnk_next) {
			const char* hunk_end = ((char*)hunk->hnk_address) + hunk->hnk_length;
			for (blk* block = (blk*) hunk->hnk_address; block != (const blk*) hunk_end;
				 block = (blk*) ((SCHAR *) block + (block->blk_length << SHIFT)))
			{
				if (block->blk_pool_id != (UCHAR) pool_id) {
					fprintf(dbg_file, "%X\t*** BAD POOL ID (%d) ***\n",
							   block, block->blk_pool_id);
					++corrupt;
					break;
				}
				if (block->blk_type <= (SCHAR) type_MIN || block->blk_type >= (SCHAR) type_MAX)
				{
					fprintf(dbg_file, "%X\t*** BAD BLOCK (%d) ***\n", block, block->blk_type);
					++corrupt;
					break;
				}
				if (!block->blk_length) {
					fprintf(dbg_file,
							   "%X\t*** BAD BLOCK LENGTH (%d) ***\n", block,
							   block->blk_length);
					++corrupt;
					break;
				}

			}
		}
	}
	*/

	return corrupt;
}


int DBG_close()
{
/**************************************
 *
 *	D B G _ c l o s e
 *
 **************************************
 *
 * Functional description
 *	Close the debugging file.
 *
 **************************************/
	fprintf(dbg_file, "\014\014");
	fclose(dbg_file);
	dbg_file = stdout;
	return TRUE;
}


int DBG_eval(int n)
{
/**************************************
 *
 *	D B G _ e v a l
 *
 **************************************
 *
 * Functional description
 *	Examine a value.
 *
 **************************************/
	fprintf(dbg_file, "octal = %X, decimal = %d, hex = %x\n", n, n, n);
	return TRUE;
}


int DBG_examine(int *n)
{
/**************************************
 *
 *	D B G _ e x a m i n e
 *
 **************************************
 *
 * Functional description
 *	Examine a value.
 *
 **************************************/
	fprintf(dbg_file, "octal = %X, decimal = %d, hex = %x\n", *n, *n, *n);
	return TRUE;
}


int DBG_init()
{
/**************************************
 *
 *	D B G _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

/*
sigset  (2, &DBG_supervisor);
*/
	return TRUE;
}


int DBG_open()
{
/**************************************
 *
 *	D B G _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a debugging output file.
 *
 **************************************/
	SCHAR filename[64];

	printf("Debug file: ");
	scanf("%s", filename);
	dbg_file = fopen(filename, "w");
	return TRUE;
}


int DBG_pool(MemoryPool *pool)
{
/**************************************
 *
 *	D B G _ p o o l
 *
 **************************************
 *
 * Functional description
 *	Print all known blocks.
 *
 **************************************/
	//pool->print_memory_pool_info(dbg_file, 0, DBG_block);
	pool->print_contents(dbg_file);
	return TRUE;
}


int DBG_pretty(const jrd_nod* node, int column)
{
/**************************************
 *
 *	D B G _ p r e t t y
 *
 **************************************
 *
 * Functional description
 *	Pretty print a node tree.
 *
 **************************************/
	int i;

#define NODE(struct)	((struct) node)

	if (node && node->blk_type == (SCHAR) type_rsb)
		return rsb_pretty(reinterpret_cast<const RecordSource*>(node), column);

	fprintf(dbg_file, "%8X\t", node);
	for (i = 0; i < column; i++)
		putc(' ', dbg_file);

	if (node == NULL)
		return fprintf(dbg_file, "*** null ***\n");

	/*
	if (node->blk_type != (SCHAR) type_nod)
		return fprintf(dbg_file, "*** bad node ***\n");
	*/

	fprintf(dbg_file, "%s (%"SLONGFORMAT")", node_names[(int) node->nod_type],
			   node->nod_impure);
	column += 4;

	const jrd_rel* relation;
	const jrd_prc* procedure;
	const jrd_nod* const* ptr;
	const jrd_nod* const* end;
	const IndexRetrieval* retrieval;

	switch (node->nod_type)
	{
	case nod_rse:
		{
			const RecordSelExpr* recse = (RecordSelExpr*) node;
			fprintf(dbg_file, "\n");
			if (recse->rse_rsb)
				DBG_pretty(reinterpret_cast<const jrd_nod*>(recse->rse_rsb), column);
			else {
				DBG_pretty(recse->rse_first, column);
				DBG_pretty(recse->rse_boolean, column);
				DBG_pretty(recse->rse_sorted, column);
				DBG_pretty(recse->rse_projection, column);
				for (ptr = recse->rse_relation, end = ptr + recse->rse_count;
					 ptr < end; ptr++)
				{
					DBG_pretty(*ptr, column);
				}
			}
			break;
		}

	case nod_argument:
		fprintf(dbg_file, ", id: %d, message: %X\n",
				   node->nod_arg[e_arg_number], node->nod_arg[e_arg_message]);
		if (node->nod_arg[e_arg_flag]) {
			for (i = 0; i < column + 10; i++)
				putc(' ', dbg_file);
			fprintf(dbg_file, "flag:\n");
			DBG_pretty(node->nod_arg[e_arg_flag], column);
		}
		if (node->nod_arg[e_arg_indicator]) {
			for (i = 0; i < column + 10; i++)
				putc(' ', dbg_file);
			fprintf(dbg_file, "indicator:\n");
			DBG_pretty(node->nod_arg[e_arg_flag], column);
		}
		return TRUE;

	case nod_message:
		fprintf(dbg_file, ", number: %d, format: %X\n",
				   node->nod_arg[e_msg_number], node->nod_arg[e_msg_format]);
		return TRUE;

	case nod_field:
		fprintf(dbg_file, ", stream: %d, id: %d\n",
				   node->nod_arg[e_fld_stream], node->nod_arg[e_fld_id]);
		return TRUE;

	case nod_index:
		retrieval = (IndexRetrieval*) node->nod_arg[e_idx_retrieval];
		fprintf(dbg_file, ", id: %d\n", retrieval->irb_index);
		for (ptr = retrieval->irb_value, end =
			 ptr + retrieval->irb_lower_count; ptr < end; ptr++)
		{
			DBG_pretty(*ptr, column);
		}
		for (end = ptr + retrieval->irb_upper_count; ptr < end; ptr++)
			DBG_pretty(*ptr, column);
		return TRUE;

	case nod_relation:
		relation = (jrd_rel*) node->nod_arg[e_rel_relation];
		fprintf(dbg_file, ", stream: %d, %s (%X)\n",
				   node->nod_arg[e_rel_stream],
				   relation->rel_name.c_str(), relation);
		return TRUE;

	case nod_procedure:
		{
			const SSHORT procedure_id = (SSHORT)(SLONG) node->nod_arg[e_prc_procedure];
			fprintf(dbg_file, ", stream: %d, prc_id: %d\n",
					   node->nod_arg[e_prc_stream], procedure_id);
			if (node->nod_arg[e_prc_inputs])
				DBG_pretty(node->nod_arg[e_prc_inputs], column);
			return TRUE;
		}

	case nod_exec_proc:
		procedure = (jrd_prc*) node->nod_arg[e_esp_procedure];
		fprintf(dbg_file, ", name: %s (%X)\n",
				   procedure->prc_name.c_str(), procedure);
		for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end;
			 ptr++)
		{
			DBG_pretty(*ptr, column);
		}
		return TRUE;

	case nod_union:
		fprintf(dbg_file, ", stream: %d\n", node->nod_arg[e_uni_stream]);
		DBG_pretty(node->nod_arg[e_uni_clauses], column);
		return TRUE;

	case nod_aggregate:
		fprintf(dbg_file, ", stream: %d\n", node->nod_arg[e_agg_stream]);
		DBG_pretty(node->nod_arg[e_agg_rse], column);
		DBG_pretty(node->nod_arg[e_agg_group], column);
		DBG_pretty(node->nod_arg[e_agg_map], column);
		return TRUE;

	case nod_max:
	case nod_min:
	case nod_average:
	case nod_total:
	case nod_count:
		fprintf(dbg_file, "\n");
		DBG_pretty(node->nod_arg[e_stat_rse], column);
		DBG_pretty(node->nod_arg[e_stat_value], column);
		DBG_pretty(node->nod_arg[e_stat_default], column);
		return TRUE;

	default:
		fprintf(dbg_file, "\n");
		for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end;
			 ptr++)
		{
			DBG_pretty(*ptr, column);
		}
	}

	if (node->nod_type == nod_for && node->nod_arg[e_for_rsb]) {
		rsb_pretty(reinterpret_cast<const RecordSource*>(node->nod_arg[e_for_rsb]),
				   column);
		return TRUE;
	}
    return FALSE;
}

int DBG_supervisor(int arg)
{
/**************************************
 *
 *	D B G _ s u p e r v i s o r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	prior_frame = (int *) *(&arg - 2);

	debug = 0;

	fprintf(dbg_file, "\nEntering JRD diagnostic DBG_supervisor\n");
	int yyparse();
	yyparse();
	fprintf(dbg_file, "\nLeaving JRD diagnostic DBG_supervisor\n");
	DBG_init();

	return TRUE;
}


int DBG_rpb(record_param* rpb)
{
/**************************************
 *
 *	D B G _ r p b
 *
 **************************************
 *
 * Functional description
 *	Print a record paramter block
 *
 **************************************/
	fprintf(dbg_file, "\n%X\tRECORD PARAMETER BLOCK", rpb);
	prt_fields(reinterpret_cast<char*>(rpb), dbt_record_param);
	//DBG_window(reinterpret_cast<int*>(&rpb->rpb_window));
	// This is the best I can get without causing an AV for having a tdbb* being null.
	if (!rpb->rpb_relation->isTemporary())
		DBG_window(reinterpret_cast<int*>(&rpb->getWindow(0)));
	return TRUE;
}


int DBG_smb(SortMap* smb, int column)
{
/**************************************
 *
 *	D B G _ s m b
 *
 **************************************
 *
 * Functional description
 *	Pretty print an SortMap (Sort Memory Block)
 *
 **************************************/
	int i;

	go_column(column);
	fprintf(dbg_file, "SortMap\n");
	go_column(column);
	fprintf(dbg_file,
			   "keys = %d, count = %d length = %d, key_length = %d\n",
			   smb->smb_keys, smb->smb_count, smb->smb_length,
			   smb->smb_key_length);
	for (i = 0; i < smb->smb_keys; i++) {
		go_column(column + 2);
		fprintf(dbg_file,
				   "key [%d] dtype = %d flags = %d length = %d offset = %d, vary_offset = %d\n",
				   i,
				   smb->smb_key_desc[i].skd_dtype,
				   smb->smb_key_desc[i].skd_flags,
				   smb->smb_key_desc[i].skd_length,
				   smb->smb_key_desc[i].skd_offset,
				   smb->smb_key_desc[i].skd_vary_offset);
	}
	for (i = 0; i < smb->smb_count; i++) {
		smb_repeat* ptr = &smb->smb_rpt[i];
		go_column(column + 2);
		fprintf(dbg_file, "fld [%d] flag = %d stream = %d field = %d\n",
				   i, ptr->smb_flag_offset, ptr->smb_stream,
				   ptr->smb_field_id);
		prt_dsc(&ptr->smb_desc, column + 4);
		DBG_pretty(ptr->smb_node, column + 4);
	}
	return TRUE;
}


int DBG_verify()
{
/**************************************
 *
 *	D B G _ v e r i f y
 *
 **************************************
 *
 * Functional description
 *	Verify integrity of all pools.
 *
 **************************************/
	Database* dbb = GET_DBB();

	if (!dbg_file)
		dbg_file = fopen("tt:", "w");

	if (!dbb || !dbb->dbb_pools.getCount())
		return TRUE;

	Database::pool_vec_type* vector = &dbb->dbb_pools;

	for (int i = 0; i < vector->getCount(); i++)
		DBG_check(i);

	return TRUE;
}


int DBG_window(int *window)
{
/**************************************
 *
 *	D B G _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Print a window paramter block
 *
 **************************************/
	fprintf(dbg_file, "\n%X\tWINDOW", window);
	prt_fields(reinterpret_cast<char*>(window), dbt_window);
	return TRUE;
}


int DBG_memory()
{
/**************************************
 *
 *	D B G _ m e m o r y
 *
 **************************************
 *
 * Functional description
 *	Print memory usage
 *
 **************************************/
	Database* dbb = GET_DBB();

	fprintf(dbg_file, "MEMORY UTILIZATION for database\n\n");

/* walk through all the pools in the database */

	perm_pool_mem = 0;
	req_pool_mem = 0;
	trans_pool_mem = 0;
	other_pool_mem = 0;
	int trans_pools = 0;
	int req_pools = 0;
	int other_pools = 0;

	if (!dbb || !dbb->dbb_pools.getCount())
		return TRUE;

	Database::pool_vec_type* vector = &dbb->dbb_pools;

	for (int pool_id = 0; pool_id < vector->getCount(); pool_id++) {
		Database::pool_ptr pool = (*vector)[pool_id];
		if (!pool)
			continue;
		const int pool_type = DBG_analyze(pool_id);
		switch (pool_type)
		{
		case 1:
			break;
		case 2:
			trans_pools++;
			break;
		case 3:
			req_pools++;
			break;
		default:
			other_pools++;
			break;
		}
	}

	fprintf(dbg_file, "\nTotal memory used in the PERMANENT pool = %ldk\n",
			   perm_pool_mem);
	fprintf(dbg_file,
			   "Total memory used in the %d TRANSACTION pools combined = %ldk\n",
			   trans_pools, trans_pool_mem);
	fprintf(dbg_file,
			   "Total memory used in the %d REQUEST pools combined = %ldk\n",
			   req_pools, req_pool_mem);
	fprintf(dbg_file,
			   "Total memory used in the %d OTHER pools combined = %ldk\n",
			   other_pools, other_pool_mem);
	fprintf(dbg_file,
			   "Total memory used in all the pools combined = %ldk\n",
			   trans_pool_mem + req_pool_mem + other_pool_mem +
			   perm_pool_mem);

	fprintf(dbg_file, "\nMemory malloc-ed = %ldk\n", gds_max_alloc / 1024);
	fprintf(dbg_file, "Memory used of the malloc-ed memory = %ldk\n",
			   gds_delta_alloc / 1024);

	return TRUE;
}


static void go_column(int column)
{
/**************************************
 *
 *	g o _ c o l u m n
 *
 **************************************
 *
 * Functional description
 *	Utility function to print a bunch of spaces.
 *
 **************************************/
	while (column-- > 0)
		fprintf(dbg_file, " ");
}


static void prt_dsc(const DSC* desc, int column)
{
/**************************************
 *
 *	p r t _ d s c
 *
 **************************************
 *
 * Functional description
 *	Pretty print an dsc.
 *
 **************************************/
	go_column(column);
	fprintf(dbg_file, "(a=%d, t=%d, l=%d, scale=%d, subtype=%d)\n",
			   desc->dsc_address, desc->dsc_dtype, desc->dsc_length,
			   desc->dsc_scale, desc->dsc_sub_type);
}


static int prt_fields(const SCHAR* block, const int* fields)
{
/**************************************
 *
 *	p r t _ f i e l d s
 *
 **************************************
 *
 * Functional description
 *	Print structured block.
 *
 **************************************/
	const TEXT* string;
	TEXT s[80];

	int column = 99;

	while ( (string = (TEXT*) *fields++) )
	{
		const int offset = *fields++;
		int length = *fields++;
		const TEXT* ptr = (SCHAR*) block + offset;
		switch (length)
		{
		case 0:
		case 1:
			sprintf(s, string, *ptr);
			break;

		case 2:
			sprintf(s, string, *(SSHORT*) ptr);
			break;

		case 4:
			sprintf(s, string, *(SLONG*) ptr);
			break;
		}
		length = 0;
		for (const TEXT* p = s; *p++;)
			length++;
		if ((column += length + 1) >= 60) {
			fprintf(dbg_file, "\n\t");
			column = length + 1;
		}
		fprintf(dbg_file, "%s ", s);
	}

	fprintf(dbg_file, "\n");
	return TRUE;
}


static int prt_que(const SCHAR* string, const que* que_inst)
{
/**************************************
 *
 *	p r t _ q u e
 *
 **************************************
 *
 * Functional description
 *	Print a formatted que_inst entry.
 *
 **************************************/
	fprintf(dbg_file, "\t%X %s forward: %X, backward: %X\n",
			   que_inst, string, que_inst->que_forward, que_inst->que_backward);
	return TRUE;
}


static int rsb_pretty(const RecordSource* rsb, int column)
{
/**************************************
 *
 *	r s b _ p r e t t y
 *
 **************************************
 *
 * Functional description
 *	Pretty print an rsb tree.
 *
 **************************************/
	fprintf(dbg_file, "%X\t", rsb);
	for (int i = 0; i < column; i++)
		putc(' ', dbg_file);

	if (rsb == NULL)
		return fprintf(dbg_file, "*** null ***\n");

	//if (rsb->blk_type != (SCHAR) type_rsb)
	//	return fprintf(dbg_file, "*** bad rsb ***\n");

	fprintf(dbg_file, "%s (%d), stream: %d",
			   rsb_names[(int) rsb->rsb_type], rsb->rsb_impure,
			   rsb->rsb_stream);

	jrd_rel* relation = rsb->rsb_relation;
	if (relation) {
		fprintf(dbg_file, " %s", relation->rel_name.c_str());
	}

	column += 4;

	fprintf(dbg_file, "\n");

	const RecordSource* const* ptr = rsb->rsb_arg;
	if (rsb->rsb_type == rsb_merge) {
		for (const RecordSource* const* const end = ptr + rsb->rsb_count * 2; ptr < end;
			 ptr += 2)
		{
			DBG_pretty(reinterpret_cast<const jrd_nod*>(*ptr), column);
		}
	}
	else if (rsb->rsb_type != rsb_left_cross) {
		for (const RecordSource* const* const end = ptr + rsb->rsb_count; ptr < end; ptr++)
		{
			DBG_pretty(reinterpret_cast<const jrd_nod*>(*ptr), column);
		}
	}
	else {
		for (const RecordSource* const* const end = ptr + rsb->rsb_count + 1; ptr < end;
			 ptr++)
		{
			DBG_pretty(reinterpret_cast<const jrd_nod*>(*ptr), column);
		}
	}

	if (rsb->rsb_next)
		DBG_pretty(reinterpret_cast<jrd_nod*>(rsb->rsb_next), column);
	return TRUE;
}


void yyerror(const char* string)
{
/**************************************
 *
 *	y y e r r o r
 *
 **************************************
 *
 * Functional description
 *	YACC error function.  Boring.
 *
 **************************************/
	fprintf(dbg_file, "%s\n", string);
}


int yywrap()
{
/**************************************
 *
 *	 y y w r a p
 *
 **************************************
 *
 * Functional description
 *	Wrapup function for YACC.
 *
 **************************************/
	return (1);
}

