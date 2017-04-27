%{
/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		parse.y
 *	DESCRIPTION:	Dynamic SQL parser
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
 * 2002-02-24 Sean Leyne - Code Cleanup of old Win 3.1 port (WINDOWS_ONLY)
 * 2001.05.20 Neil McCalden: Allow a udf to be used in a 'group by' clause.
 * 2001.05.30 Claudio Valderrama: DROP TABLE and DROP VIEW lead now to two
 *   different node types so DDL can tell which is which.
 * 2001.06.13 Claudio Valderrama: SUBSTRING is being surfaced.
 * 2001.06.30 Claudio valderrama: Feed (line,column) for each node. See node.h.
 * 2001.07.10 Claudio Valderrama: Better (line,column) report and "--" for comments.
 * 2001.07.28 John Bellardo: Changes to support parsing LIMIT and FIRST
 * 2001.08.03 John Bellardo: Finalized syntax for LIMIT, change LIMIT to SKIP
 * 2001.08.05 Claudio Valderrama: closed Bug #448062 and other spaces that appear
 *   in rdb$*_source fields when altering domains plus one unexpected null pointer.
 * 2001.08.12 Claudio Valderrama: adjust SUBSTRING's starting pos argument here
 *   and not in gen.c; this closes Bug #450301.
 * 2001.10.01 Claudio Valderrama: enable explicit GRANT...to ROLE role_name.
 * 2001.10.06 Claudio Valderrama: Honor explicit USER keyword in GRANTs and REVOKEs.
 * 2002.07.05 Mark O'Donohue: change keyword DEBUG to KW_DEBUG to avoid
 *			clashes with normal DEBUG macro.
 * 2002.07.30 Arno Brinkman:
 * 2002.07.30 	Let IN predicate handle value_expressions
 * 2002.07.30 	tokens CASE, NULLIF, COALESCE added
 * 2002.07.30 	See block < CASE expression > what is added to value as case_expression
 * 2002.07.30 	function is split up into aggregate_function, numeric_value_function, string_value_function, generate_value_function
 * 2002.07.30 	new group_by_function and added to grp_column_elem
 * 2002.07.30 	cast removed from function and added as cast_specification to value
 * 2002.08.04 Claudio Valderrama: allow declaring and defining variables at the same time
 * 2002.08.04 Dmitry Yemanov: ALTER VIEW
 * 2002.08.06 Arno Brinkman: ordinal added to grp_column_elem for using positions in group by
 * 2002.08.07 Dmitry Yemanov: INT64/LARGEINT are replaced with BIGINT and available in dialect 3 only
 * 2002.08.31 Dmitry Yemanov: allowed user-defined index names for PK/FK/UK constraints
 * 2002.09.01 Dmitry Yemanov: RECREATE VIEW
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *							exception handling in SPs/triggers,
 *							implemented ROWS_AFFECTED system variable
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2002.12.03 Dmitry Yemanov: Implemented ORDER BY clause in subqueries.
 * 2002.12.18 Dmitry Yemanov: Added support for SQL-compliant labels and LEAVE statement
 * 2002.12.28 Dmitry Yemanov: Added support for parametrized events.
 * 2003.01.14 Dmitry Yemanov: Fixed bug with cursors in triggers.
 * 2003.01.15 Dmitry Yemanov: Added support for runtime trigger action checks.
 * 2003.02.10 Mike Nordell  : Undefined Microsoft introduced macros to get a clean compile.
 * 2003.05.24 Nickolay Samofatov: Make SKIP and FIRST non-reserved keywords
 * 2003.06.13 Nickolay Samofatov: Make INSERTING/UPDATING/DELETING non-reserved keywords
 * 2003.07.01 Blas Rodriguez Somoza: Change DEBUG and IN to avoid conflicts in win32 build/bison
 * 2003.08.11 Arno Brinkman: Changed GROUP BY to support all expressions and added "AS" support
 *						   with table alias. Also removed group_by_function and ordinal.
 * 2003.08.14 Arno Brinkman: Added support for derived tables.
 * 2003.10.05 Dmitry Yemanov: Added support for explicit cursors in PSQL.
 * 2004.01.16 Vlad Horsun: added support for default parameters and
 *   EXECUTE BLOCK statement
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>

#include "gen/iberror.h"
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../jrd/ibase.h"
#include "../jrd/flags.h"
#include "../jrd/jrd.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/keywords.h"
#include "../dsql/misc_func.h"
#include "../jrd/gds_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/intlobj_new.h"
#include "../common/StatusArg.h"

/* since UNIX isn't standard, we have to define
   stuff which is in <limits.h> (which isn't available
   on all UNIXes... */

const long SHRT_POS_MAX			= 32767;
const long SHRT_UNSIGNED_MAX	= 65535;
const long SHRT_NEG_MAX			= 32768;
const long LONG_POS_MAX			= 2147483647;
const int POSITIVE	= 0;
const int NEGATIVE	= 1;
const int UNSIGNED	= 2;

//const int MIN_CACHE_BUFFERS	= 250;
//const int DEF_CACHE_BUFFERS	= 1000;

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE	2048
#define YYMAXDEPTH	2048

/* Make bison allocate static stack */
#define YYINITDEPTH 2048

// Using this option causes build problems on Win32 with bison 1.28
//#define YYSTACK_USE_ALLOCA 1

#define YYSTYPE YYSTYPE
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

#define YYMALLOC gds__alloc
#define YYFREE gds__free

static const char INTERNAL_FIELD_NAME[] = "DSQL internal"; /* NTX: placeholder */

inline SLONG trigger_type_suffix(const int slot1, const int slot2, const int slot3)
{
	return ((slot1 << 1) | (slot2 << 3) | (slot3 << 5));
}


#include "../dsql/chars.h"

const int MAX_TOKEN_LEN = 256;

using namespace Jrd;
using namespace Dsql;
using namespace Firebird;

#ifdef NOT_USED_OR_REPLACED
static bool		long_int(dsql_nod*, SLONG*);
#endif
static dsql_fld*	make_field (dsql_nod*);
static dsql_fil*	make_file();
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(dsql_nod*, SLONG*, SSHORT);
#endif
static void	stack_nodes (dsql_nod*, DsqlNodStack&);
static Firebird::MetaName toName(dsql_nod* node);

static void	yyabandon (SLONG, ISC_STATUS);

inline void check_bound(const char* const to, const char* const string)
{
	if ((to - string) >= MAX_TOKEN_LEN)
		yyabandon (-104, isc_token_too_long);
}

inline void check_copy_incr(char*& to, const char ch, const char* const string)
{
	check_bound(to, string);
	*to++ = ch;
}

%}


/* token declarations */

/* Tokens are organized chronologically by date added.
   See dsql/keywords.cpp for a list organized alphabetically */

/* Tokens in v4.0 -- not separated into v3 and v4 tokens */

%token ACTIVE
%token ADD
%token AFTER
%token ALL
%token ALTER
%token AND
%token ANY
%token AS
%token ASC
%token AT
%token AVG
%token AUTO
%token BEFORE
%token BEGIN
%token BETWEEN
%token BLOB
%token BY
%token CAST
%token CHARACTER
%token CHECK
%token COLLATE
%token COMMA
%token COMMIT
%token COMMITTED
%token COMPUTED
%token CONCATENATE
%token CONDITIONAL
%token CONSTRAINT
%token CONTAINING
%token COUNT
%token CREATE
%token CSTRING
%token CURRENT
%token CURSOR
%token DATABASE
%token DATE
%token DB_KEY
%token DECIMAL
%token DECLARE
%token DEFAULT
%token KW_DELETE
%token DESC
%token DISTINCT
%token DO
%token DOMAIN
%token DROP
%token ELSE
%token END
%token ENTRY_POINT
%token EQL
%token ESCAPE
%token EXCEPTION
%token EXECUTE
%token EXISTS
%token EXIT
%token EXTERNAL
%token FILTER
%token FOR
%token FOREIGN
%token FROM
%token FULL
%token FUNCTION
%token GDSCODE
%token GEQ
%token GENERATOR
%token GEN_ID
%token GRANT
%token GROUP
%token GTR
%token HAVING
%token IF
%token KW_IN
%token INACTIVE
%token INNER
%token INPUT_TYPE
%token INDEX
%token INSERT
%token INTEGER
%token INTO
%token IS
%token ISOLATION
%token JOIN
%token KEY
%token KW_CHAR
%token KW_DEC
%token KW_DOUBLE
%token KW_FILE
%token KW_FLOAT
%token KW_INT
%token KW_LONG
%token KW_NULL
%token KW_NUMERIC
%token KW_UPPER
%token KW_VALUE
%token LENGTH
%token LPAREN
%token LEFT
%token LEQ
%token LEVEL
%token LIKE
%token LSS
%token MANUAL
%token MAXIMUM
%token MAX_SEGMENT
%token MERGE
%token MINIMUM
%token MODULE_NAME
%token NAMES
%token NATIONAL
%token NATURAL
%token NCHAR
%token NEQ
%token NO
%token NOT
%token NOT_GTR
%token NOT_LSS
%token OF
%token ON
%token ONLY
%token OPTION
%token OR
%token ORDER
%token OUTER
%token OUTPUT_TYPE
%token OVERFLOW
%token PAGE
%token PAGES
%token KW_PAGE_SIZE
%token PARAMETER
%token PASSWORD
%token PLAN
%token POSITION
%token POST_EVENT
%token PRECISION
%token PRIMARY
%token PRIVILEGES
%token PROCEDURE
%token PROTECTED
%token READ
%token REAL
%token REFERENCES
%token RESERVING
%token RETAIN
%token RETURNING_VALUES
%token RETURNS
%token REVOKE
%token RIGHT
%token RPAREN
%token ROLLBACK
%token SEGMENT
%token SELECT
%token SET
%token SHADOW
%token KW_SHARED
%token SINGULAR
%token KW_SIZE
%token SMALLINT
%token SNAPSHOT
%token SOME
%token SORT
%token SQLCODE
%token STABILITY
%token STARTING
%token STATISTICS
%token SUB_TYPE
%token SUSPEND
%token SUM
%token TABLE
%token THEN
%token TO
%token TRANSACTION
%token TRIGGER
%token UNCOMMITTED
%token UNION
%token UNIQUE
%token UPDATE
%token USER
%token VALUES
%token VARCHAR
%token VARIABLE
%token VARYING
%token VERSION
%token VIEW
%token WAIT
%token WHEN
%token WHERE
%token WHILE
%token WITH
%token WORK
%token WRITE

%token FLOAT_NUMBER NUMBER NUMERIC SYMBOL STRING INTRODUCER

/* New tokens added v5.0 */

%token ACTION
%token ADMIN
%token CASCADE
%token FREE_IT			/* ISC SQL extension */
%token RESTRICT
%token ROLE

/* New tokens added v6.0 */

%token COLUMN
%token KW_TYPE
%token EXTRACT
%token YEAR
%token MONTH
%token DAY
%token HOUR
%token MINUTE
%token SECOND
%token WEEKDAY			/* ISC SQL extension */
%token YEARDAY			/* ISC SQL extension */
%token TIME
%token TIMESTAMP
%token CURRENT_DATE
%token CURRENT_TIME
%token CURRENT_TIMESTAMP

/* special aggregate token types returned by lex in v6.0 */

%token NUMBER64BIT SCALEDINT

/* CVC: Special Firebird additions. */

%token CURRENT_USER
%token CURRENT_ROLE
%token KW_BREAK
%token SUBSTRING
%token RECREATE
%token KW_DESCRIPTOR
%token FIRST
%token SKIP

/* tokens added for Firebird 1.5 */

%token CURRENT_CONNECTION
%token CURRENT_TRANSACTION
%token BIGINT
%token CASE
%token NULLIF
%token COALESCE
%token USING
%token NULLS
%token LAST
%token ROW_COUNT
%token LOCK
%token SAVEPOINT
%token RELEASE
%token STATEMENT
%token LEAVE
%token INSERTING
%token UPDATING
%token DELETING

/* tokens added for Firebird 2.0 */

%token BACKUP
%token KW_DIFFERENCE
%token OPEN
%token CLOSE
%token FETCH
%token ROWS
%token BLOCK
%token IIF
%token SCALAR_ARRAY
%token CROSS
%token NEXT
%token SEQUENCE
%token RESTART
%token BOTH
%token COLLATION
%token COMMENT
%token BIT_LENGTH
%token CHAR_LENGTH
%token CHARACTER_LENGTH
%token LEADING
%token KW_LOWER
%token OCTET_LENGTH
%token TRAILING
%token TRIM
%token RETURNING
%token KW_IGNORE
%token LIMBO
%token UNDO
%token REQUESTS
%token TIMEOUT

/* tokens added for Firebird 2.1 */

%token ABS
%token ACCENT
%token ACOS
%token ALWAYS
%token ASCII_CHAR
%token ASCII_VAL
%token ASIN
%token ATAN
%token ATAN2
%token BIN_AND
%token BIN_OR
%token BIN_SHL
%token BIN_SHR
%token BIN_XOR
%token CEIL
%token CONNECT
%token COS
%token COSH
%token COT
%token DATEADD
%token DATEDIFF
%token DECODE
%token DISCONNECT
%token EXP
%token FLOOR
%token GEN_UUID
%token GENERATED
%token GLOBAL
%token HASH
%token INSENSITIVE
%token LIST
%token LN
%token LOG
%token LOG10
%token LPAD
%token MATCHED
%token MATCHING
%token MAXVALUE
%token MILLISECOND
%token MINVALUE
%token MOD
%token OVERLAY
%token PAD
%token PI
%token PLACING
%token POWER
%token PRESERVE
%token RAND
%token RECURSIVE
%token REPLACE
%token REVERSE
%token ROUND
%token RPAD
%token SENSITIVE
%token SIGN
%token SIN
%token SINH
%token SPACE
%token SQRT
%token START
%token TAN
%token TANH
%token TEMPORARY
%token TRUNC
%token WEEK

// tokens added for Firebird 2.5

%token AUTONOMOUS
%token CHAR_TO_UUID
%token FIRSTNAME
%token GRANTED
%token LASTNAME
%token MIDDLENAME
%token MAPPING
%token OS_NAME
%token SIMILAR
%token UUID_TO_CHAR

%token DUMP

// new execute statement
%token CALLER
%token COMMON
%token DATA
%token SOURCE
%token TWO_PHASE
%token BIND_PARAM
%token BIN_NOT
%token SQLSTATE

/* precedence declarations for expression evaluation */

%left	OR
%left	AND
%left	NOT
%left	'=' '<' '>' LIKE EQL NEQ GTR LSS GEQ LEQ NOT_GTR NOT_LSS
%left	'+' '-'
%left	'*' '/'
%left	UMINUS UPLUS
%left	CONCATENATE
%left	COLLATE

/* Fix the dangling IF-THEN-ELSE problem */
%nonassoc THEN
%nonassoc ELSE

/* The same issue exists with ALTER COLUMN now that keywords can be used
   in order to change their names.  The syntax which shows the issue is:
	 ALTER COLUMN where column is part of the alter statement
	   or
	 ALTER COLUMN where column is the name of the column in the relation
*/
%nonassoc ALTER
%nonassoc COLUMN

%%

/* list of possible statements */

top		: statement
			{ DSQL_parse = $1; }
		| statement ';'
			{ DSQL_parse = $1; }
		;

statement	: alter
		| blob_io
		| comment
		| commit
		| create
		| create_or_alter
		| declare
		| delete
		| drop
		| grant
		| insert
		| merge
		| exec_procedure
		| exec_block
		| recreate
		| revoke
		| rollback
		| savepoint
		| select
		| set
		| update
		| update_or_insert
		| dump
		;

/* GRANT statement */

grant	: GRANT privileges ON table_noise simple_table_name
			TO non_role_grantee_list grant_option granted_by
			{ $$ = make_node (nod_grant, (int) e_grant_count,
					$2, $5, make_list($7), $8, $9); }
		| GRANT proc_privileges ON PROCEDURE simple_proc_name
			TO non_role_grantee_list grant_option granted_by
			{ $$ = make_node (nod_grant, (int) e_grant_count,
					$2, $5, make_list($7), $8, $9); }
		| GRANT role_name_list TO role_grantee_list role_admin_option granted_by
			{ $$ = make_node (nod_grant, (int) e_grant_count,
					make_list($2), make_list($4), NULL, $5, $6); }
		;

table_noise	: TABLE
		|
		;

privileges	: ALL
			{ $$ = make_node (nod_all, (int) 0, NULL); }
		| ALL PRIVILEGES
			{ $$ = make_node (nod_all, (int) 0, NULL); }
		| privilege_list
			{ $$ = make_list ($1); }
		;

privilege_list	: privilege
		| privilege_list ',' privilege
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

proc_privileges	: EXECUTE
			{ $$ = make_list (make_node (nod_execute, (int) 0, NULL)); }
		;

privilege	: SELECT
			{ $$ = make_node (nod_select, (int) 0, NULL); }
		| INSERT
			{ $$ = make_node (nod_insert, (int) 0, NULL); }
		| KW_DELETE
			{ $$ = make_node (nod_delete, (int) 0, NULL); }
		| UPDATE column_parens_opt
			{ $$ = make_node (nod_update, (int) 1, $2); }
		| REFERENCES column_parens_opt
			{ $$ = make_node (nod_references, (int) 1, $2); }
		;

grant_option	: WITH GRANT OPTION
			{ $$ = make_node (nod_grant, (int) 0, NULL); }
		|
			{ $$ = NULL; }
		;

role_admin_option   : WITH ADMIN OPTION
			{ $$ = make_node (nod_grant_admin, (int) 0, NULL); }
		|
			{ $$ = NULL; }
		;

granted_by	: granted_by_text grantor
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

granted_by_text	: GRANTED BY
		|  AS
		;

grantor		: role_grantee
			{ $$ = $1; }
		;

simple_proc_name: symbol_procedure_name
			{ $$ = make_node (nod_procedure_name, (int) 1, $1); }
		;


/* REVOKE statement */

revoke	: REVOKE rev_grant_option privileges ON table_noise simple_table_name
			FROM non_role_grantee_list granted_by
			{ $$ = make_node (nod_revoke, (int) e_grant_count,
					$3, $6, make_list($8), $2, $9); }
		| REVOKE rev_grant_option proc_privileges ON PROCEDURE simple_proc_name
			FROM non_role_grantee_list granted_by
			{ $$ = make_node (nod_revoke, (int) e_grant_count,
					$3, $6, make_list($8), $2, $9); }
		| REVOKE rev_admin_option role_name_list FROM role_grantee_list granted_by
			{ $$ = make_node (nod_revoke, (int) e_grant_count,
					make_list($3), make_list($5), NULL, $2, $6); }
		| REVOKE ALL ON ALL FROM non_role_grantee_list
			{ $$ = make_node (nod_revoke, (int) e_grant_count,
					NULL, NULL, make_list($6), NULL, NULL); }
		;

rev_grant_option : GRANT OPTION FOR
			{ $$ = make_node (nod_grant, (int) 0, NULL); }
		|
			{ $$ = NULL; }
		;

rev_admin_option : ADMIN OPTION FOR
			{ $$ = make_node (nod_grant_admin, (int) 0, NULL); }
		|
			{ $$ = NULL; }
		;

non_role_grantee_list	: grantee_list
		| user_grantee_list
		;

grantee_list	: grantee
		| grantee_list ',' grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		| grantee_list ',' user_grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		| user_grantee_list ',' grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

grantee : PROCEDURE symbol_procedure_name
		{ $$ = make_node (nod_proc_obj, (int) 1, $2); }
	| TRIGGER symbol_trigger_name
		{ $$ = make_node (nod_trig_obj, (int) 1, $2); }
	| VIEW symbol_view_name
		{ $$ = make_node (nod_view_obj, (int) 1, $2); }
	| ROLE symbol_role_name
			{ $$ = make_node (nod_role_name, (int) 1, $2); }
	;

user_grantee_list : user_grantee
		| user_grantee_list ',' user_grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

/* CVC: In the future we can deprecate the first implicit form since we'll support
explicit grant/revoke for both USER and ROLE keywords & object types. */

user_grantee	: symbol_user_name
		{ $$ = make_node (nod_user_name, (int) 1, $1); }
	| USER symbol_user_name
		{ $$ = make_node (nod_user_name, (int) 2, $2, NULL); }
	| GROUP symbol_user_name
		{ $$ = make_node (nod_user_group, (int) 1, $2); }
	;

role_name_list  : role_name
		| role_name_list ',' role_name
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

role_name   : symbol_role_name
		{ $$ = make_node (nod_role_name, (int) 1, $1); }
		;

role_grantee_list  : role_grantee
		| role_grantee_list ',' role_grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

role_grantee   : symbol_user_name
		{ $$ = make_node (nod_user_name, (int) 1, $1); }
	| USER symbol_user_name
		{ $$ = make_node (nod_user_name, (int) 1, $2); }
		;


/* DECLARE operations */

declare		: DECLARE declare_clause
			{ $$ = $2;}
		;

declare_clause  : FILTER filter_decl_clause
			{ $$ = $2; }
		| EXTERNAL FUNCTION udf_decl_clause
			{ $$ = $3; }
		;


udf_decl_clause : symbol_UDF_name arg_desc_list1 RETURNS return_value1
			ENTRY_POINT sql_string MODULE_NAME sql_string
				{ $$ = make_node (nod_def_udf, (int) e_udf_count,
				$1, $6, $8, make_list ($2), $4); }
		;

udf_data_type	: simple_type
		| BLOB
			{ lex.g_field->fld_dtype = dtype_blob; }
		| CSTRING '(' pos_short_integer ')' charset_clause
			{
			lex.g_field->fld_dtype = dtype_cstring;
			lex.g_field->fld_character_length = (USHORT)(IPTR) $3; }
		;

arg_desc_list1	:
		 	{ $$ = NULL; }
		| arg_desc_list
		| '(' arg_desc_list ')'
		 	{ $$ = $2; }
		;

arg_desc_list	: arg_desc
		| arg_desc_list ',' arg_desc
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

/*arg_desc	: init_data_type udf_data_type
  { $$ = $1; } */
arg_desc	: init_data_type udf_data_type param_mechanism
			{ $$ = make_node (nod_udf_param, (int) e_udf_param_count,
							  $1, $3); }
		;

