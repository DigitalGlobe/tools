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
#include <wms/wmsStyle.h>


std::ostream& operator <<(std::ostream& out, const wmsStyle& src)
{
   out << "Name:     " << src.theName  << std::endl
       << "Title:    " << src.theTitle << std::endl
       << "Abstract: " << src.theAbstract << std::endl;

   out << "Legend Url List\n" << std::endl;
   unsigned int idx = 0;
   for(idx = 0; idx < src.theLegendUrlList.size();++idx)
   {
      out << src.theLegendUrlList[idx] << std::endl;
   }
   
   return out;
}
