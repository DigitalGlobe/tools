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


/**	@file 	appActUtil.c
 *	@brief	Test activation application

 *
 *	This is the source code for a simple client/server activation application whose
 *	primary purpose is for internal testing.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "FlxActCommon.h"
#include "FlxActError.h"
#include "FlxActApp.h"
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
FlxActBool	g_bShortCodeCancel = FLX_ACT_FALSE;
FlxActBool	g_bShortCodePending = FLX_ACT_FALSE;
FlxActBool	g_bShortCode = FLX_ACT_FALSE;
FlxActBool	g_bFromBuffer = FLX_ACT_FALSE;
char *		g_pszASRFile = NULL;
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
char *		g_pszFRId = NULL;
void *		g_pMyData = 0;
uint32_t	g_dwDuration = 0;

/* proxy details */
char		g_pszProxyHost[128];
uint32_t	g_pszProxyPort;
char		g_pszProxyUserid[128];
char		g_pszProxyPassword[128];

/*
 *	Application defines
 */
#define OPTION_VIEW					"-view"
#define OPTION_LOCAL				"-local"
#define OPTION_SERVED				"-served"
#define OPTION_COMM					"-comm"
#define OPTION_COMM_SERVER			"-commServer"
#define OPTION_PROXY_DETAILS		"-proxyDetails"
#define OPTION_RETURN				"-return"
#define OPTION_PROCESS				"-process"
#define OPTION_GENERATE				"-gen"
#define OPTION_REPAIR				"-repair"
#define OPTION_ENTITLEMENT_ID		"-entitlementID"
#define OPTION_PRODUCT_ID			"-productID"
#define OPTION_EXPIRATION			"-expiration"
#define OPTION_DURATION				"-duration"
#define OPTION_VALID_ONLY			"-valid"
#define OPTION_BROKEN_ONLY			"-broken"
#define OPTION_LONG_VIEW			"-long"
#define OPTION_SHORT_CODE			"-shortcode"
#define OPTION_SHORT_CODE_PENDING	"-pending"
#define OPTION_SHORT_CODE_CANCEL	"-cancel"
#define OPTION_DELETE				"-delete"
#define OPTION_VENDOR_DATA			"-vendordata"
#define OPTION_DELETE_PRODUCT		"-delproduct"
#define OPTION_LOCAL_FROM_BUFFER	"-buffer"
#define OPTION_LOCAL_REPAIR			"-localrepair"
#define OPTION_UNIQUE_MACHINE_NUMBER	"-unique"
#define OPTION_TRIAL_RESET			"-reset"
#define DEFAULT_EXPIRATION			"31-dec-2007"
#define DEFAULT_ENTITLEMENT_ID		"served-123"

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

/* temporary define here */
#define DEBUG printf

typedef enum
{
	OP_TS_UNKNOWN = 0,
	OP_TS_VIEW = 1,
	OP_TS_LOCALACTIVATION,
	OP_TS_SERVEDACTIVATION,
	OP_TS_RETURN,
	OP_TS_PROCESS,
	OP_TS_REPAIR,
	OP_TS_SHORT_CODE_ACTIVATION,
	OP_TS_SHORT_CODE_REPAIR,
	OP_TS_SHORT_CODE_RETURN,
	OP_TS_DELETE,
	OP_TS_DELETE_PRODUCT,
	OP_TS_LOCAL_REPAIR,
	OP_TS_UNIQUE_MACHINE_NUMBER,
	OP_TS_TRIAL_RESET
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
	fnpActSvcInstallWin("demo", "appactutil");
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
	printf("Usage:\tappactutil -view [-valid | -broken] [-long]\n");
	printf("\tappactutil -local <asr_directory|asr_file> [-buffer]\n");
	printf("\tappactutil -served [-comm <flex|soap>]\n");
	printf("\t                   [-commServer <comm server>]\n");
	printf("\t                   [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                   [-entitlementID <entitlement_ID>]\n");
	printf("\t                   [-productID <product_ID>]\n");
	printf("\t                   [-expiration <expiration_date>]\n");
	printf("\t                   [-duration <duration_in_seconds>]\n");
	printf("\t                   [-vendordata <key> <value>]\n");
	printf("\t                   [-gen [<output_filename>]]\n");
	printf("\tappactutil -return <fulfillmentID>\n");
	printf("\t                   [-comm <flex|soap>]\n");
	printf("\t                   [-commServer <comm server>]\n");
	printf("\t                   [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                   [-gen [<output_filename>]]\n");
	printf("\t                   [-vendordata <key> <value>]\n");
	printf("\tappactutil -process <input_file>\n");
	printf("\tappactutil -repair <fulfillmentID> [-gen [<output_filename>]]\n");
	printf("\t                   [-comm <flex|soap>]\n");
	printf("\t                   [-commServer <comm server>]\n");
	printf("\t                   [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                   [-vendordata <key> <value>]\n");
	printf("\tappactutil -delete <fulfillmentID>\n");
	printf("\tappactutil -delproduct <productId>\n");
	printf("\tappactutil -localrepair\n");
	printf("\tappactutil -shortcode <ASR_file> [-repair | -return] [fulfillmentID]\n");
	printf("\tappactutil -shortcode <ASR_file> -pending\n");
	printf("\tappactutil -shortcode <ASR_file> -cancel\n");
	printf("\tappactutil -unique\n");
	printf("\tappactutil -reset <asr_directory|asr_file> [-buffer]\n");
}

