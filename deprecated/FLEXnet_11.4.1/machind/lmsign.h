/******************************************************************************

	Copyright (c) 2005-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of
	Macrovision Corporation and is protected by law.  It may
	not be copied or distributed in any form or medium, disclosed
	to third parties, reverse engineered or used in any manner not
	provided for in said License Agreement except with the prior
	written authorization from Macrovision Corporation.

 *****************************************************************************/
#ifndef INCLUDE_LMSIGN_H
#define INCLUDE_LMSIGN_H

/****************************************************************************/
/**	@brief	Sign specified license using specified hostid.
 *
 *	@param	pszHostId	Hostid to sign feature/increment lines with
 *	@param	pszInput	FEATURE/INCREMENT line(s) to sign
 *	@param	pszOutput	Pointer to buffer that will receive signed output
 *	@param	pOutSize	Size of output buffer, if 0 will return size needed
 *						in bytes (including null) to hold output.
 *	@param	pszError	Pointer to buffer that will receive error message if an
 *						an error occurs.
 *	@param	pErrorSize	Size of error buffer, if 0 and an error occurs will
 *						return size needed in bytes (including null) to hold
 *						error message.
 *
 *	@return	LM_NOERROR on success else a FLEX error code, same as lc_cryptstr()
 *	@note	If size of output buffer is smaller than generated output, data
 *			will be truncated (and null terminated).
 ****************************************************************************/
int
l_flexSignLicense(
	char *			pszHostId,
	char *			pszInput,
	char *			pszOutput,
	unsigned int *	pOutSize,
	char *			pszError,
	unsigned int *	pErrorSize);

#endif /* INCLUDE_SIGN_H */
