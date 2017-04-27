/*
 *	PROGRAM:	Windows Interactive SQL utility
 *	MODULE:		isql_win.cpp
 *	DESCRIPTION:	Windows shell for ISQL
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
#include <windows.h>
#pragma hdrstop
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#include <bios.h>
#include <ctype.h>
#include <io.h>
#include <dos.h>
#include <dir.h>
#include <windowsx.h>

#include "../jrd/common.h"
#include "../isql/isql.h"
#include "../isql/isql_win.h"
#include "../isql/isql.rh"
#include "../jrd/ibase.h"
#include "../isql/isqlw_proto.h"
#include "../isql/isql_proto.h"
#include "../jrd/constants.h"

struct scrollkeys
{
	WORD wVirtkey;
	int iMessage;
	WORD wRequest;
};

scrollkeys key2scroll[] =
{
	{VK_HOME, WM_COMMAND, IDM_HOME},
	{VK_END, WM_VSCROLL, SB_BOTTOM},
	{VK_PRIOR, WM_VSCROLL, SB_PAGEUP},
	{VK_NEXT, WM_VSCROLL, SB_PAGEDOWN},
	{VK_UP, WM_VSCROLL, SB_LINEUP},
	{VK_DOWN, WM_VSCROLL, SB_LINEDOWN},
	{VK_LEFT, WM_HSCROLL, SB_PAGEUP},
	{VK_RIGHT, WM_HSCROLL, SB_PAGEDOWN}
};

// data initialized by first instance

struct tagSETUPDATA
{
	SCHAR appName[20];
	SCHAR menuName[20];
	SCHAR iconName[20];
	SCHAR errorString[20];
};
typedef tagSETUPDATA SETUPDATA;

// various temp file names

static SCHAR defInputFile[MAXPATHLEN];	// default input file name
static SCHAR defOutputFile[MAXPATHLEN];	// default output file name
static SCHAR defHistFile[MAXPATHLEN];	// command history file name
static SCHAR defSessionFile[MAXPATHLEN];	// SQL session file
static FILE *ipf;			// input file
static FILE *opf;			// output file
static FILE *chf;			// command history
static FILE *sss;			// SQL session

// global flags

static SSHORT gflags;

const SSHORT DBINITED	= 1;			// database initilized flag
const SSHORT DEFINPUT	= 2;			// default input file exists flag
const SSHORT DEFOUTPUT	= 4;			// default output file exists flag
const SSHORT COMHIST	= 8;			// command history file exists flag
const SSHORT OVERWRITE	= 16;			// overwrite/append to window
const SSHORT SESSFILE	= 32;			// SQL session file exists flag

SETUPDATA SetUpData;

/*
   Data that can be referenced throughout the
   program but not passed to other instances
*/

HINSTANCE hInst;				// hInstance of application
HWND hWndMain;					// hWnd of main window

int xChar, yChar, yCharnl;		// character size
int xClient, yClient;			// client window size

LOGFONT cursfont;				// font structure
HFONT holdsfont;				// handle of original font
HFONT hnewsfont;				// handle of new fixed font
SCHAR tmpDialogParam[1024];		// used by dialog boxes

// window scroll/paint stuff

int nVscrollMax, nHscrollMax;	// scroll ranges
int nVscrollPos, nHscrollPos;	// current scroll positions
int numlines;					// number of lines in file
int maxwidth;					// width of display format
int nVscrollInc, nHscrollInc;	// scroll increments
int nPageMaxLines;				// max lines on screen

// arguments passed to ISQL

int ISQL_argc;					// argument count
char *ISQL_argv[20];			// argument vector
char ISQL_args[1024];			// space for arguments
char *ISQL_cursor;				// cursor into arguments

// database startup parameters

static SCHAR newDataBase[256];
static SCHAR newUserName[MAX_SQL_IDENTIFIER_SIZE];
static SCHAR newPassword[16];

// script parameters
static SCHAR scriptName[256];
static SCHAR scriptOutput[256];

// extract parameters
static SCHAR extractDbName[256];
static SCHAR extractOutput[256];
static SCHAR extractTarget[256];

static SSHORT allblobsflag;
static SSHORT allobjectsflag;


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int cmdShow)
{
/**************************************************
 *
 *      W i n M a i n
 *
 **************************************************
 *
 * Functional description:
 *      WinMain shell for the Windows ISQL function.
 *
 *
 * paramaters:
 *             hInstance     - The instance of this instance of this
 *                             application.
 *             hPrevInstance - The instance of the previous instance
 *                             of this application. This will be 0
 *                             if this is the first instance.
 *             pCmdLine      - A long pointer to the command line that
 *                             started this application.
 *             cmdShow       - Indicates how the window is to be shown
 *                             initially. ie. SW_SHOWNORMAL, SW_HIDE,
 *                             SW_MIMIMIZE.
 *
 * returns:
 *             wParam from last message.
 *
 ********************************************************************/

	if (pCmdLine && *pCmdLine)
		return cmdline_isql(hInstance, pCmdLine);

	return windows_isql(hInstance, hPrevInstance, cmdShow);
}


