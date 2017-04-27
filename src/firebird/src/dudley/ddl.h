/*
 *	PROGRAM:	JRD Data Definition Utility
 *	MODULE:		ddl.h
 *	DESCRIPTION:	Common header modules
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

#ifndef DUDLEY_DDL_H
#define DUDLEY_DDL_H

#include "../jrd/common.h"
#include "../jrd/ibase.h"

const int BLOCK_SIZE = 1024;

const int MAXSYMLEN = 257;		// max length of symbol + terminator
const int MAX_PAGE_LEN = 16384;	// max allowable length for a database page

const int DDL_MSG_FAC = 2;

/* Action block.  Do something. */

/* Actions */

enum act_t {
	act_c_database,				/* create database */
	act_m_database,				/* modify database */
	act_d_database,				/* drop a database */
	act_a_relation,				/* add relation */
	act_m_relation,				/* modify existing relation */
	act_d_relation,				/* drop existing relations */
	act_a_gfield,				/* add global field */
	act_m_gfield,				/* modify existing global fields */
	act_d_gfield,				/* drop global field */
	act_a_field,				/* add field to relation */
	act_m_field,				/* modify relation specific fields */
	act_d_field,				/* drop field from relation */
	act_a_index,				/* add index */
	act_m_index,				/* modify index */
	act_d_index,				/* delete index */
	act_a_security,				/* add security class */
	act_d_security,				/* delete security class */
	act_m_security,				/* modify security class */
	act_a_trigger,				/* add new trigger */
	act_m_trigger,				/* modify (replace) trigger */
	act_d_trigger,				/* delete trigger */
	act_a_file,					/* add file */
	act_a_function,				/* add function */
	act_d_function,				/* drop function */
	act_a_function_arg,			/* add function */
	act_d_function_arg,			/* drop function */
	act_a_trigger_msg,			/* add trigger message */
	act_m_trigger_msg,			/* modify trigger message */
	act_d_trigger_msg,			/* drop trigger message */
	act_a_type,					/* add type for field */
	act_m_type,					/* modify type for field */
	act_d_type,					/* drop type for field */
	act_a_filter,				/* add filter */
	act_d_filter,				/* drop filter */
	act_grant,					/* grant user privilege */
	act_revoke,					/* revoke user privilege */
	act_a_shadow,				/* add shadow */
	act_d_shadow,				/* drop shadow */
	act_a_generator,			/* add generator */
	act_s_generator				/* reset generator value */
};

struct dbb;

typedef struct act {
	enum act_t act_type;		// what to do
	act* act_next;				// next action in system
	dbb* act_object;		// object in question (dudley_rel, dudley_fld, idx, etc.)
	USHORT act_line;			// line the action started on
	USHORT act_flags;
} *ACT;

const int ACT_ignore	= 1;	// Ignore the action


/* Context block */

typedef struct dudley_ctx {
	struct sym* ctx_name;
	struct dudley_rel* ctx_relation;
	struct dudley_fld* ctx_field;
	bool   ctx_view_rse;
	USHORT ctx_context_id;
} *DUDLEY_CTX;


/* Database Definition Block */

typedef struct dbb {
	struct sym* dbb_name;
	struct dudley_rel* dbb_relations;
	struct gfl* dbb_fields;
	dbb* dbb_next;
	struct sym* dbb_security_class;
	isc_db_handle dbb_handle;
	isc_tr_handle dbb_transaction;
	struct txt* dbb_description;
	USHORT dbb_flags;
	struct sym* dbb_file_name;
	struct fil* dbb_files;
	USHORT dbb_page_size;
	ULONG dbb_length;			/* Length of database in pages, if known */
	struct fil* dbb_logfiles;
	struct fil* dbb_overflow;
	SLONG dbb_chkptlen;
	SSHORT dbb_numbufs;
	SSHORT dbb_bufsize;
	SLONG dbb_grp_cmt_wait;
	struct fil* dbb_cache_file;
} *DBB;

enum dbb_flags_vals {
	DBB_null_description = 1,
	DBB_null_security_class = 2,
	DBB_create_database = 4
//	DBB_drop_log = 8,
//	DBB_log_serial = 16,
//	DBB_log_preallocated = 32,
//	DBB_log_default = 64,
//	DBB_cascade = 128,
//	DBB_drop_cache = 256
};

