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
#include <cstdlib>
#include <wms/wmsCurlMemoryStream.h>
#include <wms/wmsNotify.h>
#include <fstream>
#include <sstream>

struct wmsCurlMemoryStreamStruct
{
   wmsCurlMemoryStreamStruct()
      {
         curlStream = 0;
      }
   std::ostream* curlStream;
};

static int wmsCurlWrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  int rc = 0;

  wmsCurlMemoryStreamStruct *out=(struct wmsCurlMemoryStreamStruct *)stream;

//   if(out->curlStream->getStream()->good())
//   {
//      out->curlStream->getStream()->write((char*)buffer, nmemb*size);
//      rc = nmemb*size;
//   }

   if(out->curlStream&&out->curlStream->good())
   {
      out->curlStream->write((char*)buffer, nmemb*size);
      rc = nmemb*size;
   }

  return rc;
}

wmsCurlMemoryStream::wmsCurlMemoryStream(const std::string& url)
{
   theMaxRedirects = 1;
   theFollowLocationFlag = 0;
   theCurl = curl_easy_init();
   curl_easy_setopt(theCurl, CURLOPT_WRITEFUNCTION, wmsCurlWrite);
   setUrl(url);
   theMaxRetryCount = 5;
   theTimeout = 0;
   theVerboseFlag = false;
   theStream = new wmsMemoryStream;
#ifdef WMS_DEBUG
   theVerboseFlag = true;
#endif

}

wmsCurlMemoryStream::~wmsCurlMemoryStream()
{
   if(theCurl)
   {
      curl_easy_cleanup(theCurl);
      theCurl = NULL;
   }

   theStream->clear();
}

void wmsCurlMemoryStream::setUrl(const wmsUrl& url)
{
   theUrl = url;
   curl_easy_setopt(theCurl, CURLOPT_URL, theUrl.url().c_str());
}

wmsUrl wmsCurlMemoryStream::getUrl()const
{
   return theUrl;
}

void wmsCurlMemoryStream::setFollowLocationFlag(unsigned int flag)
{
   theFollowLocationFlag = flag;
}

void wmsCurlMemoryStream::setMaxRedirects(unsigned int maxRedirects)
{
   theMaxRedirects = maxRedirects;
}

bool wmsCurlMemoryStream::download(const std::string& filename)
{
   std::string authenticationString;

   theStream = new wmsMemoryStream;
   std::ostream* outStream = theStream.get();
   bool needToDeleteStream = false;
   bool result = false;
   wmsCurlMemoryStreamStruct outStruct;
   theStream->clear();
   if(filename != "")
   {
      std::ofstream* outFileStream = new std::ofstream;
      outFileStream->open(filename.c_str(),
                          std::ios::out|std::ios::binary);
      if(!outFileStream->fail())
      {
         outStream = outFileStream;
         needToDeleteStream = true;
      }
      else
      {
         delete outFileStream;
         outFileStream = 0;
      }
   }
   outStruct.curlStream = outStream;
   curl_easy_setopt(theCurl, CURLOPT_FOLLOWLOCATION, theFollowLocationFlag);
   curl_easy_setopt(theCurl, CURLOPT_MAXREDIRS, theMaxRedirects);
   curl_easy_setopt(theCurl, CURLOPT_WRITEDATA, (void*)(&outStruct));
   if(!theProxyHost.empty())
   {
      if(!theProxyUser.empty())
      {
         authenticationString = theProxyUser+":"+theProxyPassword;
         curl_easy_setopt(theCurl, CURLOPT_PROXYUSERPWD, authenticationString.c_str());
      }
      curl_easy_setopt(theCurl, CURLOPT_PROXY,theProxyHost.c_str());
      if(!theProxyPort.empty())
      {
         curl_easy_setopt(theCurl, CURLOPT_PROXYPORT, std::atoi(theProxyPort.c_str()));
      }
   }

   curl_easy_setopt(theCurl, CURLOPT_VERBOSE, theVerboseFlag);
   if(theTimeout > 0)
   {
      curl_easy_setopt(theCurl, CURLOPT_TIMEOUT, theTimeout);
   }
   int rc = curl_easy_perform(theCurl);
   result = (rc < 1);
   unsigned int currentCount = 0;
   while(!result &&
         (currentCount < theMaxRetryCount))
   {
      wmsNotify(wmsNotifyLevel_WARN) << "wmsCurlMemoryStream::download() INFO: ***** reattempting HTTP::GET *****" << std::endl;
      rc = curl_easy_perform(theCurl);
      result = (rc < 1);
      ++currentCount;
   }

   if(!result)
   {
      wmsNotify(wmsNotifyLevel_WARN) << "wmsCurlMemoryStream::download() WARNING: Unable to get address: " << theUrl << std::endl;
   }

   if(needToDeleteStream)
   {
      delete outStream;
   }

   return result;
}

wmsRefPtr<wmsMemoryStream> wmsCurlMemoryStream::getStream()
{
   return theStream;
}

const wmsRefPtr<wmsMemoryStream> wmsCurlMemoryStream::getStream()const
{
   return theStream;
}

void wmsCurlMemoryStream::setTimeout(unsigned int timeout)
{
   theTimeout = timeout;
}
void wmsCurlMemoryStream::setMaxNumberRetry(unsigned int retryCount)
{
   theMaxRetryCount = retryCount;
}

void wmsCurlMemoryStream::setVerboseFlag(bool verboseFlag)
{
   theVerboseFlag = verboseFlag;
}

void wmsCurlMemoryStream::setProxyHost(const std::string& host)
{
   theProxyHost = host;
}

void wmsCurlMemoryStream::setProxyPort(const std::string& port)
{
   theProxyPort = port;
}

void wmsCurlMemoryStream::setProxyUser(const std::string& user)
{
   theProxyUser = user;
}

void wmsCurlMemoryStream::setProxyPassword(const std::string passwd)
{
   theProxyPassword = passwd;
}

