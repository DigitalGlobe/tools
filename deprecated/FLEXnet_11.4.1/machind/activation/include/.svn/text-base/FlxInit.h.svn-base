#if !defined( FLXINIT_H_INCLUDED )
#define FLXINIT_H_INCLUDED

/**************************************************************************************************/
/*!@file FlxInit.h
 * @brief 	Defines the interface for loading & unloading the security runtime from
 *			protected applications & dynamic libraries.
 *
 */
/***************************************************************************************************
 * Copyright 2005 - 2006 Macrovision Europe Ltd. All Rights Reserved. 
 * 
 * This software has been provided pursuant to a License Agreement containing
 * restrictions on its use. This software contains valuable trade secrets 
 * and proprietary information of Macrovision Europe Ltd. and is protected 
 * by law. It may not be copied or distributed in any form or medium, 
 * disclosed to third parties, reverse engineered or used in any manner not 
 * provided for in said License Agreement except with the prior written 
 * authorization from Macrovision Europe Ltd. 
 **************************************************************************************************/

#if defined( __cplusplus)
extern "C" {
#endif

/******************************************************************************
 * flxInitLoad error codes
 ******************************************************************************/

typedef enum flxInitStatus
{
	flxInitSuccess 					= 0, 	/* Load was successful */
	flxInitUnableToLocate			= 1,	/* Unable to find security runtime */
	flxInitUnableToLoad				= 2,	/* Unable to load security runtime */
	flxInitUnsupportedPlatform		= 3,	/* OS version too old */
	flxInitInitializationError		= 4,	/* Initialization of security runtime failed */
	flxInitAllocationError			= 5,	/* Unable to allocate resources */
	flxInitNotProtected				= 6,	/* preptool has not been run since build */
    flxInitCannotReload				= 7,   	/* Call to flxInitLoad after an flxInitUnload */

    /* Mac OS X specific error values */
    flxInitEncodingError			= 8,	/* path string not in UTF-8? */
    flxInitUrlError					= 9,	/* error creating url */
    flxInitUrlNoPathError			= 10, 	/* error creating path from url */
	flxInitFrameworkNotLoadedError	= 11,	/* framework specified by bundle ID not loaded */
	flxInitBundleIDError			= 12,	/* Not a valid bundle ID */
	flxInitPathOverflow				= 13,	/* computed path is too long */

	/* Service error values */
	flxInitServiceNotInstalled		= 20,	/* service not installed */
	flxInitServiceNotEnoughRights	= 21	/* not enough rights to talk to service.
											   Set service to start automatically to resolve this */
} flxInitStatus;


/******************************************************************************
 * flxInitLoad:	The runtimePath parameter to flxInitLoad is typically NULL,
 *				as it's normally specified at prep time.
 *
 * runtimePath: Full (or relative) path including file name to the runtime
 *              .so or .dll or .dylib (depending on platform)
 *              If 0 the default will be used (as specified at prep time)
 *
 * return:      see flxInitStatus values
 ******************************************************************************/

int    flxInitLoad( const char* runtimePath );

/******************************************************************************
 * flxInitUnload:	Call this when your protected dynamic library is being
 *					unloaded.
 *
 ******************************************************************************/

void    flxInitUnload( void );

#if defined( __cplusplus )
}
#endif

#endif /* FLXINIT_H_INCLUDED */