enum ods_versions {
	DB_VERSION_DDL4 = 4, // ods4 db
	DB_VERSION_DDL6 = 6, // ods6 db
	DB_VERSION_DDL8 = 8  // ods8 db
};

/* Field block.  Fields are what farms and databases are all about */

typedef struct dudley_fld {
	SSHORT fld_dtype;			/* data type of field */
	SSHORT fld_length;			/* field length in bytes */
	SSHORT fld_scale;			/* scale factor */
	SSHORT fld_position;		/* field position */
	SSHORT fld_segment_length;
	SSHORT fld_sub_type;
	bool   fld_has_sub_type;
	SSHORT fld_dimension;		/* size of multi-dim. array */
	SSHORT fld_system;			/* 0 if field is user defined */
	USHORT fld_flags;			/* misc trash */
	dudley_fld* fld_next;		/* next field in relation */
	struct dudley_rel* fld_relation;	/* relation */
	struct sym* fld_security_class;
	dudley_ctx* fld_context;	/* context for view */
	dbb* fld_database;			/* database for global fields */
	struct sym* fld_name;		/* field name */
	struct sym* fld_source;		/* name of global field */
	dudley_fld* fld_source_field;	/* global field for computed field */
	struct sym* fld_base;		/* base field for views */
	struct sym* fld_query_name;	/* query name */
	struct dudley_nod* fld_query_header;	/* query header */
	struct sym* fld_edit_string;	/* edit string */
	struct dudley_nod* fld_computed;	/* computed by expression */
	struct dudley_nod* fld_missing;	/* missing value */
	struct dudley_nod* fld_default;	/* default value */
	struct dudley_nod* fld_validation;	/* valid if value */
	struct txt* fld_description;	/* description of field */
	struct txt* fld_compute_src;	/* computed_by source */
	struct txt* fld_valid_src;	/* validation source */
	SLONG* fld_ranges;			/* ranges for multi-dim. array */
} *DUDLEY_FLD;

enum fld_flags_vals {
	fld_explicit_position = 1,
	fld_modify = 2,
	fld_local = 4,
	fld_null_description = 8,
	fld_null_security_class = 16,
	fld_null_validation = 32,
	fld_explicit_system = 64,
	fld_null_missing_value = 128,
	fld_null_edit_string = 256,
	fld_null_query_name = 512,
	fld_null_query_header = 1024
};


/* File description block */

typedef struct fil {
	SLONG fil_length;			/* File length in pages */
	SLONG fil_start;			/* Starting page */
	struct sym* fil_name;		/* File name */
	fil* fil_next;				/* next file */
	SSHORT fil_shadow_number;	/* shadow number if part of shadow */
	SSHORT fil_manual;			/* flag to indicate manual shadow */
	SSHORT fil_partitions;		/* number of log file partitions */
	SSHORT fil_raw;				/* on raw device? */
	SSHORT fil_conditional;		/* flag to indicate conditional shadow */
} *FIL;


/* Filter block */

typedef struct filter {
	struct sym* filter_name;	/* symbol for filter name */
	struct txt* filter_description;	/* description of filter */
	struct sym* filter_module_name;	/* symbol for module name */
	struct sym* filter_entry_point;	/* symbol for entrypoint */
	SSHORT filter_input_sub_type;
	SSHORT filter_output_sub_type;
} *FILTER;


/* Function argument block. */

typedef struct funcarg {
	struct sym* funcarg_funcname;	/* symbol for function name */
	SSHORT funcarg_position;	/* argument position */
	SSHORT funcarg_mechanism;	/* argument passed by value, or by reference */
	SSHORT funcarg_dtype;		/* data type of argument */
	SSHORT funcarg_scale;		/* scale factor */
	SSHORT funcarg_length;		/* argument length in bytes */
	SSHORT funcarg_return_arg;	/* argument is the designated return arg */
	SSHORT funcarg_sub_type;	/* sub_type of text */
	bool   funcarg_has_sub_type;	/* null field for sub_type field */
	funcarg* funcarg_next;		/* next field in function */
} *FUNCARG;

