/*****************************************************************************
	Copyright (c) 2003-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of
	Macrovision Corporation and is protected by law.
	It may 	not be copied or distributed in any form or medium, disclosed
	to third parties, reverse engineered or used in any manner not
	provided for in said License Agreement except with the prior
	written authorization from Macrovision Corporation.
*****************************************************************************/


/**	@file 	serverActUtil.c
 *	@brief	Test server activation application

 *
 *	This is the source code for a simple server activation application whose
 *	primary purpose is for internal testing. This will probably become the sample
 *	server activation application to be shipped with the product.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FlxActCommon.h"
#include "FlxActError.h"
#include "FlxActSvr.h"
#ifdef WINDOWS
#include "wininstaller.h"
#endif /* WINDOWS */

/*
 *	Application globals
 */
FlxActBool	g_bGenerateOnly = FLX_ACT_FALSE;
FlxActBool	g_bValidOnly = FLX_ACT_FALSE;
FlxActBool	g_bBrokenOnly = FLX_ACT_FALSE;
FlxActBool	g_bLongView = FLX_ACT_FALSE;
char *		g_pszOutputFile = NULL;
char *		g_pszInputFile = NULL;
char *		g_pszEntitlementID = NULL;
char *		g_pszProductID = NULL;
char *		g_pszExpiration = NULL;
char *		g_pszComm = NULL;
char *		g_pszCommServer = NULL;
char *		g_pszProxyDetails = NULL;
char *		g_pszVendorDataKey = NULL;
char *		g_pszVendorDataValue = NULL;
void *		g_pMyData = 0;

/* proxy details */
char		g_pszProxyHost[128];
uint32_t	g_pszProxyPort;
char		g_pszProxyUserid[128];
char		g_pszProxyPassword[128];

uint32_t	g_dwActCount = (uint32_t)-1;
uint32_t	g_dwActOCount = (uint32_t)-1;
uint32_t	g_dwConCount = (uint32_t)-1;
uint32_t	g_dwConOCount = (uint32_t)-1;
uint32_t	g_dwHybCount = (uint32_t)-1;
uint32_t	g_dwHybOCount = (uint32_t)-1;
uint32_t	g_dwRepairCount = (uint32_t)-1;

/* to display status information */
const char* const COMMS_STATUS_STRINGS[] = {
	"Error",
	"Success",
	"Not set",
	"Cancelled by caller",
	"Creating request",
	"Request created",
	"Context created",
	"Connected to remote server",
	"Request Sent",
	"Polling for response",
	"Waiting for response",
	"Done"
};

/*
 *	Application defines
 */
#define OPTION_VIEW				"-view"
#define OPTION_SERVED			"-served"
#define OPTION_COMM				"-comm"
#define OPTION_COMM_SERVER		"-commServer"
#define OPTION_PROXY_DETAILS	"-proxyDetails"
#define OPTION_RETURN			"-return"
#define OPTION_PROCESS			"-process"
#define OPTION_REPAIR			"-repair"
#define OPTION_GENERATE			"-gen"
#define OPTION_ENTITLEMENT_ID	"-entitlementID"
#define OPTION_PRODUCT_ID		"-productID"
#define OPTION_EXPIRATION		"-expiration"
#define OPTION_ACT_COUNT		"-activatable"
#define OPTION_ACT_O_COUNT		"-activatableO"
#define OPTION_CON_COUNT		"-concurrent"
#define OPTION_CON_O_COUNT		"-concurrentO"
#define OPTION_HYB_COUNT		"-hybrid"
#define OPTION_HYB_O_COUNT		"-hybridO"
#define OPTION_REPAIRS			"-repairs"
#define OPTION_VALID_ONLY		"-valid"
#define OPTION_BROKEN_ONLY		"-broken"
#define OPTION_VENDOR_DATA		"-vendordata"
#define OPTION_LONG_VIEW		"-long"
#define OPTION_DELETE			"-delete"
#define DEFAULT_EXPIRATION		"31-dec-2007"
#define DEFAULT_ENTITLEMENT_ID	"served-123"
#define DEFAULT_ACT_COUNT		0
#define DEFAULT_ACT_O_COUNT		0
#define DEFAULT_CON_COUNT		0
#define DEFAULT_CON_O_COUNT		0
#define DEFAULT_HYB_COUNT		0
#define DEFAULT_HYB_O_COUNT		0
#define DEFAULT_REPAIR_COUNT	0

/* temporary define here */
#define DEBUG printf

typedef enum
{
	OP_TS_UNKNOWN = 0,
	OP_TS_VIEW = 1,
	OP_TS_SERVEDACTIVATION,
	OP_TS_RETURN,
	OP_TS_PROCESS,
	OP_TS_REPAIR,
	OP_TS_DELETE
} OPERATION;

/****************************************************************************/
/**	@brief	Installs anchor service
 *
 *	@param	None
 *
 *	@return	None
 *	@note	Only applies to windows
 ****************************************************************************/
static
void
sInstallService()
{
#ifdef WINDOWS
	fnpActSvcInstallWin("demo", "serveractutil");
#endif /* WINDOWS */
}

/****************************************************************************/
/**	@brief	Prints usage message
 *
 *	@param	None
 *
 *	@return	None
 ****************************************************************************/
static
void
sUsage()
{
	printf("Usage:\tserveractutil -view [-valid | -broken] [-long]\n");
	printf("\tserveractutil -served [-comm <flex|soap>]\n");
	printf("\t                      [-commServer <comm server>]\n");
	printf("\t                      [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                      [-entitlementID <entitlement_ID>]\n");
	printf("\t                      [-productID <product_ID>]\n");
	printf("\t                      [-expiration <expiration_date>]\n");
	printf("\t                      [-activatable <count>] [-activatableO <count>]\n");
	printf("\t                      [-concurrent <count>] [-concurrentO <count>]\n");
	printf("\t                      [-hybrid <count>] [-hybridO <count>]\n");
	printf("\t                      [-repairs <count>]\n");
	printf("\t                      [-gen [<output_filename>]]\n");
	printf("\t                      [-vendordata <key> <value>]\n");
	printf("\tserveractutil -return <fulfillmentID>\n");
	printf("\t                      [-comm <flex|soap>]\n");
	printf("\t                      [-commServer <comm server>]\n");
	printf("\t                      [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                      [-gen [<output_filename>]]\n");
	printf("\t                      [-vendordata <key> <value>]\n");
	printf("\tserveractutil -process <input_file>\n");
	printf("\tserveractutil -repair <fulfillmentID>\n");
	printf("\t                      [-comm <flex|soap>]\n");
	printf("\t                      [-commServer <comm server>]\n");
	printf("\t                      [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                      [-gen [<output_filename>]]\n");
	printf("\t                      [-vendordata <key> <value>]\n");
	printf("\tserveractutil -delete <fulfillmentID>\n");
}