/****************************************************************************/
/**	@brief	Status call back
 *
 ****************************************************************************/
uint32_t statusCallback(const void* pUserData, uint32_t statusOld, uint32_t statusNew)
{
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

	if(argc && argv )
	{
		for(i = 0; i < argc; i++)
		{
			if(0 == strcmp(argv[i], OPTION_LOCAL))
			{
				op = OP_TS_LOCALACTIVATION;
			}
			if(0 == strcmp(argv[i], OPTION_TRIAL_RESET))
			{
				op = OP_TS_TRIAL_RESET;
			}
			else if(0 == strcmp(argv[i], OPTION_SERVED))
			{
				op = OP_TS_SERVEDACTIVATION;
			}
			else if(0 == strcmp(argv[i], OPTION_RETURN))
			{
				if(FLX_ACT_TRUE == g_bShortCode)
					op = OP_TS_SHORT_CODE_RETURN;
				else
					op = OP_TS_RETURN;
				if((i + 1) < argc)
				{
					g_pszFRId = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_REPAIR))
			{
				if(FLX_ACT_TRUE == g_bShortCode)
					op = OP_TS_SHORT_CODE_REPAIR;
				else
					op = OP_TS_REPAIR;
				if((i + 1) < argc)
				{
					g_pszFRId = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_SHORT_CODE))
			{
				g_bShortCode = FLX_ACT_TRUE;
				/*
				 *	Determine what type of short code operation
				 */
				if(op == OP_TS_REPAIR)
				{
					op = OP_TS_SHORT_CODE_REPAIR;
				}
				else if(op == OP_TS_RETURN)
				{
					op = OP_TS_SHORT_CODE_RETURN;
				}
				else
				{
					/*
					 *	Default is activation
					 */
					op = OP_TS_SHORT_CODE_ACTIVATION;
				}
				if((i + 1) < argc)
				{
					g_pszASRFile = argv[++i];
				}
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
			else if(0 == strcmp(argv[i], OPTION_DURATION))
			{
				if((i + 1) < argc)
				{
					g_dwDuration = atoi(argv[++i]);
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
			else if(0 == strcmp(argv[i], OPTION_LONG_VIEW))
			{
				g_bLongView = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argv[i], OPTION_SHORT_CODE_PENDING))
			{
				if(FLX_ACT_TRUE == g_bShortCodeCancel)
				{
					sUsage();
					exit(1);
				}
				g_bShortCodePending = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argv[i], OPTION_SHORT_CODE_CANCEL))
			{
				if(FLX_ACT_TRUE == g_bShortCodePending)
				{
					sUsage();
					exit(1);
				}
				g_bShortCodeCancel = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argv[i], OPTION_DELETE))
			{
				op = OP_TS_DELETE;
			}
			else if(0 == strcmp(argv[i], OPTION_DELETE_PRODUCT))
			{
				op = OP_TS_DELETE_PRODUCT;
			}
			else if(0 == strcmp(argv[i], OPTION_VENDOR_DATA))
			{
				if((i + 2) < argc)
				{
					g_pszVendorDataKey = argv[++i];
                    g_pszVendorDataValue = argv[++i];
				}
			}
			else if(0 == strcmp(argv[i], OPTION_LOCAL_FROM_BUFFER))
			{
				g_bFromBuffer = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argv[i],  OPTION_LOCAL_REPAIR))
			{
				op = OP_TS_LOCAL_REPAIR;
			}
			else if(0 == strcmp(argv[i], OPTION_UNIQUE_MACHINE_NUMBER))
			{
				op = OP_TS_UNIQUE_MACHINE_NUMBER;
			}
		}
	}
	return op;
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



	if(product)
	{
		trustflags = flxActCommonProdLicSpcTrustFlagsGet(product);
		if(g_bBrokenOnly)
		{
			if(FLAGS_FULLY_TRUSTED == trustflags)
				return;
		}
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
		printf("Feature line(s):\n%s\n", pszFeatureLine ? pszFeatureLine : "NONE");

		if ( g_bLongView == FLX_ACT_TRUE )
		{
			pszActServerChain = flxActCommonProdLicSpcActServerChainGet(product);
			printf("Activation Server Chain: %s\n\n", pszActServerChain ? pszActServerChain : "NONE");
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
			/* set proxy server */
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
	FlxActAppActivation	client = 0;
	FlxActError			error;
	char *				pszOutput = NULL;
	const char *		pszOpsError = NULL;

	if(handle)
	{
		/*
		 *	Create a client handle
		 */
		if(FLX_ACT_FALSE == flxActAppActivationCreate(handle, &client))
		{
			DEBUG("ERROR: flxActAppActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		/*
		 *	If input parameters are NULL, check to see if environment variable set,
		 *	If not, then use defaults
		 */
		if(NULL == g_pszEntitlementID)
		{
			g_pszEntitlementID = getenv("ENTITLEMENT_ID");
		}
		if(NULL == g_pszExpiration)
		{
			g_pszExpiration = getenv("EXP_DATE");
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
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the activation request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActAppActivationVendorDataSet(client, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		flxActAppActivationEntitlementIdSet(client, g_pszEntitlementID ? g_pszEntitlementID : DEFAULT_ENTITLEMENT_ID);
		if(g_pszProductID)
		{
			flxActAppActivationProductIdSet(client, g_pszProductID);
		}
		/*
		 *	No expiration or duration set, use default expiration
		 */
		if((NULL == g_pszExpiration) && (0 == g_dwDuration))
		{
			flxActAppActivationExpDateSet(client, DEFAULT_EXPIRATION);
		}
		else
		{
			if(g_pszExpiration)
			{
				flxActAppActivationExpDateSet(client, g_pszExpiration);
			}
			else
			{
				flxActAppActivationDurationSet(client, g_dwDuration);
			}
		}

		setCommDetails(handle);

		/*
		 *	Specify what we're requesting...
		 */
		printf("Generating activation request using:\n\tEntitlement ID = %s\n\tExpiration = %s\n",
			g_pszEntitlementID ? g_pszEntitlementID : DEFAULT_ENTITLEMENT_ID,
			g_pszExpiration ? g_pszExpiration : DEFAULT_EXPIRATION);
		if(g_pszProductID)
		{
			printf("\tProduct ID = %s\n", g_pszProductID);
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed activation request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActAppActivationReqCreate(client, &pszOutput))
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
				DEBUG("ERROR: flxActAppActivationReqCreate\n");
			}
		}
		else
		{
			/*
			 *	Generate and send
			 */
			if(flxActAppActivationSend(client, &error))
			{
				DEBUG("ACTIVATION REQUEST SUCCESSFULLY PROCESSED\n");
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
					DEBUG("ERROR: flxActAppActivationSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
				}
			}
		}
		/*
		 *	Cleanup
		 */
		flxActAppActivationDelete(client);
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
	FlxActAppReturn		ret = 0;
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
		if(FLX_ACT_FALSE == flxActAppReturnCreate(handle, &ret))
		{
			DEBUG("ERROR: flxActAppReturnCreate\n");
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
			flxActAppReturnDelete(ret);
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
			flxActAppReturnDelete(ret);
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
					flxActAppReturnDelete(ret);
					return FLX_ACT_FALSE;
				}
				/*
				 *	Check to see if this is the one we want
				 */
				pszFulfillId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
				if(pszFulfillId && (0 == strcmp(pszFulfillId, fulfillId)))
				{
					flxActAppReturnProdLicSpcSet(ret, prodSpec);
					bFound = FLX_ACT_TRUE;
					break;
				}
			}
		}
		else
		{
			DEBUG("WARNING: No licenses in Trusted Storage\n");
			flxActCommonLicSpcDelete(licSpec);
			flxActAppReturnDelete(ret);
			return bRV;
		}

		if(FLX_ACT_FALSE == bFound)
		{
			DEBUG("ERROR: Unable to find fulfillment id %s\n", fulfillId);
			flxActCommonLicSpcDelete(licSpec);
			flxActAppReturnDelete(ret);
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
			flxActAppReturnVendorDataSet(ret, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed return request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActAppReturnReqCreate(ret, &pszOutput))
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
				DEBUG("ERROR: flxActAppReturnReqCreate - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
		}
		else
		{
			if(flxActAppReturnSend(ret, &error))
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
					DEBUG("ERROR: flxActAppActivationSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
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
		flxActAppReturnDelete(ret);

	}
cleanup:

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



static
FlxActBool
sHandleDeleteProductFromTS(
	FlxActHandle	handle,
	const char *	productId)
{
	if(handle)
	{
		return flxActCommonHandleDeleteProduct(handle, productId);
	}
	else
		return FLX_ACT_FALSE;

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
	FlxActAppRepair		repair = 0;
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
		if(FLX_ACT_FALSE == flxActAppRepairCreate(handle, &repair))
		{
			DEBUG("ERROR: flxActAppRepairCreate\n");
			return FLX_ACT_FALSE;
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
					flxActAppRepairProdLicSpcSet(repair, prodSpec);
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
			flxActAppRepairVendorDataSet(repair, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed return request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActAppRepairReqCreate(repair, &pszOutput))
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
				DEBUG("ERROR: flxActAppRepairReqCreate - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}

		}
		else
		{
			if(flxActAppRepairSend(repair, &error))
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
					DEBUG("ERROR: flxActAppActivationSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
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
		flxActAppRepairDelete(repair);

	}
cleanup:

	return bRV;
}

/****************************************************************************/
/**	@brief	Determine if path is a file or directory
 *
 *	@param	filePath	Path to determine if file or directory
 *
 *	@return	FLX_ACT_TRUE if directory (default) or FLX_ACT_FALSE if file.
 ****************************************************************************/
static
FlxActBool
sIsADirectory(const char * filePath, struct stat * pBuf)
{
	FlxActBool		bRV = FLX_ACT_TRUE;	/* default to true */

	if(filePath && pBuf)
	{
		if(0 == stat(filePath, pBuf))
		{
			if(!(pBuf->st_mode & S_IFDIR))
			{
				bRV = FLX_ACT_FALSE;
			}
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Process local activations
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	filePath	Path where the ASR files can be found
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sHandleLocalActivation(
	FlxActHandle	handle,
	const char *	filePath)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActLicSpc	licSpc = 0;
	FlxActError		error;
	FILE *			fp = NULL;
	char *			pszData = NULL;
	struct stat		buf;

	DEBUG("Loading ASR %s\n", filePath);
	if(handle)
	{
		/*
		 *	Repair broken segments (if any) without repairing broken records to
		 *	ensure that we can do the local activation.
		 */
		bRV = flxActCommonRepairLocalTrustedStorage(handle);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: flxActCommonRepairLocalTrustedStorage - (%d,%d,%d)\n",
				error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			return bRV;
		}

		bRV = flxActCommonLicSpcCreate(handle, &licSpc);
		if(bRV)
		{
			bRV = flxActCommonLicSpcPopulateFromTS(licSpc);
			if(bRV)
			{
				if(g_bFromBuffer)
				{
					/*
					 *	Determine if a file or directory, for buffer only support file
					 */
					if(sIsADirectory(filePath, &buf))
					{
						/*
						 *	Only support single file to now versus a directory
						 */
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						return FLX_ACT_FALSE;
					}
					fp = fopen(filePath, "r");
					if(NULL == fp)
					{
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						return FLX_ACT_FALSE;
					}
					pszData = malloc(sizeof(char) * (buf.st_size + 1));
					if(NULL == pszData)
					{
						fclose(fp);
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						return FLX_ACT_FALSE;
					}
					(void)fread(pszData, sizeof(char), buf.st_size, fp);
					fclose(fp);
					fp = NULL;

					bRV = flxActCommonLicSpcAddASRFromBuffer(licSpc, pszData);
					/*
					 *	Clean up buffer
					 */
					free(pszData);
					pszData = NULL;
					if(FLX_ACT_FALSE == bRV)
					{
						flxActCommonHandleGetError(handle, &error);
						DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						return bRV;
					}
				}
				else if(FLX_ACT_FALSE == flxActCommonLicSpcAddASRs(licSpc, filePath))
				{
					flxActCommonHandleGetError(handle, &error);
					DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
							error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
					return bRV;
				}
				DEBUG("Loaded AvailableASRs\n");
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
/**	@brief	Process trial reset asr(s)
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	filePath	Path where the ASR files can be found
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sHandleTrialReset(
	FlxActHandle	handle,
	const char *	filePath)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActLicSpc	licSpc = 0;
	FlxActError		error;
	FILE *			fp = NULL;
	char *			pszData = NULL;
	struct stat		buf;

	DEBUG("Resetting trial(s) with %s\n", filePath);
	if(handle)
	{
		/*
		 *	Repair broken segments (if any) without repairing broken records to
		 *	ensure that we can do the local activation.
		 */
		bRV = flxActCommonRepairLocalTrustedStorage(handle);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: flxActCommonRepairLocalTrustedStorage - (%d,%d,%d)\n",
				error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			return bRV;
		}

		bRV = flxActCommonLicSpcCreate(handle, &licSpc);
		if(bRV)
		{
			bRV = flxActCommonLicSpcPopulateFromTS(licSpc);
			if(bRV)
			{
				if(g_bFromBuffer)
				{
					/*
					 *	Determine if a file or directory, for buffer only support file
					 */
					if(sIsADirectory(filePath, &buf))
					{
						/*
						 *	Only support single file to now versus a directory
						 */
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						return FLX_ACT_FALSE;
					}
					fp = fopen(filePath, "r");
					if(NULL == fp)
					{
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						return FLX_ACT_FALSE;
					}
					pszData = malloc(sizeof(char) * (buf.st_size + 1));
					if(NULL == pszData)
					{
						fclose(fp);
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						return FLX_ACT_FALSE;
					}
					(void)fread(pszData, sizeof(char), buf.st_size, fp);
					fclose(fp);
					fp = NULL;

					bRV = flxActCommonResetTrialASRFromBuffer(licSpc, pszData);
					/*
					 *	Clean up buffer
					 */
					free(pszData);
					pszData = NULL;
					if(FLX_ACT_FALSE == bRV)
					{
						flxActCommonHandleGetError(handle, &error);
						DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						return bRV;
					}
				}
				else if(FLX_ACT_FALSE == flxActCommonResetTrialASRs(licSpc, filePath))
				{
					flxActCommonHandleGetError(handle, &error);
					DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
							error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
					return bRV;
				}
				DEBUG("Reset trials with available ASRs\n");
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
/**	@brief	Get pending short code if there is one pending
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sPendingShortCode(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActError			error;
	FlxActAppActivation	client = 0;
	const char *		pszPendingShortCode = NULL;

	if(handle && pszASRFile)
	{
		/*
		 *	Create an activation handle, doesn't matter if it's a
		 *	activation, return, or repair handle in the case
		 *	of a pending short code, all the necessary info is in
		 *	the ASR file and you can only have one pending short
		 *	code per a ASR.
		 */
		if(FLX_ACT_FALSE == flxActAppActivationCreate(handle, &client))
		{
			DEBUG("ERROR: flxActAppActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		/*
		 *	Get pending request size.
		 *	If we had a buffer, we could instead call flxActAppActivationShortCodePendingFromBuffer.
		 */
		bRV = flxActAppActivationShortCodePending(client, pszASRFile, &pszPendingShortCode);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			flxActAppActivationDelete(client);
			return FLX_ACT_FALSE;
		}
		/*
		 *	Check to see if there's anything pending
		 */
		if(NULL == pszPendingShortCode)
		{
			printf("No short code pending for ASR %s\n", pszASRFile);
			return FLX_ACT_FALSE;
		}
		printf("Pending short code for %s is %s\n", pszASRFile, pszPendingShortCode);
		if(client)
		{
			flxActAppActivationDelete(client);
		}
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	Cancel pending short code
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sCancelShortCode(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActError			error;
	FlxActAppActivation	client = 0;

	if(handle && pszASRFile)
	{
		/*
		 *	Create an activation handle, doesn't matter if it's a
		 *	activation, return, or repair handle in the case
		 *	of a pending short code, all the necessary info is in
		 *	the ASR file and you can only have one pending short
		 *	code per a ASR.
		 */
		if(FLX_ACT_FALSE == flxActAppActivationCreate(handle, &client))
		{
			DEBUG("ERROR: flxActAppActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		/*
		 *	Cancel request
		 *	If we had a buffer, we could instead call flxActAppActivationShortCodeCancelFromBuffer
		 */
		bRV = 	flxActAppActivationShortCodeCancel(client, pszASRFile);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		}
		else
		{
			printf("Successfully cancelled pending short code for %s\n", pszASRFile);
		}
		if(client)
		{
			flxActAppActivationDelete(client);
		}
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	Generate short code return request
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sGenShortCodeReturn(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActError		error;
	FlxActAppReturn	ret = 0;
	const char *	pszShortCode = NULL;

	if(handle && pszASRFile)
	{
		/*
		 *	Create a return
		 */
		bRV = flxActAppReturnCreate(handle, &ret);
		if(FLX_ACT_FALSE == ret)
		{
			DEBUG("ERROR: flxActAppReturnCreate\n");
			return FLX_ACT_FALSE;
		}

		if(g_pszFRId)
		{
			/*
			 *	Set the FR in TS to use to overwrite the one found in the ASR file
			 */
			flxActAppReturnFRIdSet(ret, g_pszFRId);
		}
		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the shortcode return request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActAppReturnVendorDataSet(ret, g_pszVendorDataKey, g_pszVendorDataValue);
		}
		/*
		 *	If we had a buffer, we could instead call flxActAppReturnShortCodeGenerateFromBuffer
		 */
		bRV = flxActAppReturnShortCodeGenerate(ret, pszASRFile, &pszShortCode);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			if(LM_TS_SHORT_CODE_PENDING == error.majorErrorNo)
			{
				printf("Error: Pending short code for %s\n", pszASRFile);
			}
			else if(LM_TS_SHORT_CODE_RETURN_CREATE == error.majorErrorNo)
			{
				printf("Error creating short code return %d %d %d\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			else
			{
				printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			flxActAppReturnDelete(ret);
			return FLX_ACT_FALSE;
		}
		printf("Return short code: %s\n", pszShortCode);
		/*
		 *	Cleanup
		 */
		if(ret)
		{
			flxActAppReturnDelete(ret);
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Generate short code repair request
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sGenShortCodeRepair(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActError		error;
	FlxActAppRepair	repair = 0;
	const char *	pszShortCode = NULL;

	if(handle && pszASRFile)
	{
		/*
		 *	Create a repair
		 */
		bRV = flxActAppRepairCreate(handle, &repair);
		if(FLX_ACT_FALSE == repair)
		{
			DEBUG("ERROR: flxActAppRepairCreate\n");
			return FLX_ACT_FALSE;
		}
		if(g_pszFRId)
		{
			/*
			 *	Set the FR in TS to use to overwrite the one found in the ASR file
			 */
			flxActAppRepairFRIdSet(repair, g_pszFRId);
		}
		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the shortcode repair request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActAppRepairVendorDataSet(repair, g_pszVendorDataKey, g_pszVendorDataValue);
		}
		/*
		 *	If we had a buffer, we could instead call flxActAppRepairShortCodeGenerateFromBuffer
		 */
		bRV = flxActAppRepairShortCodeGenerate(repair, pszASRFile, &pszShortCode);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			if(LM_TS_SHORT_CODE_PENDING == error.majorErrorNo)
			{
				printf("Error: Pending short code for %s\n", pszASRFile);
			}
			else if(LM_TS_SHORT_CODE_REPAIR_CREATE == error.majorErrorNo)
			{
				printf("Error creating short code repair %d %d %d\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			else
			{
				printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			flxActAppRepairDelete(repair);
			return FLX_ACT_FALSE;
		}
		printf("Repair short code: %s\n", pszShortCode);
		/*
		 *	Cleanup
		 */
		if(repair)
		{
			flxActAppRepairDelete(repair);
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Generate short code activation request
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sGenShortCodeActivation(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActError			error;
	FlxActAppActivation	act = 0;
	const char *		pszShortCode = NULL;

	if(handle && pszASRFile)
	{
		/*
		 *	Create a repair
		 */
		bRV = flxActAppActivationCreate(handle, &act);
		if(FLX_ACT_FALSE == act)
		{
			DEBUG("ERROR: flxActAppActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the shortcode activation request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActAppActivationVendorDataSet(act, g_pszVendorDataKey, g_pszVendorDataValue);
		}
		/*
		 *	If we had a buffer, we could instead call flxActAppActivationShortCodeGenerateFromBuffer
		 */
		bRV = flxActAppActivationShortCodeGenerate(act, pszASRFile, &pszShortCode);
		if(FLX_ACT_FALSE == bRV)
		{
			/*
			 *	Get error
			 */
			flxActCommonHandleGetError(handle, &error);
			if(LM_TS_SHORT_CODE_PENDING == error.majorErrorNo)
			{
				printf("Error: Pending short code for %s\n", pszASRFile);
			}
			else if(LM_TS_SHORT_CODE_ACT_CREATE == error.majorErrorNo)
			{
				printf("Error creating short code activation %d %d %d\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			else
			{
				printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			flxActAppActivationDelete(act);
			return FLX_ACT_FALSE;
		}
		printf("Activation short code: %s\n", pszShortCode);
		/*
		 *	Cleanup
		 */
		if(act)
		{
			flxActAppActivationDelete(act);
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Handle short codes
 *
 *	@param	handle	FlxActHandle to use to access trusted storage
 *	@param	op		Operation being requested
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sHandleShortCode(
	FlxActHandle	handle,
	OPERATION		op)
{
	FlxActBool	bRV = FLX_ACT_FALSE;

	if(NULL == g_pszASRFile)
	{
		sUsage();
		return FLX_ACT_FALSE;
	}
	if(handle)
	{
		/*
		 *	Determine what operation to do
		 */
		if(g_bShortCodePending)
		{
			bRV = sPendingShortCode(handle, g_pszASRFile);
		}
		else if(g_bShortCodeCancel)
		{
			bRV = sCancelShortCode(handle, g_pszASRFile);
		}
		else
		{
			if(OP_TS_SHORT_CODE_REPAIR == op)
			{
				bRV = sGenShortCodeRepair(handle, g_pszASRFile);
			}
			else if(OP_TS_SHORT_CODE_RETURN == op)
			{
				bRV = sGenShortCodeReturn(handle, g_pszASRFile);
			}
			else if(OP_TS_SHORT_CODE_ACTIVATION == op)
			{
				bRV = sGenShortCodeActivation(handle, g_pszASRFile);
			}
			else
			{
				printf("ERROR: Unsupported short code request\n");
			}
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Repair local trusted storage
 *
 *	@param	handle	FlxActHandle used to access trusted storage
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static
FlxActBool
sHandleLocalRepair(FlxActHandle handle)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActError		error;

	if(handle)
	{
		bRV = flxActCommonRepairLocalTrustedStorage(handle);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: Local repair - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		}
	}
	return bRV;
}

static
FlxActBool
sUniqueMachineNumber(FlxActHandle handle)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActError		error;
	const char *	pszUMN = NULL;

	if(handle)
	{
		pszUMN = flxActCommonHandleGetUniqueMachineNumber(handle, flxUMNTypeOne);
		if(pszUMN)
		{
			printf("Unique Machine Number type one is %s\n", pszUMN);
			pszUMN = flxActCommonHandleGetUniqueMachineNumber(handle, flxUMNTypeTwo);
			if(pszUMN)
			{
				printf("Unique Machine Number type two is %s\n", pszUMN);
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: Unique machine number - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
		}
		else
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: Unique machine number - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		}
	}
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
	FlxActAppActivation	client = 0;
	FlxActError			error;
	char *				pszInput = NULL;
	char				buffer[1024] = {'\0'};
	long				size = 0;
	long				bytesread = 0;

	if(handle)
	{
		/*
		 *	Create a client handle
		 */
		if(FLX_ACT_FALSE == flxActAppActivationCreate(handle, &client))
		{
			DEBUG("ERROR: flxActAppActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		printf("\nReading response from %s\n", g_pszInputFile ? g_pszInputFile : "stdin");
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
					pszInput[size] = '\0';	/* NULL terminate */
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
					/* NULL terminate */
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
			if(flxActAppActivationRespProcess(client, (const char *)pszInput, &bIsTrustedConfig))
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
				DEBUG("ERROR: Processing response - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
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
/**	@brief	Main entry point
 *
 *	@param	argc	Number of command line arguments
 *	@param	argv	Array of command line arguments
 *
 ****************************************************************************/
int
main(int argc, char * argv[])
{
	FlxActHandle			handle = 0;
	FlxActAppActivation		activation = 0;
	FlxActMode				mode = FLX_ACT_APP;
	FlxActError				error;
	void *					pData = NULL;
	OPERATION				operation = OP_TS_VIEW;

	if(argc < 2)
	{
		sUsage();
		exit(1);
	}

	/* install service if it's not already installed */
	sInstallService();

	memset(&error, 0, sizeof(FlxActError));

	/*
	 *	Figure out what operation we're performing and initialize necessary resources
	 */
	operation = sWhichOperation(argc, argv);

	/*
	 *	Initialize
	 */
	if(flxActCommonLibraryInit(NULL))
	{
		DEBUG("ERROR: Activation library initialization failed\n");
		return -1;
	}
	else
	{

		/*
		 *	Create handle and process.
		 */
		if(flxActCommonHandleOpen(&handle, mode, &error))
		{
			switch(operation)
			{
				case OP_TS_LOCALACTIVATION:
					if (argc < 3)
					{
						sUsage();
						exit(1);
					}
					sHandleLocalActivation(handle, argv[2]);
					break;

				case OP_TS_TRIAL_RESET:
					if (argc < 3)
					{
						sUsage();
						exit(1);
					}
					sHandleTrialReset(handle, argv[2]);
					break;

				case OP_TS_SERVEDACTIVATION:
					sHandleActivation(handle);
					break;

				case OP_TS_RETURN:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleReturn(handle, g_pszFRId);
					break;

				case OP_TS_REPAIR:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleRepair(handle, g_pszFRId);
					break;

				case OP_TS_DELETE:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleDelete(handle, argv[2]);
					break;

				case OP_TS_DELETE_PRODUCT:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleDeleteProductFromTS(handle, argv[2]);
					break;

				case OP_TS_LOCAL_REPAIR:
					(void)sHandleLocalRepair(handle);
					break;

				case OP_TS_PROCESS:
					(void)sHandleManualProcessing(handle);
					break;

				case OP_TS_SHORT_CODE_ACTIVATION:
				case OP_TS_SHORT_CODE_REPAIR:
				case OP_TS_SHORT_CODE_RETURN:
					(void)sHandleShortCode(handle, operation);
					break;

				case OP_TS_UNIQUE_MACHINE_NUMBER:
					(void)sUniqueMachineNumber(handle);
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




