/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dbt.cpp
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
#include "../jrd/ibase.h"
// Those includes until the END comment comes from everything.h and was moved
// here when the file was removed.
// Most probably only a few of the includes are needed
#include "../jrd/common.h"
#include "../jrd/isc.h"
#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "../jrd/ods.h"
#include "../jrd/cch.h"
#include "../jrd/os/pio.h"
#include "../jrd/pag.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"
#include "../jrd/lls.h"
#include "../jrd/rse.h"
#include "../jrd/sbm.h"
#include "../jrd/tra.h"
#include "../jrd/sqz.h"
#include "../jrd/blb.h"
#include "../jrd/btr.h"
#include "../jrd/scl.h"
#include "../jrd/ext.h"
#include "../jrd/met.h"
#include "../jrd/sdw.h"
#include "../jrd/intl.h"
#include "../jrd/intl_classes.h"
#include "../jrd/fil.h"
#include "../jrd/tpc.h"
#include "../jrd/svc.h"
#include "../jrd/blob_filter.h"
// END
#include "../jrd/dbg.h"

#define FLD(struct, string, field) string, (SCHAR*) OFFSET (struct, field), (SCHAR*) sizeof (((struct) NULL)->field)


typedef SCHAR* TEXT_PTR;

int* ptr;

TEXT_PTR dbt_window[] =
{
	FLD(WIN*, "Page: %ld", win_page),
	FLD(WIN*, "Buffer: %x", win_buffer),
	FLD(WIN*, "BufferDesc: %x", win_bdb),
	FLD(WIN*, "Scans: %d", win_scans),
	FLD(WIN*, "Flags: %x", win_flags),
	0
},
dbt_record_param[] =
{
	FLD(record_param*, "Relation %x", rpb_relation),
	FLD(record_param*, "Number %"SQUADFORMAT, rpb_number.getValue()),
	FLD(record_param*, "Trans %ld", rpb_transaction_nr),
	FLD(record_param*, "Page %ld", rpb_page),
	FLD(record_param*, "Line %x", rpb_line),
	FLD(record_param*, "Back page %ld", rpb_b_page),
	FLD(record_param*, "Line %x", rpb_b_line),
	FLD(record_param*, "Fragment page %ld", rpb_f_page),
	FLD(record_param*, "Line %x", rpb_f_line),
	FLD(record_param*, "Format %x", rpb_format_number),
	FLD(record_param*, "Address: %x ", rpb_address),
	FLD(record_param*, "Length %x", rpb_length),
	FLD(record_param*, "Record %x", rpb_record),
	FLD(record_param*, "Flags %x", rpb_flags),
	0
};

static TEXT_PTR dbb_stuff[] =
{
	"DATABASE",
	FLD(Database*, "BCB: %x", dbb_bcb),
	FLD(Database*, "Relations: %x", dbb_relations),
	FLD(Database*, "Lock: %x", dbb_lock),
	FLD(Database*, "File: %x", dbb_file),
	FLD(Database*, "Permanent: %x", dbb_permanent),
	FLD(Database*, "Pools: %x", dbb_pools),
	FLD(Database*, "Page_size: %d", dbb_page_size),
	FLD(Database*, "dp_per_pp: %d", dbb_dp_per_pp),
	0
},
vec[] =
{
	"VECTOR",
	FLD(vec<void**>*, "Count %d", count()),
	"Count %d", (SCHAR*)4, (SCHAR*)4,
	0
},
vcl[] =
{
	"VECTOR OF ULONGS",
	FLD(vcl*, "Count %d", count()),
	"Count %d", (SCHAR*)4, (SCHAR*)4,
	0
};
/*frb[] =
{
	"FREE",
// TMN: FIXFIX! John?
//		FLD(FRB, "Next %x", frb_next),
		"Next %x", (SCHAR*)4, (SCHAR*)4,
		0
};*/
/*static TEXT_PTR hnk[] =
{
	"HUNK",
		FLD(HNK, "Addr: %x", hnk_address),
		FLD(HNK, "Len: %d", hnk_length),
		FLD(HNK, "Next: %x", hnk_next),
		0
};
*/
/*static TEXT_PTR plb[] =
{
	"POOL",
		FLD(PLB, "Id: %d", plb_pool_id),
		FLD(PLB, "Free: %x", plb_free),
		FLD(PLB, "Hunk: %x", plb_hunks),
		0
};*/

