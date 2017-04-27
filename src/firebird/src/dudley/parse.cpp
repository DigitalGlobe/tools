/*
 *	PROGRAM:	JRD data definition language parser
 *	MODULE:		parse.cpp
 *	DESCRIPTION:	Statement parser
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
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include <stdlib.h>
#include <stdio.h> // a single call to sprintf()
#include <string.h>
#include "../jrd/ibase.h"
#include "../jrd/flags.h"
#include "../dudley/ddl.h"
#include "../jrd/acl.h"
#include "../dudley/ddl_proto.h"
#include "../dudley/exe_proto.h"
#include "../dudley/expr_proto.h"
#include "../dudley/hsh_proto.h"
#include "../dudley/lex_proto.h"
#include "../dudley/parse_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../common/classes/MsgPrint.h"

using MsgFormat::SafeArg;


const char* const PROMPT			= "GDEF> ";
const char* const CONTINUATION	= "CON>  ";
const int GDS_NAME_LEN		= 31;

const int MAX_DIMENSION		= 16;
#ifdef FLINT_CACHE
const int MIN_CACHE_BUFFERS	= 250;
const int DEF_CACHE_BUFFERS	= 1000;
#endif

typedef enum {
	obj_database,
	obj_relation,
	obj_view,
	obj_field,
	obj_index,
	obj_security_class,
	obj_trigger,
	obj_file,
	obj_none,
	obj_function,
	obj_type,
	obj_filter,
	obj_shadow,
	obj_generator
} OBJ_T;

/* this table translates between the two types of types */

static TRG_T trig_table[] = {
	trg_type_none,
	trg_type_none,
	trg_store,
	trg_post_store,
	trg_modify,
	trg_post_modify,
	trg_type_none,
	trg_type_none,
	trg_pre_erase,
	trg_erase
};

static bool check_filename(SYM, bool);
static SYM copy_symbol(SYM);
static DUDLEY_FLD create_global_field(DUDLEY_FLD);
#ifdef FLINT_CACHE
static FIL define_cache();
#endif
static void define_database(enum act_t);
static void define_field();
static FIL define_file();
static void define_filter();
static void define_function();
static void define_generator();
static void define_index();
//static FIL define_log_file(USHORT);
static void define_old_trigger();
static void define_relation();
static void define_security_class();
static void define_shadow();
static void define_trigger();
static void define_type();
static void define_view();
static void drop_filter();
static void drop_function();
static void drop_gfield();
static void drop_index();
static void drop_relation();
static void drop_security_class();
static void drop_shadow();
static void drop_trigger();
static void drop_type();
static void end_text(TXT);
static SYM gen_trigger_name(TRG_T, DUDLEY_REL);
static int get_system_flag();
static void get_trigger_attributes(int *, int *, int *);
static void grant_user_privilege();
static DUDLEY_CTX lookup_context(SYM, dudley_lls*);
static DUDLEY_FLD lookup_global_field(DUDLEY_FLD);
static ACT make_action(enum act_t, DBB);
static ACT make_computed_field(DUDLEY_FLD);
static DUDLEY_CTX make_context(TEXT *, DUDLEY_REL);
static ACT make_global_field(DUDLEY_FLD);
static void mod_old_trigger();
static void modify_field();
static void modify_index();
static void modify_relation();
static void modify_security_class();
static void modify_trigger();
static void modify_trigger_action(DUDLEY_TRG, DUDLEY_REL);
static void modify_type();
static void modify_view();
static bool parse_action();
static void parse_array(DUDLEY_FLD);
static TXT parse_description();
static void parse_end();
static DUDLEY_FLD parse_field(DUDLEY_FLD);
static void parse_field_clauses(DUDLEY_FLD);
static void parse_field_dtype(DUDLEY_FLD);
static void parse_field_subtype(DUDLEY_FLD);
static FUNCARG parse_function_arg(FUNC, USHORT *);
static SCE parse_identifier();
static OBJ_T parse_object();
static int parse_page_size();
static SLONG parse_privileges();
static void revoke_user_privilege();
static SLONG score_entry(SCE);
static DUDLEY_NOD set_generator();
static void sort_out_attributes(DUDLEY_TRG, SLONG, SLONG, SLONG);
static TXT start_text();
static void validate_field(DUDLEY_FLD);

static dudley_lls* local_context;


void PARSE_actions()
{
/**************************************
 *
 *	P A R S E_ a c t i o n s
 *
 **************************************
 *
 * Functional description
 *	Parse actions in a loop until end of file.
 *
 **************************************/

	parse_action();
	while (!dudleyGlob.DDL_eof && dudleyGlob.DDL_errors < 20) {
		if ((!dudleyGlob.database || !dudleyGlob.database->dbb_handle) && (!dudleyGlob.DDL_drop_database)) {
			DDL_err(111);	/* msg 111: no database declared */
			if (dudleyGlob.database) {
				gds__free(dudleyGlob.database);
				dudleyGlob.database = NULL;
			}
			if (dudleyGlob.DDL_interactive)
				parse_action();
			else {
				DDL_err(112);	/* msg 112: ceasing processing */
				break;
			}
		}
		else {
			dudleyGlob.DDL_drop_database = false;
			parse_action();
		}
	}
}


void PARSE_error( USHORT number, const TEXT* arg1, const TEXT* arg2)
{
/**************************************
 *
 *	P A R S E _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Generate a syntax error.
 *
 **************************************/

	DDL_err(number, SafeArg() << arg1 << arg2);
	Firebird::LongJump::raise();
}


void PARSE_error( USHORT number, int arg1, int arg2)
{
/**************************************
 *
 *	P A R S E _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Generate a syntax error.
 *
 **************************************/

	DDL_err(number, SafeArg() << arg1 << arg2);
	Firebird::LongJump::raise();
}


FUNC PARSE_function(int existingFunction)
{
/**************************************
 *
 *	P A R S E _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Get the function block associated with the current token.  Also
 *	advance the token.  Create a new function requested.
 *
 **************************************/
	if (dudleyGlob.DDL_token.tok_type != tok_ident)
		PARSE_error(113, dudleyGlob.DDL_token.tok_string, 0);	/* msg 113: expected function, encountered \"%s\" */

	SYM symbol = HSH_typed_lookup(dudleyGlob.DDL_token.tok_string, dudleyGlob.DDL_token.tok_length,
							  SYM_function);

	FUNC function;
	if (symbol && (function = (FUNC) symbol->sym_object) &&
		function->func_database == dudleyGlob.database)
	{
		LEX_token();
		return function;
	}

	if (existingFunction) {
		PARSE_error(114, dudleyGlob.DDL_token.tok_string, 0);	/* msg 114: expected function, encountered \"%s\" */
		return NULL; // silence uninitialized warning
	}

	function = (FUNC) DDL_alloc(sizeof(func));
	function->func_name = symbol = PARSE_symbol(tok_ident);
	symbol->sym_type = SYM_function;
	symbol->sym_object = (DUDLEY_CTX) function;
	HSH_insert(symbol);

	if (!(function->func_database = dudleyGlob.database))
		PARSE_error(111, 0, 0);	/* msg 111: no database declared */

	return function;
}


enum kwwords PARSE_keyword()
{
/**************************************
 *
 *	P A R S E _ k e y w o r d
 *
 **************************************
 *
 * Functional description
 *	Get a real token and return the keyword number.
 *
 **************************************/
	LEX_real();

	for (SYM symbol = dudleyGlob.DDL_token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_keyword)
			return (enum kwwords) symbol->sym_keyword;

	return KW_none;
}


DUDLEY_NOD PARSE_make_list(dudley_lls* stack)
{
/**************************************
 *
 *	P A R S E _ m a k e _ l i s t
 *
 **************************************
 *
 * Functional description
 *	Dump a stack of junk into a list node.  Best count
 *	them first.
 *
 **************************************/
	dudley_lls* temp = stack;
	USHORT count = 0;

	while (temp) {
		count++;
		temp = temp->lls_next;
	}

	DUDLEY_NOD node = PARSE_make_node(nod_list, count);

	while (stack)
		node->nod_arg[--count] = LLS_POP(&stack);

	return node;
}


DUDLEY_NOD PARSE_make_node(enum nod_t type, USHORT count)
{
/**************************************
 *
 *	P A R S E _ m a k e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Allocate and initialize a syntax node of given type.
 *
 **************************************/
	DUDLEY_NOD node = (DUDLEY_NOD) DDL_alloc(NOD_LEN(count));
	node->nod_type = type;
	node->nod_count = count;

	return node;
}


bool PARSE_match( enum kwwords keyword)
{
/**************************************
 *
 *	P A R S E _ m a t c h
 *
 **************************************
 *
 * Functional description
 *	Test the current token for a particular keyword.  If so, advanced
 *	the token stream.
 *
 **************************************/
	if (dudleyGlob.DDL_token.tok_keyword == keyword) {
		LEX_token();
		return true;
	}

	for (SYM symbol = dudleyGlob.DDL_token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_keyword &&
			symbol->sym_keyword == (int) keyword)
		{
			LEX_token();
			return true;
		}

	return false;
}


int PARSE_number()
{
/**************************************
 *
 *	P A R S E _ n u m b e r
 *
 **************************************
 *
 * Functional description
 *	Parse the next token as a number and return its value.
 *
 **************************************/
	const bool negate = PARSE_match(KW_MINUS);

	if (dudleyGlob.DDL_token.tok_type != tok_number)
		PARSE_error(115, dudleyGlob.DDL_token.tok_string, 0);	/* msg 115: expected number, encountered \"%s\" */

	const int number = atoi(dudleyGlob.DDL_token.tok_string);
	LEX_token();

	if (negate)
		return -number;

	return number;
}


DUDLEY_REL PARSE_relation()
{
/**************************************
 *
 *	P A R S E _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Get the relation block associated with the current token.
 *	If the relatio was defined before, use the old definition.
 *	If not, create a new one.  Somebody else will decide whether
 *	this one is worth keeping.
 *
 **************************************/
	if (dudleyGlob.DDL_token.tok_type != tok_ident)
		PARSE_error(116, dudleyGlob.DDL_token.tok_string, 0);	/* msg 116: expected relation name, encountered \"%s\" */

	SYM symbol = HSH_typed_lookup(dudleyGlob.DDL_token.tok_string, dudleyGlob.DDL_token.tok_length,
							  SYM_relation);

	DUDLEY_REL relation;
	if (symbol && (relation = (DUDLEY_REL) symbol->sym_object) &&
		relation->rel_database == dudleyGlob.database)
	{
		LEX_token();
		return relation;
	}

	relation = (DUDLEY_REL) DDL_alloc(sizeof(dudley_rel));
	relation->rel_name = symbol = PARSE_symbol(tok_ident);
	symbol->sym_type = SYM_relation;
	symbol->sym_object = (DUDLEY_CTX) relation;

	if (!(relation->rel_database = dudleyGlob.database))
		PARSE_error(111, 0, 0);	/* msg 111: no database declared */

	relation->rel_next = dudleyGlob.database->dbb_relations;
	dudleyGlob.database->dbb_relations = relation;

	return relation;
}


