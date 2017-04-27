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
 *	FlexAct error codes
 */


#ifndef FlxActError_h
#define FlxActError_h	1

#define	LM_TS_BASE_ERROR		50000
#define LM_TS_BADPARAM			(LM_TS_BASE_ERROR+1)
#define LM_TS_CANTMALLOC		(LM_TS_BASE_ERROR+2)
#define LM_TS_OPEN_ERROR		(LM_TS_BASE_ERROR+3)
#define LM_TS_CLOSE_ERROR		(LM_TS_BASE_ERROR+4)
#define LM_TS_INIT				(LM_TS_BASE_ERROR+5)
#define LM_TS_ACT_GEN_REQ		(LM_TS_BASE_ERROR+6)
#define LM_TS_RETURN_GEN_REQ	(LM_TS_BASE_ERROR+7)
#define LM_TS_REPAIR_GEN_REQ	(LM_TS_BASE_ERROR+8)
#define LM_TS_RESERVED_9		(LM_TS_BASE_ERROR+9)
#define LM_TS_DELETE			(LM_TS_BASE_ERROR+10)
#define LM_TS_CANT_FIND			(LM_TS_BASE_ERROR+11)
#define LM_TS_INTERNAL_ERROR	(LM_TS_BASE_ERROR+12)
#define LM_TS_RESERVED_13		(LM_TS_BASE_ERROR+13)
#define LM_TS_INVALID_INDEX		(LM_TS_BASE_ERROR+14)
#define LM_TS_RESERVED_15		(LM_TS_BASE_ERROR+15)
#define LM_TS_NO_ASR_FOUND		(LM_TS_BASE_ERROR+16)
#define LM_TS_UPDATING_TS		(LM_TS_BASE_ERROR+17)
#define LM_TS_SEND_RECEIVE		(LM_TS_BASE_ERROR+18)
#define LM_TS_PROCESS_RESP		(LM_TS_BASE_ERROR+19)
#define LM_TS_UNKNOWN_RESP		(LM_TS_BASE_ERROR+20)
#define LM_TS_ASR_LOAD_ONCE		(LM_TS_BASE_ERROR+21)
#define LM_TS_RESERVED_22		(LM_TS_BASE_ERROR+22)
#define LM_TS_SHORT_CODE_PENDING	(LM_TS_BASE_ERROR+23)
#define LM_TS_SHORT_CODE_ACT_CREATE	(LM_TS_BASE_ERROR+24)
#define LM_TS_SHORT_CODE_REPAIR_CREATE	(LM_TS_BASE_ERROR+25)
#define LM_TS_SHORT_CODE_RETURN_CREATE	(LM_TS_BASE_ERROR+26)
#define LM_TS_SHORT_CODE_CANCEL	(LM_TS_BASE_ERROR+27)
#define LM_TS_SHORT_CODE_PROCESS	(LM_TS_BASE_ERROR+28)
#define LM_TS_SHORT_CODE_UNSUPPORTED	(LM_TS_BASE_ERROR+29)
#define LM_TS_LOAD				(LM_TS_BASE_ERROR+30)
#define LM_TS_FR_DISABLE		(LM_TS_BASE_ERROR+31)
#define LM_TS_TIMEDOUT					(LM_TS_BASE_ERROR+32)
#define LM_TS_INSUFFICIENT_RESOURCE		(LM_TS_BASE_ERROR+33)
#define LM_TS_INVALID_REQUEST_TYPE		(LM_TS_BASE_ERROR+34)
#define LM_TS_NO_MATCHING_FULFILLMENT	(LM_TS_BASE_ERROR+35)
#define LM_TS_INVALID_REQUEST			(LM_TS_BASE_ERROR+36)
#define LM_TS_RETURN_OUT_OF_CHAIN		(LM_TS_BASE_ERROR+37)
#define LM_TS_MAX_COUNT_EXCEEDED		(LM_TS_BASE_ERROR+38)
#define LM_TS_INSUFFICIENT_REPAIR_COUNT	(LM_TS_BASE_ERROR+39)
#define LM_TS_OPERATIONS				(LM_TS_BASE_ERROR+40)
#define LM_TS_CONNECTION_FAILED			(LM_TS_BASE_ERROR+41)
#define LM_TS_SSL_ERROR					(LM_TS_BASE_ERROR+42)
#define LM_TS_RETURN_INCOMPLETE			(LM_TS_BASE_ERROR+43)
#define LM_TS_LOCAL_REPAIR				(LM_TS_BASE_ERROR+44)
#define LM_TS_UNSUPPORTED_REQUEST_VERSION		(LM_TS_BASE_ERROR+45)
#define LM_TS_UNSUPPORTED				(LM_TS_BASE_ERROR+46)
#define LM_TS_CONFIGURATION				(LM_TS_BASE_ERROR+47)

#endif /* FlxActError_h */
