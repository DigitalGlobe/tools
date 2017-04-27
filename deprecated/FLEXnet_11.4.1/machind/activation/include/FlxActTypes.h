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
 * FlxActTypes include file
 *
 * The FlxActTypes include file contains the type definitions required for using
 * the Flex activation library API.
 */

#ifndef FlxActTypes_h

#define FlxActTypes_h  1

#ifdef __cplusplus
extern "C" {
#endif

#if defined( SUNOS5 ) || defined (HAVE_INTTYPES_H)
#include <inttypes.h>
#elif  defined (PC) || defined (_WIN32)
#ifdef _MSC_VER
typedef unsigned __int32	uint32_t;
#else /* _MSC_VER */
typedef unsigned			uint32_t;
#endif /* _MSC_VER */
#else
#if defined (HP)
#include "inttypes.h"
#else
#include "stdint.h"
#endif /* HP */
#endif

/* define our own boolean */
#define FLX_ACT_TRUE (1)			/* true */
#define FLX_ACT_FALSE (0)			/* false */
typedef uint32_t FlxActBool;

/**
 * define our own error structure to return detailed information
 * regarding errors
 */
typedef struct {
	uint32_t	majorErrorNo; 	/* major error code, get information from error types.h */
	uint32_t	minorErrorNo;	/* corresponds to code location - source of error */
	uint32_t	sysErrorNo;		/* corresponds to system to 3rd party library error code */
} FlxActError;

#ifdef __cplusplus
}
#endif

#endif


