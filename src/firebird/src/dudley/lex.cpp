/*
 *	PROGRAM:	JRD Data Definition Language
 *	MODULE:		lex.cpp
 *	DESCRIPTION:	Lexical analyser
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
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../jrd/ibase.h"
#include "../dudley/ddl.h"
#include "../dudley/ddl_proto.h"
#include "../dudley/hsh_proto.h"
#include "../dudley/lex_proto.h"
#include "../common/classes/TempFile.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

const char* const SCRATCH = "fb_query_";

static int nextchar();
static void retchar(SSHORT);
static int skip_white();

/* Input line control */

static FILE *input_file, *trace_file = NULL;
static TEXT *DDL_char, DDL_buffer[256], trace_file_name[MAXPATHLEN];


const SCHAR CHR_ident = 1;
const SCHAR CHR_letter = 2;
const SCHAR CHR_digit = 4;
const SCHAR CHR_quote = 8;
const SCHAR CHR_white = 16;
const SCHAR CHR_eol = 32;

const SCHAR CHR_IDENT = CHR_ident;
const SCHAR CHR_LETTER = CHR_letter | CHR_ident;
const SCHAR CHR_DIGIT = CHR_digit | CHR_ident;
const SCHAR CHR_QUOTE = CHR_quote;
const SCHAR CHR_WHITE = CHR_white;
const SCHAR CHR_EOL = CHR_white;


static const SCHAR classes_array[256] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, CHR_WHITE, CHR_EOL, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	CHR_WHITE, 0, CHR_QUOTE, CHR_IDENT, CHR_IDENT, 0, 0, CHR_QUOTE,
	0, 0, 0, 0, 0, 0, 0, 0,
	CHR_DIGIT, CHR_DIGIT, CHR_DIGIT, CHR_DIGIT, CHR_DIGIT, CHR_DIGIT,
		CHR_DIGIT, CHR_DIGIT,
	CHR_DIGIT, CHR_DIGIT, 0, 0, 0, 0, 0, 0,
	0, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER,
		CHR_LETTER,
	CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER,
		CHR_LETTER, CHR_LETTER,
	CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER,
		CHR_LETTER, CHR_LETTER,
	CHR_LETTER, CHR_LETTER, CHR_LETTER, 0, 0, 0, 0, CHR_LETTER,
	0, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER,
		CHR_LETTER,
	CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER,
		CHR_LETTER, CHR_LETTER,
	CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER, CHR_LETTER,
		CHR_LETTER, CHR_LETTER,
	CHR_LETTER, CHR_LETTER, CHR_LETTER, 0
};

inline SCHAR classes(int idx)
{
	return classes_array[(UCHAR) idx];
}

inline SCHAR classes(UCHAR idx)
{
	return classes_array[idx];
}



TOK LEX_filename()
{
/**************************************
 *
 *	L E X _ f i l e n a m e
 *
 **************************************
 *
 * Functional description
 *	Parse the next token as a filename.
 *
 **************************************/
	TOK token;
	SSHORT c;

	token = &dudleyGlob.DDL_token;
	TEXT* p = token->tok_string;
	*p++ = c = skip_white();

	if (dudleyGlob.DDL_eof) {
		token->tok_symbol = NULL;
		token->tok_keyword = KW_none;
		return NULL;
	}

	while (!(classes(c = nextchar()) & (CHR_white | CHR_eol)))
		*p++ = c;

	retchar(c);
	token->tok_length = p - token->tok_string;
	*p = 0;
	token->tok_symbol = NULL;
	token->tok_keyword = KW_none;

	return token;
}


void LEX_fini()
{
/**************************************
 *
 *	L E X _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Shut down LEX in a more or less clean way.
 *
 **************************************/

	if (trace_file != NULL) {
		fclose(trace_file);
		unlink(trace_file_name);
	}
}


void LEX_flush()
{
/**************************************
 *
 *	L E X _ f l u s h
 *
 **************************************
 *
 * Functional description
 *	Flush the input stream after an error.  For now, just look for
 *	an end of line.
 *
 **************************************/
	SSHORT c;

	while (!dudleyGlob.DDL_eof) {
		if ((c = nextchar()) == '\n')
			break;
	}
	*DDL_char = 0;
}