static TEXT_PTR BufferControl[] =
{
	"BUFFER CONTROL",
		FLD(BufferControl*, "Count: %x", bcb_count),
		0
},
BufferDesc[] =
{
	"BUFFER DESCRIPTOR",
		FLD(BufferDesc*, "Page: %ld", bdb_page),
		FLD(BufferDesc*, "Lock: %x", bdb_lock),
		FLD(BufferDesc*, "Buffer: %x", bdb_buffer),
		FLD(BufferDesc*, "Use count: %x", bdb_use_count),
		FLD(BufferDesc*, "Flags: %x", bdb_flags),
		0
},
Precedence[] =
{
	"PRECEDENCE",
		FLD(Precedence*, "Flags: %x", pre_flags),
		FLD(Precedence*, "Low: %x", pre_low),
		FLD(Precedence*, "High: %x", pre_hi),
		0
},
Lock[] =
{
	"LOCK",
		FLD(Lock*, "Parent: %x", lck_parent),
		FLD(Lock*, "Object: %x", lck_object),
		FLD(Lock*, "Type: %x", lck_type),
		FLD(Lock*, "Physical: %x", lck_physical),
		FLD(Lock*, "Logical: %x", lck_logical),
		FLD(Lock*, "Length: %x", lck_length),
		0
},
jrd_file[] =
{
	"FILE",
		FLD(jrd_file*, "File desc: %x", fil_desc),
		0
},
PageControl[] =
{
	"PAGE CONTROL",
		FLD(PageControl*, "High water: %d", pgc_high_water),
		FLD(PageControl*, "Pages/PIP: %x", pgc_ppp),
		FLD(PageControl*, "First PIP: %x", pgc_pip),
		FLD(PageControl*, "Bytes/PIP: %x", pgc_bytes),
		0
},