LRESULT CALLBACK _export ISQLWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      I S Q L W n d P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      This handles all messages for the application's main window,
 *      including scrolling and menu picks.
 *
 * paramaters:
 *             hWnd          - The window handle for this message
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
 ********************************************************************/
	DLGPROC lpproc;				// pointer to thunk for dialog box
	SCHAR buf[1024];			// temp buffer
	SCHAR pwbuf[50];
	SCHAR unbuf[50];
	int ret;

	switch (message)
	{
	case WM_CREATE:
		nVscrollPos = 0;
		nHscrollPos = 0;
		return (DefWindowProc(hWnd, message, wParam, lParam));

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDM_QUIT:
			// User selected Quit on menu
			PostMessage(hWnd, WM_CLOSE, 0, 0L);
			break;

		case IDM_HOME:
			// Used to implement home to topleft from keyboard.
			SendMessage(hWnd, WM_HSCROLL, GET_WM_HSCROLL_MPS(SB_TOP, 0, 0));
			SendMessage(hWnd, WM_VSCROLL, GET_WM_VSCROLL_MPS(SB_TOP, 0, 0));
			break;

		case IDM_ABOUT:
			// Display about box.
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) aboutDlgProc, hInst);
			DialogBox(hInst, MAKEINTRESOURCE(ABOUT), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			break;

		case IDM_EXEC_SQL:
			tmpDialogParam[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) execDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(EXEC_SQL), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret)
			{
				fprintf(sss, "%s\n\r", tmpDialogParam);
				fflush(sss);
				test_overwrite();
				ISQL_sql_statement(tmpDialogParam, ipf, opf, chf);
				fprintf(opf, "\n\r\n\r");
				display_page(hWnd);
			}
			break;

		case IDM_CONNECT_DB:
			newDataBase[0] = '\0';
			newUserName[0] = '\0';
			newPassword[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) dbNameDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(CONNECT_DB), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret) {
				ISQL_exit_db();
				if (ISQL_init
					(newDataBase, newUserName, newPassword, ipf,
					 opf)) gflags |= DBINITED;
			}
			break;

		case IDM_CREATE_DB:
			newDataBase[0] = '\0';
			newUserName[0] = '\0';
			newPassword[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) createDbDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(CREATE_DB), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret)
			{
				ISQL_exit_db();
				if (newUserName[0])
					sprintf(unbuf, " USER \"%s\" ", newUserName);
				else
					unbuf[0] = '\0';
				if (newPassword[0])
					sprintf(pwbuf, " PASSWORD \"%s\" ", newPassword);
				else
					pwbuf[0] = '\0';
				sprintf(buf, "CREATE DATABASE \"%s\" %s %s",
						newDataBase, unbuf, pwbuf);
				ISQL_frontend_command(buf, ipf, opf, chf);
				gflags |= DBINITED;
			}
			break;

		case IDM_DROP_DB:
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) dropDbDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(DROP_DB), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret) {
				ISQL_frontend_command("DROP DATABASE", ipf, opf, chf);
				gflags &= ~DBINITED;
			}
			break;

		case IDM_SAVE_SESSION:
			tmpDialogParam[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) sessionDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(SAVE_SESSION), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret) {
				fflush(chf);
				fclose(chf);
				xfer_file(defHistFile, tmpDialogParam, false);
				chf = fopen(defHistFile, "a");
			}
			break;

		case IDM_SAVE_OUTPUT:
			tmpDialogParam[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) outputDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(SAVE_OUTPUT), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret) {
				fflush(opf);
				fclose(opf);
				xfer_file(defOutputFile, tmpDialogParam, false);
				opf = fopen(defOutputFile, "a");
			}
			break;

		case IDM_EXEC_SCRIPT:
			scriptName[0] = '\0';
			scriptOutput[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) scriptDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(EXEC_SCRIPT), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret)
			{
				// generate an argc/argv

				ISQL_cursor = ISQL_args;
				ISQL_argc = 0;
				pusharg("isql");
				pusharg("-input");
				pusharg(scriptName);
				pusharg("-output");

				// use specified output file, or default

				if (scriptOutput[0])
					pusharg(scriptOutput);
				else
					pusharg(defOutputFile);
				fclose(opf);
				fclose(ipf);

				/* if database already open, exit and add database
				   name to arguments */

				if (gflags & DBINITED) {
					ISQL_exit_db();
					pusharg(newDataBase);
				}

				/* if overwrite flag is set, make sure ISQL appends to
				   an empty file */

				if (gflags & OVERWRITE) {
					opf = fopen(defOutputFile, "w");
					fclose(opf);
				}
				ISQL_main(ISQL_argc, ISQL_argv);

				// reopen default files and database

				ipf = fopen(defInputFile, "r");
				opf = fopen(defOutputFile, "a");
				if (gflags & DBINITED)
					ISQL_init(newDataBase, newUserName, newPassword, ipf, opf);
				fprintf(opf, "\n\r\n\r");
				display_page(hWnd);
			}
			break;

		case IDM_EXTRACT_DB:
			extractOutput[0] = '\0';
			extractDbName[0] = '\0';
			extractTarget[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) extractDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(EXTRACT_DB), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret)
			{
				// create an argument vector for ISQL

				ISQL_cursor = ISQL_args;
				ISQL_argc = 0;
				pusharg("isql");
				pusharg("-extract");
				pusharg("-o");
				if (extractOutput[0])
					pusharg(extractOutput);
				else
					pusharg(defOutputFile);
				pusharg(extractDbName);
				if (extractTarget[0]) {
					pusharg("-database");
					pusharg(extractTarget);
				}
				fclose(opf);
				fclose(ipf);
				ISQL_exit_db();
				ISQL_main(ISQL_argc, ISQL_argv);
				ISQL_exit_db();

				// reopen default files and database

				ipf = fopen(defInputFile, "r");
				opf = fopen(defOutputFile, "a");
				if (gflags & DBINITED)
					ISQL_init(newDataBase, newUserName, newPassword, ipf,
							  opf);
				fprintf(opf, "\n\r\n\r");
				display_page(hWnd);
			}
			break;

		case IDM_HELP:
			ISQL_win_err("not implemented yet");
			break;

		case IDM_APPEND:
			if (gflags & OVERWRITE)
				gflags &= ~OVERWRITE;
			else
				gflags |= OVERWRITE;
			break;

			// Send the proper frontend commands for these:

		case IDM_BLOB_TYPE:
			tmpDialogParam[0] = '\0';
			allblobsflag = FALSE;
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) blobDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(BLOB_TYPE), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret) {
				sprintf(buf, "SET BLOBDISPLAY %s", tmpDialogParam);
				ISQL_frontend_command(buf, ipf, opf, chf);
			}
			break;

		case IDM_STATISTICS:
			ISQL_frontend_command("SET STATS", ipf, opf, chf);
			break;

		case IDM_COUNT:
			ISQL_frontend_command("SET COUNT", ipf, opf, chf);
			break;

		case IDM_AUTOCOMMIT:
			ISQL_frontend_command("SET AUTOCOMMIT", ipf, opf, chf);
			break;

		case IDM_LIST:
			ISQL_frontend_command("SET LIST", ipf, opf, chf);
			break;

		case IDM_PLAN:
			ISQL_frontend_command("SET PLAN", ipf, opf, chf);
			break;

		case IDM_TRANSACTION:
			tmpDialogParam[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) transDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(TRANS_STRING), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret) {
				sprintf(buf, "SET TRANSACTION %s", tmpDialogParam);
				ISQL_frontend_command(buf, ipf, opf, chf);
			}
			break;

		case IDM_AUTODDL:
			ISQL_frontend_command("SET AUTO", ipf, opf, chf);
			break;

		case IDM_TERMINATOR:
			tmpDialogParam[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) termDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(TERMINATOR), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret) {
				sprintf(buf, "SET TERM %s", tmpDialogParam);
				ISQL_frontend_command(buf, ipf, opf, chf);
			}
			break;

		case IDM_CHAR_SET:
			tmpDialogParam[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) charSetDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(CHAR_SET), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret) {
				sprintf(buf, "SET NAMES %s", tmpDialogParam);
				ISQL_frontend_command(buf, ipf, opf, chf);
			}
			break;

			// Send the proper show commands for these:

		case IDM_SHOW_VERSION:
			test_overwrite();
			ISQL_frontend_command("SHOW VERSION", ipf, opf, chf);
			fprintf(opf, "\n\r\n\r");
			display_page(hWnd);
			break;

		case IDM_SHOW_SYSTEM:
			test_overwrite();
			ISQL_frontend_command("SHOW SYSTEM", ipf, opf, chf);
			fprintf(opf, "\n\r\n\r");
			display_page(hWnd);
			break;

		case IDM_SHOW_SETTINGS:
			test_overwrite();
			ISQL_frontend_command("SET", ipf, opf, chf);
			fprintf(opf, "\n\r\n\r");
			display_page(hWnd);
			break;

		case IDM_SHOW_DATABASE:
			test_overwrite();
			ISQL_frontend_command("SHOW DATABASE", ipf, opf, chf);
			fprintf(opf, "\n\r\n\r");
			display_page(hWnd);
			break;

		case IDM_SHOW_CHECK:
		case IDM_SHOW_TRIGGER:
		case IDM_SHOW_PROCEDURE:
		case IDM_SHOW_GENERATOR:
		case IDM_SHOW_GRANT:
		case IDM_SHOW_FUNCTION:
		case IDM_SHOW_FILTER:
		case IDM_SHOW_EXCEPTS:
		case IDM_SHOW_DOMAIN:
		case IDM_SHOW_INDEX:
		case IDM_SHOW_VIEW:
		case IDM_SHOW_TABLE:
			tmpDialogParam[0] = '\0';
			lpproc = (DLGPROC) MakeProcInstance((FARPROC) objectDlgProc, hInst);
			ret = DialogBox(hInst, MAKEINTRESOURCE(DB_OBJECT), hWnd, lpproc);
			FreeProcInstance((FARPROC) lpproc);
			if (ret)
			{
				switch (GET_WM_COMMAND_ID(wParam, lParam))
				{
				case IDM_SHOW_CHECK:
					sprintf(buf, "SHOW CHECK %s", tmpDialogParam);
					break;

				case IDM_SHOW_TRIGGER:
					sprintf(buf, "SHOW TRIGGER %s", tmpDialogParam);
					break;

				case IDM_SHOW_PROCEDURE:
					sprintf(buf, "SHOW PROCEDURE %s", tmpDialogParam);
					break;

				case IDM_SHOW_GENERATOR:
					sprintf(buf, "SHOW GENERATOR %s", tmpDialogParam);
					break;

				case IDM_SHOW_GRANT:
					sprintf(buf, "SHOW GRANT %s", tmpDialogParam);
					break;

				case IDM_SHOW_FUNCTION:
					sprintf(buf, "SHOW FUNCTION %s", tmpDialogParam);
					break;

				case IDM_SHOW_FILTER:
					sprintf(buf, "SHOW FILTER %s", tmpDialogParam);
					break;

				case IDM_SHOW_EXCEPTS:
					sprintf(buf, "SHOW EXCEPTION %s", tmpDialogParam);
					break;

				case IDM_SHOW_DOMAIN:
					sprintf(buf, "SHOW DOMAIN %s", tmpDialogParam);
					break;

				case IDM_SHOW_INDEX:
					sprintf(buf, "SHOW INDEX %s", tmpDialogParam);
					break;

				case IDM_SHOW_VIEW:
					if (allobjectsflag)
						strcpy(buf, "SHOW VIEWS");
					else
						sprintf(buf, "SHOW VIEW %s", tmpDialogParam);
					break;

				case IDM_SHOW_TABLE:
					if (allobjectsflag)
						strcpy(buf, "SHOW TABLES");
					else
						sprintf(buf, "SHOW TABLE %s", tmpDialogParam);
					break;

				}
				test_overwrite();
				ISQL_frontend_command(buf, ipf, opf, chf);
				fprintf(opf, "\n\r\n\r");
				display_page(hWnd);
			}
			break;

		default:
			break;
		}
		break;

	case WM_SIZE:
		// Save size of window client area.
		if (lParam)
		{
			yClient = HIWORD(lParam);
			xClient = LOWORD(lParam);
			yClient = (yClient / yCharnl + 1) * yCharnl;
			lParam = MAKELONG(xClient, yClient);

			// Go setup scroll ranges and file display area based upon
			// client area size.

			setup_scroll(hWnd);
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_VSCROLL:
		// React to the various vertical scroll related actions.

		switch (GET_WM_VSCROLL_CODE(wParam, lParam))
		{
		case SB_TOP:
			nVscrollInc = -nVscrollPos;
			break;
		case SB_BOTTOM:
			nVscrollInc = nVscrollMax - nVscrollPos;
			break;
		case SB_LINEUP:
			nVscrollInc = -1;
			break;
		case SB_LINEDOWN:
			nVscrollInc = 1;
			break;
		case SB_PAGEUP:
			nVscrollInc = -max(1, yClient / yChar);
			break;
		case SB_PAGEDOWN:
			nVscrollInc = max(1, yClient / yChar);
			break;
		case SB_THUMBPOSITION:
			nVscrollInc = GET_WM_VSCROLL_POS(wParam, lParam) - nVscrollPos;
			break;
		case SB_THUMBTRACK:
			nVscrollInc = GET_WM_VSCROLL_POS(wParam, lParam) - nVscrollPos;
			break;
		default:
			nVscrollInc = 0;
		}
		nVscrollInc = max(-nVscrollPos, min(nVscrollInc, nVscrollMax - nVscrollPos));
		if (nVscrollInc) {
			nVscrollPos += nVscrollInc;
			ScrollWindow(hWnd, 0, -yChar * nVscrollInc, NULL, NULL);
			SetScrollPos(hWnd, SB_VERT, nVscrollPos, TRUE);
			UpdateWindow(hWnd);
		}
		break;

	case WM_HSCROLL:
		// React to the various horizontal scroll related actions.

		switch (GET_WM_HSCROLL_CODE(wParam, lParam))
		{
		case SB_LINEUP:
			nHscrollInc = -1;
			break;
		case SB_LINEDOWN:
			nHscrollInc = 1;
			break;
		case SB_PAGEUP:
			nHscrollInc = -8;
			break;
		case SB_PAGEDOWN:
			nHscrollInc = 8;
			break;
		case SB_THUMBPOSITION:
			nHscrollInc = GET_WM_HSCROLL_POS(wParam, lParam) - nHscrollPos;
			break;
		case SB_THUMBTRACK:
			nHscrollInc = GET_WM_HSCROLL_POS(wParam, lParam) - nHscrollPos;
			break;
		default:
			nHscrollInc = 0;
		}
		nHscrollInc = max(-nHscrollPos, min(nHscrollInc, nHscrollMax - nHscrollPos));
		if (nHscrollInc) {
			nHscrollPos += nHscrollInc;
			ScrollWindow(hWnd, -xChar * nHscrollInc, 0, NULL, NULL);
			SetScrollPos(hWnd, SB_HORZ, nHscrollPos, TRUE);
			UpdateWindow(hWnd);
		}
		break;

	case WM_KEYDOWN:
		// Translate various keydown messages to appropriate horizontal
		// and vertical scroll actions.

		for (SSHORT i = 0; i < FB_NELEM(key2scroll); i++)
		{
			if (wParam == key2scroll[i].wVirtkey) {
				SendMessage(hWnd, key2scroll[i].iMessage, key2scroll[i].wRequest, 0L);
				break;
			}
		}
		break;

	case WM_PAINT:
		// Go paint the client area of the window with the appropriate
		// part of the selected file.

		paint_isql(hWnd);
		break;

	case WM_DESTROY:
		// This is the end if we were closed by a DestroyWindow call.
		close_isql();			// take any necessary wrapup action.
		PostQuitMessage(0);		// this is the end...
		break;

	case WM_QUERYENDSESSION:
		// If we return TRUE we are saying it's ok with us to end the
		// windows session.
		close_isql();			// take any necessary wrapup action.
		return (long) TRUE;		// we agree to end session.

	case WM_CLOSE:
		// Tell windows to destroy our window.
		DestroyWindow(hWnd);
		break;

	default:
		// Let windows handle all messages we choose to ignore.
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0L;
}


