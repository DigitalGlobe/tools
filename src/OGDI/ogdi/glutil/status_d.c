//================================================================================
//
// status_d.c     Francois Levert LAS inc.
// Create a Dialog with a cancel button. This dialog will be used to send status
// messages to the user. Two strings messages are provided by the status dialog.
//================================================================================

#include <windows.h>
#include "status_d.h"
#include "glutil.h"

/* Prototype */
void ThreadProc (LPVOID arg);
int CreateStatus_D_Thread(HANDLE outHandle);
void CloseStatus_D_Thread();

/*** global variables ***/
static BOOL bCancelChecked = TRUE;  /* Is Cancel Button pressed */
LPTSTR  lpszText1;            /* String message 1 */
LPTSTR  lpszText2;            /* String message 2 */
BOOL bDialog_created = FALSE; /* Is The status Dialog created */
HWND global_hDlg = NULL;      /* Handle of the main dialog */
HANDLE myThread;	      /* Thread created for the display event of the status dislog */
int bStatus;		      /* Status of the creation of the Status Dialog */

BOOL CALLBACK status_dDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
  
  switch(msg)
  {
     case WM_COMMAND:
          Handle_WM_COMMAND(hDlg, wParam, lParam);
          return TRUE;
     case WM_INITDIALOG:
          Handle_WM_INITDIALOG(hDlg);
          return TRUE;
 /*    case WM_MY_PAINT:
          Handle_WM_PAINT(hDlg);
          return TRUE;	*/
     case WM_CLOSE:
/*          SetCursor(LoadCursor(NULL, IDC_ARROW)); */
          EndDialog(hDlg, 0);
          CloseStatus_D_Thread();
  }
  return FALSE;
}

void Handle_WM_COMMAND(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
  if (HIWORD(wParam) != BN_CLICKED)
     return;

  switch(LOWORD(wParam) )
  {
     case ID_CANCEL:
	   bCancelChecked = TRUE;
           SendMessage(hDlg, WM_CLOSE, 0, 0);
           break;
  }
  return;
}

void Handle_WM_INITDIALOG(HWND hDlg)
{
  RECT  rect;
  int cx,cy;

  GetWindowRect(hDlg, &rect);
  cx = rect.right - rect.left;
  cy = rect.top - rect.bottom;

  lpszText1= "Status 1";
  lpszText2= "Status 2";

  
  bDialog_created = TRUE;
  bCancelChecked = FALSE;
  SetWindowPos(hDlg, HWND_TOP, 5 ,5, cx, cy, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
  SetFocus((HWND) ID_CANCEL);
 
/*  SetCursor(LoadCursor(NULL, IDC_APPSTARTING)); */
}

void Handle_WM_PAINT(HWND hDlg)
{
    SetDlgItemText(hDlg, ID_TEXT1, lpszText1);
    SetDlgItemText(hDlg, ID_TEXT2, lpszText2);
    SendMessage(hDlg, WM_PAINT, 0, 0);
}

//================================================================================
// Tell if the cancel has been pressed
//================================================================================
int bIs_status_d_Cancel_checked()
{
   return (int) bCancelChecked;
}

//================================================================================
// Update the status dialog messages
//================================================================================
int bUpdate_status_d_text(char *text1,char *text2)
{
  if(bDialog_created)
  {
     lpszText1 = (LPTSTR) text1;
     lpszText2 = (LPTSTR) text2;
     SetFocus((HWND) global_hDlg);
     if (global_hDlg != NULL) Handle_WM_PAINT(global_hDlg);
     return 1;
  }
  return 0;
}

//================================================================================
// Create a status message dialog
//================================================================================
int bCreate_status_d_text(char *text1,char *text2)
{
   int bResult;
   lpszText1 = (LPTSTR) text1;
   lpszText2 = (LPTSTR) text2;
   
   bResult = CreateStatus_D_Thread(NULL);
   
   if (bResult == 0) return 0; /* Not able to create a thread == not able to create a dialog */

   return bStatus;		       /* Return the Create Dialog Status (see ThreadProc) */
}

//================================================================================
// Close the status message dialog
//================================================================================
void close_status_d()
{
   if (global_hDlg != NULL) SendMessage(global_hDlg, WM_CLOSE, 0, 0);
}

//================================================================================
// Create a thread for the status Dialog
//================================================================================
int CreateStatus_D_Thread(HANDLE outHandle)
{
    DWORD threadId;
    CHAR Buf[80];
	HANDLE sem;

    sem = CreateSemaphore(NULL, 0, 1, NULL);
    myThread = CreateThread(NULL, 0, 
			   (LPTHREAD_START_ROUTINE)ThreadProc, 
                           (LPVOID)sem, 0, 
			   (LPDWORD)&threadId);
    if (!myThread)
    {
         wsprintf(Buf, "Error in creating the Status Dialog thread: %d",
                      GetLastError());
         MessageBox (GetFocus(), Buf, "WM_CREATE", MB_OK);
         return 0;
    }
	WaitForSingleObject(sem, INFINITE);

    return 1;

}

//================================================================================
// Create a thread for the status Dialog
//================================================================================
void CloseStatus_D_Thread()
{
    TerminateThread(myThread, 0);
}

//================================================================================
// A thread procedure which only send messages to be taken
// by the Satus Dialog Procedure.	
//================================================================================

VOID ThreadProc ( LPVOID arg)
{
  MSG msg;
  HANDLE sem = (HANDLE) arg;
  
  
  global_hDlg = CreateDialog( NULL, MAKEINTRESOURCE(STATUS_D), GetFocus(), (DLGPROC)status_dDlgProc);

  if ( global_hDlg == (HWND) NULL) bStatus = 0;
  else bStatus = 1; 
  ReleaseSemaphore(sem, 1, NULL); /* this will resumed the main thread permetting 
					bCreate_status_d_text() to return bStatus */

  do {                                        // do forever ...
  	GetMessage(&msg, NULL, 0, 0);
 	TranslateMessage(&msg);
	DispatchMessage(&msg);  
	SendMessage(global_hDlg, msg.message, 0, 0); 
  /*      PostThreadMessage((DWORD) arg, msg.message, 0, 0); */

  }while(1);
}

