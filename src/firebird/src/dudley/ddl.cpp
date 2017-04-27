/*
 *	PROGRAM:	Data Definition Utility
 *	MODULE:		ddl.cpp
 *	DESCRIPTION:	Main line routine
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
#include <stdlib.h>
#include <string.h>
#include "../dudley/ddl.h"
#include "../jrd/ibase.h"
#include "../jrd/license.h"
#include "../dudley/ddl_proto.h"
#include "../dudley/exe_proto.h"
#include "../dudley/expan_proto.h"
#include "../dudley/extra_proto.h"
#include "../dudley/hsh_proto.h"
#include "../dudley/lex_proto.h"
#include "../dudley/parse_proto.h"
#include "../dudley/trn_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/why_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_IO_H
#include <io.h> // isatty
#endif

#include "../common/classes/MsgPrint.h"
#include "../common/utils_proto.h"

using MsgFormat::SafeArg;


DudleyGlobals dudleyGlob;

static dudley_lls* free_stack;
static TEXT DDL_message[256];

static const char* const FOPEN_INPUT_TYPE	= "r";

const char* DDL_EXT		= ".gdl";	// normal extension for a ddl file
const int MAX_ERRORS	= 50;

enum in_sw_values
{
	IN_SW_GDEF_0 = 0,		// null switch value
	IN_SW_GDEF_G,			// generate DDL from a database file
	IN_SW_GDEF_R,			// replace existing database
	IN_SW_GDEF_D,			// generate dynamic DDL
	IN_SW_GDEF_Z,			// print version number
	IN_SW_GDEF_T,			// print tokens as they are read
	IN_SW_GDEF_C = 7,		// source is C
	IN_SW_GDEF_F,			// source is FORTRAN
	IN_SW_GDEF_P, 			// source is PASCAL
	IN_SW_GDEF_COB, 		// source is (shudder) cobol
	IN_SW_GDEF_ANSI,		// source is (worse and worse!) ansi format
	IN_SW_GDEF_ADA = 14,	// source is ada
	IN_SW_GDEF_CXX,			// source is C++
	IN_SW_GDEF_USER = 17,	// user name for PC security
	IN_SW_GDEF_PASSWORD,	// password for PC security
#ifdef TRUSTED_AUTH
	IN_SW_GDEF_TRUSTED,		// trusted auth
#endif
	IN_SW_GDEF_FETCH_PASS	// fetch password from file
};

static const in_sw_tab_t gdef_in_sw_table[] =
{
	{ IN_SW_GDEF_G, 0, "EXTRACT", 0, 0, 0, false, 0, 0,
		"\t\textract definition from database"}, 	/* extract DDL from database */
	{ IN_SW_GDEF_R, 0, "REPLACE", 0, 0, 0, false, 0, 0,
		"\t\treplace existing database"},	/* replace database */
	{ IN_SW_GDEF_D, 0, "DYNAMIC", 0, 0, 0, false, 0, 0,
		"\t\tgenerate dynamic DDL"},
	{ IN_SW_GDEF_T, 0, "T", 0, 0, 0, false, 0, 0, NULL },
	{ IN_SW_GDEF_C, 0, "C", 0, 0, 0, false, 0, 0, "\t\tDYN for C" },
	{ IN_SW_GDEF_F, 0, "FORTRAN", 0, 0, 0, false, 0, 0, "\t\tDYN for FORTRAN" },
	{ IN_SW_GDEF_P, 0, "PASCAL", 0, 0, 0, false, 0, 0, "\t\tDYN for PASCAL" },
	{ IN_SW_GDEF_COB, 0, "COB", 0, 0, 0, false, 0, 0, "\t\tDYN for COBOL" },
	{ IN_SW_GDEF_ANSI, 0, "ANSI", 0, 0, 0, false, 0, 0,
		"\t\tDYN for ANSI COBOL" },
	{ IN_SW_GDEF_ADA, 0, "ADA", 0, 0, 0, false, 0, 0, "\t\tDYN for ADA" },
	{ IN_SW_GDEF_CXX, 0, "CXX", 0, 0, 0, false, 0, 0, "\t\tDYN for C++" },
	{ IN_SW_GDEF_USER, 0, "USER", 0, 0, 0, false, 0, 0,
		"\t\tuser name to use in attaching database" },
	{ IN_SW_GDEF_PASSWORD, 0, "PASSWORD", 0, 0, 0, false, 0, 0,
		"\t\tpassword to use with user name" },
	{ IN_SW_GDEF_FETCH_PASS, 0, "FETCH_PASSWORD", 0, 0, 0, false, 0, 0,
		"\t\tfetch password from file" },