param_mechanism :
			{ $$ = NULL; } /* Beware: ddl.cpp converts this to mean FUN_reference. */
		| BY KW_DESCRIPTOR
			{ $$ = MAKE_const_slong (FUN_descriptor); }
		| BY SCALAR_ARRAY
			{ $$ = MAKE_const_slong (FUN_scalar_array); }
		| KW_NULL
			{ $$ = MAKE_const_slong (FUN_ref_with_null); }
		;

return_value1	: return_value
		| '(' return_value ')'
			{ $$ = $2; }
		;

return_value	: init_data_type udf_data_type return_mechanism
			{ $$ = make_node (nod_udf_return_value, (int) e_udf_param_count,
							  $1, $3); }
		| PARAMETER pos_short_integer
			{ $$ = make_node (nod_udf_return_value, (int) e_udf_param_count,
				NULL, MAKE_const_slong ((IPTR) $2));}
		;

return_mechanism :
			{ $$ = MAKE_const_slong (FUN_reference); }
		| BY KW_VALUE
			{ $$ = MAKE_const_slong (FUN_value); }
		| BY KW_DESCRIPTOR
			{ $$ = MAKE_const_slong (FUN_descriptor); }
		| FREE_IT
			{ $$ = MAKE_const_slong (-1 * FUN_reference); }
										 /* FUN_refrence with FREE_IT is -ve */
		| BY KW_DESCRIPTOR FREE_IT
			{ $$ = MAKE_const_slong (-1 * FUN_descriptor); }
		;


filter_decl_clause : symbol_filter_name INPUT_TYPE blob_filter_subtype OUTPUT_TYPE blob_filter_subtype
			ENTRY_POINT sql_string MODULE_NAME sql_string
				{ $$ = make_node (nod_def_filter, (int) e_filter_count,
						$1, $3, $5, $7, $9); }
		;

blob_filter_subtype :	symbol_blob_subtype_name
				{ $$ = MAKE_constant ((dsql_str*) $1, CONSTANT_STRING); }
		|
						signed_short_integer
				{ $$ = MAKE_const_slong ((IPTR) $1); }
		;

/* CREATE metadata operations */

create	 	: CREATE create_clause
			{ $$ = $2; }
		;

create_clause	: EXCEPTION exception_clause
			{ $$ = $2; }
		| unique_opt order_direction INDEX symbol_index_name ON simple_table_name index_definition
			{ $$ = make_node (nod_def_index, (int) e_idx_count,
					$1, $2, $4, $6, $7); }
		| PROCEDURE procedure_clause
			{ $$ = $2; }
		| TABLE table_clause
			{ $$ = $2; }
		| GLOBAL TEMPORARY TABLE gtt_table_clause
			{ $$ = $4; }
		| TRIGGER trigger_clause
			{ $$ = $2; }
		| VIEW view_clause
			{ $$ = $2; }
		| GENERATOR generator_clause
			{ $$ = $2; }
		| SEQUENCE generator_clause
			{ $$ = $2; }
		| DATABASE db_clause
			{ $$ = $2; }
		| DOMAIN domain_clause
			{ $$ = $2; }
		| SHADOW shadow_clause
			{ $$ = $2; }
		| ROLE role_clause
			{ $$ = $2; }
		| COLLATION collation_clause
			{ $$ = $2; }
		| USER create_user_clause
			{ $$ = $2; }
		;


recreate 	: RECREATE recreate_clause
			{ $$ = $2; }
		;

recreate_clause	: PROCEDURE rprocedure_clause
			{ $$ = $2; }
		| TABLE rtable_clause
			{ $$ = $2; }
		| GLOBAL TEMPORARY TABLE gtt_recreate_clause
			{ $$ = $4; }
		| VIEW rview_clause
			{ $$ = $2; }
		| TRIGGER rtrigger_clause
			{ $$ = $2; }
/*
		| DOMAIN rdomain_clause
			{ $$ = $2; }
*/
		| EXCEPTION rexception_clause
			{ $$ = $2; }
		;

create_or_alter	: CREATE OR ALTER replace_clause
			{ $$ = $4; }
		;

replace_clause	: PROCEDURE replace_procedure_clause
			{ $$ = $2; }
		| TRIGGER replace_trigger_clause
			{ $$ = $2; }
		| VIEW replace_view_clause
			{ $$ = $2; }
		| EXCEPTION replace_exception_clause
			{ $$ = $2; }
		;


/* CREATE EXCEPTION */

exception_clause	: symbol_exception_name sql_string
			{ $$ = make_node (nod_def_exception, (int) e_xcp_count,
						$1, $2); }
		;

rexception_clause	: symbol_exception_name sql_string
			{ $$ = make_node (nod_redef_exception, (int) e_xcp_count,
						$1, $2); }
		;

replace_exception_clause	: symbol_exception_name sql_string
			{ $$ = make_node (nod_replace_exception, (int) e_xcp_count,
						$1, $2); }
		;

alter_exception_clause	: symbol_exception_name sql_string
			{ $$ = make_node (nod_mod_exception, (int) e_xcp_count,
						$1, $2); }
		;


/* CREATE INDEX */

unique_opt	: UNIQUE
			{ $$ = make_node (nod_unique, 0, NULL); }
		|
			{ $$ = NULL; }
		;

index_definition : column_list
			{ $$ = make_list ($1); }
		| column_parens
		| computed_by '(' begin_trigger value end_trigger ')'
			{ $$ = make_node (nod_def_computed, 2, $4, $5); }
		;


/* CREATE SHADOW */
shadow_clause	: pos_short_integer manual_auto conditional sql_string
			first_file_length sec_shadow_files
		 	{ $$ = make_node (nod_def_shadow, (int) e_shadow_count,
				 $1, $2, $3, $4, $5, make_list ($6)); }
		;

manual_auto	: MANUAL
			{ $$ = MAKE_const_slong (1); }
		| AUTO
			{ $$ = MAKE_const_slong (0); }
		|
			{ $$ = MAKE_const_slong (0); }
		;

conditional	:
			{ $$ = MAKE_const_slong (0); }
		| CONDITIONAL
			{ $$ = MAKE_const_slong (1); }
		;

first_file_length :
			{ $$ = (dsql_nod*) 0;}
		| LENGTH equals long_integer page_noise
			{ $$ = $3; }
		;

sec_shadow_files :
		 	{ $$ = NULL; }
		| db_file_list
		;

db_file_list	: db_file
		| db_file_list db_file
			{ $$ = make_node (nod_list, (int) 2, $1, $2); }
		;


/* CREATE DOMAIN */

domain_clause	: column_def_name
		as_opt
		data_type
		domain_default_opt
		domain_constraint_clause
		collate_clause
			{ $$ = make_node (nod_def_domain, (int) e_dom_count, $1, $4, make_list ($5), $6); }
		;

/*
rdomain_clause	: DOMAIN alter_column_name alter_domain_ops
			{ $$ = make_node (nod_mod_domain, (int) e_alt_count,
					$2, make_list ($3)); }
*/

as_opt	: AS
			{ $$ = NULL; }
		|
			{ $$ = NULL; }
		;

domain_default	: DEFAULT begin_trigger default_value end_default
			{ $$ = make_node (nod_def_default, (int) e_dft_count, $3, $4); }
		;

domain_default_opt	: domain_default
		|
			{ $$ = NULL; }
		;

domain_constraint_clause	: domain_constraint_list
		|
			{ $$ = NULL; }
		;

domain_constraint_list  : domain_constraint_def
		| domain_constraint_list domain_constraint_def
			{ $$ = make_node (nod_list, (int) 2, $1, $2); }
		;

domain_constraint_def	: domain_constraint
			{ $$ = make_node (nod_rel_constraint, (int) 2, NULL, $1);}
		;

domain_constraint	: null_constraint
		| check_constraint
		;

null_constraint	: NOT KW_NULL
			{ $$ = make_node (nod_null, (int) 0, NULL); }
		;

check_constraint	: CHECK begin_trigger '(' search_condition ')' end_trigger
			{ $$ = make_node (nod_def_constraint, (int) e_cnstr_count,
					NULL, NULL, $4, NULL, $6); }
		;


/* CREATE SEQUENCE/GENERATOR */

generator_clause : symbol_generator_name
			{ $$ = make_node (nod_def_generator, (int) e_gen_count, $1); }
		 ;


/* CREATE ROLE */

role_clause : symbol_role_name
			{ $$ = make_node (nod_def_role, (int) 1, $1); }
		;


/* CREATE COLLATION */

collation_clause : symbol_collation_name FOR symbol_character_set_name
		collation_sequence_definition
		collation_attribute_list_opt collation_specific_attribute_opt
			{ $$ = make_node (nod_def_collation,
						(int) e_def_coll_count, $1, $3, $4, make_list($5), $6); }
		;

collation_sequence_definition :
		FROM symbol_collation_name
			{ $$ = make_node(nod_collation_from, 1, $2); }
		| FROM EXTERNAL '(' sql_string ')'
			{ $$ = make_node(nod_collation_from_external, 1, $4); }
		|
			{ $$ = NULL; }
		;

collation_attribute_list_opt :
			{ $$ = NULL; }
		| collation_attribute_list
		;

collation_attribute_list : collation_attribute
		| collation_attribute_list collation_attribute
			{ $$ = make_node(nod_list, 2, $1, $2); }
		;

collation_attribute :
		  collation_pad_attribute
		| collation_case_attribute
		| collation_accent_attribute
		;

collation_pad_attribute : NO PAD
			{ $$ = make_node(nod_collation_attr, 1, -TEXTTYPE_ATTR_PAD_SPACE); }
		| PAD SPACE
			{ $$ = make_node(nod_collation_attr, 1, TEXTTYPE_ATTR_PAD_SPACE); }
		;

collation_case_attribute : CASE SENSITIVE
			{ $$ = make_node(nod_collation_attr, 1, -TEXTTYPE_ATTR_CASE_INSENSITIVE); }
		| CASE INSENSITIVE
			{ $$ = make_node(nod_collation_attr, 1, TEXTTYPE_ATTR_CASE_INSENSITIVE); }
		;

collation_accent_attribute : ACCENT SENSITIVE
			{ $$ = make_node(nod_collation_attr, 1, -TEXTTYPE_ATTR_ACCENT_INSENSITIVE); }
		| ACCENT INSENSITIVE
			{ $$ = make_node(nod_collation_attr, 1, TEXTTYPE_ATTR_ACCENT_INSENSITIVE); }
		;

collation_specific_attribute_opt :
			{ $$ = NULL; }
		| sql_string
			{ $$ = make_node(nod_collation_specific_attr, 1,
				MAKE_constant((dsql_str*)$1, CONSTANT_STRING)); }
		;

// ALTER CHARACTER SET

alter_charset_clause
	: symbol_character_set_name SET DEFAULT COLLATION symbol_collation_name
		{
			$$ = makeClassNode(FB_NEW(getPool())
					AlterCharSetNode(getPool(), toName($1), toName($5)));
		}
	;

/* CREATE DATABASE */

db_clause	:  db_name db_initial_desc1 db_rem_desc1
			{ $$ = make_node (nod_def_database, (int) e_cdb_count,
				 $1, make_list($2), make_list ($3));}
		;

equals		:
		| '='
		;

db_name		: sql_string
			{ $$ = (dsql_nod*) $1; }
		;

db_initial_desc1 :
			{$$ = NULL;}
		| db_initial_desc
		;

db_initial_desc : db_initial_option
		| db_initial_desc db_initial_option
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

db_initial_option: KW_PAGE_SIZE equals pos_short_integer
			{ $$ = make_node (nod_page_size, 1, $3);}
		| LENGTH equals long_integer page_noise
			{ $$ = make_node (nod_file_length, 1, $3);}
		| USER sql_string
			{ $$ = make_node (nod_user_name, 1, $2);}
		| PASSWORD sql_string
			{ $$ = make_node (nod_password, 1, $2);}
		| SET NAMES sql_string
			{ $$ = make_node (nod_lc_ctype, 1, $3);}
		;

db_rem_desc1	:
			{$$ = NULL;}
		| db_rem_desc
		;

db_rem_desc	: db_rem_option
		| db_rem_desc db_rem_option
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

db_rem_option   : db_file
		| DEFAULT CHARACTER SET symbol_character_set_name
			{ $$ = make_node (nod_dfl_charset, 1, $4);}
		| DEFAULT CHARACTER SET symbol_character_set_name COLLATION symbol_collation_name
			{ $$ = make_node (nod_list, 2,
				make_node (nod_dfl_charset, 1, $4),
				make_node (nod_dfl_collate, 1, $6));
			}
		| KW_DIFFERENCE KW_FILE sql_string
			{ $$ = make_node (nod_difference_file, 1, $3); }
		;

db_file		: file1 sql_string file_desc1
			{ lex.g_file->fil_name = (dsql_str*) $2;
			  $$ = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
		;

file1		: KW_FILE
			{ lex.g_file  = make_file();}
		;

file_desc1	:
		| file_desc
		;

file_desc	: file_clause
		| file_desc file_clause
		;

file_clause	: STARTING file_clause_noise long_integer
			{ lex.g_file->fil_start = (IPTR) $3;}
		| LENGTH equals long_integer page_noise
			{ lex.g_file->fil_length = (IPTR) $3;}
		;

file_clause_noise :
		| AT
		| AT PAGE
		;

page_noise	:
		| PAGE
		| PAGES
		;


/* CREATE TABLE */

table_clause	: simple_table_name external_file '(' table_elements ')'
			{ $$ = make_flag_node (nod_def_relation, NOD_PERMANENT_TABLE,
				(int) e_drl_count, $1, make_list ($4), $2); }
		;

rtable_clause	: simple_table_name external_file '(' table_elements ')'
			{ $$ = make_flag_node (nod_redef_relation, NOD_PERMANENT_TABLE,
				(int) e_drl_count, $1, make_list ($4), $2); }
		;

gtt_table_clause :	simple_table_name '(' table_elements ')' gtt_scope
			{ $$ = make_flag_node (nod_def_relation, (SSHORT) (IPTR) ($5),
				(int) e_drl_count, $1, make_list ($3), NULL); }
		;

gtt_recreate_clause	:	simple_table_name '(' table_elements ')' gtt_scope
			{ $$ = make_flag_node (nod_redef_relation, (SSHORT) (IPTR) ($5),
				(int) e_drl_count, $1, make_list ($3), NULL); }
		;

gtt_scope : ON COMMIT PRESERVE ROWS
			{ $$ = (dsql_nod*) NOD_GLOBAL_TEMP_TABLE_PRESERVE_ROWS; }
		|	ON COMMIT KW_DELETE ROWS
			{ $$ = (dsql_nod*) NOD_GLOBAL_TEMP_TABLE_DELETE_ROWS; }
		|
			{ $$ = (dsql_nod*) NOD_GLOBAL_TEMP_TABLE_DELETE_ROWS; }
		;

external_file	: EXTERNAL KW_FILE sql_string
			{ $$ = $3; }
		| EXTERNAL sql_string
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

table_elements	: table_element
		| table_elements ',' table_element
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

table_element	: column_def
		| table_constraint_definition
		;



/* column definition */

column_def	: column_def_name data_type_or_domain domain_default_opt
			column_constraint_clause collate_clause
			{ $$ = make_node (nod_def_field, (int) e_dfl_count,
					$1, $3, make_list ($4), $5, $2, NULL); }
		| column_def_name non_array_type def_computed
			{ $$ = make_node (nod_def_field, (int) e_dfl_count,
					$1, NULL, NULL, NULL, NULL, $3); }
		| column_def_name def_computed
			{ $$ = make_node (nod_def_field, (int) e_dfl_count,
					$1, NULL, NULL, NULL, NULL, $2); }
		;

/* value does allow parens around it, but there is a problem getting the
 * source text
 */

def_computed	: computed_clause '(' begin_trigger value end_trigger ')'
			{
				lex.g_field->fld_flags |= FLD_computed;
				$$ = make_node (nod_def_computed, 2, $4, $5);
			}
		;

computed_clause	: computed_by
		| GENERATED ALWAYS AS
		;

computed_by	: COMPUTED BY
		| COMPUTED
		;

data_type_or_domain	: data_type
			  { $$ = NULL; }
		| simple_column_name
			  { $$ = make_node (nod_def_domain, (int) e_dom_count, $1, NULL, NULL, NULL); }
		;

collate_clause	: COLLATE symbol_collation_name
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;


column_def_name	: simple_column_name
			{
				lex.g_field_name = $1;
				lex.g_field = make_field ($1);
				$$ = (dsql_nod*) lex.g_field;
			}
		;

simple_column_def_name  : simple_column_name
			{
				lex.g_field = make_field ($1);
				$$ = (dsql_nod*) lex.g_field;
			}
		;


data_type_descriptor :	init_data_type data_type
			{ $$ = $1; }
		| KW_TYPE OF column_def_name
			{
				((dsql_fld*) $3)->fld_type_of_name = ((dsql_fld*) $3)->fld_name;
				$$ = $3;
			}
		| KW_TYPE OF COLUMN symbol_column_name '.' symbol_column_name
			{
				lex.g_field = make_field(NULL);
				lex.g_field->fld_type_of_table = ((dsql_str*) $4)->str_data;
				lex.g_field->fld_type_of_name = ((dsql_str*) $6)->str_data;
				$$ = (dsql_nod*) lex.g_field;
			}
		| column_def_name
			{
				((dsql_fld*) $1)->fld_type_of_name = ((dsql_fld*) $1)->fld_name;
				((dsql_fld*) $1)->fld_full_domain = true;
				$$ = $1;
			}
		;

init_data_type :
			{ lex.g_field = make_field (NULL);
			  $$ = (dsql_nod*) lex.g_field; }
		;


default_value	: constant
		| current_user
		| current_role
		| internal_info
		| null_value
		| datetime_value_expression
		;

column_constraint_clause :
				{ $$ = NULL; }
			| column_constraint_list
			;

column_constraint_list	: column_constraint_def
				| column_constraint_list column_constraint_def
			{ $$ = make_node (nod_list, (int) 2, $1, $2); }
				;

column_constraint_def : constraint_name_opt column_constraint
			{ $$ = make_node (nod_rel_constraint, (int) 2, $1, $2);}
		;


column_constraint : null_constraint
				  | check_constraint
				  | REFERENCES simple_table_name column_parens_opt
			referential_trigger_action constraint_index_opt
						{ $$ = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), $2, $3, $4, $5); }

				  | UNIQUE constraint_index_opt
						{ $$ = make_node (nod_unique, 2, NULL, $2); }
				  | PRIMARY KEY constraint_index_opt
						{ $$ = make_node (nod_primary, (int) e_pri_count, NULL, $3); }
		;



/* table constraints */

table_constraint_definition : constraint_name_opt table_constraint
		   { $$ = make_node (nod_rel_constraint, (int) 2, $1, $2);}
		;

constraint_name_opt : CONSTRAINT symbol_constraint_name
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

table_constraint : unique_constraint
			| primary_constraint
			| referential_constraint
			| check_constraint
		;

unique_constraint	: UNIQUE column_parens constraint_index_opt
			{ $$ = make_node (nod_unique, 2, $2, $3); }
		;

primary_constraint	: PRIMARY KEY column_parens constraint_index_opt
			{ $$ = make_node (nod_primary, (int) e_pri_count, $3, $4); }
		;

referential_constraint	: FOREIGN KEY column_parens REFERENCES
			  simple_table_name column_parens_opt
			  referential_trigger_action constraint_index_opt
			{ $$ = make_node (nod_foreign, (int) e_for_count, $3, $5,
					 $6, $7, $8); }
		;

constraint_index_opt	: USING order_direction INDEX symbol_index_name
			{ $$ = make_node (nod_def_index, (int) e_idx_count,
					NULL, $2, $4, NULL, NULL); }
/*
		| NO INDEX
			{ $$ = NULL; }
*/
		|
			{ $$ = make_node (nod_def_index, (int) e_idx_count,
					NULL, NULL, NULL, NULL, NULL); }
		;

referential_trigger_action:
		  update_rule
		  { $$ = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, $1, NULL);}
		| delete_rule
		  { $$ = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, $1);}
		| delete_rule update_rule
		  { $$ = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, $2, $1); }
		| update_rule delete_rule
		  { $$ = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, $1, $2);}
		| /* empty */
		  { $$ = NULL;}
		;

update_rule	: ON UPDATE referential_action
		  { $$ = $3;}
		;
delete_rule	: ON KW_DELETE referential_action
		  { $$ = $3;}
		;

