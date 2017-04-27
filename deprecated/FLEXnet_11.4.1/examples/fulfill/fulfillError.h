/******************************************************************************

	    COPYRIGHT (c) 1989, 2003 by Macrovision Corporation.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of 
	Macrovision Corporation and is protected by law.  It may 
	not be copied or distributed in any form or medium, disclosed 
	to third parties, reverse engineered or used in any manner not 
	provided for in said License Agreement except with the prior 
	written authorization from Macrovision Corporation.

 *****************************************************************************/	
 /*	$Id: fulfillError.h 64 2007-12-13 19:20:30Z harsh.govind $ */
 /**
 *      @file           fulfillError.h
 *	@brief        	error codes returned by fulfill API.
 *      @version        $Revision: 64 $
 *****************************************************************************/

#ifndef _FULFILL_ERROR_H_
#define _FULFILL_ERROR_H_

#define FC_NO_ERROR             	0       /* no error */

/* gsoap error codes */
#define FC_SOAP_CLI_FAULT		1
#define FC_SOAP_SVR_FAULT		2
#define FC_SOAP_TAG_MISMATCH		3
#define FC_SOAP_TYPE_MISMATCH		4
#define FC_SOAP_SYNTAX_ERROR		5
#define FC_SOAP_NO_TAG			6
#define FC_SOAP_IOB			7
#define FC_SOAP_MUSTUNDERSTAND		8
#define FC_SOAP_NAMESPACE		9
#define FC_SOAP_OBJ_MISMATCH		10
#define FC_SOAP_FATAL_ERROR		11
#define FC_SOAP_FAULT			12
#define FC_SOAP_NO_METHOD		13
#define FC_SOAP_GET_METHOD		14
#define FC_SOAP_EOM			15
#define FC_SOAP_NULL			16
#define FC_SOAP_MULTI_ID		17
#define FC_SOAP_MISSING_ID		18
#define FC_SOAP_HREF			19
#define FC_SOAP_TCP_ERROR		20
#define FC_SOAP_HTTP_ERROR		21
#define FC_SOAP_SSL_ERROR		22
#define FC_SOAP_ZLIB_ERROR		23
#define FC_SOAP_DIME_ERROR		24
#define FC_SOAP_EOD			25
#define FC_SOAP_VERSIONMISMATCH		26
#define FC_SOAP_DIME_MISMATCH		27
#define FC_SOAP_PLUGIN_ERROR		28
#define FC_SOAP_DATAENCODINGUNKNOWN	29

#define FC_MISSING_PARAM        	32       /* missing parameter */
#define FC_INVALID_PARAM        	33       /* invalid parameter */
#define FC_INSUFF_ENTIT         	34       /* insufficient entitlement */
#define FC_VCG_ERROR            	35       /* vcg error */

#define FC_UNEXPECTED_ERROR           	40
#define FC_MEMORY_ALLOC_ERROR           41
#define FC_INVALID_HOSTID               42
#define FC_INVALID_ITEMID               43
#define FC_INVALID_CUSTOM_ATTRIBUTE     44
#define FC_AMOUNT_EXCEEDS_AVAIL         45
#define FC_MISSING_INPUT_QUANTITY       46
#define FC_MISSING_INPUT_DURATION       47
#define FC_EXPIRED_MAINT_DBV            48
#define FC_NO_DBV_FOUND                 49
#define FC_MAX_RETRY_FAILED             50

#endif




