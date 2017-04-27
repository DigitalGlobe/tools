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
 *      Module: Installs.c
 *
 *      Function:       main() - main routine for installing FLEXlm Service
 *

 *
 *      Description:    Example program for installing FLEXlm Service.
 *
 *      Parameters:     None
 *
 *      Return:         None.
 *
 *
 *
 *
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SC_HANDLE   schService;
SC_HANDLE   schSCManager;
#define MAX_PATH_SIZE	255

char service_name[80];
char license_path[MAX_PATH_SIZE];
char log_file_path[MAX_PATH_SIZE];
char lmgrd_path[MAX_PATH_SIZE];
char CmdLine_Params[MAX_PATH_SIZE];
char * token;
HKEY hcpl;
DWORD version_number;
int OsType;
#define NT_OS 1
#define WIN_95 2
#define FLEXLM_SERVICE_NAME     "FLEXlm License Manager"
void CheckLmgrdSecurity (char * lpszFullName);

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Gets the TCP/IP Connect Retry Number Setting from Registry            //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
void
l_get_registry(int *value)
{
	HKEY hcpl;
	unsigned long length=4,type=REG_DWORD;
	char * RegistryAddress = NULL;
	char * FieldName = NULL;
	long error = 0;
	DWORD   version_number= GetVersion();

	if (version_number < 0x80000000)
	{
		RegistryAddress= "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters";
		FieldName="TcpMaxConnectRetransmissions";
	}
	else
	{
		RegistryAddress="System\\CurrentControlSet\\Services\\VxD\\MSTCP";
		FieldName="MaxConnectRetries";
	}

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				 RegistryAddress,
				 0,
				 KEY_READ,
				 &hcpl) == ERROR_SUCCESS)
	{
		// Set the value
		error = RegQueryValueEx(hcpl,
					FieldName,
					0,&type,
					(unsigned char *)value,
					&length);

		if (ERROR_SUCCESS != error)
			*value=-1;

		// Finished with keys
		RegCloseKey(hcpl);
		return ;
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Sets the TCP/IP Connect Retry Number Setting in  Registry             //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void
l_set_registry(int value)
{
	HKEY hcpl;
	long length=4;
	char * RegistryAddress;
	char * FieldName;
	DWORD   version_number= GetVersion();

	if (version_number < 0x80000000)
	{
		RegistryAddress= "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters";
		FieldName="TcpMaxConnectRetransmissions";
	}
	else
	{
		RegistryAddress="System\\CurrentControlSet\\Services\\VxD\\MSTCP";
		FieldName="MaxConnectRetries";
	}

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					RegistryAddress,
					0,
					KEY_WRITE,
					&hcpl) == ERROR_SUCCESS)
	{


		RegSetValueEx(hcpl,
				FieldName,
				0,
				REG_DWORD,
				(unsigned char *) &value,
				length);


		// Finished with keys
		RegCloseKey(hcpl);
		return ;
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
// Installs Service on Nt platform                                        //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

int
InstallServiceNT(
	LPCTSTR	Service_Name,
	LPCTSTR	License_Path,
	LPCTSTR	Log_File_Path,
	LPCTSTR Lmgrd_Path)
{
	LPCTSTR lpszBinaryPathName = Lmgrd_Path;

	schService = CreateService(
				schSCManager,               // SCManager database
				Service_Name,               // name of service
				Service_Name,               // name to display
				SERVICE_ALL_ACCESS,         // desired access
				SERVICE_WIN32_OWN_PROCESS,  // service type
				SERVICE_AUTO_START,         // start type:
											//   SERVICE_DEMAND_START = Manual start of service
											//   SERVICE_AUTO_START   = Starts service automatically at start of OS
				SERVICE_ERROR_NORMAL,       // error control type
				lpszBinaryPathName,         // service's binary
				NULL,                       // no load ordering group
				NULL,                       // no tag identifier
				"+NetworkProvider",         // dependent on Network Provider
				NULL,                       // LocalSystem account
				NULL);                      // no password

	if (schService == NULL)
	{
		printf("Failed to install FLEXnet License Manager:\n");
		switch (GetLastError())
		{
			case ERROR_ACCESS_DENIED:
					printf( "Access to Windows NT Service Control "
							"Manager Database denied.\n" );
					break;

			case ERROR_DUP_NAME:
			case ERROR_SERVICE_EXISTS:
					printf("FLEXnet License Manager is already installed\n");
					break;

			default:
					printf( "Error code: 0X%X\n", GetLastError());
					break;
		}
		return FALSE;
	}
	else
	{
		CloseServiceHandle(schService);
		return TRUE;
	}

}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Installs a PSEUDO service on windows 95                               //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


int
InstallServiceWin95(
	LPCTSTR	Service_Name,
	LPCTSTR	License_Path,
	LPCTSTR	Log_File_Path,
	LPCTSTR	Lmgrd_Path)
{
	char logfile[MAX_PATH_SIZE] = {'\0'};
	char commandLine[MAX_PATH_SIZE] = {'\0'};
	char temp[MAX_PATH_SIZE] = {'\0'};
	HKEY hcpl;
	HKEY happ;
	DWORD dwType = 0;
	DWORD dwSize=0;
	DWORD dwDisp;
	BOOL Aauto ;
	char * cptr = NULL;

	if (!Log_File_Path[0])
	{
		strcpy(logfile,"NUL");
	}
	else
	{
		strcpy(logfile, Log_File_Path);
	}

	strcpy(temp,Lmgrd_Path);
	cptr=strrchr(temp,'\\')  ;
	*cptr=0;
	sprintf(commandLine,"%s\\lmgrd.exe %s -win -c %s -l %s",
			temp, Lmgrd_Path,License_Path,logfile);

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					"SOFTWARE",
					0,
					KEY_QUERY_VALUE,
					&hcpl) == ERROR_SUCCESS)
	{
		if (RegCreateKeyEx(hcpl,
						"Microsoft\\Windows\\CurrentVersion\\RunServices",
						0,
						"",
						REG_OPTION_NON_VOLATILE,
						KEY_WRITE,
						NULL,
						&happ,
						&dwDisp) == ERROR_SUCCESS)
		{
			// Set the value
			RegSetValueEx(happ,
					Service_Name,
					0,
					REG_SZ,
					(unsigned char *)commandLine,
					strlen(commandLine)+1);
			RegCloseKey(happ);
			RegCloseKey(hcpl);
			return TRUE;
		}
		RegCloseKey(hcpl);
		return FALSE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Installs the service, figures out which os, writes Registry settings  //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


VOID
InstallService(
	LPCTSTR	Service_Name,
	LPCTSTR	License_Path,
	LPCTSTR	Log_File_Path,
	LPCTSTR	Lmgrd_Path,
	LPCTSTR CmdLine_Params)
{
    LPCTSTR lpszBinaryPathName = Lmgrd_Path;
    int RetOk;
        if (NT_OS==OsType)
        {
                RetOk=InstallServiceNT(Service_Name,License_Path,Log_File_Path,Lmgrd_Path);
        }
        else   // WINDOWS 95 case
        {
                RetOk=InstallServiceWin95(Service_Name,License_Path,Log_File_Path,Lmgrd_Path);
        }


        if ((RetOk) && (NT_OS==OsType))
        {

         // next write registry entries
        // Update the registry
        // Try creating/opening the registry key
                if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     "SOFTWARE",
                     0,
                     KEY_WRITE,
                     &hcpl) == ERROR_SUCCESS)
                {
                HKEY happ, hdefaultapp;
                DWORD dwDisp;
                char new_name[120];
                        sprintf(new_name,"FLEXlm License Manager\\%s", Service_Name);
                        if (RegCreateKeyEx(hcpl,
                           new_name,
                           0,
                           "",
                           REG_OPTION_NON_VOLATILE,
                           KEY_WRITE,
                           NULL,
                           &happ,
                           &dwDisp) == ERROR_SUCCESS)
                        {
                        RegSetValueEx(happ,
                          "Lmgrd",
                          0,
                          REG_SZ,
                          Lmgrd_Path,
                          strlen(Lmgrd_Path)+1);
                        RegSetValueEx(happ,
                          "LMGRD_LOG_FILE",
                          0,
                          REG_SZ,
                          Log_File_Path,
                          strlen(Log_File_Path)+1);
                        RegSetValueEx(happ,
                          "License",
                          0,
                          REG_SZ,
                          License_Path,
                          strlen(License_Path)+1);
                        RegSetValueEx(happ,
                          "cmdlineparams",
                          0,
                          REG_SZ,
                          CmdLine_Params,
                          strlen(CmdLine_Params)+1);

                        RegSetValueEx(happ,
                          "Service",
                          0,
                          REG_SZ,
                          Service_Name,
                          strlen(Service_Name)+1);

			/* update default service */
                        sprintf(new_name,"FLEXlm License Manager");
			if (RegCreateKeyEx(hcpl,
                           new_name,
                           0,
                           "",
                           REG_OPTION_NON_VOLATILE,
                           KEY_WRITE,
                           NULL,
                           &hdefaultapp,
                           &dwDisp) == ERROR_SUCCESS)
                        {
				RegSetValueEx(hdefaultapp,
					"Service",
					0,
					REG_SZ,
					Service_Name,
					strlen(Service_Name)+1);

			}
			RegCloseKey(hdefaultapp);
                // Finished with keys
                        RegCloseKey(happ);

                        }
                        RegCloseKey(hcpl);
                }

         printf("\nFLEXnet License Manager is successfully installed \n"
               "as one of your Windows Services.  Some handy tips: \n\n"
               "\n"
               "\t* The FLEXnet License Manager will be automatically started\n"
               "\t  every time your system is booted.\n"
               "\n"
               "\t* The FLEXnet service log file is lmgrd.log in your NT system\n"
               "\t  directory.\n"
               "\n"
               "\t* To remove FLEXnet License Manager, type 'installs -r'\n");
    }
    else
         printf("The FLEXnet License Manager was not successfully \n"
                "installed as a service on your system.\n");
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Removes service from NT                                               //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