referential_action: CASCADE
		  { $$ = make_flag_node (nod_ref_trig_action,
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
		| SET DEFAULT
		  { $$ = make_flag_node (nod_ref_trig_action,
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
		| SET KW_NULL
		  { $$ = make_flag_node (nod_ref_trig_action,
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
		| NO ACTION
		  { $$ = make_flag_node (nod_ref_trig_action,
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
		;


/* PROCEDURE */


procedure_clause	: symbol_procedure_name input_parameters
			 	  output_parameters
				  AS begin_string
			  local_declaration_list
			  full_proc_block
			  end_trigger
				{ $$ = make_node (nod_def_procedure,
						(int) e_prc_count, $1, $2, $3, $6, $7, $8); }
		;


rprocedure_clause	: symbol_procedure_name input_parameters
			 	  output_parameters
				  AS begin_string
			  local_declaration_list
			  full_proc_block
			  end_trigger
				{ $$ = make_node (nod_redef_procedure,
						(int) e_prc_count, $1, $2, $3, $6, $7, $8); }
		;

replace_procedure_clause	: symbol_procedure_name input_parameters
			 	  output_parameters
				  AS begin_string
			  local_declaration_list
			  full_proc_block
			  end_trigger
				{ $$ = make_node (nod_replace_procedure,
						(int) e_prc_count, $1, $2, $3, $6, $7, $8); }
		;

alter_procedure_clause	: symbol_procedure_name input_parameters
			 	  output_parameters
				  AS begin_string
			  local_declaration_list
			  full_proc_block
			  end_trigger
				{ $$ = make_node (nod_mod_procedure,
						(int) e_prc_count, $1, $2, $3, $6, $7, $8); }
		;

input_parameters :	'(' input_proc_parameters ')'
			{ $$ = make_list ($2); }
		|
			{ $$ = NULL; }
		;

output_parameters :	RETURNS '(' output_proc_parameters ')'
			{ $$ = make_list ($3); }
		|
			{ $$ = NULL; }
		;

input_proc_parameters	: input_proc_parameter
		| input_proc_parameters ',' input_proc_parameter
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

input_proc_parameter	: simple_column_def_name domain_or_non_array_type collate_clause
				default_par_opt
			{ $$ = make_node (nod_def_field, (int) e_dfl_count,
				$1, $4, NULL, $3, NULL, NULL); }
		;

output_proc_parameters	: proc_parameter
		| output_proc_parameters ',' proc_parameter
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

proc_parameter	: simple_column_def_name domain_or_non_array_type collate_clause
			{ $$ = make_node (nod_def_field, (int) e_dfl_count,
				$1, NULL, NULL, $3, NULL, NULL); }
		;

default_par_opt	: DEFAULT begin_trigger default_value end_default
			{ $$ = make_node (nod_def_default, (int) e_dft_count, $3, $4); }
		| '=' begin_trigger default_value end_default
			{ $$ = make_node (nod_def_default, (int) e_dft_count, $3, $4); }
		|
			{ $$ = NULL; }
		;

local_declaration_list	: local_declarations
			{ $$ = make_list ($1); }
		|
			{ $$ = NULL; }
		;

local_declarations	: local_declaration
		| local_declarations local_declaration
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

local_declaration : stmt_start_line stmt_start_column DECLARE var_decl_opt local_declaration_item ';'
			{
				$$ = $5;
				$$->nod_line = (IPTR) $1;
				$$->nod_column = (IPTR) $2;
			}
		;

local_declaration_item	: var_declaration_item
		| cursor_declaration_item
		;

var_declaration_item	: column_def_name domain_or_non_array_type collate_clause default_par_opt
			{ $$ = make_node (nod_def_field, (int) e_dfl_count,
				$1, $4, NULL, $3, NULL, NULL); }
		;

var_decl_opt	: VARIABLE
			{ $$ = NULL; }
		|
			{ $$ = NULL; }
		;

cursor_declaration_item	: symbol_cursor_name CURSOR FOR '(' select ')'
			{ $$ = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, $1, $5, NULL, NULL); }
 		;

proc_block	: proc_statement
		| full_proc_block
		;

full_proc_block	: stmt_start_line stmt_start_column BEGIN full_proc_block_body END
			{
				$$ = make_node (nod_src_info, e_src_info_count, $1, $2, $4);
			}
		;

full_proc_block_body	: proc_statements
			{ $$ = make_node (nod_block, (int) e_blk_count, make_list ($1), NULL); }
		| proc_statements excp_hndl_statements
			{ $$ = make_node (nod_block, (int) e_blk_count, make_list ($1), make_list ($2)); }
		|
			{ $$ = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
		;

proc_statements	: proc_block
		| proc_statements proc_block
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

proc_statement	: stmt_start_line stmt_start_column simple_proc_statement ';'
			{
				$$ = make_node (nod_src_info, e_src_info_count, $1, $2, $3);
			}
		| stmt_start_line stmt_start_column complex_proc_statement
			{
				$$ = make_node (nod_src_info, e_src_info_count, $1, $2, $3);
			}
		;

stmt_start_line :
		{ $$ = (dsql_nod*) (IPTR) lex.lines_bk; }

stmt_start_column :
		{
			const USHORT column = (lex.last_token_bk - lex.line_start_bk + 1);
			$$ = (dsql_nod*) (IPTR) column;
		}

simple_proc_statement	: assignment
		| insert
		| merge
		| update
		| update_or_insert
		| delete
		| singleton_select
		| exec_procedure
		| exec_sql
		| exec_into
		| exec_function
		| excp_statement
		| raise_statement
		| post_event
		| cursor_statement
		| breakleave
		| SUSPEND
			{ $$ = make_node (nod_return, (int) e_rtn_count, NULL); }
		| EXIT
			{ $$ = make_node (nod_exit, 0, NULL); }
		;

complex_proc_statement
	: in_autonomous_transaction
	| if_then_else
	| while
	| for_select
	| for_exec_into
	;

in_autonomous_transaction
	: KW_IN AUTONOMOUS TRANSACTION DO proc_block
		{
			InAutonomousTransactionNode* node = FB_NEW(getPool())
				InAutonomousTransactionNode(getPool());
			node->dsqlAction = $5;
			$$ = makeClassNode(node);
		}
	;

excp_statement	: EXCEPTION symbol_exception_name
			{ $$ = make_node (nod_exception_stmt, (int) e_xcp_count, $2, NULL); }
		| EXCEPTION symbol_exception_name value
			{ $$ = make_node (nod_exception_stmt, (int) e_xcp_count, $2, $3); }
		;

raise_statement	: EXCEPTION
			{ $$ = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
		;

//exec_sql	: EXECUTE STATEMENT value
//			{ $$ = make_node (nod_exec_sql, (int) e_exec_sql_count, $3); }
//		;

for_select	: label_opt FOR select INTO variable_list cursor_def DO proc_block
			{ $$ = make_node (nod_for_select, (int) e_flp_count, $3,
					  make_list ($5), $6, $8, $1); }
		;

//for_exec_into	: label_opt FOR EXECUTE STATEMENT value INTO variable_list DO proc_block
//			{ $$ = make_node (nod_exec_into, (int) e_exec_into_count, $5, $9, make_list ($7), $1); }
//		;
//
//exec_into	: EXECUTE STATEMENT value INTO variable_list
//			{ $$ = make_node (nod_exec_into, (int) e_exec_into_count, $3, NULL, make_list ($5), NULL); }
//		;

exec_sql
	: EXECUTE STATEMENT exec_stmt_inputs exec_stmt_options
		{
			$$ = make_node (nod_exec_stmt, int (e_exec_stmt_count),
					($3)->nod_arg[0], ($3)->nod_arg[1], NULL, NULL, NULL, make_list($4), NULL, NULL, NULL, NULL, NULL, NULL);
		}
	;

exec_into
	: EXECUTE STATEMENT exec_stmt_inputs exec_stmt_options
			INTO variable_list
		{
			$$ = make_node (nod_exec_stmt, int (e_exec_stmt_count),
					($3)->nod_arg[0], ($3)->nod_arg[1], make_list($6), NULL, NULL, make_list($4), NULL, NULL, NULL, NULL, NULL, NULL);
		}
	;

for_exec_into
	: label_opt FOR EXECUTE STATEMENT exec_stmt_inputs exec_stmt_options
			INTO variable_list
			DO proc_block
		{
			$$ = make_node (nod_exec_stmt, int (e_exec_stmt_count),
					($5)->nod_arg[0], ($5)->nod_arg[1], make_list($8), $10, $1, make_list($6), NULL, NULL, NULL, NULL, NULL, NULL);
		}
	;

exec_stmt_inputs
	: value
		{ $$ = make_node (nod_exec_stmt_inputs, e_exec_stmt_inputs_count, $1, NULL); }
	| '(' value ')' '(' named_params_list ')'
		{ $$ = make_node (nod_exec_stmt_inputs, e_exec_stmt_inputs_count, $2, make_list ($5)); }
	| '(' value ')' '(' not_named_params_list ')'
		{ $$ = make_node (nod_exec_stmt_inputs, e_exec_stmt_inputs_count, $2, make_list ($5)); }
	;

named_params_list
	: named_param
	| named_params_list ',' named_param
		{ $$ = make_node (nod_list, 2, $1, $3); }
	;

named_param
	: symbol_variable_name BIND_PARAM value
		  { $$ = make_node (nod_named_param, e_named_param_count, $1, $3); }
	;

not_named_params_list
	: not_named_param
	| not_named_params_list ',' not_named_param
		{ $$ = make_node (nod_list, 2, $1, $3); }
	;

not_named_param
	: value
		{ $$ = make_node (nod_named_param, e_named_param_count, NULL, $1); }
	;

exec_stmt_options
	: exec_stmt_options_list
	|
		{ $$ = NULL; }
	;

exec_stmt_options_list
	: exec_stmt_options_list exec_stmt_option
		{ $$ = make_node (nod_list, 2, $1, $2); }
	| exec_stmt_option
	;

exec_stmt_option
	: ext_datasrc
	| ext_user
	| ext_pwd
	| ext_role
	| ext_tran
	| ext_privs
	;

ext_datasrc
	: ON EXTERNAL DATA SOURCE value
		{ $$ = make_node (nod_exec_stmt_datasrc, 1, $5); }
	| ON EXTERNAL value
		{ $$ = make_node (nod_exec_stmt_datasrc, 1, $3); }
	;

ext_user
	: AS USER value
		{ $$ = make_node (nod_exec_stmt_user, 1, $3); }
	;

ext_pwd
	: PASSWORD value
		{ $$ = make_node (nod_exec_stmt_pwd, 1, $2); }
	;

ext_role
	: ROLE value
		{ $$ = make_node (nod_exec_stmt_role, 1, $2); }
	;

ext_tran
	: WITH AUTONOMOUS TRANSACTION
		{ $$ = make_flag_node(nod_tran_params, NOD_TRAN_AUTONOMOUS, 1, NULL); }
	| WITH COMMON TRANSACTION
		{ $$ = make_flag_node(nod_tran_params, NOD_TRAN_COMMON, 1, NULL); }
	// | WITH TWO_PHASE TRANSACTION
	//		{ $$ = make_flag_node(nod_tran_params, NOD_TRAN_2PC, 1, NULL); }
	;

ext_privs
	: WITH CALLER PRIVILEGES
		{ $$ = make_node (nod_exec_stmt_privs, 1, NULL); }
	;

if_then_else	: IF '(' search_condition ')' THEN proc_block ELSE proc_block
			{ $$ = make_node (nod_if, (int) e_if_count, $3, $6, $8); }
		| IF '(' search_condition ')' THEN proc_block
			{ $$ = make_node (nod_if, (int) e_if_count, $3, $6, NULL); }
		;

post_event	: POST_EVENT value event_argument_opt
			{ $$ = make_node (nod_post, (int) e_pst_count, $2, $3); }
		;

event_argument_opt	: /*',' value
			{ $$ = $2; }
		|*/
			{ $$ = NULL; }
		;

singleton_select	: select INTO variable_list
			{ $$ = make_node (nod_for_select, (int) e_flp_count, $1,
					  make_list ($3), NULL, NULL, NULL); }
		;

variable	: ':' symbol_variable_name
			{ $$ = make_node (nod_var_name, (int) e_vrn_count,
							$2); }
		;

variable_list	: variable
 		| column_name
		| variable_list ',' column_name
			{ $$ = make_node (nod_list, 2, $1, $3); }
		| variable_list ',' variable
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

while		: label_opt WHILE '(' search_condition ')' DO proc_block
			{ $$ = make_node (nod_while, (int) e_while_count, $4, $7, $1); }
		;

label_opt	: symbol_label_name ':'
			{ $$ = make_node (nod_label, (int) e_label_count, $1, NULL); }
		|
			{ $$ = NULL; }
		;

breakleave	: KW_BREAK
			{ $$ = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
		| LEAVE
			{ $$ = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
		| LEAVE symbol_label_name
			{ $$ = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, $2, NULL)); }
		;

cursor_def	: AS CURSOR symbol_cursor_name
			{ $$ = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, $3, NULL, NULL, NULL); }
		|
			{ $$ = NULL; }
		;

excp_hndl_statements	: excp_hndl_statement
		| excp_hndl_statements excp_hndl_statement
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

excp_hndl_statement	: WHEN errors DO proc_block
			{ $$ = make_node (nod_on_error, (int) e_err_count,
					make_list ($2), $4); }
		;

errors	: err
	| errors ',' err
		{ $$ = make_node (nod_list, 2, $1, $3); }
	;

err	: SQLCODE signed_short_integer
		{ $$ = make_node (nod_sqlcode, 1, $2); }
	| GDSCODE symbol_gdscode_name
		{ $$ = make_node (nod_gdscode, 1, $2); }
	| EXCEPTION symbol_exception_name
		{ $$ = make_node (nod_exception, 1, $2); }
	| ANY
		{ $$ = make_node (nod_default, 1, NULL); }
	;

cursor_statement	: open_cursor
	| fetch_cursor
	| close_cursor
	;

open_cursor	: OPEN symbol_cursor_name
		{ $$ = make_node (nod_cursor_open, (int) e_cur_stmt_count, $2, NULL, NULL); }
	;

close_cursor	: CLOSE symbol_cursor_name
		{ $$ = make_node (nod_cursor_close, (int) e_cur_stmt_count, $2, NULL, NULL); }
	;

fetch_cursor	: FETCH fetch_opt symbol_cursor_name INTO variable_list
		{ $$ = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, $3, $2, make_list ($5)); }
	;

fetch_opt	:
		{ $$ = NULL; }
	;
/*
fetch_opt	: fetch_seek_opt FROM
	;

fetch_seek_opt	:
	| FIRST
		{ $$ = make_node (nod_fetch_seek, 2,
				// corresponds to (blr_bof_forward, 0)
				MAKE_const_slong (3),
				MAKE_const_slong (0)); }
	| LAST
		{ $$ = make_node (nod_fetch_seek, 2,
				// corresponds to (blr_eof_backward, 0)
				MAKE_const_slong (4),
				MAKE_const_slong (0)); }
	| PRIOR
		{ $$ = make_node (nod_fetch_seek, 2,
				// corresponds to (blr_backward, 1)
				MAKE_const_slong (2),
				MAKE_const_slong (1)); }
	| NEXT
		{ $$ = make_node (nod_fetch_seek, 2,
				// corresponds to (blr_forward, 1)
				MAKE_const_slong (1),
				MAKE_const_slong (1)); }
	| ABSOLUTE value
		{ $$ = make_node (nod_fetch_seek, 2,
				// corresponds to (blr_bof_forward, value)
				MAKE_const_slong (3),
				$2); }
	| RELATIVE value
		{ $$ = make_node (nod_fetch_seek, 2,
				// corresponds to (blr_forward, value)
				MAKE_const_slong (1),
				$2); }
	;
*/

/* EXECUTE PROCEDURE */

exec_procedure	: EXECUTE PROCEDURE symbol_procedure_name proc_inputs proc_outputs_opt
			{ $$ = make_node (nod_exec_procedure, (int) e_exe_count,
					$3, $4, $5); }
		;

proc_inputs	: value_list
			{ $$ = make_list ($1); }
		| '(' value_list ')'
			{ $$ = make_list ($2); }
		|
			{ $$ = NULL; }
		;

proc_outputs_opt	: RETURNING_VALUES variable_list
			{ $$ = make_list ($2); }
		| RETURNING_VALUES '(' variable_list  ')'
			{ $$ = make_list ($3); }
		|
			{ $$ = NULL; }
		;

/* EXECUTE BLOCK */

exec_block : EXECUTE BLOCK block_input_params output_parameters AS
			local_declaration_list
			full_proc_block
				{ $$ = make_node (nod_exec_block,
						  (int) e_exe_blk_count,
					          $3, $4, $6, $7, make_node (nod_all, (int) 0, NULL)); }
		;

block_input_params :	'(' block_parameters ')'
				{ $$ = make_list ($2); }
			|
				{ $$ = NULL; }
			;

block_parameters	: block_parameter
		| block_parameters ',' block_parameter
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

block_parameter	: proc_parameter '=' parameter
			{ $$ = make_node (nod_param_val, e_prm_val_count, $1, $3); }
		;

/* CREATE VIEW */

view_clause	: symbol_view_name column_parens_opt AS begin_string select_expr
															check_opt end_trigger
			{ $$ = make_node (nod_def_view, (int) e_view_count,
					  $1, $2, $5, $6, $7); }
		;


rview_clause	: symbol_view_name column_parens_opt AS begin_string select_expr
															check_opt end_trigger
			{ $$ = make_node (nod_redef_view, (int) e_view_count,
					  $1, $2, $5, $6, $7); }
		;

replace_view_clause	: symbol_view_name column_parens_opt AS begin_string select_expr
															check_opt end_trigger
			{ $$ = make_node (nod_replace_view, (int) e_view_count,
					  $1, $2, $5, $6, $7); }
		;

alter_view_clause	: symbol_view_name column_parens_opt AS begin_string select_expr
															check_opt end_trigger
 			{ $$ = make_node (nod_mod_view, (int) e_view_count,
					  $1, $2, $5, $6, $7); }
 		;


/* these rules will capture the input string for storage in metadata */

begin_string	:
			{ lex.beginnings.push(lex_position()); }
		;
/*
end_string	:
			{
				const TEXT* start = lex.beginnings.pop();
				$$ = (dsql_nod*) MAKE_string(start,
					(lex_position() == lex.end) ? lex_position() - start : lex.last_token - start);
			}
		;
*/
begin_trigger	:
			{ lex.beginnings.push(lex.last_token); }
		;

end_trigger	:
			{
				const TEXT* start = lex.beginnings.pop();
				string str;
				transformString(start, lex_position() - start, str);
				$$ = (dsql_nod*) MAKE_string(str.c_str(), str.length());
			}
		;

end_default	:
			{
				const TEXT* start = lex.beginnings.pop();
				$$ = (dsql_nod*) MAKE_string(start,
					(yychar <= 0 ? lex_position() : lex.last_token) - start);
			}
		;

check_opt	: WITH CHECK OPTION
			{ $$ = make_node (nod_def_constraint, (int) e_cnstr_count,
					NULL, NULL, NULL, NULL, NULL); }
		|
			{ $$ = 0; }
		;



/* CREATE TRIGGER */

trigger_clause
	:	symbol_trigger_name
		trigger_active
		trigger_type
		trigger_position
		trigger_action
		end_trigger
		{
			$$ = make_node (nod_def_trigger, (int) e_trg_count,
				$1, NULL, $2, $3, $4, $5, $6);
		}
	|	symbol_trigger_name FOR simple_table_name
		trigger_active
		trigger_type
		trigger_position
		trigger_action
		end_trigger
		{
			$$ = make_node (nod_def_trigger, (int) e_trg_count,
				$1, $3, $4, $5, $6, $7, $8);
		}
	|	symbol_trigger_name
		trigger_active
		trigger_type
		trigger_position
		ON simple_table_name
		trigger_action
		end_trigger
		{
			$$ = make_node (nod_def_trigger, (int) e_trg_count,
				$1, $6, $2, $3, $4, $7, $8);
		}
	;

rtrigger_clause
	:	symbol_trigger_name
		trigger_active
		trigger_type
		trigger_position
		trigger_action
		end_trigger
		{
			$$ = make_node (nod_redef_trigger, (int) e_trg_count,
				$1, NULL, $2, $3, $4, $5, $6);
		}
	|	symbol_trigger_name FOR simple_table_name
		trigger_active
		trigger_type
		trigger_position
		trigger_action
		end_trigger
		{
			$$ = make_node (nod_redef_trigger, (int) e_trg_count,
				$1, $3, $4, $5, $6, $7, $8);
		}
	|	symbol_trigger_name
		trigger_active
		trigger_type
		trigger_position
		ON simple_table_name
		trigger_action
		end_trigger
		{
			$$ = make_node (nod_redef_trigger, (int) e_trg_count,
				$1, $6, $2, $3, $4, $7, $8);
		}
	;

replace_trigger_clause
	:	symbol_trigger_name
		trigger_active
		trigger_type
		trigger_position
		trigger_action
		end_trigger
		{
			$$ = make_node (nod_replace_trigger, (int) e_trg_count,
				$1, NULL, $2, $3, $4, $5, $6);
		}
	|	symbol_trigger_name FOR simple_table_name
		trigger_active
		trigger_type
		trigger_position
		trigger_action
		end_trigger
		{
			$$ = make_node (nod_replace_trigger, (int) e_trg_count,
				$1, $3, $4, $5, $6, $7, $8);
		}
	|	symbol_trigger_name
		trigger_active
		trigger_type
		trigger_position
		ON simple_table_name
		trigger_action
		end_trigger
		{
			$$ = make_node (nod_replace_trigger, (int) e_trg_count,
				$1, $6, $2, $3, $4, $7, $8);
		}
	;

trigger_active	: ACTIVE
			{ $$ = MAKE_const_slong (0); }
		| INACTIVE
			{ $$ = MAKE_const_slong (1); }
		|
			{ $$ = NULL; }
		;

trigger_type
	:	trigger_type_prefix trigger_type_suffix
			{ $$ = MAKE_trigger_type ($1, $2); }
	|	ON trigger_db_type
			{ $$ = $2; }
	;

trigger_db_type
	:	CONNECT
			{ $$ = MAKE_const_slong (TRIGGER_TYPE_DB | DB_TRIGGER_CONNECT); }
	|	DISCONNECT
			{ $$ = MAKE_const_slong (TRIGGER_TYPE_DB | DB_TRIGGER_DISCONNECT); }
	|	TRANSACTION START
			{ $$ = MAKE_const_slong (TRIGGER_TYPE_DB | DB_TRIGGER_TRANS_START); }
	|	TRANSACTION COMMIT
			{ $$ = MAKE_const_slong (TRIGGER_TYPE_DB | DB_TRIGGER_TRANS_COMMIT); }
	|	TRANSACTION ROLLBACK
			{ $$ = MAKE_const_slong (TRIGGER_TYPE_DB | DB_TRIGGER_TRANS_ROLLBACK); }
	;

trigger_type_prefix	: BEFORE
			{ $$ = MAKE_const_slong (0); }
		| AFTER
			{ $$ = MAKE_const_slong (1); }
		;

trigger_type_suffix	: INSERT
			{ $$ = MAKE_const_slong (trigger_type_suffix (1, 0, 0)); }
		| UPDATE
			{ $$ = MAKE_const_slong (trigger_type_suffix (2, 0, 0)); }
		| KW_DELETE
			{ $$ = MAKE_const_slong (trigger_type_suffix (3, 0, 0)); }
		| INSERT OR UPDATE
			{ $$ = MAKE_const_slong (trigger_type_suffix (1, 2, 0)); }
		| INSERT OR KW_DELETE
			{ $$ = MAKE_const_slong (trigger_type_suffix (1, 3, 0)); }
		| UPDATE OR INSERT
			{ $$ = MAKE_const_slong (trigger_type_suffix (2, 1, 0)); }
		| UPDATE OR KW_DELETE
			{ $$ = MAKE_const_slong (trigger_type_suffix (2, 3, 0)); }
		| KW_DELETE OR INSERT
			{ $$ = MAKE_const_slong (trigger_type_suffix (3, 1, 0)); }
		| KW_DELETE OR UPDATE
			{ $$ = MAKE_const_slong (trigger_type_suffix (3, 2, 0)); }
		| INSERT OR UPDATE OR KW_DELETE
			{ $$ = MAKE_const_slong (trigger_type_suffix (1, 2, 3)); }
		| INSERT OR KW_DELETE OR UPDATE
			{ $$ = MAKE_const_slong (trigger_type_suffix (1, 3, 2)); }
		| UPDATE OR INSERT OR KW_DELETE
			{ $$ = MAKE_const_slong (trigger_type_suffix (2, 1, 3)); }
		| UPDATE OR KW_DELETE OR INSERT
			{ $$ = MAKE_const_slong (trigger_type_suffix (2, 3, 1)); }
		| KW_DELETE OR INSERT OR UPDATE
			{ $$ = MAKE_const_slong (trigger_type_suffix (3, 1, 2)); }
		| KW_DELETE OR UPDATE OR INSERT
			{ $$ = MAKE_const_slong (trigger_type_suffix (3, 2, 1)); }
		;

trigger_position : POSITION nonneg_short_integer
			{ $$ = MAKE_const_slong ((IPTR) $2); }
		|
			{ $$ = NULL; }
		;

trigger_action : AS begin_trigger local_declaration_list full_proc_block
			{ $$ = make_node (nod_list, (int) e_trg_act_count, $3, $4); }
		;

/* ALTER statement */

alter	: ALTER alter_clause
			{ $$ = $2; }
		;

alter_clause	: EXCEPTION alter_exception_clause
			{ $$ = $2; }
		| TABLE simple_table_name alter_ops
			{ $$ = make_node (nod_mod_relation, (int) e_alt_count,
						$2, make_list ($3)); }
 		| VIEW alter_view_clause
 			{ $$ = $2; }
		| TRIGGER alter_trigger_clause
			{ $$ = $2; }
		| PROCEDURE alter_procedure_clause
			{ $$ = $2; }
		| DATABASE init_alter_db alter_db
			{ $$ = make_node (nod_mod_database, (int) e_adb_count,
				make_list ($3)); }
		| DOMAIN alter_column_name alter_domain_ops
			{ $$ = make_node (nod_mod_domain, (int) e_alt_count,
										  $2, make_list ($3)); }
		| INDEX alter_index_clause
			{ $$ = make_node (nod_mod_index, (int) e_mod_idx_count, $2); }
		| SEQUENCE alter_sequence_clause
			{ $$ = $2; }
		| EXTERNAL FUNCTION alter_udf_clause
			{ $$ = $3; }
		| ROLE alter_role_clause
			{ $$ = $2; }
		| USER alter_user_clause
			{ $$ = $2; }
		| CHARACTER SET alter_charset_clause
			{ $$ = $3; }
		;

alter_domain_ops	: alter_domain_op
		| alter_domain_ops alter_domain_op
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

alter_domain_op	: SET domain_default
			{ $$ = $2; }
		| ADD CONSTRAINT check_constraint
			{ $$ = $3; }
		| ADD check_constraint
			{ $$ = $2; }
		| DROP DEFAULT
			{$$ = make_node (nod_del_default, (int) 0, NULL); }
		| DROP CONSTRAINT
			{ $$ = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
		| TO simple_column_name
			{ $$ = $2; }
		| KW_TYPE init_data_type non_array_type
			{ $$ = make_node (nod_mod_domain_type, 2, $2); }
		;

alter_ops	: alter_op
		| alter_ops ',' alter_op
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

alter_op	: DROP simple_column_name drop_behaviour
			{ $$ = make_node (nod_del_field, 2, $2, $3); }
		| DROP CONSTRAINT symbol_constraint_name
			{ $$ = make_node (nod_delete_rel_constraint, (int) 1, $3);}
		| ADD column_def
			{ $$ = $2; }
		| ADD table_constraint_definition
			{ $$ = $2; }
/* CVC: From SQL, field positions start at 1, not zero. Think in ORDER BY, for example.
		| col_opt simple_column_name POSITION nonneg_short_integer
			{ $$ = make_node (nod_mod_field_pos, 2, $2,
			MAKE_const_slong ((IPTR) $4)); } */
		| col_opt simple_column_name POSITION pos_short_integer
			{ $$ = make_node(nod_mod_field_pos, 2, $2,
				MAKE_const_slong((IPTR) $4)); }
		| col_opt alter_column_name TO simple_column_name
			{ $$ = make_node(nod_mod_field_name, 2, $2, $4); }
		| col_opt alter_col_name KW_TYPE alter_data_type_or_domain
			{ $$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, $4, NULL, NULL); }
		| col_opt alter_col_name KW_TYPE non_array_type def_computed
			{
				// Due to parser hacks, we should not pass $4 (non_array_type) to make_node.
				$$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, NULL, NULL, $5);
			}
		| col_opt alter_col_name def_computed
			{ $$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, NULL, NULL, $3); }
		| col_opt alter_col_name SET domain_default
			{ $$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, NULL, $4, NULL); }
		| col_opt alter_col_name DROP DEFAULT
			{ $$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, NULL,
					make_node(nod_del_default, (int) 0, NULL), NULL); }
		;

alter_column_name  : keyword_or_column
		   { $$ = make_node (nod_field_name, (int) e_fln_count,
						NULL, $1); }
	   ;

/* below are reserved words that could be used as column identifiers
   in the previous versions */

keyword_or_column	: valid_symbol_name
		| ADMIN					/* added in IB 5.0 */
		| COLUMN				/* added in IB 6.0 */
		| EXTRACT
		| YEAR
		| MONTH
		| DAY
		| HOUR
		| MINUTE
		| SECOND
		| TIME
		| TIMESTAMP
		| CURRENT_DATE
		| CURRENT_TIME
		| CURRENT_TIMESTAMP
		| CURRENT_USER			/* added in FB 1.0 */
		| CURRENT_ROLE
		| RECREATE
		| CURRENT_CONNECTION	/* added in FB 1.5 */
		| CURRENT_TRANSACTION
		| BIGINT
		| CASE
		| RELEASE
		| ROW_COUNT
		| SAVEPOINT
		| OPEN					/* added in FB 2.0 */
		| CLOSE
		| FETCH
		| ROWS
		| USING
		| CROSS
		| BIT_LENGTH
		| BOTH
		| CHAR_LENGTH
		| CHARACTER_LENGTH
		| COMMENT
		| LEADING
		| KW_LOWER
		| OCTET_LENGTH
		| TRAILING
		| TRIM
		| CONNECT				/* added in FB 2.1 */
		| DISCONNECT
		| GLOBAL
		| INSENSITIVE
		| RECURSIVE
		| SENSITIVE
		| START
		| SIMILAR				/* added in FB 2.5 */
		| SQLSTATE
		;

col_opt	: ALTER
			{ $$ = NULL; }
		| ALTER COLUMN
			{ $$ = NULL; }
		;

alter_data_type_or_domain	: non_array_type
			{ $$ = NULL; }
		| simple_column_name
			{ $$ = make_node (nod_def_domain, (int) e_dom_count, $1, NULL, NULL, NULL); }
		;

alter_col_name	: simple_column_name
			{ lex.g_field_name = $1;
			  lex.g_field = make_field ($1);
			  $$ = (dsql_nod*) lex.g_field; }
		;

drop_behaviour	: RESTRICT
			{ $$ = make_node (nod_restrict, 0, NULL); }
		| CASCADE
			{ $$ = make_node (nod_cascade, 0, NULL); }
		|
			{ $$ = make_node (nod_restrict, 0, NULL); }
		;

alter_index_clause	: symbol_index_name ACTIVE
				{ $$ = make_node (nod_idx_active, 1, $1); }
			| symbol_index_name INACTIVE
				{ $$ = make_node (nod_idx_inactive, 1, $1); }
			;

alter_sequence_clause	: symbol_generator_name RESTART WITH signed_long_integer
			{ $$ = make_node (nod_set_generator2, e_gen_id_count, $1,
				MAKE_const_slong ((IPTR) $4)); }
		| symbol_generator_name RESTART WITH NUMBER64BIT
			{ $$ = make_node (nod_set_generator2, e_gen_id_count, $1,
				MAKE_constant((dsql_str*) $4, CONSTANT_SINT64)); }
		| symbol_generator_name RESTART WITH '-' NUMBER64BIT
			{ $$ = make_node (nod_set_generator2, e_gen_id_count, $1,
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) $5, CONSTANT_SINT64))); }
		;