/****************************************************************************/
/**	@brief	Status call back
 *
 ****************************************************************************/
uint32_t statusCallback(const void* pUserData, uint32_t statusOld, uint32_t statusNew) {
	printf("Status: %d, %s\n", statusNew, COMMS_STATUS_STRINGS[statusNew]);
	return flxCallbackReturnContinue;
}

/****************************************************************************/
/**	@brief	Determine which operation is being requested
 *
 *	@param	argc	Number of command line arguments
 *	@param	argv	Array of command line arguments
 *
 *	@return	Operation requested
 *	@note	No error handling is done in this routine, this is a sample only.
 ****************************************************************************/
static
OPERATION
sWhichOperation(
	int		argc,
	char *	argv[])
{
	OPERATION	op = OP_TS_VIEW;	/* default */
	uint32_t	i = 0;

	if(argc && argv)
	{
		for(i = 1; i < argc; i++)
		{
			if(0 == strcmp(argv[i], OPTION_SERVED))
			{
				op = OP_TS_SERVEDACTIVATION;
			}
			else if(0 == strcmp(argv[i], OPTION_COMM))
			{
				if((i + 1) < argc)
				{
					g_pszComm = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_COMM_SERVER))
			{
				if((i + 1) < argc)
				{
					g_pszCommServer = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_PROXY_DETAILS))
			{
				if((i + 1) < argc)
				{
					g_pszProxyDetails = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_RETURN))
			{
				op = OP_TS_RETURN;
			}
			else if(0 == strcmp(argv[i], OPTION_REPAIR))
			{
				op = OP_TS_REPAIR;
			}
			else if(0 == strcmp(argv[i], OPTION_PROCESS))
			{
				op = OP_TS_PROCESS;
				if((i + 1) < argc)
				{
					g_pszInputFile = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_GENERATE))
			{
				g_bGenerateOnly = FLX_ACT_TRUE;
				if((i + 1) < argc)
				{
					g_pszOutputFile = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_ENTITLEMENT_ID))
			{
				if((i + 1) < argc)
				{
					g_pszEntitlementID = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_PRODUCT_ID))
			{
				if((i + 1) < argc)
				{
					g_pszProductID = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_EXPIRATION))
			{
				if((i + 1) < argc)
				{
					g_pszExpiration = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_ACT_COUNT))
			{
				if((i + 1) < argc)
				{
					g_dwActCount = atoi(argv[++i]);
				}
			}
			else if(0 == strcmp(argv[i], OPTION_ACT_O_COUNT))
			{
				if((i + 1) < argc)
				{
					g_dwActOCount = atoi(argv[++i]);
				}
			}
			else if(0 == strcmp(argv[i], OPTION_CON_COUNT))
			{
				if((i + 1) < argc)
				{
					g_dwConCount = atoi(argv[++i]);
				}
			}
			else if(0 == strcmp(argv[i], OPTION_CON_O_COUNT))
			{
				if((i + 1) < argc)
				{
					g_dwConOCount = atoi(argv[++i]);
				}
			}
			else if(0 == strcmp(argv[i], OPTION_HYB_COUNT))
			{
				if((i + 1) < argc)
				{
					g_dwHybCount = atoi(argv[++i]);
				}
			}
			else if(0 == strcmp(argv[i], OPTION_HYB_O_COUNT))
			{
				if((i + 1) < argc)
				{
					g_dwHybOCount = atoi(argv[++i]);
				}
			}
			else if(0 == strcmp(argv[i], OPTION_REPAIRS))
			{
				if((i + 1) < argc)
				{
					g_dwRepairCount = atoi(argv[++i]);
				}
			}
			else if(0 == strcmp(argv[i], OPTION_VALID_ONLY))
			{
				if(g_bBrokenOnly)
				{
					sUsage();
					exit(1);
				}
				g_bValidOnly = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argv[i], OPTION_BROKEN_ONLY))
			{
				if(g_bValidOnly)
				{
					sUsage();
					exit(1);
				}
				g_bBrokenOnly = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argv[i], OPTION_VENDOR_DATA))
			{
				if((i + 2) < argc)
				{
					g_pszVendorDataKey = argv[++i];
                    g_pszVendorDataValue = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_LONG_VIEW))
			{
				g_bLongView = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argv[i], OPTION_DELETE))
			{
				op = OP_TS_DELETE;
			}
		}
	}
	return op;
}

/****************************************************************************/
/**	@brief	Format and output deduction specs of a specific FlxActProdLicSpc.
 *
 *	@param	prod	The FlxActProdLicSpc to output
 *
 *	@return	None
 ****************************************************************************/