void ISQL_win_err(const char* string)
{
/***************************************************************
 *
 *      I S Q L _ w i n _ e r r
 *
 ***************************************************************
 *
 *  Functional description:
 *      Output a Windows ISQL error in a message box.
 *
 ***************************************************************/

	if (Merge_stderr)
		fprintf(Out, "%s\n", string);
	else
		MessageBox(NULL, string, SetUpData.errorString, MB_ICONEXCLAMATION | MB_OK);
}


static void close_isql()
{
/********************************************************************
 *
 *      c l o s e _ i s q l
 *
 ********************************************************************
 *
 *  Functional description:
 *      Shut down the database prior to exit.
 *
 ********************************************************************/

	ISQL_exit_db();
	if (gflags & COMHIST)
		fclose(chf);
	unlink(defHistFile);
	if (gflags & DEFINPUT)
		fclose(ipf);
	unlink(defInputFile);
	if (gflags & DEFOUTPUT)
		fclose(opf);
	unlink(defOutputFile);
	if (gflags & SESSFILE)
		fclose(sss);
	unlink(defSessionFile);
	DeleteObject(hnewsfont);
}


static int cmdline_isql( HINSTANCE hInstance, LPSTR pCmdLine)
{
/********************************************************************
 *
 *      c m d l i n e _ i s q l
 *
 ********************************************************************
 *
 * Functional Description:
 *      Take the command line and turn it into an argc/argv to feed
 *      to ISQL.
 *
 * paramaters:
 *	       hInstance  - Instance used for error messsages
 *             pCmdLine   - A long pointer to the command line that
 *                          started this application.
 *
 * returns:
 *              return from ISQL
 *
 ********************************************************************/
	FILE *inputfile;			// input file
	FILE *outputfile;		// output file
	SCHAR inputfilename[MAXPATHLEN];	// input file name
	SCHAR outputfilename[MAXPATHLEN];	// output file name
	SCHAR arg[MAXPATHLEN];		// current argument
	const SCHAR *cp;			// command line cursor
	SCHAR *ap;					// current argument cursor

// create default input and output files

	if (!open_temp_file(hInstance, &inputfile, inputfilename, IDS_TEMP_IN_FILE))
		return 0;
	if (!open_temp_file(hInstance, &outputfile, outputfilename, IDS_TEMP_OUT_FILE))
		return 0;

// create failsafe input file

	fprintf(inputfile, "QUIT;\n");
	fclose(inputfile);
	fclose(outputfile);

// create an argument vector, including the default files and command line args

	ISQL_cursor = ISQL_args;
	ISQL_argc = 0;
	pusharg("isql");
	pusharg("-input");
	pusharg(inputfilename);
	pusharg("-output");
	pusharg(outputfilename);
	ap = arg;
	for (cp = (SCHAR *) pCmdLine; *cp; cp++)
	{
		*ap = *cp++;
		if (*ap == ' ') {
			*ap = '\0';
			pusharg(arg);
			ap = arg;
		}
		else
			ap++;
		if (ISQL_argc == 20)
			break;
	}
	ISQL_main(ISQL_argc, ISQL_argv);
	unlink(inputfilename);
	unlink(outputfilename);

	return 0;
}


