/************************************************************************

        Copyright (c) 2003-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
        This software has been provided pursuant to a License Agreement
        containing restrictions on its use.  This software contains
        valuable trade secrets and proprietary information of
        Macrovision Corp. and is protected by law.  It may not be copied
        or distributed in any form or medium, disclosed to third parties,
        reverse engineered or used in any manner not provided for in
        said License Agreement except with the prior written authorization
        from Macrovision Corp.

 ************************************************************************/

#include <windows.h>
#include <string.h>

#include "lmpolicy.h"
#include "lmprikey.h"
#include "lmseeds.h"

#include "lmgr11.h"

/***********************************************************************
*
*  DLL Entry point.
*
************************************************************************/

BOOL WINAPI DllMain( HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved )
{
	// add any additional DLL initialization code here
	return TRUE;
}

/***********************************************************************
*
*  Function     lt_checkout
*
*  Description  This will attempt to checkout a feature passed to it
*               from the interfacing code.
*
*  Parameters   (int) iPolicy - feature policies
*               (char*) szFeatureName - Feature name provided by caller
*               (char*) szFeatureVer - Feature version id provided by
*                                      caller
*               (char*) szLicFileList - location of lic file
*
************************************************************************/

int WINAPI lt_checkout(
	int iPolicy,
	char *szFeatureName,
	char *szFeatureVer,
	char *szLicFileList)
{
	return CHECKOUT(iPolicy, szFeatureName, szFeatureVer, szLicFileList);
}

/***********************************************************************
*
*  Function     lt_checkin
*
*  Description  Returns a checked out license.
*
*  Parameters   none
*
************************************************************************/

void WINAPI lt_checkin()
{
	CHECKIN();
}

/***********************************************************************
*
*  Function     lt_heartbeat
*
*  Description  Sends a manual heartbeat to the license server.
*
*  Parameters   none
*
************************************************************************/

int WINAPI lt_heartbeat()
{
	return HEARTBEAT();
}

/***********************************************************************
*
*  Function     lt_errstring
*
*  Description  Returns ASCII string for the last FLEXlm error set.
*
*  Parameters   none
*
************************************************************************/

char* WINAPI lt_errstring()
{
	return ERRSTRING();
}

/***********************************************************************
*
*  Function     lt_perror()
*
*  Description  Presents an error dialog box on windows only, along with
*               a user provided string.
*
*  Parameters   (char*) szErrStr - a string describing error context
*
************************************************************************/

void WINAPI lt_perror(char * szErrStr)
{
	PERROR(szErrStr);
}

/***********************************************************************
*
*  Function     lt_pwarn()
*
*  Description  Presents an error dialog box on windows only, along with
*               a user provided string.
*
*  Parameters   (char*) szErrStr - a string describing error context
*
************************************************************************/

void WINAPI lt_pwarn(char * szErrStr)
{
	PWARN(szErrStr);
}

/***********************************************************************
*
*  Function     lt_warning()
*
*  Description  Returns a string describing the last FLEXlm warning.
*
*  Parameters   none
*
************************************************************************/

char* WINAPI lt_warning()
{
	return WARNING();
}
