/*
 *	PROGRAM:	JRD Data Definition Language
 *	MODULE:		hsh.cpp
 *	DESCRIPTION:	Hash table and symbol manager
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
#include "../dudley/ddl.h"
#include "../dudley/parse.h"

#include "../dudley/ddl_proto.h"
#include "../dudley/hsh_proto.h"


const int HASH_SIZE = 101;

static USHORT hash(const SCHAR*, USHORT);
static bool scompare(const SCHAR*, USHORT, const SCHAR*, const USHORT);

static SYM hash_table[HASH_SIZE];
static SYM key_symbols;

struct word {
	enum kwwords id;
	const char* keyword;
} keywords[] = {
	{KW_OR, "||"},
		{KW_AND, "&&"},
		{KW_GE, ">="},
		{KW_LE, "<="},
		{KW_NE, "<>"},
		{KW_NE, "!="},
		{KW_NE, "~="},
		{KW_EQ, "=="},
		{KW_GT, ">"},
		{KW_LT, "<"},
		{KW_PERCENT, "%"},
		{KW_EQUALS, "="},
		{KW_COMMA, ","},
		{KW_DOT, "."},
		{KW_L_BRCKET, "["},
		{KW_LEFT_PAREN, "("},
		{KW_MINUS, "-"},
		{KW_R_BRCKET, "]"},
		{KW_RIGHT_PAREN, ")"},
		{KW_PLUS, "+"},
		{KW_SEMI, ";"},
		{KW_SLASH, "/"},
		{KW_ASTERISK, "*"},
		{KW_BAR, "|"},
		{KW_COLON, ":"},
		{KW_ABORT, "ABORT"},
		{KW_ACL, "ACL"},
		{KW_ACTIVE, "ACTIVE"},
		{KW_ADD, "ADD"},
		{KW_ALL, "ALL"},
		{KW_ARRAY, "ARRAY"},
		{KW_AT, "AT"},
		{KW_AUTO, "AUTO"},
		{KW_BASED, "BASED"},
//		{KW_BASE_NAME, "BASE_NAME"},
		{KW_BEGIN, "BEGIN"},
		{KW_BLOB, "BLOB"},
		{KW_BLR, "BLR"},
//		{KW_CACHE, "CACHE"},
//		{KW_CASCADE, "CASCADE"},
		{KW_CHAR, "CHAR"},
//		{KW_CHECK_POINT_LEN, "CHECK_POINT_LENGTH"},
		{KW_COMPUTED, "COMPUTED_BY"},
		{KW_COMPUTED, "COMPUTED"},
		{KW_CONDITIONAL, "CONDITIONAL"},
		{KW_CONSTRAINT, "CONSTRAINT"},
		{KW_CSTRING, "CSTRING"},
		{KW_DATABASE, "DATABASE"},
		{KW_DATE, "DATE"},
		{KW_DEFINE, "DEFINE"},
		{KW_DEFAULT, "DEFAULT"},
		{KW_DELETE, "DELETE"},
		{KW_DESCRIP, "DESCRIPTION"},
		{KW_DESCRIPTION, "{"},
		{KW_DOUBLE, "DOUBLE"},
		{KW_DROP, "DROP"},
		{KW_DROP, "DELETE"},
		{KW_DUPLICATES, "DUPLICATE"},
		{KW_DUPLICATES, "DUPLICATES"},
		{KW_EDIT_STRING, "EDIT_STRING"},
		{KW_ELSE, "ELSE"},
		{KW_END, "END"},
		{KW_END_DESCRIPTION, "}"},
		{KW_END_FOR, "END_FOR"},
		{KW_END_MODIFY, "END_MODIFY"},
		{KW_END_STORE, "END_STORE"},
		{KW_END_TRIGGER, "END_TRIGGER"},
		{KW_ERASE, "ERASE"},
		{KW_EXIT, "EXIT"},
		{KW_EXTERNAL_FILE, "EXTERNAL_FILE"},
		{KW_FIELD, "FIELD"},
		{KW_FILE, "FILE"},
		{KW_FILTER, "FILTER"},
		{KW_FIXED, "FIXED"},
		{KW_FLOAT, "FLOAT"},
		{KW_FOR, "FOR"},
		{KW_FUNCTION, "FUNCTION"},
		{KW_FUNCTION_ENTRY_POINT, "ENTRY_POINT"},
		{KW_FUNCTION_MODULE_NAME, "MODULE_NAME"},
		{KW_GENERATOR, "GENERATOR"},
		{KW_GRANT, "GRANT"},
		{KW_GROUP, "GROUP"},
//		{KW_GROUP_COMMIT_WAIT, "GROUP_COMMIT_WAIT_TIME"},
		{KW_IF, "IF"},
		{KW_INACTIVE, "INACTIVE"},
		{KW_INDEX, "INDEX"},
		{KW_INPUT_TYPE, "INPUT_TYPE"},
		{KW_INSERT, "INSERT"},
		{KW_IS, "IS"},
		{KW_LENGTH, "LENGTH"},
//		{KW_LOG_BUF_SIZE, "LOG_BUFFER_SIZE"},
//		{KW_LOG_FILE, "LOGFILE"},
		{KW_LONG, "LONG"},
		{KW_MANUAL, "MANUAL"},
		{KW_MESSAGE, "MESSAGE"},
		{KW_MISSING, "MISSING_VALUE"},
		{KW_MISSING, "MISSING"},
		{KW_MODIFY, "MODIFY"},
		{KW_MSGADD, "MSGADD"},
		{KW_MSGDROP, "MSGDROP"},
		{KW_MSGMODIFY, "MSGMODIFY"},
		{KW_NULL, "NULL"},
//		{KW_NUM_LOG_BUFS, "NUM_LOG_BUFFERS"},
		{KW_OFFSET, "OFFSET"},
		{KW_ON, "ON"},
		{KW_OPTION, "OPTION"},
		{KW_OUTPUT_TYPE, "OUTPUT_TYPE"},
//		{KW_OVERFLOW, "OVERFLOW"},
		{KW_OVERWRITE, "OVERWRITE"},
		{KW_PAGE, "PAGE"},
		{KW_PAGES, "PAGES"},
		{KW_PAGE_NUMBER, "PAGE_NUMBER"},
		{KW_PAGE_SIZE, "PAGE_SIZE"},
		{KW_PARTITIONS, "PARTITIONS"},
		{KW_PASSWORD, "PASSWORD"},
		{KW_POSITION, "POSITION"},
		{KW_POST, "POST"},
		{KW_POST_ERASE, "POST_ERASE"},
		{KW_POST_MODIFY, "POST_MODIFY"},
		{KW_POST_STORE, "POST_STORE"},
		{KW_PRE, "PRE"},
		{KW_PRE_ALLOCATE, "PRE_ALLOCATE"},
		{KW_PRE_ERASE, "PRE_ERASE"},
		{KW_PRE_MODIFY, "PRE_MODIFY"},
		{KW_PRE_STORE, "PRE_STORE"},
		{KW_PRIVILEGES, "PRIVILEGES"},
		{KW_QUAD, "QUAD"},
		{KW_QUERY_NAME, "QUERY_NAME"},
		{KW_QUERY_HEADER, "QUERY_HEADER"},
//		{KW_RAW, "RAW"},
//		{KW_RAW_PARTITIONS, "RAW_PARTITIONS"},
		{KW_REFERENCE, "REFERENCE"},
    	{KW_RELATION, "RELATION"},
        {KW_RETURN_ARGUMENT, "RETURN_ARGUMENT"},	/* function argument return_mode */
		{KW_RETURN_VALUE, "RETURN_VALUE"},	/* function argument return_mode */
		{KW_REVOKE, "REVOKE"},
		{KW_SCALAR_ARRAY_DESCRIPTOR, "SCALAR_ARRAY_DESCRIPTOR"},
		{KW_SCALE, "SCALE"},
		{KW_SELECT, "SELECT"},
		{KW_SECURITY_CLASS, "SECURITY_CLASS"},
		{KW_SEGMENT_LENGTH, "SEGMENT_LENGTH"},
		{KW_SET, "SET"},
		{KW_SET_GENERATOR, "SET_GENERATOR"},
		{KW_SHADOW, "SHADOW"},
		{KW_SHORT, "SHORT"},