SYM PARSE_symbol(enum tok_t type)
{
/**************************************
 *
 *	P A R S E _ s y m b o l
 *
 **************************************
 *
 * Functional description
 *	Swallow the current token as a symbol.
 *
 **************************************/
	if (dudleyGlob.DDL_token.tok_type != type)
		switch (type) {
		case tok_ident:
			PARSE_error(117, dudleyGlob.DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		case tok_quoted:
			PARSE_error(118, dudleyGlob.DDL_token.tok_string, 0);	/* msg 118: expected quoted string, encountered \"%s\" */
		default:
			PARSE_error(119, dudleyGlob.DDL_token.tok_string, 0);	/* msg 119: expected symbol, encountered \"%s\" */
		}

	USHORT l = dudleyGlob.DDL_token.tok_length;
	const TEXT* q = dudleyGlob.DDL_token.tok_string;

	if (type == tok_quoted) {
		if (l < 2)
			PARSE_error(118, dudleyGlob.DDL_token.tok_string, 0);	/* msg 118: expected quoted string, encountered \"%s\" */
		q++;
		l -= 2;
	}

	SYM symbol = (SYM) DDL_alloc(SYM_LEN + l);
	symbol->sym_length = l;
	symbol->sym_string = symbol->sym_name;
	TEXT* p = symbol->sym_name;

	if (l)
		if (type == tok_ident)
			do {
				const TEXT c = *q++;
				*p++ = UPPER(c);
			} while (--l);
		else
			do {
				*p++ = *q++;
			} while (--l);

	LEX_token();

	return symbol;
}


static bool check_filename(SYM name,
						   bool decnet_flag)
{
/**************************************
 *
 *	c h e c k _ f i l e n a m e
 *
 **************************************
 *
 * Functional description
 *	Make sure that a file path doesn't contain an
 *	inet node name.
 *
 **************************************/
	TEXT file_name[256];

	USHORT l = name->sym_length;
	if (!l)
		return true;
	l = MIN(l, sizeof(file_name) - 1);
	TEXT* pp = file_name;
	for (const TEXT* q = name->sym_string; l--; *pp++ = *q++);
	*pp = 0;

	for (const TEXT* p = file_name; *p; p++)
		if (p[0] == ':' && p[1] == ':')
			if (!decnet_flag)
				return false;
			else {
				for (p = file_name; *p;)
					if (*p++ == '^')
						return false;
				return true;
			}

	return (!ISC_check_if_remote(file_name, false));
}


static SYM copy_symbol( SYM old_name)
{
/**************************************
 *
 *	c o p y _ s y m b o l
 *
 **************************************
 *
 * Functional description
 *    Create a new symbol block for
 *    the same symbol as an existing block.
 *    This seems dumber than it is, because
 *    the implicit creation of global fields
 *    from local definitions depends on it as
 *    does the implicit invocation of same-named
 *    global fields.
 *
 *    We'll just leave the type blank for now.
 *
 **************************************/
	SYM new_name = (SYM) DDL_alloc(SYM_LEN + old_name->sym_length);
	new_name->sym_length = old_name->sym_length;
	new_name->sym_string = new_name->sym_name;
	strcpy(new_name->sym_name, old_name->sym_name);

	return new_name;
}


static DUDLEY_FLD create_global_field( DUDLEY_FLD local_field)
{
/**************************************
 *
 *	c r e a t e _ g l o b a l _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Copy a field block so we can
 *	have separate blocks for global
 *	and local fields, even when the
 *	global field is defined implicitly.
 *
 *	This creates a new field and symbol
 *	block.
 *
 **************************************/
	DUDLEY_FLD  global_field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
	global_field->fld_dtype = local_field->fld_dtype;
	global_field->fld_length = local_field->fld_length;
	global_field->fld_scale = local_field->fld_scale;
	global_field->fld_segment_length = local_field->fld_segment_length;
	global_field->fld_sub_type = local_field->fld_sub_type;
	global_field->fld_has_sub_type = local_field->fld_has_sub_type;
	global_field->fld_dimension = local_field->fld_dimension;
	global_field->fld_system = local_field->fld_system;
	global_field->fld_database = local_field->fld_database;
	global_field->fld_query_name = local_field->fld_query_name;
	global_field->fld_query_header = local_field->fld_query_header;
	global_field->fld_edit_string = local_field->fld_edit_string;
	global_field->fld_computed = local_field->fld_computed;
	global_field->fld_missing = local_field->fld_missing;
	global_field->fld_default = local_field->fld_default;
	global_field->fld_validation = local_field->fld_validation;
	global_field->fld_description = local_field->fld_description;
	global_field->fld_compute_src = local_field->fld_compute_src;
	global_field->fld_valid_src = local_field->fld_valid_src;
	global_field->fld_ranges = local_field->fld_ranges;

	if (local_field->fld_source)
		global_field->fld_name = local_field->fld_source;
	else {
		SYM old_name = local_field->fld_name;
		SYM new_name = copy_symbol(old_name);
		global_field->fld_name = new_name;
		new_name->sym_type = SYM_global;
		new_name->sym_object = (DUDLEY_CTX) global_field;
		local_field->fld_source = new_name;
	}

	global_field->fld_flags = local_field->fld_flags & ~fld_local;
	local_field->fld_source_field = global_field;
	local_field->fld_flags |= fld_local;

	return global_field;
}

#ifdef FLINT_CACHE
static FIL define_cache()
{
/**************************************
 *
 *	d e f i n e _ c a c h e
 *
 **************************************
 *
 * Functional description
 *	Add a shared cache to an existing database.
 *
 **************************************/
	FIL file = (FIL) DDL_alloc(sizeof(fil));
	file->fil_name = PARSE_symbol(tok_quoted);
	if (!check_filename(file->fil_name, false))
		PARSE_error(322, 0, 0);	/* msg 322: a node name is not permitted in a shared cache file name */

	if (PARSE_match(KW_LENGTH)) {
		if ((file->fil_length = PARSE_number()) < MIN_CACHE_BUFFERS)
			PARSE_error(339, MIN_CACHE_BUFFERS);	/* msg 339: minimum of %d cache pages required */
		PARSE_match(KW_PAGES);
	}
	else
		file->fil_length = DEF_CACHE_BUFFERS;	/* default cache buffers */

	return file;
}
#endif

static void define_database( enum act_t action_type)
{
/**************************************
 *
 *	d e f i n e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Parse the tail of a DEFINE, MODIFY, or DELETE DATABASE statement.
 *
 **************************************/
	if (dudleyGlob.database)
		DDL_error_abort(0, 120);
		// msg 120: GDEF processes only one database at a time

	dudleyGlob.database = (DBB) DDL_alloc(sizeof(dbb));
	dudleyGlob.database->dbb_name = PARSE_symbol(tok_quoted);
	dudleyGlob.DB_file_name = dudleyGlob.database->dbb_name->sym_name;
	dudleyGlob.database->dbb_grp_cmt_wait = -1;	/* Initialized for default value */

/* parse options for the database parameter block */

	while (true) {
		if (PARSE_match(KW_LENGTH)) {
			dudleyGlob.database->dbb_length = PARSE_number();
			PARSE_match(KW_PAGES);
		}
		else if (PARSE_match(KW_USER)) {
			SYM symbol = PARSE_symbol(tok_quoted);
			dudleyGlob.DDL_default_user = symbol->sym_name;
		}
		else if (PARSE_match(KW_PASSWORD)) {
			SYM symbol = PARSE_symbol(tok_quoted);
			dudleyGlob.DDL_default_password = symbol->sym_name;
		}
		else
			break;
	}

	if (action_type == act_d_database) {
//		if (PARSE_match(KW_CASCADE))
//			dudleyGlob.database->dbb_flags |= DBB_cascade;
		EXE_drop_database(dudleyGlob.database);
		dudleyGlob.DDL_drop_database = true;
		return;
	}

/* parse add/drop items */

	while (true) {
		PARSE_match(KW_ADD);
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			dudleyGlob.database->dbb_description = parse_description();
		else if (PARSE_match(KW_SECURITY_CLASS))
			dudleyGlob.database->dbb_security_class = PARSE_symbol(tok_ident);
		else if (PARSE_match(KW_DROP)) {
			if (PARSE_match(KW_DESCRIP))
				dudleyGlob.database->dbb_flags |= DBB_null_description;
			else if (PARSE_match(KW_SECURITY_CLASS))
				dudleyGlob.database->dbb_flags |= DBB_null_security_class;
/*
			else if (PARSE_match(KW_LOG_FILE)) {
				if ((dudleyGlob.database->dbb_flags & DBB_log_default) ||
					(dudleyGlob.database->dbb_logfiles))
				{
					PARSE_error(337, 0, 0);
				}

				dudleyGlob.database->dbb_flags |= DBB_drop_log;
			}
			else if (PARSE_match(KW_CASCADE))
				dudleyGlob.database->dbb_flags |= DBB_cascade;
#ifdef FLINT_CACHE
			else if (PARSE_match(KW_CACHE))
				dudleyGlob.database->dbb_flags |= DBB_drop_cache;
#endif // FLINT_CACHE
*/
			else
				PARSE_error(121, 0, 0);
			// msg 121 only SECURITY_CLASS, DESCRIPTION and CACHE can be dropped
		}
		else if (PARSE_match(KW_FILE)) {
			FIL file = define_file();
			file->fil_next = dudleyGlob.database->dbb_files;
			dudleyGlob.database->dbb_files = file;
		}
		else if (PARSE_match(KW_PAGE_SIZE)) {
			if (action_type == act_m_database)
				PARSE_error(122, 0, 0);	/* msg 122: PAGE_SIZE can not be modified */
			dudleyGlob.database->dbb_page_size = parse_page_size();
		}
/*
		else if (PARSE_match(KW_CHECK_POINT_LEN)) {
			PARSE_match(KW_EQUALS);
			dudleyGlob.database->dbb_chkptlen = PARSE_number();
		}
		else if (PARSE_match(KW_NUM_LOG_BUFS)) {
			PARSE_match(KW_EQUALS);
			dudleyGlob.database->dbb_numbufs = PARSE_number();
		}
		else if (PARSE_match(KW_LOG_BUF_SIZE)) {
			PARSE_match(KW_EQUALS);
			dudleyGlob.database->dbb_bufsize = PARSE_number();
		}
		else if (PARSE_match(KW_GROUP_COMMIT_WAIT)) {
			PARSE_match(KW_EQUALS);
			dudleyGlob.database->dbb_grp_cmt_wait = PARSE_number();
		}
		else if (PARSE_match(KW_LOG_FILE)) {
			if ((dudleyGlob.database->dbb_flags & DBB_log_default) ||
				(dudleyGlob.database->dbb_logfiles))
			{
					PARSE_error(338, 0, 0);
			}

			if (dudleyGlob.database->dbb_flags & DBB_drop_log)
				PARSE_error(337, 0, 0);

			if (PARSE_match(KW_LEFT_PAREN)) {
				dudleyGlob.database->dbb_flags |= DBB_log_preallocated;
				while (true) {
					FIL file = define_log_file(DBB_log_preallocated);
					file->fil_next = dudleyGlob.database->dbb_logfiles;
					dudleyGlob.database->dbb_logfiles = file;
					if (!PARSE_match(KW_COMMA))
						break;
				}

				if (!PARSE_match(KW_RIGHT_PAREN))
				{
					PARSE_error(341, dudleyGlob.DDL_token.tok_string, 0);
					// msg 341: expected comma or ')', encountered \"%s\"
				}

				if (PARSE_match(KW_OVERFLOW))
					dudleyGlob.database->dbb_overflow = define_log_file(DBB_log_serial);
				else
					PARSE_error(340, 0, 0);
			}
			else if (PARSE_match(KW_BASE_NAME)) {
				dudleyGlob.database->dbb_flags |= DBB_log_serial;
				dudleyGlob.database->dbb_logfiles = define_log_file(DBB_log_serial);
			}
			else
				dudleyGlob.database->dbb_flags |= DBB_log_default;
		}
#ifdef FLINT_CACHE
		else if (PARSE_match(KW_CACHE))
			dudleyGlob.database->dbb_cache_file = define_cache();
#endif // FLINT_CACHE
*/
		else
			break;
	}

	parse_end();

	if (action_type == act_c_database) {
		dudleyGlob.database->dbb_flags |= DBB_create_database;
		EXE_create_database(dudleyGlob.database);
	}
	else {
		EXE_modify_database(dudleyGlob.database);
	}

	make_action(action_type, dudleyGlob.database);
}


static void define_field()
{
/**************************************
 *
 *	d e f i n e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE FIELD statement.
 *
 **************************************/
	DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
	parse_field(field);
	field->fld_database = dudleyGlob.database;

	if (!field->fld_dtype)
		PARSE_error(123, 0, 0);	/* msg 123: data type required for global field */

	if (field->fld_security_class)
		PARSE_error(124, 0, 0);	/* msg 124: Security class can appear only on local field references */

	parse_end();

	make_global_field(field);
}


static FIL define_file()
{
/**************************************
 *
 *	d e f i n e _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Add a new file to an existing database.
 *
 **************************************/
	FIL file = (FIL) DDL_alloc(sizeof(fil));
	file->fil_name = PARSE_symbol(tok_quoted);
	if (!check_filename(file->fil_name, false))
		PARSE_error(297, 0, 0);	/* msg 297: A node name is not permitted in a shadow or secondary file name */

	while (true) {
		if (PARSE_match(KW_LENGTH)) {
			file->fil_length = PARSE_number();
			PARSE_match(KW_PAGES);
		}
		else if (PARSE_match(KW_STARTS)) {
			PARSE_match(KW_AT);
			PARSE_match(KW_PAGE);
			file->fil_start = PARSE_number();
		}
		else
			break;
	}

	return file;
}


static void define_filter()
{
/**************************************
 *
 *	d e f i n e _ f i l t e r
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE FILTER statement.
 *
 **************************************/
	if (dudleyGlob.DDL_token.tok_type != tok_ident)
		PARSE_error(126, dudleyGlob.DDL_token.tok_string, 0);	/* msg 126: expected filter name, encountered \"%s\" */

	FILTER new_filter = (FILTER) DDL_alloc(sizeof(filter));
	new_filter->filter_name = PARSE_symbol(tok_ident);

	while (true) {
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			new_filter->filter_description = parse_description();
		else if (PARSE_match(KW_INPUT_TYPE))
			new_filter->filter_input_sub_type = PARSE_number();
		else if (PARSE_match(KW_OUTPUT_TYPE))
			new_filter->filter_output_sub_type = PARSE_number();
		else if (PARSE_match(KW_FUNCTION_MODULE_NAME))
			new_filter->filter_module_name = PARSE_symbol(tok_quoted);
		else if (PARSE_match(KW_FUNCTION_ENTRY_POINT))
			new_filter->filter_entry_point = PARSE_symbol(tok_quoted);
		else
			break;
	}

	if (!new_filter->filter_entry_point)
		PARSE_error(127, 0, 0);	/* msg 127: Filter entry point must be specified */

#if (defined WIN_NT)
	if (!new_filter->filter_module_name)
		PARSE_error(128, 0, 0);	/* msg 128: Filter module name must be specified */
#endif

	parse_end();
	make_action(act_a_filter, (DBB) new_filter);
}


static void define_function()
{
/**************************************
 *
 *	d e f i n e _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE FUNCTION statement.
 *
 **************************************/
	FUNC function = PARSE_function(FALSE);

	while (true) {
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			function->func_description = parse_description();
		else if (PARSE_match(KW_FUNCTION_MODULE_NAME))
			function->func_module_name = PARSE_symbol(tok_quoted);
		else if (PARSE_match(KW_FUNCTION_ENTRY_POINT))
			function->func_entry_point = PARSE_symbol(tok_quoted);
		else if (PARSE_match(KW_QUERY_NAME))
			function->func_query_name = PARSE_symbol(tok_ident);
		else
			break;
	}

	if (!function->func_entry_point)
		PARSE_error(130, 0, 0);	/* msg 130: Function entry point must be specified */

#if (defined WIN_NT)
	if (!function->func_module_name)
		PARSE_error(131, 0, 0);	/* msg 131: Function module name must be specified */
#endif

/* Gobble function arguments */

	SSHORT position = 1;

	while (true) {
		if (dudleyGlob.DDL_token.tok_keyword == KW_SEMI)
			break;
		FUNCARG function_arg = parse_function_arg(function, (USHORT*) &position);
		function_arg->funcarg_funcname = function->func_name;
		make_action(act_a_function_arg, (DBB) function_arg);
		if (!PARSE_match(KW_COMMA))
			break;
	}

	if (!(dudleyGlob.DDL_token.tok_keyword == KW_SEMI))
		PARSE_error(132, dudleyGlob.DDL_token.tok_string, 0);	/* msg 132: expected comma or semi-colon, encountered \"%s\" */

	make_action(act_a_function, (DBB) function);
}


static void define_generator()
{
/**************************************
 *
 *	d e f i n e _ g e n e r a t o r
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE GENERATOR statement.
 *
 **************************************/
	if (dudleyGlob.DDL_token.tok_type != tok_ident)
		PARSE_error(274, dudleyGlob.DDL_token.tok_string, 0);	/* msg 274: expected generator name, encountered \"%s\" */

	SYM symbol = PARSE_symbol(tok_ident);
	parse_end();
	make_action(act_a_generator, (DBB) symbol);
}


static void define_index()
{
/**************************************
 *
 *	d e f i n e _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Define a new index.
 *
 **************************************/
	TXT description = NULL;
	bool unique = false;
	bool inactive = false;
	idx_direction descending = IDX_type_none;

	SYM index_name = PARSE_symbol(tok_ident);
	PARSE_match(KW_FOR);
	SYM rel_name = PARSE_symbol(tok_ident);

	while (true) {
		if (PARSE_match(KW_DUPLICATES))
			unique = false;
		else if (PARSE_match(KW_UNIQUE))
			unique = true;
		else if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			description = parse_description();
		else if (PARSE_match(KW_ACTIVE))
			inactive = false;
		else if (PARSE_match(KW_INACTIVE))
			inactive = true;
		else if (PARSE_match(KW_ASCENDING))
			descending = IDX_type_none;
		else if (PARSE_match(KW_DESCENDING))
			descending = IDX_type_descend;
		else
			break;
	}

	SSHORT count = 0;

	dudley_lls* stack = NULL;

	while (true) {
		LLS_PUSH((DUDLEY_NOD) PARSE_symbol(tok_ident), &stack);
		count++;
		if (!PARSE_match(KW_COMMA))
			break;
	}

	if (!(dudleyGlob.DDL_token.tok_keyword == KW_SEMI))
		PARSE_error(135, dudleyGlob.DDL_token.tok_string, 0);
	// msg 135: expected comma or semi-colon, encountered \"%s\"
	DUDLEY_IDX index = (DUDLEY_IDX) DDL_alloc(IDX_LEN(count == 0 ? 1 : count));
	index->idx_count = count;
	index->idx_name = index_name;
	index->idx_relation = rel_name;
	index->idx_unique = unique;
	index->idx_description = description;
	index->idx_inactive = inactive;
	index->idx_type = descending;

	while (stack)
		index->idx_field[--count] = (SYM) LLS_POP(&stack);

	make_action(act_a_index, (DBB) index);
}


/*
static FIL define_log_file( USHORT log_type)
{
// **************************************
// *
// *	d e f i n e _ l o g _ f i l e
// *
// **************************************
// *
// * Functional description
// *	define a log file
// *
// **************************************
	FIL file = (FIL) DDL_alloc(sizeof(fil));
	file->fil_name = PARSE_symbol(tok_quoted);
	if (!check_filename(file->fil_name, false))
		PARSE_error(297, 0, 0);
	// msg 297: A node name is not permitted in a shadow or secondary file name

	while (true) {
		if (PARSE_match(KW_SIZE)) {
			PARSE_match(KW_EQUALS);
			file->fil_length = PARSE_number();
		}
		else if (PARSE_match(KW_RAW_PARTITIONS)) {
			if (log_type != DBB_log_preallocated)
				PARSE_error(332, 0, 0);
			// msg 332: Partitions not supported in series of log file specification
			PARSE_match(KW_EQUALS);
			file->fil_partitions = PARSE_number();
			file->fil_raw = LOG_raw;
		}
		else
			break;
	}

// Check for the specified length of the log file

	if (file->fil_partitions) {
		if (!file->fil_length)
			PARSE_error(335, file->fil_name->sym_string, 0);
			// msg 335: Total length of the partitioned log <logname> must be specified
	}

	return file;
}
*/


static void define_old_trigger()
{
/**************************************
 *
 *	d e f i n e _ o l d _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse old style trigger definition
 *
 **************************************/
	SYM name = NULL;
	DUDLEY_TRG trigger = NULL;
	DUDLEY_REL relation = PARSE_relation();

	while (!PARSE_match(KW_END_TRIGGER)) {
		trigger = (DUDLEY_TRG) DDL_alloc(sizeof(dudley_trg));
		trigger->trg_relation = relation;

		if (PARSE_match(KW_STORE))
			trigger->trg_type = trg_store;
		else if (PARSE_match(KW_MODIFY))
			trigger->trg_type = trg_modify;
		else if (PARSE_match(KW_ERASE))
			trigger->trg_type = trg_erase;
		else
			PARSE_error(136, dudleyGlob.DDL_token.tok_string, 0);
		// msg 136: expected STORE, MODIFY, ERASE, END_TRIGGER, encountered \"%s\"
		PARSE_match(KW_COLON);

		name = gen_trigger_name(trigger->trg_type, relation);

		trigger->trg_name = name;
		trigger->trg_source = start_text();
		trigger->trg_statement = EXPR_statement();
		end_text(trigger->trg_source);

		make_action(act_a_trigger, (DBB) trigger);
	}

	parse_end();

	if (trigger) {
		name->sym_type = SYM_trigger;
		name->sym_object = (DUDLEY_CTX) trigger;
		HSH_insert(name);
	}
}


static void define_relation()
{
/**************************************
 *
 *	d e f i n e _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE RELATION statement.
 *
 **************************************/
	DUDLEY_REL relation = PARSE_relation();
	if (!(relation->rel_flags & rel_marked_for_delete) &&
		((relation->rel_flags & rel_marked_for_creation) || EXE_relation(relation)))
	{
		PARSE_error(137, relation->rel_name->sym_string, 0);
		// msg 137: relation %s already exists
	}

	if (PARSE_match(KW_EXTERNAL_FILE)) {
		relation->rel_filename = PARSE_symbol(tok_quoted);
		if (!check_filename(relation->rel_filename, true))
			PARSE_error(298, 0, 0);
		// msg 298: A non-Decnet node name is not permitted in an external file name
	}

	ACT action = make_action(act_a_relation, (DBB) relation);
	ACT rel_actions = action;
	action->act_flags |= ACT_ignore;
	SSHORT position = 1;

	while (true)
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			relation->rel_description = parse_description();
		else if (PARSE_match(KW_SECURITY_CLASS))
			relation->rel_security_class = PARSE_symbol(tok_ident);
		else if (PARSE_match(KW_SYSTEM_FLAG)) {
			relation->rel_system = get_system_flag();
			relation->rel_flags |= rel_explicit_system;
		}
		else
			break;

/* Gobble fields */

	DUDLEY_FLD global;
	while (true) {
		PARSE_match(KW_ADD);
		PARSE_match(KW_FIELD);
		DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
		parse_field(field);
		field->fld_relation = relation;
		field->fld_database = dudleyGlob.database;
		if (field->fld_computed)
			make_computed_field(field);
		else if (!(field->fld_flags & fld_local))
			make_global_field(create_global_field(field));
		else if (global = lookup_global_field(field)) {
			field->fld_dtype = global->fld_dtype;
			field->fld_length = global->fld_length;
			field->fld_scale = global->fld_scale;
			field->fld_segment_length = global->fld_segment_length;
			field->fld_sub_type = global->fld_sub_type;
			field->fld_has_sub_type = global->fld_has_sub_type;
		}
		if (field->fld_flags & fld_explicit_position)
			position = field->fld_position + 1;
		else
			field->fld_position = position++;
		field->fld_flags |= fld_explicit_position;
		field->fld_name->sym_type = SYM_field;
		HSH_insert(field->fld_name);
		action = make_action(act_a_field, (DBB) field);
		action->act_flags |= ACT_ignore;
		if (!PARSE_match(KW_COMMA))
			break;
	}

	if (!(dudleyGlob.DDL_token.tok_keyword == KW_SEMI))
		PARSE_error(138, dudleyGlob.DDL_token.tok_string, 0);
	// msg 138: expected comma or semi-colon, encountered \"%s\"

// We started off by assuming that the relation and field actions that
// have been defined would have to be ignored.  Everything has gone well so
// turn them on.

	while (true) {
		action->act_flags &= ~ACT_ignore;
		if (action == rel_actions)
			break;
		action = action->act_next;
	}

	HSH_insert(relation->rel_name);
	relation->rel_flags &= ~rel_marked_for_delete;
	relation->rel_flags |= rel_marked_for_creation;
}


static void define_security_class()
{
/**************************************
 *
 *	d e f i n e _ s e c u r i t y _ c l a s s
 *
 **************************************
 *
 * Functional description
 *	Parse an access control list.
 *
 **************************************/
	SCL sec_class = (SCL) DDL_alloc(sizeof(scl));
	sec_class->scl_name = PARSE_symbol(tok_ident);
	if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
		sec_class->scl_description = parse_description();

/* Pick up entries. Use the users's order */

	while (true) {
		SCE element = parse_identifier();
		if (!element)
			return;
		SCE* next;
		for (next = &sec_class->scl_entries; *next; next = &(*next)->sce_next);
		*next = element;
		element->sce_privileges = parse_privileges();
		if (!PARSE_match(KW_COMMA))
			break;
	}

	parse_end();

	make_action(act_a_security, (DBB) sec_class);
}


static void define_shadow()
{
/**************************************
 *
 *	d e f i n e _ s h a d o w
 *
 **************************************
 *
 * Functional description
 *	Define a shadow file to the database.
 *	Parse it as a set of normal file additions,
 *	setting the shadow number on all files.
 *
 **************************************/
	FIL shadow = (FIL) DDL_alloc(sizeof(fil));

/* get the shadow number, defaulting to 1 */

	int number;
	if (dudleyGlob.DDL_token.tok_type != tok_number)
		number = 1;
	else {
		number = PARSE_number();
		if (number < 1)
			PARSE_error(139, 0, 0);	/* msg 139: shadow number must be a positive integer */
	}

/* match the keywords MANUAL or AUTO to imply whether the shadow
   should be automatically deleted when something goes awry */

	if (PARSE_match(KW_MANUAL))
		shadow->fil_manual = 1;
	else
		PARSE_match(KW_AUTO);

	if (PARSE_match(KW_CONDITIONAL))
		shadow->fil_conditional = 1;

	shadow->fil_name = PARSE_symbol(tok_quoted);
	if (!check_filename(shadow->fil_name, false))
		PARSE_error(297, 0, 0);
	// msg 297: A node name is not permitted in a shadow or secondary file name

	if (PARSE_match(KW_LENGTH)) {
		shadow->fil_length = PARSE_number();
		PARSE_match(KW_PAGES);
	}

	while (true) {
		if (PARSE_match(KW_FILE)) {
			FIL file = define_file();
			file->fil_next = shadow;
			shadow = file;
		}
		else
			break;
	}

	parse_end();

	for (FIL file = shadow; file; file = file->fil_next)
		file->fil_shadow_number = number;

	make_action(act_a_shadow, (DBB) shadow);
}


static void define_trigger()
{
/**************************************
 *
 *	d e f i n e _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse new trigger definition
 *      syntax, being a little loose about
 *	what comes when.
 *
 **************************************/
	if (PARSE_match(KW_FOR)) {
		define_old_trigger();
		return;
	}

	DUDLEY_TRG trigger = (DUDLEY_TRG) DDL_alloc(sizeof(dudley_trg));
	trigger->trg_name = PARSE_symbol(tok_ident);

	PARSE_match(KW_FOR);

	trigger->trg_relation = PARSE_relation();

	int flags = 0, trg_state = 0, trg_sequence = 0;
	get_trigger_attributes(&flags, &trg_state, &trg_sequence);
	trigger->trg_type = trig_table[trg_state & ~trig_inact];
	trigger->trg_inactive = (trg_state & trig_inact);
	trigger->trg_sequence = trg_sequence;

	if (!(int) trigger->trg_type)	/* still none */
		PARSE_error(141, dudleyGlob.DDL_token.tok_string, 0);
		/* msg 141: expected STORE, MODIFY, ERASE, encountered \"%s\" */

	bool action = false;
	bool end = false;

	while (!(dudleyGlob.DDL_token.tok_keyword == KW_SEMI)) {
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			trigger->trg_description = parse_description();
		else if (PARSE_match(KW_END_TRIGGER))
			action = end = true;
		else if (!action) {
			trigger->trg_source = start_text();
			trigger->trg_statement = EXPR_statement();
			end_text(trigger->trg_source);
			action = true;
		}
		else if (PARSE_match(KW_MESSAGE)) {
			TRGMSG trigmsg = (TRGMSG) DDL_alloc(sizeof(trgmsg));
			trigmsg->trgmsg_trg_name = trigger->trg_name;
			trigmsg->trgmsg_number = PARSE_number();
			if (trigmsg->trgmsg_number > 255)
				PARSE_error(142, trigmsg->trgmsg_number);
			/* msg 142: message number %d exceeds 255 */
			PARSE_match(KW_COLON);
			trigmsg->trgmsg_text = PARSE_symbol(tok_quoted);
			make_action(act_a_trigger_msg, (DBB) trigmsg);
			PARSE_match(KW_COMMA);
		}
		else {
			/* if none of the other cases were true, we must be stuck on a bum token */
			if (end)
				PARSE_error(304, dudleyGlob.DDL_token.tok_string, 0);
			/* msg 304: expected message keyword, encountered \"%s\" */
			else if (!action)
				PARSE_error(305, dudleyGlob.DDL_token.tok_string, 0);
			/* msg 305: expected trigger action, encountered \"%s\" */
			else
				PARSE_error(306, dudleyGlob.DDL_token.tok_string, 0);
			/* msg 306: expected end_trigger or description keyword, encountered \"%s\" */
			break;
		}
	}

	parse_end();

	make_action(act_a_trigger, (DBB) trigger);
	trigger->trg_name->sym_type = SYM_trigger;
	trigger->trg_name->sym_object = (DUDLEY_CTX) trigger;
	HSH_insert(trigger->trg_name);
}


static void define_type()
{
/**************************************
 *
 *	d e f i n e _ t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a type for a field.
 *
 **************************************/
	PARSE_match(KW_FOR);
	SYM fldname = PARSE_symbol(tok_ident);

	while (true) {
		TYP fldtype = (TYP) DDL_alloc(sizeof(typ));
		fldtype->typ_field_name = fldname;
		fldtype->typ_name = PARSE_symbol(tok_ident);
		PARSE_match(KW_COLON);
		fldtype->typ_type = PARSE_number();
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			fldtype->typ_description = parse_description();
		make_action(act_a_type, (DBB) fldtype);
		if (!PARSE_match(KW_COMMA))
			break;
	}

	parse_end();
}


static void define_view()
{
/**************************************
 *
 *	d e f i n e _ v i e w
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE VIEW statement.
 *
 **************************************/

/* Pick up relation name */

	DUDLEY_REL relation = PARSE_relation();
	if (!(relation->rel_flags & rel_marked_for_delete) &&
		((relation->rel_flags & rel_marked_for_creation) || EXE_relation(relation)))
	{
		PARSE_error(300, relation->rel_name->sym_string, 0);
		// msg 300: relation %s already exists
	}

	PARSE_match(KW_OF);

/* Parse record selection expression */

	dudley_lls* contexts = NULL;
	relation->rel_view_source = start_text();
	relation->rel_rse = EXPR_rse(true);
	end_text(relation->rel_view_source);

/* add my context to the context stack */

	contexts = (dudley_lls*) relation->rel_rse->nod_arg[s_rse_contexts];
	DUDLEY_CTX my_context = make_context(0, relation);
	LLS_PUSH((DUDLEY_NOD) my_context, &contexts);
	relation->rel_rse->nod_arg[s_rse_contexts] = (DUDLEY_NOD) contexts;

	ACT action = make_action(act_a_relation, (DBB) relation);
	ACT rel_actions = action;
	action->act_flags |= ACT_ignore;

/* Pick up various fields and clauses */

	while (true) {
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			relation->rel_description = parse_description();
		else if (PARSE_match(KW_SECURITY_CLASS))
			relation->rel_security_class = PARSE_symbol(tok_ident);
		else if (PARSE_match(KW_SYSTEM_FLAG)) {
			relation->rel_system = get_system_flag();
			relation->rel_flags |= rel_explicit_system;
		}
		else
			break;
	}

/* Gobble fields */

	SSHORT position = 1;
	DUDLEY_FLD* ptr = &relation->rel_fields;

	while (true) {
		PARSE_match(KW_ADD);
		PARSE_match(KW_FIELD);
		DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
		field->fld_flags |= fld_local;
		field->fld_relation = relation;
		*ptr = field;
		ptr = &field->fld_next;
		SYM symbol = PARSE_symbol(tok_ident);
		field->fld_name = symbol;
		DUDLEY_CTX context = lookup_context(symbol, contexts);
		if (context) {
			if (!PARSE_match(KW_DOT))
				PARSE_error(144, dudleyGlob.DDL_token.tok_string, 0);	/* msg 144: expected period, encountered \"%s\" */
			field->fld_name = field->fld_base = PARSE_symbol(tok_ident);
		}
		else {
			if (PARSE_match(KW_FROM)) {
				symbol = PARSE_symbol(tok_ident);
				if (!(context = lookup_context(symbol, contexts)))
					PARSE_error(145, dudleyGlob.DDL_token.tok_string, 0);
				/* msg 145: expected qualified field name, encountered \"%s\" */
				if (!PARSE_match(KW_DOT))
					PARSE_error(146, dudleyGlob.DDL_token.tok_string, 0);
				/* msg 146: expected period, encountered \"%s\" */
				field->fld_base = PARSE_symbol(tok_ident);
			}
			else {
				parse_field_dtype(field);
				if (field->fld_dtype == blr_cstring)
					PARSE_error(147, 0, 0);	/* msg 147: datatype cstring not supported for fields  */

				if (PARSE_match(KW_COMPUTED)) {
					PARSE_match(KW_BY);
					if (!PARSE_match(KW_LEFT_PAREN))
						PARSE_error(148, 0, 0);	/* msg 148: computed by expression must be parenthesized  */
					field->fld_compute_src = start_text();
					field->fld_computed = EXPR_value(0, NULL);
					end_text(field->fld_compute_src);
					if (!PARSE_match(KW_RIGHT_PAREN))
						PARSE_error(149, 0, 0);	/* msg 149: unmatched parenthesis */
					context = my_context;
				}
				else
					PARSE_error(150, dudleyGlob.DDL_token.tok_string, 0);
				/* msg 150: expected FROM, COMPUTED, or qualified field, encountered \"%s\" */
			}
		}
		field->fld_context = context;
		parse_field_clauses(field);
		if (field->fld_computed)
			make_computed_field(field);
		if (field->fld_flags & fld_explicit_position)
			position = field->fld_position + 1;
		else
			field->fld_position = position++;
		field->fld_flags |= fld_explicit_position;
		field->fld_name->sym_type = SYM_field;
		field->fld_name->sym_object = (DUDLEY_CTX) field;
		HSH_insert(field->fld_name);
		if (!PARSE_match(KW_COMMA))
			break;
	}

	parse_end();

/* We started off by assuming that the relation and field actions that
   have been defined would have to be ignored.  Everything has gone well so
   turn them on. */

	while (true) {
		action->act_flags &= ~ACT_ignore;
		if (action == rel_actions)
			break;
		action = action->act_next;
	}

	HSH_insert(relation->rel_name);
	relation->rel_flags &= ~rel_marked_for_delete;
	relation->rel_flags |= rel_marked_for_creation;
}


static void drop_filter()
{
/**************************************
 *
 *	d r o p _ f i l t e r
 *
 **************************************
 *
 * Functional description
 *	Parse the DROP (DELETE) filter statement.
 *
 **************************************/
	FILTER filter_to_drop = (FILTER) DDL_alloc(sizeof(filter));
	filter_to_drop->filter_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_filter, (DBB) filter_to_drop);
}


