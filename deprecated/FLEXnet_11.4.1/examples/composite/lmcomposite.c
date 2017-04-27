/******************************************************************************

	    Copyright (c) 2003-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of
	Macrovision Corporation and is protected by law.  It may
	not be copied or distributed in any form or medium, disclosed
	to third parties, reverse engineered or used in any manner not
	provided for in said License Agreement except with the prior
	written authorization from Macrovision Corporation

 *****************************************************************************/
/*
 *
 *	Description:	This is a sample application program, to illustrate
 *			the generation of a composite hostid.
 *
 *	Last changed:  05 Mar 2003
 *	J. Wong
 *
 */

#include "lmclient.h"
#include <stdio.h>
#include <stdlib.h>

VENDORCODE code;
LM_HANDLE *lm_job;

void
main(int argc, char * argv[])
{
int ret, i;
char buf[MAX_CONFIG_LINE];
#ifdef PC
int hostid_list[] = {HOSTID_ETHER, HOSTID_DISK_SERIAL_NUM};
int hostid_list_count = 2;
#else
int hostid_list[] = {HOSTID_INTERNET, HOSTID_HOSTNAME};
int hostid_list_count = 2;
#endif /* PC */


	if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
	{
		lc_perror(lm_job, "lc_new_job failed");
		exit(lc_get_errno(lm_job));
	}

/*
 *	Initialize Composite HostID settings
 */
	ret = lc_init_simple_composite(lm_job, hostid_list, hostid_list_count);
	if (ret != 0)
	{
		printf("Failed to init composite hostid. Errno: %d\n", errno);

		lc_free_job(lm_job);
		exit(0);
	}

	ret = lc_hostid(lm_job, HOSTID_COMPOSITE, buf);
	if (ret == 0)
		printf("The FLEXnet Licensing Composite HostID of this machine is %s\n", buf);
	else
		lc_get_errno(lm_job);

	lc_free_job(lm_job);
	exit(0);
}
