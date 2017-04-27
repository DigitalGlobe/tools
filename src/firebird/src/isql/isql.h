/*
 *	PROGRAM:	Interactive SQL utility
 *	MODULE:		isql.h
 *	DESCRIPTION:	Component wide include file
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
 * Revision 1.2  2000/11/18 16:49:24  fsg
 * Increased PRINT_BUFFER_LENGTH to 2048 to show larger plans
 * Fixed Bug #122563 in extract.e get_procedure_args
 * Apparently this has to be done in show.e also,
 * but that is for another day :-)
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef ISQL_ISQL_H
#define ISQL_ISQL_H

#include "../jrd/flags.h"
#include <stdlib.h>

// Define lengths used in isql.e

/* Increased this to allow display of somewhat lengthy plans
   hope this will last a year or so :-)
   FSG 17.Nov.2000
*/
const int PRINT_BUFFER_LENGTH	= 2048;
const int MAXTERM_SIZE			= 32;	// SQL termination character
const int USER_LENGTH 			= 128;
const int PASSWORD_LENGTH		= 128;
const int ROLE_LENGTH			= 128;

/* these constants are purely idiotic; there's no point in having
   a predefined constant with no meaning, but that's Ed Simon the
   master programmer for you! */

const int BUFFER_LENGTH128	= 128;
const int BUFFER_LENGTH155	= 155;
const int BUFFER_LENGTH256	= 256;
const int BUFFER_LENGTH360	= 360;
const int BUFFER_LENGTH400	= 400;
const int BUFFER_LENGTH512	= 512;
const int BUFFER_LENGTH80	= 80;
const int BUFFER_LENGTH60	= 60;
const int BUFFER_LENGTH120	= 120;
const int BUFFER_LENGTH180	= 180;

// Define the possible states of processing commands

enum processing_state {
	FOUND_EOF   =   EOF,
	CONT		=	0,
	EXIT		=	1,
	BACKOUT		=	2,
	ps_ERR		=	3,
	END			=	4,
	SKIP		=	5,
	FAIL		=	6,
	EXTRACT		=	7,
	EXTRACTALL	=	8,
	FETCH		=	9,
	OBJECT_NOT_FOUND = 10,
	ERR_BUFFER_OVERFLOW = 11
};

// Which blob subtypes to print

const int ALL_BLOBS	= -2;
const int NO_BLOBS	= -1;

// Flag to decode all vs sql only objects
enum LegacyTables
{
	SQL_objects,
	ALL_objects
};

const size_t WORDLENGTH			= 32;
// The worst case of a quoted identifier is 31 * 2 => 62 + 2 DQUOTES + TERM => 65.
const size_t QUOTEDLENGTH       = 65;
static const char* const DEFTERM	= ";";
static const char* const DEFCHARSET	= "NONE";
const int NULL_DISP_LEN				= 6;

// Error codes

const int MSG_LENGTH	= 1024;
const int ISQL_MSG_FAC	= 17;

