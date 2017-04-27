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
#include <sstream>
#include <wms/wmsClient.h>
#include <wms/wmsCurlMemoryStream.h>
#include <wms/wmsUrl.h>
#include <wms/wmsMemoryStream.h>
#include <string.h>
#include <wms/wms.h>
#include <iomanip>

wmsClient::wmsClient()
   :theTimeout(0),
    theMaxNumberRetry(5)
{
}

wmsClient::~wmsClient()
{
}

wmsUrl wmsClient::getMapUrl(const wmsUrl& url,
                            unsigned int width,
                            unsigned int height,
                            const double& minLat,
                            const double& minLon,
                            const double& maxLat,
                            const double& maxLon,
                            const std::string& imageFormat,
                            const std::string& version,
                            const std::string& projection)const
{
   wmsUrl tempUrl = url;
   std::stringstream s;
   std::string protocol = url.protocol();
   std::string server   = url.server();
   std::string path     = url.path();
   std::string options  = url.options();
   s << "REQUEST=GetMap";

   if(!version.empty())
   {
      s<<"&VERSION=" <<version;
   }
   if(!imageFormat.empty())
   {
      s << "&FORMAT=" << imageFormat;
   }
   if(!projection.empty())
   {
     s<< "&SRS=" << projection;
   }

   s << "&WIDTH=" << width
     << "&HEIGHT=" << height << std::setiosflags(std::ios::fixed)
     << std::setprecision(15)
     << "&BBOX="<<minLon <<","<<minLat<<","<<maxLon<<","<<maxLat;
   
   if(options == "")
   {
      options = s.str();
   }
   else if( (*(--options.end())) != '&')
   {
      options += "&";
   }
   options += s.str();
   tempUrl.mergeUrl(protocol,
                    server,
                    path,
                    options);

   return tempUrl;
};

bool wmsClient::getMap(const wmsUrl& url,
                       unsigned int width,
                       unsigned int height,
                       const double& minLat,
                       const double& minLon,
                       const double& maxLat,
                       const double& maxLon,
                       const std::string& imageFormat,
                       const std::string& version,
                       const std::string& filename,
                       const std::string& projection)const
{
   wmsUrl tempUrl = getMapUrl(url,
                              width,
                              height,
                              minLat,
                              minLon,
                              maxLat,
                              maxLon,
                              imageFormat,
                              version,
                              projection);

   return get(tempUrl, filename);
}


bool wmsClient::getCapabilities(const wmsUrl& url,
                                const std::string filename)const
{
   wmsUrl tempUrl = getCapabilitiesUrl(url);
   if(!tempUrl.url().empty())
   {
      return get(tempUrl, filename);
   }
   
   return false;
}

wmsUrl wmsClient::getCapabilitiesUrl(const wmsUrl& url)const
{
   wmsUrl result;
   if(url!= "")
   {
      std::string optionStr = url.options();
      std::string request = url.options();
      optionStr = wmsUpcase(optionStr);
      if (!wmsStringContains(optionStr, "REQUEST"))
      {
         if(optionStr == "")
         {
            request = "REQUEST=GetCapabilities";
         }
         else
         {
            request = request+"&REQUEST=GetCapabilities";
         }
      }
      if(!wmsStringContains(optionStr,"VERSION"))
      {
        request = request + "&VERSION=1.0";
      }
      if(!wmsStringContains(optionStr, "SERVICE"))
      {
        request = request + "&SERVICE=WMS";
      }
      result.mergeUrl("http",
                      url.server(),
                      url.path(),
                      request);
   }

   return result;
}



bool wmsClient::get(const wmsUrl& url,
                    const std::string& filename)const
{
   bool result = false;
   theStream = 0;
   wmsRefPtr<wmsCurlMemoryStream> curlMemoryStream = new wmsCurlMemoryStream;
   
   if(url!= "")
   {
      curlMemoryStream->setUrl(url.url());
      curlMemoryStream->setTimeout(theTimeout);
      curlMemoryStream->setMaxNumberRetry(theMaxNumberRetry);
      curlMemoryStream->setProxyHost(theProxyHost);
      curlMemoryStream->setProxyPort(theProxyPort);
      curlMemoryStream->setProxyUser(theProxyUser);
      curlMemoryStream->setProxyPassword(theProxyPassword);
      result = curlMemoryStream->download(filename);
      if(result&&(filename==""))
      {
         if(curlMemoryStream->getStream().valid())
         {
            theStream =  new wmsMemoryStream(curlMemoryStream->getStream()->getBuffer(),
                                             curlMemoryStream->getStream()->getBufferSize());
            theStream->seekg(0);
            theStream->seekp(0);
         }
      }
   }

   return result;
}

wmsRefPtr<wmsMemoryStream> wmsClient::getStream()
{
   return theStream.get();
}

const wmsRefPtr<wmsMemoryStream> wmsClient::getStream()const
{
   return theStream.get();
}

void wmsClient::setMaxNumberRetry(unsigned int maxNumberRetry)
{
   theMaxNumberRetry = maxNumberRetry;
}

unsigned int wmsClient::getMaxNumberRetry()const
{
   return theMaxNumberRetry;
}


void wmsClient::setTimeout(unsigned int timeout)
{
   theTimeout = timeout;
}

unsigned int wmsClient::getTimeout()const
{
   return theTimeout;
}

void wmsClient::setProxyHost(const std::string& host)
{
   theProxyHost = host;
}

void wmsClient::setProxyPort(const std::string& port)
{
   theProxyPort = port;
}

void wmsClient::setProxyUser(const std::string& user)
{
   theProxyUser = user;
}

void wmsClient::setProxyPassword(const std::string passwd)
{
   theProxyPassword = passwd;
}