void LEX_get_text(UCHAR * buffer, TXT text)
{
/**************************************
 *
 *	L E X _ g e t _ t e x t
 *
 **************************************
 *
 * Functional description
 *	Write text from the scratch trace file into a buffer.
 *
 **************************************/
	SLONG start;
	int length;
	UCHAR *p;

	start = text->txt_position;
	length = text->txt_length;

	if (fseek(trace_file, start, 0)) {
		fseek(trace_file, 0, 2);
		DDL_err(275);
		/* msg 275: fseek failed */
	}

	p = buffer;
	while (length--)
		*p++ = getc(trace_file);

	fseek(trace_file, 0, 2);
}


void LEX_init( void *file)
{
/**************************************
 *
 *	L E X _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize for lexical scanning.  While we're at it, open a
 *	scratch trace file to keep all input.
 *
 **************************************/
	const Firebird::PathName filename = Firebird::TempFile::create(SCRATCH);
	strcpy(trace_file_name, filename.c_str());
	trace_file = fopen(trace_file_name, "w+b");
	if (!trace_file)
	{
		DDL_err(276);
		/* msg 276: couldn't open scratch file */
	}

	input_file = (FILE*) file;
	DDL_char = DDL_buffer;
	dudleyGlob.DDL_token.tok_position = 0;
	dudleyGlob.DDL_description = false;
	dudleyGlob.DDL_line = 1;
}


void LEX_put_text (FB_API_HANDLE blob, TXT text)
{
/**************************************
 *
 *	L E X _ p u t _ t e x t
 *
 **************************************
 *
 * Functional description
 *	Write text from the scratch trace file into a blob.
 *
 **************************************/
	SLONG start;
	ISC_STATUS_ARRAY status_vector;
	int length;
	SSHORT l, c;
	TEXT buffer[1024];

	start = text->txt_position;
	length = text->txt_length;

	if (fseek(trace_file, start, 0)) {
		fseek(trace_file, 0, 2);
		DDL_err(275);
		/* msg 275: fseek failed */
	}

	while (length) {
		TEXT* p = buffer;
		while (length) {
			--length;
			*p++ = c = getc(trace_file);
			if (c == '\n')
				break;
		}
		if (l = p - buffer)
			if (isc_put_segment(status_vector, &blob, l, buffer))
				DDL_err(277);
		/* msg 277: isc_put_segment failed */
	}

	fseek(trace_file, 0, 2);
}


void LEX_real()
{
/**************************************
 *
 *	L E X _ r e a l
 *
 **************************************
 *
 * Functional description
 *	If the token is an end of line, get the next token.
 *
 **************************************/

	if (dudleyGlob.DDL_token.tok_string[0] != '\n')
		return;

	LEX_token();
}


TOK LEX_token()
{
/**************************************
 *
 *	L E X _ t o k e n
 *
 **************************************
 *
 * Functional description
 *	Parse and return the next token.
 *
 **************************************/
	SSHORT c, next;
	SYM symbol;

	TOK token = &dudleyGlob.DDL_token;
	TEXT* p = token->tok_string;
	*p++ = c = skip_white();

/* On end of file, generate furious but phony end of line tokens */

	TEXT char_class = classes(c);

	if (dudleyGlob.DDL_eof) {
		p = token->tok_string;
		*p++ = '*';
		*p++ = 'E';
		*p++ = 'O';
		*p++ = 'F';
		*p++ = '*';
		token->tok_symbol = NULL;
		token->tok_keyword = KW_none;
		token->tok_type = tok_eol;
		token->tok_length = p - token->tok_string;
		*p = '\0';
		return NULL;
	}
	else if (char_class & CHR_letter) {
		while (classes(c = nextchar()) & CHR_ident)
			*p++ = c;

		retchar(c);
		token->tok_type = tok_ident;
	}
	else if (char_class & CHR_digit) {
		while (classes(c = nextchar()) & CHR_digit)
			*p++ = c;
		if (c == '.') {
			*p++ = c;
			while (classes(c = nextchar()) & CHR_digit)
				*p++ = c;
		}
		retchar(c);
		token->tok_type = tok_number;
	}
	else if ((char_class & CHR_quote) && !dudleyGlob.DDL_description) {
		token->tok_type = tok_quoted;
		do {
			if (!(next = nextchar()) || next == '\n') {
				DDL_err(278);
				/* msg 278: unterminated quoted string */
				break;
			}
			*p++ = next;
		} while (next != c);
	}
	else if (c == '\n')
		token->tok_type = tok_eol;
	else {
		token->tok_type = tok_punct;
		*p++ = nextchar();
		if (!HSH_lookup(token->tok_string, 2))
			retchar(*--p);
	}

	token->tok_length = p - token->tok_string;
	*p = '\0';
	token->tok_symbol = symbol = HSH_lookup(token->tok_string, token->tok_length);
	if (symbol && symbol->sym_type == SYM_keyword)
		token->tok_keyword = (enum kwwords) symbol->sym_keyword;
	else
		token->tok_keyword = KW_none;

	if (dudleyGlob.DDL_trace)
		puts(token->tok_string);

	return token;
}