RemoveServiceNT(LPCTSTR Service_Name)
{
    int ret;
    HKEY hcpl;
    char szServiceEntry[120];
    long lReturn;

                schService = OpenService(schSCManager,
                             Service_Name, SERVICE_ALL_ACCESS);

                if (schService == NULL)
                {
                        switch ( GetLastError() )
                        {
                        case ERROR_ACCESS_DENIED:
                                printf( "Access to Windows NT Service Control "
                                        "Manager Database denied.\n" );
                                break;
                        case ERROR_INVALID_NAME:
                        case ERROR_SERVICE_DOES_NOT_EXIST:
                                printf( "%s service is already removed.\n",Service_Name );
                                break;
                        default:
                                printf( "OpenService error(0x%02x)\n", GetLastError());
                                break;
                        }
                return FALSE;
                }

                ret = DeleteService(schService);

                if (ret)
                {
                       printf("The %s service has been removed.\n",Service_Name);
					/* remove reg entries */
					wsprintf(szServiceEntry, "SOFTWARE\\FLEXlm License Manager");
			        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
									szServiceEntry,
									0,
									KEY_SET_VALUE,
									&hcpl) == ERROR_SUCCESS)

                         lReturn = RegDeleteKey(hcpl,Service_Name);
					if (lReturn == ERROR_SUCCESS)
                    {
						RegCloseKey(hcpl);
						return TRUE;
                    }
                    RegCloseKey(hcpl);
                    return TRUE;
                }
                else
                {
                        switch ( GetLastError() )
                        {
                        case ERROR_ACCESS_DENIED:
                                printf( "Access to Windows NT Service Control "
                                        "Manager Database denied.\n" );
                                break;

                        case ERROR_SERVICE_MARKED_FOR_DELETE:
                        case ERROR_INVALID_NAME:
                        case ERROR_SERVICE_DOES_NOT_EXIST:
                                printf( "The FLEXnet License Manager service is already removed.\n" );
                                break;

                        default:
                                printf("failure: DeleteService (0x%02x)\n", GetLastError());
                                break;
                        }
                return FALSE;
                }


}
////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Removes Psuedo service from windows 95                                //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