const int GEN_ERR 					= 0;	// General non-zero SQLCODE error
const int SWITCH 					= 2;	// Bad command line arg
const int NO_DB 					= 3;	// No database specified
const int FILE_OPEN_ERR				= 4;	// Can't open specified file
const int COMMIT_PROMPT				= 5;	// Commit or rollback question
const int COMMIT_MSG				= 6;	// Committing ...
const int ROLLBACK_MSG				= 7;	// Rolling back
const int CMD_ERR					= 8;	// Unknown frontend command
const int ADD_PROMPT				= 9;	// Prompt for add function
const int VERSION					= 10;		// Version string for -z
const int NUMBER_PAGES				= 12;		// Number of DB pages allocated = %d \n
const int SWEEP_INTERV				= 13;		// Sweep interval = %d \n
//const int NUM_WAL_BUFF				= 14;		// Number of wal buffers = %d \n
//const int WAL_BUFF_SIZE				= 15;		// Wal buffer size = %d \n
const int CKPT_LENGTH				= 16;		// Check point length = %d \n
const int CKPT_INTERV				= 17;		// Check point interval = %d \n
//const int WAL_GRPC_WAIT				= 18;		// Wal group commit wait = %d \n
const int BASE_LEVEL				= 19;		// Base level = %d \n
const int LIMBO						= 20;		// Transaction in limbo = %d \n
// Help list
const int HLP_FRONTEND				= 21;		// Frontend commands:\n
const int HLP_BLOBVIEW				= 22;		// BLOBVIEW [<blobid as high:low>] -- edit a blob\n
const int HLP_BLOBDMP				= 23;		// BLOBDUMP <blobid as high:low> <file> -- dump blob to a file\n
const int HLP_EDIT					= 24;		// EDIT [<filename>] -- edit and read a SQL file\n\tWithout file name, edits current command buffer\n
const int HLP_INPUT					= 25;		// INput <filename> -- enter a named SQL file\n
const int HLP_OUTPUT				= 26;		// OUTput [<filename>] -- write output to named file\n
const int HLP_SHELL					= 27;		// SHELL <shell command> -- execute command shell\n
const int HLP_HELP					= 28;		// HELP -- Displays this menu\n
const int HLP_SETCOM				= 29;		// "Set commands: "
const int HLP_SET					= 30;		// \tSET -- Display current options \n
const int HLP_SETAUTO				= 31;		// \tSET AUTOcommit  -- toggle autocommit of DDL statments\n
const int HLP_SETBLOB				= 32;		// \tSET BLOBdisplay [ALL|N]-- Display blobs of type N\n\t SET BLOB turns off blob display\n
const int HLP_SETCOUNT				= 33;		// \tSET COUNT  -- toggle count of selected rows on/off \n
const int HLP_SETROWCOUNT			= 165;		// \tSET ROWCOUNT [N] -- limits the number of rows returned, zero is no limit \n
const int HLP_SETECHO				= 34;		// \tSET ECHO  -- toggle command echo on/off \n
const int HLP_SETSTAT				= 35;		// \tSET STATs -- toggles performance statistics display\n
const int HLP_SETTERM				= 36;		// \tSET TERM <string> -- changes termination character\n
const int HLP_SHOW					= 37;		// SHOW <object type> [<object name>] -- displays information on metadata\n
const int HLP_OBJTYPE				= 38;		// "    <object> = CHECK, COLLATION, DATABASE, DOMAIN, EXCEPTION, FILTER, FUNCTION,"
const int HLP_EXIT					= 39;		// EXIT -- Exit program and commit changes\n
const int HLP_QUIT					= 40;		// QUIT -- Exit program and rollback changes\n\n
const int HLP_ALL					= 41;		// All commands may be abbreviated to letters in CAPs\n
const int HLP_SETSCHEMA				= 42;		// \tSET SCHema/DB <db name> -- changes current database\n
const int YES_ANS					= 43;		// Yes
const int REPORT1					= 44;		// Current memory = !c\nDelta memory = !d\nMax memory = !x\nElapsed time= !e sec\n
#if (defined WIN_NT)
const int REPORT2					= 93;		// Buffers = !b\nReads = !r\nWrites = !w\nFetches = !f\n
#else
const int REPORT2					= 45;		// Cpu = !u sec\nBuffers = !b\nReads = !r\nWrites = !w\nFetches = !f\n
#endif
const int BLOB_SUBTYPE				= 46;		// Blob display set to subtype %d. This blob: subtype = %d\n
const int BLOB_PROMPT				= 47;		// Blob: %s, type 'edit' or filename to load>
const int DATE_PROMPT				= 48;		// Enter %s as M/D/Y>
const int NAME_PROMPT				= 49;		// Enter %s>
const int DATE_ERR					= 50;		// Bad date %s\n
const int CON_PROMPT				= 51;		// "CON> "
const int HLP_SETLIST				= 52;		// \tSET LIST -- toggles column or table display\n
const int NOT_FOUND_MSG				= 53;		// %s not found\n
const int COPY_ERR 					= 54;		// Errors occurred (possibly duplicate domains) in creating %s in %s\n"
const int SERVER_TOO_OLD			= 55;		// Server version too old to support the isql command
const int REC_COUNT 				= 56;		// Total returned: %ld
const int UNLICENSED				= 57;		// Unlicensed for database "%s"
const int HLP_SETWIDTH				= 58;		// \tSET WIDTH <column name> [<width>] --sets/unsets print width for column name
const int HLP_SETPLAN				= 59;		// Toggle display of query access plan
const int HLP_SETTIME				= 60;		// Toggle display of timestamp with DATE values
const int HLP_EDIT2					= 61;		// edits current command buffer\n
const int HLP_OUTPUT2				= 62;		// \tWithout file name, returns output to stdout\n
const int HLP_SETNAMES				= 63;		// Set current character set
const int HLP_OBJTYPE2				= 64;		// More objects
const int HLP_SETBLOB2				= 65;		// \t SET BLOB turns off blob display\n
const int HLP_SET_ROOT				= 66;		// (Use HELP SET for set options)
const int NO_TABLES					= 67;		// There are no tables in this database
const int NO_TABLE					= 68;		// There is no table %s in this database
const int NO_VIEWS					= 69;		// There are no views in this database
const int NO_VIEW					= 70;		// There is no view %s in this database
const int NO_INDICES_ON_REL			= 71;		// There are no indices on table %s in this database
const int NO_REL_OR_INDEX			= 72;		// There is no table or index %s in this database
const int NO_INDICES				= 73;		// There are no indices in this database
const int NO_DOMAIN					= 74;		// There is no domain %s in this database
const int NO_DOMAINS				= 75;		// There are no domains in this database
const int NO_EXCEPTION				= 76;		// There is no exception %s in this database
const int NO_EXCEPTIONS				= 77;		// There are no exceptions in this database
const int NO_FILTER					= 78;		// There is no filter %s in this database
const int NO_FILTERS				= 79;		// There are no filters in this database
const int NO_FUNCTION				= 80;		// There is no user-defined function %s in this database
const int NO_FUNCTIONS				= 81;		// There are no user-defined functions in this database
const int NO_GEN					= 82;		// There is no generator %s in this database
const int NO_GENS					= 83;		// There are no generators in this database
const int NO_GRANT_ON_REL			= 84;		// There is no privilege granted on table %s in this database
const int NO_GRANT_ON_PROC			= 85;		// There is no privilege granted on stored procedure %s in this database
const int NO_REL_OR_PROC			= 86;		// There is no table or stored procedure %s in this database
const int NO_PROC					= 87;		// There is no stored procedure %s in this database
const int NO_PROCS					= 88;		// There are no stored procedures in this database
const int NO_TRIGGERS_ON_REL		= 89;		// There are no triggers on table %s in this database
const int NO_REL_OR_TRIGGER			= 90;		// There is no table or trigger %s in this database
const int NO_TRIGGERS				= 91;		// There are no triggers in this database
const int NO_TRIGGER				= 121;      // There is no trigger %s in this database
const int NO_CHECKS_ON_REL			= 92;		// There are no check constraints on table %s in this database
const int NO_COMMENTS				= 115;		// There are no comments for objects in this database.
const int BUFFER_OVERFLOW			= 94;		// An isql command exceeded maximum buffer size
#ifdef SCROLLABLE_CURSORS
const int HLP_SETFETCH				= 95;		// \tSET AUTOfetch  -- toggle autofetch of records\n
#endif