enum funcarg_mechanism_vals {
	FUNCARG_mechanism_value = 0,
	FUNCARG_mechanism_reference,
	FUNCARG_mechanism_descriptor,
	FUNCARG_mechanism_blob_struc,
	FUNCARG_mechanism_sc_array_desc
};


/* Function description block */

typedef struct func {
	struct sym* func_name;		/* symbol for function name */
	struct sym* func_query_name;	/* symbol for query name */
	struct sym* func_module_name;	/* symbol for module name */
	struct sym* func_entry_point;	/* symbol for entrypoint */
	SSHORT func_return_arg;		/* return argument position */
	func* func_next;			/* next function in database */
	dbb* func_database;			/* database for function */
	struct txt* func_description;	/* description of function */
	funcarg* func_args;			/* Known function arguments */
	funcarg* func_return;		/* Return argument */
} *FUNC;


/* Index description block */

enum idx_direction
{
	IDX_type_none = 0,
	IDX_type_descend = 1
};

typedef struct dudley_idx {
	USHORT idx_count;			/* Number of fields */
	bool idx_unique;			/* true if unique index */
	bool idx_inactive;			/* false if index is active */
	idx_direction idx_type;		/* true descending */
	USHORT idx_flags;			/* Indicate which attributes have changed */
	struct sym* idx_name;		/* Index name */
	struct sym* idx_relation;	/* Relation in question */
	struct txt* idx_description;	/* Description pointer */
	struct sym* idx_field[1];	/* Fields */
} *DUDLEY_IDX;

enum idx_flags_vals {
	IDX_active_flag = 1,
	IDX_unique_flag = 2,
	IDX_null_description = 4,
	IDX_type_flag = 8,
	IDX_statistics_flag = 16
};

static inline size_t IDX_LEN(const size_t cnt)
{
	return sizeof (struct dudley_idx) +
		(cnt ? cnt - 1 : 0) * sizeof (((DUDLEY_IDX) NULL)->idx_field[0]);
}

/* Linked list stack stuff */

struct dudley_lls {
	struct dudley_nod *lls_object;		// object on stack
	dudley_lls* lls_next;				// next item on stack
};


/* General Syntax node, produced by parser */

enum nod_t {
	nod_field = 1, nod_literal, nod_value,
	nod_and, nod_or, nod_not,
	nod_eql, nod_neq, nod_geq,
	nod_leq, nod_gtr, nod_lss,
	nod_containing, nod_matches, nod_any,
	nod_unique, nod_add, nod_multiply,
	nod_divide, nod_subtract, nod_negate,
	nod_msg, nod_for, nod_send,
	nod_receive, nod_block, nod_select,
	nod_boolean, nod_projection, nod_sort,
	nod_store, nod_modify, nod_erase,
	nod_if, nod_assignment, nod_rse,
	nod_first, nod_context, nod_end,
	nod_label, nod_leave, nod_loop,
	nod_max, nod_min, nod_count,
	nod_total, nod_average, nod_list,
	nod_deferred, nod_between, nod_missing,
	nod_field_name, nod_name, nod_starts,
	nod_from, nod_fid, nod_concatenate,
	nod_abort, nod_null, nod_user_name,
	nod_post, nod_function, nod_gen_id,
	nod_uppercase, nod_sleuth, nod_over,
	nod_set_generator, nod_index,
	nod_lowercase
};

typedef struct dudley_nod {
	enum nod_t nod_type;		/* node type */
	UCHAR* nod_blr;				/* symbolic blr string */
	SSHORT nod_count;			/* number of sub-items */
	dudley_nod* nod_arg[1];		/* argument */
} *DUDLEY_NOD;

static inline size_t NOD_LEN(const size_t cnt)
{
	return sizeof(dudley_nod) +
		(cnt ? cnt - 1 : 0) * sizeof (((DUDLEY_NOD) NULL)->nod_arg[0]);
}

/* Relation block, not to be confused with siblings or in-laws */

