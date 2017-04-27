/*
 *	PROGRAM:	Interactive SQL utility
 *	MODULE:		isqlw_proto.h
 *	DESCRIPTION:	Prototype header file for isql_win.c
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

#ifndef ISQL_WIN_PROTO_H
#define ISQL_WIN_PROTO_H

BOOL CALLBACK _export aboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export blobDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export charSetDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export createDbDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export dbNameDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export dropDbDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export execDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export extractDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK _export isqlWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export objectDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export outputDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export scriptDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export sessionDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export termDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK _export transDlgProc(HWND, UINT, WPARAM, LPARAM);
int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

#endif // ISQL_WIN_PROTO_H