static
void
sOutputDedSpc(FlxActDedSpc dedSpc)
{
	unsigned int		concurrent		= 0;
	unsigned int		concurrentO		= 0;
	unsigned int		hybrid			= 0;
	unsigned int		hybridO			= 0;
	unsigned int		activatable		= 0;
	unsigned int		activatableO	= 0;
	unsigned int		repairs			= 0;

	const char *		pszDestinationFulfillmentId	= NULL;
	const char *		pszDestinationSystemName	= NULL;
	const char *		pszExpDate					= NULL;

	FlxActDeductionType	type = FULFILLMENT_TYPE_UNKNOWN;


	if ( dedSpc == NULL )
	{
		return;
	}

	type = flxActCommonDedSpcTypeGet(dedSpc);

	pszDestinationFulfillmentId	= flxActCommonDedSpcDestinationFulfillmentIdGet(dedSpc);
	pszDestinationSystemName	= flxActCommonDedSpcDestinationSystemNameGet(dedSpc);
	pszExpDate					= flxActCommonDedSpcExpDateGet(dedSpc);

	concurrent		= flxActCommonDedSpcCountGet(dedSpc, CONCURRENT);
	concurrentO		= flxActCommonDedSpcCountGet(dedSpc, CONCURRENT_OD);
	hybrid			= flxActCommonDedSpcCountGet(dedSpc, HYBRID);
	hybridO			= flxActCommonDedSpcCountGet(dedSpc, HYBRID_OD);
	activatable		= flxActCommonDedSpcCountGet(dedSpc, ACTIVATABLE);
	activatableO	= flxActCommonDedSpcCountGet(dedSpc, ACTIVATABLE_OD);
	repairs			= flxActCommonDedSpcCountGet(dedSpc, REPAIRS);

	printf("-------------------------------\n");

	switch ( type )
	{
		case DEDUCTION_TYPE_ACTIVATION:
			printf("Deduction Type: ACTIVATION\n");
			printf("Destination Fulfillment ID: %s\n",
					pszDestinationFulfillmentId ? pszDestinationFulfillmentId : "NONE");
			printf("Destination System Name: %s\n",
					pszDestinationSystemName ? pszDestinationSystemName : "NONE");
			printf("Expiration Date: %s\n", pszExpDate ? pszExpDate : "NONE");
			printf("Hybrid: %d\n", hybrid);
			printf("Activatable: %d\n",	activatable);
			break;


		case DEDUCTION_TYPE_TRANSFER:
			printf("Deduction Type: TRANSFER\n");
			printf("Destination Fulfillment ID: %s\n",
					pszDestinationFulfillmentId ? pszDestinationFulfillmentId : "NONE");
			printf("Destination System Name: %s\n",
					pszDestinationSystemName ? pszDestinationSystemName : "NONE");
			printf("Expiration Date: %s\n", pszExpDate ? pszExpDate : "NONE");
			printf("Concurrent: %d\n",				concurrent);
			printf("Concurrent overdraft: %d\n",	concurrentO);
			printf("Hybrid: %d\n",					hybrid);
			printf("Hybrid overdraft: %d\n",		hybridO);
			printf("Activatable: %d\n",				activatable);
			printf("Activatable overdraft: %d\n",	activatableO);
			printf("Repairs: %d\n",					repairs);
			break;


		case DEDUCTION_TYPE_REPAIR:
			printf("Deduction Type: REPAIR\n");
			printf("Destination Fulfillment ID: %s\n",
					pszDestinationFulfillmentId ? pszDestinationFulfillmentId : "NONE");
			printf("Destination System Name: %s\n",
					pszDestinationSystemName ? pszDestinationSystemName : "NONE");
			break;


		case DEDUCTION_TYPE_UNKNOWN:
		default:
			printf("Deduction Type: UNKNOWN\n");
	}

    printf("\n");
	return;
}

/****************************************************************************/
/**	@brief	Format and output contents of a specific FlxActProdLicSpc.
 *
 *	@param	prod	The FlxActProdLicSpc to output
 *
 *	@return	None
 ****************************************************************************/