jrd_rel[] =
{
	"RELATION",
		FLD(jrd_rel*, "%s", rel_name.c_str()),
		FLD(jrd_rel*, "Id: %d", rel_id),
		FLD(jrd_rel*, "Current format: %x", rel_current_format),
		FLD(jrd_rel*, "Formats: %x", rel_formats),
		FLD(jrd_rel*, "Pages: %x", rel_pages),
		FLD(jrd_rel*, "Root: %ld", rel_index_root),
		0
},
Format[] =
{
	"FORMAT",
		FLD(Format*, "Count: %d", fmt_count),
		FLD(Format*, "Length: %d", fmt_length),
		FLD(Format*, "Version: %d", fmt_version),
		0
},
jrd_req[] =
{
	"REQUEST",
		FLD(jrd_req*, "COUNT: %x", req_count),
		FLD(jrd_req*, "Impure: %x", req_impure_size),
		FLD(jrd_req*, "Incarn: %x", req_incarnation),
		FLD(jrd_req*, "Pool: %x", req_pool),
		FLD(jrd_req*, "Sub req: %x", req_sub_requests),
		FLD(jrd_req*, "Trans: %x", req_transaction),
		FLD(jrd_req*, "Next req: %x", req_request),
		FLD(jrd_req*, "Msg: %x", req_message),
		FLD(jrd_req*, "Length: %x", req_length),
		//FLD(jrd_req*, "#msgs: %x", req_nmsgs),
		//FLD(jrd_req*, "Max send: %x", req_msend),
		//FLD(jrd_req*, "Max receive: %x", req_mreceive),
		FLD(jrd_req*, "Top: %x", req_top_node),
		FLD(jrd_req*, "Next: %x", req_next),
		FLD(jrd_req*, "Label: %x", req_label),
		FLD(jrd_req*, "Op: %x", req_operation),
		FLD(jrd_req*, "Flags: %x", req_flags),
		0
},
jrd_tra[] =
{
	"TRANSACTION",
		FLD(jrd_tra*, "Number: %ld", tra_number),
		FLD(jrd_tra*, "Oldest: %ld", tra_oldest),
		FLD(jrd_tra*, "Next: %x", tra_next),
		FLD(jrd_tra*, "Pool: %x", tra_pool),
		FLD(jrd_tra*, "Lock: %x", tra_lock),
		FLD(jrd_tra*, "Locks: %x", tra_relation_locks),
		FLD(jrd_tra*, "Flags: %x", tra_flags),
		0
},
jrd_nod[] =
{
	"NODE",
		FLD(jrd_nod*, "Type: %x", nod_type),
		FLD(jrd_nod*, "Impure: %x", nod_impure),
		0
},
lls[] =
{
	"LINKED LIST STACK",
		FLD(LLS, "Object: %x", lls_object),
		FLD(LLS, "Next: %x", lls_next),
		0
},
VerbAction[] =
{
	"RECORD",
		FLD(Record*, "Format: %x", rec_format),
		0
},
RecordSource[] =
{
	"RECORD SOURCE BLOCK",
		FLD(RecordSource*, "Type: %x", rsb_type),
		FLD(RecordSource*, "Stream: %x", rsb_stream),
		FLD(RecordSource*, "Relation: %x", rsb_relation),
		FLD(RecordSource*, "Next: %x", rsb_next),
		FLD(RecordSource*, "Count: %x", rsb_count),
		0
},
OptimizerBlk[] =
{
	"OPTIMIZER",
		FLD(OptimizerBlk*, "CompilerScratch*: %x", opt_csb),
		FLD(OptimizerBlk*, "Cnt: %x", opt_count),
		0
},
/* CVC: Obsolete
BitmapSegment[] =
{
	"BIT MAP SEGMENT",
		FLD(BitmapSegment*, "Min: %x", bms_min),
		FLD(BitmapSegment*, "Max: %x", bms_max),
		0
},
*/
DeferredWork[] =
{
	"DEFERRED WORK BLOCK",
		FLD(DeferredWork*, "type: %d", dfw_type),
		FLD(DeferredWork*, "next: %x", dfw_next),
		FLD(DeferredWork*, "name: %s", dfw_name.c_str()),
		0
},
TemporaryField[] =
{
	"TEMPORARY FIELD BLOCK",
		FLD(TemporaryField*, "id: %d", tfb_id),
		FLD(TemporaryField*, "dtype: %d", tfb_desc.dsc_dtype),
		FLD(TemporaryField*, "scale: %d", tfb_desc.dsc_scale),
		FLD(TemporaryField*, "len: %d", tfb_desc.dsc_length),
		0
},
str[] =
{
	"string",
		FLD(STR, "length: %d", str_length),
		0
},
/* CVC: It doesn't work with the new definition.
SparseBitmap[] =
{
	"SPARSE BIT MAP",
		FLD(SparseBitmap*, "state: %d", sbm_state),
		FLD(SparseBitmap*, "count: %d", sbm_count),
		FLD(SparseBitmap*, "used: %d", sbm_used),
		FLD(SparseBitmap*, "high water: %d", sbm_high_water),
		FLD(SparseBitmap*, "number: %d", sbm_number),
		0
},
*/
SortMap[] =
{
	"SORT MAP",
		FLD(SortMap*, "count: %d", smb_count),
		FLD(SortMap*, "keys: %d", smb_keys),
		FLD(SortMap*, "length: %d", smb_length),
		FLD(SortMap*, "sort key: %x", smb_key_desc),
		0
},
blb[] =
{
	"BLOB",
		FLD(blb*, "Relation: %x", blb_relation),
		FLD(blb*, "Count: %d", blb_count),
		FLD(blb*, "Length: %d", blb_length),
		FLD(blb*, "Max seg: %d", blb_max_segment),
		FLD(blb*, "Flags: %x", blb_flags),
		FLD(blb*, "Trans: %x", blb_transaction),
		FLD(blb*, "Next: %x", blb_segment),
		0
},
IndexRetrieval[] =
{
	"INDEX RETRIEVAL",
		FLD(IndexRetrieval*, "index: %d", irb_index),
		FLD(IndexRetrieval*, "relation: %x", irb_relation),
		FLD(IndexRetrieval*, "lower bounds: %d", irb_lower_count),
		FLD(IndexRetrieval*, "upper boudns: %d", irb_upper_count),
		0
},
BlobControl[] =
{
	"BLOB CONTROL", 0
};