static void drop_function()
{
/**************************************
 *
 *	d r o p _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse the DROP (DELETE) function statement.
 *
 **************************************/
	FUNC function = (FUNC) DDL_alloc(sizeof(func));
	function->func_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_function, (DBB) function);
}


static void drop_gfield()
{
/**************************************
 *
 *	d r o p _ g f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse the DROP (DELETE) field statement.
 *
 **************************************/
	DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
	field->fld_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_gfield, (DBB) field);
}


static void drop_index()
{
/**************************************
 *
 *	d r o p _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Parse DROP INDEX statement.
 *
 **************************************/
	DUDLEY_IDX index = (DUDLEY_IDX) DDL_alloc(IDX_LEN(1)); // 0 is invalid
	index->idx_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_index, (DBB) index);
}


static void drop_relation()
{
/**************************************
 *
 *	d r o p _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse DROP RELATION statement.
 *
 **************************************/
	DUDLEY_REL relation = PARSE_relation();
	parse_end();

	if (relation->rel_flags & rel_marked_for_creation)
		PARSE_error(303, 0, 0);	/* msg 303: A relation or view may not be defined and then deleted in a single execution of GDEF */

	relation->rel_flags |= rel_marked_for_delete;

	make_action(act_d_relation, (DBB) relation);
}


