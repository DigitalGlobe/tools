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

/*****************************************************************************/
/*
 *	Module:	client.c v2.14
 *
 *	Description:	This is a process used for checking out licenses
 *			as the child of another FLEXenabled process.
 *
 *
 *	M. Christiano
 *	2/18/88
 *	Adapted from lmclient.c
 *
 */

#include "lmclient.h"
#include "lm_code.h"
#include "lm_attr.h"
#include <stdio.h>
#include <errno.h>
extern int errno;
char *strcpy();
static int quit(), reconn(), r_done();

LM_CODE(code, ENCRYPTION_SEED1, ENCRYPTION_SEED2, VENDOR_KEY1, VENDOR_KEY2,
					VENDOR_KEY3, VENDOR_KEY4, VENDOR_KEY5);

static int status;

main(argc, argv)
int argc;
char *argv[];
{
  char string[200];
  char feature[200];
  char  version[15];
  int number;
  LM_HANDLE *lm_job;

	status = lc_init((LM_HANDLE *)0, VENDOR_NAME, &code, &lm_job);
	if (status)
	{
		lc_perror(lm_job, "lm_init failed");
		exit(status);
	}
	if (argc > 2 && !strcmp(argv[1], "-c"))
	{
		lc_set_errno(lm_job, 0);
		if (lc_set_attr(lm_job, LM_A_LICENSE_FILE,
				(LM_A_VAL_TYPE) argv[2]))
			lc_perror(lm_job, "Error setting new license file");
	}
/*
 *	License manager parameter intialization (optional)
 */
	(void) lc_set_attr(lm_job, LM_A_USER_EXITCALL, (LM_A_VAL_TYPE)quit);
	(void) lc_set_attr(lm_job, LM_A_USER_RECONNECT, (LM_A_VAL_TYPE)reconn);
	(void) lc_set_attr(lm_job, LM_A_USER_RECONNECT_DONE, (LM_A_VAL_TYPE)r_done);
	(void) lc_set_attr(lm_job, LM_A_USER_EXITCALL, (LM_A_VAL_TYPE)quit);
	(void) lc_set_attr(lm_job, LM_A_CHECK_INTERVAL, (LM_A_VAL_TYPE)-1);
	(void) lc_set_attr(lm_job, LM_A_RETRY_INTERVAL, (LM_A_VAL_TYPE)-1);
/*
 *	Find out which feature to test
 */
	(void) gets(string);
	(void) sscanf(string, "checkout %s %d %s\n", feature, &number,
								version);
	status = lc_checkout(lm_job, feature, version, number, LM_CO_NOWAIT, &code,
							LM_DUP_NONE);
	if (status)
	{
		(void) printf("checkout %s: %s\n", feature,
							lc_errstring(lm_job));
		exit(status);
	}
	while (1)
	{
		if (status == 0)
			(void) strcpy(string, "OK\n");
		else if (status == LM_CANTCONNECT)
			(void) strcpy(string, "CANTCONNECT\n");
		else
			(void) strcpy(string, lc_errstring(lm_job));
		encrypt(string, strlen(string)-1);
		(void) printf(string);
		(void) fflush(stdout);
		errno = EINTR;
		while (errno == EINTR)
		{
			errno = 0;
			(void) gets(string);
		}
		(void) lc_timer(lm_job);
	}
}
static quit()  { exit(0); }
static reconn() { status = CANTCONNECT; }
static r_done() { status = 0; }