static TEXT_PTR SecurityClass[] = {	"SECURITY CLASS", 0};
static TEXT_PTR jrd_fld[] = {	"FIELD", 0};
static TEXT_PTR ExternalFile[] = {	"EXTERNAL FILE", 0};
static TEXT_PTR merge_file[] = {	"MERGE EQUIVALENCE FILE BLOCK", 0};
static TEXT_PTR River[] = {	"SORT MERGE RIVER", 0};
static TEXT_PTR UserId[] = {	"USER IDENTIFICATION BLOCK ", 0};
static TEXT_PTR Attachment[] = {	"ATTACHMENT BLOCK", 0};
static TEXT_PTR Symbol[] = {	"SYMBOL", 0};
static TEXT_PTR UserFunction[] = {	"FUNCTION", 0};
static TEXT_PTR IndexedRelationship[] = {	"INDEXED RELATIONSHIP", 0};
static TEXT_PTR AccessItem[] = {	"ACCESS", 0};
static TEXT_PTR Resource[] = {	"RESOURCE", 0};
static TEXT_PTR IndexLock[] = {	"INDEX LOCK", 0};
static TEXT_PTR Shadow[] = {	"SHADOW", 0};
static TEXT_PTR Savepoint[] = {	"SAVE POINT", 0};
static TEXT_PTR VerbAction[] = {	"VERB", 0};
static TEXT_PTR BlobFilter[] = {	"BLOB FILTER", 0};
static TEXT_PTR ArrayField[] = {	"ARRAY DESCRIPTION", 0};
//static TEXT_PTR blb_map[] = {	"MAP BLOCK", 0};
static TEXT_PTR dir_list[] = {	"DIR LIST BLOCK", 0};
static TEXT_PTR jrd_prc[] =
{
	"PROCEDURE",
		FLD(jrd_prc*, "%s", prc_name.c_str()),
		FLD(jrd_prc*, "Id: %d", prc_id), 0
};
static TEXT_PTR Parameter[] = {	"PARAMETER", FLD(PRM, "%s", prm_name.c_str()), 0};
static TEXT_PTR IndexBlock[] = {	"INDEX BLOCK", 0};
//static TEXT_PTR Bookmark[] = {	"BOOKMARK BLOCK", 0};
//static TEXT_PTR RefreshRange[] = {	"REFRESH RANGE BLOCK", 0};
static TEXT_PTR TxPageCache[] = {	"TIP CACHE BLOCK", 0};
static TEXT_PTR PsqlException[] = {	"EXCEPTION LIST BLOCK", 0};
static TEXT_PTR OptimizerBlk[] = {	"OPTIMIZATION BLOCK", 0};
#ifdef SUPERSERVER_V2
static TEXT_PTR Prefetch[] = {	"PRF", 0};
#endif
static TEXT_PTR RecordSelExpr[] = {	"RECORD SELECTION EXPRESSION", 0};
static TEXT_PTR Literal[] = {	"LITERAL", 0};
static TEXT_PTR AggregateSort[] = {	"AggregateSort", 0};
//static TEXT_PTR srl[] = {	"SRL", 0}; // Obsolete.

	/* xxx
	   x [] = {
	   "x",
	   FLD (x, "x: %x", x),
	   0},
	 */
static TEXT_PTR CompilerScratch[] =
{
	"COMPILE SCRATCH BLOCK",
		FLD(CompilerScratch*, "Count: %x", csb_count),
		FLD(CompilerScratch*, "Node: %x", csb_node),
		FLD(CompilerScratch*, "Streams: %x", csb_n_stream),
		FLD(CompilerScratch*, "Running: %x", csb_running),
		FLD(CompilerScratch*, "BLR: %x", csb_blr),
		0
};

