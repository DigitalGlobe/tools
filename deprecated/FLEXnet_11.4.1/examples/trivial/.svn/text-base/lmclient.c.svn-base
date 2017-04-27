/******************************************************************************

	    Copyright (c) 1995-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
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
 *	Description: 	 Trivial API example: CHECKOUT, CHECKIN and ERRSTRING
 *
 *	D. Birns
 *	Jan 17, 1995
 *
 *	Last changed:  11/6/97
 *
 */
#define FEATURE "f1"

#include "lmpolicy.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef PC
#define LICPATH "license.dat;."
#else
#define LICPATH "@localhost:license.dat:."
#endif /* PC */




int
main(int argc, char * argv[])
{
	char feature[MAX_FEATURE_LEN * 2] = {'\0'};

	printf("Enter \"f1\" to demo floating functionality\n");
	printf("Enter \"f2\" to node-locked functionality\n");

	printf("Enter feature to checkout [default: \"%s\"]: ", FEATURE);


	fgets(feature, MAX_FEATURE_LEN + 2, stdin);

	if (*feature == '\n')
		strcpy(feature, FEATURE);
	else
		feature[strlen(feature) - 1] = '\0';

	if (CHECKOUT(LM_RESTRICTIVE, feature, "1.0", LICPATH))
	{
		PERROR("Checkout failed");
	}
	else
	{
		printf("%s checked out...press return to exit...", feature);
		getchar();
		CHECKIN();
	}
	exit(0);
	return 0;
}