static void display_page( HWND hWnd)
{
/***************************************************************
 *
 *      d i s p l a y _ p a g e
 *
 ***************************************************************
 *
 *  Functional description:
 *      Throw the result up on the window.
 *
 ***************************************************************/
	FILE *fh;

// Determine file size and some display paramaters.
	nVscrollPos = numlines;
	numlines = 0;
	maxwidth = 0;
	fflush(opf);
	fclose(opf);
	fh = fopen(defOutputFile, "r+b");
	if (fh)
	{
		while (fgets(tmpDialogParam, sizeof(tmpDialogParam), fh)) {
			numlines++;
			if (strlen(tmpDialogParam) > maxwidth)
				maxwidth = strlen(tmpDialogParam);
		}
		fclose(fh);
	}
	opf = fopen(defOutputFile, "a");

// Go setup scroll ranges for this file.

	setup_scroll(hWnd);

// Show first part of file.

	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
}


static SSHORT init_isql(HINSTANCE hInstance, HINSTANCE hPrevInstance, int cmdShow)
{
/********************************************************************
 *
 *      i n i t _ i s q l
 *
 ********************************************************************
 *
 *  Functional description:
 *      Startup code for Windows.   This builds the window and also
 *      presents the user with a dialog box to specify the database
 *      connection parameters.
 *
 * paramaters:
 *             hInstance     - The instance of this instance of this
 *                             application.
 *             hPrevInstance - The instance of the previous instance
 *                             of this application. This will be 0
 *                             if this is the first instance.
 *             cmdShow       - Indicates how the window is to be shown
 *                             initially. ie. SW_SHOWNORMAL, SW_HIDE,
 *                             SW_MIMIMIZE.
 *
 *********************************************************************/
#pragma argsused
	DLGPROC dlgProc;
	int iReturn;

// perform instance dependant Windows initialization

	if (!hPrevInstance)
		init_isql_first(hInstance);
#if !defined(__WIN32__)
	else
		init_isql_added(hPrevInstance);
#endif

// perform common instance Windows initialization

	init_isql_every(hInstance, cmdShow);

// open all the files

	if (!open_temp_file(hInstance, &ipf, defInputFile, IDS_DEF_IN_FILE))
		return FALSE;
	fprintf(ipf, "QUIT;\n");
	fclose(ipf);
	ipf = fopen(defInputFile, "r");
	gflags |= DEFINPUT;
	if (!open_temp_file(hInstance, &opf, defOutputFile, IDS_DEF_OUT_FILE))
		return FALSE;
	gflags |= DEFOUTPUT;
	if (!open_temp_file(hInstance, &chf, defHistFile, IDS_DEF_HIST_FILE))
		return FALSE;
	gflags |= COMHIST;
	if (!open_temp_file(hInstance, &sss, defSessionFile, IDS_DEF_SESS_FILE))
		return FALSE;
	gflags |= SESSFILE;
	newDataBase[0] = '\0';
	newUserName[0] = '\0';
	newPassword[0] = '\0';
	dlgProc = (DLGPROC) MakeProcInstance((FARPROC) dbNameDlgProc, hInst);
	iReturn = DialogBox(hInst, MAKEINTRESOURCE(CONNECT_DB), hWndMain, dlgProc);
	FreeProcInstance((FARPROC) dlgProc);
	if (iReturn)
		if (ISQL_init(newDataBase, newUserName, newPassword, ipf, opf))
			gflags |= DBINITED;
	return TRUE;
}


