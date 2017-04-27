/******************************************************************************

	    Copyright (c) 1995-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of
	Macrovision Corporation and is protected by law.  It may
	not be copied or distributed in any form or medium, disclosed
	to third parties, reverse engineered or used in any manner not
	provided for in said License Agreement except with the prior
	written authorization from Macrovision Corporation.

 *****************************************************************************/
/*

 *
 *	Description: 	 Trivial API example: CHECKOUT, CHECKIN and PERROR
 *
 *	D. Birns
 *	Jun3 23, 1999
 *
 *	Last changed:  6/23/99
 *
 */

#include <windows.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lmwin.h"
#include "lmpolicy.h"

#define LICENSE_PATH  "license.dat;."

static char  szAppName[] = "LMWIN" ;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{

  HWND hwnd ;
  MSG msg ;
  WNDCLASSEX wndclass ;
  HWND hButton, hcheckinbtn ;

	wndclass.cbSize        = sizeof (wndclass) ;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = DLGWINDOWEXTRA ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (hInstance, szAppName) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW ) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;
	wndclass.hIconSm       = LoadIcon (hInstance, szAppName) ;

	RegisterClassEx (&wndclass) ;

	hwnd = CreateDialog (hInstance, szAppName, 0, NULL) ;
        hButton = GetDlgItem( hwnd, IDC_FEATURE );
        SetWindowText(hButton, "f1");
        hcheckinbtn = GetDlgItem( hwnd, IDC_CHECKIN );
	EnableWindow(hcheckinbtn, FALSE);
	ShowWindow (hwnd, iCmdShow) ;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}



LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
  HWND hButton , hcheckoutbtn, hcheckinbtn;
  char FeatureName[32];
  int rc;
  HCURSOR busy_cursor, norm_cursor;

	switch (iMsg)
	{

	case WM_COMMAND :
		switch( wParam )
		{

		case IDC_CHECKOUT:

			hButton = GetDlgItem( hwnd, IDC_FEATURE );
			GetWindowText(hButton, FeatureName, 31);
			if ( !FeatureName[0] )
				strcpy(FeatureName,"f1");

			norm_cursor = GetCursor();
            busy_cursor = LoadCursor(NULL,IDC_WAIT);
			SetCursor (busy_cursor);
            ShowCursor(TRUE);
			rc = CHECKOUT( LM_RESTRICTIVE, FeatureName, "1.0", LICENSE_PATH );
			SetCursor (norm_cursor);
            ShowCursor(TRUE);
			if (rc)
			{
				PERROR("Checkout Failed");
				return 0;
			}
			hButton = GetDlgItem( hwnd, IDC_STATUS );
			SetWindowText(hButton,"Checkout Succeeded ");

			hcheckoutbtn = GetDlgItem( hwnd, IDC_CHECKOUT );
			hcheckinbtn = GetDlgItem( hwnd, IDC_CHECKIN );
			EnableWindow(hcheckoutbtn, FALSE);
			EnableWindow(hcheckinbtn, TRUE);

			return 0;

		case IDC_CHECKIN:

			CHECKIN();
			hcheckoutbtn = GetDlgItem( hwnd, IDC_CHECKOUT );
			hcheckinbtn = GetDlgItem( hwnd, IDC_CHECKIN );

			EnableWindow(hcheckoutbtn, TRUE);
			EnableWindow(hcheckinbtn, FALSE);

			hButton = GetDlgItem( hwnd, IDC_STATUS );
			SetWindowText(hButton,"Checking in license");

			return 0;



		}
		return 0 ;

	case WM_DESTROY :
		PostQuitMessage (0) ;
		return 0 ;
	}

	return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}