#ifdef TRUSTED_AUTH
	{ IN_SW_GDEF_TRUSTED, 0, "TRUSTED", 0, 0, 0, false, 0, 0,
		"\t\tuse trusted authentication" },
#endif
	{ IN_SW_GDEF_Z, 0, "Z", 0, 0, 0, false, 0, 0, "\t\tprint version number" },
	{ IN_SW_GDEF_0, 0, NULL, 0, 0, 0, false, 0, 0, NULL }
};

#ifndef SUPERSERVER
int CLIB_ROUTINE main( int argc, char* argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Main line routine for C preprocessor.  Initializes
 *	system, performs pass 1 and pass 2.  Interpretes
 *	command line.
 *
 **************************************/

// CVC: Notice that gdef is NEVER run as a service! It doesn't make sense.
/* Perform some special handling when run as a Firebird service.  The
   first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
   by 3 file descriptors to use in re-directing stdin, stdout, and stderr. */

	dudleyGlob.DDL_service = false;

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		dudleyGlob.DDL_service = true;
		argv++;
		argc--;
	}
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		dudleyGlob.DDL_service = true;
		long redir_in = atol(argv[2]);
		long redir_out = atol(argv[3]);
		long redir_err = atol(argv[4]);
#ifdef WIN_NT
		redir_in = _open_osfhandle(redir_in, 0);
		redir_out = _open_osfhandle(redir_out, 0);
		redir_err = _open_osfhandle(redir_err, 0);
#endif
		if (redir_in != 0)
			if (dup2((int) redir_in, 0))
				close((int) redir_in);
		if (redir_out != 1)
			if (dup2((int) redir_out, 1))
				close((int) redir_out);
		if (redir_err != 2)
			if (dup2((int) redir_err, 2))
				close((int) redir_err);
		argv += 4;
		argc -= 4;
	}

	dudleyGlob.DDL_file_name = NULL;
	dudleyGlob.DB_file_name = NULL;
	dudleyGlob.DDL_drop_database = false;
	dudleyGlob.DDL_quit = false;
	dudleyGlob.DDL_extract = false;
	dudleyGlob.DDL_dynamic = false;
	dudleyGlob.DDL_trace = false;
	dudleyGlob.DDL_version = false;
#ifdef TRUSTED_AUTH
	dudleyGlob.DDL_trusted = false;