#if !defined(__WIN32__)
static void init_isql_added( HINSTANCE hPrevInstance)
{
/********************************************************************
 *
 *      i n i t _ i s q l _ a d d e d
 *
 ********************************************************************
 *
 *  Functional description:
 *      This gets called for each added instance of the application.
 *
 * paramaters:
 *             hPrevInstance - The instance of the previous instance
 *                             of this application.
 *
 *********************************************************************/

// get the results of the initialization of first instance

	GetInstanceData(hPrevInstance, (BYTE *) & SetUpData, sizeof(SETUPDATA));
}
#endif


static void init_isql_every( HINSTANCE hInstance, int cmdShow)
{
/********************************************************************
 *
 *      i n i t _ i s q l _ e v e r y
 *
 ********************************************************************
 *
 *  Functional description:
 *      Perform initialization done for all instances.
 *
 * paramaters:
 *             hInstance     - The instance of this instance of this
 *                             application.
 *             cmdShow       - Indicates how the window is to be shown
 *                             initially. ie. SW_SHOWNORMAL, SW_HIDE,
 *                             SW_MIMIMIZE.
 *
 ********************************************************************/
	TEXTMETRIC tm;
	HDC hDC;

	hInst = hInstance;			// save for use by window procs

// Create applications main window.

	hWndMain = CreateWindow(SetUpData.appName,	// window class name
							SetUpData.appName,	// window title
							WS_OVERLAPPEDWINDOW |	// type of window
							WS_HSCROLL | WS_VSCROLL, CW_USEDEFAULT,	// x  window location
							CW_USEDEFAULT,	// y
							CW_USEDEFAULT,	// cx and size
							CW_USEDEFAULT,	// cy
							NULL,	// no parent for this window
							NULL,	// use the class menu
							hInstance,	// who created this window
							NULL	// no parms to pass on
		);

// Get the display context.

	hDC = GetDC(hWndMain);

// Build fixed screen font.

	cursfont.lfHeight = 14;
	cursfont.lfWidth = 9;
	cursfont.lfEscapement = 0;
	cursfont.lfOrientation = 0;
	cursfont.lfWeight = FW_NORMAL;
	cursfont.lfItalic = FALSE;
	cursfont.lfUnderline = FALSE;
	cursfont.lfStrikeOut = FALSE;
	cursfont.lfCharSet = ANSI_CHARSET;
	cursfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	cursfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	cursfont.lfQuality = DEFAULT_QUALITY;
	cursfont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	strcpy((char *) cursfont.lfFaceName, "System");

	hnewsfont = CreateFontIndirect((LPLOGFONT) & cursfont);

// Install the font in the current display context.

	holdsfont = SelectObject(hDC, hnewsfont);

// get text metrics for paint

	GetTextMetrics(hDC, &tm);
	xChar = tm.tmAveCharWidth;
	yChar = tm.tmHeight + tm.tmExternalLeading;
	yCharnl = tm.tmHeight;
	numlines = 0;

// Release the display context.

	ReleaseDC(hWndMain, hDC);

// Update display of main window.

	ShowWindow(hWndMain, cmdShow);
	UpdateWindow(hWndMain);
}


