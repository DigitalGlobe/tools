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
 *	Function:	start_child(path, feature, number, version)
 *				- starts a child process.
 *			check_child(handle)
 *				- Checks the license that the child holds
 *			kill_child(handle)
 *				- Kills the child, thereby checking the
 *				  license back in.
 *
 *	Parameters:	(char *) path - Pathname to child process
 *			(char *) feature - feature to check out
 *			(int) number - Number of licenses to checkout
 *			(double) version - Version of feature
 *			(CHILD_HANDLE) handle - Handle for child
 *
 *	Return:		start_child() returns CHILD_HANDLE
 *			check_child() returns 0 for OK, 1 for re-acquiring
 *						license, -1 for no license
 *			kill_child() returns nothing
 *
 *	M. Christiano
 *	12/17/90
 *
 *	Last changed:  %G%
 *
 */
#include "lmclient.h"
#include "pnopen.h"
#include "parent.h"
#include <stdio.h>

CHILD_HANDLE
start_child(path, feature, number, version)
char *path;	/* Path to child process */
char *feature;	/* Which feature */
int number;	/* # of licenses */
double version;	/* Version of feature */
{
  struct pnopen *x;
  char buf[MAXBUF];
  int len;

	x = pnopen(path, "wr");
	if (x)
	{
		(void) sprintf(buf, "checkout %s %d %lf\n", feature, number,
								version);
		len = strlen(buf);
		encrypt(buf, len-1);
		fwrite(buf, 1, len, x->files[0]);
		fflush(x->files[0]);
	}
	return(x);
}

check_child(handle)
CHILD_HANDLE handle;
{
  char buf[MAXBUF];
  int n;
  int ret = 0;

	(void) fgets(buf, MAXBUF, handle->files[1]);
	n = strlen(buf);
	if (n <= 0) { perror("fgets from child"); return(-1); }
	decrypt(buf, n-1);
	n = fwrite("\n", 1, 1, handle->files[0]);
	if (n <= 0) { perror("fwrite to child"); return(-1); }
	fflush(handle->files[0]);
	if (!strcmp(buf, "OK\n")) ret = 0;
	else if (!strcmp(buf, "CANTCONNECT\n")) ret = 1;
	else ret = -1;
	return(ret);
}

kill_child(handle)
CHILD_HANDLE handle;
{
	kill(handle->pid);
}