//		{KW_SIZE, "SIZE"},
		{KW_SORTED, "SORTED"},
		{KW_STATISTICS, "STATISTICS"},
		{KW_STORE, "STORE"},
		{KW_SUB_TYPE, "SUB_TYPE"},
		{KW_SYSTEM_FLAG, "SYSTEM_FLAG"},
		{KW_TEXT, "TEXT"},
		{KW_THEN, "THEN"},
		{KW_TO, "TO"},
		{KW_TRIGGER, "TRIGGER"},
		{KW_TYPES, "TYPES"},
		{KW_UPDATE, "UPDATE"},
		{KW_USER, "USER"},
		{KW_USER_NAME, "RDB$USER_NAME"},
		{KW_USING, "USING"},
		{KW_VALID_IF, "VALID_IF"},
		{KW_VALID_IF, "VALID"},
		{KW_VALUE, "VALUE"},
		{KW_VARYING, "VARYING"},
		{KW_VIEW, "VIEW"},
		{KW_AND, "AND"},
		{KW_ANY, "ANY"},
		{KW_ASCENDING, "ASC"},
		{KW_ASCENDING, "ASCENDING"},
		{KW_ASTERISK, "ASTERISK"},
		{KW_AVERAGE, "AVERAGE"},
		{KW_BETWEEN, "BETWEEN"},
		{KW_BY, "BY"},
		{KW_CONTAINING, "CONTAINING"},
		{KW_COUNT, "COUNT"},
		{KW_CROSS, "CROSS"},
		{KW_DESCENDING, "DESC"},
		{KW_DESCENDING, "DESCENDING"},
		{KW_ELSE, "ELSE"},
		{KW_EQ, "EQ"},
		{KW_FIRST, "FIRST"},
		{KW_FROM, "FROM"},
		{KW_GEN_ID, "GEN_ID"},
		{KW_GE, "GE"},
		{KW_GT, "GT"},
		{KW_IN, "IN"},
		{KW_LE, "LE"},
		{KW_LT, "LT"},
		{KW_MATCHES, "MATCHES"},
		{KW_MATCHES, "MATCHING"},
		{KW_MIN, "MIN"},
		{KW_MINUS, "MINUS"},
		{KW_MISSING, "MISSING"},
		{KW_NE, "NE"},
		{KW_NOT, "NOT"},
		{KW_OF, "OF"},
		{KW_OR, "OR"},
		{KW_OVER, "OVER"},
		{KW_QUIT, "QUIT"},
		{KW_REDUCED, "REDUCED"},
		{KW_SLASH, "SLASH"},
		{KW_SORTED, "SORTED"},
		{KW_STARTS, "STARTS"},
		{KW_STARTS, "STARTING"},
		{KW_STARTS, "STARTS_WITH"},
		{KW_STARTS, "STARTING_WITH"},
		{KW_MAX, "MAX"},
		{KW_TO, "TO"},
		{KW_TOTAL, "TOTAL"},
    	{KW_UNIQUE, "UNIQUE"},
		{KW_UPPERCASE, "UPPERCASE"},
		{KW_WITH, "WITH"}
};