alter_udf_clause    : symbol_UDF_name entry_op module_op
			{ $$ = make_node(nod_mod_udf, e_mod_udf_count, $1, $2, $3); }
			;

/*
alter_role_clause	: symbol_role_name alter_role_action OS_NAME os_security_name
			{ $$ = make_node(nod_mod_role, e_mod_role_count, $4, $1, $2); }
			;

alter_role_action	: ADD
			{ $$ = MAKE_const_slong (isc_dyn_map_role); }
		| DROP
			{ $$ = MAKE_const_slong (isc_dyn_unmap_role); }
		;
 */

alter_role_clause	: symbol_role_name alter_role_enable AUTO ADMIN MAPPING
			{ $$ = make_node(nod_mod_role, e_mod_role_count, NULL, $1, $2); }
			;

alter_role_enable	: SET
			{ $$ = MAKE_const_slong (isc_dyn_automap_role); }
		| DROP
			{ $$ = MAKE_const_slong (isc_dyn_autounmap_role); }
		;
/*
os_security_name	: STRING
			{ $$ = $1; }
		;
*/
entry_op	: ENTRY_POINT sql_string
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

module_op	: MODULE_NAME sql_string
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;


/* ALTER DATABASE */

init_alter_db	:
			{ $$ = NULL; }
		;

alter_db	: db_alter_clause
		| alter_db db_alter_clause
				{ $$ = make_node (nod_list, (int) 2, $1, $2); }
		;

db_alter_clause : ADD db_file_list
			{ $$ = $2; }
		| ADD KW_DIFFERENCE KW_FILE sql_string
			{ $$ = make_node (nod_difference_file, (int) 1, $4); }
		| DROP KW_DIFFERENCE KW_FILE
			{ $$ = make_node (nod_drop_difference, (int) 0, NULL); }
		| BEGIN BACKUP
			{ $$ = make_node (nod_begin_backup, (int) 0, NULL); }
		| END BACKUP
			{ $$ = make_node (nod_end_backup, (int) 0, NULL); }
		;


/* ALTER TRIGGER */

alter_trigger_clause : symbol_trigger_name trigger_active
		new_trigger_type
		trigger_position
		begin_trigger
		new_trigger_action
		end_trigger
			{ $$ = make_node (nod_mod_trigger, (int) e_trg_count,
				$1, NULL, $2, $3, $4, $6, $7); }
		;

new_trigger_type : trigger_type
		|
			{ $$ = NULL; }
		;

new_trigger_action : trigger_action
		|
			{ $$ = NULL; }
		;

/* DROP metadata operations */

drop		: DROP drop_clause
			{ $$ = $2; }
				;

drop_clause	: EXCEPTION symbol_exception_name
			{ $$ = make_node (nod_del_exception, 1, $2); }
		| INDEX symbol_index_name
			{ $$ = make_node (nod_del_index, (int) 1, $2); }
		| PROCEDURE symbol_procedure_name
			{ $$ = make_node (nod_del_procedure, (int) 1, $2); }
		| TABLE symbol_table_name
			{ $$ = make_node (nod_del_relation, (int) 1, $2); }
		| TRIGGER symbol_trigger_name
			{ $$ = make_node (nod_del_trigger, (int) 1, $2); }
		| VIEW symbol_view_name
			{ $$ = make_node (nod_del_view, (int) 1, $2); }
		| FILTER symbol_filter_name
			{ $$ = make_node (nod_del_filter, (int) 1, $2); }
		| DOMAIN symbol_domain_name
			{ $$ = make_node (nod_del_domain, (int) 1, $2); }
		| EXTERNAL FUNCTION symbol_UDF_name
			{ $$ = make_node (nod_del_udf, (int) 1, $3); }
		| SHADOW pos_short_integer
			{ $$ = make_node (nod_del_shadow, (int) 1, $2); }
		| ROLE symbol_role_name
			{ $$ = make_node (nod_del_role, (int) 1, $2); }
		| GENERATOR symbol_generator_name
			{ $$ = make_node (nod_del_generator, (int) 1, $2); }
		| SEQUENCE symbol_generator_name
			{ $$ = make_node (nod_del_generator, (int) 1, $2); }
		| COLLATION symbol_collation_name
			{ $$ = make_node (nod_del_collation, (int) 1, $2); }
		| USER drop_user_clause
			{ $$ = $2; }
		;


/* these are the allowable datatypes */

data_type	: non_array_type
		| array_type
		;

domain_or_non_array_type
	:	domain_or_non_array_type_name
	|	domain_or_non_array_type_name NOT KW_NULL
			{ lex.g_field->fld_not_nullable = true; }
	;

domain_or_non_array_type_name
	:	non_array_type
	|	domain_type
	;

domain_type
	:	KW_TYPE OF symbol_column_name
			{ lex.g_field->fld_type_of_name = ((dsql_str*) $3)->str_data; }
	|	KW_TYPE OF COLUMN symbol_column_name '.' symbol_column_name
			{
				lex.g_field->fld_type_of_name = ((dsql_str*) $6)->str_data;
				lex.g_field->fld_type_of_table = ((dsql_str*) $4)->str_data;
			}
	|	symbol_column_name
			{
				lex.g_field->fld_type_of_name = ((dsql_str*) $1)->str_data;
				lex.g_field->fld_full_domain = true;
			}
	;


non_array_type	: simple_type
		| blob_type
		;

array_type	: non_charset_simple_type '[' array_spec ']'
			{ lex.g_field->fld_ranges = make_list ($3);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  $$ = $1; }
		| character_type '[' array_spec ']' charset_clause
			{ lex.g_field->fld_ranges = make_list ($3);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  $$ = $1; }
		;

array_spec	: array_range
		| array_spec ',' array_range
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

array_range	: signed_long_integer
				{ if ((IPTR) $1 < 1)
			 		$$ = make_node (nod_list, (int) 2,
					MAKE_const_slong ((IPTR) $1),
					MAKE_const_slong (1));
				  else
			 		$$ = make_node (nod_list, (int) 2,
			   		MAKE_const_slong (1),
					MAKE_const_slong ((IPTR) $1) ); }
		| signed_long_integer ':' signed_long_integer
				{ $$ = make_node (nod_list, (int) 2,
			 	MAKE_const_slong ((IPTR) $1),
				MAKE_const_slong ((IPTR) $3)); }
		;

simple_type	: non_charset_simple_type
		| character_type charset_clause
		;

non_charset_simple_type	: national_character_type
		| numeric_type
		| float_type
		| BIGINT
			{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
																	Arg::Str("BIGINT"));
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
																	Arg::Str("BIGINT"));
			lex.g_field->fld_dtype = dtype_int64;
			lex.g_field->fld_length = sizeof (SINT64);
			}
		| integer_keyword
			{
			lex.g_field->fld_dtype = dtype_long;
			lex.g_field->fld_length = sizeof (SLONG);
			}
		| SMALLINT
			{
			lex.g_field->fld_dtype = dtype_short;
			lex.g_field->fld_length = sizeof (SSHORT);
			}
		| DATE
			{
			stmt_ambiguous = true;
			if (client_dialect <= SQL_DIALECT_V5)
				{
				/* Post warning saying that DATE is equivalent to TIMESTAMP */
				ERRD_post_warning(Arg::Warning(isc_sqlwarn) << Arg::Num(301) <<
								  Arg::Warning(isc_dtype_renamed));
				lex.g_field->fld_dtype = dtype_timestamp;
				lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
				}
			else if (client_dialect == SQL_DIALECT_V6_TRANSITION)
				yyabandon (-104, isc_transitional_date);
			else
				{
				lex.g_field->fld_dtype = dtype_sql_date;
				lex.g_field->fld_length = sizeof (ULONG);
				}
			}
		| TIME
			{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
																		  Arg::Str("TIME"));
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
																		  Arg::Str("TIME"));
			lex.g_field->fld_dtype = dtype_sql_time;
			lex.g_field->fld_length = sizeof (SLONG);
			}
		| TIMESTAMP
			{
			lex.g_field->fld_dtype = dtype_timestamp;
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
		;

integer_keyword	: INTEGER
		| KW_INT
		;


/* allow a blob to be specified with any combination of
   segment length and subtype */

blob_type	: BLOB blob_subtype blob_segsize charset_clause
			{
			lex.g_field->fld_dtype = dtype_blob;
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
		| BLOB '(' unsigned_short_integer ')'
			{
			lex.g_field->fld_dtype = dtype_blob;
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) $3;
			lex.g_field->fld_sub_type = 0;
			}
		| BLOB '(' unsigned_short_integer ',' signed_short_integer ')'
			{
			lex.g_field->fld_dtype = dtype_blob;
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) $3;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) $5;
			}
		| BLOB '(' ',' signed_short_integer ')'
			{
			lex.g_field->fld_dtype = dtype_blob;
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) $4;
			}
		;

blob_segsize	: SEGMENT KW_SIZE unsigned_short_integer
		  	{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) $3;
		  	}
		|
		  	{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
		;

blob_subtype	: SUB_TYPE signed_short_integer
			{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) $2;
			}
		| SUB_TYPE symbol_blob_subtype_name
			{
			lex.g_field->fld_sub_type_name = $2;
			}
		|
			{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
		;

charset_clause	: CHARACTER SET symbol_character_set_name
			{
			lex.g_field->fld_character_set = $3;
			}
		|
		;


/* character type */


national_character_type	: national_character_keyword '(' pos_short_integer ')'
			{
			lex.g_field->fld_dtype = dtype_text;
			lex.g_field->fld_character_length = (USHORT)(IPTR) $3;
			lex.g_field->fld_flags |= FLD_national;
			}
		| national_character_keyword
			{
			lex.g_field->fld_dtype = dtype_text;
			lex.g_field->fld_character_length = 1;
			lex.g_field->fld_flags |= FLD_national;
			}
		| national_character_keyword VARYING '(' pos_short_integer ')'
			{
			lex.g_field->fld_dtype = dtype_varying;
			lex.g_field->fld_character_length = (USHORT)(IPTR) $4;
			lex.g_field->fld_flags |= FLD_national;
			}
		;

character_type	: character_keyword '(' pos_short_integer ')'
			{
			lex.g_field->fld_dtype = dtype_text;
			lex.g_field->fld_character_length = (USHORT)(IPTR) $3;
			}
		| character_keyword
			{
			lex.g_field->fld_dtype = dtype_text;
			lex.g_field->fld_character_length = 1;
			}
		| varying_keyword '(' pos_short_integer ')'
			{
			lex.g_field->fld_dtype = dtype_varying;
			lex.g_field->fld_character_length = (USHORT)(IPTR) $3;
			}
		;

varying_keyword 	   : VARCHAR
			   | CHARACTER VARYING
			   | KW_CHAR VARYING
			   ;

character_keyword 	   : CHARACTER
			   | KW_CHAR
			   ;

national_character_keyword : NCHAR
			   | NATIONAL CHARACTER
			   | NATIONAL KW_CHAR
			   ;



/* numeric type */

numeric_type	: KW_NUMERIC prec_scale
						{
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
		| decimal_keyword prec_scale
			{
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
		;

prec_scale	:
			{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				lex.g_field->fld_precision = 9;
			}
		| '(' signed_long_integer ')'
			{
			if ( ((IPTR) $2 < 1) || ((IPTR) $2 > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision most be between 1 and 18. */
			if ((IPTR) $2 > 9)
			{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect > SQL_DIALECT_V5) ) ||
				 ( (client_dialect > SQL_DIALECT_V5) &&
				   (db_dialect <= SQL_DIALECT_V5) ) )
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-817) <<
							  Arg::Gds(isc_ddl_not_allowed_by_db_sql_dial) << Arg::Num(db_dialect));
				if (client_dialect <= SQL_DIALECT_V5)
				{
					lex.g_field->fld_dtype = dtype_double;
					lex.g_field->fld_length = sizeof (double);
				}
				else
				{
					if (client_dialect == SQL_DIALECT_V6_TRANSITION)
					{
						ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous));
						ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous1));
						ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous2));
					}
					lex.g_field->fld_dtype = dtype_int64;
					lex.g_field->fld_length = sizeof (SINT64);
				}
			}
			else
			{
				if ((IPTR) $2 < 5)
				{
					lex.g_field->fld_dtype = dtype_short;
					lex.g_field->fld_length = sizeof (SSHORT);
				}
				else
				{
					lex.g_field->fld_dtype = dtype_long;
					lex.g_field->fld_length = sizeof (SLONG);
				}
			}
			lex.g_field->fld_precision = (USHORT)(IPTR) $2;
			}
		| '(' signed_long_integer ',' signed_long_integer ')'
			{
			if ( ((IPTR) $2 < 1) || ((IPTR) $2 > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision should be between 1 and 18 */
			if (((IPTR) $4 > (IPTR) $2) || ((IPTR) $4 < 0))
				yyabandon (-842, isc_scale_nogt);
				/* Scale must be between 0 and precision */
			if ((IPTR) $2 > 9)
			{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect > SQL_DIALECT_V5) ) ||
				 ( (client_dialect > SQL_DIALECT_V5) &&
				   (db_dialect <= SQL_DIALECT_V5) ) )
				{
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-817) <<
							  Arg::Gds(isc_ddl_not_allowed_by_db_sql_dial) << Arg::Num(db_dialect));
				}
				if (client_dialect <= SQL_DIALECT_V5)
				{
					lex.g_field->fld_dtype = dtype_double;
					lex.g_field->fld_length = sizeof (double);
				}
				else
				{
					if (client_dialect == SQL_DIALECT_V6_TRANSITION)
					{
						ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous));
						ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous1));
						ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous2));
					}
					/* client_dialect >= SQL_DIALECT_V6 */
					lex.g_field->fld_dtype = dtype_int64;
					lex.g_field->fld_length = sizeof (SINT64);
				}
			}
			else
			{
				if ((IPTR) $2 < 5)
				{
					lex.g_field->fld_dtype = dtype_short;
					lex.g_field->fld_length = sizeof (SSHORT);
				}
				else
				{
					lex.g_field->fld_dtype = dtype_long;
					lex.g_field->fld_length = sizeof (SLONG);
				}
			}
			lex.g_field->fld_precision = (USHORT)(IPTR) $2;
			lex.g_field->fld_scale = - (SSHORT)(IPTR) $4;
			}
		;

decimal_keyword	: DECIMAL
		| KW_DEC
		;



/* floating point type */

