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
 *	Description: 	Definitions of child process parameters
 *
 *	M. Christiano
 *	12/17/90
 *
 *	Last changed:  %G%
 *
 */
#include "pnopen.h"

#define MAXBUF 100
typedef struct pnopen *CHILD_HANDLE;

CHILD_HANDLE start_child();
