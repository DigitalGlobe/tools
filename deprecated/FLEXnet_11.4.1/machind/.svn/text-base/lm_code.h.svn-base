/******************************************************************************
 *

 *
 *          Copyright (c) 1990-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
 *      This software has been provided pursuant to a License Agreement
 *      containing restrictions on its use.  This software contains
 *      valuable trade secrets and proprietary information of
 *      Macrovision Corporation and is protected by law.  It may
 *      not be copied or distributed in any form or medium, disclosed
 *      to third parties, reverse engineered or used in any manner not
 *      provided for in said License Agreement except with the prior
 *      written authorization from Macrovision Corporation.
 ******************************************************************************
 *
 *      Description:    Used to generate lm_new.o/.obj and by license-
 *                      generators.
 *
 *      Once the kit is "built" (using make or nmake) this file is no longer
 *	needed, but should be stored somewhere safe.
 *
 *	Set the following values:
 *      VENDOR_KEY1-5 		Provided by Macrovision Corporation.
 *      VENDOR_NAME 		If not evaluating, set to vendor name.
 *      LM_SEED1-3 			Make up 3 32-bit numbers, (or use
 *				'lmrand1 -seed' to make up), keep secret, safe,
 *				and never change.
 *	TRL_KEY1-2 		Provided by Macrovision if TRL used.
 *      LM_STRENGTH: 		If using TRL, set to desired length
 *
 *	Upgrading: 		from version older than 8.1: Copy your
 *				ENCRYPTION_SEEDs from the old lm_code.h file.
 *				Make sure LM_STRENGTH matches, if you were
 *				using LM_STRENGTH
 */

#ifndef LM_CODE_H
#define LM_CODE_H
#include "lm_trl.h"
/*
 * 	Vendor keys:   		Enter keys received from Macrovision.
 *				Changing keys has NO impact on license files
 *				(unlike LM_SEEDs).
 */
#define VENDOR_KEY1 0xfd56b688
#define VENDOR_KEY2 0x3635e3ac
#define VENDOR_KEY3 0x14e6b96f
#define VENDOR_KEY4 0xd5ae62ef
#define VENDOR_KEY5 0x396756ea
/*
 * 	Vendor name.  		Leave "demo" if evaluating.  Otherwise,
 *			 	set to your vendor daemon name.
 */
#define VENDOR_NAME "sanzew"
/*
 * 	Private SEEDs: 		Make up 3, 8-hex-char numbers, replace and
 *				guard securely.  You can also use the command
 *				'lmrand1 -seed' to make these numbers up.
 */
#define LM_SEED1 0x9d2fa5f0
#define LM_SEED2 0x7be83a18
#define LM_SEED3 0xea6e32c8
/*
 *	Pick an LM_STRENGTH:
 */
#define LM_STRENGTH LM_STRENGTH_239BIT
/*
 *			       	LM_STRENGTH Options are
 *			       	LM_STRENGTH_DEFAULT:
 *			      	 Public key protection unused. Use SIGN=
 *			      	 attribute. sign length = 12
 *			       	TRL:
 *			       	LM_STRENGTH_113BIT, LOW:   sign length= 58 chars
 *			       	LM_STRENGTH_163BIT, MEDIUM:sign length= 84 chars
 *			       	LM_STRENGTH_239BIT, HIGH:  sign length=120 chars
 *				Pre-v7.1:
 *			       	LM_STRENGTH_LICENSE_KEY:   Use old license-key.
 *			       	If you're upgrading from
 *			       	pre-v7.1, and want no changes, set this to
 *			       	LM_STRENGTH_LICENSE_KEY.
*/
/*
 *	TRL Keys:		Provided by Macrovision Corporation, if TRL is used.
 * 				Be sure to set LM_STRENGTH to
 * 				LM_STRENGTH_DEFAULT, above, if TRL_KEYs are zero.
 */
#define TRL_KEY1    0x3e68a88b
#define TRL_KEY2    0x41e77b68


#include "lm_code2.h"
#endif /* LM_CODE_H */