const int NO_ROLES					= 95;		// There are no roles in this database
const int NO_REL_OR_PROC_OR_ROLE	= 96;		// There is no table, stored procedure, or
											// role %s in this database
const int NO_GRANT_ON_ROL			= 97;		// There is no membership privilege granted
											// on %s in this database
const int UNEXPECTED_EOF			= 98;		// Expected end of statement, encountered EOF
const int TIME_ERR					= 101;		// Bad TIME: %s\n
const int HLP_OBJTYPE3				= 102;		// Some more objects
const int NO_ROLE					= 103;		// There is no role %s in this database
const int INCOMPLETE_STR			= 105;		// Incomplete string in %s
const int HLP_SETSQLDIALECT			= 106;		// \tSET SQL DIALECT <n>    -- set sql dialect to <n>
const int NO_GRANT_ON_ANY			= 107;		// There is no privilege granted in this database.
const int HLP_SETPLANONLY			= 108;		// toggle display of query plan without executing
const int HLP_SETHEADING			= 109;		// toggle display of query column titles
const int HLP_SETBAIL				= 110;		// toggle bailing out on errors in non-interactive mode
const int TIME_PROMPT				= 112;		// Enter %s as H:M:S>
const int TIMESTAMP_PROMPT			= 113;		// Enter %s as Y/MON/D H:MIN:S[.MSEC]>
const int TIMESTAMP_ERR				= 114;		// Bad TIMESTAMP: %s\n
const int ONLY_FIRST_BLOBS			= 116;		// Printing only the first %d blobs.
const int MSG_TABLES				= 117;		// Tables:
const int MSG_FUNCTIONS				= 118;		// Functions:
const int EXACTLINE					= 119;		// At line %d in file %s
const int AFTERLINE					= 120;		// After line %d in file %s
const int USAGE						= 1;		// usage: syntax
const int USAGE_NOARG				= 142;		// usage: missing argument for "%s"
const int USAGE_NOTINT				= 143;		// usage: argument "%s" for switch "%s" is not an integer
const int USAGE_RANGE				= 144;		// usage: value "%s" for switch "%s" is out of range
const int USAGE_DUPSW				= 145;		// usage: switch "%s" or its equivalent used more than once
const int USAGE_DUPDB				= 146;		// usage: more than one database name: "%s", "%s"
const int NO_DEPENDENCIES			= 147;		// No dependencies for %s were found
const int NO_COLLATION				= 148;		// There is no collation %s in this database
const int NO_COLLATIONS				= 149;		// There are no collations in this database
const int MSG_COLLATIONS			= 150;		// Collations:
const int NO_SECCLASS				= 151;      // There are no security classes for %s
const int NO_DB_WIDE_SECCLASS		= 152;      // There is no database-wide security class
const int CANNOT_GET_SRV_VER		= 153;		// Cannot get server version without database connection
const int BULK_PROMPT				= 156;		// "BULK> "
const int NO_CONNECTED_USERS		= 157;   	// There are no connected users
const int USERS_IN_DB				= 158;      // Users in the database
const int OUTPUT_TRUNCATED			= 159;      // Output was truncated
const int VALID_OPTIONS             = 160;      // Valid options are:
const int USAGE_FETCH				= 161;		// -f(etch_password)       fetch password from file
const int PASS_FILE_OPEN			= 162;		// could not open password file @1, errno @2
const int PASS_FILE_READ			= 163;		// could not read password file @1, errno @2
const int EMPTY_PASS				= 164;		// empty password file @1

