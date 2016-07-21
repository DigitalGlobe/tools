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
#ifndef wmsCapabilitiesRoot_HEADER
#define wmsCapabilitiesRoot_HEADER
#include <string>
#include <iostream>
#include <vector>
#include <wms/wmsReferenced.h>
#include <wms/wmsRefPtr.h>

class wmsCapabilitiesState;

class WMS_EXPORT wmsCapabilitiesRoot : public wmsReferenced
{
public:
   friend std::ostream& operator <<(std::ostream& out, const wmsCapabilitiesRoot& src);
   wmsCapabilitiesRoot();
   ~wmsCapabilitiesRoot();
   wmsRefPtr<wmsCapabilitiesState> root();
   const wmsRefPtr<wmsCapabilitiesState> root()const;
   const std::string& wmsVersion()const;
   std::string& wmsVersion();
   const std::string& wmsServerUrl()const;
   std::string& wmsServerUrl();
   wmsRefPtr<wmsCapabilitiesState> getNodeGivenName(const std::string& name);
   void getLeafNodes(std::vector<wmsRefPtr<wmsCapabilitiesState> >& leafNodes, bool includeGroupNodes=false);
   bool getLatLonBoundingBox(double& minLat,
                             double& minLon,
                             double& maxLat,
                             double& maxLon)const;
   
protected:
   wmsRefPtr<wmsCapabilitiesState> theRootLayer;
   std::string theWmsVersion;
   std::string theWmsServerUrl;
};

#endif
