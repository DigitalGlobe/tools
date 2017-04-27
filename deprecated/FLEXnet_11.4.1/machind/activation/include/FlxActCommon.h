/***************************************************************
	    Copyright (c) 2005-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of
	Macrovision Corporation and is protected by law.  It may
	not be copied or distributed in any form or medium, disclosed
	to third parties, reverse engineered or used in any manner not
	provided for in said License Agreement except with the prior
	written authorization from Macrovision Corporation.
**************************************************************/

/**
 * FlxActCommon include file
 *
 * The FlxActCommon include file contains the function definitions for those
 * functions which are common to both the client and the server activation
 * applications.
 */

#ifndef FlxActCommon_h

#define FlxActCommon_h  1

#include "FlxActTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Setup the FlxActMode enumerated type. Used to specify the mode of operation when the
 * handle is created and initialized
 */
typedef enum flxActMode
{
	FLX_ACT_APP, FLX_ACT_SVR
} FlxActMode;


/**
 *	Reason for error response
 */
typedef enum flxErrorResponseReason
{
	flxErrorResponseReasonUnknown = 0,
	flxErrorResponseReasonError,
	flxErrorResponseReasonDenied
} FlxErrorResponseReason;

/**
 *	Unique Machine Number types
 */
typedef enum flxUMNType
{
	flxUMNTypeOne,
	flxUMNTypeTwo
} FlxUMNType;

/**
 * Setup the FlxActCountType enumerated type. Used to specify the different kinds of license
 * counts possible for each product
 */
typedef enum flxActCountType
{
	CONCURRENT,
	ACTIVATABLE,
	HYBRID,
	CONCURRENT_OD,
	ACTIVATABLE_OD,
	HYBRID_OD,
	REPAIRS
} FlxActCountType;

/**
 *	Enumeration for fulfillment types
 */
typedef enum flxFulfillmentType
{
	FULFILLMENT_TYPE_UNKNOWN = 0,
	FULFILLMENT_TYPE_TRIAL = 1,
	FULFILLMENT_TYPE_SHORTCODE,
	FULFILLMENT_TYPE_EMERGENCY,
	FULFILLMENT_TYPE_SERVED_ACTIVATION,				/*	From a vendor daemon */
	FULFILLMENT_TYPE_SERVED_OVERDRAFT_ACTIVATION,	/*	From a vendor daemon */
	FULFILLMENT_TYPE_PUBLISHER_ACTIVATION,
	FULFILLMENT_TYPE_PUBLISHER_OVERDRAFT_ACTIVATION
} FlxFulfillmentType;

/**
 *	Enumeration for deducion types
 */
typedef enum flxActDeductionType
{
	DEDUCTION_TYPE_UNKNOWN    = 0,
	DEDUCTION_TYPE_ACTIVATION = 1,
	DEDUCTION_TYPE_TRANSFER   = 2,
	DEDUCTION_TYPE_REPAIR     = 3
} FlxActDeductionType;

typedef enum flxActCommType
{
	flxCommsNone     = 0,
	flxCommsMvsnFlex = 1,
	flxCommsMvsnSoap = 4,
} FlxActCommType;

/*
 * Status change callback definition
 * SSC - Should be defined in one place.  Currently it is defined in fnptypes.h and here
 */
typedef uint32_t(*FLX_STATUS_CALLBACK)(const void* pUserData, uint32_t statusOld, uint32_t statusNew);

/*
 * Status related defs.
 * SSC - They are also defined in CommsDefs.h.  Should reuse the same header
 */
typedef enum flxCommsStatusCodes {
	flxCommsStatusError = 0,
	flxCommsStatusSuccess,
	flxCommsStatusNotSet,
	flxCommsStatusCancelledByUser,
    flxCommsStatusCreatingRequest,
    flxCommsStatusRequestCreated,
	flxCommsStatusContextCreated,
	flxCommsStatusConnected,
	flxCommsStatusRequestSent,
	flxCommsStatusPolingForResponse,
	flxCommsStatusWaitingForResponse,
	flxCommsStatusDone
} flxCommsStatusCodes;

