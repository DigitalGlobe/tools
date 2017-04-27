/**************************************************************************************************/
/*!@file
 * @brief A brief description goes here
 *
 * Detailed description goes here
 *
 * @version \$Revision: 64 $
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
#if !defined( FNP_STDTYPES_H )
#define FNP_STDTYPES_H

#include "fnpplatformtypes.h"

#if FNPPUB_HAVE_STDINT_H
#include <stdint.h>
#elif FNPPUB_HAVE_INTTYPES_H
#include <inttypes.h>
#elif FNPPUB_HAVE_WINSTDINT_H
#include "windows/winstdint.h"
#endif

#endif /* FNP_STDTYPES_H */

