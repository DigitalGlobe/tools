/**************************************************************************************************/
/*!@file
 * @brief C Interface Definition of the System Installer.	 	
 *
 * See function comments and docs for more information on usage
 *
 * @version \$Revision: 64 $
 */
/***************************************************************************************************
 * Copyright 2005 - 2006 Macrovision Europe Ltd. All Rights Reserved. 
 * 
 * This software has been provided pursuant to a License Agreement containing
 * restrictions on its use. This software contains valuable trade secrets 
 * and proprietary information of Macrovision Europe Ltd. and is protected 
 * by law. It may not be copied or distributed in any form or medium, 
 * disclosed to third parties, reverse engineered or used in any manner not 
 * provided for in said License Agreement except with the prior written 
 * authorization from Macrovision Europe Ltd. 
 **************************************************************************************************/

#ifndef ACTSVCINSTALLER_H
#define ACTSVCINSTALLER_H

/* C API definition */
#ifndef INSTALLER_API
#define INSTALLER_API
#endif

/* start of c functions. */
#ifdef __cplusplus
extern "C" 
{
#endif

typedef unsigned char FNPINS_BOOL;

/* NOTE: the publisher and product name MUST NOT contain any ',' (comma) characters. */

/**************************************************************************************************
 NAME:          fnpActSvcInstallWin

 DESCRIPTION:   Called to installed (or upgrade existing) system files.
                Should be called even if you want to register an interest in the 
                services and must be paired with an UnInstall.

 PARAMETERS:    None

 RETURNS:       BOOL - TRUE = Success, FALSE = Look at error code.
                call fnpActSvcGetLastErrorWin for addition return information.
                Valid fnpActSvcGetLastErrorWin codes are:
                FNP_INSTALL_NOEXTENDEDINFO
                FNP_INSTALL_UPGRADED
                FNP_INSTALL_NOUPGRADEREQ
                FNP_INSTALL_INSTALLED
                FNP_INSTALL_ACCEPTABLEINSTALLED

                FNP_INSTALL_REBOOTREQ
                FNP_INSTALL_INUSE
                FNP_INSTALL_CANNOTCOPYFILE
                FNP_INSTALL_CANNOTSTART
                FNP_INSTALL_CANNOTSTOP
                FNP_INSTALL_NORIGHTS
                FNP_INSTALL_INVALIDPATH
                FNP_INSTALL_UNSUPPORTEDOS
                FNP_INSTALL_DISABLED
                FNP_INSTALL_EARLYREBOOT
 *************************************************************************************************/
extern INSTALLER_API FNPINS_BOOL fnpActSvcInstallWin( const char* publisher, const char* product );

/**************************************************************************************************
 NAME:          fnpActSvcUninstallWin

 DESCRIPTION:   Called to uninstall your use of the services. 
                Using a reference counter it determines if the services 
                should be removed from the system.

 PARAMETERS:    None

 RETURNS:       BOOL - TRUE = Success, FALSE = Look at error code.
                call fnpActSvcGetLastErrorWin for addition return information.
                Valid fnpActSvcGetLastErrorWin codes are:
                FNP_INSTALL_NOEXTENDEDINFO
                FNP_INSTALL_UNINSTALLED

                FNP_INSTALL_REBOOTREQ
                FNP_INSTALL_INUSE
                FNP_INSTALL_CANNOTDELETEFILE
                FNP_INSTALL_CANNOTSTOP
                FNP_INSTALL_NORIGHTS
                FNP_INSTALL_INVALIDPATH
                FNP_INSTALL_UNSUPPORTEDOS

                NOTE: Returning TRUE with FNP_INSTALL_NOEXTENDEDINFO as an
                      extended error indicates that other products are installed
                      and have registered with the shared components
 *************************************************************************************************/
extern INSTALLER_API FNPINS_BOOL fnpActSvcUninstallWin( const char* publisher, const char* product );


/**************************************************************************************************
 NAME:          fnpActSvcGetLastErrorWin

 DESCRIPTION:   Gives extended information for the return value for the above
                functions.

 PARAMETERS:    None

 RETURNS:       unsigned int - see below for all possible return codes.
 *************************************************************************************************/
extern INSTALLER_API unsigned int fnpActSvcGetLastErrorWin( void );

/* end of C functions. */
#ifdef __cplusplus
}
#endif


/* Extended return codes, given by fnpActSvcGetLastErrorWin */

/* by All - there is no more information about the return code.*/
#define FNP_INSTALL_NOEXTENDEDINFO  0       

/* Success code extended values - for use when a TRUE is given */

/* by CdaSysInstall - The installed driver was older than the new driver so it was upgraded. */
#define FNP_INSTALL_UPGRADED        2001	    
/* by CdaSysInstall - The new driver is the same as or older than the installed driver so no upgrade is required */
#define FNP_INSTALL_NOUPGRADEREQ    2002       
/* by CdaSysInstall - There was not a driver on the system so the new one was installed. */
#define FNP_INSTALL_INSTALLED       2003	    
/* by CdaSysUnInstall - The installed driver has been successfully removed from the system.*/
#define FNP_INSTALL_UNINSTALLED     2004	    
/* by CdaSysInstall - A driver or service is in use and cannot be updated until a reboot, but an acceptable version is installed.*/
#define FNP_INSTALL_ACCEPTABLEINSTALLED 2006   
/* by CdaSysUnInstallLicence - No more licenses exists, so the diretory structure has been removed */
#define FNP_INSTALL_LICUNINSTALLED  2007

/* Failure codes - for use when a FALSE is given */

/* by All - The system requires a reboot at to complete the operation.*/
#define FNP_INSTALL_REBOOTREQ       1001    
/* by All - driver is in use install/uninstall will fail, try shutting down all other apps then try again. Otherwise reboot if it failed this way a second time */
#define FNP_INSTALL_INUSE           1002    
/* by CdaSysInstall - Failed to copy new driver to system, may not be at source, or cannot copy to destination folder.*/
#define FNP_INSTALL_CANNOTCOPYFILE  1003	
/* by CdaSysUninstall - Unable to delete the driver from disk, might have a share violation or have no access rights.*/
#define FNP_INSTALL_CANNOTDELETEFILE 1004	
/* by CdaSysInstall - Failed to start a service/driver that is required.*/
#define FNP_INSTALL_CANNOTSTART     1005	
/* by All - Could not stop the current service, either for an upgrade or uninstall. It may be in use, could exit all other apps and try again, same as FNP_INSTALL_INUSE.*/
#define FNP_INSTALL_CANNOTSTOP      1006	
/* by All - Access has been denied to a resource, either a service or a file, or the registry. Administrator privilege is required to resolve this problem.*/
#define FNP_INSTALL_NORIGHTS        1007	
/* by CdaSysInstall,The path for with the source or the destination is invalid (i.e. too long or not set)*/
#define FNP_INSTALL_INVALIDPATH     1008	
/* by CdaSysInstall,the (NT) service is disabled and we are unable to restart it. Could be in use or a reboot may resolve it*/
#define FNP_INSTALL_DISABLED        1009
/* by CdaSysInstall,CdaSysUnInstall - The system is in a unrecoverable state left over from a previous uninstall, install or upgrade, and to successfully run the command you must first reboot, then run it again */
#define FNP_INSTALL_EARLYREBOOT     1010

/* by All - The current OS is not supported.*/
#define FNP_INSTALL_UNSUPPORTEDOS   1100	

#endif /* ACTSVCINSTALLER_H */