#endif
	dudleyGlob.DDL_default_user = NULL;
	dudleyGlob.DDL_default_password = NULL;

	TEXT file_name_1[256], file_name_2[256];
	file_name_1[0] = file_name_2[0] = 0;
	USHORT in_sw = 0; // silence uninitialized warning

	for (--argc; argc; argc--) {
		const TEXT* string = *++argv;
		if ((*string != '-') && (*string != '?')) {
			if (!*file_name_1)
				strcpy(file_name_1, string);
			else
				strcpy(file_name_2, string);
			continue;
		}
		else if (*string == '-') {
			/* iterate through the switch table, looking for matches */

			in_sw = IN_SW_GDEF_0;
			const TEXT* q;
			for (const in_sw_tab_t* in_sw_tab = gdef_in_sw_table;
				q = in_sw_tab->in_sw_name; in_sw_tab++)
			{
				const TEXT* p = string + 1;

				/* handle orphaned hyphen case */

				if (!*p--)
					break;

				/* compare switch to switch name in table */

				while (*p) {
					if (!*++p)
						in_sw = in_sw_tab->in_sw;
					if (UPPER(*p) != *q++)
						break;
				}

				/* end of input means we got a match.  stop looking */

				if (!*p)
					break;
			}
		}
		switch (in_sw) {
		case IN_SW_GDEF_D:
			dudleyGlob.DDL_dynamic = true;
			dudleyGlob.DYN_file_name[0] = 0;
			if (argc == 1)
				break;
			string = *++argv;
			if (*string != '-') {
				argc--;
				strcpy(dudleyGlob.DYN_file_name, string);
			}
			else
				argv--;
			break;

		case IN_SW_GDEF_C:
			dudleyGlob.language = lan_c;
			break;

		case IN_SW_GDEF_P:
			dudleyGlob.language = lan_pascal;
			break;

		case IN_SW_GDEF_COB:
			dudleyGlob.language = lan_cobol;
			break;

		case IN_SW_GDEF_ANSI:
			dudleyGlob.language = lan_ansi_cobol;
			break;
		case IN_SW_GDEF_F:
			dudleyGlob.language = lan_fortran;
			break;

		case IN_SW_GDEF_ADA:
			dudleyGlob.language = lan_ada;
			break;

		case IN_SW_GDEF_CXX:
			dudleyGlob.language = lan_cxx;
			break;

		case IN_SW_GDEF_G:
			dudleyGlob.DDL_extract = true;
			break;

		case IN_SW_GDEF_R:
			dudleyGlob.DDL_replace = true;
			break;

		case IN_SW_GDEF_T:
			dudleyGlob.DDL_trace = true;
			break;

		case IN_SW_GDEF_Z:
			DDL_msg_put(0, SafeArg() << GDS_VERSION);	/* msg 0: gdef version %s\n */
			dudleyGlob.DDL_version = true;
			break;

		case IN_SW_GDEF_PASSWORD:
			if (argc > 1) {
				dudleyGlob.DDL_default_password = fb_utils::get_passwd(*++argv);
				argc--;
			}

		case IN_SW_GDEF_FETCH_PASS:
			if (argc > 1) {
				if (fb_utils::fetchPassword(*++argv, dudleyGlob.DDL_default_password) != fb_utils::FETCH_PASS_OK)
				{
					DDL_msg_put(345);	// msg 4: gdef: Error fetching password from file
					DDL_exit(FINI_ERROR);
				}
				argc--;
			}
			break;

		case IN_SW_GDEF_USER:
			if (argc > 1) {
				dudleyGlob.DDL_default_user = *++argv;
				argc--;
			}
			break;

#ifdef TRUSTED_AUTH
		case IN_SW_GDEF_TRUSTED:
			dudleyGlob.DDL_trusted = true;
			break;
#endif

		case IN_SW_GDEF_0:
			if (*string != '?')
				DDL_msg_put(1, SafeArg() << string);	/* msg 1: gdef: unknown switch %s */
			DDL_msg_put(2);	/* msg 2: \tlegal switches are: */
			for (const in_sw_tab_t* in_sw_tab = gdef_in_sw_table;
				in_sw_tab->in_sw; in_sw_tab++)
			{
				if (in_sw_tab->in_sw_text) {
					DDL_msg_put(3, SafeArg() << in_sw_tab->in_sw_name <<
								in_sw_tab->in_sw_text);	/* msg 3: %s%s */
				}
			}
			DDL_exit(FINI_ERROR);
		}
	}

	FILE* input_file;

	if (dudleyGlob.DDL_extract) {
		strcpy(dudleyGlob.DB_file_string, file_name_1);
		strcpy(dudleyGlob.DDL_file_string, file_name_2);
		if (!*dudleyGlob.DB_file_string) {
			DDL_msg_put(4);	/* msg 4: gdef: Database name is required for extract */
			DDL_exit(FINI_ERROR);
		}
		dudleyGlob.DB_file_name = dudleyGlob.DB_file_string;
		dudleyGlob.DDL_file_name = dudleyGlob.DDL_file_string;
		DDL_ext();
		DDL_exit(FINI_OK);
	}
	else if (*file_name_1) {
		strcpy(dudleyGlob.DDL_file_string, file_name_1);
		dudleyGlob.DDL_file_name = dudleyGlob.DDL_file_string;
	}
	if (dudleyGlob.DDL_file_name == NULL) {
		dudleyGlob.DDL_file_name = "standard input";
		input_file = stdin;
		dudleyGlob.DDL_interactive = dudleyGlob.DDL_service || isatty(0);
	}
	else {
		/*
		   * try to open the input DDL file.
		   * If it already has a .GDL extension, just try to open it.
		   * Otherwise, add the extension, try, remove the extension,
		   * and try again.
		 */

		input_file = NULL;

		/* first find the extension by going to the end and backing up */

		const TEXT* p = dudleyGlob.DDL_file_name;
		while (*p)
			++p;
		while ((p != dudleyGlob.DDL_file_name) && (*p != '.') && (*p != '/'))
			--p;

		/* then handle the case where the input already ends in .GDL */

		if (*p == '.') {
			for (const TEXT* q2 = DDL_EXT; UPPER(*p) == UPPER(*q2); p++, q2++) {
				if (!*p) {
					input_file = fopen(dudleyGlob.DDL_file_name, FOPEN_INPUT_TYPE);
					if (!input_file) {
						DDL_msg_put(5, SafeArg() << dudleyGlob.DDL_file_name);
						/* msg 5: gdef: can't open %s */
						DDL_exit(FINI_ERROR);
					}
				}
			}
		}

		/* if we got this far without opening it, it's time to add the new extension */

		if (!input_file) {
			sprintf(file_name_1, "%s%s", dudleyGlob.DDL_file_name, DDL_EXT);
			input_file = fopen(file_name_1, FOPEN_INPUT_TYPE);
			if (input_file)
				dudleyGlob.DDL_file_name = file_name_1;
			else {
				input_file = fopen(dudleyGlob.DDL_file_name, FOPEN_INPUT_TYPE);
				if (!input_file)
					DDL_msg_put(6, SafeArg() << dudleyGlob.DDL_file_name << file_name_1);
				/* msg 6: gdef: can't open %s or %s */
			}
		}
		if (!input_file)
			DDL_exit(FINI_ERROR);
	}

	LEX_init(input_file);
	HSH_init();
	PARSE_actions();

	if (input_file != stdin)
		fclose(input_file);

	if (dudleyGlob.DDL_actions && ((dudleyGlob.DDL_errors && dudleyGlob.DDL_interactive) || dudleyGlob.DDL_quit)) {
		rewind(stdin);
		if (dudleyGlob.DDL_errors > 1)
			DDL_msg_partial(7, SafeArg() << dudleyGlob.DDL_errors);	/* msg 7: \n%d errors during input. */
		else if (dudleyGlob.DDL_errors)
			DDL_msg_partial(9);	/* msg 9: \n1 error during input. */
		else
			DDL_msg_partial(8);	/* msg 8: \nNo errors. */
		if (DDL_yes_no(10)) { // msg 10 : save changes before exiting?
			dudleyGlob.DDL_quit = false;
			dudleyGlob.DDL_errors = 0;
		}
	}

