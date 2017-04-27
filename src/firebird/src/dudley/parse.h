/*
 *	PROGRAM:	JRD Data Definition Language
 *	MODULE:		parse.h
 *	DESCRIPTION:	Parser definitions
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

#ifndef DUDLEY_PARSE_H
#define DUDLEY_PARSE_H

/* Keywords */

enum kwwords {
	KW_none = 0,
	KW_ABORT,
	KW_ACTIVE,
	KW_ADD,
	KW_ACL,
	KW_ALL,
	KW_AMPERSAND,
	KW_AND,
	KW_ANY,
	KW_ARRAY,
	KW_ASCENDING,
	KW_ASTERISK,
	KW_AT,
	KW_AUTO,
	KW_AVERAGE,
	KW_BAR,
	KW_BASED,
//	KW_BASE_NAME,
	KW_BEGIN,
	KW_BETWEEN,
	KW_BLOB,
	KW_BLR,
	KW_BY,
//	KW_CACHE,
//	KW_CASCADE,
	KW_CHAR,
//	KW_CHECK_POINT_LEN,
	KW_COLON,
	KW_COMMA,
	KW_CONDITIONAL,
	KW_CONTAINING,
	KW_COMPUTED,
	KW_CONSTRAINT,
	KW_COUNT,
	KW_CROSS,
	KW_CSTRING,
	KW_CURSOR,
	KW_DATABASE,
	KW_DATE,
	KW_DEFAULT,
	KW_DEFINE,
	KW_DELETE,
	KW_DESCENDING,
	KW_DESCRIP,
	KW_DESCRIPTION,
	KW_DISTINCT,
	KW_DOT,
	KW_DOUBLE,
	KW_DROP,
	KW_DUPLICATES,
	KW_EDIT_STRING,
	KW_ELSE,
	KW_END,
	KW_END_DESCRIPTION,
	KW_END_FOR,
	KW_END_MODIFY,
	KW_END_STORE,
	KW_END_TRIGGER,
	KW_EQ,
	KW_EQUALS,
	KW_ERASE,
	KW_EXIT,
	KW_EXTERNAL_FILE,
	KW_FIELD,
	KW_FILE,
	KW_FIRST,
	KW_FIXED,
	KW_FLOAT,
	KW_FOR,
	KW_FROM,
	KW_FILTER,
	KW_FUNCTION,
	KW_FUNCTION_CLASS,
	KW_FUNCTION_ENTRY_POINT,
	KW_FUNCTION_MODULE_NAME,
	KW_GE,
	KW_GEN_ID,
	KW_GENERATOR,
	KW_GRANT,
	KW_GT,
	KW_GROUP,
//	KW_GROUP_COMMIT_WAIT,
	KW_IF,
	KW_IN,
	KW_INACTIVE,
	KW_INDEX,
	KW_INPUT_TYPE,
	KW_INTO,
	KW_INSERT,
	KW_IS,
	KW_LE,
	KW_LENGTH,
	KW_L_BRACE,
	KW_L_BRCKET,
	KW_LEFT_PAREN,
//	KW_LOG_BUF_SIZE,
//	KW_LOG_FILE,
	KW_LONG,
	KW_LT,
	KW_MANUAL,
	KW_MATCHES,
	KW_MAX,
	KW_MESSAGE,
	KW_MIN,
	KW_MINUS,
	KW_MISSING,
	KW_MODIFY,
	KW_MSGADD,
	KW_MSGDROP,
	KW_MSGMODIFY,
	KW_NE,
	KW_NO,
	KW_NOT,
	KW_NULL,
//	KW_NUM_LOG_BUFS,
	KW_OF,
	KW_OFFSET,
	KW_ON,
	KW_OPTION,
	KW_OR,
	KW_OUTPUT_TYPE,
	KW_OVER,
//	KW_OVERFLOW,
	KW_OVERWRITE,
	KW_PAGES,
	KW_PAGE,
	KW_PAGE_NUMBER,
	KW_PAGE_SIZE,
	KW_PARTITIONS,
	KW_PASSWORD,
	KW_PERCENT,
	KW_PLUS,
	KW_POSITION,
	KW_POST,
	KW_POST_ERASE,
	KW_POST_MODIFY,
	KW_POST_STORE,
	KW_PRE,
	KW_PRE_ALLOCATE,
	KW_PRE_ERASE,
	KW_PRE_MODIFY,
	KW_PRE_STORE,
	KW_PRIVILEGES,
	KW_QUAD,
	KW_QUERY_HEADER,
	KW_QUERY_NAME,
	KW_QUIT,
//	KW_RAW,
//	KW_RAW_PARTITIONS,
	KW_REDUCED,
	KW_R_BRACE,
	KW_R_BRCKET,
	KW_REFERENCE,
	KW_RELATION,
	KW_RETURN_ARGUMENT,			/* function argument return_mode */
	KW_RETURN_VALUE,			/* function argument return_mode */
	KW_REVOKE,
	KW_RIGHT_PAREN,
	KW_SCALAR_ARRAY_DESCRIPTOR,
	KW_SCALE,
	KW_SECURITY_CLASS,
	KW_SEGMENT_LENGTH,
	KW_SELECT,
	KW_SEMI,
	KW_SET,
	KW_SET_GENERATOR,
	KW_SHADOW,
	KW_SHORT,
//	KW_SIZE,
	KW_SLASH,
	KW_SORTED,
	KW_STARTS,
	KW_STARTING,
	KW_STATISTICS,
	KW_STORE,
	KW_SUB_TYPE,
	KW_SYSTEM_FLAG,
	KW_TEXT,
	KW_THEN,
	KW_TO,
	KW_TOTAL,
	KW_TRIGGER,
	KW_TYPES,
	KW_UNION,
	KW_UNIQUE,
	KW_UPDATE,
	KW_UPPERCASE,
	KW_USER,
	KW_USER_NAME,
	KW_USING,
	KW_VALID,
	KW_VALID_IF,
	KW_VALUE,
	KW_VARYING,
	KW_VIEW,
	KW_WITH
};

/* Token block, used to hold a lexical token. */

enum tok_t {
	tok_ident,
	tok_number,
	tok_quoted,
	tok_punct,
	tok_eol
};

typedef struct tok {
	enum tok_t tok_type;		/* type of token */
	sym* tok_symbol;			/* hash block if recognized */
	enum kwwords tok_keyword;	/* keyword number, if recognized */
	SLONG tok_position;			/* byte number in input stream */
	USHORT tok_length;
	TEXT tok_string[MAXSYMLEN];
} *TOK;

#endif // DUDLEY_PARSE_H

