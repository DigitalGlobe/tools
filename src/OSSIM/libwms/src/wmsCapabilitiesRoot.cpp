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
#include <wms/wmsCapabilitiesRoot.h>
#include <wms/wmsCapabilitiesState.h>

std::ostream& operator <<(std::ostream& out, const wmsCapabilitiesRoot& src)
{
   out << "WmsVersion:    " << src.theWmsVersion << std::endl
       << "WmsServer:     " << src.theWmsServerUrl << std::endl
       << *src.root();
   
   return out;
}

wmsCapabilitiesRoot::wmsCapabilitiesRoot()
{
   theRootLayer = new wmsCapabilitiesState;
}

wmsCapabilitiesRoot::~wmsCapabilitiesRoot()
{
   theRootLayer = NULL;
}

wmsRefPtr<wmsCapabilitiesState> wmsCapabilitiesRoot::root()
{
   return theRootLayer;
}

const wmsRefPtr<wmsCapabilitiesState> wmsCapabilitiesRoot::root()const
{
   return theRootLayer;
}

const std::string& wmsCapabilitiesRoot::wmsVersion()const
{
   return theWmsVersion;
}

std::string& wmsCapabilitiesRoot::wmsVersion()
{
   return theWmsVersion;
}

const std::string& wmsCapabilitiesRoot::wmsServerUrl()const
{
   return theWmsServerUrl;
}

std::string& wmsCapabilitiesRoot::wmsServerUrl()
{
   return theWmsServerUrl;
}

wmsRefPtr<wmsCapabilitiesState> wmsCapabilitiesRoot::getNodeGivenName(const std::string& name)
{
   if(theRootLayer->name() == name)
   {
      return theRootLayer.get();
   }
   if(theRootLayer->hasChildren())
   {
      std::vector<wmsRefPtr<wmsCapabilitiesState> > stateStack;
      stateStack = theRootLayer->children();
      while(stateStack.size()>0)
      {
         wmsRefPtr<wmsCapabilitiesState> current = stateStack[stateStack.size()-1];
         stateStack.pop_back();
         if(current.valid())
         {
            if(current->name() == name)
            {
               return current.get();
            }
            if(current->hasChildren())
            {
               stateStack.insert(stateStack.end(),
                                 current->children().begin(),
                                 current->children().end());
            }
         }
      }
   }

   return 0;
}

void wmsCapabilitiesRoot::getLeafNodes(std::vector<wmsRefPtr<wmsCapabilitiesState> >& leafNodes, bool includeGroupNodes)
{
   if(!theRootLayer.valid()) return;

   if(theRootLayer->hasChildren())
   {
      std::vector<wmsRefPtr<wmsCapabilitiesState> > stateStack;
      stateStack = theRootLayer->children();
      while(stateStack.size()>0)
      {
         wmsRefPtr<wmsCapabilitiesState> current = stateStack[stateStack.size()-1];
         stateStack.pop_back();
         if(current.valid())
         {
            if(current->hasChildren())
            {
               stateStack.insert(stateStack.end(),
                                 current->children().begin(),
                                 current->children().end());
               if(includeGroupNodes&& (current.get() != root().get()))
               {
                  leafNodes.push_back(current);
               }
            }
            else
            {
               leafNodes.push_back(current);
            }
         }
      }
   }
   else
   {
      leafNodes.push_back(theRootLayer);
   }
}

bool wmsCapabilitiesRoot::getLatLonBoundingBox(double& minLat,
                                      double& minLon,
                                      double& maxLat,
                                      double& maxLon)const
{
   if(theRootLayer.valid())
   {
      return theRootLayer->getLatLonBoundingBox(minLat,
                                                minLon,
                                                maxLat,
                                                maxLon);
   }
   return false;
}