// Initialize types

struct sqltypes
{
	SSHORT type;
	SCHAR type_name[WORDLENGTH];
};

//
// Use T_FLOAT and T_CHAR to avoid collisions with windows defines
//
const int SMALLINT		= 7;
const int INTEGER		= 8;
const int QUAD			= 9;
const int T_FLOAT		= 10;
const int T_CHAR		= 14;
const int DOUBLE_PRECISION = 27;
const int DATE			= 35;
const int VARCHAR		= 37;
const int CSTRING		= 40;
const int BLOB_ID		= 45;
const int BLOB			= 261;
//const int SQL_DATE	= 12;
//const int SQL_TIME	= 13;
const int BIGINT		= 16;

static const sqltypes Column_types[] = {
	{SMALLINT, "SMALLINT"},		// NTX: keyword
	{INTEGER, "INTEGER"},		// NTX: keyword
	{QUAD, "QUAD"},				// NTX: keyword
	{T_FLOAT, "FLOAT"},			// NTX: keyword
	{T_CHAR, "CHAR"},			// NTX: keyword
	{DOUBLE_PRECISION, "DOUBLE PRECISION"},	// NTX: keyword
	{VARCHAR, "VARCHAR"},		// NTX: keyword
	{CSTRING, "CSTRING"},		// NTX: keyword
	{BLOB_ID, "BLOB_ID"},		// NTX: keyword
	{BLOB, "BLOB"},				// NTX: keyword
	{blr_sql_time, "TIME"},		// NTX: keyword
	{blr_sql_date, "DATE"},		// NTX: keyword
	{blr_timestamp, "TIMESTAMP"},	// NTX: keyword
	{BIGINT, "BIGINT"},		// keyword
	{0, ""}
};

