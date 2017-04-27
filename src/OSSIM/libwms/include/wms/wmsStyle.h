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
#ifndef wmsStyle_HEADER
#define wmsStyle_HEADER
#include <vector>
#include <string>
#include <iostream>
#include <wms/wmsUrl.h>
#include <wms/wmsReferenced.h>

class wmsStyle : public wmsReferenced
{
public:
   friend std::ostream& operator <<(std::ostream& out, const wmsStyle& src);
   wmsStyle(){}
   ~wmsStyle(){}
   std::string& name()
      {
         return theName;
      }
   const std::string& name()const
      {
         return theName;
      }
   std::string& title()
      {
         return theTitle;
      }
   const std::string& title()const
      {
         return theTitle;
      }
   
   std::string& abstract()
      {
         return theAbstract;
      }
   const std::string& abstract()const
      {
         return theAbstract;
      }
   wmsUrl getLegendUrl(unsigned int idx)const
      {
         if(idx < theLegendUrlList.size())
         {
            return theLegendUrlList[idx];
         }
         return wmsUrl("");
      }
   void addLegendUrl(const wmsUrl& legendUrl)
      {
         theLegendUrlList.push_back(legendUrl);
      }
   const std::vector<wmsUrl>& legendUrlList()const
      {
         return theLegendUrlList;
      }
   std::vector<wmsUrl>& legendUrlList()
      {
         return theLegendUrlList;
      }
   void eraseLegendUrl(unsigned int idx)
      {
         if(idx < theLegendUrlList.size())
         {
            theLegendUrlList.erase(theLegendUrlList.begin()+idx);
         }
      }
protected:
   std::string theName;
   std::string theTitle;
   std::string theAbstract;
   std::vector<wmsUrl> theLegendUrlList;
};

#endif