/* Reverse the set of actions */

	act* stack = NULL;
	while (dudleyGlob.DDL_actions) {
		act* temp = dudleyGlob.DDL_actions;
		dudleyGlob.DDL_actions = temp->act_next;
		temp->act_next = stack;
		stack = temp;
	}
	dudleyGlob.DDL_actions = stack;

	if (!dudleyGlob.DDL_errors && !dudleyGlob.DDL_quit) {
		EXP_actions();
		if (!dudleyGlob.DDL_errors && !dudleyGlob.DDL_quit) {
			EXE_execute();
			if (dudleyGlob.DDL_dynamic)
				TRN_translate();
		}
	}

	if (dudleyGlob.DDL_actions && (dudleyGlob.DDL_errors || dudleyGlob.DDL_quit))
		if (dudleyGlob.DDL_errors)
			DDL_msg_put(307);	/* msg 307: Ceasing processing because of errors. */
		else
			DDL_msg_put(308);	/* msg 308: Ceasing processing. */

	EXE_fini(dudleyGlob.database);

	if (dudleyGlob.DDL_errors) {
		if (dudleyGlob.database && (dudleyGlob.database->dbb_flags & DBB_create_database)) {
			for (const fil* file = dudleyGlob.database->dbb_files; file;
				file = file->fil_next)
			{
				unlink(file->fil_name->sym_name);
			}
			unlink(dudleyGlob.database->dbb_name->sym_string);
		}
		DDL_exit(FINI_ERROR);
	}

	DDL_exit(FINI_OK);
	// This will never execute, see exit() in DDL_exit. Make the compiler happy.
	return 0;
}
#endif