typedef enum flxActCallbackReturn {
	flxCallbackReturnContinue = 0,
	flxCallbackReturnCancelRequest
} flxActCallbackReturn;

/*
 *	Initialization/cleanup routine
 */
int	flxActCommonLibraryInit(const char * runtimepath);
void flxActCommonLibraryCleanup();

/**
 * Define the FlxActHandle and related operations
 */
typedef struct flxActHandle * FlxActHandle;

	/* initialization */
	FlxActBool flxActCommonHandleOpen(FlxActHandle *handle, FlxActMode mode, FlxActError *err);

	/* cleanup */
	FlxActBool flxActCommonHandleClose(FlxActHandle handle);

	/* get back error code for last operation performed using handle */
	void flxActCommonHandleGetError(FlxActHandle handle, FlxActError *err);

	/* get back additional error info from operations server */
	uint32_t flxActCommonHandleGetLastOpsError(FlxActHandle handle);
	const char * flxActCommonHandleGetLastOpsErrorString(FlxActHandle handle);

	/* get last error code from last processed error response */
	uint32_t flxActCommonHandleGetLastResponseError(FlxActHandle handle);
	const char *flxActCommonHandleGetLastResponseErrorString(FlxActHandle handle);
	FlxErrorResponseReason flxActCommonHandleGetLastResponseErrorReason(FlxActHandle handle);

	/* UMN */
	const char * flxActCommonHandleGetUniqueMachineNumber(FlxActHandle handle, FlxUMNType type);


	/* set the remote server to be used for subsequent operations */
	FlxActBool flxActCommonHandleSetRemoteServer(FlxActHandle handle, const char *targetServer);

	const char * flxActCommonHandleGetRemoteServer(FlxActHandle handle);

	/* which comm to use */
	FlxActBool flxActCommonHandleSetCommType(FlxActHandle handle, FlxActCommType commType);

	FlxActCommType flxActCommonHandleGetCommType(FlxActHandle handle);

	/* timeout */
	FlxActBool flxActCommonHandleSetTimeout(FlxActHandle handle, uint32_t timeout);

	/* poll interval */
	FlxActBool flxActCommonHandleSetPollInterval(FlxActHandle handle, uint32_t pollInterval);

	/* Soap specific target details */
	/* proxy details */
	FlxActBool flxActCommonHandleSetProxyDetails(FlxActHandle handle, uint32_t port, const char* pHost, const char* pUserid, const char* pPasswd);

	/* SSL details */
	FlxActBool flxActCommonHandleSetSSLDetails(FlxActHandle handle, const char* pCacert, const char* pCapath);

	/* status callback */
	FlxActBool flxActCommonHandleSetStatusCallback(FlxActHandle handle, FLX_STATUS_CALLBACK callback, const void* pCallerData);

	/* delete specified product from all of trusted storage */
	FlxActBool flxActCommonHandleDeleteProduct(FlxActHandle handle, const char * pszProductId);

	FlxActBool flxActCommonRepairLocalTrustedStorage(FlxActHandle handle);

/**
 * Define the FlxActLicSpc and related operations. This represents the Flex Activation
 * License specification and the operations related to the population and retrieval of
 * defined license counts for products
 */