// Integral subtypes

const int MAX_INTSUBTYPES	= 2;

static const SCHAR* Integral_subtypes[] = {
	"UNKNOWN",					// Defined type, NTX: keyword
	"NUMERIC",					// NUMERIC, NTX: keyword
	"DECIMAL"					// DECIMAL, NTX: keyword
};

// Blob subtypes

const int MAX_BLOBSUBTYPES	= 8;

static const SCHAR* Sub_types[] = {
	"BINARY",					// NTX: keyword
	"TEXT",						// NTX: keyword
	"BLR",						// NTX: keyword
	"ACL",						// NTX: keyword
	"RANGES",					// NTX: keyword
	"SUMMARY",					// NTX: keyword
	"FORMAT",					// NTX: keyword
	"TRANSACTION_DESCRIPTION",	// NTX: keyword
	"EXTERNAL_FILE_DESCRIPTION"	// NTX: keyword
};

/* CVC: Notice that
BY REFERENCE is the default for scalars and can't be specified explicitly;
BY VMS_DESCRIPTOR is known simply as BY DESCRIPTOR and works for FB1;
BY ISC_DESCRIPTOR is the default for BLOBs and can't be used explicitly;
BY SCALAR_ARRAY_DESCRIPTOR is supported in FB2 as BY SCALAR_ARRAY, since
the server has already the capability to deliver arrays to UDFs;
BY REFERENCE_WITH_NULL his supported in FB2 to be able to signal SQL NULL
in input parameters.
The names mentioned here are documented in jrd/types.h. */

const int MAX_UDFPARAM_TYPES = 6;

static const char* UDF_param_types[] = {
	" BY VALUE",			// NTX: keyword
	"",						// BY REFERENCE
	" BY DESCRIPTOR",		// keyword in FB, internally VMS descriptor
	"",						// BY ISC_DESCRIPTOR => BLOB
	" BY SCALAR_ARRAY",		// keyword in FB v2
	" NULL",				// BY REFERENCE WITH NULL, but only appends NULL to the type
	" ERROR-type-unknown"
};

class IsqlGlobals
{
public:
	FILE* Out;
	FILE* Errfp;
	SCHAR global_Db_name[MAXPATHLEN];
	SCHAR global_Target_db[MAXPATHLEN];
	SCHAR global_Term[MAXTERM_SIZE];
	SCHAR User[128];
	SCHAR Role[256];
	USHORT SQL_dialect;
	USHORT db_SQL_dialect;
	// from isql.epp
	USHORT major_ods;
	USHORT minor_ods;
	void printf(const char* buffer, ...);
	void prints(const char* buffer);
};

extern IsqlGlobals isqlGlob;

static const char* const SCRATCH = "fb_query_";

inline void STDERROUT(const char* st, bool cr = true)
{
	fprintf (isqlGlob.Errfp, "%s", st);
	if (cr)
		fprintf (isqlGlob.Errfp, "\n");
	fflush (isqlGlob.Errfp);
}

#ifdef DEBUG_GDS_ALLOC
#define ISQL_ALLOC(x)     gds__alloc_debug(x, __FILE__, __LINE__)
#else
#define ISQL_ALLOC(x)     gds__alloc(x)
#endif
#define ISQL_FREE(x)     {gds__free(x); x = NULL;}

static const char* const NEWLINE		= "\n";
static const char* const TAB_AS_SPACES	= "        ";

const char BLANK		= '\040';
const char DBL_QUOTE	= '\042';
const char SINGLE_QUOTE	= '\'';

#endif // ISQL_ISQL_H
