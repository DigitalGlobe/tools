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
#ifndef wmsNotifyContext_HEADER
#define wmsNotifyContext_HEADER 1
#include <wms/wmsExport.h>
#include <iostream>
#include <string>
#include <fstream>

/**
  * Notification level enumeration.  This specifies the
  * level of the mesage being written.
  */
WMS_EXPORT enum wmsNotifyLevel
{
   wmsNotifyLevel_ALWAYS = 0,
   wmsNotifyLevel_FATAL  = 1,
   wmsNotifyLevel_WARN   = 2,
   wmsNotifyLevel_NOTICE = 3,
   wmsNotifyLevel_INFO   = 4,
   wmsNotifyLevel_DEBUG  = 5
};

/**
 * Flags to allow the user to turn off certain notification levels
 * 
 */
WMS_EXPORT enum wmsNotifyFlags
{
   wmsNotifyFlags_NONE   = 0,
   wmsNotifyFlags_FATAL  = 1,
   wmsNotifyFlags_WARN   = 2,
   wmsNotifyFlags_NOTICE = 4,
   wmsNotifyFlags_INFO   = 8,
   wmsNotifyFlags_DEBUG  = 16,
   wmsNotifyFlags_ALL    = (wmsNotifyFlags_FATAL|
                              wmsNotifyFlags_WARN|
                              wmsNotifyFlags_NOTICE|
                              wmsNotifyFlags_INFO|
                              wmsNotifyFlags_DEBUG)
};

WMS_EXPORT void wmsSetDefaultNotifyHandlers();

WMS_EXPORT void wmsSetNotifyStream(std::ostream* outputStream,
                                   wmsNotifyFlags whichLevelsToRedirect=wmsNotifyFlags_ALL);

WMS_EXPORT std::ostream* wmsGetNotifyStream(wmsNotifyLevel whichLevel);

WMS_EXPORT bool wmsIsNotifyEnabled();

WMS_EXPORT std::ostream& wmsNotify(wmsNotifyLevel level = wmsNotifyLevel_WARN);

/**
 *
 */
WMS_EXPORT void wmsSetLogFilename(const std::string& filename);


/**
 *
 */
WMS_EXPORT void wmsDisableNotify(wmsNotifyFlags notifyLevel=wmsNotifyFlags_ALL);

/**
 *
 */
WMS_EXPORT void wmsEnableNotify(wmsNotifyFlags notifyLevel=wmsNotifyFlags_ALL);


WMS_EXPORT void wmsSetNotifyFlag(wmsNotifyFlags notifyFlags);
WMS_EXPORT void wmsPushNotifyFlags();
WMS_EXPORT void wmsPopNotifyFlags();
WMS_EXPORT wmsNotifyFlags wmsGetNotifyFlags();

#endif /* #ifndef wmsNotifyContext_HEADER */