static void drop_security_class()
{
/**************************************
 *
 *	d r o p _ s e c u r i t y _ c l a s s
 *
 **************************************
 *
 * Functional description
 *	Parse DROP SECURITY_CLASS statement.
 *
 **************************************/
	SCL sec_class = (SCL) DDL_alloc(sizeof(scl));
	sec_class->scl_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_security, (DBB) sec_class);
}


static void drop_shadow()
{
/**************************************
 *
 *	d r o p _ s h a d o w
 *
 **************************************
 *
 * Functional description
 *	Parse DROP SHADOW statement.
 *
 **************************************/
	const int number = PARSE_number();
	parse_end();

	make_action(act_d_shadow, (DBB)(IPTR) number);
}


static void drop_trigger()
{
/**************************************
 *
 *	d r o p _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse trigger deletion
 *      (old and new trigger syntax).
 *
 **************************************/


	if (PARSE_match(KW_FOR)) {
		DUDLEY_REL relation = PARSE_relation();
		while (!PARSE_match(KW_END_TRIGGER)) {
			DUDLEY_TRG trigger = (DUDLEY_TRG) DDL_alloc(sizeof(dudley_trg));
			if (PARSE_match(KW_STORE))
				trigger->trg_type = trg_store;
			else if (PARSE_match(KW_MODIFY))
				trigger->trg_type = trg_modify;
			else if (PARSE_match(KW_ERASE))
				trigger->trg_type = trg_erase;
			else
				PARSE_error(153, dudleyGlob.DDL_token.tok_string, 0);
			/* msg 153: "expected STORE, MODIFY, ERASE, or END_TRIGGER, encountered \"%s\" */
			trigger->trg_name = gen_trigger_name(trigger->trg_type, relation);
			trigger->trg_relation = relation;
			make_action(act_d_trigger, (DBB) trigger);
		}
	}
	else {
		DUDLEY_TRG trigger = (DUDLEY_TRG) DDL_alloc(sizeof(dudley_trg));
		trigger->trg_name = PARSE_symbol(tok_ident);
		make_action(act_d_trigger, (DBB) trigger);
	}
	parse_end();
}


static void drop_type()
{
/**************************************
 *
 *	d r o p _ t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse the DROP (DELETE) type statement.
 *
 **************************************/
	PARSE_match(KW_FOR);
	if (PARSE_match(KW_ALL)) {
		TYP fldtype = (TYP) DDL_alloc(sizeof(typ));
		fldtype->typ_field_name = PARSE_symbol(tok_ident);
		fldtype->typ_name->sym_length = 3;
		//strncpy(fldtype->typ_name->sym_string, "ALL", 3);
		// Bug: writing in null pointer?
		fldtype->typ_name->sym_string = "ALL";
		make_action(act_d_type, (DBB) fldtype);
	}
	else {
		SYM fldname = PARSE_symbol(tok_ident);
		while (true) {
			TYP fldtype = (TYP) DDL_alloc(sizeof(typ));
			fldtype->typ_field_name = fldname;
			fldtype->typ_name = PARSE_symbol(tok_ident);
			make_action(act_d_type, (DBB) fldtype);
			if (!PARSE_match(KW_COMMA))
				break;
		}
	}
	parse_end();
}


static void end_text( TXT text)
{
/**************************************
 *
 *	e n d _ t e x t
 *
 **************************************
 *
 * Functional description
 *	Mark end of a text description.
 *
 **************************************/

	text->txt_length =
		dudleyGlob.DDL_token.tok_position - dudleyGlob.DDL_token.tok_length - text->txt_position;

#if (defined WIN_NT)
/* the length of the text field should subtract out the
   line feeds, since they are automatically filtered out
   when reading from a file */

	text->txt_length -= (dudleyGlob.DDL_line - text->txt_start_line);
#endif
}


static SYM gen_trigger_name( TRG_T type, DUDLEY_REL relation)
{
/**************************************
 *
 *	g e n _ t r i g g e r _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Generate a trigger name for an old style trigger.
 *
 **************************************/
	SYM symbol = (SYM) DDL_alloc(SYM_LEN + GDS_NAME_LEN);
	symbol->sym_string = symbol->sym_name;
	TEXT* p = symbol->sym_name;
	const TEXT* const end = p + GDS_NAME_LEN;

/* Start by copying relation name */

	const TEXT* q = relation->rel_name->sym_string;

	while (*q && p < end)
		*p++ = *q++;

	switch (type) {
	case trg_store:
		q = "$STORE";
		break;

	case trg_modify:
		q = "$MODIFY";
		break;

	case trg_erase:
		q = "$ERASE";
		break;

	default:
		DDL_err(156);	/* msg 156: gen_trigger_name: invalid trigger type */
	}

	while (*q && p < end)
		*p++ = *q++;

	*p = 0;

	symbol->sym_length = p - symbol->sym_string;
	return symbol;
}


static int get_system_flag()
{
/**************************************
 *
 *	g e t _ s y s t e m _ f l a g
 *
 **************************************
 *
 * Functional description
 *	Return the (signed) numeric system flag value.
 *
 **************************************/
	const SSHORT number = PARSE_number();

	if (number == 1)
		PARSE_error(157, 0, 0);	/* msg 157: System flag value of 1 is reserved for system relations */

	return number;
}


static void get_trigger_attributes( int *flags, int *type, int *sequence)
{
/**************************************
 *
 *	g e t _ t r i g g e r _ a t t r i b u t e s
 *
 **************************************
 *
 * Functional description
 *
 *	trigger attributes include [[PRE | POST]
 *	{STORE | MODIFY | ERASE } [SEQUENCE]].
 *	All are optional on a new style modify,
 *	PRE and POST are optional in new style definitions.
 *	For STORE & MODIFY PRE is the default.
 *	For ERASE, POST is the default.
 *
 **************************************/

	if (PARSE_match(KW_INACTIVE)) {
		*flags |= trg_mflag_onoff;
		*type |= trig_inact;
	}
	else if (PARSE_match(KW_ACTIVE))
		*flags |= trg_mflag_onoff;

	*flags |= trg_mflag_order;
	if (PARSE_match(KW_PRE_STORE))
		*type |= trig_sto;
	else if (PARSE_match(KW_PRE_MODIFY))
		*type |= trig_mod;
	else if (PARSE_match(KW_PRE_ERASE))
		*type |= trig_era;
	else if (!PARSE_match(KW_PRE)) {
		*type |= trig_post;
		if (PARSE_match(KW_POST_STORE))
			*type |= trig_sto;
		else if (PARSE_match(KW_POST_MODIFY))
			*type |= trig_mod;
		else if (PARSE_match(KW_POST_ERASE))
			*type |= trig_era;
		else if (!PARSE_match(KW_POST)) {
			*type &= ~trig_post;
			*flags &= ~trg_mflag_order;
		}
	}

	if (!(*type & ~(trig_inact | trig_post))) {
		if (PARSE_match(KW_STORE))
			*type |= trig_sto;
		else if (PARSE_match(KW_MODIFY))
			*type |= trig_mod;
		else if (PARSE_match(KW_ERASE)) {
			*type |= trig_era;
			if (!(*flags & trg_mflag_order))
				*type |= trig_post;
		}
	}


	if (!PARSE_match(KW_COLON) && ((*flags & trg_mflag_order) ||
							   (*type & (trig_sto | trig_mod | trig_era))))
	{
		*sequence = PARSE_number();
		*flags |= trg_mflag_seqnum;
		PARSE_match(KW_COLON);
	}

	if ((*type & ~trig_inact) || (*flags & trg_mflag_order))
		*flags |= trg_mflag_type;
}


static void grant_user_privilege()
{
/**************************************
 *
 *	g r a n t _ u s e r _ p r i v i l e g e
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL grant statement.
 *
 **************************************/
	USERPRIV upriv = (USERPRIV) DDL_alloc(sizeof(userpriv));
	upriv->userpriv_flags = 0;

	while (true) {
		/* ALL is translated to mean four individual privileges */

		if (PARSE_match(KW_ALL)) {
			/* optional keyword following ALL */

			PARSE_match(KW_PRIVILEGES);
			upriv->userpriv_flags |= USERPRIV_select;
			upriv->userpriv_flags |= USERPRIV_delete;
			upriv->userpriv_flags |= USERPRIV_insert;
			upriv->userpriv_flags |= USERPRIV_update;
			if (!PARSE_match(KW_ON))
				PARSE_error(159, dudleyGlob.DDL_token.tok_string, 0);	/* msg 159: expected ON, encountered \"%s\" */

			break;
		}
		else if (PARSE_match(KW_SELECT))
			upriv->userpriv_flags |= USERPRIV_select;
		else if (PARSE_match(KW_DELETE))
			upriv->userpriv_flags |= USERPRIV_delete;
		else if (PARSE_match(KW_INSERT))
			upriv->userpriv_flags |= USERPRIV_insert;
		else if (PARSE_match(KW_UPDATE)) {
			/* look for a field list for the update privilege */

			upriv->userpriv_flags |= USERPRIV_update;
			if (PARSE_match(KW_ON))
				break;
			if (PARSE_match(KW_COMMA))
				continue;
			if (!PARSE_match(KW_LEFT_PAREN))
				PARSE_error(313, dudleyGlob.DDL_token.tok_string, 0);	/* msg 313: expected ON or '(', encountered "%s" */

			do {
				if (dudleyGlob.DDL_token.tok_keyword == KW_SELECT ||
					dudleyGlob.DDL_token.tok_keyword == KW_INSERT ||
					dudleyGlob.DDL_token.tok_keyword == KW_DELETE ||
					dudleyGlob.DDL_token.tok_keyword == KW_UPDATE)
				{
					break;
				}
				UPFE upf = (UPFE) DDL_alloc(sizeof(upfe));
				upf->upfe_fldname = PARSE_symbol(tok_ident);
				upf->upfe_next = upriv->userpriv_upflist;
				upriv->userpriv_upflist = upf;
			} while (PARSE_match(KW_COMMA));

			if (!PARSE_match(KW_RIGHT_PAREN))
				PARSE_error(314, dudleyGlob.DDL_token.tok_string, 0);	/* msg 314: expected ')', encountered "%s" */

			continue;
		}
		if (!PARSE_match(KW_COMMA)) {
			if (!PARSE_match(KW_ON))
				PARSE_error(159, dudleyGlob.DDL_token.tok_string, 0);	/* msg 159: expected ON, encountered \"%s\" */
			break;
		}
	}

	if (!upriv->userpriv_flags)
		PARSE_error(160, 0, 0);	/* msg 160: GRANT privilege was not specified */

	upriv->userpriv_relation = PARSE_symbol(tok_ident);
	if (!PARSE_match(KW_TO))
		PARSE_error(161, dudleyGlob.DDL_token.tok_string, 0);	/* msg 161: expected TO, encountered "%s" */

/* get the userlist */

	while (true) {
		USRE usr = (USRE) DDL_alloc(sizeof(usre));
		usr->usre_name = PARSE_symbol(tok_ident);
		usr->usre_next = upriv->userpriv_userlist;
		upriv->userpriv_userlist = usr;
		if (!PARSE_match(KW_COMMA))
			break;
	}

/* check for the optional WITH GRANT OPTION specification */

	if (PARSE_match(KW_WITH)) {
		if (!PARSE_match(KW_GRANT))
			PARSE_error(162, dudleyGlob.DDL_token.tok_string, 0);	/* msg 162:expected GRANT, encountered \"%s\" */
		if (!PARSE_match(KW_OPTION))
			PARSE_error(163, dudleyGlob.DDL_token.tok_string, 0);	/* msg 163: expected OPTION, encountered \"%s\" */
		upriv->userpriv_flags |= USERPRIV_grant;
	}

	parse_end();

	make_action(act_grant, (DBB) upriv);
}


