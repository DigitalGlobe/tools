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
#include <wms/wms.h>
#include <curl/curl.h>
#include <algorithm>
#include <ctype.h>

std::string wmsUpcase(const std::string& input)
{
   std::string s(input);

   std::string::iterator eachCharacter = s.begin();
   while(eachCharacter != s.end())
   {
      *eachCharacter = toupper(*eachCharacter);
      ++eachCharacter;
   }

   return s;
}

bool wmsStringContains(const std::string& srcStr, const std::string& searchStr)
{
   return (srcStr.find(searchStr)!=std::string::npos);
}

void wmsInitialize()
{
   curl_global_init(CURL_GLOBAL_DEFAULT);
}

void wmsFinalize()
{
   curl_global_cleanup();
}