UCHAR *DDL_alloc(int size)
{
/**************************************
 *
 *	D D L _ a l l o c
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	UCHAR* const block = (UCHAR*) gds__alloc((SLONG) size);
	UCHAR* p = block;

#ifdef DEBUG_GDS_ALLOC
/* For V4.0 we don't care about gdef specific memory leaks */
	gds_alloc_flag_unfreed(block);
#endif

	if (!p)
		DDL_err(14);	/* msg 14: memory exhausted */
	else
		do {
			*p++ = 0;
		} while (--size);

	return block;
}


int DDL_db_error(ISC_STATUS* status_vector,
				 USHORT number,
				 const SafeArg& arg)
{
/**************************************
 *
 *	D D L _ d b _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Issue an error message.
 *
 **************************************/

	gds__print_status(status_vector);

	return DDL_err(number, arg);
}


int DDL_err(USHORT number,
			const SafeArg& arg)
{
/**************************************
 *
 *	D D L _ e r r
 *
 **************************************
 *
 * Functional description
 *	Issue an error message. Quit if >MAX_ERRORS
 *
 **************************************/

	SafeArg temp;
	DDL_msg_partial(15, temp << dudleyGlob.DDL_file_name << dudleyGlob.DDL_line);	/*msg 15: %s:%d: */
	DDL_msg_put(number, arg);
	if (dudleyGlob.DDL_errors++ > MAX_ERRORS) {
		temp.clear();
		DDL_msg_put(16, temp << MAX_ERRORS);	/* msg 16: error count exceeds limit (%d) */
		DDL_msg_put(17);	/* msg 17: what we have here is a failure to communicate! */
		if (dudleyGlob.database && (dudleyGlob.database->dbb_flags & DBB_create_database))
			unlink(dudleyGlob.DB_file_name);
		DDL_exit(FINI_ERROR);
	}

	return 0;
}


void DDL_error_abort(ISC_STATUS* status_vector,
					 USHORT number,
					 const SafeArg& arg)
{
/**************************************
 *
 *	D D L _ e r r o r _ a b o r t
 *
 **************************************
 *
 * Functional description
 *	Things are going very poorly, so put out an error message
 *	and give up.
 *
 **************************************/

	if (status_vector)
		gds__print_status(status_vector);

	DDL_err(number, arg);
	DDL_exit(FINI_ERROR);
}


void DDL_exit( int stat)
{
/**************************************
 *
 *	D D L _ e x i t
 *
 **************************************
 *
 * Functional description
 *	Exit with status.
 *
 **************************************/

	LEX_fini();
	exit(stat);
}