RemoveService95(LPCTSTR Service_Name)
{
                HKEY hcpl,happ;

                if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                        "SOFTWARE",
                        0,
                        KEY_QUERY_VALUE,
                        &hcpl) == ERROR_SUCCESS)
                        {
                        if (RegOpenKeyEx(hcpl,
                                "Microsoft\\Windows\\CurrentVersion\\RunServices",
                                0,
                                KEY_QUERY_VALUE,
                                 &happ) == ERROR_SUCCESS)
                                if (RegDeleteValue(happ,Service_Name)==ERROR_SUCCESS)
                                {
                                        RegCloseKey(happ);
                                        RegCloseKey(hcpl);
                                        return TRUE;

                                }
                                RegCloseKey(happ);
                                RegCloseKey(hcpl);
                        }
                return FALSE;

}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Removes service, figures out which os                                 //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

VOID
RemoveService(LPCTSTR Service_Name)
{
    BOOL    ret;
    Sleep(2000);
    switch (OsType)
    {
	case NT_OS:
        {
            RemoveServiceNT( Service_Name);
        }
	break;
        case WIN_95:
        {
            RemoveService95( Service_Name);
        }

    }

}
int
main(int argc, char *argv[])
{
BOOL remove=FALSE;

    if (argc == 1 || *argv[1] != '-') {
		printf("installs - Copyright (c) 1989-2007 Macrovision Europe Ltd. and/or Macrovision Corporation.\nAll Rights Reserved.\n");
        printf("\nUsage: \n");
        printf("        To install the license manager as a service:\n");
        printf("          installs -c <license file path> \\\n");
        printf("                   -e <path to lmgrd.exe> \\\n");
        printf("                   -l <log file path> \\\n");
        printf("                   -n <service name> \\\n");
        printf("                   [-k <lmgrd parameters>] \n");
        printf("\n");
        printf("        To remove the license manager as a service:\n");
        printf("          installs -r -n <service name>\n");
        printf("\n");
        printf("If -n is not specified, \"FLEXnet License Manager\" is used as the service name.\n");
        printf("\n");
		printf("The -k switch is optional and is used to pass one or more startup command-line\n");
		printf("parameters (-local, -x lmdown, and -x lmremove) to lmgrd.\n");
        printf("\n");
        printf("Refer to the FLEXnet Licensing Programming and Reference Guide for more details on the \"installs\" command.\n");
        exit(1);
    }

   strcpy(service_name,FLEXLM_SERVICE_NAME); //use the default name

   *license_path=0;
   *log_file_path=0;
   *lmgrd_path=0;
   *CmdLine_Params=0;

   while (argc > 1)
        {
                if (*argv[1] == '-')
                {
                  char *p = argv[1]+1;

                        switch(*p)
                        {


                          case 'n':
                          case 'N':
                                /*
                                 *
                                 *      specify the name that the service will use
                                 *
                                 * .
                                 */
                                strcpy(service_name,argv[2]);
                                argc--; argv++;
                                break;

                          case 'c':
                          case 'C':
                                strcpy(license_path,argv[2]);
                                argc--; argv++;
                                break;


                          case 'e':
                          case 'E':
                                strcpy(lmgrd_path,argv[2]);
                                argc--; argv++;
                                break;


                          case 'l':
                          case 'L':
                                strcpy(log_file_path,argv[2]);
                                argc--; argv++;
                                break;


                          case 'r':
                          case 'R':
                                remove=TRUE;
                                break;

                          case 'k':
                          case 'K':
                                strcpy(CmdLine_Params, argv[2]);
                                argc--; argv++;
                                break;

                          default:
                                printf("Unrecognized command-line switch ");
                                return 0;
                        }
                }
                argc--; argv++;
        }

        version_number= GetVersion();

        if (  version_number < 0x80000000 )
                  OsType=NT_OS; else  OsType=WIN_95;

        if (NT_OS == OsType )
                schSCManager = OpenSCManager(
                        NULL,                   // machine (NULL == local)
                        NULL,                   // database (NULL == default)
                        SC_MANAGER_ALL_ACCESS   // access required
                        );

        if (remove)
		{
                RemoveService(service_name);
		}
        else
        {

                if (!lmgrd_path)
                {
                        printf (" Need to specify path to LMGRD.EXE, Service not installed \n");
                        return 0;
                }
                InstallService(service_name, license_path, log_file_path,lmgrd_path, CmdLine_Params);
		if (NT_OS == OsType )	CheckLmgrdSecurity( lmgrd_path );
        }
        if (NT_OS == OsType )
                CloseServiceHandle(schSCManager);

	return 0;
}