void HSH_init()
{
/**************************************
 *
 *	H S H _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the hash table.  This mostly involves
 *	inserting all known keywords.
 *
 **************************************/
	SYM symbol;
	int i;
	SSHORT length;

	for (i = 0; i < FB_NELEM(keywords); i++) {
		const char* string = keywords[i].keyword;
		for (length = 0; string[length] != '\0'; length++);
		symbol = (SYM) DDL_alloc(SYM_LEN);
		symbol->sym_type = SYM_keyword;
		symbol->sym_length = length;
		symbol->sym_string = keywords[i].keyword;
		symbol->sym_keyword = (int) keywords[i].id;
		HSH_insert(symbol);
		symbol->sym_object = (DUDLEY_CTX) key_symbols;
		key_symbols = symbol;
	}
}


void HSH_insert( SYM symbol)
{
/**************************************
 *
 *	H S H _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert a symbol into the hash table.
 *
 **************************************/
	const USHORT h = hash(symbol->sym_string, symbol->sym_length);

	for (SYM old = hash_table[h]; old; old = old->sym_collision)
	{
		if (scompare(symbol->sym_string, symbol->sym_length,
					 old->sym_string, old->sym_length))
		{
			symbol->sym_homonym = old->sym_homonym;
			old->sym_homonym = symbol;
			return;
		}
	}

	symbol->sym_collision = hash_table[h];
	hash_table[h] = symbol;
}