float_type	: KW_FLOAT precision_opt
			{
			if ((IPTR) $2 > 7)
				{
				lex.g_field->fld_dtype = dtype_double;
				lex.g_field->fld_length = sizeof (double);
				}
			else
				{
				lex.g_field->fld_dtype = dtype_real;
				lex.g_field->fld_length = sizeof (float);
				}
			}
		| KW_LONG KW_FLOAT precision_opt
			{
			lex.g_field->fld_dtype = dtype_double;
			lex.g_field->fld_length = sizeof (double);
			}
		| REAL
			{
			lex.g_field->fld_dtype = dtype_real;
			lex.g_field->fld_length = sizeof (float);
			}
		| KW_DOUBLE PRECISION
			{
			lex.g_field->fld_dtype = dtype_double;
			lex.g_field->fld_length = sizeof (double);
			}
		;

precision_opt	: '(' nonneg_short_integer ')'
			{ $$ = $2; }
		|
			{ $$ = 0; }
		;



/* SET statements */
set		: set_transaction
		| set_generator
		| set_statistics
		;


set_generator	: SET GENERATOR symbol_generator_name TO signed_long_integer
			{ $$ = make_node (nod_set_generator2, e_gen_id_count, $3,
				MAKE_const_slong ((IPTR) $5)); }
		| SET GENERATOR symbol_generator_name TO NUMBER64BIT
			{ $$ = make_node (nod_set_generator2, e_gen_id_count, $3,
				MAKE_constant((dsql_str*) $5, CONSTANT_SINT64)); }
		| SET GENERATOR symbol_generator_name TO '-' NUMBER64BIT
			{ $$ = make_node (nod_set_generator2, e_gen_id_count, $3,
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) $6, CONSTANT_SINT64))); }
		;


/* transaction statements */

savepoint	: set_savepoint
		| release_savepoint
		| undo_savepoint
		;

set_savepoint : SAVEPOINT symbol_savepoint_name
			{ $$ = make_node (nod_user_savepoint, 1, $2); }
		;

release_savepoint	: RELEASE SAVEPOINT symbol_savepoint_name release_only_opt
			{ $$ = make_node (nod_release_savepoint, 2, $3, $4); }
		;

release_only_opt	: ONLY
			{ $$ = make_node (nod_flag, 0, NULL); }
		|
			{ $$ = 0; }
		;

undo_savepoint : ROLLBACK optional_work TO optional_savepoint symbol_savepoint_name
			{ $$ = make_node (nod_undo_savepoint, 1, $5); }
		;

optional_savepoint	: SAVEPOINT
		|
		;

commit		: COMMIT optional_work optional_retain
			{ $$ = make_node (nod_commit, e_commit_count, $3); }
		;

dump		: DUMP
			{
#ifdef POOL_DUMP
			  Firebird::MemoryPool::printAll();
			  yyerror("Pseudo-error: DUMP complete");
#else
			  yyerror("DUMP not supported");
#endif
			  $$ = NULL; }
		;

rollback	: ROLLBACK optional_work optional_retain
			{ $$ = make_node (nod_rollback, e_rollback_count, $3); }
		;

optional_work	: WORK
		|
		;

optional_retain	: RETAIN opt_snapshot
			{ $$ = make_node (nod_retain, 0, NULL); }
		|
		 	{ $$ = NULL; }
		;

opt_snapshot	: SNAPSHOT
		|
		 	{ $$ = NULL; }
		;

set_transaction	: SET TRANSACTION tran_opt_list_m
			{$$ = make_node (nod_trans, 1, make_list ($3)); }
		;

tran_opt_list_m	: tran_opt_list
		|
		 	{ $$ = NULL; }
		;

tran_opt_list	: tran_opt
		| tran_opt_list tran_opt
			{ $$ = make_node (nod_list, (int) 2, $1, $2); }
		;

tran_opt	: access_mode
		| lock_wait
		| isolation_mode
		| tra_misc_options
		| tra_timeout
		| tbl_reserve_options
		;

access_mode	: READ ONLY
			{ $$ = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
		| READ WRITE
			{ $$ = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
		;

lock_wait	: WAIT
			{ $$ = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
		| NO WAIT
			{ $$ = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
		;

isolation_mode	: ISOLATION LEVEL iso_mode
			{ $$ = $3;}
		| iso_mode
		;

iso_mode	: snap_shot
			{ $$ = $1;}
		| READ UNCOMMITTED version_mode
			{ $$ = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, $3); }
		| READ COMMITTED version_mode
			{ $$ = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, $3); }
		;

snap_shot	: SNAPSHOT
			{ $$ = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
		| SNAPSHOT TABLE
			{ $$ = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
		| SNAPSHOT TABLE STABILITY
			{ $$ = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
		;

version_mode	: VERSION
			{ $$ = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
		| NO VERSION
			{ $$ = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
		|
			{ $$ = 0; }
		;

tra_misc_options: NO AUTO UNDO
			{ $$ = make_flag_node(nod_tra_misc, NOD_NO_AUTO_UNDO, 0, NULL); }
		| KW_IGNORE LIMBO
			{ $$ = make_flag_node(nod_tra_misc, NOD_IGNORE_LIMBO, 0, NULL); }
		| RESTART REQUESTS
			{ $$ = make_flag_node(nod_tra_misc, NOD_RESTART_REQUESTS, 0, NULL); }
		;

tra_timeout: LOCK TIMEOUT nonneg_short_integer
			{ $$ = make_node(nod_lock_timeout, 1, MAKE_const_slong ((IPTR) $3)); }
		;

tbl_reserve_options: RESERVING restr_list
			{ $$ = make_node (nod_reserve, 1, make_list ($2)); }
		;

lock_type	: KW_SHARED
			{ $$ = (dsql_nod*) NOD_SHARED; }
		| PROTECTED
			{ $$ = (dsql_nod*) NOD_PROTECTED; }
		|
			{ $$ = (dsql_nod*) 0; }
		;

lock_mode	: READ
			{ $$ = (dsql_nod*) NOD_READ; }
		| WRITE
			{ $$ = (dsql_nod*) NOD_WRITE; }
		;

restr_list	: restr_option
		| restr_list ',' restr_option
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

restr_option	: table_list table_lock
			{ $$ = make_node (nod_table_lock, (int) 2, make_list ($1), $2); }
		;

table_lock	: FOR lock_type lock_mode
			{ $$ = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) $2 | (SSHORT)(IPTR) $3), (SSHORT) 0, NULL); }
		|
			{ $$ = 0; }
		;

table_list	: simple_table_name
		| table_list ',' simple_table_name
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;


set_statistics	: SET STATISTICS INDEX symbol_index_name
				{ $$ = make_node (nod_set_statistics, (int) e_stat_count, $4); }
			;

comment		: COMMENT ON ddl_type0 IS ddl_desc
				{ $$ = make_node(nod_comment, e_comment_count, $3, NULL, NULL, $5); }
			| COMMENT ON ddl_type1 symbol_ddl_name IS ddl_desc
				{ $$ = make_node(nod_comment, e_comment_count, $3, $4, NULL, $6); }
			| COMMENT ON ddl_type2 symbol_ddl_name ddl_subname IS ddl_desc
				{ $$ = make_node(nod_comment, e_comment_count, $3, $4, $5, $7); }
			;

ddl_type0	: DATABASE
				{ $$ = MAKE_const_slong(ddl_database); }
			;

ddl_type1	: DOMAIN
				{ $$ = MAKE_const_slong(ddl_domain); }
			| TABLE
				{ $$ = MAKE_const_slong(ddl_relation); }
			| VIEW
				{ $$ = MAKE_const_slong(ddl_view); }
			| PROCEDURE
				{ $$ = MAKE_const_slong(ddl_procedure); }
			| TRIGGER
				{ $$ = MAKE_const_slong(ddl_trigger); }
			| EXTERNAL FUNCTION
				{ $$ = MAKE_const_slong(ddl_udf); }
			| FILTER
				{ $$ = MAKE_const_slong(ddl_blob_filter); }
			| EXCEPTION
				{ $$ = MAKE_const_slong(ddl_exception); }
			| GENERATOR
				{ $$ = MAKE_const_slong(ddl_generator); }
			| SEQUENCE
				{ $$ = MAKE_const_slong(ddl_generator); }
			| INDEX
				{ $$ = MAKE_const_slong(ddl_index); }
			| ROLE
				{ $$ = MAKE_const_slong(ddl_role); }
			| CHARACTER SET
				{ $$ = MAKE_const_slong(ddl_charset); }
			| COLLATION
				{ $$ = MAKE_const_slong(ddl_collation); }
/*
			| SECURITY CLASS
				{ $$ = MAKE_const_slong(ddl_sec_class); }
*/
			;

ddl_type2	: COLUMN
				{ $$ = MAKE_const_slong(ddl_relation); }
			| PARAMETER
				{ $$ = MAKE_const_slong(ddl_procedure); }
			;

ddl_subname	: '.' symbol_ddl_name
				{ $$ = $2; }
			;

ddl_desc    : sql_string
			| KW_NULL
			    { $$ = NULL; }
			;


/* SELECT statement */

select		: select_expr for_update_clause lock_clause
			{ $$ = make_node (nod_select, (int) e_select_count, $1, $2, $3); }
		;

for_update_clause : FOR UPDATE for_update_list
			{ $$ = make_node (nod_for_update, (int) e_fpd_count, $3); }
		|
			{ $$ = NULL; }
		;

for_update_list	: OF column_list
			{ $$ = $2; }
		|
			{ $$ = make_node (nod_flag, 0, NULL); }
		;

lock_clause : WITH LOCK
			{ $$ = make_node (nod_flag, 0, NULL); }
		|
			{ $$ = NULL; }
		;


/* SELECT expression */

select_expr	: with_clause select_expr_body order_clause rows_clause
				{ $$ = make_node (nod_select_expr, (int) e_sel_count, $2, $3, $4, $1); }
 		;

with_clause	: WITH RECURSIVE with_list
				{ $$ = make_flag_node (nod_with, NOD_UNION_RECURSIVE, 1, make_list($3)); }
			| WITH with_list
				{ $$ = make_node (nod_with, 1, make_list($2)); }
		    |
				{ $$ = NULL; }
 		;

with_list	: with_item
			| with_item ',' with_list
				{ $$ = make_node (nod_list, 2, $1, $3); }
		;

with_item	: symbol_table_alias_name derived_column_list AS '(' select_expr ')'
				{ $$ = make_node (nod_derived_table, (int) e_derived_table_count, $5, $1, $2, NULL); }
		;

column_select	: with_clause select_expr_body order_clause rows_clause
			{ $$ = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, $2, $3, $4, $1); }
		;

column_singleton	: with_clause select_expr_body order_clause rows_clause
			{ $$ = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, $2, $3, $4, $1); }
		;

select_expr_body	: query_term
		| select_expr_body UNION distinct_noise query_term
			{ $$ = make_node (nod_list, 2, $1, $4); }
		| select_expr_body UNION ALL query_term
			{ $$ = make_flag_node (nod_list, NOD_UNION_ALL, 2, $1, $4); }
		;

query_term	: query_spec
		;

query_spec	: SELECT limit_clause
			 distinct_clause
			 select_list
			 from_clause
			 where_clause
			 group_clause
			 having_clause
			 plan_clause
			{ $$ = make_node (nod_query_spec, (int) e_qry_count,
					$2, $3, $4, $5, $6, $7, $8, $9); }
		;

limit_clause	: first_clause skip_clause
			{ $$ = make_node (nod_limit, (int) e_limit_count, $2, $1); }
		|   first_clause
			{ $$ = make_node (nod_limit, (int) e_limit_count, NULL, $1); }
		|   skip_clause
			{ $$ = make_node (nod_limit, (int) e_limit_count, $1, NULL); }
		|
			{ $$ = 0; }
		;

first_clause	: FIRST long_integer
			{ $$ = MAKE_const_slong ((IPTR) $2); }
		| FIRST '(' value ')'
			{ $$ = $3; }
		| FIRST parameter
			{ $$ = $2; }
		;

skip_clause	: SKIP long_integer
			{ $$ = MAKE_const_slong ((IPTR) $2); }
		| SKIP '(' value ')'
			{ $$ = $3; }
		| SKIP parameter
			{ $$ = $2; }
		;

distinct_clause	: DISTINCT
			{ $$ = make_node (nod_flag, 0, NULL); }
		| all_noise
			{ $$ = 0; }
		;

select_list	: select_items
			{ $$ = make_list ($1); }
		| '*'
			{ $$ = 0; }
		;

select_items	: select_item
		| select_items ',' select_item
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

select_item	: value
		| value as_noise symbol_item_alias_name
			{ $$ = make_node (nod_alias, 2, $1, $3); }
		;

as_noise : AS
		|
		;

/* FROM clause */

from_clause	: FROM from_list
		 	{ $$ = make_list ($2); }
		;

from_list	: table_reference
		| from_list ',' table_reference
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

table_reference	: joined_table
		| table_primary
		;

table_primary	: table_proc
		| derived_table
		| '(' joined_table ')'
			{ $$ = $2; }
		;

// AB: derived table support
derived_table :
		'(' select_expr ')' as_noise correlation_name derived_column_list
			{ $$ = make_node(nod_derived_table, (int) e_derived_table_count, $2, $5, $6, NULL); }
		;

correlation_name : symbol_table_alias_name
		|
			{ $$ = NULL; }
		;

derived_column_list : '(' alias_list ')'
			{ $$ = make_list ($2); }
		|
			{ $$ = NULL; }
		;

alias_list : symbol_item_alias_name
		| alias_list ',' symbol_item_alias_name
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

joined_table	: cross_join
		| natural_join
		| qualified_join
		;

cross_join	: table_reference CROSS JOIN table_primary
			{ $$ = make_node (nod_join, (int) e_join_count, $1,
				make_node (nod_join_inner, (int) 0, NULL), $4, NULL); }
		;

natural_join	: table_reference NATURAL join_type JOIN table_primary
			{ $$ = make_node (nod_join, (int) e_join_count, $1, $3, $5,
					make_node (nod_flag, 0, NULL)); }
		;

qualified_join	: table_reference join_type JOIN table_reference join_specification
			{ $$ = make_node (nod_join, (int) e_join_count, $1, $2, $4, $5); }
		;

join_specification	: join_condition
		| named_columns_join
		;

join_condition	: ON search_condition
			{ $$ = $2; }
		;

named_columns_join	: USING '(' column_list ')'
			{ $$ = make_list ($3); }
		;

table_proc	: symbol_procedure_name table_proc_inputs as_noise symbol_table_alias_name
			{ $$ = make_node (nod_rel_proc_name,
					(int) e_rpn_count, $1, $4, $2); }
		| symbol_procedure_name table_proc_inputs
			{ $$ = make_node (nod_rel_proc_name,
					(int) e_rpn_count, $1, NULL, $2); }
		;

table_proc_inputs	: '(' value_list ')'
				{ $$ = make_list ($2); }
			|
				{ $$ = NULL; }
			;

table_name	: simple_table_name
		| symbol_table_name as_noise symbol_table_alias_name
			{ $$ = make_node (nod_relation_name,
						(int) e_rln_count, $1, $3); }
		;

simple_table_name: symbol_table_name
			{ $$ = make_node (nod_relation_name,
						(int) e_rln_count, $1, NULL); }
		;

join_type	: INNER
			{ $$ = make_node (nod_join_inner, (int) 0, NULL); }
		| LEFT outer_noise
			{ $$ = make_node (nod_join_left, (int) 0, NULL); }
		| RIGHT outer_noise
			{ $$ = make_node (nod_join_right, (int) 0, NULL); }
		| FULL outer_noise
			{ $$ = make_node (nod_join_full, (int) 0, NULL); }
		|
			{ $$ = make_node (nod_join_inner, (int) 0, NULL); }
		;

outer_noise	: OUTER
		|
		;


/* other clauses in the select expression */

group_clause	: GROUP BY group_by_list
			{ $$ = make_list ($3); }
		|
			{ $$ = NULL; }
		;

group_by_list	: group_by_item
		| group_by_list ',' group_by_item
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

/* Except aggregate-functions are all expressions supported in group_by_item,
   they are caught inside pass1.cpp */
group_by_item : value
		;

having_clause	: HAVING search_condition
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

where_clause	: WHERE search_condition
		 	{ $$ = $2; }
		|
			{ $$ = NULL; }
		;


/* PLAN clause to specify an access plan for a query */

plan_clause	: PLAN plan_expression
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

plan_expression	: plan_type '(' plan_item_list ')'
			{ $$ = make_node (nod_plan_expr, 2, $1, make_list ($3)); }
		;

plan_type	: JOIN
			{ $$ = 0; }
		| SORT MERGE
			{ $$ = make_node (nod_merge_plan, (int) 0, NULL); }
		| MERGE
			{ $$ = make_node (nod_merge_plan, (int) 0, NULL); }

		/* for now the SORT operator is a no-op; it does not
		   change the place where a sort happens, but is just intended
		   to read the output from a SET PLAN */
		| SORT
			{ $$ = 0; }
		|
			{ $$ = 0; }
		;

plan_item_list	: plan_item
		| plan_item ',' plan_item_list
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

plan_item	: table_or_alias_list access_type
			{ $$ = make_node (nod_plan_item, 2, make_list ($1), $2); }
		| plan_expression
		;

table_or_alias_list : symbol_table_name
		| symbol_table_name table_or_alias_list
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

access_type	: NATURAL
			{ $$ = make_node (nod_natural, (int) 0, NULL); }
		| INDEX '(' index_list ')'
			{ $$ = make_node (nod_index, 1, make_list ($3)); }
		| ORDER symbol_index_name extra_indices_opt
			{ $$ = make_node (nod_index_order, 2, $2, $3); }
		;

index_list	: symbol_index_name
		| symbol_index_name ',' index_list
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

extra_indices_opt	: INDEX '(' index_list ')'
			{ $$ = make_list ($3); }
		|
			{ $$ = 0; }
		;

/* ORDER BY clause */

order_clause	: ORDER BY order_list
			{ $$ = make_list ($3); }
		|
			{ $$ = 0; }
		;

order_list	: order_item
		| order_list ',' order_item
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

order_item	: value order_direction nulls_clause
			{ $$ = make_node (nod_order, (int) e_order_count, $1, $2, $3); }
		;

order_direction	: ASC
			{ $$ = 0; }
		| DESC
			{ $$ = make_node (nod_flag, 0, NULL); }
		|
			{ $$ = 0; }
		;

nulls_clause : NULLS nulls_placement
			{ $$ = $2; }
		|
			{ $$ = 0; }
		;

nulls_placement : FIRST
			{ $$ = MAKE_const_slong(NOD_NULLS_FIRST); }
		| LAST
			{ $$ = MAKE_const_slong(NOD_NULLS_LAST); }
		;

/* ROWS clause */

rows_clause	: ROWS value
			/* equivalent to FIRST value */
			{ $$ = make_node (nod_rows, (int) e_rows_count, NULL, $2); }
		| ROWS value TO value
			/* equivalent to FIRST (upper_value - lower_value + 1) SKIP (lower_value - 1) */
			{ $$ = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, $2,
					MAKE_const_slong (1)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, $4, $2),
					MAKE_const_slong (1))); }
		|
			{ $$ = NULL; }
		;


/* INSERT statement */
/* IBO hack: replace column_parens_opt by ins_column_parens_opt. */
insert		: INSERT INTO simple_table_name ins_column_parens_opt
				VALUES '(' value_list ')' returning_clause
			{ $$ = make_node (nod_insert, (int) e_ins_count,
				$3, $4, make_list ($7), NULL, $9); }
		| INSERT INTO simple_table_name ins_column_parens_opt select_expr returning_clause
			{ $$ = make_node (nod_insert, (int) e_ins_count,
				$3, $4, NULL, $5, $6); }
		| INSERT INTO simple_table_name DEFAULT VALUES returning_clause
			{ $$ = make_node (nod_insert, (int) e_ins_count,
				$3, NULL, NULL, NULL, $6); }
		;


/* MERGE statement */
merge
	:	MERGE INTO table_name USING table_reference ON search_condition
			merge_when_clause
		{
			$$ = make_node(nod_merge, e_mrg_count, $3, $5, $7, $8);
		}
	;

merge_when_clause
	:	merge_when_matched_clause merge_when_not_matched_clause
		{ $$ = make_node(nod_merge_when, e_mrg_when_count, $1, $2); }
	|	merge_when_not_matched_clause merge_when_matched_clause
		{ $$ = make_node(nod_merge_when, e_mrg_when_count, $2, $1); }
	|	merge_when_matched_clause
		{ $$ = make_node(nod_merge_when, e_mrg_when_count, $1, NULL); }
	|	merge_when_not_matched_clause
		{ $$ = make_node(nod_merge_when, e_mrg_when_count, NULL, $1); }
	;

merge_when_matched_clause
	:	WHEN MATCHED THEN merge_update_specification
		{ $$ = $4; }
	;

merge_when_not_matched_clause
	:	WHEN NOT MATCHED THEN merge_insert_specification
		{ $$ = $5; }
	;

merge_update_specification
	:	UPDATE SET assignments
		{ $$ = make_node(nod_merge_update, e_mrg_update_count, make_list($3)); }
	;

merge_insert_specification
	:	INSERT ins_column_parens_opt VALUES '(' value_list ')'
		{ $$ = make_node(nod_merge_insert, e_mrg_insert_count, make_list($2), make_list($5)); }
	;


/* DELETE statement */

delete		: delete_searched
		| delete_positioned
		;

delete_searched	: KW_DELETE FROM table_name where_clause
		plan_clause order_clause rows_clause returning_clause
			{ $$ = make_node (nod_delete, (int) e_del_count,
				$3, $4, $5, $6, $7, NULL, $8); }
		;

delete_positioned : KW_DELETE FROM table_name cursor_clause
			{ $$ = make_node (nod_delete, (int) e_del_count,
				$3, NULL, NULL, NULL, NULL, $4, NULL); }
		;


/* UPDATE statement */

update		: update_searched
		| update_positioned
		;

update_searched	: UPDATE table_name SET assignments where_clause
		plan_clause order_clause rows_clause returning_clause
			{ $$ = make_node (nod_update, (int) e_upd_count,
				$2, make_list ($4), $5, $6, $7, $8, NULL, $9, NULL); }
		  	;

update_positioned : UPDATE table_name SET assignments cursor_clause
			{ $$ = make_node (nod_update, (int) e_upd_count,
				$2, make_list ($4), NULL, NULL, NULL, NULL, $5, NULL, NULL); }
		;