typedef struct flxActLicSpc * FlxActLicSpc;
typedef struct flxActProdLicSpc * FlxActProdLicSpc;
typedef struct flxActDedSpc * FlxActDedSpc;

	/* initialization */
	FlxActBool flxActCommonLicSpcCreate(FlxActHandle handle, FlxActLicSpc *licSpc);

	/* cleanup */
	void flxActCommonLicSpcDelete(FlxActLicSpc licSpc);

	FlxActBool flxActCommonLicSpcPopulateFromTS(FlxActLicSpc licSpc);
	FlxActBool flxActCommonLicSpcPopulateAllFromTS(FlxActLicSpc licSpc);
	FlxActBool flxActCommonLicSpcAddASRs(FlxActLicSpc licSpc, const char *filePath);
	FlxActBool flxActCommonLicSpcAddASRFromBuffer(FlxActLicSpc licSpc, const char * pszBuffer);

	FlxActBool flxActCommonResetTrialASRs(FlxActLicSpc licSpc, const char *filePath);
	FlxActBool flxActCommonResetTrialASRFromBuffer(FlxActLicSpc licSpc, const char * pszBuffer);

	uint32_t flxActCommonLicSpcGetNumberProducts(FlxActLicSpc licSpc);
	FlxActProdLicSpc flxActCommonLicSpcGet(FlxActLicSpc licSpc, uint32_t index);

	uint32_t flxActCommonLicSpcGetNumProdEntries(FlxActLicSpc licSpc, const char *productId);
	FlxActProdLicSpc flxActCommonLicSpcGetProd(FlxActLicSpc licSpc, const char *productId, uint32_t index);

	FlxActBool flxActCommonLicSpcProductDelete(FlxActLicSpc licSpc, FlxActProdLicSpc product);

/**
 * Object to define license specification attributes for a particular product
 */
	const char *flxActCommonProdLicSpcProductIdGet(FlxActProdLicSpc prodSpc);
	uint32_t flxActCommonProdLicSpcCountGet(FlxActProdLicSpc prodSpc, FlxActCountType countType);
	const char *flxActCommonProdLicSpcEntitlementIdGet(FlxActProdLicSpc prodSpc);
	const char *flxActCommonProdLicSpcFulfillmentIdGet(FlxActProdLicSpc prodSpc);
	const char *flxActCommonProdLicSpcExpDateGet(FlxActProdLicSpc prodSpc);
	const char *flxActCommonProdLicSpcSuiteIdGet(FlxActProdLicSpc prodSpc);
	const char * flxActCommonProdLicSpcFeatureLineGet(FlxActProdLicSpc prodSpc);
	uint32_t flxActCommonProdLicSpcTrustFlagsGet(FlxActProdLicSpc prodSpc);
	const char *flxActCommonProdLicSpcActServerChainGet(FlxActProdLicSpc prodSpc);
	FlxFulfillmentType flxActCommonProdLicSpcFulfillmentTypeGet(FlxActProdLicSpc prodSpc);
	FlxActBool flxActCommonProdLicSpcIsDisabled(FlxActProdLicSpc prodSpc);

	/* deductions */
	uint32_t		flxActCommonProdLicSpcNumberDedSpcGet(FlxActProdLicSpc prodSpc);
	FlxActDedSpc	flxActCommonProdLicSpcDedSpcGet(FlxActProdLicSpc prodSpc, uint32_t index);

	uint32_t		flxActCommonProdLicSpcNumberValidDedSpcGet(FlxActProdLicSpc prodSpc);

	const char *		flxActCommonDedSpcDestinationFulfillmentIdGet(FlxActDedSpc dedSpc);
	const char *		flxActCommonDedSpcDestinationSystemNameGet	(FlxActDedSpc dedSpc);
	const char *		flxActCommonDedSpcExpDateGet				(FlxActDedSpc dedSpc);
	FlxActDeductionType flxActCommonDedSpcTypeGet					(FlxActDedSpc dedSpc);
	uint32_t			flxActCommonDedSpcCountGet(FlxActDedSpc dedSpc, FlxActCountType countType);

	/*
	 *	Trust flags
	 */
	#define	FLAGS_TRUST_RESTORE		0x1		/* The restore trust (1 = trusted, 0 = untrusted) */
	#define	FLAGS_TRUST_HOST		0x2		/* The host trust (1 = trusted, 0 = untrusted) */
	#define FLAGS_TRUST_TIME		0x4		/* The time trust (1 = trusted, 0 = untrusted) */
	#define FLAGS_FULLY_TRUSTED		(FLAGS_TRUST_RESTORE | FLAGS_TRUST_HOST | FLAGS_TRUST_TIME)

#ifdef __cplusplus
}
#endif

#endif


