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
 *	Description: 	 lt_ API example: lt_checkout, lt_checkin and lt_errstring
 *
 *	J. Wong
 *	Jan 14, 2003
 *
 *	Last changed:  1/14/2003
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "lmpolicy.h"

#ifdef PC
#define LICPATH "license.dat;."
#endif /* PC */

#define FEATURE "f1"

int main(int argc, char * argv[])
{
#ifdef PC
	char feature[MAX_FEATURE_LEN * 2] = {'\0'};

	printf("Enter \"f1\" to demo floating functionality\n");
	printf("Enter \"f2\" to node-locked functionality\n");
	printf("Enter feature to checkout [default: \"%s\"]: ", FEATURE);

	fgets(feature, MAX_FEATURE_LEN + 2, stdin);

	if (*feature == '\n')
		strcpy(feature, FEATURE);
	else
		feature[strlen(feature) - 1] = '\0';

	if (lt_checkout(LM_RESTRICTIVE, feature, "1.0", LICPATH))
	{
		lt_perror("ERROR - checkout failed");
	}
	else
	{
		printf("%s checked out...press return to exit...", feature);
		getchar();
		lt_checkin();
	}
	/* cleanup memory for lt_ API calls */
#endif
	exit(0);
	return 0;
}