/* UPDATE OR INSERT statement */

update_or_insert
	:	UPDATE OR INSERT INTO simple_table_name ins_column_parens_opt
			VALUES '(' value_list ')'
			update_or_insert_matching_opt
			returning_clause
		{
			$$ = make_node (nod_update_or_insert, (int) e_upi_count,
				$5, make_list ($6), make_list ($9), $11, $12);
		}
	;

update_or_insert_matching_opt
	:	MATCHING ins_column_parens
		{ $$ = $2; }
	|
		{ $$ = NULL; }
	;


returning_clause	: RETURNING value_list
			{ $$ = make_node (nod_returning, (int) e_ret_count,
					make_list ($2), NULL); }
		| RETURNING value_list INTO variable_list
			{ $$ = make_node (nod_returning, (int) e_ret_count,
					make_list ($2), make_list ($4)); }
		|
			{ $$ = NULL; }
		;

cursor_clause	: WHERE CURRENT OF symbol_cursor_name
			{ $$ = make_node (nod_cursor, (int) e_cur_count, $4, NULL, NULL, NULL); }
		;


/* Assignments */

assignments	: assignment
		| assignments ',' assignment
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

assignment	: update_column_name '=' value
			{ $$ = make_node (nod_assign, e_asgn_count, $3, $1); }
		;

exec_function
	: udf
		{ $$ = make_node (nod_assign, e_asgn_count, $1, make_node (nod_null, 0, NULL)); }
	| non_aggregate_function
		{ $$ = make_node (nod_assign, e_asgn_count, $1, make_node (nod_null, 0, NULL)); }
	;


/* BLOB get and put */

blob_io			: READ BLOB simple_column_name FROM simple_table_name filter_clause_io segment_clause_io
			{ $$ = make_node (nod_get_segment, (int) e_blb_count, $3, $5, $6, $7); }
				| INSERT BLOB simple_column_name INTO simple_table_name filter_clause_io segment_clause_io
			{ $$ = make_node (nod_put_segment, (int) e_blb_count, $3, $5, $6, $7); }
		;

filter_clause_io	: FILTER FROM blob_subtype_value_io TO blob_subtype_value_io
			{ $$ = make_node (nod_list, 2, $3, $5); }
		| FILTER TO blob_subtype_value_io
			{ $$ = make_node (nod_list, 2, NULL, $3); }
		|
			{ $$ = NULL; }
		;

blob_subtype_value_io : blob_subtype_io
		| parameter
		;

blob_subtype_io	: signed_short_integer
			{ $$ = MAKE_const_slong ((IPTR) $1); }
		;

segment_clause_io	: MAX_SEGMENT segment_length_io
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

segment_length_io	: unsigned_short_integer
			{ $$ = MAKE_const_slong ((IPTR) $1); }
		| parameter
		;


/* column specifications */

column_parens_opt : column_parens
		|
			{ $$ = NULL; }
		;

column_parens	: '(' column_list ')'
			{ $$ = make_list ($2); }
		;

column_list	: simple_column_name
		| column_list ',' simple_column_name
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

/* begin IBO hack */
ins_column_parens_opt : ins_column_parens
		|
			{ $$ = NULL; }
		;

ins_column_parens	: '(' ins_column_list ')'
			{ $$ = make_list ($2); }
		;

ins_column_list	: update_column_name
		| ins_column_list ',' update_column_name
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;
/* end IBO hack */

column_name	 : simple_column_name
		| symbol_table_alias_name '.' symbol_column_name
			{ $$ = make_node (nod_field_name, (int) e_fln_count,
							$1, $3); }
		| symbol_table_alias_name '.' '*'
			{ $$ = make_node (nod_field_name, (int) e_fln_count,
							$1, NULL); }
		;

simple_column_name : symbol_column_name
			{ $$ = make_node (nod_field_name, (int) e_fln_count,
						NULL, $1); }
		;

update_column_name : simple_column_name
/* CVC: This option should be deprecated! The only allowed syntax should be
Update...set column = expr, without qualifier for the column. */
		| symbol_table_alias_name '.' symbol_column_name
			{ $$ = make_node (nod_field_name, (int) e_fln_count,
							$1, $3); }
		;

/* boolean expressions */

search_condition : predicate
		| search_condition OR search_condition
			{ $$ = make_node (nod_or, 2, $1, $3); }
		| search_condition AND search_condition
			{ $$ = make_node (nod_and, 2, $1, $3); }
		| NOT search_condition
			{ $$ = make_node (nod_not, 1, $2); }
		;

predicate : comparison_predicate
		| distinct_predicate
		| between_predicate
		| like_predicate
		| in_predicate
		| null_predicate
		| quantified_predicate
		| exists_predicate
		| containing_predicate
		| similar_predicate
		| starting_predicate
		| singular_predicate
		| trigger_action_predicate
		| '(' search_condition ')'
			{ $$ = $2; }
		;


/* comparisons */

comparison_predicate : value '=' value
			{ $$ = make_node (nod_eql, 2, $1, $3); }
		| value '<' value
			{ $$ = make_node (nod_lss, 2, $1, $3); }
		| value '>' value
			{ $$ = make_node (nod_gtr, 2, $1, $3); }
		| value GEQ value
			{ $$ = make_node (nod_geq, 2, $1, $3); }
		| value LEQ value
			{ $$ = make_node (nod_leq, 2, $1, $3); }
		| value NOT_GTR value
			{ $$ = make_node (nod_leq, 2, $1, $3); }
		| value NOT_LSS value
			{ $$ = make_node (nod_geq, 2, $1, $3); }
		| value NEQ value
			{ $$ = make_node (nod_neq, 2, $1, $3); }
		;

/* quantified comparisons */

quantified_predicate : value '=' ALL '(' column_select ')'
		{ $$ = make_node (nod_eql_all, 2, $1, $5); }
	| value '<' ALL '(' column_select ')'
		{ $$ = make_node (nod_lss_all, 2, $1, $5); }
	| value '>' ALL '(' column_select ')'
		{ $$ = make_node (nod_gtr_all, 2, $1, $5); }
	| value GEQ ALL '(' column_select ')'
		{ $$ = make_node (nod_geq_all, 2, $1, $5); }
	| value LEQ ALL '(' column_select ')'
		{ $$ = make_node (nod_leq_all, 2, $1, $5); }
	| value NOT_GTR ALL '(' column_select ')'
		{ $$ = make_node (nod_leq_all, 2, $1, $5); }
	| value NOT_LSS ALL '(' column_select ')'
		{ $$ = make_node (nod_geq_all, 2, $1, $5); }
	| value NEQ ALL '(' column_select ')'
		{ $$ = make_node (nod_neq_all, 2, $1, $5); }
	| value '=' some '(' column_select ')'
		{ $$ = make_node (nod_eql_any, 2, $1, $5); }
	| value '<' some '(' column_select ')'
		{ $$ = make_node (nod_lss_any, 2, $1, $5); }
	| value '>' some '(' column_select ')'
		{ $$ = make_node (nod_gtr_any, 2, $1, $5); }
	| value GEQ some '(' column_select ')'
		{ $$ = make_node (nod_geq_any, 2, $1, $5); }
	| value LEQ some '(' column_select ')'
		{ $$ = make_node (nod_leq_any, 2, $1, $5); }
	| value NOT_GTR some '(' column_select ')'
		{ $$ = make_node (nod_leq_any, 2, $1, $5); }
	| value NOT_LSS some '(' column_select ')'
		{ $$ = make_node (nod_geq_any, 2, $1, $5); }
	| value NEQ some '(' column_select ')'
		{ $$ = make_node (nod_neq_any, 2, $1, $5); }
	;

some	: SOME
	| ANY
	;


/* other predicates */

distinct_predicate : value IS DISTINCT FROM value
		{ $$ = make_node (nod_not, 1, make_node (nod_equiv, 2, $1, $5)); }
	| value IS NOT DISTINCT FROM value
		{ $$ = make_node (nod_equiv, 2, $1, $6); }
	;

between_predicate : value BETWEEN value AND value
		{ $$ = make_node (nod_between, 3, $1, $3, $5); }
	| value NOT BETWEEN value AND value
		{ $$ = make_node (nod_not, 1, make_node (nod_between,
						3, $1, $4, $6)); }
	;

like_predicate	: value LIKE value
		{ $$ = make_node (nod_like, 2, $1, $3); }
	| value NOT LIKE value
		{ $$ = make_node (nod_not, 1, make_node (nod_like, 2, $1, $4)); }
	| value LIKE value ESCAPE value
		{ $$ = make_node (nod_like, 3, $1, $3, $5); }
	| value NOT LIKE value ESCAPE value
		{ $$ = make_node (nod_not, 1, make_node (nod_like,
						3, $1, $4, $6)); }
	;

in_predicate	: value KW_IN in_predicate_value
		{ $$ = make_node (nod_eql_any, 2, $1, $3); }
	| value NOT KW_IN in_predicate_value
		{ $$ = make_node (nod_not, 1, make_node (nod_eql_any, 2, $1, $4)); }
	;

containing_predicate	: value CONTAINING value
		{ $$ = make_node (nod_containing, 2, $1, $3); }
	| value NOT CONTAINING value
		{ $$ = make_node (nod_not, 1, make_node (nod_containing, 2, $1, $4)); }
	;

similar_predicate
	: value SIMILAR TO value
		{ $$ = make_node(nod_similar, e_similar_count, $1, $4, NULL); }
	| value NOT SIMILAR TO value
		{ $$ = make_node(nod_not, 1, make_node(nod_similar, e_similar_count, $1, $5, NULL)); }
	| value SIMILAR TO value ESCAPE value
		{ $$ = make_node(nod_similar, e_similar_count, $1, $4, $6); }
	| value NOT SIMILAR TO value ESCAPE value
		{ $$ = make_node(nod_not, 1, make_node(nod_similar, e_similar_count, $1, $5, $7)); }
	;

starting_predicate	: value STARTING value
		{ $$ = make_node (nod_starting, 2, $1, $3); }
	| value NOT STARTING value
		{ $$ = make_node (nod_not, 1, make_node (nod_starting, 2, $1, $4)); }
	| value STARTING WITH value
		{ $$ = make_node (nod_starting, 2, $1, $4); }
	| value NOT STARTING WITH value
		{ $$ = make_node (nod_not, 1, make_node (nod_starting, 2, $1, $5)); }
	;

exists_predicate : EXISTS '(' select_expr ')'
		{ $$ = make_node (nod_exists, 1, $3); }
	;

singular_predicate : SINGULAR '(' select_expr ')'
		{ $$ = make_node (nod_singular, 1, $3); }
	;

null_predicate	: value IS KW_NULL
		{ $$ = make_node (nod_missing, 1, $1); }
	| value IS NOT KW_NULL
		{ $$ = make_node (nod_not, 1, make_node (nod_missing, 1, $1)); }
	;

trigger_action_predicate	: INSERTING
		{ $$ = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_trigger_action)),
						MAKE_const_slong (1)); }
	| UPDATING
		{ $$ = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_trigger_action)),
						MAKE_const_slong (2)); }
	| DELETING
		{ $$ = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_trigger_action)),
						MAKE_const_slong (3)); }
	;

/* set values */

in_predicate_value	: table_subquery
	| '(' value_list ')'
		{ $$ = make_list ($2); }
	;

table_subquery	: '(' column_select ')'
			{ $$ = $2; }
		;

/* USER control SQL interface */

create_user_clause : symbol_user_name passwd_clause firstname_opt middlename_opt lastname_opt grant_admin_opt
		{ $$ = make_node(nod_add_user, (int) e_user_count, $1, $2, $3, $4, $5, $6); }
	;

alter_user_clause : symbol_user_name passwd_opt firstname_opt middlename_opt lastname_opt admin_opt
		{ $$ = make_node(nod_mod_user, (int) e_user_count, $1, $2, $3, $4, $5, $6); }
	| symbol_user_name SET passwd_opt firstname_opt middlename_opt lastname_opt admin_opt
		{ $$ = make_node(nod_mod_user, (int) e_user_count, $1, $3, $4, $5, $6, $7); }
	;

drop_user_clause : symbol_user_name
		{ $$ = make_node(nod_del_user, (int) e_del_user_count, $1); }

passwd_clause : PASSWORD sql_string
		{ $$ = $2; }
	;

passwd_opt : passwd_clause
		{ $$ = $1; }
	|
		{ $$ = NULL; }
	;

firstname_opt : FIRSTNAME sql_string
		{ $$ = $2; }
	|
		{ $$ = NULL; }
	;

middlename_opt : MIDDLENAME sql_string
		{ $$ = $2; }
	|
		{ $$ = NULL; }
	;

lastname_opt : LASTNAME sql_string
		{ $$ = $2; }
	|
		{ $$ = NULL; }
	;

admin_opt : revoke_admin
		{ $$ = $1; }
	| grant_admin
		{ $$ = $1; }
	| 
		{ $$ = NULL; }
	;

grant_admin_opt : grant_admin
		{ $$ = $1; }
	| 
		{ $$ = NULL; }
	;

revoke_admin: REVOKE ADMIN ROLE
		{ $$ = (dsql_nod*) MAKE_cstring("0"); }
	;

grant_admin: GRANT ADMIN ROLE
		{ $$ = (dsql_nod*) MAKE_cstring("1"); }
	;

/* value types */

value	: column_name
		| array_element
		| function
		| u_constant
		| parameter
		| variable
		| cast_specification
		| case_expression
		| next_value_expression
		| udf
		| '-' value %prec UMINUS
			{ $$ = make_node (nod_negate, 1, $2); }
		| '+' value %prec UPLUS
			{ $$ = $2; }
		| value '+' value
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  $$ = make_node (nod_add2, 2, $1, $3);
			  else
				  $$ = make_node (nod_add, 2, $1, $3);
			}
		| value CONCATENATE value
			{ $$ = make_node (nod_concatenate, 2, $1, $3); }
		| value COLLATE symbol_collation_name
			{ $$ = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) $3, $1); }
		| value '-' value
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  $$ = make_node (nod_subtract2, 2, $1, $3);
			  else
				  $$ = make_node (nod_subtract, 2, $1, $3);
			}
		| value '*' value
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   $$ = make_node (nod_multiply2, 2, $1, $3);
			  else
				   $$ = make_node (nod_multiply, 2, $1, $3);
			}
		| value '/' value
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  $$ = make_node (nod_divide2, 2, $1, $3);
			  else
				  $$ = make_node (nod_divide, 2, $1, $3);
			}
		| '(' value ')'
			{ $$ = $2; }
		| '(' column_singleton ')'
			{ $$ = $2; }
		| current_user
		| current_role
		| internal_info
		| DB_KEY
			{ $$ = make_node (nod_dbkey, 1, NULL); }
		| symbol_table_alias_name '.' DB_KEY
			{ $$ = make_node (nod_dbkey, 1, $1); }
				| KW_VALUE
						{
			  $$ = make_node (nod_dom_value, 0, NULL);
						}
		| datetime_value_expression
		| null_value
		;

datetime_value_expression : CURRENT_DATE
			{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
						  												  Arg::Str("DATE"));
			}
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
						  												  Arg::Str("DATE"));
			}
			$$ = make_node (nod_current_date, 0, NULL);
			}
		| CURRENT_TIME sec_precision_opt
			{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
						  												  Arg::Str("TIME"));
			}
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
						  												  Arg::Str("TIME"));
			}
			$$ = make_node (nod_current_time, 1, $2);
			}
		| CURRENT_TIMESTAMP sec_precision_opt
			{ $$ = make_node (nod_current_timestamp, 1, $2); }
		;

sec_precision_opt	: '(' nonneg_short_integer ')'
			{ $$ = MAKE_const_slong ((IPTR) $2); }
		|
			{ $$ = NULL; }
		;

array_element   : column_name '[' value_list ']'
			{ $$ = make_node (nod_array, (int) e_ary_count, $1, make_list ($3)); }
		;

value_list_opt
	:	value_list
	|	// nothing
		{ $$ = NULL; }
	;

value_list	: value
		| value_list ',' value
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

constant	: u_constant
		| '-' u_numeric_constant
			{ $$ = make_node (nod_negate, 1, $2); }
		;

u_numeric_constant : NUMERIC
			{ $$ = MAKE_constant ((dsql_str*) $1, CONSTANT_STRING); }
		| NUMBER
			{ $$ = MAKE_const_slong ((IPTR) $1); }
		| FLOAT_NUMBER
			{ $$ = MAKE_constant ((dsql_str*) $1, CONSTANT_DOUBLE); }
		| NUMBER64BIT
			{ $$ = MAKE_constant ((dsql_str*) $1, CONSTANT_SINT64); }
		| SCALEDINT
			{ $$ = MAKE_constant ((dsql_str*) $1, CONSTANT_SINT64); }
		;

u_constant	: u_numeric_constant
		| sql_string
			{ $$ = MAKE_str_constant ((dsql_str*) $1, lex.att_charset); }
		| DATE STRING
			{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
						  												  Arg::Str("DATE"));
			}
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
						  												  Arg::Str("DATE"));
			}
			$$ = MAKE_constant ((dsql_str*) $2, CONSTANT_DATE);
			}
		| TIME STRING
			{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
						  												  Arg::Str("TIME"));
			}
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
						  												  Arg::Str("TIME"));
			}
			$$ = MAKE_constant ((dsql_str*) $2, CONSTANT_TIME);
			}
		| TIMESTAMP STRING
			{ $$ = MAKE_constant ((dsql_str*) $2, CONSTANT_TIMESTAMP); }
		;

parameter	: '?'
			{ $$ = make_parameter (); }
		;

current_user	: USER
			{ $$ = make_node (nod_user_name, 0, NULL); }
		| CURRENT_USER
			{ $$ = make_node (nod_user_name, 0, NULL); }
		;

current_role	: CURRENT_ROLE
			{ $$ = make_node (nod_current_role, 0, NULL); }
		;

internal_info	: CURRENT_CONNECTION
			{ $$ = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_connection_id)); }
		| CURRENT_TRANSACTION
			{ $$ = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_transaction_id)); }
		| GDSCODE
			{ $$ = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_gdscode)); }
		| SQLCODE
			{ $$ = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_sqlcode)); }
		| SQLSTATE
			{ $$ = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_sqlstate)); }
 		| ROW_COUNT
			{ $$ = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_rows_affected)); }
		;

sql_string
	: STRING			// string in current charset
		{ $$ = $1; }
	| INTRODUCER STRING	// string in specific charset
		{
			dsql_str* str = (dsql_str*) $2;
			str->str_charset = (TEXT*) $1;
			if (str->type == dsql_str::TYPE_SIMPLE)
			{
				StrMark* mark = strMarks.get(str);
				fb_assert(mark);
				if (mark)
					mark->introduced = true;
			}
			$$ = $2;
		}
	;

signed_short_integer	:	nonneg_short_integer
		| '-' neg_short_integer
			{ $$ = (dsql_nod*) - (IPTR) $2; }
		;

nonneg_short_integer	: NUMBER
			{ if ((IPTR) $1 > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  $$ = $1;}
		;

neg_short_integer : NUMBER
			{ if ((IPTR) $1 > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  $$ = $1;}
		;

pos_short_integer : nonneg_short_integer
			{ if ((IPTR) $1 == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  $$ = $1;}
		;

unsigned_short_integer : NUMBER
			{ if ((IPTR) $1 > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  $$ = $1;}
		;

signed_long_integer	:	long_integer
		| '-' long_integer
			{ $$ = (dsql_nod*) - (IPTR) $2; }
		;

long_integer	: NUMBER
			{ $$ = $1;}
		;


/* functions */

function
	: aggregate_function
	| non_aggregate_function
	;

non_aggregate_function
	: numeric_value_function
	| string_value_function
	| system_function_expression
	;

aggregate_function	: COUNT '(' '*' ')'
			{ $$ = make_node (nod_agg_count, 0, NULL); }
		| COUNT '(' all_noise value ')'
			{ $$ = make_node (nod_agg_count, 1, $4); }
		| COUNT '(' DISTINCT value ')'
			{ $$ = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, $4); }
		| SUM '(' all_noise value ')'
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  $$ = make_node (nod_agg_total2, 1, $4);
			  else
				  $$ = make_node (nod_agg_total, 1, $4);
			}
		| SUM '(' DISTINCT value ')'
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  $$ = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, $4);
			  else
				  $$ = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, $4);
			}
		| AVG '(' all_noise value ')'
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  $$ = make_node (nod_agg_average2, 1, $4);
			  else
				  $$ = make_node (nod_agg_average, 1, $4);
			}
		| AVG '(' DISTINCT value ')'
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  $$ = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, $4);
			  else
				  $$ = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, $4);
			}
		| MINIMUM '(' all_noise value ')'
			{ $$ = make_node (nod_agg_min, 1, $4); }
		| MINIMUM '(' DISTINCT value ')'
			{ $$ = make_node (nod_agg_min, 1, $4); }
		| MAXIMUM '(' all_noise value ')'
			{ $$ = make_node (nod_agg_max, 1, $4); }
		| MAXIMUM '(' DISTINCT value ')'
			{ $$ = make_node (nod_agg_max, 1, $4); }
		| LIST '(' all_noise value delimiter_opt ')'
			{ $$ = make_node (nod_agg_list, 2, $4, $5); }
		| LIST '(' DISTINCT value delimiter_opt ')'
			{ $$ = make_flag_node (nod_agg_list, NOD_AGG_DISTINCT, 2, $4, $5); }
		;

delimiter_opt
	: ',' value
		{ $$ = $2; }
	|
		{ $$ = MAKE_str_constant (MAKE_cstring(","), lex.att_charset); }
	;

numeric_value_function
	: extract_expression
	| length_expression
	;

extract_expression	: EXTRACT '(' timestamp_part FROM value ')'
			{ $$ = make_node (nod_extract, (int) e_extract_count, $3, $5); }
		;

length_expression	: bit_length_expression
		| char_length_expression
		| octet_length_expression
		;

bit_length_expression	: BIT_LENGTH '(' value ')'
			{ $$ = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_const_slong(blr_strlen_bit), $3); }
		;

char_length_expression	: CHAR_LENGTH '(' value ')'
			{ $$ = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_const_slong(blr_strlen_char), $3); }
		| CHARACTER_LENGTH '(' value ')'
			{ $$ = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_const_slong(blr_strlen_char), $3); }
		;

octet_length_expression	: OCTET_LENGTH '(' value ')'
			{ $$ = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_const_slong(blr_strlen_octet), $3); }
		;