SYM HSH_lookup(const SCHAR* string, USHORT length)
{
/**************************************
 *
 *	H S H _ l o o k u p
 *
 **************************************
 *
 * Functional description
 *	Perform a string lookup against hash table.
 *
 **************************************/
	for (SYM symbol = hash_table[hash(string, length)]; symbol; symbol = symbol->sym_collision)
	{
		if (scompare(string, length, symbol->sym_string, symbol->sym_length))
			return symbol;
	}

	return NULL;
}


void HSH_remove( SYM symbol)
{
/**************************************
 *
 *	H S H _ r e m o v e
 *
 **************************************
 *
 * Functional description
 *	Remove a symbol from the hash table.
 *
 **************************************/
	USHORT h;
	SYM *next, *ptr, homonym;

	h = hash(symbol->sym_string, symbol->sym_length);

	for (next = &hash_table[h]; *next; next = &(*next)->sym_collision)
		if (symbol == *next)
			if (homonym = symbol->sym_homonym) {
				homonym->sym_collision = symbol->sym_collision;
				*next = homonym;
				return;
			}
			else {
				*next = symbol->sym_collision;
				return;
			}
		else
			for (ptr = &(*next)->sym_homonym; *ptr; ptr = &(*ptr)->sym_homonym)
				if (symbol == *ptr) {
					*ptr = symbol->sym_homonym;
					return;
				}

	DDL_err(280);	/* msg 280: HSH_remove failed  */
}


SYM HSH_typed_lookup(const TEXT* string,
					 USHORT length, enum sym_t type)
{
/**************************************
 *
 *	H S H _ t y p e d _ l o o k u p
 *
 **************************************
 *
 * Functional description
 *	Perform a string lookup against hash table
 *	considering the object type.  If length is
 *	0, assume that the string is terminated by
 *	a null or space and compute the length.
 *
 **************************************/
	if (!length) {
		const TEXT* p;
		for (p = string; *p && *p != ' '; p++)
			if ((p - string) >= 32)
				break;
		length = p - string;
	}

	SYM symbol = HSH_lookup(string, length);

	for (; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == type)
			break;

	return symbol;
}


static USHORT hash(const SCHAR* string, USHORT length)
{
/**************************************
 *
 *	h a s h
 *
 **************************************
 *
 * Functional description
 *	Returns the hash function of a string.
 *
 **************************************/
	USHORT value = 0;

	while (length--) {
		const SCHAR c = *string++;
		value = (value << 1) + UPPER(c);
	}

	return value % HASH_SIZE;
}


static bool scompare(const SCHAR* string1,
					 USHORT length1,
					 const SCHAR* string2,
					 const USHORT length2)
{
/**************************************
 *
 *	s c o m p a r e
 *
 **************************************
 *
 * Functional description
 *	Compare two strings
 *
 **************************************/
	SCHAR c1, c2;

	if (length1 != length2)
		return false;

	while (length1--)
		if ((c1 = *string1++) != (c2 = *string2++) && UPPER(c1) != UPPER(c2))
			return false;

	return true;
}

