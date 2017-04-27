/*
 *	PROGRAM:	Interactive SQL utility
 *	MODULE:		isql_win.h
 *	DESCRIPTION:	Windows ISQL include file
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

#ifndef ISQL_WIN_H
#define ISQL_WIN_H

// prototypes

static void close_isql();
static int cmdline_isql(HINSTANCE, LPSTR);
static void display_page(HWND);
static SSHORT init_isql(HINSTANCE, HINSTANCE, int);
static void init_isql_added(HINSTANCE);
static void init_isql_every(HINSTANCE, int);
static void init_isql_first(HINSTANCE);
static SSHORT open_temp_file(HINSTANCE, FILE**, SCHAR*, SSHORT);
static void paint_isql(HWND);
static void pusharg(const char*);
static void setup_scroll(HWND);
static void test_overwrite();
static int windows_isql(HINSTANCE, HINSTANCE, int);
static void xfer_file(const char*, const char*, bool);

#endif // ISQL_WIN_H

