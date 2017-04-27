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
 /*  $Id: fulfillExample.c 64 2007-12-13 19:20:30Z harsh.govind $ */
 /**
 *    @file          fulfillExample.c
 *    @brief        example code showing how to use the fulfill API calls.
 *    @version    $Revision: 64 $
 *****************************************************************************/

#include "lmclient.h" 
#include "fulfill.h"
#include "fulfillError.h"
#include <stdio.h>

/************************************************************************/
/** @brief                check if error occurred.
 *  @param      msg       message to print if error occurred.
 *  @param      error     the error code.
 ************************************************************************/
void
checkErrorCode(char *msg, int error)
{
	if (error != FC_NO_ERROR)
	{
		printf("Error: %s: %s\n", msg, fc_getErrorMessage(error));
		fflush(NULL);
		exit (1);
	}
}

/************************************************************************/
/** @brief                handle error
 *  @param      msg       message to print if error occurred.
 ************************************************************************/
void
handleError(char *msg)
{
	printf("Error: %s\n", msg);
	fflush(NULL);
	exit (2);
}

/************************************************************************/
int
main(int argc, char* argv[]) 
{
	FCP_FULFILLREQUEST req;
	int err = 0;
	FCP_ITEM item;
	FCP_LICENSE lic;
	char *licStr = NULL;

	/*
	 *	Obtain the appropriate hostid.  For this example, the ethernet address
	 *	is hard coded below.   Replace this assignment with your custom method
	 *	for obtaining the hostid.
	 */
	char etherAddr[] = "001122334455"; 
    
	/*
	 *	Define the Fulfillment service URL at your site.  Replace this value
	 *	with the appropriate URL at your site.
	 */
	char fulfillUrl[] = "http://mycompany.com:8080/gtlws/soap/Fulfill";  

	/*
	 *	Define the License Authorization Code (LAC).  Replace this assignment
	 *	with your custom method for determining such.  For this example, it
	 *	is hard coded.
	 */
	char lac[] = "OR12345-67890";

	/* Initialize the fulfill request. */
	req = fc_newFulfillRequest(fulfillUrl);
	if (req == NULL)
	{
		handleError("fc_newFulfillRequest() failed.");
	}
    
    /* If the client goes through a http proxy server, set proxy-server info */
    /*
    fc_setProxyServer(req, "myproxy.com", 3128, username, password);
     */

	/*
	 *	Add hostid to the fulfill request.  The hostid type here is
	 *	HOSTID_ETHER.  Change the type as appropriate to your hostid.
     *  For a vendor-defined hostid, the call should look like:
     *      fc_addHostid(req, HOSTID_VENDOR, "MY_VDH=1234567") 
	 */
	err = fc_addHostid(req, HOSTID_ETHER, etherAddr);
	checkErrorCode("fc_addHostid", err);

	/* Add an item to fulfill request.   */
	item = fc_addItem(req, lac);
	if (item == NULL)
	{
		handleError("fc_addItem() failed.");
	}

	/* Get the license by sending this request to the Fulfill service. */
	lic = fc_getLicense(req);
	if (lic == NULL)
	{
		handleError("fc_getLicense() failed.");
	}

	/* Did we get an error from fc_getLicense() ? */
	err = fc_getLicenseError(lic);
	checkErrorCode("fc_getLicense", err);

	/* Get license string from the license and print it. */
	licStr = fc_getLicenseString(lic);
	printf("license string=%s\n", licStr); fflush(NULL);

	/* free resources allocated for the fulfill request */
	fc_release(req);

	return 0;
}