void DDL_msg_partial(USHORT number,
					 const SafeArg& arg)
{
/**************************************
 *
 *	D D L _ m s g _ p a r t i a l
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it
 *	without a newline.
 *
 **************************************/

	fb_msg_format(0, DDL_MSG_FAC, number, sizeof(DDL_message), DDL_message, arg);
	printf("%s", DDL_message);
}


void DDL_msg_put(USHORT number,
				 const SafeArg& arg)
{
/**************************************
 *
 *	D D L _ m s g _ p u t
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it.
 *
 **************************************/

	fb_msg_format(0, DDL_MSG_FAC, number, sizeof(DDL_message), DDL_message, arg);
	printf("%s\n", DDL_message);
}


DUDLEY_NOD DDL_pop(dudley_lls** pointer)
{
/**************************************
 *
 *	D D L _ p o p
 *
 **************************************
 *
 * Functional description
 *	Pop an item off a linked list stack.  Free the stack node.
 *
 **************************************/
	dudley_lls* stack = *pointer;
	DUDLEY_NOD node = stack->lls_object;
	*pointer = stack->lls_next;
	stack->lls_next = free_stack;
	free_stack = stack;

	return node;
}


void DDL_push( DUDLEY_NOD object, dudley_lls** pointer)
{
/**************************************
 *
 *	D D L _ p u s h
 *
 **************************************
 *
 * Functional description
 *	Push an arbitrary object onto a linked list stack.
 *
 **************************************/
	dudley_lls* stack;

	if (free_stack) {
		stack = free_stack;
		free_stack = stack->lls_next;
	}
	else
		stack = (dudley_lls*) DDL_alloc(sizeof(dudley_lls));

	stack->lls_object = object;
	stack->lls_next = *pointer;
	*pointer = stack;
}


bool DDL_yes_no( USHORT number)
{
/**************************************
 *
 *	D D L _ y e s _ n o
 *
 **************************************
 *
 * Functional description
 *	Ask a yes/no question.
 *
 **************************************/
	TEXT prompt[128], reprompt[128], yes_ans[128], no_ans[128];

	static const SafeArg dummy;

	fb_msg_format(0, DDL_MSG_FAC, number, sizeof(prompt), prompt, dummy);

	USHORT yes_num = 342;				/* Msg342 YES   */
	USHORT no_num = 343;				/* Msg343 NO    */
	USHORT re_num = 344;				/* Msg344 Please respond with YES or NO. */
	reprompt[0] = '\0';

	if (fb_msg_format
		(0, DDL_MSG_FAC, no_num, sizeof(no_ans), no_ans, dummy) <= 0)
	{
		strcpy(no_ans, "NO");	/* default if msg_format fails */
	}
	if (fb_msg_format
		(0, DDL_MSG_FAC, yes_num, sizeof(yes_ans), yes_ans, dummy) <= 0)
	{
		strcpy(yes_ans, "YES");
	}

	for (;;) {
		printf("%s", prompt);
		if (dudleyGlob.DDL_service)
			putc('\001', stdout);
		fflush(stdout);
		int count = 0;
		int c;
		while ((c = getc(stdin)) == ' ')
			count++;
		if (c != '\n' && c != EOF)
		{
			int d;
			while ((d = getc(stdin)) != '\n' && d != EOF);
		}
		if (!count && c == EOF)
			return false;
		if (UPPER(c) == UPPER(yes_ans[0]))
			return true;
		if (UPPER(c) == UPPER(no_ans[0]))
			return false;
		if (!reprompt &&
			fb_msg_format(0, DDL_MSG_FAC, re_num, sizeof(reprompt), reprompt, dummy) <= 0)
		{
			sprintf(reprompt, "Please respond with YES or NO.");	/* default if msg_format fails */
		}
		printf("%s\n", reprompt);
	}
}

