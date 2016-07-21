/* -*-c++-*- libwms - Copyright (C) since 2004 Garrett Potts 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the libwms Public License (WMSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * libwms Public License for more details.
*/
#ifndef wms_HEADER
#define wms_HEADER
#include <string>
#include "wmsExport.h"

WMS_EXPORT std::string wmsUpcase(const std::string& input);

#ifdef __cplusplus
extern "C"
{
#endif
WMS_EXPORT bool wmsStringContains(const std::string& srcStr, const std::string& searchStr);
WMS_EXPORT void wmsInitialize();
WMS_EXPORT void wmsFinalize();
#ifdef __cplusplus
}
#endif
   
#endif
