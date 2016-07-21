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
#ifndef wmsUrl_HEADER
#define wmsUrl_HEADER
#include <string>
#include <iostream>
#include <wms/wmsExport.h>

class WMS_EXPORT wmsUrl
{
public:
   friend WMS_EXPORT std::ostream& operator <<(std::ostream& out, const wmsUrl& src);

   wmsUrl(const char* url)
      {
         theUrl = ((url)?url:"");
      }
   wmsUrl(const std::string& url="")
      :theUrl(url)
      {
      }
   wmsUrl(const wmsUrl& src)
      :theUrl(src.theUrl)
      {
      }
   ~wmsUrl()
      {}

   /**
    * Decodes a URL that had encode special characters
    * using the %.
    */ 
   std::string decode()const;
   std::string& url()
      {
         return  theUrl;
      }
   const std::string& url()const
      {
         return  theUrl;
      }

   bool operator !=(const wmsUrl& rhs)const
      {
         return (theUrl!=rhs.theUrl);
      }
   bool operator ==(const wmsUrl& rhs)const
      {
         return (theUrl==rhs.theUrl);
      }
   
   void mergeUrl(const std::string& tempProtocol,
                 const std::string& tempServer,
                 const std::string& tempPath,
                 const std::string& tempOptions);
   /*!
    * http://myserver/mypath/toprogram?options=value
    * 
    * Will be split apart as follows:
    * 
    * protocol = http
    * server   = myserver
    * path     = mypath/toprogram
    * options  = optoins=value
    */
   void splitUrl(std::string& tempProtocol,
                 std::string& tempServer,
                 std::string& tempPath,
                 std::string& tempOptions)const;

   
   std::string protocol()const;
   std::string server()const;
   std::string path()const;
   std::string options()const;
 protected:
   std::string theUrl;
   
};

#endif