static DUDLEY_CTX lookup_context( SYM symbol, dudley_lls* contexts)
{
/**************************************
 *
 *	l o o k u p _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Search the context stack.  If a context name is given, consider
 *	only named contexts; otherwise return the first unnamed context.
 *	In either case, if nothing matches, return NULL.
 *
 **************************************/

/* If no name is given, look for a nameless one. */

	if (!symbol) {
		for (; contexts; contexts = contexts->lls_next) {
			DUDLEY_CTX context = (DUDLEY_CTX) contexts->lls_object;
			if (!context->ctx_name && !context->ctx_view_rse)
				return context;
		}
		return NULL;
	}

/* Other search by name */

	for (; contexts; contexts = contexts->lls_next) {
		DUDLEY_CTX context = (DUDLEY_CTX) contexts->lls_object;
		SYM name = context->ctx_name;
		if (name && !strcmp(name->sym_string, symbol->sym_string))
			return context;
	}

	return NULL;
}


static DUDLEY_FLD lookup_global_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	l o o k u p _ g l o b a l _ f i e l d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

/* Find symbol */

	SYM name = (field->fld_source) ? field->fld_source : field->fld_name;

	SYM symbol = HSH_typed_lookup(name->sym_string, name->sym_length,
								  SYM_global);
	if (symbol)
		return (DUDLEY_FLD) symbol->sym_object;

	PARSE_error(230, name->sym_string, 0);	/* msg 230: global field %s isn't defined */

	return NULL;
}


static ACT make_action( enum act_t type, DBB object)
{
/**************************************
 *
 *	m a k e _ a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Allocate and link an action block.
 *
 **************************************/
	ACT action = (ACT) DDL_alloc(sizeof(act));
	action->act_type = type;
	action->act_next = dudleyGlob.DDL_actions;
	action->act_object = object;
	action->act_line = dudleyGlob.DDL_line;
	dudleyGlob.DDL_actions = action;

	return action;
}


static ACT make_computed_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	m a k e _ c o m p u t e d _ f i e l d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	TEXT s[64];

/* Start by generating a unique name */

	USHORT l;
	for (USHORT i = 1;; i++) {
		sprintf(s, "RDB$%d", i);
		l = strlen(s);
		if (!HSH_lookup(s, l))
			break;
	}

/* Make up new symbol block */

	SYM symbol = (SYM) DDL_alloc(SYM_LEN + l);
	field->fld_source = symbol;
	symbol->sym_type = SYM_global;
	symbol->sym_length = l;
	symbol->sym_string = symbol->sym_name;
	strcpy(symbol->sym_name, s);

	DUDLEY_FLD computed = create_global_field(field);
	field->fld_source_field = computed;
	symbol->sym_object = (DUDLEY_CTX) computed;
	return make_global_field(computed);
}


static DUDLEY_CTX make_context( TEXT * string, DUDLEY_REL relation)
{
/**************************************
 *
 *	m a k e _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Make context for trigger.
 *
 **************************************/
	DUDLEY_CTX context = (DUDLEY_CTX) DDL_alloc(sizeof(dudley_ctx));
	context->ctx_relation = relation;

	if (string) {
		SYM symbol = (SYM) DDL_alloc(SYM_LEN);
		context->ctx_name = symbol;
		symbol->sym_object = context;
		symbol->sym_string = string;
		symbol->sym_length = strlen(string);
		symbol->sym_type = SYM_context;
	}

	return context;
}


static ACT make_global_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	m a k e _ g l o b a l _ f i e l d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

/* Make sure symbol is unique */

	SYM symbol = field->fld_name;
	symbol->sym_object = (DUDLEY_CTX) field;
	symbol->sym_type = SYM_global;

	if (symbol = HSH_typed_lookup(symbol->sym_string,
								  symbol->sym_length,
								  SYM_global))
	{
		PARSE_error(164, symbol->sym_string, 0);	/* msg 164: global field %s already exists */
	}

	HSH_insert(field->fld_name);

	return make_action(act_a_gfield, (DBB) field);
}


static void mod_old_trigger()
{
/**************************************
 *
 *	m o d _ o l d  _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse and old style modify trigger.
 *      (first part was done earlier)
 *
 **************************************/
	DUDLEY_REL relation = PARSE_relation();

	while (!PARSE_match(KW_END_TRIGGER)) {
		int flags = 0, type = 0, sequence = 0;
		get_trigger_attributes(&flags, &type, &sequence);
		if (!type)
			PARSE_error(165, dudleyGlob.DDL_token.tok_string, 0);	/* msg 165: expected STORE, MODIFY, ERASE, END_TRIGGER, encountered \"%s\"  */
		DUDLEY_TRG trigger = (DUDLEY_TRG) DDL_alloc(sizeof(dudley_trg));
		trigger->trg_relation = relation;
		trigger->trg_mflag = flags & ~trg_mflag_order;
		if (trigger->trg_mflag & trg_mflag_type)
			trigger->trg_type = trig_table[type & ~trig_inact];
		SYM name = gen_trigger_name(trigger->trg_type, relation);
		trigger->trg_name = name;
		SYM symbol = HSH_typed_lookup(name->sym_string, name->sym_length,
							  SYM_trigger);
		if (!symbol)
			PARSE_error(166, name->sym_string, 0);	/* msg 166: Trigger %s does not exist */
		modify_trigger_action(trigger, relation);
		make_action(act_m_trigger, (DBB) trigger);
	}
	parse_end();
}


static void modify_field()
{
/**************************************
 *
 *	m o d i f y _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a MODIFY FIELD statement.
 *
 **************************************/
	if (!local_context)
		LLS_PUSH((DUDLEY_NOD) DDL_alloc(sizeof(dudley_ctx)), &local_context);

/* Lookup global field */

	SYM symbol;
	for (symbol = dudleyGlob.DDL_token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_global)
			break;

	if (!symbol)
		PARSE_error(167, dudleyGlob.DDL_token.tok_string, 0);	/*msg 167: expected global field name, encountered \"%s\" */
	DUDLEY_FLD field = (DUDLEY_FLD) symbol->sym_object;
	LEX_token();
	field->fld_flags |= fld_modify;

	parse_field_dtype(field);
	if (field->fld_dtype == blr_cstring)
		PARSE_error(168, 0, 0);	/*msg 168: datatype cstring not supported for fields */
	parse_field_clauses(field);

	if (field->fld_computed)
		PARSE_error(169, 0, 0);	/* msg 169: A computed expression can not be changed or added */

	if (field->fld_security_class)
		PARSE_error(170, 0, 0);	/* msg 170: Security class can appear only on local field references */

	parse_end();

	make_action(act_m_gfield, (DBB) field);
}


static void modify_index()
{
/**************************************
 *
 *	m o d i f y _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Alter an index.  Does not currently
 *	allow adding or dropping fields from
 *	an index
 *
 **************************************/
	DUDLEY_IDX index = (DUDLEY_IDX) DDL_alloc(IDX_LEN(1)); // 0 is invalid
	index->idx_name = PARSE_symbol(tok_ident);

	while (true) {
		if (PARSE_match(KW_DUPLICATES)) {
			index->idx_unique = false;
			index->idx_flags |= IDX_unique_flag;
		}
		else if (PARSE_match(KW_UNIQUE)) {
			index->idx_unique = true;
			index->idx_flags |= IDX_unique_flag;
		}
		else if (PARSE_match(KW_ACTIVE)) {
			index->idx_inactive = false;
			index->idx_flags |= IDX_active_flag;
		}
		else if (PARSE_match(KW_INACTIVE)) {
			index->idx_inactive = true;
			index->idx_flags |= IDX_active_flag;
		}
		else if (PARSE_match(KW_ASCENDING)) {
			index->idx_type = IDX_type_none;
			index->idx_flags |= IDX_type_flag;
		}
		else if (PARSE_match(KW_DESCENDING)) {
			index->idx_type = IDX_type_descend;
			index->idx_flags |= IDX_type_flag;
		}
		else if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION) {
			index->idx_description = parse_description();
		}
		else if (PARSE_match(KW_DROP)) {
			if (PARSE_match(KW_DESCRIP))
				index->idx_flags |= IDX_null_description;
			else
				PARSE_error(172, dudleyGlob.DDL_token.tok_string, 0);	/*msg 172: expected DESCRIPTION, encountered \"%s\" */
		}
		else if (PARSE_match(KW_STATISTICS))
			index->idx_flags |= IDX_statistics_flag;
		else
			break;
	}

	make_action(act_m_index, (DBB) index);
}


static void modify_relation()
{
/**************************************
 *
 *	m o d i f y _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a MODIFY RELATION statement.
 *
 **************************************/
	DUDLEY_REL relation = PARSE_relation();
	make_action(act_m_relation, (DBB) relation);

	if (PARSE_match(KW_EXTERNAL_FILE)) {
		relation->rel_filename = PARSE_symbol(tok_quoted);
		if (!check_filename(relation->rel_filename, true))
			PARSE_error(298, 0, 0);	/* msg 298: A non-Decnet node name is not permitted in an external file name */
	}

	bool modify_relation = false;

	while (true) {
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION) {
			relation->rel_description = parse_description();
			modify_relation = true;
		}
		else if (PARSE_match(KW_SECURITY_CLASS))
		 {
			modify_relation = true;
			relation->rel_security_class = PARSE_symbol(tok_ident);
		}
		else if (PARSE_match(KW_SYSTEM_FLAG)) {
			relation->rel_system = get_system_flag();
			relation->rel_flags |= rel_explicit_system;
			modify_relation = true;
		}
		else
			break;
	}

/* Act on field actions */

	if (!(dudleyGlob.DDL_token.tok_keyword == KW_SEMI))
		while (true) {
			if (PARSE_match(KW_ADD)) {
				PARSE_match(KW_FIELD);
				{
					DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
					parse_field(field);
					field->fld_relation = relation;
					field->fld_database = dudleyGlob.database;
					DUDLEY_FLD global;
					if (field->fld_computed)
						make_computed_field(field);
					else if (!(field->fld_flags & fld_local))
						make_global_field(create_global_field(field));
					else if (global = lookup_global_field(field)) {
						field->fld_dtype = global->fld_dtype;
						field->fld_length = global->fld_length;
						field->fld_scale = global->fld_scale;
						field->fld_segment_length = global->fld_segment_length;
						field->fld_sub_type = global->fld_sub_type;
						field->fld_has_sub_type = global->fld_has_sub_type;
					}
					if (!(field->fld_flags & fld_explicit_position))
						field->fld_position = ++relation->rel_field_position;
					field->fld_name->sym_type = SYM_field;
					HSH_insert(field->fld_name);
					make_action(act_a_field, (DBB) field);
				}
			}
			else if (PARSE_match(KW_MODIFY)) {
				PARSE_match(KW_FIELD);
				DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
				field->fld_flags |= (fld_modify | fld_local);
				parse_field(field);
				field->fld_relation = relation;
				field->fld_database = dudleyGlob.database;
				if (field->fld_computed)
					PARSE_error(173, 0, 0);	/* msg 173: A computed expression can not be changed or added */
				make_action(act_m_field, (DBB) field);
			}
			else if (PARSE_match(KW_DROP)) {
				if (PARSE_match(KW_SECURITY_CLASS)) {
					relation->rel_flags |= rel_null_security_class;
					modify_relation = true;
					PARSE_match(KW_COMMA);
					if (dudleyGlob.DDL_token.tok_keyword == KW_SEMI)
						break;
					else
						continue;
				}
				else if (PARSE_match(KW_DESCRIP)) {
					modify_relation = true;
					relation->rel_flags |= rel_null_description;
					PARSE_match(KW_COMMA);
					if (dudleyGlob.DDL_token.tok_keyword == KW_SEMI)
						break;
					else
						continue;
				}
				else {
					PARSE_match(KW_FIELD);
					DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
					field->fld_flags |= fld_local;
					field->fld_relation = relation;
					field->fld_database = dudleyGlob.database;
					field->fld_name = PARSE_symbol(tok_ident);
					make_action(act_d_field, (DBB) field);
				}
			}
			else
				PARSE_error(174, dudleyGlob.DDL_token.tok_string, 0);	/* msg 174: expected field action, encountered \"%s\" */
			if (!PARSE_match(KW_COMMA))
				break;
		}

	parse_end();

	if (modify_relation)
		relation->rel_flags |= rel_marked_for_modify;
}


static void modify_security_class()
{
/**************************************
 *
 *	m o d i f y _ s e c u r i t y _ c l a s s
 *
 **************************************
 *
 * Functional description
 *	Modify an existing security class.
 *
 **************************************/

/* return error msg for now until fully coded */

	PARSE_error(247, 0, 0);		/* msg 247: action not implemented yet */

	SCL sec_class = (SCL) DDL_alloc(sizeof(scl));
	sec_class->scl_name = PARSE_symbol(tok_ident);
	if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
		sec_class->scl_description = parse_description();

	while (true) {
		SCE element = parse_identifier();
		if (!element)
			return;
		const USHORT score = score_entry(element);
		for (SCE* next = &sec_class->scl_entries;; next = &(*next)->sce_next)
			if (!*next || score > score_entry(*next)) {
				element->sce_next = *next;
				*next = element;
				break;
			}
		element->sce_privileges = parse_privileges();
		if (!PARSE_match(KW_COMMA))
			break;
	}

	parse_end();
	make_action(act_m_security, (DBB) sec_class);
}