static
void
sOutputProdLic(FlxActProdLicSpc product)
{
	unsigned int		concurrent = 0;
	unsigned int		concurrentO = 0;
	unsigned int		hybrid = 0;
	unsigned int		hybridO = 0;
	unsigned int		activatable = 0;
	unsigned int		activatableO = 0;
	unsigned int		repairs = 0;
	const char *		pszProductID = NULL;
	const char *		pszFulfillmentID = NULL;
	const char *		pszEntitlementID = NULL;
	const char *		pszSuiteID = NULL;
	const char *		pszExpDate = NULL;
	const char *		pszFeatureLine = NULL;
	const char *		pszActServerChain = NULL;
	uint32_t			trustflags = 0;
	char				szTrustFlags[256] = {'\0'};
	static char			oneshot = 0;
	FlxActBool			bIsDisabled = FLX_ACT_TRUE;
	FlxFulfillmentType	type = FULFILLMENT_TYPE_UNKNOWN;
	char *				szType[] = {"UNKNOWN", "TRIAL", "SHORTCODE",  "EMERGENCY", \
									"SERVED ACTIVATION", "SERVED OVERDRAFT ACTIVATION", \
									"PUBLISHER ACTIVATION", "PUBLISHER OVERDRAFT ACTIVATION"};
	FlxActDedSpc		dedSpc = NULL;
	uint32_t			dedCount = 0;
	uint32_t			validDedCount = 0;
	uint32_t			ii = 0;


	if(product)
	{
		trustflags = flxActCommonProdLicSpcTrustFlagsGet(product);
		if(g_bBrokenOnly)
		{
			if(FLAGS_FULLY_TRUSTED == trustflags)
			{
				return;
			}
		}
		concurrent = flxActCommonProdLicSpcCountGet(product, CONCURRENT);
		concurrentO = flxActCommonProdLicSpcCountGet(product, CONCURRENT_OD);
		hybrid = flxActCommonProdLicSpcCountGet(product, HYBRID);
		hybridO = flxActCommonProdLicSpcCountGet(product, HYBRID_OD);
		activatable = flxActCommonProdLicSpcCountGet(product, ACTIVATABLE);
		activatableO = flxActCommonProdLicSpcCountGet(product, ACTIVATABLE_OD);
		repairs = flxActCommonProdLicSpcCountGet(product, REPAIRS);
		pszProductID = flxActCommonProdLicSpcProductIdGet(product);
		pszFulfillmentID = flxActCommonProdLicSpcFulfillmentIdGet(product);
		pszEntitlementID = flxActCommonProdLicSpcEntitlementIdGet(product);
		pszSuiteID = flxActCommonProdLicSpcSuiteIdGet(product);
		pszExpDate = flxActCommonProdLicSpcExpDateGet(product);
		pszFeatureLine = flxActCommonProdLicSpcFeatureLineGet(product);
		bIsDisabled = flxActCommonProdLicSpcIsDisabled(product);
		type = flxActCommonProdLicSpcFulfillmentTypeGet(product);

		if(0 == oneshot)
		{
			printf("\n--------------------------------------------------------------------\n");
			oneshot = 1;
		}
		if(trustflags == FLAGS_FULLY_TRUSTED)
		{
			printf("Trust Flags: FULLY TRUSTED\n");
		}
		else
		{
			if(!(FLAGS_TRUST_RESTORE & trustflags))
			{
				strcpy(szTrustFlags, "RESTORE");
			}
			if(!(FLAGS_TRUST_HOST & trustflags))
			{
				if(szTrustFlags[0] == '\0')
				{
					strcpy(szTrustFlags, "HOST");
				}
				else
				{
					strcat(szTrustFlags, ",HOST");
				}
			}
			if(!(FLAGS_TRUST_TIME & trustflags))
			{
				if(szTrustFlags[0] == '\0')
				{
					strcpy(szTrustFlags, "TIME");
				}
				else
				{
					strcat(szTrustFlags, ",TIME");
				}
			}
			printf("Trust Flags: **BROKEN** %s\n", szTrustFlags);
		}
		printf("Fulfillment Type: %s\n",
			(type <= FULFILLMENT_TYPE_PUBLISHER_OVERDRAFT_ACTIVATION) ? szType[type] : szType[0]);
		printf("Status: %s\n", bIsDisabled ? "**DISABLED**" : "ENABLED");
		printf("Fulfillment ID: %s\n", pszFulfillmentID ? pszFulfillmentID : "NONE");
		printf("Entitlement ID: %s\n", pszEntitlementID ? pszEntitlementID : "NONE");
		printf("Product ID: %s\n", pszProductID ? pszProductID : "NONE");
		printf("Suite ID: %s\n", pszSuiteID ? pszSuiteID : "NONE");
		printf("Expiration date: %s\n", pszExpDate ? pszExpDate : "NONE");
		printf("Concurrent: %d\n", concurrent);
		printf("Concurrent overdraft:%d\n", concurrentO);
		printf("Hybrid: %d\n", hybrid);
		printf("Hybrid overdraft: %d\n", hybridO);
		printf("Activatable: %d\n", activatable);
		printf("Activatable overdraft: %d\n", activatableO);
		printf("Repairs: %d\n", repairs);
		printf("Feature line(s):\n%s\n", pszFeatureLine ? pszFeatureLine : "NONE");

		if ( g_bLongView == FLX_ACT_TRUE )
		{
			pszActServerChain = flxActCommonProdLicSpcActServerChainGet(product);
			dedCount = flxActCommonProdLicSpcNumberDedSpcGet(product);
			validDedCount = flxActCommonProdLicSpcNumberValidDedSpcGet(product);

			printf("Activation Server Chain: %s\n", pszActServerChain ? pszActServerChain : "NONE");
			printf("Deduction Count: %d\n", dedCount);
			printf("Valid Deduction Count: %d\n\n", validDedCount);

			for ( ii = 0; ii < dedCount; ii++ )
			{
				sOutputDedSpc(flxActCommonProdLicSpcDedSpcGet(product, ii));
			}
		}
		printf("--------------------------------------------------------------------\n");
	}
	return;
}

/****************************************************************************/
/**	@brief	Display the contents of trusted storage
 *
 *	@param	handle	FlxActHandle to use to access trusted storage
 *
 *	@return	FLX_ACT_TRUE if anything is found else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sDumpTS(FlxActHandle handle)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActLicSpc		licSpc = 0;
	FlxActProdLicSpc	prodSpec = 0;
	uint32_t			count = 0;
	uint32_t			i = 0;

	if(handle)
	{
		/*
		 *	Dump contents of trusted storage
		 */
		bRV = flxActCommonLicSpcCreate(handle, &licSpc);
		if(bRV)
		{
			if(g_bValidOnly)
				bRV = flxActCommonLicSpcPopulateFromTS(licSpc);
			else
                bRV = flxActCommonLicSpcPopulateAllFromTS(licSpc);
			if(bRV)
			{
				count = flxActCommonLicSpcGetNumberProducts(licSpc);
				if(0 == count)
				{
					printf("No fulfillment records in trusted storage\n");
				}
				else
				{
					bRV = FLX_ACT_TRUE;
					for(i = 0; i < count; i++)
					{
						prodSpec = flxActCommonLicSpcGet(licSpc, i);
						if(prodSpec)
						{
							sOutputProdLic(prodSpec);
						}
					}
				}
			}

			/*
			 *	Cleanup
			 */
			flxActCommonLicSpcDelete(licSpc);
		}
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	Set parameters to use in activation (transfer) request
 *
 *	@param	server		Server activation object
 *
 *	@return	None
 *	@note	If not specified on commandline, check to see if any environment
 *			variables are set to override the default.
 ****************************************************************************/
