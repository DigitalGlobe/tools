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
#ifndef wmsCurlMemoryStream_HEADER
#define wmsCurlMemoryStream_HEADER
#include <wms/wmsMemoryStream.h>
#include <wms/wmsUrl.h>
#include <wms/wmsReferenced.h>
#include <wms/wmsRefPtr.h>
#include <string>

#include <curl/curl.h>

class WMS_EXPORT wmsCurlMemoryStream : public wmsReferenced
{
public:
   wmsCurlMemoryStream(const std::string& url="");
   ~wmsCurlMemoryStream();
   bool download(const std::string& filename="");
   void setUrl(const wmsUrl& url);
   wmsUrl getUrl()const;
   
   wmsRefPtr<wmsMemoryStream> getStream();
   const wmsRefPtr<wmsMemoryStream> getStream()const;
   void setTimeout(unsigned int timeout);
   void setMaxNumberRetry(unsigned int retryCount);
   void setVerboseFlag(bool verboseFlag);
   void setFollowLocationFlag(unsigned int flag);
   void setMaxRedirects(unsigned int maxRedirects);
   void setProxyHost(const std::string& host);
   void setProxyPort(const std::string& port);
   void setProxyUser(const std::string& user);
   void setProxyPassword(const std::string passwd);
   
protected:
   wmsRefPtr<wmsMemoryStream> theStream;
   wmsUrl theUrl;
   CURL* theCurl;
   unsigned int theMaxRetryCount;
   unsigned int theTimeout;
   bool theVerboseFlag;
   int theFollowLocationFlag;
   int theMaxRedirects;
   std::string theProxyHost;
   std::string theProxyPort;
   std::string theProxyUser;
   std::string theProxyPassword;
};

#endif
