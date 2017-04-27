/******************************************************************************

	    Copyright (c) 1994-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of
	Macrovision Inc and is protected by law.  It may
	not be copied or distributed in any form or medium, disclosed
	to third parties, reverse engineered or used in any manner not
	provided for in said License Agreement except with the prior
	written authorization from Macrovision Corporation.

 *****************************************************************************/
/*
 *	Description: 	Example composite hostid implementation for FLEXnet license
 *                      server.
 *
 *	Parameters:	(misc)
 *
 *	Return:		(misc)
 *
 *	J. Wong
 *	3/7/03
 *
 *
 *			  All programs that call this function MUST
 *			  have  a variable (LM_HANDLE *) lm_job that points
 *			  to the current job.  If your job handle is not
 *			  called lm_job, make one that points to it.
 *
 *	See  the FLEXnet Licensing Programming and Reference Guide for additional information
 *      regarding "Initializing Composite Hostid in Your Vendor Daemon".
 *
 */


#include "lmclient.h"

extern LM_HANDLE *lm_job; 	/* This must be the current job! */

void
init_composite_hostid()
{
 #ifdef PC
  int ls_hostid_list[] = {HOSTID_ETHER, HOSTID_DISK_SERIAL_NUM};
  int ls_hostid_count = 2;
#else
  int ls_hostid_list[] = {HOSTID_INTERNET, HOSTID_HOSTNAME};
  int ls_hostid_count = 2;
#endif	/* PC */

         /* Composite HostID initialization call for server */
         lc_init_simple_composite(lm_job, ls_hostid_list, ls_hostid_count);

}
