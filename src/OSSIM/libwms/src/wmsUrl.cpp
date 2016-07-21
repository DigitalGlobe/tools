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
#include <wms/wmsUrl.h>

std::ostream& operator <<(std::ostream& out, const wmsUrl& src)
{
   out << src.theUrl;
   
   return out;
}
static int offsetFromBase(char c)
{
   if((c >= 'a') && (c <='f'))
   {
      return (int)((c-'a')+10);
   }
   else if((c>='A') && (c <= 'F'))
   {
      return (int)((c-'A') + 10);
   }
   else if((c>='0')&&(c<='9'))
   {
      return (int)(c-'0');
   }
   return -1;
}
static char toChar(const std::string& s)
{
   char hexChars[2];
   if(*s.begin() == '%')
   {
      hexChars[0] = *(s.begin()+1);
      hexChars[1] = *(s.begin()+2);
   }
   else
   {
      hexChars[0] = *(s.begin());
      hexChars[1] = *(s.begin()+1);
   }
   int c1 = offsetFromBase(hexChars[0]);
   int c2 = offsetFromBase(hexChars[1]);

   if((c1 < 0)||
      (c2 < 0))
   {
      return '\0';
   }
   return (char)(c1*16+
                 c2);
}


std::string wmsUrl::decode()const
{
   std::string tempS;
   std::string::const_iterator iter = theUrl.begin();
   
   while(iter != theUrl.end())
   {
      if(*iter == '%')
      {
		  tempS += toChar(std::string(iter, iter+3));
         iter+=3;
      }
      else
      {
         tempS += *iter;
         ++iter;
      }
   }
   
   return tempS;
}

void wmsUrl::mergeUrl(const std::string& tempProtocol,
                      const std::string& tempServer,
                      const std::string& tempPath,
                      const std::string& tempOptions)
{
   theUrl  = "";
   if(tempProtocol!="")
   {
      theUrl = tempProtocol + "://";
   }
   if(tempServer != "")
   {
      theUrl = theUrl + tempServer;
   }
   if(tempPath != "")
   {
      if((*(--theUrl.end()) != '/')&&
         ((*tempPath.begin())!='/'))
      {
         theUrl += "/"; // tack on a slash if one is not present
      }
      theUrl += tempPath;
   }
   if(tempOptions!= "")
   {
      if(*(--theUrl.end()) != '?')
      {
         theUrl += "?";
      }
      theUrl += tempOptions;
   }
}

void wmsUrl::splitUrl(std::string& tempProtocol,
                      std::string& tempServer,
                      std::string& tempPath,
                      std::string& tempOptions)const
{
   tempProtocol = protocol();
   tempServer   = server();
   tempPath     = path();
   tempOptions  = options();
}

std::string wmsUrl::protocol()const
{
   std::string result;
   std::string::size_type pos = theUrl.find(":");

   if(pos != std::string::npos)
   {
      result = std::string(theUrl.begin(),
                           theUrl.begin()+pos);
   }
   
   return result;
}

std::string wmsUrl::server()const
{
   std::string::size_type posStart = theUrl.find("//");
   std::string::size_type posEnd = 0;

   if(posStart == std::string::npos)
   {
      posStart = 0;
   }
   else
   {
      posStart += 2; // skip the "//" string
   }

   posEnd = theUrl.find("/", posStart);

   if(posEnd == std::string::npos)
   {
      return std::string(theUrl.begin() + posStart,
                         theUrl.end());
   }
   return std::string(theUrl.begin() + posStart,
                      theUrl.begin() + posEnd);
}

std::string wmsUrl::path()const
{
   std::string::size_type posStart = theUrl.find("//");
   std::string::size_type posEnd = 0;

   if(posStart == std::string::npos)
   {
      posStart = theUrl.find("/");
   }
   else
   {
      posStart +=2; //skip "//" string and start seaarching for / after that
      posStart = theUrl.find("/", posStart);
   }

   if(posStart == std::string::npos)
   {
      return "/";
   }
   posEnd = theUrl.find("?");
   if(posEnd == std::string::npos)
   {
      return std::string(theUrl.begin() + posStart,
                           theUrl.end());
   }
   return std::string(theUrl.begin() + posStart,
                      theUrl.begin() + posEnd);
}

std::string wmsUrl::options()const
{
   std::string result;
   std::string::size_type pos = theUrl.find("?");

   if(pos != std::string::npos)
   {
      // skip the question mark
      //
      result = std::string(theUrl.begin() + pos + 1,
                           theUrl.end());
   }

   return result;
}