system_function_expression
	: system_function_std_syntax '(' value_list_opt ')'
		{ $$ = make_node(nod_sys_function, e_sysfunc_count, $1, make_list($3)); }
	| system_function_special_syntax
	;

system_function_std_syntax
	: ABS
	| ACOS
	| ASCII_CHAR
	| ASCII_VAL
	| ASIN
	| ATAN
	| ATAN2
	| BIN_AND
	| BIN_NOT
	| BIN_OR
	| BIN_SHL
	| BIN_SHR
	| BIN_XOR
	| CEIL
	| CHAR_TO_UUID
	| COS
	| COSH
	| COT
	| EXP
	| FLOOR
	| GEN_UUID
	| HASH
	| LEFT
	| LN
	| LOG
	| LOG10
	| LPAD
	| MAXVALUE
	| MINVALUE
	| MOD
	| PI
	| POSITION
	| POWER
	| RAND
	| REPLACE
	| REVERSE
	| RIGHT
	| ROUND
	| RPAD
	| SIGN
	| SIN
	| SINH
	| SQRT
	| TAN
	| TANH
	| TRUNC
	| UUID_TO_CHAR
	;

system_function_special_syntax
	: DATEADD '(' value timestamp_part TO value ')'
		{
			$$ = make_flag_node(nod_sys_function, NOD_SPECIAL_SYNTAX, e_sysfunc_count,
				$1, make_node(nod_list, 3, $3, $4, $6));
		}
	| DATEADD '(' timestamp_part ',' value ',' value ')'
		{
			$$ = make_flag_node(nod_sys_function, NOD_SPECIAL_SYNTAX, e_sysfunc_count,
				$1, make_node(nod_list, 3, $5, $3, $7));
		}
	| DATEDIFF '(' timestamp_part FROM value TO value ')'
		{
			$$ = make_flag_node(nod_sys_function, NOD_SPECIAL_SYNTAX, e_sysfunc_count,
				$1, make_node(nod_list, 3, $3, $5, $7));
		}
	| DATEDIFF '(' timestamp_part ',' value ',' value ')'
		{
			$$ = make_flag_node(nod_sys_function, NOD_SPECIAL_SYNTAX, e_sysfunc_count,
				$1, make_node(nod_list, 3, $3, $5, $7));
		}
	| OVERLAY '(' value PLACING value FROM value FOR value ')'
		{
			$$ = make_flag_node(nod_sys_function, NOD_SPECIAL_SYNTAX, e_sysfunc_count,
				$1, make_node(nod_list, 4, $3, $5, $7, $9));
		}
	| OVERLAY '(' value PLACING value FROM value ')'
		{
			$$ = make_flag_node(nod_sys_function, NOD_SPECIAL_SYNTAX, e_sysfunc_count,
				$1, make_node(nod_list, 3, $3, $5, $7));
		}
	| POSITION '(' value KW_IN value ')'
		{
			$$ = make_flag_node(nod_sys_function, NOD_SPECIAL_SYNTAX, e_sysfunc_count,
				$1, make_node(nod_list, 2, $3, $5));
		}
	;

string_value_function	:  substring_function
		| trim_function
		| KW_UPPER '(' value ')'
			{ $$ = make_node (nod_upcase, 1, $3); }
		| KW_LOWER '(' value ')'
			{ $$ = make_node (nod_lowcase, 1, $3); }
		;

substring_function	: SUBSTRING '(' value FROM value string_length_opt ')'
			/* SQL spec requires numbering to start with 1,
			   hence we decrement the first parameter to make it
			   compatible with the engine's implementation */
			{ $$ = make_node (nod_substr, (int) e_substr_count, $3,
				make_node (nod_subtract, 2, $5,
					MAKE_const_slong (1)), $6); }
		;

string_length_opt	: FOR value
			{ $$ = $2; }
		|
			{ $$ = MAKE_const_slong (LONG_POS_MAX); }
		;

trim_function	: TRIM '(' trim_specification value FROM value ')'
			{ $$ = make_node (nod_trim, (int) e_trim_count, $3, $4, $6); }
		| TRIM '(' value FROM value ')'
			{ $$ = make_node (nod_trim, (int) e_trim_count,
				MAKE_const_slong (blr_trim_both), $3, $5); }
		| TRIM '(' trim_specification FROM value ')'
			{ $$ = make_node (nod_trim, (int) e_trim_count, $3, NULL, $5); }
		| TRIM '(' value ')'
			{ $$ = make_node (nod_trim, (int) e_trim_count,
				MAKE_const_slong (blr_trim_both), NULL, $3); }
		;

trim_specification	: BOTH
			{ $$ = MAKE_const_slong (blr_trim_both); }
		| TRAILING
			{ $$ = MAKE_const_slong (blr_trim_trailing); }
		| LEADING
			{ $$ = MAKE_const_slong (blr_trim_leading); }
		;

udf		: symbol_UDF_call_name '(' value_list ')'
			{ $$ = make_node (nod_udf, 2, $1, $3); }
		| symbol_UDF_call_name '(' ')'
			{ $$ = make_node (nod_udf, 1, $1); }
		;

cast_specification	: CAST '(' value AS data_type_descriptor ')'
			{ $$ = make_node (nod_cast, (int) e_cast_count, $5, $3); }
		;

/* case expressions */

case_expression	: case_abbreviation
		| case_specification
		;

case_abbreviation	: NULLIF '(' value ',' value ')'
			{ $$ = make_node (nod_searched_case, 2,
				make_node (nod_list, 2, make_node (nod_eql, 2, $3, $5),
				make_node (nod_null, 0, NULL)), $3); }
		| IIF '(' search_condition ',' value ',' value ')'
			{ $$ = make_node (nod_searched_case, 2,
				make_node (nod_list, 2, $3, $5), $7); }
		| COALESCE '(' value ',' value_list ')'
			{ $$ = make_node (nod_coalesce, 2, $3, $5); }
		| DECODE '(' value ',' decode_pairs ')'
			{ $$ = make_node(nod_simple_case, 3, $3, make_list($5), make_node(nod_null, 0, NULL)); }
		| DECODE '(' value ',' decode_pairs ',' value ')'
			{ $$ = make_node(nod_simple_case, 3, $3, make_list($5), $7); }
		;

case_specification	: simple_case
		| searched_case
		;

simple_case	: CASE case_operand simple_when_clause END
			{ $$ = make_node (nod_simple_case, 3, $2, make_list($3), make_node (nod_null, 0, NULL)); }
		| CASE case_operand simple_when_clause ELSE case_result END
			{ $$ = make_node (nod_simple_case, 3, $2, make_list($3), $5); }
		;

simple_when_clause	: WHEN when_operand THEN case_result
				{ $$ = make_node (nod_list, 2, $2, $4); }
			| simple_when_clause WHEN when_operand THEN case_result
				{ $$ = make_node (nod_list, 2, $1, make_node (nod_list, 2, $3, $5)); }
			;

searched_case	: CASE searched_when_clause END
			{ $$ = make_node (nod_searched_case, 2, make_list($2), make_node (nod_null, 0, NULL)); }
		| CASE searched_when_clause ELSE case_result END
			{ $$ = make_node (nod_searched_case, 2, make_list($2), $4); }
		;

searched_when_clause	: WHEN search_condition THEN case_result
			{ $$ = make_node (nod_list, 2, $2, $4); }
		| searched_when_clause WHEN search_condition THEN case_result
			{ $$ = make_node (nod_list, 2, $1, make_node (nod_list, 2, $3, $5)); }
		;

when_operand	: value
		;

case_operand	: value
		;

case_result	: value
		;

decode_pairs
	: value ',' value
		{ $$ = make_node(nod_list, 2, $1, $3); }
	| decode_pairs ',' value ',' value
		{ $$ = make_node(nod_list, 2, $1, make_node(nod_list, 2, $3, $5)); }
	;

/* next value expression */

next_value_expression	: NEXT KW_VALUE FOR symbol_generator_name
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  $$ = make_node (nod_gen_id2, 2, $4,
						MAKE_const_slong(1));
			  else
				  $$ = make_node (nod_gen_id, 2, $4,
						MAKE_const_slong(1));
			}
		| GEN_ID '(' symbol_generator_name ',' value ')'
			{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  $$ = make_node (nod_gen_id2, 2, $3, $5);
			  else
				  $$ = make_node (nod_gen_id, 2, $3, $5);
			}
		;


timestamp_part	: YEAR
			{ $$ = MAKE_const_slong (blr_extract_year); }
		| MONTH
			{ $$ = MAKE_const_slong (blr_extract_month); }
		| DAY
			{ $$ = MAKE_const_slong (blr_extract_day); }
		| HOUR
			{ $$ = MAKE_const_slong (blr_extract_hour); }
		| MINUTE
			{ $$ = MAKE_const_slong (blr_extract_minute); }
		| SECOND
			{ $$ = MAKE_const_slong (blr_extract_second); }
		| MILLISECOND
			{ $$ = MAKE_const_slong (blr_extract_millisecond); }
		| WEEK
			{ $$ = MAKE_const_slong (blr_extract_week); }
		| WEEKDAY
			{ $$ = MAKE_const_slong (blr_extract_weekday); }
		| YEARDAY
			{ $$ = MAKE_const_slong (blr_extract_yearday); }
		;

all_noise	: ALL
		|
		;

distinct_noise	: DISTINCT
		|
		;

null_value	: KW_NULL
			{ $$ = make_node (nod_null, 0, NULL); }
		;



/* Performs special mapping of keywords into symbols */

symbol_UDF_call_name	: SYMBOL
	;

symbol_UDF_name	: valid_symbol_name
	;

symbol_blob_subtype_name	: valid_symbol_name
	;

symbol_character_set_name	: valid_symbol_name
	;

symbol_collation_name	: valid_symbol_name
	;

symbol_column_name	: valid_symbol_name
	;

symbol_constraint_name	: valid_symbol_name
	;

symbol_cursor_name	: valid_symbol_name
	;

symbol_domain_name	: valid_symbol_name
	;

symbol_exception_name	: valid_symbol_name
	;

symbol_filter_name	: valid_symbol_name
	;

symbol_gdscode_name	: valid_symbol_name
	;

symbol_generator_name	: valid_symbol_name
	;

symbol_index_name	: valid_symbol_name
	;

symbol_item_alias_name	: valid_symbol_name
	;

symbol_label_name	: valid_symbol_name
	;

symbol_ddl_name	: valid_symbol_name
	;

symbol_procedure_name	: valid_symbol_name
	;

symbol_role_name	: valid_symbol_name
	;

symbol_table_alias_name	: valid_symbol_name
	;

symbol_table_name	: valid_symbol_name
	;

symbol_trigger_name	: valid_symbol_name
	;

symbol_user_name	: valid_symbol_name
	;

symbol_variable_name	: valid_symbol_name
	;

symbol_view_name	: valid_symbol_name
	;

symbol_savepoint_name	: valid_symbol_name
	;

/* symbols */

valid_symbol_name	: SYMBOL
	| non_reserved_word
	;

/* list of non-reserved words */

non_reserved_word :
	ACTION					// added in IB 5.0/
	| CASCADE
	| FREE_IT
	| RESTRICT
	| ROLE
	| KW_TYPE				// added in IB 6.0
	| KW_BREAK				// added in FB 1.0
	| KW_DESCRIPTOR
	| SUBSTRING
	| COALESCE				// added in FB 1.5
	| LAST
	| LEAVE
	| LOCK
	| NULLIF
	| NULLS
	| STATEMENT
	| INSERTING
	| UPDATING
	| DELETING
	| FIRST
	| SKIP
	| BLOCK					// added in FB 2.0
	| BACKUP
	| KW_DIFFERENCE
	| IIF
	| SCALAR_ARRAY
	| WEEKDAY
	| YEARDAY
	| SEQUENCE
	| NEXT
	| RESTART
	| COLLATION
	| RETURNING
	| KW_IGNORE
	| LIMBO
	| UNDO
	| REQUESTS
	| TIMEOUT
	| ABS					// added in FB 2.1
	| ACCENT
	| ACOS
	| ALWAYS
	| ASCII_CHAR
	| ASCII_VAL
	| ASIN
	| ATAN
	| ATAN2
	| BIN_AND
	| BIN_OR
	| BIN_SHL
	| BIN_SHR
	| BIN_XOR
	| CEIL
	| COS
	| COSH
	| COT
	| DATEADD
	| DATEDIFF
	| DECODE
	| EXP
	| FLOOR
	| GEN_UUID
	| GENERATED
	| HASH
	| LIST
	| LN
	| LOG
	| LOG10
	| LPAD
	| MATCHED
	| MATCHING
	| MAXVALUE
	| MILLISECOND
	| MINVALUE
	| MOD
	| OVERLAY
	| PAD
	| PI
	| PLACING
	| POWER
	| PRESERVE
	| RAND
	| REPLACE
	| REVERSE
	| ROUND
	| RPAD
	| SIGN
	| SIN
	| SINH
	| SPACE
	| SQRT
	| TAN
	| TANH
	| TEMPORARY
	| TRUNC
	| WEEK
	| AUTONOMOUS			// added in FB 2.5
	| CHAR_TO_UUID
	| FIRSTNAME
	| MIDDLENAME
	| LASTNAME
	| MAPPING
	| OS_NAME
	| UUID_TO_CHAR
	| GRANTED
	| CALLER				// new execute statement
	| COMMON
	| DATA
	| SOURCE
	| TWO_PHASE
	| BIN_NOT
	| ACTIVE				// old keywords, that were reserved pre-Firebird.2.5
//	| ADD					// words commented it this list remain reserved due to conflicts
	| AFTER
	| ASC
	| AUTO
	| BEFORE
	| COMMITTED
	| COMPUTED
	| CONDITIONAL
	| CONTAINING
	| CSTRING
	| DATABASE
//	| DB_KEY
	| DESC
	| DO
	| DOMAIN
	| ENTRY_POINT
	| EXCEPTION
	| EXIT
	| KW_FILE
//	| GDSCODE
	| GENERATOR
	| GEN_ID
	| IF
	| INACTIVE
//	| INDEX
	| INPUT_TYPE
	| ISOLATION
	| KEY
	| LENGTH
	| LEVEL
//	| KW_LONG
	| MANUAL
	| MODULE_NAME
	| NAMES
	| OPTION
	| OUTPUT_TYPE
	| OVERFLOW
	| PAGE
	| PAGES
	| KW_PAGE_SIZE
	| PASSWORD
//	| PLAN
//	| POST_EVENT
	| PRIVILEGES
	| PROTECTED
	| READ
	| RESERVING
	| RETAIN
//	| RETURNING_VALUES
	| SEGMENT
	| SHADOW
	| KW_SHARED
	| SINGULAR
	| KW_SIZE
	| SNAPSHOT
	| SORT
//	| SQLCODE
	| STABILITY
	| STARTING
	| STATISTICS
	| SUB_TYPE
	| SUSPEND
	| TRANSACTION
	| UNCOMMITTED
//	| VARIABLE
//	| VIEW
	| WAIT
//	| WEEK
//	| WHILE
	| WORK
	| WRITE				// end of old keywords, that were reserved pre-Firebird.2.5
	;

%%


/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		lex.c
 *	DESCRIPTION:	Lexical routine
 *
 */


void LEX_dsql_init(MemoryPool& pool)
{
/**************************************
 *
 *	L E X _ d s q l _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize LEX for processing.  This is called only once
 *	per session.
 *
 **************************************/
	for (const TOK* token = KEYWORD_getTokens(); token->tok_string; ++token)
	{
		dsql_sym* symbol = FB_NEW_RPT(pool, 0) dsql_sym;
		symbol->sym_string = token->tok_string;
		symbol->sym_length = strlen(token->tok_string);
		symbol->sym_type = SYM_keyword;
		symbol->sym_keyword = token->tok_ident;
		symbol->sym_version = token->tok_version;
		dsql_str* str = FB_NEW_RPT(pool, symbol->sym_length) dsql_str;
		str->str_length = symbol->sym_length;
		strncpy(str->str_data, symbol->sym_string, symbol->sym_length);
		//str->str_data[str->str_length] = 0; Is it necessary?
		symbol->sym_object = (void *) str;
		HSHD_insert(symbol);
	}
}


const TEXT* Parser::lex_position()
{
/**************************************
 *
 *	l e x _ p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Return the current position of LEX
 *	in the input string.
 *
 **************************************/

	return lex.ptr;
}


#ifdef NOT_USED_OR_REPLACED
static bool long_int(dsql_nod* string,
					 SLONG *long_value)
{
/*************************************
 *
 *	l o n g _ i n t
 *
 *************************************
 *
 * Functional description
 *	checks for all digits in the
 *	number and return an atol().
 *
 *************************************/

	const char* data = ((dsql_str*) string)->str_data;
	for (const UCHAR* p = (UCHAR*) data; true; p++)
	{
		if (!(classes(*p) & CHR_DIGIT)) {
			return false;
		}
	}

	*long_value = atol(data);

	return true;
}
#endif


static dsql_fld* make_field (dsql_nod* field_name)
{
/**************************************
 *
 *	m a k e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Make a field block of given name.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	if (field_name == NULL)
	{
		dsql_fld* field = FB_NEW(*tdbb->getDefaultPool())
			dsql_fld(*tdbb->getDefaultPool());
		field->fld_name = INTERNAL_FIELD_NAME;
		return field;
	}
	const dsql_str* string = (dsql_str*) field_name->nod_arg[1];
	dsql_fld* field = FB_NEW(*tdbb->getDefaultPool())
		dsql_fld(*tdbb->getDefaultPool());
	field->fld_name = string->str_data;
	field->fld_explicit_collation = false;
	field->fld_not_nullable = false;
	field->fld_full_domain = false;

	return field;
}


static dsql_fil* make_file()
{
/**************************************
 *
 *	m a k e _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Make a file block
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	dsql_fil* temp_file = FB_NEW(*tdbb->getDefaultPool()) dsql_fil;

	return temp_file;
}


dsql_nod* Parser::make_list (dsql_nod* node)
{
/**************************************
 *
 *	m a k e _ l i s t
 *
 **************************************
 *
 * Functional description
 *	Collapse nested list nodes into single list.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	if (node)
	{
		DsqlNodStack stack;
		stack_nodes(node, stack);
		USHORT l = stack.getCount();

		const dsql_nod* old = node;
		node = FB_NEW_RPT(*tdbb->getDefaultPool(), l) dsql_nod;
		node->nod_count = l;
		node->nod_type = nod_list;
		node->nod_line = (USHORT) lex.lines_bk;
		node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
		if (old->getType() == dsql_type_nod)
		{
			node->nod_flags = old->nod_flags;
		}
		dsql_nod** ptr = node->nod_arg + node->nod_count;

		while (stack.hasData())
			*--ptr = stack.pop();
	}

	return node;
}


dsql_nod* Parser::make_parameter()
{
/**************************************
 *
 *	m a k e _ p a r a m e t e r
 *
 **************************************
 *
 * Functional description
 *	Make parameter node
 *	Any change should also be made to function below
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdbb->getDefaultPool(), e_par_count) dsql_nod;
	node->nod_type = nod_parameter;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = e_par_count;
	node->nod_arg[e_par_index] = (dsql_nod*)(IPTR) lex.param_number++;

	return node;
}


dsql_nod* Parser::make_node(NOD_TYPE type, int count, ...)
{
/**************************************
 *
 *	m a k e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Make a node of given type.
 *	Any change should also be made to function below
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdbb->getDefaultPool(), count) dsql_nod;
	node->nod_type = type;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	va_start (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

	va_end(ptr);
	return node;
}


dsql_nod* Parser::makeClassNode(Node* node)
{
	return make_node(nod_class_node, 1, reinterpret_cast<dsql_nod*>(node));
}


dsql_nod* Parser::make_flag_node(NOD_TYPE type, SSHORT flag, int count, ...)
{
/**************************************
 *
 *	m a k e _ f l a g _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Make a node of given type. Set flag field
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdbb->getDefaultPool(), count) dsql_nod;
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	va_start (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

	va_end(ptr);
	return node;
}


#ifdef NOT_USED_OR_REPLACED
static bool short_int(dsql_nod* string,
					  SLONG *long_value,
					  SSHORT range)
{
/*************************************
 *
 *	s h o r t _ i n t
 *
 *************************************
 *
 * Functional description
 *	is the string a valid representation
 *	of a positive short int?
 *
 *************************************/

	if (((dsql_str*) string)->str_length > 5) {
		return false;
	}

	for (UCHAR* p = (UCHAR*)((dsql_str*) string)->str_data; true; p++)
	{
		if (!(classes(*p) & CHR_DIGIT)) {
			return false;
		}
	}

	/* there are 5 or fewer digits, it's value may still be greater
	 * than 32767... */

	SCHAR buf[10];
	buf[0] = ((dsql_str*) string)->str_data[0];
	buf[1] = ((dsql_str*) string)->str_data[1];
	buf[2] = ((dsql_str*) string)->str_data[2];
	buf[3] = ((dsql_str*) string)->str_data[3];
	buf[4] = ((dsql_str*) string)->str_data[4];
	buf[5] = '\0';

	*long_value = atoi (buf);

	bool return_value;

	switch (range)
	{
		case POSITIVE:
			return_value = *long_value > SHRT_POS_MAX;
			break;
		case NEGATIVE:
			return_value = *long_value > SHRT_NEG_MAX;
			break;
		case UNSIGNED:
			return_value = *long_value > SHRT_UNSIGNED_MAX;
			break;
	}
	return !return_value;
}
#endif