static int nextchar()
{
/**************************************
 *
 *	n e x t c h a r
 *
 **************************************
 *
 * Functional description
 *	Get the next character from the input stream.
 *
 **************************************/
	SSHORT c;

/* mark the end of the buffer */

	const char* const end = DDL_buffer + sizeof(DDL_buffer);

/* If there isn't anything floating around, get a new line */

	while (!(c = *DDL_char++)) {
		DDL_char = DDL_buffer;
		if (dudleyGlob.DDL_interactive) {
			printf("%s", dudleyGlob.DDL_prompt);
			if (dudleyGlob.DDL_service)
				putc('\001', stdout);
			fflush(stdout);
		}
		while (c = getc(input_file)) {
			if (c == EOF && SYSCALL_INTERRUPTED(errno)) {
				errno = 0;
				continue;
			}
			if (c == EOF)
				break;
			if (DDL_char < end)
				*DDL_char++ = c;
			else
				DDL_err(279);
				/* msg 279: line too SLONG */
			if (c == '\n')
				break;
		}
		*DDL_char = 0;
		if (c == EOF && DDL_char == DDL_buffer) {
#ifdef UNIX
			if (dudleyGlob.DDL_interactive)
				printf("\n");
#endif
			dudleyGlob.DDL_eof = true;
			return EOF;
		}
		DDL_char = DDL_buffer;
		fputs(DDL_buffer, trace_file);
	}

	dudleyGlob.DDL_token.tok_position++;
	if (c == '\n') {
		++dudleyGlob.DDL_line;
#if (defined WIN_NT)
		/* need to account for extra linefeed on newline */

		dudleyGlob.DDL_token.tok_position++;
#endif
	}

	return c;
}


static void retchar( SSHORT c)
{
/**************************************
 *
 *	r e t c h a r
 *
 **************************************
 *
 * Functional description
 *	Return a character to the input stream.
 *
 **************************************/

	if (c == '\n') {
		--dudleyGlob.DDL_line;
#if (defined WIN_NT)
		/* account for the extra linefeed in a newline */

		--dudleyGlob.DDL_token.tok_position;
#endif
	}

	--dudleyGlob.DDL_token.tok_position;
	--DDL_char;
}


static int skip_white()
{
/**************************************
 *
 *	s k i p _ w h i t e
 *
 **************************************
 *
 * Functional description
 *	Skip over white space and comments in input stream
 *
 **************************************/
	SSHORT c;

	while ((c = nextchar()) != EOF) {

		const SSHORT char_class = classes(c);
		if (char_class & CHR_white)
			continue;
		if (c == '/') {
			SSHORT next = nextchar();
			if (next != '*') {
				retchar(next);
				return c;
			}
			c = nextchar();
			while ((next = nextchar()) &&
				   (next != EOF) && !(c == '*' && next == '/'))
			{
				c = next;
			}
			continue;
		}
		break;
	}

	return c;
}