BOOL SetPrivilegeInAccessToken(VOID)
{
  HANDLE           hProcess;
  HANDLE           hAccessToken;
  LUID             luidPrivilegeLUID;
  TOKEN_PRIVILEGES tpTokenPrivilege;

	hProcess = GetCurrentProcess();
	if (!hProcess)
	{
		return(FALSE);
	}

	if (!OpenProcessToken(hProcess,
                        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                        &hAccessToken))
	{
		return(FALSE);
	}

	/**************************************************************************\
	*
	*	 Get LUID of SeSecurityPrivilege privilege
	*
	\**************************************************************************/
	if (!LookupPrivilegeValue(NULL,
                            SE_SECURITY_NAME,
                            &luidPrivilegeLUID))
	{

		return(FALSE);
	}
	/**************************************************************************\
	*
	* Enable the SeSecurityPrivilege privilege using the LUID just
	*   obtained
	*
	\**************************************************************************/

	tpTokenPrivilege.PrivilegeCount = 1;
	tpTokenPrivilege.Privileges[0].Luid = luidPrivilegeLUID;
	tpTokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges (hAccessToken,
                         FALSE,  // Do not disable all
                         &tpTokenPrivilege,
                         0,//sizeof(TOKEN_PRIVILEGES),
                         NULL,   // Ignore previous info
                         NULL);  // Ignore previous info

	if (( GetLastError()) != NO_ERROR )
	{
		return(FALSE);
	}

	return(TRUE);

}

