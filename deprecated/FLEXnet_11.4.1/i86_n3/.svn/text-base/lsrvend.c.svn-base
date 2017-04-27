/******************************************************************************
Copyright (c) 1988-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
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
*	Function:  None
*
*	Description: Vendor customization data for server.
*
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lm_code.h"
#include "lmclient.h"
#include "lm_attr.h"
/*
*	Prototypes for persistent borrow/linger
*/
static void sBorrowInit(char **	ppBuffer, int *	piSize);
static void sBorrowOut(char * szBorrowData,	int	iSize);
static void sBorrowIn(char * szBorrowData, int iSize);
extern LM_HANDLE* lm_job;
/* Vendor encryption routine */
char *(*ls_user_crypt)() = 0;
/* Vendor initialization routines */
/* Initalize any vendor specific attributes by calling lc_set_attr_vendor() 
* from this routine or lc_set_attr(). Internally this callback is called after the 
* vendor daemon is initalized but before any license files are read. */
void (*ls_user_init_attributes)() = 0; 
/* the user_init0() callback should be used to setup for common 
* vendor daemon usage. Internally this callback is called at the start of the 
* vendor daemon intialization before lm_job is initalized. */
void (*ls_user_init0)() = 0;
void (*ls_user_init1)() = 0;
void (*ls_user_init2)() = 0;
void (*ls_user_init3)() = 0;
/* Checkout, checkin filters, checkin callback */
int (*ls_outfilter)() = 0;
int (*ls_infilter)() = 0;
int (*ls_incallback)() = 0;
/* vendor message, challenge */
char *(*ls_vendor_msg)() = 0;
char *(*ls_vendor_challenge)() = 0;
/* callback for client shutdown p8031 */
void (*ls_user_down) () = 0;
int ls_crypt_case_sensitive = 0; /* Is license key case-sensitive? -
Only used for vendor-supplied
encryption routines. */
/*
*      ls_user_lockfile:  We no longer recommend changing this
*          here, since the text string for the lockfile will
*          be visible in the binary, and could be therefore
*          changed.
*/
char *ls_user_lockfile = (char *)NULL;
int ls_read_wait = 10;		/* How long to wait for solicited reads */
int ls_dump_send_data = 0;	/* Set to non-zero value for debug output */
int ls_normal_hostid = 1;	/* <> 0 -> normal hostid call, 0 -> extended */
int ls_conn_timeout = MASTER_WAIT;  /* How long to wait for a connection */
int ls_enforce_startdate = 1;	/* Enforce start date in features */
int ls_tell_startdate = 1;	/* Tell the user if it is earlier than start
date */
int ls_minimum_user_timeout = 900; /* Minimum user inactivity timeout (seconds)
<= 0 -> activity timeout disabled */
int ls_min_lmremove = 120;	/* Minimum amount of time (seconds) that a
client must be connected to the daemon before
an "lmremove" command will work. */
int ls_use_featset = 0;		/* Use the FEATURESET line from the license
file */
char *ls_dup_sel = LM_NO_COUNT_DUP_STRING;  /* Duplicate selection criteria
to emulate count_duplicates for
pre-v2 clients */
int ls_use_all_feature_lines = 0; /* Use ALL copies of feature lines that are
unique if non-zero (ADDITIVE licenses) */
int ls_do_checkroot = 0;	/* Perform check that we are running on the
real root of filesystem.  NOTE: this check
will fail on diskless systems, but they
shouldn't be used as server nodes, anyway */
int ls_show_vendor_def = 0;	/* If non-zero, the vendor daemon will send
the vendor-defined checkout data back in
lm_userlist() calls */
int ls_allow_borrow = 0;	/* Allow "Borrowing" of licenses by another
server */
/* Hostid redirection verification routine */
int (*ls_hostid_redirect_verify)() = 0;
/* Hostid Redirection verification */
/* Vendor-defined periodic call */
void (*ls_daemon_periodic)() = 0;
/* Vendor-defined periodic call in daemon */
void (*ls_child_exited)() = 0;
/* Vendor-defined callback -- called with
CLIENT_DATA * argument */
int ls_compare_vendor_on_increment = 0;	/* Compare vendor-defined fields to
declare matches for INCREMENT lines? */
int ls_compare_vendor_on_upgrade = 0;	/* Compare vendor-defined fields to
declare matches for UPGRADE lines? */
/*
*	ls_a_behavior_ver can also be set in lm_code.h.
*	lm_code.h takes priority.  If set there, the value here
*	will not be used.
*/
char *ls_a_behavior_ver = 0;	/* like LM_A_BEHAVIOR_VER */
int ls_a_check_baddate = 0;		/* like LM_A_CHECK_BADDATE */
int ls_a_lkey_start_date = 0;		/* like LM_A_KEY_START_DATE */
int ls_a_lkey_long = 0;			/* like LM_A_KEY_LONG */
int ls_a_license_case_sensitive = 0;	/* like LM_A_LICENSE_CASE_SENSITIVE */
int ls_hud_hostid_case_sensitive = 0; 	/* Forces hostid checks for hostname,     */
/* user, or display to be case sensitive. */
void (*ls_a_user_crypt_filter)() = 0;
void (*ls_a_phase2_app)() = 0;
#define TWELVE_HOURS (12 * 60 * 60)
int ls_user_based_reread_delay =
TWELVE_HOURS;	/* Affects USER_BASED and HOST_BASED:
* a reread changes the  INCLUDE
* after this many hours.  Default
* is 12 hours.  Value is in seconds.
* -1 means that the INCLUDE list
* for these features cannot be changed
* via lmreread.
*/
/* callback routine for semaphore name change 
* use lc_set_attr_vendor(  LM_A_USER_LOCK_CALLBACK ) to set this for 
* a vendor other than the primary vendor*/
char *(*ls_user_lock)() = NULL;         
int ls_borrow_return_early = 0;	/* Set to 1 to allow users to return borrowed licenses early */
/*
*	Forces vendor daemon to send oldest license if a client's version doesn't match a supported signature
*/
int ls_send_oldest_signature = 0;
/*
*  Allows access to trusted storage.
*/
int ls_use_trusted_storage = 1;
void (*ls_borrow_out)(char * szBorrowData, int iSize)  = sBorrowOut;
void (*ls_borrow_in)(char * szBorrowData, int iSize) = sBorrowIn;
void (*ls_borrow_init)(char ** pszBorrowBuffer, int * piSize) = sBorrowInit;
/*
*	Code to determine location of "borrowing" file
*/
#ifdef PC /* WINNT */
static
int
sDetermineSystemVersion()
{
int		iOSVer = 0;
DWORD	dwVersion = 0;
DWORD	dwWindowsMajorVersion = 0;
DWORD	dwWindowsMinorVersion = 0;
dwVersion = GetVersion();
dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
if (dwVersion < 0x80000000)			/* Windows NT/2000/XP */
iOSVer = OS_NT_WIN2K_XP;
else if (dwWindowsMajorVersion < 4)	/* Win32s */
iOSVer = OS_WIN32S;
else								/* Windows 95/98/Me */
iOSVer = OS_95_98_ME;
return iOSVer;
}
static
void
sGetFLEXPath(
char *	buffer,
int		size)
{
char				defaultflexdir[MAX_PATH] = {'\0'};
char				originalpath[MAX_PATH] = {'\0'};
char				tempbuffer[MAX_PATH] = {'\0'};
char				tempbuffer2[MAX_PATH] = {'\0'};
char *				commonfilebuffer = NULL;
int					mask = 0;
L_SHGETFOLDERPATHA	GSISHGetFolderPath = NULL;
HANDLE				hSHFolder = NULL;
DWORD				err = 0;
char				szPath[MAX_PATH] = {'\0'};
/*
*	determin OS
*	if Win2000 or above use /program files/common files/macrovision/
*	if NT4 create & use program files/common files/macrovision/
*	if Win9.x use c:/flexlm
*/
mask = umask(0);
_getcwd(originalpath, MAX_PATH);	/* save original path */
switch(sDetermineSystemVersion())
{
case OS_NT_WIN2K_XP:
hSHFolder = LoadLibrary("shfolder.dll");
if (hSHFolder != NULL)
{	/*
*	Use Microsoft recommended method of finding
*	"common file" folder
*/
GSISHGetFolderPath = (L_SHGETFOLDERPATHA)
GetProcAddress(hSHFolder, "SHGetFolderPathA");
if ((err = (*GSISHGetFolderPath)(NULL, CSIDL_COMMON_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, szPath)) == S_OK)
{
sprintf(defaultflexdir,"%s\\Macrovision\\FLEXlm", szPath);
if ((_access(defaultflexdir, 00) == -1))
{	/* create level 1 */
sprintf(defaultflexdir, "%s", szPath );
_chdir(defaultflexdir);
sprintf(defaultflexdir, "%s\\Macrovision", szPath );
if ((_mkdir(defaultflexdir) == -1))
{
/* use default */
sprintf(defaultflexdir, DEFAULT_FLEXLM_DIR);
_mkdir(defaultflexdir);
}
else
{	/* create level 2 */
_chdir(defaultflexdir);
sprintf(defaultflexdir, "%s\\Macrovision\\FLEXlm", szPath );
if ((_mkdir(defaultflexdir) == -1))
{
/* use default */
sprintf(defaultflexdir, DEFAULT_FLEXLM_DIR);
_mkdir(defaultflexdir);
}
}
}
}
else	/* error on calling DLL */
{
/*
*	Use default
*/
sprintf(defaultflexdir, DEFAULT_FLEXLM_DIR);
_mkdir(defaultflexdir);
}
if(hSHFolder)
{
FreeLibrary(hSHFolder);
hSHFolder = NULL;
}
}
else /* use old fashion directory create if shfolder.dll is not found */
{
commonfilebuffer = getenv("ALLUSERSPROFILES");
if (commonfilebuffer == NULL)
{
/*
*	Use default
*/
sprintf(defaultflexdir, DEFAULT_FLEXLM_DIR);
_mkdir(defaultflexdir);
}
else
{
sprintf(defaultflexdir,"%s\\Application Data\\Macrovision\\FLEXlm", commonfilebuffer );	/* OVERRUN */
if ((_access(defaultflexdir, 00) == -1))
{	/* create level 1 */
sprintf(defaultflexdir, "%s", commonfilebuffer );
_chdir(defaultflexdir);
sprintf(defaultflexdir, "%s\\Application Data", commonfilebuffer );
if ((_mkdir(defaultflexdir) == -1))
{
/*
*	Use default
*/
sprintf(defaultflexdir, DEFAULT_FLEXLM_DIR);
_mkdir(defaultflexdir);
}
else
{	/* create level 2 */
_chdir(defaultflexdir);
sprintf(defaultflexdir, "%s\\Application Data\\Macrovision", commonfilebuffer );
if ((_mkdir(defaultflexdir) == -1))
{
/*
*	Use default
*/
sprintf(defaultflexdir, DEFAULT_FLEXLM_DIR);
_mkdir(defaultflexdir);
}
else
{	/* create level 3 */
sprintf(defaultflexdir, "%s", commonfilebuffer );
_chdir(defaultflexdir);
sprintf(defaultflexdir, "%s\\Application Data\\Macrovision\\FLEXlm", commonfilebuffer );
if ((_mkdir(defaultflexdir) == -1))
{
/*
*	Use default
*/
sprintf(defaultflexdir, DEFAULT_FLEXLM_DIR);
_mkdir(defaultflexdir);
}
}
}
}
}
commonfilebuffer = NULL;
}
break;
case OS_95_98_ME:
default:
sprintf(defaultflexdir, DEFAULT_FLEXLM_DIR);
_mkdir(defaultflexdir);
break;
}
_chdir(originalpath);	/* restore original path */
umask(mask);
/*
*	Copy data over
*/
if((int)strlen(defaultflexdir) > size)
memcpy(buffer, defaultflexdir, size);
else
strcpy(buffer, defaultflexdir);
}
#else /* !PC WINNT */
static
void
sGetFLEXPath(
char *	buffer,
int		size)
{
strcpy(buffer, "/var/tmp");
}
#endif /* WINNT */
static
void
sBorrowOut(
char *	szBorrowData,
int		iSize)
{
FILE *	fp = NULL;
char	buffer[MAX_PATH] = {'\0'};
char	szBorrowFilename[MAX_PATH] = {'\0'};
sGetFLEXPath(buffer, MAX_PATH);
sprintf(szBorrowFilename, "%s%s%sborrow", buffer,
DIRECTORY_SEPARATOR, VENDOR_NAME);
/*
*	Open file and position at EOF
*/
fp = fopen(szBorrowFilename, "ab");
if(fp)
{
/*
*	Write entry
*/
(void)fwrite(szBorrowData, iSize, 1, fp);
(void)fclose(fp);
}
return;
}
static
void
sBorrowIn(
char *	szBorrowData,
int		iSize)
{
FILE *	fp = NULL;
char	entry[4096] = {'\0'};
char *	pPosition = NULL;
char *	pBuffer = NULL;
char *	pCurr = NULL;
int		length = 0;
long	filesize = 0;
long	remaining = 0;
char	buffer[MAX_PATH] = {'\0'};
char	szBorrowFilename[MAX_PATH] = {'\0'};
sGetFLEXPath(buffer, MAX_PATH);
sprintf(szBorrowFilename, "%s%s%sborrow", buffer,
DIRECTORY_SEPARATOR, VENDOR_NAME);
/*
*	Open borrow file
*/
fp = fopen(szBorrowFilename, "rb");
if(fp)
{
/*
*	Allocate buffer to hold contents of new file
*/
(void)fseek(fp, 0, SEEK_END);
remaining = filesize = ftell(fp);
if(filesize > 0)
{
pBuffer = (char *)malloc(sizeof(char) * filesize);
if(!pBuffer)
{
(void)fclose(fp);
return;
}
(void)fseek(fp, 0, SEEK_SET);
/*
*	Read entire contents of file into memory
*/
(void)fread(pBuffer, filesize, 1, fp);	/* overrun checked */
(void)fclose(fp);
fp = NULL;
pCurr = pBuffer;
}
else
{
/*
*	File has no data
*/
(void)fclose(fp);
return;
}
while(remaining)
{
/*
*	Read length of entry
*/
memset(entry, 0, sizeof(entry));
memcpy((void *)&length, pCurr, sizeof(length));
if(length > remaining)
{
if(pBuffer)
{
free(pBuffer);
pBuffer = NULL;
}
return;
}
memcpy(entry, pCurr, length);
remaining -= length;
pPosition = pCurr;
pCurr += length;
/*
*	Compare with borrow/linger coming back in
*/
if(length == iSize &&
memcmp(entry, szBorrowData, length) == 0)
{
/*
*	Found it
*/
if(remaining)
{
/*
*	Append the remaining portion of the file, starting
*	at the location for this current entry
*/
memcpy(pPosition, pCurr, remaining);
}
/*
*	Update what the new file size will be
*/
filesize -= length;
/*
*	Delete original file
*/
(void)remove(szBorrowFilename);
/*
*	Create new file with updated info
*/
fp = fopen(szBorrowFilename, "ab");
if(fp)
{
(void)fwrite(pBuffer, filesize, 1, fp);
(void)fclose(fp);
fp = NULL;
break;
}
}
}
/*
*	Cleanup
*/
if(pBuffer)
free(pBuffer);
if(fp)
(void)fclose(fp);
}
return;
}
static
void
sBorrowInit(
char **	ppBuffer,
int *	piSize)
{
FILE *	fp = NULL;
long	filesize = 0;
char	buffer[MAX_PATH] = {'\0'};
char	szBorrowFilename[MAX_PATH] = {'\0'};
sGetFLEXPath(buffer, MAX_PATH);
sprintf(szBorrowFilename, "%s%s%sborrow", buffer,
DIRECTORY_SEPARATOR, VENDOR_NAME);
fp = fopen(szBorrowFilename, "rb");
if(fp)
{
(void)fseek(fp, 0, SEEK_END);
filesize = ftell(fp);
(void)fseek(fp, 0, SEEK_SET);
*ppBuffer = (char *)malloc(sizeof(char) * filesize);
if(*ppBuffer)
{
(void)fread(*ppBuffer, filesize, 1, fp);	/* overrun checked */
*piSize = filesize;
}
else
{
*piSize = 0;
}
}
if(fp)
(void)fclose(fp);
remove(szBorrowFilename);
}