typedef struct dudley_rel {
	dbb* rel_database;			/* parent database */
	struct sym* rel_filename;	/* external filename */
	dudley_fld* rel_fields;		/* linked list of known fields */
	struct sym* rel_name;		/* symbol for relation */
	struct sym* rel_security_class;	/* name of security class */
	dudley_rel* rel_next;		/* next relation in database */
	dudley_nod* rel_rse;		/* view rse */
	struct txt* rel_description;	/* description of relation */
	struct txt* rel_view_source;	/* source dml for view definition */
	USHORT rel_field_position;	/* highest used field position */
	SSHORT rel_system;			/* 0 if relation is user defined */
	USHORT rel_flags;
} *DUDLEY_REL;

enum rel_flags_values {
	rel_null_description = 1,
	rel_null_security_class = 2,
	rel_explicit_system = 4,
	rel_marked_for_delete = 8,
	rel_null_ext_file = 16,
	rel_marked_for_modify = 32,
	rel_marked_for_creation = 64
};


/* Security class handling */

typedef struct scl {
	struct sym* scl_name;		/* name of security class */
	struct txt* scl_description;	/* description of security class */
	struct sce* scl_entries;	/* list of entries */
} *SCL;

const int SCL_write = 2;

/* Security entry */

typedef struct sce {
	sce* sce_next;				/* next security item in list */
	SLONG sce_privileges;		/* bitmask of privileges */
	UCHAR* sce_idents[20];		/* misc identification stuff */
	UCHAR sce_strings[1];
} *SCE;


/* String block for build DYN & BLR strings */

class str {
public:
	UCHAR* str_start;			/* start of string buffer */
	UCHAR* str_current;			/* current position in string being built */
	USHORT str_length;			/* length of buffer */
	inline void add_byte(const int byte) {
		*str_current++ = byte;
	}
	inline void add_word(const int word) {
		add_byte(word);
		add_byte(word >> 8);
	}
};

typedef str* STR;
/* Symbol block, also used for hash table */

enum sym_t {
	SYM_keyword,				/* unspecified */
	SYM_context,				/* context variable */
	SYM_database,				/* seems like a good idea */
	SYM_relation,				/* if you don't know your relations, how do you know your friends? */
	SYM_global,					/* Global field */
	SYM_field,					/* Local field */
	SYM_function,				/* UDF */
	SYM_trigger					/* any named element deserves to be hashed */
};

typedef struct sym {
	const char* sym_string;			/* address of asciz string */
	SSHORT sym_length;			/* length of string (exc. term.) */
	enum sym_t sym_type;		/* symbol type */
	SSHORT sym_keyword;			/* keyword number, if keyword */
	dudley_ctx* sym_object;		/* general pointer to object */
	sym* sym_collision;			/* collision pointer */
	sym* sym_homonym;			/* homonym pointer */
	TEXT sym_name[1];			/* space for name, if necessary */
} *SYM;

const size_t SYM_LEN = sizeof(sym);

/* Trigger block */

/* these are the externally visible trigger types */

typedef enum {
	trg_type_none = 0,			/* pre store */
	trg_store = 1,				/* pre store */
	trg_post_store = 2,
	trg_modify = 3,				/* pre modify */
	trg_post_modify = 4,
	trg_pre_erase = 5,
	trg_erase = 6				/* post erase */
} TRG_T;

/* these types are used in parsing */

enum parse_trig_types {
	trig_pre = 0,
	trig_post  = 1,
	trig_sto = 2,
	trig_mod = 4,
	trig_era = 8,			// erase defaults to post
	trig_inact = 16
};

typedef struct dudley_trg {
	TRG_T trg_type;
	DUDLEY_REL trg_relation;
	DUDLEY_NOD trg_statement;			/* blr */
	sym* trg_name;				/* symbol for trigger */
	struct txt* trg_description;	/* description of relation */
	struct txt* trg_source;		/* source of trigger */
	SSHORT trg_sequence;
	SSHORT trg_inactive;		/* 0 = on, 1 = off */
	USHORT trg_mflag;			/* modify attributes */
} *DUDLEY_TRG;

/* trg_modify_flag */

enum trg_modify_flag_vals {
	trg_mflag_onoff = 1,
	trg_mflag_type = 2,
	trg_mflag_seqnum = 4,
	trg_mflag_order = 8
};