/****************************************************************************\
*
* FUNCTION: ExamineACL
*
\****************************************************************************/

int ExamineACL   (PACL paclACL )

{
  #define                          SZ_INDENT_BUF 80
  UCHAR					ucNameBuf[SZ_INDENT_BUF] = "";
  UCHAR					ucDomainBuf[SZ_INDENT_BUF];
  DWORD dwNameLength=SZ_INDENT_BUF;
  DWORD dwDomainBuf=SZ_INDENT_BUF;

  ACL_SIZE_INFORMATION  asiAclSize;
  DWORD                dwBufLength;
  DWORD                dwAcl_i;
  ACCESS_ALLOWED_ACE   *paaAllowedAce;
  SID_NAME_USE			peAcctNameUse;
  BOOL AccessAllowed=FALSE;

	if (!IsValidAcl(paclACL))
	{
		return(-1);
	}

	dwBufLength = sizeof(asiAclSize);

	if (!GetAclInformation(paclACL,
                         (LPVOID)&asiAclSize,
                         (DWORD)dwBufLength,
                         (ACL_INFORMATION_CLASS)AclSizeInformation))
	{
		return(-1);
	}

	for (dwAcl_i = 0; dwAcl_i < asiAclSize.AceCount;  dwAcl_i++)
	{
		if (!GetAce(paclACL,
                dwAcl_i,
                (LPVOID *)&paaAllowedAce))
		{
			return(-1);
		}

		dwNameLength=SZ_INDENT_BUF;
		dwDomainBuf=SZ_INDENT_BUF;
		if ( !LookupAccountSid(
				"",  // local machine
				&paaAllowedAce->SidStart,
				ucNameBuf,
				&dwNameLength,
				ucDomainBuf,
				&dwDomainBuf,
				&peAcctNameUse))
		{
			  return(-1);
		}
		else
		{
//		printf (" %s  %X \n",ucNameBuf, paaAllowedAce->Mask);
			switch (paaAllowedAce->Header.AceType)
			{
				case   ACCESS_ALLOWED_ACE_TYPE :
				{
					if (!strcmp("SYSTEM",ucNameBuf) && (paaAllowedAce->Mask == 0x1f01ff))
						AccessAllowed=TRUE;
					if (!strcmp("Guests",ucNameBuf) && (paaAllowedAce->Mask == 0x1f01ff))
						AccessAllowed=TRUE;
					if (!strcmp("Everyone",ucNameBuf) && (paaAllowedAce->Mask == 0x1f01ff))
						AccessAllowed=TRUE;
					break;
				}
				default :
				{	// Ignore it , it doesnt have privleges
					break;
				}
			}

		}
	}
	if (AccessAllowed)
	{
		  return 1;
	}
	else
	{
		 return 0;
	}

}

