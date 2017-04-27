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
#ifndef wmsCapabilitiesParser_HEADER
#define wmsCapabilitiesParser_HEADER
#include <iostream>
#include <string>
#include <vector>
#include <wms/wmsReferenced.h>
#include <wms/wmsRefPtr.h>

class wmsCapabilitiesState;
class wmsCapabilitiesRoot;
class WMS_EXPORT wmsCapabilitiesParser : public wmsReferenced
{
public:
   wmsCapabilitiesParser();
   
   wmsRefPtr<wmsCapabilitiesRoot> parse(const std::string& inputFile);
   wmsRefPtr<wmsCapabilitiesRoot> parse(std::istream& in);
   wmsRefPtr<wmsCapabilitiesRoot> parseRawString(const std::string& rawCapabilities);

   static void start(void *data, const char *element, const char **attrs);
   static void end(void *data, const char *element);
   static void handler(void *data, const char *element, int len);
   
   // Function to determine if we're inside a particular tag.  This could
   // eliminate a lot of state variables.  Returns the depth of the
   // current tag under the queried tag or 0 if not under the queried tag.
   unsigned int getTagDepth(const std::string& tag)const;
   std::vector<wmsRefPtr<wmsCapabilitiesState> >& getStateStack();
   std::vector<std::string>& getTagStack();
   wmsRefPtr<wmsCapabilitiesRoot> root();
   const wmsRefPtr<wmsCapabilitiesRoot> root()const;
   const bool& processDimSeqExtent()const;
   bool& processDimSeqExtent();
   bool& processTimeExtent();
   const bool& processTimeExtent()const;
   static std::string trim(const std::string& s);
   static void addTimes(std::vector<std::string> & times, std::string el);
   
protected:
   ~wmsCapabilitiesParser();
   
   wmsRefPtr<wmsCapabilitiesRoot> theRoot;
   std::vector<wmsRefPtr<wmsCapabilitiesState> > theStateStack;
   std::vector<std::string>  theTagStack;
   mutable bool theProcessDimSeqExtent;
   mutable bool theProcessTimeExtent;
};

#endif