/* Trigger message block */

typedef struct trgmsg {
	sym* trgmsg_trg_name;		/* symbol for trigger */
	SSHORT trgmsg_number;		/* abort code */
	sym* trgmsg_text;
} *TRGMSG;

typedef enum {
	trgmsg_none = 0,
	trgmsg_add = 1,
	trgmsg_modify = 2,
	trgmsg_drop = 3
} TRGMSG_T;


/* Text block */

typedef struct txt {
	TEXT* txt_file;
	ULONG txt_position;
	USHORT txt_length;
	USHORT txt_start_line;
} *TXT;


/* Type block */

typedef struct typ {
	sym* typ_field_name;		/* field name */
	sym* typ_name;				/* type name */
	SSHORT typ_type;			/* type value */
	txt* typ_description;		/* description of relation */
} *TYP;


/* User privilege block */

typedef struct userpriv {
	sym* userpriv_relation;
	struct usre* userpriv_userlist;
	struct upfe* userpriv_upflist;
	USHORT userpriv_flags;
} *USERPRIV;


/* user privilege flags */
enum userpriv_flags_vals {
	USERPRIV_select = 1,
	USERPRIV_delete = 2,
	USERPRIV_insert = 4,
	USERPRIV_update = 8,
	USERPRIV_grant = 16
};

/* rdb$user_privilege.rdb$privilege */

static const char* const UPRIV_SELECT = "SELECT";
static const char* const UPRIV_DELETE = "DELETE";
static const char* const UPRIV_INSERT = "INSERT";
static const char* const UPRIV_UPDATE = "UPDATE";

/* user name entry */

typedef struct usre {
	usre* usre_next;
	sym* usre_name;
} *USRE;

/* update field entry */

typedef struct upfe {
	upfe* upfe_next;
	sym* upfe_fldname;
} *UPFE;


/* Data types */

#include "../jrd/dsc.h"


/* Constant block */

typedef struct con {
	dsc con_desc;
	UCHAR con_data[1];
} *CON;


/* Program globals */

typedef enum lan_t {
	lan_undef,
	lan_pascal,
	lan_fortran,
	lan_cobol,
	lan_ansi_cobol,
	lan_c,
	lan_ada,
	lan_cxx
} LAN_T;

#include "parse.h"

struct DudleyGlobals {
	enum lan_t language;
	bool DDL_eof;
	USHORT DDL_errors;
	USHORT DDL_line;
	bool DDL_interactive;
	bool DDL_quit;
	bool DDL_dynamic;
	bool DDL_drop_database;
	bool DDL_service;
	bool DDL_replace;
	bool DDL_description;
	bool DDL_extract;
	bool DDL_trace;
	bool DDL_version;
#ifdef TRUSTED_AUTH
	bool DDL_trusted;
#endif
	const TEXT* DDL_prompt;
	const TEXT* DDL_file_name;
	TEXT DYN_file_name[256];
	const TEXT* DB_file_name;
	TEXT DDL_file_string[256];
	TEXT DB_file_string[256];
	const TEXT* DDL_default_user;
	const TEXT* DDL_default_password;
	ACT DDL_actions;
	DBB database;
	// from parse.h
	tok DDL_token;
};

extern DudleyGlobals dudleyGlob;

#include "../dudley/ddl_proto.h"

enum nod_val_pos {
	s_rse_first = 0,		// FIRST clause, if any
	s_rse_boolean,			// Boolean clause, if any
	s_rse_sort,				// Sort clause, if any
	s_rse_reduced,			// Reduced clause, if any
	s_rse_contexts,			// Relation block
	s_rse_count,

	s_stt_rse = 0,
	s_stt_value,
	s_stt_default,
	s_stt_count,

	s_fld_field = 0,		// Field block
	s_fld_context,			// Context block
	s_fld_name,
	s_fld_subs,
	s_fld_count,

	s_if_boolean = 0,
	s_if_true,
	s_if_false,

	s_for_rse = 0,
	s_for_action,

	s_store_rel = 0,
	s_store_action,

	s_mod_old_ctx = 0,
	s_mod_new_ctx,
	s_mod_action
};

#endif // DUDLEY_DDL_H