static void modify_trigger()
{
/**************************************
 *
 *	m o d i f y _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse new trigger modification
 *      (go elsewhere if this is an old trigger).
 *
 **************************************/
	TRGMSG_T msg_type = trgmsg_none;

	if (PARSE_match(KW_FOR)) {		/* modify trigger for ... is the old syntax */
		mod_old_trigger();
		return;
	}

/* lookup the trigger name, complain and quit if it's unknown */

	SYM name;
	for (name = dudleyGlob.DDL_token.tok_symbol;
		 name && (name->sym_type != SYM_trigger); name = name->sym_homonym);
	{
		if (!name)
			PARSE_error(176, dudleyGlob.DDL_token.tok_string, 0);
		// msg 176: expected trigger name, encountered \"%s\"
	}
	DUDLEY_TRG trigger = (DUDLEY_TRG) name->sym_object;
	LEX_token();

/* in case somebody compulsive specifies the relation name */

	DUDLEY_REL relation;
	if (PARSE_match(KW_FOR)) {
		relation = PARSE_relation();
		if (relation != trigger->trg_relation)
			PARSE_error(177, 0, 0);
		/* msg 177: Unsuccesful attempt to modify trigger relation */
	}
	else
		relation = trigger->trg_relation;

	SLONG flags = 0;
	SLONG type = 0;
	SLONG sequence = 0;
	get_trigger_attributes((int*) &flags, (int*) &type, (int*) &sequence);

	bool action = false;
	bool end = false;

	while (!(dudleyGlob.DDL_token.tok_keyword == KW_SEMI)) {
		if ((PARSE_match(KW_MESSAGE)) || (PARSE_match(KW_MSGADD)) ||
			(PARSE_match(KW_MSGMODIFY)))
		{
			msg_type = trgmsg_modify;
		}
		else if ((PARSE_match(KW_MSGDROP)) || (PARSE_match(KW_DROP)))
		{
			PARSE_match(KW_MESSAGE);
			msg_type = trgmsg_drop;
		}
		if (msg_type) {
			TRGMSG trigmsg = (TRGMSG) DDL_alloc(sizeof(trgmsg));
			trigmsg->trgmsg_trg_name = trigger->trg_name;
			trigmsg->trgmsg_number = PARSE_number();
			if (trigmsg->trgmsg_number > 255)
				PARSE_error(178, trigmsg->trgmsg_number, 0);
			/* msg 178: message number %d exceeds 255 */
			if (msg_type == trgmsg_drop)
				make_action(act_d_trigger_msg, (DBB) trigmsg);
			else if (msg_type == trgmsg_modify) {
				PARSE_match(KW_COLON);
				trigmsg->trgmsg_text = PARSE_symbol(tok_quoted);
				make_action(act_m_trigger_msg, (DBB) trigmsg);
			}
			PARSE_match(KW_COMMA);
			msg_type = trgmsg_none;
		}
		else if (PARSE_match(KW_END_TRIGGER))
			end = true;
		else if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			trigger->trg_description = parse_description();
		else if (!action && !end) {
			modify_trigger_action(trigger, relation);
			action = true;
		}
		else
			PARSE_error(179, dudleyGlob.DDL_token.tok_string, 0);
		/* msg 179: expected message modification keyword, encountered \"%s\" */
	}
	if (flags || type || sequence)
		sort_out_attributes(trigger, flags, type, sequence);

	make_action(act_m_trigger, (DBB) trigger);
}


static void modify_trigger_action( DUDLEY_TRG trigger, DUDLEY_REL relation)
{
/**************************************
 *
 *	m o d i f y _ t r i g g e r _ a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a trigger statement.
 *
 **************************************/
	trigger->trg_source = start_text();

	trigger->trg_statement = EXPR_statement();
	end_text(trigger->trg_source);
}


static void modify_type()
{
/**************************************
 *
 *	m o d i f y _ t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a modify of an existing type value for a field.
 *
 **************************************/
	PARSE_match(KW_FOR);
	SYM fldname = PARSE_symbol(tok_ident);
	while (true) {
		TYP fldtype = (TYP) DDL_alloc(sizeof(typ));
		fldtype->typ_field_name = fldname;
		fldtype->typ_name = PARSE_symbol(tok_ident);
		PARSE_match(KW_COLON);
		fldtype->typ_type = PARSE_number();
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION)
			fldtype->typ_description = parse_description();
		make_action(act_m_type, (DBB) fldtype);
		if (!PARSE_match(KW_COMMA))
			break;
	}
	parse_end();
}


static void modify_view()
{
/**************************************
 *
 *	m o d i f y _ v i e w
 *
 **************************************
 *
 * Functional description
 *	Parse a MODIFY VIEW  statement.
 *
 **************************************/
	bool view_modify = false;

	DUDLEY_REL relation = PARSE_relation();
	make_action(act_m_relation, (DBB) relation);

	while (true) {
		if (dudleyGlob.DDL_token.tok_keyword == KW_DESCRIPTION) {
			relation->rel_description = parse_description();
			view_modify = true;
		}
		else if (PARSE_match(KW_SECURITY_CLASS)) {
			relation->rel_security_class = PARSE_symbol(tok_ident);
			view_modify = true;
		}
		else if (PARSE_match(KW_SYSTEM_FLAG)) {
			relation->rel_system = get_system_flag();
			relation->rel_flags |= rel_explicit_system;
			view_modify = true;
		}
		else
			break;
	}

/* Act on field actions */

	if (!(dudleyGlob.DDL_token.tok_keyword == KW_SEMI))
		while (true) {
			if (PARSE_match(KW_MODIFY)) {
				PARSE_match(KW_FIELD);
				DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
				field->fld_flags |= (fld_modify | fld_local);
				parse_field(field);
				field->fld_relation = relation;
				field->fld_database = dudleyGlob.database;
				if (field->fld_computed)
					PARSE_error(181, 0, 0);	/* msg 181: A computed expression can not be changed or added */
				make_action(act_m_field, (DBB) field);
			}
			else if (PARSE_match(KW_DROP)) {
				if (PARSE_match(KW_DESCRIP)) {
					view_modify = true;
					relation->rel_flags |= rel_null_description;

					if (dudleyGlob.DDL_token.tok_keyword == KW_SEMI)
						break;
					else
						continue;
				}
				else if (PARSE_match(KW_SECURITY_CLASS)) {
					view_modify = true;
					relation->rel_flags |= rel_null_security_class;

					if (dudleyGlob.DDL_token.tok_keyword == KW_SEMI)
						break;
					else
						continue;
				}
				else {
					PARSE_match(KW_FIELD);
					DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
					field->fld_flags |= fld_local;
					field->fld_relation = relation;
					field->fld_database = dudleyGlob.database;
					field->fld_name = PARSE_symbol(tok_ident);
					make_action(act_d_field, (DBB) field);
				}
			}
			else
				PARSE_error(182, dudleyGlob.DDL_token.tok_string, 0);
			/* msg 182: expected drop/modify of field or security class, encountered \"%s\" */
			if (!PARSE_match(KW_COMMA))
				break;
		}

	if (view_modify)
		relation->rel_flags |= rel_marked_for_modify;
	parse_end();
}


static bool parse_action()
{
/**************************************
 *
 *	p a r s e _ a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a single action.  If an token flush is required, return true.
 *
 **************************************/

/* Set up an environment to catch syntax errors.  If one occurs, scan looking
   for semi-colon to continue processing. */

	try {

	dudleyGlob.DDL_prompt = PROMPT;
	LEX_token();
	dudleyGlob.DDL_prompt = CONTINUATION;
	if (dudleyGlob.DDL_eof)
		return true;

	if (PARSE_match(KW_DEFINE))
		switch (parse_object()) {
		case obj_database:
			define_database(act_c_database);
			return true;
		case obj_relation:
			define_relation();
			return true;
		case obj_view:
			define_view();
			return true;
		case obj_field:
			define_field();
			return true;
		case obj_index:
			define_index();
			return true;
		case obj_security_class:
			define_security_class();
			return true;
		case obj_trigger:
			define_trigger();
			return true;
		case obj_file:
			define_file();
			return true;
		case obj_function:
			define_function();
			return true;
		case obj_type:
			define_type();
			return true;
		case obj_filter:
			define_filter();
			return true;
		case obj_shadow:
			define_shadow();
			return true;
		case obj_generator:
			define_generator();
			return true;

		default:
			if (dudleyGlob.database) {
				define_relation();
				return true;
			}
			PARSE_error(183, dudleyGlob.DDL_token.tok_string, 0);	/* msg 183: expected object for DEFINE, encountered \"%s\" */
		}
	else if (PARSE_match(KW_MODIFY))
		switch (parse_object()) {
		case obj_database:
			define_database(act_m_database);
			return true;
		case obj_relation:
			modify_relation();
			return true;
		case obj_view:
			modify_view();
			return true;
		case obj_field:
			modify_field();
			return true;
		case obj_index:
			modify_index();
			return true;
		case obj_security_class:
			modify_security_class();
			return true;
		case obj_trigger:
			modify_trigger();
			return true;
		case obj_function:
			PARSE_error(233, 0, 0);	/* msg 233: action not implemented yet */
		case obj_type:
			modify_type();
			return true;
		case obj_filter:
			PARSE_error(231, 0, 0);	/* msg 231: action not implemented yet */
		case obj_shadow:
			PARSE_error(232, 0, 0);	/* msg 232: action not implemented yet */
		default:
			PARSE_error(184, dudleyGlob.DDL_token.tok_string, 0);
			/* msg 184: expected object for MODIFY, encountered \"%s\" */
		}
	else if (PARSE_match(KW_DROP))
		switch (parse_object()) {
		case obj_database:
			define_database(act_d_database);
			return true;
		case obj_relation:
		case obj_view:
			drop_relation();
			return true;
		case obj_field:
			drop_gfield();
			return true;
		case obj_index:
			drop_index();
			return true;
		case obj_security_class:
			drop_security_class();
			return true;
		case obj_trigger:
			drop_trigger();
			return true;
		case obj_function:
			drop_function();
			return true;
		case obj_type:
			drop_type();
			return true;
		case obj_filter:
			drop_filter();
			return true;
		case obj_shadow:
			drop_shadow();
			return true;
		default:
			PARSE_error(185, dudleyGlob.DDL_token.tok_string, 0);
			/* msg 185: expected object for DROP, encountered \"%s\" */
		}
	else if (PARSE_match(KW_GRANT)) {
		grant_user_privilege();
		return true;
	}
	else if (PARSE_match(KW_REVOKE)) {
		revoke_user_privilege();
		return true;
	}
	else if (PARSE_match(KW_SET_GENERATOR)) {
		set_generator();
		return true;
	}
	else if (PARSE_match(KW_SET)) {
		if (!PARSE_match(KW_GENERATOR))
			PARSE_error(318, dudleyGlob.DDL_token.tok_string, 0);	/* msg 318: expected GENERATOR, encountered \"%s\" */
		set_generator();
		return true;
	}
	else if (dudleyGlob.DDL_interactive && dudleyGlob.DDL_token.tok_keyword == KW_EXIT) {
		dudleyGlob.DDL_eof = true;
		return false;
	}
	else if (dudleyGlob.DDL_interactive && dudleyGlob.DDL_token.tok_keyword == KW_QUIT) {
		dudleyGlob.DDL_quit = dudleyGlob.DDL_eof = true;
		return false;
	}

	PARSE_error(186, dudleyGlob.DDL_token.tok_string, 0);	/* msg 186: expected command, encountered \"%s\" */
	return false;

	}	// try
	catch (const Firebird::Exception&) {
		if (dudleyGlob.DDL_interactive)
			LEX_flush();
		else
			while (!dudleyGlob.DDL_eof && !(dudleyGlob.DDL_token.tok_keyword == KW_SEMI))
				LEX_token();
		return true;
	}
}


static void parse_array( DUDLEY_FLD field)
{
/**************************************
 *
 *	p a r s e _ a r r a y
 *
 **************************************
 *
 * Functional description
 *	Parse the multi-dimensional array specification.
 *
 **************************************/
	if (!PARSE_match(KW_LEFT_PAREN))
		return;

/* Pick up ranges */

	SLONG ranges[2 * MAX_DIMENSION];
	SLONG* range = ranges;
	for (;;) {
		++field->fld_dimension;
		range[0] = 1;
		range[1] = PARSE_number();

		if (PARSE_match(KW_COLON)) {
			range[0] = range[1];
			range[1] = PARSE_number();
		}

		if ((range[1] - range[0]) <= 0)
			PARSE_error(188, 0, 0);	/* msg 188: array size must be positive */

		range += 2;

		if (PARSE_match(KW_RIGHT_PAREN))
			break;

		if (!PARSE_match(KW_COMMA))
			PARSE_error(189, dudleyGlob.DDL_token.tok_string, 0);	/* msg 189: expected comma, encountered \"%s\" */
	}

/* Allocate and copy block to hold ranges */

	const SLONG n = field->fld_dimension * sizeof(SLONG) * 2;
	SLONG* ptr = (SLONG*) DDL_alloc(n);
	field->fld_ranges = ptr;

	const SLONG* const end = range;
	for (const SLONG* range2 = ranges; range2 < end;)
		*ptr++ = *range2++;
}


static TXT parse_description()
{
/**************************************
 *
 *	p a r s e _ d e s c r i p t i o n
 *
 **************************************
 *
 * Functional description
 *	Create a text block to hold the pointer and length
 *	of the description of a metadata item.
 *
 **************************************/
	dudleyGlob.DDL_description = true;
	TXT description = start_text();
	description->txt_position = dudleyGlob.DDL_token.tok_position;

	while (!dudleyGlob.DDL_eof && (!(dudleyGlob.DDL_token.tok_keyword == KW_END_DESCRIPTION)))
		LEX_token();

	if (dudleyGlob.DDL_eof)
		return NULL;

	end_text(description);
	PARSE_match(KW_END_DESCRIPTION);
	dudleyGlob.DDL_description = false;

	return description;
}


static void parse_end()
{
/**************************************
 *
 *	p a r s e _ e n d
 *
 **************************************
 *
 * Functional description
 *	Parse the end of command.  Better be a semi-colon.
 *
 **************************************/

	if (!(dudleyGlob.DDL_token.tok_keyword == KW_SEMI))
		PARSE_error(190, dudleyGlob.DDL_token.tok_string, 0);	/* msg 190: expected semi-colon, encountered \"%s\" */
}


