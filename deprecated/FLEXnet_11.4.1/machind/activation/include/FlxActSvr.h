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
 * FlxActServer include file
 *
 * The FlxActServer include file contains the object and function definitions
 * to be used as part of a client activation application.
 */

#ifndef FlxActSvr_h

#define FlxActSvr_h  1

#include "FlxActCommon.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * FlxActSvrActivation related operations
 */
typedef struct flxActSvrActivation * FlxActSvrActivation;

	/* initialization and cleanup functions */
FlxActBool
flxActSvrActivationCreate(
	FlxActHandle			handle,
	FlxActSvrActivation *	serverAct);

void
flxActSvrActivationDelete(FlxActSvrActivation serverAct);

/* key method to send request to remote server and get back response */
FlxActBool
flxActSvrActivationSend(
	FlxActSvrActivation serverAct,
	FlxActError *			err);

FlxActBool
flxActSvrActivationReqCreate(
	FlxActSvrActivation serverAct,
	char **				xmlString);

FlxActBool
flxActSvrActivationRespProcess(
	FlxActSvrActivation serverAct,
	const char *		xmlString,
	FlxActBool *		pbIsConfig);

void
flxActSvrActivationCountSet(
	FlxActSvrActivation	serverAct,
	FlxActCountType		type,
	uint32_t			count);

uint32_t
flxActSvrActivationCountGet(
	FlxActSvrActivation	serverAct,
	FlxActCountType		type);

const char *
flxActSvrActivationEntitlementIdGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationEntitlementIdSet(
	FlxActSvrActivation	serverAct,
	const char *		entitlementId);

const char *
flxActSvrActivationProductIdGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationProductIdSet(
	FlxActSvrActivation	serverAct,
	const char *		productId);

const char *
flxActSvrActivationExpDateGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationExpDateSet(
	FlxActSvrActivation	serverAct,
	const char *		expDate);

const char *
flxActSvrActivationUsernameGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationUsernameSet(
	FlxActSvrActivation serverAct,
	const char *		username);

const char *
flxActSvrActivationHostnameGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationHostnameSet(
	FlxActSvrActivation	serverAct,
	const char *		hostname);

FlxActBool
flxActSvrActivationIncludeEnterpriseData(
	FlxActSvrActivation	act,
	FlxActBool			bInclude);

void
flxActSvrActivationVendorDataSet(
	FlxActSvrActivation svr,
	const char *		key,
	const char *		value);

const char *
flxActSvrActivationVendorDataGet(
	FlxActSvrActivation	svr,
	const char *		key);


typedef struct flxActSvrReturn *	FlxActSvrReturn;


FlxActBool
flxActSvrReturnCreate(
	FlxActHandle			handle,
	FlxActSvrReturn *		serverRet);

void
flxActSvrReturnDelete(FlxActSvrReturn serverRet);

FlxActBool
flxActSvrReturnSend(
	FlxActSvrReturn	serverRet,
	FlxActError *		err);

FlxActBool
flxActSvrReturnReqCreate(
	FlxActSvrReturn	serverRet,
	char **			xmlString);

FlxActBool
flxActSvrReturnRespProcess(
	FlxActSvrReturn	serverRet,
	const char *	xmlString,
	FlxActBool *	pbIsConfig);

const char *
flxActSvrReturnProductIdGet(FlxActSvrReturn serverRet);

const char *
flxActSvrReturnSuiteIdGet(FlxActSvrReturn serverRet);

const char *
flxActSvrReturnEntitlementIdGet(FlxActSvrReturn serverRet);

const char *
flxActSvrReturnFRIdGet(FlxActSvrReturn serverRet);

uint32_t
flxActSvrReturnNumberValidDedSpcGet(FlxActSvrReturn serverRet);

void
flxActSvrReturnProdLicSpcSet(
	FlxActSvrReturn		ret,
	FlxActProdLicSpc	product);

FlxActBool
flxActSvrReturnForceIncompleteSet(
	FlxActSvrReturn		serverRet,
	FlxActBool			bForceIncomplete);

FlxActBool
flxActSvrReturnForceIncompleteGet(FlxActSvrReturn serverRet);

void flxActSvrReturnVendorDataSet(
	FlxActSvrReturn		ret,
	const char *		key,
	const char *		value);

const char *flxActSvrReturnVendorDataGet(FlxActSvrReturn ret, const char * key);

typedef struct flxActSvrRepair *	FlxActSvrRepair;

FlxActBool
flxActSvrRepairCreate(
	FlxActHandle		handle,
	FlxActSvrRepair *	serverRep);


void
flxActSvrRepairDelete(FlxActSvrRepair serverRep);

FlxActBool
flxActSvrRepairSend(
	FlxActSvrRepair		serverRep,
	FlxActError *		err);

FlxActBool
flxActSvrRepairReqCreate(
	FlxActSvrRepair	serverRep,
	char **			xmlString);

FlxActBool
flxActSvrRepairRespProcess(
	FlxActSvrRepair	serverRep,
	const char *	xmlString,
	FlxActBool *	pbIsConfig);

const char *flxActSvrRepairFRIdGet(FlxActSvrRepair serverRep);
const char *flxActSvrRepairProductIdGet(FlxActSvrRepair serverRep);
const char *flxActSvrRepairSuiteIdGet(FlxActSvrRepair serverRep);
const char *flxActSvrRepairEntitlementIdGet(FlxActSvrRepair serverRep);
void flxActSvrRepairProdLicSpcSet(FlxActSvrRepair serverRep, FlxActProdLicSpc product);

void
flxActSvrRepairVendorDataSet(
	FlxActSvrRepair		repair,
	const char *		key,
	const char *		value);

const char *flxActSvrRepairVendorDataGet(FlxActSvrRepair repair, const char * key);


#ifdef __cplusplus
}
#endif

#endif


