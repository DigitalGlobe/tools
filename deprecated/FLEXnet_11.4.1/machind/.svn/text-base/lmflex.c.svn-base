/******************************************************************************

	    Copyright (c) 1988-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved
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
 *			the use of the Flexible License Manager.
 *
 */

#include "lmclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lm_attr.h"
#ifdef PC
#define LICPATH "@localhost"
#else
#define LICPATH "@localhost:license.dat:."
#endif /* PC */

#define FEATURE "f1"
VENDORCODE code;
LM_HANDLE *lm_job;
static void init(struct flexinit_property_handle **);
static void cleanup(struct flexinit_property_handle *);

int
main(int argc, char * argv[])
{
	char feature[MAX_FEATURE_LEN * 2] = {'\0'};
	char * output = NULL;
	struct flexinit_property_handle *initHandle;
	int stat;
	int nlic = 1;

	init(&initHandle);

	if (argc > 1)
	{
		nlic = atoi(argv[1]);
	}

	if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
	{
		lc_perror(lm_job, "lc_new_job failed");
		cleanup(initHandle);
		exit(lc_get_errno(lm_job));
	}

	printf("Enter \"f1\" to demo floating functionality\n");
	printf("Enter \"f2\" to node-locked functionality\n");
	printf("Enter feature to checkout [default: \"%s\"]: ", FEATURE);


	fgets(feature, MAX_FEATURE_LEN + 2, stdin);	/*	add 2 for \n and \0	*/
	feature[strlen(feature) - 1] = '\0';
	if(!*feature)
		strcpy(feature, FEATURE);

	(void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);

	if(lc_checkout(lm_job, feature, "1.0", nlic, LM_CO_NOWAIT, &code, LM_DUP_NONE))
	{
		lc_perror(lm_job, "checkout failed");
		cleanup(initHandle);
		exit (lc_get_errno(lm_job));
	}
	printf("%s checked out...", feature);
	printf("press return to exit...");

	/*
	*	Wait till user hits return
	*/
	getchar();
	lc_checkin(lm_job, feature, 0);
	lc_free_job(lm_job);
	cleanup(initHandle);
	return 0;
}

static void init(struct flexinit_property_handle **handle)
{
#ifndef NO_ACTIVATION_SUPPORT
	struct flexinit_property_handle *ourHandle;
	int stat;

	if (stat = lc_flexinit_property_handle_create(&ourHandle))
	{
		fprintf(stderr, "lc_flexinit_property_handle_create() failed: %d\n", stat);
		exit(1);
	}
	if (stat = lc_flexinit_property_handle_set(ourHandle,
			(FLEXINIT_PROPERTY_TYPE)FLEXINIT_PROPERTY_USE_TRUSTED_STORAGE,
			(FLEXINIT_VALUE_TYPE)1))
	{
		fprintf(stderr, "lc_flexinit_property_handle_set failed: %d\n", stat);
	    exit(1);
	}
	if (stat = lc_flexinit(ourHandle))
	{
		fprintf(stderr, "lc_flexinit failed: %d\n", stat);
	    exit(1);
	}
	*handle = ourHandle;
#endif /* NO_ACTIVATION_SUPPORT */
}

static void cleanup(struct flexinit_property_handle *initHandle)
{
#ifndef NO_ACTIVATION_SUPPORT
	int stat;

	if (stat = lc_flexinit_cleanup(initHandle))
	{
		fprintf(stderr, "lc_flexinit_cleanup failed: %d\n", stat);
	}
	if (stat = lc_flexinit_property_handle_free(initHandle))
	{
		fprintf(stderr, "lc_flexinit_property_handle_free failed: %d\n", stat);
	}
#endif /* NO_ACTIVATION_SUPPORT */
}