static DUDLEY_FLD parse_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	p a r s e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a field definition, complete with data type and clauses.
 *
 **************************************/

	field->fld_name = PARSE_symbol(tok_ident);
	field->fld_name->sym_object = (DUDLEY_CTX) field;

	if (PARSE_match(KW_BASED)) {
		PARSE_match(KW_ON);
		field->fld_source = PARSE_symbol(tok_ident);
		field->fld_flags |= fld_local;
	}
	else {
		parse_field_dtype(field);
		if (field->fld_dtype == blr_cstring)
			PARSE_error(191, 0, 0);	/* msg 191: datatype cstring not supported for fields */
	}

	if (!field->fld_dtype)
		field->fld_flags |= fld_local;

	parse_field_clauses(field);

	if (!(dudleyGlob.DDL_token.tok_keyword == KW_COMMA) && !(dudleyGlob.DDL_token.tok_keyword == KW_SEMI))
		PARSE_error(192, dudleyGlob.DDL_token.tok_string, 0);	/* msg 192: expected field clause, encountered \"%s\" */

	validate_field(field);

	return field;
}



static void parse_field_clauses( DUDLEY_FLD field)
{
/**************************************
 *
 *	p a r s e _ f i e l d _ c l a u s e s
 *
 **************************************
 *
 * Functional description
 *	Parse a field definition, complete with data type and clauses.
 *
 **************************************/

/* Pick up purely optional clauses */

	while (true)
		switch (PARSE_keyword()) {
		case KW_POSITION:
			LEX_token();
			field->fld_position = PARSE_number();
			field->fld_flags |= fld_explicit_position;
			break;

		case KW_SECURITY_CLASS:
			LEX_token();
			field->fld_security_class = PARSE_symbol(tok_ident);
			break;

		case KW_DROP:
			LEX_token();
			if (PARSE_match(KW_SECURITY_CLASS))
				field->fld_flags |= fld_null_security_class;
			else if (PARSE_match(KW_VALID_IF))
				field->fld_flags |= fld_null_validation;
			else if (PARSE_match(KW_DESCRIP))
				field->fld_flags |= fld_null_description;
			else if (PARSE_match(KW_QUERY_NAME))
				field->fld_flags |= fld_null_query_name;
			else if (PARSE_match(KW_QUERY_HEADER))
				field->fld_flags |= fld_null_query_header;
			else if (PARSE_match(KW_EDIT_STRING))
				field->fld_flags |= fld_null_edit_string;
			else if (PARSE_match(KW_MISSING)) {
				PARSE_match(KW_VALUE);
				field->fld_flags |= fld_null_missing_value;
			}
			else
				PARSE_error(193, dudleyGlob.DDL_token.tok_string, 0);
			/* msg 193: expected DESCRIPTION, EDIT_STRING, MISSING VALUE, SECURITY_CLASS or VALID_IF", encountered \"%s\" */
			break;

		case KW_QUERY_NAME:
			LEX_token();
			PARSE_match(KW_IS);
			field->fld_query_name = PARSE_symbol(tok_ident);
			break;

		case KW_EDIT_STRING:
			LEX_token();
			field->fld_edit_string = PARSE_symbol(tok_quoted);
			break;

		case KW_QUERY_HEADER:
			{
				LEX_token();
				PARSE_match(KW_IS);
				dudley_lls* stack = NULL;
				for (;;) {
					LLS_PUSH((DUDLEY_NOD) PARSE_symbol(tok_quoted), &stack);
					if (!PARSE_match(KW_SLASH))
						break;
				}
				field->fld_query_header = PARSE_make_list(stack);
				break;
			}

		case KW_COMPUTED:
			LEX_token();
			PARSE_match(KW_BY);
			if (!PARSE_match(KW_LEFT_PAREN))
				PARSE_error(194, 0, 0);	/* msg 194: computed by expression must be parenthesized */
			field->fld_compute_src = start_text();
			field->fld_computed = EXPR_value(0, NULL);
			end_text(field->fld_compute_src);
			if (!PARSE_match(KW_RIGHT_PAREN))
				PARSE_error(195, 0, 0);	/* msg 195: unmatched parenthesis */
			break;

		case KW_MISSING:
			LEX_token();
			PARSE_match(KW_VALUE);
			PARSE_match(KW_IS);
			field->fld_missing = EXPR_value(0, NULL);
			break;

		case KW_VALID_IF:
			LEX_token();
			PARSE_match(KW_IF);
			if (!PARSE_match(KW_LEFT_PAREN))
				PARSE_error(196, 0, 0);	/* msg 196: validation expression must be parenthesized */
			field->fld_valid_src = start_text();
			field->fld_validation = EXPR_boolean(0);
			end_text(field->fld_valid_src);
			if (!PARSE_match(KW_RIGHT_PAREN))
				PARSE_error(195, 0, 0);	/* msg 195: unmatched parenthesis */
			break;

		case KW_SEGMENT_LENGTH:
			{
				LEX_token();
				PARSE_match(KW_IS);
				const int n = PARSE_number();
				field->fld_segment_length = n;
				if (n <= 0)
					PARSE_error(197, 0, 0);	/* msg 197: segment length must be positive */
				break;
			}

		case KW_SUB_TYPE:
			LEX_token();
			parse_field_subtype(field);
			break;

		case KW_DEFAULT:
			LEX_token();
			PARSE_match(KW_VALUE);
			PARSE_match(KW_IS);
			field->fld_default = EXPR_value(0, NULL);
			break;

		case KW_DESCRIPTION:
			field->fld_description = parse_description();
			break;

		case KW_SYSTEM_FLAG:
			LEX_token();
			field->fld_system = get_system_flag();
			field->fld_flags |= fld_explicit_system;
			break;

		default:
			return;
		}
}


static void parse_field_dtype( DUDLEY_FLD field)
{
/**************************************
 *
 *	p a r s e _ f i e l d _ d t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a field definition, complete with data type and clauses.
 *
 **************************************/
	switch (PARSE_keyword()) {
	case KW_CHAR:
		field->fld_dtype = blr_text;
		break;

	case KW_VARYING:
		field->fld_dtype = blr_varying;
		break;

	case KW_CSTRING:
		field->fld_dtype = blr_cstring;
		break;

	case KW_SHORT:
		field->fld_dtype = blr_short;
		field->fld_length = sizeof(SSHORT);
		break;

	case KW_LONG:
		field->fld_dtype = blr_long;
		field->fld_length = sizeof(SLONG);
		break;

	case KW_QUAD:
		field->fld_dtype = blr_quad;
		field->fld_length = sizeof(ISC_QUAD);
		break;

	case KW_FLOAT:
		field->fld_dtype = blr_float;
		field->fld_length = sizeof(float);
		break;

	case KW_DOUBLE:
		field->fld_dtype = blr_double;
		field->fld_length = sizeof(double);
		break;

	case KW_DATE:
		field->fld_dtype = blr_timestamp;
		field->fld_length = sizeof(ISC_QUAD);
		break;

	case KW_BLOB:
		field->fld_dtype = blr_blob;
		field->fld_length = sizeof(ISC_QUAD);
		break;

	default:
		return;
	}

	LEX_token();

	if (field->fld_dtype == blr_text ||
		field->fld_dtype == blr_varying || field->fld_dtype == blr_cstring)
	{
		if (!PARSE_match(KW_L_BRCKET) && !PARSE_match(KW_LT))
			PARSE_error(200, dudleyGlob.DDL_token.tok_string, 0);	/* msg 200: expected \"[\", encountered \"%s\" */
		const int n = PARSE_number();
		field->fld_length = n;
		if (n <= 0)
			PARSE_error(201, 0, 0);	/* msg 201: character field length must be positive */
		if (!PARSE_match(KW_R_BRCKET) && !PARSE_match(KW_GT))
			PARSE_error(202, dudleyGlob.DDL_token.tok_string, 0);	/* msg 202: expected \"]\", encountered \"%s\" */

		if (PARSE_match(KW_SUB_TYPE))
			parse_field_subtype(field);

	}

	if (field->fld_dtype != blr_blob)
		parse_array(field);

	field->fld_scale = 0;

	if ((field->fld_dtype == blr_short ||
		 field->fld_dtype == blr_long || field->fld_dtype == blr_quad))
	{
		if (PARSE_match(KW_SCALE))
			field->fld_scale = PARSE_number();
	}
}


static void parse_field_subtype( DUDLEY_FLD field)
{
/**************************************
 *
 *	p a r s e _ f i e l d _ s u b t y p e
 *
 **************************************
 *
 * Functional description
 *	Match a datatype sub_type clause.  Set the (fld_has_sub_type) flag
 *	so we know this type is a type set by the user.
 *
 **************************************/

	PARSE_match(KW_IS);
	if (PARSE_match(KW_TEXT) || PARSE_match(KW_FIXED))
		field->fld_sub_type = 1;
	else if (PARSE_match(KW_BLR))
		field->fld_sub_type = 2;
	else if (PARSE_match(KW_ACL))
		field->fld_sub_type = 3;
	else if (PARSE_keyword() == KW_MINUS)
		field->fld_sub_type = PARSE_number();
	else if (dudleyGlob.DDL_token.tok_type != tok_number)
		PARSE_error(198, dudleyGlob.DDL_token.tok_string, 0);	/* msg 198: expected field sub_type, encountered \"%s\" */
	else
		field->fld_sub_type = PARSE_number();

	field->fld_has_sub_type = true;
}


static FUNCARG parse_function_arg( FUNC function, USHORT * position)
{
/**************************************
 *
 *	p a r s e _ f u n c t i o n _ a r g
 *
 **************************************
 *
 * Functional description
 *	Parse a function argument definition, complete with data type.
 *
 **************************************/
	FUNCARG func_arg = (FUNCARG) DDL_alloc(sizeof(funcarg));
	DUDLEY_FLD field = (DUDLEY_FLD) DDL_alloc(sizeof(dudley_fld));
	parse_field_dtype(field);
	func_arg->funcarg_dtype = field->fld_dtype;
	func_arg->funcarg_scale = field->fld_scale;
	func_arg->funcarg_length = field->fld_length;
	func_arg->funcarg_sub_type = field->fld_sub_type;
	func_arg->funcarg_has_sub_type = field->fld_has_sub_type;
	func_arg->funcarg_position = (*position)++;

	PARSE_match(KW_BY);
	LEX_token();

	switch (PARSE_keyword()) {
	case KW_VALUE:
		LEX_token();
		func_arg->funcarg_mechanism = FUNCARG_mechanism_value;
		if (field->fld_dtype == blr_text ||
			field->fld_dtype == blr_varying ||
			field->fld_dtype == blr_cstring ||
			field->fld_dtype == blr_blob || field->fld_dtype == blr_timestamp)
		{
			PARSE_error(203, 0, 0);	/* msg 203: argument mode by value not allowed for this data type */
		}
		break;

	case KW_REFERENCE:
		LEX_token();
		if (field->fld_dtype == blr_blob)
			func_arg->funcarg_mechanism = FUNCARG_mechanism_blob_struc;
		else
			func_arg->funcarg_mechanism = FUNCARG_mechanism_reference;
		break;

	case KW_SCALAR_ARRAY_DESCRIPTOR:
		LEX_token();
		func_arg->funcarg_mechanism = FUNCARG_mechanism_sc_array_desc;
		break;

	default:
		PARSE_error(204, 0, 0);	/* msg 204: argument mode is by value, or by reference */
	}

/* (kw_comma or kw_semi) here means this argument is not a
   return_value or a return_argument in which case it had
   better not be passed by value */

	if (dudleyGlob.DDL_token.tok_keyword == KW_COMMA || dudleyGlob.DDL_token.tok_keyword == KW_SEMI) {
		if (func_arg->funcarg_mechanism == FUNCARG_mechanism_value)
			PARSE_error(293, 0, 0);	/* msg 293: argument mode 'by value' requires a return mode */
	}
	else {
		if (func_arg->funcarg_mechanism == FUNCARG_mechanism_sc_array_desc)
			PARSE_error(295, 0, 0);	/* msg 295: "Functions can't return arrays." */
		switch (PARSE_keyword())
		{
		case KW_RETURN_VALUE:
			--(*position);
			LEX_token();
			function->func_return_arg = func_arg->funcarg_position = 0;
			function->func_return = func_arg;
			break;

		case KW_RETURN_ARGUMENT:
			LEX_token();
			if (func_arg->funcarg_mechanism == FUNCARG_mechanism_value)
				PARSE_error(292, 0, 0);	/* msg 292: argument mode of a return_argument must be 'by reference' */
			function->func_return_arg = func_arg->funcarg_position;
			function->func_return = func_arg;
			break;

		default:
			PARSE_error(205, 0, 0);	/* msg 205: return mode must be return_value or return_argument */
		}
	}

	if (*position > 11)
		PARSE_error(310, 0, 0);	/* msg 310: UDF is limited to 10 parameters */

	return func_arg;
}


