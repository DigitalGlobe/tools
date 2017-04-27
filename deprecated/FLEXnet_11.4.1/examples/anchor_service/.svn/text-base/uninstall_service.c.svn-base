/*****************************************************************************
	Copyright (c) 2006-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of
	Macrovision Corporation and is protected by law.
	It may 	not be copied or distributed in any form or medium, disclosed
	to third parties, reverse engineered or used in any manner not
	provided for in said License Agreement except with the prior
	written authorization from Macrovision Corporation.
*****************************************************************************/

#include <stdio.h>
#include "wininstaller.h"


static
void
sUsage()
{
	printf("Usage: uninstallanchorservice publisher_name product_name\n");
}

int
main(int argc, char * argv[])
{
	const char *	pszPublisherName = NULL;
	const char *	pszProductName = NULL;

	if(argc != 3)
	{
		sUsage();
		return 0;
	}

	pszPublisherName = argv[1];
	pszProductName = argv[2];

	if(!fnpActSvcUninstallWin(pszPublisherName, pszProductName))
	{
		printf("Error %d attempting to uninstall service\n", fnpActSvcGetLastErrorWin());
	}
	else
	{
		printf("Successfully uninstalled anchor service for publisher %s, product %s\n", pszPublisherName, pszProductName);
	}

	return 0;
}