static TEXT_PTR texttype[] =
{
/*	"INTL TEXT OBJECT",
		FLD(TEXTTYPE, "Name: %s", texttype_name),
		FLD(TEXTTYPE, "Vers: %d", texttype_version),
		FLD(TEXTTYPE, "ID:   %d", texttype_type),
		FLD(TEXTTYPE, "CS:   %d", texttype_character_set),
		FLD(TEXTTYPE, "Cntry:%d", texttype_country),
		FLD(TEXTTYPE, "Flags:%d", texttype_flags),*/
		0
};
static TEXT_PTR charset[] =
{
/*	"INTL Character Set",
		FLD(charset*, "Name: %s", charset_name),
		FLD(charset*, "Vers: %d", charset_version),
		FLD(charset*, "ID:   %d", charset_id),
		FLD(charset*, "B/Ch: %d", charset_max_bytes_per_char),
		FLD(charset*, "B/Ch: %d", charset_min_bytes_per_char),
		FLD(charset*, "Flags:%d", charset_flags), */
		0
};
static TEXT_PTR csconvert[] =
{
/*	"INTL Character set converter",
		FLD(csconvert*, "Name: %s", csconvert_name),
		FLD(csconvert*, "from: %d", csconvert_from),
		FLD(csconvert*, "to:   %d", csconvert_to), */
		0
};
static TEXT_PTR thread_db[] =
{
	"THREAD DATA BLOCK",
		FLD(thread_db*, "Status vec: %x", tdbb_status_vector),
		FLD(thread_db*, "Default: %x", getDefaultPool()),
		0
};
static TEXT_PTR Service[] =			{	"SERVICE MANAGER BLOCK", 0};
static TEXT_PTR LatchWait[] =		{	"LATCH WAIT BLOCK", 0};
static TEXT_PTR ViewContext[] =		{	"VIEW CONTEXT BLOCK", 0};
static TEXT_PTR SaveRecordParam[] =	{	"RPB BLOCK", 0};


typedef int (*t_intvoid)();

t_intvoid dbg_all, dbg_block, dbg_examine, dbg_eval,
	dbg_open, dbg_close, dbg_pool, dbg_pretty,
	dbg_window, dbg_rpb, dbg_bdbs, dbg_analyze,
	dbg_check, dmp_page, dmp_active, dmp_dirty, dbg_verify;


symb dbt_symbols[] =
{
	{"blk", &dbg_block, symb_printer, sizeof(int)},
	{"ev", &dbg_eval, symb_printer, sizeof(int)},
	{"ex", &dbg_examine, symb_printer, sizeof(int)},
	{"dump", &dmp_page, symb_printer, sizeof(SLONG)},
	{"pool", &dbg_pool, symb_printer, 0},
	{"pretty", &dbg_pretty, symb_printer, 0},
	{"analyze", &dbg_analyze, symb_printer, 0},
	{"window", &dbg_window, symb_printer, 0},
	{"rpb", &dbg_rpb, symb_printer, 0},
	{"check", &dbg_check, symb_printer, 0},

	{"dirty", &dmp_dirty, symb_routine, 0},
	{"active", &dmp_active, symb_routine, 0},
	{"all", &dbg_all, symb_routine, 0},
	{"open", &dbg_open, symb_routine, 0},
	{"close", &dbg_close, symb_routine, 0},
	{"bdbs", &dbg_bdbs, symb_routine, 0},
	{"verify", &dbg_verify, symb_routine, 0},

//
//#define SYM(struct, name)	"name", OFFSET (struct, name), 0, symb_offset, sizeof (((struct) NULL)->name),
//    "Database", &Database, symb_absolute, sizeof(Database),
//    SYM (Database*, dbb_bcb)
//    SYM (Database*, dbb_relations)
//    SYM (Database*, dbb_pools)
//    SYM (Database*, dbb_requests)
//    SYM (REL, rel_formats)
//    SYM (REL, rel_pages)
//    SYM (jrd_req*, req_top_node)
//    SYM (jrd_req*, req_next)
//
	{NULL, 0, symb_routine, 0}
};

#define BLKDEF(type, name, tail) (TEXT*) name,

#define Database dbb_stuff

TEXT* dbt_blocks[] =
{
	0,
#include "../jrd/blk.h"
	0
};
#undef BLKDEF