static void init_isql_first( HINSTANCE hInstance)
{
/********************************************************************
 *
 *      i n i t _ i s q l _ f i r s t
 *
 ********************************************************************
 *
 *  Functions description:
 *      Initialization for first instance of application.
 *
 * paramaters:
 *             hInstance     - The instance of this instance of this
 *                             application.
 *
 ********************************************************************/
	WNDCLASS wcISQLClass;

// Get string from resource with application name.

	LoadString(hInstance, IDS_NAME, (LPSTR) SetUpData.appName, 20);
	LoadString(hInstance, IDS_MENUNAME, (LPSTR) SetUpData.menuName, 20);
	LoadString(hInstance, IDS_ICONNAME, (LPSTR) SetUpData.iconName, 20);
	LoadString(hInstance, IDS_ERROR, (LPSTR) SetUpData.errorString, 20);

// Define the window class for this application.

	wcISQLClass.lpszClassName = SetUpData.appName;
	wcISQLClass.hInstance = hInstance;
	wcISQLClass.lpfnWndProc = isqlWndProc;
	wcISQLClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(ISQLCURSOR));
	wcISQLClass.hIcon = LoadIcon(hInstance, SetUpData.iconName);
	wcISQLClass.lpszMenuName = (LPSTR) SetUpData.menuName;
	wcISQLClass.hbrBackground = GetStockObject(WHITE_BRUSH);
	wcISQLClass.style = CS_HREDRAW | CS_VREDRAW;
	wcISQLClass.cbClsExtra = 0;
	wcISQLClass.cbWndExtra = 0;

// Register the class

	RegisterClass(&wcISQLClass);
}


static SSHORT open_temp_file(HINSTANCE hInstance,
							 FILE** file,
							 SCHAR* fileName, SSHORT errStrNum)
{
/********************************************************************
 *
 *      o p e n _ t e m p _ f i l e
 *
 ********************************************************************
 *
 *  Functions description:
 *	Create a temp file, get its name, and print the proper
 *	error on failure.
 *
 ********************************************************************/
	SCHAR errorString[100];
	SCHAR message[100];

	*file = (FILE *) gds__temp_file(TRUE, "isql_", fileName);
	if (*file == (FILE *) - 1) {
		LoadString(hInstance, errStrNum, errorString, 100);
		sprintf(message, errorString, fileName);
		ISQL_win_err(message);
		return 0;
	}

	return 1;
}