static SCE parse_identifier()
{
/**************************************
 *
 *	p a r s e _ i d e n t i f i e r
 *
 **************************************
 *
 * Functional description
 *	Parse the hard part of an access control element.
 *
 **************************************/
	TEXT* idents[10];
	TEXT** s;
	const TEXT* const* end;
	TEXT strings[256];

	const TEXT* q;
	TEXT* p = strings;
	for (s = idents, end = s + 10; s < end; s++)
		*s = NULL;

/* Get explicit identifiers, if any */

	switch (dudleyGlob.DDL_token.tok_keyword) {
	case KW_VIEW:
		LEX_token();
		if (dudleyGlob.DDL_token.tok_type != tok_ident)
			PARSE_error(117, dudleyGlob.DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		idents[id_view] = p;
		q = dudleyGlob.DDL_token.tok_string;
		while (*p++ = *q++);
		LEX_token();
		break;

	case KW_USER:
		LEX_token();
		if (dudleyGlob.DDL_token.tok_type != tok_ident)
			PARSE_error(117, dudleyGlob.DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		idents[id_user] = p;
		q = dudleyGlob.DDL_token.tok_string;
		while (*p++ = *q++);
		LEX_token();
		if (PARSE_match(KW_GROUP)) {
			if (dudleyGlob.DDL_token.tok_type != tok_ident)
				PARSE_error(117, dudleyGlob.DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
			idents[id_group] = p;
			q = dudleyGlob.DDL_token.tok_string;
			while (*p++ = *q++);
			LEX_token();
		}
		break;

	case KW_GROUP:
		LEX_token();
		if (dudleyGlob.DDL_token.tok_type != tok_ident)
			PARSE_error(117, dudleyGlob.DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		idents[id_group] = p;
		q = dudleyGlob.DDL_token.tok_string;
		while (*p++ = *q++);
		LEX_token();
		if (PARSE_match(KW_USER)) {
			if (dudleyGlob.DDL_token.tok_type != tok_ident)
				PARSE_error(117, dudleyGlob.DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
			idents[id_user] = p;
			q = dudleyGlob.DDL_token.tok_string;
			while (*p++ = *q++);
			LEX_token();
		}
		break;

	case KW_L_BRCKET:
	case KW_LT:
		LEX_token();
		if (!PARSE_match(KW_ASTERISK)) {
			if (dudleyGlob.DDL_token.tok_type != tok_number)
				PARSE_error(206, dudleyGlob.DDL_token.tok_string, 0);	/* msg 206: expected number, encountered \"%s\" */
			idents[id_group] = p;
			q = dudleyGlob.DDL_token.tok_string;
			while (*p++ = *q++);
			LEX_token();
		}
		if (!PARSE_match(KW_COMMA))
			PARSE_error(207, dudleyGlob.DDL_token.tok_string, 0);	/* msg 207: expected comma between group and user ids, encountered \"%s\" */
		if (!PARSE_match(KW_ASTERISK)) {
			if (dudleyGlob.DDL_token.tok_type != tok_number)
				PARSE_error(206, dudleyGlob.DDL_token.tok_string, 0);	/* msg 206: expected number, encountered \"%s\" */
			idents[id_user] = p;
			q = dudleyGlob.DDL_token.tok_string;
			while (*p++ = *q++);
			LEX_token();
		}
		if (!(PARSE_match(KW_R_BRCKET) || PARSE_match(KW_GT)))
			PARSE_error(208, dudleyGlob.DDL_token.tok_string, 0);	/* msg 208: expected trailing bracket, encountered \"%s\" */

		break;
	}

/* Build access control element */

	SCE element = (SCE) DDL_alloc(sizeof(sce) + (p - strings));
	p = (TEXT *) element->sce_strings;

	TEXT** s1 = (TEXT **) element->sce_idents;
	for (s = idents, end = s + 10; s < end; s++, s1++)
	{
		if (q = *s) {
			*s1 = p;
			while (*p++ = *q++);
		}
	}

	return element;
}


static OBJ_T parse_object()
{
/**************************************
 *
 *	p a r s e _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Parse on object name returning what we found.
 *
 **************************************/

	if (PARSE_match(KW_DATABASE))
		return obj_database;
	else if (!dudleyGlob.database || !dudleyGlob.database->dbb_handle)
		PARSE_error(209, 0, 0);	/* msg 209: no database declared */

	if (PARSE_match(KW_RELATION))
		return obj_relation;

	if (PARSE_match(KW_FIELD))
		return obj_field;

	if (PARSE_match(KW_INDEX))
		return obj_index;

	if (PARSE_match(KW_VIEW))
		return obj_view;

	if (PARSE_match(KW_SECURITY_CLASS))
		return obj_security_class;

	if (PARSE_match(KW_TRIGGER))
		return obj_trigger;

	if (PARSE_match(KW_FILE))
		return obj_file;

	if (PARSE_match(KW_FUNCTION))
		return obj_function;

	if (PARSE_match(KW_TYPES))
		return obj_type;

	if (PARSE_match(KW_FILTER))
		return obj_filter;

	if (PARSE_match(KW_SHADOW))
		return obj_shadow;

	if (PARSE_match(KW_GENERATOR))
		return obj_generator;

	return obj_none;
}


static int parse_page_size()
{
/**************************************
 *
 *	p a r s e _ p a g e _ s i z e
 *
 **************************************
 *
 * Functional description
 *	parse the page_size clause of a
 *	define database statement
 *
 *
 **************************************/
	PARSE_match(KW_EQUALS);
	int n1, n2;
	n2 = n1 = PARSE_number();

	if (n1 <= 1024)
		n2 = 1024;
	else if (n1 <= 2048)
		n2 = 2048;
	else if (n1 <= 4096)
		n2 = 4096;
	else if (n1 <= 8192)
		n2 = 8192;
	else
		PARSE_error(210, n1, MAX_PAGE_LEN);
	/* msg 210: PAGE_SIZE specified (%d) longer than limit of %d bytes */
	if (n1 != n2)
		DDL_msg_put(211, SafeArg() << n1 << n2);
	/* msg 211: PAGE_SIZE specified (%d) was rounded up to %d bytes\n */

	return n2;
}


static SLONG parse_privileges()
{
/**************************************
 *
 *	p a r s e _ p r i v i l e g e s
 *
 **************************************
 *
 * Functional description
 *	Parse an access control list.
 *
 **************************************/
	SLONG privileges = 0;

	if (!PARSE_match(KW_MINUS)) {
		if (dudleyGlob.DDL_token.tok_type != tok_ident)
			PARSE_error(117, dudleyGlob.DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		const TEXT* p = dudleyGlob.DDL_token.tok_string;
		TEXT c;
		while (c = *p++)
			switch (UPPER(c))
			{
			case 'P':
				privileges |= 1 << priv_protect;
				break;

			case 'G':
				privileges |= 1 << priv_grant;
				break;

			case 'D':
				privileges |= 1 << priv_delete;
				break;

			case 'R':
				privileges |= 1 << priv_read;
				break;

			case 'W':
				privileges |= 1 << priv_write;
				break;

			case 'C':
				privileges |= 1 << priv_control;
				break;

			default:
				{
					char s[2] = { p[-1], '\0' };
					PARSE_error(212, s, 0);
					/* msg 212: Unrecognized privilege \"%c\" or unrecognized identifier */
				}
			}
		LEX_token();
	}

	return privileges;
}


static void revoke_user_privilege()
{
/**************************************
 *
 *	r e v o k e _ u s e r _ p r i v i l e g e
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL grant statement.
 *
 **************************************/
	USERPRIV upriv = (USERPRIV) DDL_alloc(sizeof(userpriv));

	while (true) {
		if (PARSE_match(KW_ALL)) {
			/* optional keyword following ALL */

			PARSE_match(KW_PRIVILEGES);
			upriv->userpriv_flags |= USERPRIV_select;
			upriv->userpriv_flags |= USERPRIV_delete;
			upriv->userpriv_flags |= USERPRIV_insert;
			upriv->userpriv_flags |= USERPRIV_update;
		}
		else if (PARSE_match(KW_SELECT))
			upriv->userpriv_flags |= USERPRIV_select;
		else if (PARSE_match(KW_DELETE))
			upriv->userpriv_flags |= USERPRIV_delete;
		else if (PARSE_match(KW_INSERT))
			upriv->userpriv_flags |= USERPRIV_insert;
		else if (PARSE_match(KW_UPDATE)) {
			/* revoke update privilege applies to all fields in the grant
			   update list */

			upriv->userpriv_flags |= USERPRIV_update;
			if (PARSE_match(KW_ON))
				break;
			if (PARSE_match(KW_COMMA))
				continue;
			if (!PARSE_match(KW_LEFT_PAREN))
				PARSE_error(315, dudleyGlob.DDL_token.tok_string, 0);	/* msg 315: expected ON or '(', encountered "%s" */

			do {
				if (dudleyGlob.DDL_token.tok_keyword == KW_SELECT ||
					dudleyGlob.DDL_token.tok_keyword == KW_INSERT ||
					dudleyGlob.DDL_token.tok_keyword == KW_DELETE ||
					dudleyGlob.DDL_token.tok_keyword == KW_UPDATE)
				{
					break;
				}
				UPFE upf = (UPFE) DDL_alloc(sizeof(upfe));
				upf->upfe_fldname = PARSE_symbol(tok_ident);
				upf->upfe_next = upriv->userpriv_upflist;
				upriv->userpriv_upflist = upf;
			} while (PARSE_match(KW_COMMA));

			if (!PARSE_match(KW_RIGHT_PAREN))
				PARSE_error(316, dudleyGlob.DDL_token.tok_string, 0);	/* msg 316: expected ')', encountered "%s" */

			continue;
		}
		if (!PARSE_match(KW_COMMA)) {
			if (!PARSE_match(KW_ON))
				PARSE_error(214, dudleyGlob.DDL_token.tok_string, 0);	/* msg 214: expected ON, encountered \"%s\" */
			break;
		}
	}	// while (true)

	if (!upriv->userpriv_flags)
		PARSE_error(215, 0, 0);	/* msg 215: REVOKE privilege was not specified */

	upriv->userpriv_relation = PARSE_symbol(tok_ident);
	if (!PARSE_match(KW_FROM))
		PARSE_error(216, dudleyGlob.DDL_token.tok_string, 0);	/* msg 216: expected FROM, encountered \"%s\" */

/* get the userlist */
	while (true) {
		USRE usr = (USRE) DDL_alloc(sizeof(usre));
		usr->usre_name = PARSE_symbol(tok_ident);
		usr->usre_next = upriv->userpriv_userlist;
		upriv->userpriv_userlist = usr;
		if (!PARSE_match(KW_COMMA))
			break;
	}

	parse_end();
	make_action(act_revoke, (DBB) upriv);
}


static SLONG score_entry( SCE element)
{
/**************************************
 *
 *	s c o r e _ e n t r y
 *
 **************************************
 *
 * Functional description
 *	Compute a value to determine placement of an
 *	access control element in an Apollo access
 *	control list.
 *
 **************************************/
	SLONG score = 0;
	if (element->sce_idents[id_view])
		score++;

	const TEXT* const* ptr = (TEXT**) element->sce_idents;
	for (const TEXT* const* const end = ptr + id_max; ptr < end;
		 ptr++)
	{
		score <<= 1;
		if (*ptr)
			score |= 1;
	}

	return score;
}


static DUDLEY_NOD set_generator()
{
/**************************************
 *
 *	s e t _ g e n e r a t o r
 *
 **************************************
 *
 * Functional description
 *      get the name and new value for generator
 **************************************/
	if (dudleyGlob.DDL_token.tok_type != tok_ident)
		PARSE_error(274, dudleyGlob.DDL_token.tok_string, 0);	/* msg 274: expected generator name, encountered \"%s\" */

	DUDLEY_NOD node = PARSE_make_node(nod_set_generator, 2);
	node->nod_count = 1;
	node->nod_arg[1] = (DUDLEY_NOD) PARSE_symbol(tok_ident);
	PARSE_match(KW_TO);
	node->nod_arg[0] = EXPR_value(0, NULL);

	parse_end();
	make_action(act_s_generator, (DBB) node);
	return 0;
}


static void sort_out_attributes(DUDLEY_TRG trigger,
								SLONG flags, SLONG type, SLONG sequence)
{
/**************************************
 *
 *	s o r t _ o u t _ a t t r i b u t e s
 *
 **************************************
 *
 * Functional description
 *	Somebody is being very cute and changing
 *	attributes of a trigger.  We're going to do
 *	it, but we'd better remember the other, unchanged
 *	attributes so we don't make a big mess.
 *
 **************************************/

	trigger->trg_mflag = flags & ~trg_mflag_order;

	if (flags & trg_mflag_type)
		switch (trigger->trg_type) {
		case trg_store:
		case trg_post_store:
			if (!(type & (trig_mod | trig_era)))
				type |= trig_sto;
			else if (!trigger->trg_statement && (type & trig_era))
				PARSE_error(217, 0, 0);	/* msg 217: Attempt change trigger type from STORE to ERASE */
			break;
		case trg_modify:
		case trg_post_modify:
			if (!(type & (trig_sto | trig_era)))
				type |= trig_mod;
			else if (!trigger->trg_statement)
				if (type & trig_sto)
					PARSE_error(218, 0, 0);	/* msg 218: Attempt change trigger type from MODIFY to STORE */
				else
					PARSE_error(219, 0, 0);	/* msg 219: Attempt change trigger type from MODIFY to ERASE */
			break;
		case trg_pre_erase:
		case trg_erase:
			if (!(type & (trig_sto | trig_mod)))
				type |= trig_era;
			else if (!trigger->trg_statement && (type & trig_sto))
				PARSE_error(220, 0, 0);	/* msg 220: Attempt to change trigger type from ERASE to STORE */
		}

	if (!(flags & trg_mflag_order) && (flags & trg_mflag_type))
		switch (trigger->trg_type) {
		case trg_erase:
		case trg_post_modify:
		case trg_post_store:
			type |= trig_post;
			break;
		case trg_store:
		case trg_modify:
		case trg_pre_erase:
			type &= ~trig_post;
		}

	if (trigger->trg_mflag & trg_mflag_type)
		trigger->trg_type = trig_table[type & ~trig_inact];

	if (trigger->trg_mflag & trg_mflag_seqnum)
		trigger->trg_sequence = sequence;

	if (trigger->trg_mflag & trg_mflag_onoff)
		trigger->trg_inactive = type & trig_inact;
}


static TXT start_text()
{
/**************************************
 *
 *	s t a r t _ t e x t
 *
 **************************************
 *
 * Functional description
 *	Make the current position to save description text.
 *
 **************************************/
	TXT text = (TXT) DDL_alloc(sizeof(txt));
	text->txt_position = dudleyGlob.DDL_token.tok_position - dudleyGlob.DDL_token.tok_length;
	text->txt_start_line = dudleyGlob.DDL_line;

	return text;
}


static void validate_field( DUDLEY_FLD field)
{
/**************************************
 *
 *     v a l i d a t e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Validate that the field clauses make sense
 *	together.
 *
 **************************************/
	static const SafeArg dummy;
	TEXT option[128] = "";

	if (field->fld_flags & fld_local) {
		if (field->fld_validation)
			fb_msg_format(0, DDL_MSG_FAC, 221, sizeof(option), option, dummy);
		/* msg 221 /'valid if/' */
		if (field->fld_missing)
			fb_msg_format(0, DDL_MSG_FAC, 222, sizeof(option), option, dummy);
		/* msg 222: missing value */
		if ((field->fld_dtype) && !(field->fld_computed))
			fb_msg_format(0, DDL_MSG_FAC, 223, sizeof(option), option, dummy);
		/* msg 223: data type */
		if (field->fld_has_sub_type && !(field->fld_computed))
			fb_msg_format(0, DDL_MSG_FAC, 224, sizeof(option), option, dummy);
		/* msg 224: sub type */
		if ((field->fld_segment_length) && !(field->fld_computed))
			fb_msg_format(0, DDL_MSG_FAC, 225, sizeof(option), option, dummy);
		/* msg 225: segment_length */
		if (*option)
			PARSE_error(226, option, 0);	/* msg 226: %s is a global, not local, attribute */
		return;
	}

	if (field->fld_computed && !(field->fld_dtype))
		PARSE_error(227, 0, 0);	/* msg 227: computed fields need datatypes */

	if (field->fld_flags & fld_modify)
		return;

	if (field->fld_has_sub_type && (field->fld_dtype != blr_blob) &&
		(field->fld_dtype != blr_text) && (field->fld_dtype != blr_varying))
	{
		PARSE_error(228, 0, 0);	/* msg 228: subtypes are valid only for blobs and text */
	}

	if (field->fld_segment_length && (field->fld_dtype != blr_blob))
		PARSE_error(229, 0, 0);	/* msg 229: segment length is valid only for blobs */

	if ((field->fld_dtype == blr_blob) && !(field->fld_segment_length))
		field->fld_segment_length = 80;
}