static
void
sSetActivatonRequestParameters(FlxActSvrActivation server)
{
	char *	pszActivate = NULL;
	char *	pszOActivate = NULL;
	char *	pszCon = NULL;
	char *	pszOCon = NULL;
	char *	pszHyb = NULL;
	char *	pszOHyb = NULL;
	char *	pszRepairCount = NULL;

	if(NULL == g_pszEntitlementID)
		g_pszEntitlementID = getenv("ENTITLEMENT_ID");
	if(NULL == g_pszExpiration)
		g_pszExpiration = getenv("EXP_DATE");

	if (NULL == g_pszCommServer)
	{
		g_pszCommServer = getenv("COMM_SERVER");
		if (NULL == g_pszCommServer)
		{
			g_pszCommServer = getenv("LM_LICENSE_FILE");
			if(NULL == g_pszCommServer)
			{
				g_pszCommServer = "@localhost";
			}
		}
	}

	if(((uint32_t)-1) == g_dwActCount)
	{
		pszActivate = getenv("ACTIVATABLE");
		g_dwActCount = pszActivate ? atoi(pszActivate) : DEFAULT_ACT_COUNT;
	}
	if(((uint32_t)-1) == g_dwActOCount)
	{
		pszOActivate = getenv("OVERDRAFT_ACTIVATABLE");
		g_dwActOCount = pszOActivate ? atoi(pszOActivate) : DEFAULT_ACT_O_COUNT;
	}
	if(((uint32_t)-1) == g_dwConCount)
	{
		pszCon = getenv("CONCURRENT");
		g_dwConCount = pszCon ? atoi(pszCon) : DEFAULT_CON_COUNT;
	}
	if(((uint32_t)-1) == g_dwConOCount)
	{
		pszOCon = getenv("OVERDRAFT_CONCURRENT");
		g_dwConOCount = pszOCon ? atoi(pszOCon) : DEFAULT_CON_O_COUNT;
	}
	if(((uint32_t)-1) == g_dwHybCount)
	{
		pszHyb = getenv("HYBRID");
		g_dwHybCount = pszHyb ? atoi(pszHyb) : DEFAULT_HYB_COUNT;
	}
	if(((uint32_t)-1) == g_dwHybOCount)
	{
		pszOHyb = getenv("OVERDRAFT_HYBRID");
		g_dwHybOCount = pszOHyb ? atoi(pszOHyb) : DEFAULT_HYB_O_COUNT;
	}
	if(((uint32_t)-1) == g_dwRepairCount)
	{
		pszRepairCount = getenv("REPAIRS");
		g_dwRepairCount = pszRepairCount ? atoi(pszRepairCount) : DEFAULT_REPAIR_COUNT;
	}

	flxActSvrActivationEntitlementIdSet(server, g_pszEntitlementID ? g_pszEntitlementID : DEFAULT_ENTITLEMENT_ID);
	if(g_pszProductID)
	{
		flxActSvrActivationProductIdSet(server, g_pszProductID);
	}
	flxActSvrActivationExpDateSet(server, g_pszExpiration ? g_pszExpiration : DEFAULT_EXPIRATION);
	flxActSvrActivationCountSet(server, ACTIVATABLE, g_dwActCount);
	flxActSvrActivationCountSet(server, ACTIVATABLE_OD, g_dwActOCount);
	flxActSvrActivationCountSet(server, CONCURRENT, g_dwConCount);
	flxActSvrActivationCountSet(server, CONCURRENT_OD, g_dwConOCount);
	flxActSvrActivationCountSet(server, HYBRID, g_dwHybCount);
	flxActSvrActivationCountSet(server, HYBRID_OD, g_dwHybOCount);
	flxActSvrActivationCountSet(server, REPAIRS, g_dwRepairCount);

	printf("Generating transfer request using:\n");
	printf("\tEntitlement ID = %s\n", g_pszEntitlementID ? g_pszEntitlementID : DEFAULT_ENTITLEMENT_ID);
	if(g_pszProductID)
	{
		printf("\tProduct ID = %s\n", g_pszProductID);
	}
	printf("\tExpiration = %s\n", g_pszExpiration ? g_pszExpiration : DEFAULT_EXPIRATION);
	printf("\tActivatable Count = %d\n", g_dwActCount);
	printf("\tActivatable Overdraft Count = %d\n", g_dwActOCount);
	printf("\tConcurrent Count = %d\n", g_dwConCount);
	printf("\tConcurrent Overdraft Count = %d\n", g_dwConOCount);
	printf("\tHybrid Count = %d\n", g_dwHybCount);
	printf("\tHybrid Overdraft Count = %d\n", g_dwHybOCount);
	printf("\tRepair Count = %d\n", g_dwRepairCount);
}

/****************************************************************************/
/**	@brief	To set comm type and comm server
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *
 *	@return	void
 ****************************************************************************/
static
void
setCommDetails(FlxActHandle	handle)
{
	flxActCommonHandleSetRemoteServer(handle, g_pszCommServer);

	if (g_pszComm != NULL && !strcmp(g_pszComm, "soap")) {
		flxActCommonHandleSetCommType(handle, flxCommsMvsnSoap);
		flxActCommonHandleSetStatusCallback(handle, &statusCallback, g_pMyData);

		if (g_pszProxyDetails) {
			/*set proxy server */
			sscanf(g_pszProxyDetails, "%s %d %s %s", g_pszProxyHost,
													 &g_pszProxyPort,
													 g_pszProxyUserid,
													 g_pszProxyPassword);
			flxActCommonHandleSetProxyDetails(handle,
											g_pszProxyPort,   /* (uint32_t) proxy_port, */
											g_pszProxyHost,   /* proxy_host, */
											g_pszProxyUserid,           /* proxy_userName, */
											g_pszProxyPassword);           /* proxy_password); */
		}

	}
	else {
		flxActCommonHandleSetCommType(handle, flxCommsMvsnFlex);
	}
}