static int windows_isql(HINSTANCE hInstance, HINSTANCE hPrevInstance, int cmdShow)
{
/********************************************************************
 *
 *      w i n d o w s _ i s q l
 *
 ********************************************************************
 *
 *  Functional description:
 *      Process ISQL for Windows (no command line was passed).
 *
 * paramaters:
 *             hInstance     - The instance of this instance of this
 *                             application.
 *             hPrevInstance - The instance of the previous instance
 *                             of this application. This will be 0
 *                             if this is the first instance.
 *             cmdShow       - Indicates how the window is to be shown
 *                             initially. ie. SW_SHOWNORMAL, SW_HIDE,
 *                             SW_MIMIMIZE.
 *
 * returns:
 *             wParam from last message.
 *
 ********************************************************************/ \
		MSG msg;

// Go init this application.

	if (!init_isql(hInstance, hPrevInstance, cmdShow))
		return 0;

// Get and dispatch messages for this applicaton.

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


static void paint_isql( HWND hWnd)
{
/********************************************************************
 *
 *      p a i n t _ i s q l
 *
 ********************************************************************
 *  Functional description:
 *      If a file has been selected, as indicated by numlines being
 *      greater than 0, this module will read and display a portion
 *      of the file as determined by the current size and scroll
 *      position of the window.
 *
 * paramaters:
 *             hWnd          - The callers window handle
 *
 ********************************************************************/
	PAINTSTRUCT ps;
	HDC hDC;
	SSHORT e;
	SCHAR buf[1024];
	FILE *hfile;

	BeginPaint(hWnd, (LPPAINTSTRUCT) & ps);
	hDC = ps.hdc;

// Establish fixed font in display context.

	SelectObject(hDC, hnewsfont);

	if (numlines)
	{
		// Open the file to display
		// (files should not stay open over multiple windows messages)

		hfile = fopen(defOutputFile, "r");
		if (hfile)
		{
			// Skip lines outside window limits

			for (SSHORT i = 0; i < nVscrollPos; i++)
				fgets(buf, sizeof(buf), hfile);

			// Read visible lines

			for (SSHORT i = 0; i < nPageMaxLines; i++) {
				if (!fgets(buf, sizeof(buf), hfile))
					break;

				// figure out shortest text to put

				for (e = strlen(buf); e >= 0; e--)
					if (buf[e] > ' ' && buf[e] <= '~')
						break;
				if (e == -1)
					buf[++e] = ' ';
				buf[++e] = '\0';
				TextOut(hDC, xChar * (-nHscrollPos + 0), yChar * i, buf, e);
			}
			fclose(hfile);
		}
	}

	EndPaint(hWnd, (LPPAINTSTRUCT) & ps);
}


static void pusharg(const char* argument)
{
/********************************************************************
 *
 *      p u s h a r g
 *
 ********************************************************************
 *
 *  Functional description:
 *      Used to push arguments onto an argv to be sent to ISQL.
 *
 ********************************************************************/

	ISQL_argv[ISQL_argc++] = ISQL_cursor;
	strcpy(ISQL_cursor, argument);
	ISQL_cursor += strlen(argument);
	*ISQL_cursor++ = '\0';
}


static void setup_scroll( HWND hWnd)
{
/********************************************************************
 *
 *      s e t u p _ s c r o l l
 *
 ********************************************************************
 *
 *  Functional description:
 *      Set up the vertical and horizontal scroll ranges and positions
 *      of the applicatons main window based on:
 *
 *         numlines - The maximum number of lines to display.
 *         maxwidth - The width of each line to display.
 *
 *      The resulting variables, nVscrollPos and nPageMaxLines, are used
 *      by the function paint_isql to determine what to display in the window.
 *
 * paramaters:
 *             hWnd          - The callers window handle
 *
 *********************************************************************/

// numlines established during open

	nVscrollMax = max(0, numlines - yClient / yChar);
	nVscrollPos = min(nVscrollPos, nVscrollMax);

	nHscrollMax = max(0, maxwidth - xClient / xChar);
	nHscrollPos = min(nHscrollPos, nHscrollMax);

	SetScrollRange(hWnd, SB_VERT, 0, nVscrollMax, FALSE);
	SetScrollPos(hWnd, SB_VERT, nVscrollPos, TRUE);

	SetScrollRange(hWnd, SB_HORZ, 0, nHscrollMax, FALSE);
	SetScrollPos(hWnd, SB_HORZ, nHscrollPos, TRUE);

	nPageMaxLines = min(numlines, yClient / yChar);
}


static void test_overwrite()
{
/***************************************************************
 *
 *      t e s t _ o v e r w r i t e
 *
 ***************************************************************
 *
 *  Functional description:
 *      If the overwrite flag is set, clear the file and reset
 *      the scroll positions.
 *
 ***************************************************************/

	if (gflags & OVERWRITE)
	{
		fclose(opf);
		opf = fopen(defOutputFile, "w");
		nVscrollPos = 0;
		nHscrollPos = 0;
	}
}


static void xfer_file(const char* inFileName,
					  const char* outFileName, bool appendFlag)
{
/***************************************************************
 *
 *      x f e r _ f i l e
 *
 ***************************************************************
 *
 *  Functional description:
 *      Copy current file to a save file.
 *
 ***************************************************************/
	SCHAR xferbuff[1024];

	FILE* xferin = fopen(inFileName, "r");
	if (xferin)
	{
        FILE* xferout;
		if (appendFlag)
			xferout = fopen(outFileName, "a");
		else
			xferout = fopen(outFileName, "w");
		if (xferout) {
			while (fgets(xferbuff, sizeof(xferbuff), xferin)) {
				fprintf(xferout, "%s", xferbuff);
			}
			fclose(xferout);
		}
		fclose(xferin);
	}
}


BOOL CALLBACK _export aboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      a b o u t D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Put up a little dislog box saying what this is.
 *
 * paramaters:
 *             hDlg          - The window handle for this message
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 ********************************************************************/
#pragma argsused

	if (message == WM_INITDIALOG)
		return TRUE;

	if (message == WM_COMMAND)
	{
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDOK:
			EndDialog(hDlg, TRUE);
			return TRUE;
		default:
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CALLBACK _export blobDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      b l o b D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get blob display type, or "ALL" by managing the BLOB_TYPE
 *      dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_BLOB_TYPE, EM_LIMITTEXT, 10, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_ALL_BLOBS:
			allblobsflag = TRUE;
			tmpDialogParam[0] = '\0';
			EndDialog(hDlg, TRUE);
			break;

		case IDD_BLOB_TYPE:
			allblobsflag = FALSE;
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_BLOB_TYPE, tmpDialogParam, 10);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			strcpy(tmpDialogParam, "0");
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export charSetDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      c h a r S e t D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get characetr set name by managing the NAMES dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_CHAR_SET, EM_LIMITTEXT, 100, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_CHAR_SET:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_CHAR_SET, tmpDialogParam, 100);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			tmpDialogParam[0] = '\0';
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export createDbDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      c r e a t e D b D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get database name, user name and password for a newly created
 *	database by managing the CREATE_DB dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_DB_DBNAME, EM_LIMITTEXT, 256, 0L);
		SendDlgItemMessage(hDlg, IDD_DB_USERNAME, EM_LIMITTEXT, MAX_SQL_IDENTIFIER_SIZE, 0L);
		SendDlgItemMessage(hDlg, IDD_DB_PASSWORD, EM_LIMITTEXT, 16, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_CREATE_DBNAME:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDD_CREATE_USERNAME:
		case IDD_CREATE_PASSWORD:
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_DB_DBNAME, newDataBase, 256);
			GetDlgItemText(hDlg, IDD_DB_USERNAME, newUserName, MAX_SQL_IDENTIFIER_SIZE);
			GetDlgItemText(hDlg, IDD_DB_PASSWORD, newPassword, 16);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			// Terminate this dialog box.
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export dbNameDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      d b N a m e D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get database name, user name and password by managing the
 *      DB_NAME dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_DB_DBNAME, EM_LIMITTEXT, 256, 0L);
		SendDlgItemMessage(hDlg, IDD_DB_USERNAME, EM_LIMITTEXT, MAX_SQL_IDENTIFIER_SIZE, 0L);
		SendDlgItemMessage(hDlg, IDD_DB_PASSWORD, EM_LIMITTEXT, 16, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_DB_DBNAME:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDD_DB_USERNAME:
		case IDD_DB_PASSWORD:
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_DB_DBNAME, newDataBase, 256);
			GetDlgItemText(hDlg, IDD_DB_USERNAME, newUserName, MAX_SQL_IDENTIFIER_SIZE);
			GetDlgItemText(hDlg, IDD_DB_PASSWORD, newPassword, 16);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			// Terminate this dialog box.
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export dropDbDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      d r o p D b D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Drop the database by  managing the DROP_DB dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDOK:
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			// Terminate this dialog box.
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export execDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      e x e c D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get a SQL command by managing the EXEC dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/
	FILE *fh;
	SCHAR buf[256];
	SSHORT i;

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_SQL_COMMAND, EM_LIMITTEXT, 1024, 0L);
		fflush(sss);
		fclose(sss);
		fh = fopen(defSessionFile, "r");
		if (fh)
		{
			while (fgets(buf, sizeof(buf), fh)) {
				for (i = strlen(buf); i; i--)
					if (buf[i] >= ' ' && buf[i] <= '~')
						break;
				if (!i)
					break;
				buf[i + 1] = '\0';
				SendMessage(GetDlgItem(hDlg, IDD_SQL_HISTORY), LB_ADDSTRING, 0, (SLONG) buf);
			}
			fclose(fh);
		}
		sss = fopen(defSessionFile, "a");
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_SQL_COMMAND:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE) {
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			}
			break;

		case IDD_SQL_HISTORY:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == LBN_DBLCLK) {
				i = (SSHORT) SendMessage(GetDlgItem(hDlg, IDD_SQL_HISTORY), LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hDlg, IDD_SQL_HISTORY), LB_GETTEXT, i, (SLONG) buf);
				SetDlgItemText(hDlg, IDD_SQL_COMMAND, buf);
			}
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_SQL_COMMAND, tmpDialogParam, 1024);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export extractDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      e x t r a c t D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get extract database name plus optional target database and
 *      output file by managing the EXTRACT_DB dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_EXTRACT_DBNAME, EM_LIMITTEXT, 256, 0L);
		SendDlgItemMessage(hDlg, IDD_EXTRACT_OUTPUT, EM_LIMITTEXT, 256, 0L);
		SendDlgItemMessage(hDlg, IDD_EXTRACT_TARGET, EM_LIMITTEXT, 256, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_EXTRACT_DBNAME:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDD_EXTRACT_TARGET:
		case IDD_EXTRACT_OUTPUT:
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_EXTRACT_DBNAME, extractDbName, 256);
			GetDlgItemText(hDlg, IDD_EXTRACT_TARGET, extractTarget, 256);
			GetDlgItemText(hDlg, IDD_EXTRACT_OUTPUT, extractOutput, 256);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			// Terminate this dialog box.
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export objectDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      o b j e c t D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get database object name or "ALL" by managing the
 *      DB_OBJECT dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_OBJECT, EM_LIMITTEXT, 256, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_ALL_OBJECTS:
			allobjectsflag = TRUE;
			tmpDialogParam[0] = '\0';
			EndDialog(hDlg, TRUE);
			break;

		case IDD_OBJECT:
			allobjectsflag = FALSE;
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_OBJECT, tmpDialogParam, 256);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			tmpDialogParam[0] = '\0';
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export outputDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      o u t p u t D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get name of file to dump current window to, using
 *      SAVE_OUTPUT dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_SAVE_OUTPUT_FILE, EM_LIMITTEXT, 256, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_SAVE_OUTPUT_FILE:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_SAVE_OUTPUT_FILE, tmpDialogParam, 256);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			tmpDialogParam[0] = '\0';
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export scriptDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      s c r i p t D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get script name and optional output file name by managing the
 *      SCRIPT dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_SCRIPT_INPUT, EM_LIMITTEXT, 256, 0L);
		SendDlgItemMessage(hDlg, IDD_SCRIPT_OUTPUT, EM_LIMITTEXT, 256, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_SCRIPT_INPUT:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDD_SCRIPT_OUTPUT:
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_SCRIPT_INPUT, scriptName, 256);
			GetDlgItemText(hDlg, IDD_SCRIPT_OUTPUT, scriptOutput, 256);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			// Terminate this dialog box.
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export sessionDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      s e s s i o n D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get name of file to dump current session's commands to, using
 *      SAVE_SESSION dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_SAVE_SESSION_FILE, EM_LIMITTEXT, 256, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_SAVE_SESSION_FILE:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_SAVE_SESSION_FILE, tmpDialogParam, 256);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			tmpDialogParam[0] = '\0';
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export termDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      t e r m D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get SQL statement terminator by managing the TERMINATOR dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_TERMINATOR, EM_LIMITTEXT, 10, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_TERMINATOR:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_TERMINATOR, tmpDialogParam, 10);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			strcpy(tmpDialogParam, ";");
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


BOOL CALLBACK _export transDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
/********************************************************************
 *
 *      t r a n s D l g P r o c
 *
 ********************************************************************
 *
 *  Functional description:
 *      Get transaction definition string by managing the TRANS dialog box.
 *
 * paramaters:
 *             hDlg          - The window handle of this dialog box
 *             message       - The message number
 *             wParam        - The WPARAM parameter for this message
 *             lParam        - The LPARAM parameter for this message
 *
 * returns:
 *             depends on message.
 *
/********************************************************************/

	switch (iMessage)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDD_TRANS_STRING, EM_LIMITTEXT, 256, 0L);
		return (TRUE);

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDD_TRANS_STRING:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
				EnableWindow(GetDlgItem(hDlg, IDOK),
							 (BOOL) SendMessage((HWND) lParam, WM_GETTEXTLENGTH, 0, 0L));
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDD_TRANS_STRING, tmpDialogParam, 256);
			EndDialog(hDlg, TRUE);
			break;

		case IDCANCEL:
			tmpDialogParam[0] = '\0';
			EndDialog(hDlg, FALSE);
			break;

		case IDD_HELP_BUTTON:
			ISQL_win_err("Not implemented yet");
			break;

		default:
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

