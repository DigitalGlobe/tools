/***************************************************************
	Copyright (c) 2006-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
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
 * FlxActBorrow
 *
 * The FlxActBorrow include file contains the object and function definitions
 * used to support client side activation.
 */

#ifndef FlxActBorrow_h
#define FlxActBorrow_h  1

#include "FlxActCommon.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef enum flxActBorrowStatusAttribute
{
	kActBorrowStatusAttributeUnknown = 0,
	kActBorrowStatusAttributeEntitlementId,	/*	Entitlement ID */
	kActBorrowStatusAttributeProductId,		/*	Product ID */
	kActBorrowStatusAttributeFulfillmentId,	/*	Fulfillment ID */
	kActBorrowStatusAttributeExpiration		/*	Expiration */
} FlxActBorrowStatusAttribute;




/**
 * FlxActivation
 */
typedef struct flex_act_borrow_context *	FlxActBorrowContext;

int
flxActBorrowTSViewCreate(
	FlxActBorrowContext *	pContext,
	int *					pFulfillmentCount);

void
flxActBorrowTSViewDelete(FlxActBorrowContext context);

int
flxActBorrowTSViewFRAttributeGet(
	FlxActBorrowContext		context,
	int							index,
	FlxActBorrowStatusAttribute	attribute,
	char *						pszBuffer,
	int *						pLen);

int
flxActBorrowReturn(
	const char *	pszFulfillmentId,
	const char *	pszServer,
	FlxActError *	pError);

int
flxActBorrowActivate(
	const char *	pszEntitlementId,
	const char *	pszProductId,
	const char *	pszExpiration,
	const char *	pszServer,
	char *			pszFulfillmentId,
	int				length,
	FlxActError *	pError);

#ifdef __cplusplus
}
#endif

#endif /* FlxActivation_h */