static void stack_nodes (dsql_nod*	node,
						 DsqlNodStack& stack)
{
/**************************************
 *
 *	s t a c k _ n o d e s
 *
 **************************************
 *
 * Functional description
 *	Assist in turning a tree of misc nodes into a clean list.
 *
 **************************************/
	if (node->nod_type != nod_list)
	{
		stack.push(node);
		return;
	}

	/* To take care of cases where long lists of nodes are in a chain
	   of list nodes with exactly one entry, this algorithm will look
	   for a pattern of repeated list nodes with two entries, the first
	   being a list node and the second being a non-list node.   Such
	   a list will be reverse linked, and then re-reversed, stacking the
	   non-list nodes in the process.   The purpose of this is to avoid
	   massive recursion of this function. */

	dsql_nod* start_chain = node;
	dsql_nod* end_chain = NULL;
	dsql_nod* curr_node = node;
	dsql_nod* next_node = node->nod_arg[0];
	while ( curr_node->nod_count == 2 &&
			curr_node->nod_arg[0]->nod_type == nod_list &&
			curr_node->nod_arg[1]->nod_type != nod_list &&
			next_node->nod_arg[0]->nod_type == nod_list &&
			next_node->nod_arg[1]->nod_type != nod_list)
	{

		/* pattern was found so reverse the links and go to next node */

		dsql_nod* save_link = next_node->nod_arg[0];
		next_node->nod_arg[0] = curr_node;
		curr_node = next_node;
		next_node = save_link;
		end_chain = curr_node;
	}

	/* see if any chain was found */

	if (end_chain)
	{

		/* first, handle the rest of the nodes */
		/* note that next_node still points to the first non-pattern node */

		stack_nodes (next_node, stack);

		/* stack the non-list nodes and reverse the chain on the way back */

		curr_node = end_chain;
		while (true)
		{
			stack.push(curr_node->nod_arg[1]);
			if ( curr_node == start_chain)
				break;
			dsql_nod* save_link = curr_node->nod_arg[0];
			curr_node->nod_arg[0] = next_node;
			next_node = curr_node;
			curr_node = save_link;
		}
		return;
	}

	dsql_nod** ptr = node->nod_arg;
	for (const dsql_nod* const* const end = ptr + node->nod_count; ptr < end; ptr++)
		stack_nodes (*ptr, stack);
}

static Firebird::MetaName toName(dsql_nod* node)
{
	return Firebird::MetaName(((dsql_str*) node)->str_data);
}

int Parser::yylex()
{
	lex.prev_keyword = yylexAux();
	return lex.prev_keyword;
}

int Parser::yylexAux()
{
/**************************************
 *
 *	y y l e x A u x
 *
 **************************************
 *
 * Functional description: lexer.
 *
 **************************************/
	UCHAR tok_class;
	char string[MAX_TOKEN_LEN];
	SSHORT c;

	/* Find end of white space and skip comments */

	for (;;)
	{
		if (lex.ptr >= lex.end)
			return -1;

		c = *lex.ptr++;

		/* Process comments */

		if (c == '\n') {
			lex.lines++;
			lex.line_start = lex.ptr;
			continue;
		}

		if (c == '-' && lex.ptr < lex.end && *lex.ptr == '-')
		{
			/* single-line */

			lex.ptr++;
			while (lex.ptr < lex.end) {
				if ((c = *lex.ptr++) == '\n') {
					lex.lines++;
					lex.line_start = lex.ptr /* + 1*/; /* CVC: +1 left out. */
					break;
				}
			}
			if (lex.ptr >= lex.end)
				return -1;
			continue;
		}
		else if (c == '/' && lex.ptr < lex.end && *lex.ptr == '*')
		{
			/* multi-line */

			const TEXT& start_block = lex.ptr[-1];
			lex.ptr++;
			while (lex.ptr < lex.end) {
				if ((c = *lex.ptr++) == '*') {
					if (*lex.ptr == '/')
						break;
				}
				if (c == '\n') {
					lex.lines++;
					lex.line_start = lex.ptr /* + 1*/; /* CVC: +1 left out. */

				}
			}
			if (lex.ptr >= lex.end)
			{
				// I need this to report the correct beginning of the block,
				// since it's not a token really.
				lex.last_token = &start_block;
				yyerror("unterminated block comment");
				return -1;
			}
			lex.ptr++;
			continue;
		}

		tok_class = classes(c);

		if (!(tok_class & CHR_WHITE))
			break;
	}

	/* Depending on tok_class of token, parse token */

	lex.last_token = lex.ptr - 1;

	if (tok_class & CHR_INTRODUCER)
	{
		/* The Introducer (_) is skipped, all other idents are copied
		 * to become the name of the character set
		 */
		char* p = string;
		for (; lex.ptr < lex.end && classes(*lex.ptr) & CHR_IDENT; lex.ptr++)
		{
			if (lex.ptr >= lex.end)
				return -1;
			check_copy_incr(p, UPPER7(*lex.ptr), string);
		}

		check_bound(p, string);
		*p = 0;

		/* make a string value to hold the name, the name
		 * is resolved in pass1_constant */

		yylval = (dsql_nod*) (MAKE_string(string, p - string))->str_data;

		return INTRODUCER;
	}

	/* parse a quoted string, being sure to look for double quotes */

	if (tok_class & CHR_QUOTE)
	{
		StrMark mark;
		mark.pos = lex.last_token - lex.start;

		char* buffer = string;
		size_t buffer_len = sizeof (string);
		const char* buffer_end = buffer + buffer_len - 1;
		char* p;
		for (p = buffer; ; ++p)
		{
			if (lex.ptr >= lex.end)
			{
				if (buffer != string)
					gds__free (buffer);
				yyerror("unterminated string");
				return -1;
			}
			// Care about multi-line constants and identifiers
			if (*lex.ptr == '\n') {
				lex.lines++;
				lex.line_start = lex.ptr + 1;
			}
			/* *lex.ptr is quote - if next != quote we're at the end */
			if ((*lex.ptr == c) && ((++lex.ptr == lex.end) || (*lex.ptr != c)))
				break;
			if (p > buffer_end)
			{
				char* const new_buffer = (char*) gds__alloc (2 * buffer_len);
			/* FREE: at outer block */
				if (!new_buffer)		/* NOMEM: */
				{
					if (buffer != string)
						gds__free (buffer);
					return -1;
				}
				memcpy (new_buffer, buffer, buffer_len);
				if (buffer != string)
					gds__free (buffer);
				buffer = new_buffer;
				p = buffer + buffer_len;
				buffer_len = 2 * buffer_len;
				buffer_end = buffer + buffer_len - 1;
			}
			*p = *lex.ptr++;
		}
		if (c == '"')
		{
			stmt_ambiguous = true; /* string delimited by double quotes could be
					**   either a string constant or a SQL delimited
					**   identifier, therefore marks the SQL
					**   statement as ambiguous  */
			if (client_dialect == SQL_DIALECT_V6_TRANSITION)
			{
				if (buffer != string)
					gds__free (buffer);
				yyabandon (-104, isc_invalid_string_constant);
			}
			else if (client_dialect >= SQL_DIALECT_V6)
			{
				if ((p - buffer) >= MAX_TOKEN_LEN)
				{
					if (buffer != string)
						gds__free (buffer);
					yyabandon (-104, isc_token_too_long);
				}
				yylval = (dsql_nod*) MAKE_string(buffer, p - buffer);
				dsql_str* delimited_id_str = (dsql_str*) yylval;
				delimited_id_str->type = dsql_str::TYPE_DELIMITED;
				if (buffer != string)
					gds__free (buffer);
				return SYMBOL;
			}
		}
		yylval = (dsql_nod*) MAKE_string(buffer, p - buffer);
		if (buffer != string)
			gds__free (buffer);

		mark.length = lex.ptr - lex.last_token;
		mark.str = (dsql_str*) yylval;
		strMarks.put(mark.str, mark);

		return STRING;
	}

/*
 * Check for a numeric constant, which starts either with a digit or with
 * a decimal point followed by a digit.
 *
 * This code recognizes the following token types:
 *
 * NUMBER: string of digits which fits into a 32-bit integer
 *
 * NUMBER64BIT: string of digits whose value might fit into an SINT64,
 *   depending on whether or not there is a preceding '-', which is to
 *   say that "9223372036854775808" is accepted here.
 *
 * SCALEDINT: string of digits and a single '.', where the digits
 *   represent a value which might fit into an SINT64, depending on
 *   whether or not there is a preceding '-'.
 *
 * FLOAT: string of digits with an optional '.', and followed by an "e"
 *   or "E" and an optionally-signed exponent.
 *
 * NOTE: we swallow leading or trailing blanks, but we do NOT accept
 *   embedded blanks:
 *
 * Another note: c is the first character which need to be considered,
 *   ptr points to the next character.
 */

	fb_assert(lex.ptr <= lex.end);

	// Hexadecimal string constant.  This is treated the same as a
	// string constant, but is defined as: X'bbbb'
	//
	// Where the X is a literal 'x' or 'X' character, followed
	// by a set of nibble values in single quotes.  The nibble
	// can be 0-9, a-f, or A-F, and is converted from the hex.
	// The number of nibbles should be even.
	//
	// The resulting value is stored in a string descriptor and
	// returned to the parser as a string.  This can be stored
	// in a character or binary item.
	if ((c == 'x' || c == 'X') && lex.ptr < lex.end && *lex.ptr == '\'')
	{
		bool hexerror = false;

		// Remember where we start from, to rescan later.
		// Also we'll need to know the length of the buffer.

		const char* hexstring = ++lex.ptr;
		int charlen = 0;

		// Time to scan the string. Make sure the characters are legal,
		// and find out how long the hex digit string is.

		for (;;)
		{
			if (lex.ptr >= lex.end)	// Unexpected EOS
			{
				hexerror = true;
				break;
			}

			c = *lex.ptr;

			if (c == '\'')			// Trailing quote, done
			{
				++lex.ptr;			// Skip the quote
				break;
			}

			if (!(classes(c) & CHR_HEX))	// Illegal character
			{
				hexerror = true;
				break;
			}

			++charlen;	// Okay, just count 'em
			++lex.ptr;	// and advance...
		}

		hexerror = hexerror || (charlen & 1);	// IS_ODD(charlen)

		// If we made it this far with no error, then convert the string.
		if (!hexerror)
		{
			// Figure out the length of the actual resulting hex string.
			// Allocate a second temporary buffer for it.

			Firebird::string temp;

			// Re-scan over the hex string we got earlier, converting
			// adjacent bytes into nibble values.  Every other nibble,
			// write the saved byte to the temp space.  At the end of
			// this, the temp.space area will contain the binary
			// representation of the hex constant.

			UCHAR byte = 0;
			for (int i = 0; i < charlen; i++)
			{
				c = UPPER7(hexstring[i]);

				// Now convert the character to a nibble

				if (c >= 'A')
					c = (c - 'A') + 10;
				else
					c = (c - '0');

				if (i & 1) // nibble?
				{
					byte = (byte << 4) + (UCHAR) c;
					temp.append(1, (char) byte);
				}
				else
					byte = c;
			}

			dsql_str* string = MAKE_string(temp.c_str(), temp.length());
			string->type = dsql_str::TYPE_HEXA;
			string->str_charset = "BINARY";
			yylval = (dsql_nod*) string;

			return STRING;
		}  // if (!hexerror)...

		// If we got here, there was a parsing error.  Set the
		// position back to where it was before we messed with
		// it.  Then fall through to the next thing we might parse.

		c = *lex.last_token;
		lex.ptr = lex.last_token + 1;
	}

	// Hexadecimal numeric constants - 0xBBBBBB
	//
	// where the '0' and the 'X' (or 'x') are literal, followed
	// by a set of nibbles, using 0-9, a-f, or A-F.  Odd numbers
	// of nibbles assume a leading '0'.  The result is converted
	// to an integer, and the result returned to the caller.  The
	// token is identified as a NUMBER if it's a 32-bit or less
	// value, or a NUMBER64INT if it requires a 64-bit number.
	if (c == '0' && lex.ptr + 1 < lex.end && (*lex.ptr == 'x' || *lex.ptr == 'X') &&
		(classes(lex.ptr[1]) & CHR_HEX))
	{
		bool hexerror = false;

		// Remember where we start from, to rescan later.
		// Also we'll need to know the length of the buffer.

		++lex.ptr;  // Skip the 'X' and point to the first digit
		const char* hexstring = lex.ptr;
		int charlen = 0;

		// Time to scan the string. Make sure the characters are legal,
		// and find out how long the hex digit string is.

		for (;;)
		{
			if (charlen == 0 && lex.ptr >= lex.end)			// Unexpected EOS
			{
				hexerror = true;
				break;
			}

			c = *lex.ptr;

			if (!(classes(c) & CHR_HEX))	// End of digit string
				break;

			++charlen;			// Okay, just count 'em
			++lex.ptr;			// and advance...

			if (charlen > 16)	// Too many digits...
			{
				hexerror = true;
				break;
			}
		}

		// we have a valid hex token. Now give it back, either as
		// an NUMBER or NUMBER64BIT.
		if (!hexerror)
		{
			// if charlen > 8 (something like FFFF FFFF 0, w/o the spaces)
			// then we have to return a NUMBER64BIT. We'll make a string
			// node here, and let make.cpp worry about converting the
			// string to a number and building the node later.
			if (charlen > 8)
			{
				char cbuff[32];
				cbuff[0] = 'X';
				strncpy(&cbuff[1], hexstring, charlen);
				cbuff[charlen + 1] = '\0';

				char* p = &cbuff[1];

				while (*p != '\0')
				{
					if ((*p >= 'a') && (*p <= 'f'))
						*p = UPPER(*p);
					p++;
				}

				yylval = (dsql_nod*) MAKE_string(cbuff, strlen(cbuff));
				return NUMBER64BIT;
			}
			else
			{
				// we have an integer value. we'll return NUMBER.
				// but we have to make a number value to be compatible
				// with existing code.

				// See if the string length is odd.  If so,
				// we'll assume a leading zero.  Then figure out the length
				// of the actual resulting hex string.  Allocate a second
				// temporary buffer for it.

				bool nibble = (charlen & 1);  // IS_ODD(temp.length)

				// Re-scan over the hex string we got earlier, converting
				// adjacent bytes into nibble values.  Every other nibble,
				// write the saved byte to the temp space.  At the end of
				// this, the temp.space area will contain the binary
				// representation of the hex constant.

				UCHAR byte = 0;
				SINT64 value = 0;

				for (int i = 0; i < charlen; i++)
				{
					c = UPPER(hexstring[i]);

					// Now convert the character to a nibble

					if (c >= 'A')
						c = (c - 'A') + 10;
					else
						c = (c - '0');

					if (nibble)
					{
						byte = (byte << 4) + (UCHAR) c;
						nibble = false;
						value = (value << 8) + byte;
					}
					else
					{
						byte = c;
						nibble = true;
					}
				}

				yylval = (dsql_nod*)(long) value;
				return NUMBER;
			} // integer value
		}  // if (!hexerror)...

		// If we got here, there was a parsing error.  Set the
		// position back to where it was before we messed with
		// it.  Then fall through to the next thing we might parse.

		c = *lex.last_token;
		lex.ptr = lex.last_token + 1;
	} // headecimal numeric constants

	if ((tok_class & CHR_DIGIT) ||
		((c == '.') && (lex.ptr < lex.end) && (classes(*lex.ptr) & CHR_DIGIT)))
	{
		/* The following variables are used to recognize kinds of numbers. */

		bool have_error	 = false;	/* syntax error or value too large */
		bool have_digit	 = false;	/* we've seen a digit			  */
		bool have_decimal   = false;	/* we've seen a '.'				*/
		bool have_exp	   = false;	/* digit ... [eE]				  */
		bool have_exp_sign  = false; /* digit ... [eE] {+-]			 */
		bool have_exp_digit = false; /* digit ... [eE] ... digit		*/
		FB_UINT64 number		= 0;
		FB_UINT64 limit_by_10	= MAX_SINT64 / 10;

		for (--lex.ptr; lex.ptr < lex.end; lex.ptr++)
		{
			c = *lex.ptr;
			if (have_exp_digit && (! (classes(c) & CHR_DIGIT)))
				/* First non-digit after exponent and digit terminates
				 the token. */
				break;
			else if (have_exp_sign && (! (classes(c) & CHR_DIGIT)))
			{
				/* only digits can be accepted after "1E-" */
				have_error = true;
				break;
			}
			else if (have_exp)
			{
				/* We've seen e or E, but nothing beyond that. */
				if ( ('-' == c) || ('+' == c) )
					have_exp_sign = true;
				else if ( classes(c) & CHR_DIGIT )
					/* We have a digit: we haven't seen a sign yet,
					but it's too late now. */
					have_exp_digit = have_exp_sign  = true;
				else
				{
					/* end of the token */
					have_error = true;
					break;
				}
			}
			else if ('.' == c)
			{
				if (!have_decimal)
					have_decimal = true;
				else
				{
					have_error = true;
					break;
				}
			}
			else if (classes(c) & CHR_DIGIT)
			{
				/* Before computing the next value, make sure there will be
				   no overflow.  */

				have_digit = true;

				if (number >= limit_by_10)
				{
				/* possibility of an overflow */
					if ((number > limit_by_10) || (c > '8'))
					{
						have_error = true;
						break;
					}
				}
				number = number * 10 + (c - '0');
			}
			else if ( (('E' == c) || ('e' == c)) && have_digit )
				have_exp = true;
			else
				/* Unexpected character: this is the end of the number. */
				break;
		}

		/* We're done scanning the characters: now return the right kind
		   of number token, if any fits the bill. */

		if (!have_error)
		{
			fb_assert(have_digit);

			if (have_exp_digit)
			{
				yylval = (dsql_nod*) MAKE_string(lex.last_token, lex.ptr - lex.last_token);
				lex.last_token_bk = lex.last_token;
				lex.line_start_bk = lex.line_start;
				lex.lines_bk = lex.lines;

				return FLOAT_NUMBER;
			}
			else if (!have_exp)
			{

				/* We should return some kind (scaled-) integer type
				   except perhaps in dialect 1. */

				if (!have_decimal && (number <= MAX_SLONG))
				{
					yylval = (dsql_nod*) (IPTR) number;
					//printf ("parse.y %p %d\n", yylval, number);
					return NUMBER;
				}
				else
				{
					/* We have either a decimal point with no exponent
					   or a string of digits whose value exceeds MAX_SLONG:
					   the returned type depends on the client dialect,
					   so warn of the difference if the client dialect is
					   SQL_DIALECT_V6_TRANSITION.
					*/

					if (SQL_DIALECT_V6_TRANSITION == client_dialect)
					{
						/* Issue a warning about the ambiguity of the numeric
						 * numeric literal.  There are multiple calls because
						 * the message text exceeds the 119-character limit
						 * of our message database.
						 */
						ERRD_post_warning(Arg::Warning(isc_dsql_warning_number_ambiguous) <<
										  Arg::Str(Firebird::string(lex.last_token, lex.ptr - lex.last_token)));
						ERRD_post_warning(Arg::Warning(isc_dsql_warning_number_ambiguous1));
					}

					yylval = (dsql_nod*) MAKE_string(lex.last_token, lex.ptr - lex.last_token);

					lex.last_token_bk = lex.last_token;
					lex.line_start_bk = lex.line_start;
					lex.lines_bk = lex.lines;

					if (client_dialect < SQL_DIALECT_V6_TRANSITION)
						return FLOAT_NUMBER;
					else if (have_decimal)
						return SCALEDINT;
					else
						return NUMBER64BIT;
				}
			} /* else if (!have_exp) */
		} /* if (!have_error) */

		/* we got some kind of error or overflow, so don't recognize this
		 * as a number: just pass it through to the next part of the lexer.
		 */
	}

	/* Restore the status quo ante, before we started our unsuccessful
	   attempt to recognize a number. */
	lex.ptr = lex.last_token;
	c   = *lex.ptr++;
	/* We never touched tok_class, so it doesn't need to be restored. */

	/* end of number-recognition code */


	if (tok_class & CHR_LETTER)
	{
		char* p = string;
		check_copy_incr(p, UPPER (c), string);
		for (; lex.ptr < lex.end && classes(*lex.ptr) & CHR_IDENT; lex.ptr++)
		{
			if (lex.ptr >= lex.end)
				return -1;
			check_copy_incr(p, UPPER (*lex.ptr), string);
		}

		check_bound(p, string);
		*p = 0;
		dsql_sym* sym =
			HSHD_lookup (NULL, string, (SSHORT)(p - string), SYM_keyword, parser_version);
		if (sym && (sym->sym_keyword != COMMENT || lex.prev_keyword == -1))
		{
			yylval = (dsql_nod*) sym->sym_object;
			lex.last_token_bk = lex.last_token;
			lex.line_start_bk = lex.line_start;
			lex.lines_bk = lex.lines;
			return sym->sym_keyword;
		}
		yylval = (dsql_nod*) MAKE_string(string, p - string);
		lex.last_token_bk = lex.last_token;
		lex.line_start_bk = lex.line_start;
		lex.lines_bk = lex.lines;
		return SYMBOL;
	}

	/* Must be punctuation -- test for double character punctuation */

	if (lex.last_token + 1 < lex.end)
	{
		dsql_sym* sym =
			HSHD_lookup (NULL, lex.last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
		if (sym)
		{
			++lex.ptr;
			return sym->sym_keyword;
		}
	}

	/* Single character punctuation are simply passed on */

	return (UCHAR) c;
}


void Parser::yyerror_detailed(const TEXT*
#ifdef DEV_BUILD
										  error_string
#endif
													  , int yychar, YYSTYPE&, YYPOSN&)
{
/**************************************
 *
 *	y y e r r o r _ d e t a i l e d
 *
 **************************************
 *
 * Functional description
 *	Print a syntax error.
 *
 **************************************/
	const TEXT* line_start = lex.line_start;
	SLONG lines = lex.lines;
	if (lex.last_token < lex.line_start)
	{
		line_start = lex.line_start_bk;
		lines--;
	}

	if (yychar < 1)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  /* Unexpected end of command */
				  Arg::Gds(isc_command_end_err2) << Arg::Num(lines) <<
													Arg::Num(lex.last_token - line_start + 1)
#ifdef DEV_BUILD
				  << Arg::Gds(isc_random) << error_string
#endif
				  );
	}
	else
	{
		ERRD_post (Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  /* Token unknown - line %d, column %d */
				  Arg::Gds(isc_dsql_token_unk_err) << Arg::Num(lines) <<
				  									  Arg::Num(lex.last_token - line_start + 1) << /*CVC: +1*/
				  /* Show the token */
				  Arg::Gds(isc_random) << Arg::Str(string(lex.last_token, lex.ptr - lex.last_token)));
	}
}


void Parser::yyerror(const TEXT* error_string)
{
	YYSTYPE errt_value =  0;
	YYPOSN errt_posn = -1;
	yyerror_detailed(error_string, -1, errt_value, errt_posn);
}


static void yyabandon (SLONG		sql_code,
					   ISC_STATUS	error_symbol)
{
/**************************************
 *
 *	y y a b a n d o n
 *
 **************************************
 *
 * Functional description
 *	Abandon the parsing outputting the supplied string
 *
 **************************************/

	ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(sql_code) <<
			  Arg::Gds(error_symbol));
}
