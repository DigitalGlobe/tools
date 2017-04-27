/******************************************************************************

	    COPYRIGHT (c) 1989, 2003 by Macrovision Corporation.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of 
	Macrovision Corporation and is protected by law.  It may 
	not be copied or distributed in any form or medium, disclosed 
	to third parties, reverse engineered or used in any manner not 
	provided for in said License Agreement except with the prior 
	written authorization from Macrovision Corporation.

 *****************************************************************************/	
 /*	$Id: fulfill.h 64 2007-12-13 19:20:30Z harsh.govind $ */
 /**
 *      @file           fulfill.h
 *	@brief        	API calls used to send a XML Soap request to the
 *                      GTL fulfill service to get licenses.
 *                      This header file is exposed to the vendor.
 *      @version        $Revision: 64 $
 *****************************************************************************/
 

#ifndef _FULFILL_H_
#define _FULFILL_H_

#include <time.h>

/* hostid types are defined in flexlm/machind/lmhostid.h.
 */

typedef struct _fcItem       FcItem;
typedef struct _fcItem      *FCP_ITEM;

typedef struct _fcFulfillRequest    FcFulfillRequest;
typedef struct _fcFulfillRequest   *FCP_FULFILLREQUEST;

typedef struct _fcLicense    FcLicense;
typedef struct _fcLicense   *FCP_LICENSE;

/* API function declarations. */
extern FCP_FULFILLREQUEST fc_newFulfillRequest( char *url );
extern int fc_addHostid( FCP_FULFILLREQUEST req, int type, char *value );
extern FCP_ITEM fc_addItem( FCP_FULFILLREQUEST req, char *lac );
extern FCP_LICENSE fc_getLicense( FCP_FULFILLREQUEST req );
extern char *fc_getLicenseString( FCP_LICENSE license );
extern int fc_getLicenseError( FCP_LICENSE license );
extern char *fc_getErrorMessage( int error );
extern void fc_release( FCP_FULFILLREQUEST req );

extern int fc_setExpiryDate( FCP_ITEM item, time_t date );
extern int fc_setDuration( FCP_ITEM item, int duration );
extern int fc_setCount( FCP_ITEM item, int count );
extern int fc_setItemNumber( FCP_ITEM item, int itemNum );
extern int fc_setProxyServer( FCP_FULFILLREQUEST req, char * server, int port,
                char * username, char * password );
extern int fc_addParameter( FCP_ITEM item, char *name, char *value );


#endif






