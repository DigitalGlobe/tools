/**************************************************************************************************/
/*!@file
 * @brief A brief description goes here
 *
 * Detailed description goes here
 *
 * @version \$Revision: 64 $
 */
/***************************************************************************************************
 * NOTICE OF COPYRIGHT AND OWNERSHIP OF SOFTWARE:
 *
 * Copyright (c) 2005, Macrovision Europe Ltd.  All Rights Reserved.
 *
 * This computer program is the property of Macrovision Europe Ltd. of Maidenhead, Berkshire, 
 * England and Macrovision Corporation of Santa Clara, California, U.S.A. 
 * Any use, copy, publication, distribution, display, modification, or transmission of this computer
 * program in whole or in part in any form or by any means without the prior express written 
 * permission of Macrovision Europe Ltd. or Macrovision Corporation is strictly prohibited.
 * 
 * Except when expressly provided by Macrovision Europe Ltd. or Macrovision Corporation in writing,
 * possession of this computer program shall not be construed to confer any license or rights under 
 * any of Macrovision Europe Ltd.'s or Macrovision Corporation's intellectual property rights, 
 * whether by estoppel, implication, or otherwise.  
 * 
 * ALL COPIES OF THIS PROGRAM MUST DISPLAY THIS NOTICE OF COPYRIGHT AND OWNERSHIP IN FULL.
 **************************************************************************************************/
#if !defined( FNP_PLATFORMTYPES_H )
#define FNP_PLATFORMTYPES_H

#if defined(__linux__) 
#	define FNPPUB_HAVE_STDINT_H 1

#elif ( defined(__SVR4) && defined(__sun) )
#	define FNPPUB_HAVE_INTTYPES_H 			1

#elif defined(__APPLE__)
#	define FNPPUB_HAVE_STDINT_H 1
#	define FNPPUB_HAVE_APPLEINSTALLER_H    	1

#elif defined(_WIN32)
#	define FNPPUB_HAVE_WINSTDINT_H         	1
#	define FNPPUB_HAVE_WININSTALLER_H      	1
    
#elif defined(__CYGWIN__)
#	define FNPPUB_HAVE_STDINT_H            	1
#	define FNPPUB_HAVE_WININSTALLER_H      	1

#elif defined(_AIX)
#	define FNPPUB_HAVE_STDINT_H            	1
#elif defined(__sgi__)
#	define FNPPUB_HAVE_INTTYPES_H 			1
#elif defined(__hpux__)
#	define FNPPUB_HAVE_INTTYPES_H 			1
#else
#	error Unknown platform
#endif

#ifndef FNPPUB_HAVE_STDINT_H
#	define FNPPUB_HAVE_STDINT_H 0
#endif

#ifndef FNPPUB_HAVE_INTTYPES_H
#	define FNPPUB_HAVE_INTTYPES_H 0
#endif

#ifndef FNPPUB_HAVE_WINSTDINT_H
#	define FNPPUB_HAVE_WINSTDINT_H 0
#endif

#ifndef FNPPUB_HAVE_APPLEINSTALLER_H
#	define FNPPUB_HAVE_APPLEINSTALLER_H 0
#endif

#ifndef FNPPUB_WININSTALLER_H
#	define FNPPUB_WININSTALLER_H 0
#endif


#endif /* FNP_PLATFORMTYPES_H */