/****************************************************************************/
/**	@brief	Handle end to end server transfer
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sHandleActivation(FlxActHandle	handle)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActSvrActivation	server = 0;
	FlxActError			error;
	char *				pszOutput = NULL;
	const char *		pszOpsError = NULL;

	if(handle)
	{
		/*
		 *	Create a client handle
		 */
		if(FLX_ACT_FALSE == flxActSvrActivationCreate(handle, &server))
		{
			DEBUG("ERROR: flxActSvrActivationCreate\n");
			return FLX_ACT_FALSE;
		}

		sSetActivatonRequestParameters(server);

		setCommDetails(handle);

		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the activation request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActSvrActivationVendorDataSet(server, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed activation request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActSvrActivationReqCreate(server, &pszOutput))
			{
				bRV = FLX_ACT_TRUE;
				if(g_pszOutputFile)
				{
					FILE *	fp = NULL;

					fp = fopen(g_pszOutputFile, "w+");
					if(fp)
					{
						fwrite(pszOutput, strlen(pszOutput), 1, fp);
						fclose(fp);
					}
					else
					{
						DEBUG("ERROR: Unable to open file %s\n", g_pszOutputFile);
					}
				}
				else
				{
					/*
					 *	Output to stdout
					 */
					printf("%s\n", pszOutput);
				}
				if(pszOutput)
				{
					free(pszOutput);
					pszOutput = NULL;
				}
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActSvrActivationReqCreate - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
		}
		else
		{
			/*
			 *	Generate and send
			 */
			if(flxActSvrActivationSend(server, &error))
			{
				bRV = FLX_ACT_TRUE;
				DEBUG("TRANSFER REQUEST SUCCESSFULLY PROCESSED\n");
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				if(LM_TS_OPERATIONS == error.majorErrorNo)
				{
					pszOpsError = flxActCommonHandleGetLastOpsErrorString(handle);
					if(pszOpsError)
					{
						printf("Operations error: %d %s\n", flxActCommonHandleGetLastOpsError(handle), pszOpsError);
					}
				}
				else
				{
					DEBUG("ERROR: flxActSvrActivationSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
				}
			}
		}
		/*
		 *	Cleanup
		 */
		flxActSvrActivationDelete(server);
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	Handle return of a fulfillment.
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *	@param	fulfillID	FulfillmentID to return
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static
FlxActBool
sHandleReturn(
	FlxActHandle	handle,
	const char *	fulfillId)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActBool			bFound = FLX_ACT_FALSE;
	FlxActSvrReturn		ret = 0;
	FlxActError			error;
	FlxActLicSpc		licSpec = 0;
	FlxActProdLicSpc	prodSpec = 0;
	unsigned int		count = 0;
	unsigned int		i = 0;
	const char *		pszEntitlementId = NULL;
	const char *		pszProdId = NULL;
	const char *		pszFulfillId = NULL;
	const char *		pszSuiteId = NULL;
	const char *		pszFRUniqueId = NULL;
	const char *		pszOpsError = NULL;
	char *				pszOutput = NULL;

	if(handle && fulfillId)
	{
		/*
		 *	Create a return
		 */
		if(FLX_ACT_FALSE == flxActSvrReturnCreate(handle, &ret))
		{
			DEBUG("ERROR: flxActSvrReturnCreate\n");
			return FLX_ACT_FALSE;
		}

		if (NULL == g_pszCommServer)
		{
			g_pszCommServer = getenv("COMM_SERVER");
			if (NULL == g_pszCommServer)
			{
				g_pszCommServer = getenv("LM_LICENSE_FILE");
				if(NULL == g_pszCommServer)
				{
					g_pszCommServer = "@localhost";
				}
			}
		}

		/*
		 *	Find out which one we want to return
		 */
		bRV = flxActCommonLicSpcCreate(handle, &licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			DEBUG("ERROR: flxActCommonLicSpcCreate\n");
			return bRV;
		}
		/*
		 *	Populate from TS
		 */
		bRV = flxActCommonLicSpcPopulateFromTS(licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			DEBUG("ERROR: flxActCommonLicSpcPopulateFromTS\n");
			flxActCommonLicSpcDelete(licSpec);
			return bRV;
		}

		/*
		 *	Figure out how many there are and which one we want
		 */
		count = flxActCommonLicSpcGetNumberProducts(licSpec);
		if(count)
		{
			for(i = 0; i < count; i++)
			{
				prodSpec = flxActCommonLicSpcGet(licSpec, i);
				if(0 == prodSpec)
				{
					DEBUG("ERROR: flxActCommonLicSpcGet - Invalid index\n");
					flxActCommonLicSpcDelete(licSpec);
					return FLX_ACT_FALSE;
				}
				/*
				 *	Check to see if this is the one we want
				 */
				pszFulfillId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
				if(pszFulfillId && (0 == strcmp(pszFulfillId, fulfillId)))
				{
					flxActSvrReturnProdLicSpcSet(ret, prodSpec);
					bFound = FLX_ACT_TRUE;
					break;
				}
			}
		}
		else
		{
			DEBUG("WARNING: No licenses in Trusted Storage\n");
			flxActCommonLicSpcDelete(licSpec);
			return bRV;
		}

		if(FLX_ACT_FALSE == bFound)
		{
			DEBUG("ERROR: Unable to find fulfillment id %s\n", fulfillId);
			return bRV;
		}

		setCommDetails(handle);

		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the return request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActSvrReturnVendorDataSet(ret, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed return request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActSvrReturnReqCreate(ret, &pszOutput))
			{
				bRV = FLX_ACT_TRUE;
				if(g_pszOutputFile)
				{
					FILE *	fp = NULL;

					fp = fopen(g_pszOutputFile, "w+");
					if(fp)
					{
						fwrite(pszOutput, strlen(pszOutput), 1, fp);
						fclose(fp);
					}
					else
					{
						DEBUG("ERROR: fopen\n");
					}
				}
				else
				{
					/*
					 *	Output to stdout
					 */
					printf("%s\n", pszOutput);
				}
				if(pszOutput)
				{
					free(pszOutput);
					pszOutput = NULL;
				}
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActSvrReturnReqCreate - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}

		}
		else
		{
			if(flxActSvrReturnSend(ret, &error))
			{
				DEBUG("SUCCESSFULLY SENT RETURN REQUEST\n");
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				if(LM_TS_OPERATIONS == error.majorErrorNo)
				{
					pszOpsError = flxActCommonHandleGetLastOpsErrorString(handle);
					if(pszOpsError)
					{
						printf("Operations error: %d %s\n", flxActCommonHandleGetLastOpsError(handle), pszOpsError);
					}
				}
				else
				{
					DEBUG("ERROR: flxActSvrReturnSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
				}
			}
		}

		/*
		 *	Cleanup
		 */
		if(licSpec)
		{
			flxActCommonLicSpcDelete(licSpec);
		}
		flxActSvrReturnDelete(ret);

	}
cleanup:

	return bRV;
}

/****************************************************************************/
/**	@brief	Process XML
 *
 *	@param	handle	FlxActHandle used to access trusted storage
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 *	@note	Primary use if for manual transactions
 ****************************************************************************/