/****************************************************************************\
*
* FUNCTION: ExamineSD
*
\****************************************************************************/

int ExamineSD    (PSECURITY_DESCRIPTOR psdSD )
{

  PACL                        paclDACL;
  BOOL                        bHasDACL        = FALSE;
  BOOL                        bHasSACL        = FALSE;
  BOOL                        bDaclDefaulted  = FALSE;
  BOOL                        bSaclDefaulted  = FALSE;
  BOOL                        bOwnerDefaulted = FALSE;
  BOOL                        bGroupDefaulted = FALSE;
  DWORD                       dwSDLength;

	if (!IsValidSecurityDescriptor(psdSD))
	{
		return(-1);
	}

	dwSDLength = GetSecurityDescriptorLength(psdSD);

	if (!GetSecurityDescriptorDacl(psdSD,
                                 (LPBOOL)&bHasDACL,
                                 (PACL *)&paclDACL,
                                 (LPBOOL)&bDaclDefaulted))
	{
		return(-1);
	}

	if (bHasDACL && !bDaclDefaulted)
	{

		return ExamineACL(paclDACL );

	}
	return -1;

}

void CheckLmgrdSecurity (char * lpszFullName)
{
UCHAR      ucBuf       [4096] = "";
DWORD      dwSDLength = 4096;
DWORD      dwSDLengthNeeded;

PSECURITY_DESCRIPTOR psdSD = (PSECURITY_DESCRIPTOR)&ucBuf;
BOOL SecurityOK =FALSE;
BOOL CheckSecurity=TRUE;
int SecurityResult=-1;

	CheckSecurity=SetPrivilegeInAccessToken();

	if (CheckSecurity &&!GetFileSecurity(
		lpszFullName,
		(SECURITY_INFORMATION)( DACL_SECURITY_INFORMATION),
        psdSD,
        dwSDLength,
        (LPDWORD)&dwSDLengthNeeded))
	{
		CheckSecurity=FALSE;
	}


	if(CheckSecurity )
	{
		SecurityResult= ExamineSD( psdSD );
	}

	switch (SecurityResult)
	{

	case -1:
			printf("\n\n********************************************************\n"
					"Unable to check the permissions of the files you just installed\n"
					" Make sure that if you have installed on a NTFS partition, that \n"
					" the System user account has access to these files. \n");
			break;

	case 0:
			printf ("\n\n********************************************************\n"
					" The permissions of the files that you just installed may not have the\n"
					" correct permissions for the user System.\n"
					" Make sure that if you have installed on a NTFS partition, that \n"
					" the System user account has access to these files. \n");

			break;

	case 1:
			printf ("\n\n********************************************************\n"
					" The permissions of one of the files that you just installed seems \n"
					" to have the correct settings.\n");
			break;

	}
}
