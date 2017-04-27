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
 *
 *	Description: 	Test program to checkout a license using a subprocess
 *
 *	Parameters:	test feature number version
 *
 *	Return:		None
 *
 *	M. Christiano
 *	12/17/90
 *
 *	Last changed:  %G%
 *
 */
#include "lmclient.h"
#include "parent.h"

#define CHILD "/child"
extern double atof();

main(argc, argv)
int argc;
char *argv[];
{
  char progpath[LM_MAXPATHLEN];
  CHILD_HANDLE x;
  int i;

	if (argc < 3)
	{
		(void) printf("usage: test feature number version\n");
		exit(1);
	}
	getcwd(progpath, LM_MAXPATHLEN);
	strcat(progpath, CHILD);
	x = start_child(progpath, argv[1], atoi(argv[2]), atof(argv[3]));
	if (x)
	{
		if (check_child(x) != 0)
		{
			(void) printf("Never acquired license\n");
			exit(3);
		}
		while (1)
		{
			i = check_child(x);
			if (i == 1)
			{
				(void) printf("re-acquiring license\n");
			}
			else if (i == -1)
			{
				(void) printf("lost license\n");
				exit(4);
			}
			else
			{
				(void) printf("license OK\n");
			}
			sleep(2);
		}
	}
	else
	{
		(void) printf("startup of child failed\n");
		exit(2);
	}
}