static
FlxActBool
sHandleManualProcessing(FlxActHandle handle)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActBool			bIsTrustedConfig = FLX_ACT_FALSE;
	FlxActSvrActivation	server = 0;
	FlxActError			error;
	char *				pszInput = NULL;
	char				buffer[1024] = {'\0'};
	long				size = 0;
	long				bytesread = 0;

	if(handle)
	{
		/*
		 *	Create a server handle
		 */
		if(FLX_ACT_FALSE == flxActSvrActivationCreate(handle, &server))
		{
			DEBUG("ERROR: flxActSvrActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		printf("\nReading request from %s\n", g_pszInputFile ? g_pszInputFile : "stdin");
		if(g_pszInputFile)
		{
			FILE *	fp = NULL;

			fp = fopen(g_pszInputFile, "rb");
			if(fp)
			{
				(void)fseek(fp, 0L, SEEK_END);
				size = ftell(fp);
				(void)fseek(fp, 0L, SEEK_SET);
				pszInput = malloc(sizeof(char) * (size + 1));
				if(pszInput)
				{
					(void)fread(pszInput, sizeof(char), size, fp);
					pszInput[size] = '\0';
					fclose(fp);
					fp = NULL;
				}
				else
				{
					DEBUG("ERROR: malloc\n");
					return FLX_ACT_FALSE;
				}
			}
			else
			{
				DEBUG("ERROR: fopen\n");
				return FLX_ACT_FALSE;
			}
		}
		else
		{
			/*
			 *	Read input from stdin
			 */
			bytesread = read(0, &buffer, sizeof(buffer));
            while(bytesread)
            {
				size += bytesread;
				pszInput = realloc(pszInput, size + 1);
				if(pszInput)
				{
					memcpy(&(pszInput[size - bytesread]), buffer, bytesread);
					pszInput[size] = '\0';
				}
				else
				{
					DEBUG("ERROR: malloc\n");
					return FLX_ACT_FALSE;
				}
				bytesread = read(0, &buffer, sizeof(buffer));
			}
		}
		if(pszInput)
		{
			if(flxActSvrActivationRespProcess(server, (const char *)pszInput, &bIsTrustedConfig))
			{
				if(FLX_ACT_TRUE == bIsTrustedConfig)
				{
					DEBUG("SUCCESSFULLY PROCESSED TRUSTED CONFIG RESPONSE, RESUBMIT REQUEST\n");
				}
				else
				{
					DEBUG("SUCCESSFULLY PROCESSED RESPONSE\n");
				}
				bRV = FLX_ACT_TRUE;
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActSvrActivationRespProcess - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			free(pszInput);
			pszInput = NULL;
		}
		else
		{
			DEBUG("ERROR: NO INPUT\n");
		}
	}

	return bRV;

}

/****************************************************************************/
/**	@brief	Delete fulfillment whose ID matches
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *	@param	fulfillID	FulfillmentID to delete
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static
FlxActBool
sHandleDelete(
	FlxActHandle	handle,
	const char *	fulfillId)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActLicSpc		licSpc = 0;
	FlxActProdLicSpc	prodSpec = 0;
	FlxActError			error;
	uint32_t			count = 0;
	uint32_t			i = 0;
	const char *		pszFRID = NULL;

	if(handle)
	{
		/*
		 *	Create a license spec and populate it with all FR's
		 */
		bRV = flxActCommonLicSpcCreate(handle, &licSpc);
		if(bRV)
		{
			if(g_bValidOnly)
                bRV = flxActCommonLicSpcPopulateFromTS(licSpc);
			else
                bRV = flxActCommonLicSpcPopulateAllFromTS(licSpc);
			if(bRV)
			{
				count = flxActCommonLicSpcGetNumberProducts(licSpc);
				if(0 == count)
				{
					printf("No fulfillment records in trusted storage\n");
				}
				else
				{
					for(i = 0; i < count; i++)
					{
						prodSpec = flxActCommonLicSpcGet(licSpc, i);
						if(prodSpec)
						{
							/*
							 *	Check to see if this is the one we're interested in
							 */
							pszFRID = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
							if(pszFRID && (0 == strcmp(pszFRID, fulfillId)))
							{
								/*
								 *	Delete this FR
								 */
								bRV = flxActCommonLicSpcProductDelete(licSpc, prodSpec);
								if(FLX_ACT_FALSE == bRV)
								{
									flxActCommonHandleGetError(handle, &error);
									DEBUG("ERROR: flxActCommonLicSpcProductDelete - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
								}
								else
								{
									printf("Successfully deleted fulfillment %s\n", fulfillId);
								}
								break;
							}
						}
					}
				}
			}

			/*
			 *	Cleanup
			 */
			flxActCommonLicSpcDelete(licSpc);
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Handle repair of a fulfillment.
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *	@param	fulfillID	FulfillmentID to repair
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static
FlxActBool
sHandleRepair(
	FlxActHandle	handle,
	const char *	fulfillId)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActBool			bFound = FLX_ACT_FALSE;
	FlxActSvrRepair		repair = 0;
	FlxActError			error;
	FlxActLicSpc		licSpec = 0;
	FlxActProdLicSpc	prodSpec = 0;
	uint32_t			trustflags = 0;
	unsigned int		count = 0;
	unsigned int		i = 0;
	const char *		pszEntitlementId = NULL;
	const char *		pszProdId = NULL;
	const char *		pszFulfillId = NULL;
	const char *		pszSuiteId = NULL;
	const char *		pszFrUniqueId = NULL;
	const char *		pszOpsError = NULL;
	char *				pszOutput = NULL;
	uint32_t			tid = 0;

	if(handle && fulfillId)
	{
		/*
		 *	Create a return
		 */
		if(FLX_ACT_FALSE == flxActSvrRepairCreate(handle, &repair))
		{
			DEBUG("ERROR: flxActSvrRepairCreate\n");
			return FLX_ACT_FALSE;
		}

		if (NULL == g_pszCommServer)
		{
			g_pszCommServer = getenv("COMM_SERVER");
			if (NULL == g_pszCommServer)
			{
				g_pszCommServer = getenv("LM_LICENSE_FILE");
				if(NULL == g_pszCommServer)
				{
					g_pszCommServer = "@localhost";
				}
			}
		}
		/*
		 *	Find out which one we want to return
		 */
		bRV = flxActCommonLicSpcCreate(handle, &licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			DEBUG("ERROR: flxActCommonLicSpcCreate\n");
			return bRV;
		}
		/*
		 *	Populate from TS
		 */
		bRV = flxActCommonLicSpcPopulateAllFromTS(licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonLicSpcDelete(licSpec);
			return bRV;
		}

		/*
		 *	Figure out how many there are and which one we want
		 */
		count = flxActCommonLicSpcGetNumberProducts(licSpec);
		if(count)
		{
			for(i = 0; i < count; i++)
			{
				prodSpec = flxActCommonLicSpcGet(licSpec, i);
				if(0 == prodSpec)
				{
					DEBUG("ERROR: flxActCommonLicSpcGet - Invalid index\n");
					flxActCommonLicSpcDelete(licSpec);
					return FLX_ACT_FALSE;
				}
				/*
				 *	Check to see if this is trusted or not
				 */
				if(FLAGS_FULLY_TRUSTED == flxActCommonProdLicSpcTrustFlagsGet(prodSpec))
				{
					/*
					 *	Fully trusted, no need to do repair
					 */
					continue;
				}
				/*
				 *	Check to see if this is the one we want
				 */
				pszFulfillId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
				if(pszFulfillId && (0 == strcmp(pszFulfillId, fulfillId)))
				{
					flxActSvrRepairProdLicSpcSet(repair, prodSpec);
					bFound = FLX_ACT_TRUE;
					break;
				}
			}
		}
		else
		{
			DEBUG("WARNING: No licenses in Trusted Storage\n");
			flxActCommonLicSpcDelete(licSpec);
			return bRV;
		}

		if(FLX_ACT_FALSE == bFound)
		{
			DEBUG("ERROR: Unable to find fulfillment id %s\n", fulfillId);
			return bRV;
		}

		setCommDetails(handle);

		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the repair request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActSvrRepairVendorDataSet(repair, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed return request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActSvrRepairReqCreate(repair, &pszOutput))
			{
				bRV = FLX_ACT_TRUE;
				if(g_pszOutputFile)
				{
					FILE *	fp = NULL;

					fp = fopen(g_pszOutputFile, "w+");
					if(fp)
					{
						fwrite(pszOutput, strlen(pszOutput), 1, fp);
						fclose(fp);
					}
					else
					{
						DEBUG("ERROR: fopen\n");
					}
				}
				else
				{
					/*
					 *	Output to stdout
					 */
					printf("%s\n", pszOutput);
				}
				if(pszOutput)
				{
					free(pszOutput);
					pszOutput = NULL;
				}
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActSvrRepairReqCreate - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}

		}
		else
		{
			if(flxActSvrRepairSend(repair, &error))
			{
				DEBUG("SUCCESSFULLY SENT REPAIR REQUEST\n");
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				if(LM_TS_OPERATIONS == error.majorErrorNo)
				{
					pszOpsError = flxActCommonHandleGetLastOpsErrorString(handle);
					if(pszOpsError)
					{
						printf("Operations error: %d %s\n", flxActCommonHandleGetLastOpsError(handle), pszOpsError);
					}
				}
				else
				{
					DEBUG("ERROR: flxActSvrRepairSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
				}
			}
		}

		/*
		 *	Cleanup
		 */
		if(licSpec)
		{
			flxActCommonLicSpcDelete(licSpec);
		}
		flxActSvrRepairDelete(repair);

	}
cleanup:

	return bRV;
}

/****************************************************************************/
/**	@brief	Main entry point
 *
 *	@param	argc	Number of command line arguments
 *	@param	argv	Array of command line arguments
 *
 ****************************************************************************/
int
main(int argc, char * argv[])
{
	FlxActHandle				handle = 0;
	FlxActMode					mode = FLX_ACT_SVR;
	FlxActError					error;
	void *						pData = NULL;
	OPERATION					operation = OP_TS_VIEW;

	if(argc < 2)
	{
		sUsage();
		exit(1);
	}

	/* install service if it's not already installed */
	sInstallService();

	memset(&error, 0, sizeof(FlxActError));

	/*
	 *	Handle processing command line arguments, whatever they are.
	 *	Default mode is for this to function as a client activation application.
	 */

	/*
	 *	Figure out what operation we're performing and initialize necessary resources
	 */
	operation = sWhichOperation(argc, argv);

	if(flxActCommonLibraryInit(NULL))
	{
		DEBUG("ERROR: Activation library initialization failed\n");
		return -1;
	}
	else
	{
		/*
		 *	Create handle
		 */

		if(flxActCommonHandleOpen(&handle, mode, &error))
		{
			switch(operation)
		{
			case OP_TS_SERVEDACTIVATION:
				sHandleActivation(handle);
				break;

			case OP_TS_RETURN:
				if (argc < 3)
				{
					sUsage();
					exit(1);
				}
				(void)sHandleReturn(handle, argv[2]);
				break;

			case OP_TS_REPAIR:
				if(argc < 3)
				{
					sUsage();
					exit(1);
				}
				(void)sHandleRepair(handle, argv[2]);
				break;

			case OP_TS_DELETE:
				if(argc < 3)
				{
					sUsage();
					exit(1);
				}
				(void)sHandleDelete(handle, argv[2]);
				break;

			case OP_TS_PROCESS:
				(void)sHandleManualProcessing(handle);
				break;

			case OP_TS_VIEW:
				case OP_TS_UNKNOWN:
			default:
				sDumpTS(handle);
				break;
		}

		/*
		 *	Cleanup
		 */
		flxActCommonHandleClose(handle);
	}
	else
	{
			DEBUG("ERROR: flxActCommonHandleOpen - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		}
		flxActCommonLibraryCleanup();
	}
	return 0;
}




