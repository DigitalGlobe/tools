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
#include <cstring>
#include <iterator>
#include <wms/wmsCapabilitiesState.h>

std::ostream& operator <<(std::ostream& out, const wmsCapabilitiesState& src)
{
   unsigned int idx = 0;
   out << "Name:                 " << src.theName << std::endl
       << "Title:                " << src.theTitle << std::endl
       << "Abstract:             " << src.theAbstract << std::endl
       << "Contact Person:       " << src.theContactPerson << std::endl
       << "Contact Organization: " << src.theContactOrganization << std::endl
       << "Contact Information:  " << src.theContactInformation << std::endl
       << "Address:              " << src.theAddress << std::endl
       << "City:                 " << src.theCity << std::endl
       << "State or Province:    " << src.theStateOrProvince << std::endl
       << "Postal Code:          " << src.thePostalCode << std::endl
       << "Country:              " << src.theCountry << std::endl
       << "Voice Telephone:      " << src.theVoiceTelephone << std::endl
       << "Fax Number:           " << src.theFaxNumber << std::endl
       << "E-mail Address:       " << src.theEmailAddress << std::endl
       << "Fees:                 " << src.theFees << std::endl
       << "Access Contraints:    " << src.theAccessConstraints << std::endl
       << "LatMin:               " << src.theLatMin << std::endl
       << "LatMax:               " << src.theLatMax << std::endl
       << "LonMin:               " << src.theLonMin << std::endl
       << "LonMax:               " << src.theLonMax << std::endl
       << "Min Scale Hint:       " << src.theMinScaleHint << std::endl
       << "Max Scale Hint:       " << src.theMaxScaleHint << std::endl
       << "Min Scale Denom:      " << src.theMinScaleDenominator << std::endl
       << "TimeDefault:          " << src.theTimeDefault << std::endl
       << "DimSeqBegin:          " << src.theDimSeqBegin << std::endl
       << "DimSeqEnd:            " << src.theDimSeqEnd << std::endl
       << "DimSeqStep:           " << src.theDimSeqStep << std::endl
       << "DimSeqDefault:        " << src.theDimSeqDefault << std::endl
       << "OpaqueFlag:           " << src.theOpaqueFlag << std::endl
       << "NoSubsets:            " << src.theNoSubsets << std::endl
       << "FixedWidth:           " << src.theFixedWidth << std::endl
       << "FixedHeight:          " << src.theFixedHeight << std::endl
       << "InheritedTimes:       " << src.theInheritedTimes << std::endl
       << "Children:             " << src.theChildren.size() << std::endl
       << "IsSequence:           " << src.theIsSequence << std::endl
       << "Queryable:            " << src.theQueryableFlag << std::endl
       << "Opaque:               " << src.theOpaqueFlag << std::endl
       << "Cascaded:             " << src.theCascadedFlag << std::endl;

   if(src.theStyles.size()>0)
   {
      out << "STYLES\n" << std::endl;
      for(idx = 0; idx < (src.theStyles.size()-1); ++idx)
      {
         out << src.theStyles[idx] << std::endl;
      }
      out << src.theStyles[src.theStyles.size()-1];

   }
   if(src.formats().size() > 0)
   {
      if(src.theStyles.size() > 0)
      {
         out << std::endl;
      }
      out << "Formats: ";
      for(idx = 0; idx < src.formats().size()-1; ++idx)
      {
         out << src.formats()[idx] << " ";
      }
      out << src.formats()[src.formats().size()-1] << " ";
   }

   if(src.times().size() > 0)
   {
      out << std::endl;
      out << "Times: ";
      for(idx = 0; idx < src.times().size()-1; ++idx)
      {
         out << src.getTime(idx) << ", ";
      }
      out << src.times()[src.times().size()-1] << " ";
   }
   if(src.theChildren.size()>0)
   {
      if(src.formats().size() > 0)
      {
         out << std::endl;
      }
      for(idx = 0; idx < (src.theChildren.size()-1);++idx)
      {
         if(src.theChildren[idx].valid())
         {
            out << "Child " << idx << std::endl
                << *src.theChildren[idx] << std::endl;
         }
      }
      if(src.theChildren[src.theChildren.size()-1].valid())
      {
         out << "Child " << (src.theChildren.size()-1) << std::endl
             << *src.theChildren[src.theChildren.size()-1];
      }
   }
   return out;
}


wmsCapabilitiesState::~wmsCapabilitiesState()
{
}

wmsCapabilitiesState::wmsCapabilitiesState()
{
   theLatMin = "-90";
   theLatMax = "90";
   theLonMin = "-180";
   theLonMax = "180";

   theNoSubsets = false;
   theFixedWidth = 0;
   theFixedHeight = 0;

   theInheritedTimes = false;
   theIsSequence     = false;
}

void wmsCapabilitiesState::inherit(const wmsCapabilitiesState& src)
{
   theStyles = src.theStyles;
   theLatMin = src.theLatMin;
   theLatMax = src.theLatMax;
   theLonMin = src.theLonMin;
   theLonMax = src.theLonMax;
   theTimes  = src.theTimes;
   theTimeDefault = src.theTimeDefault;
   theDimSeqBegin = src.theDimSeqBegin;
   theDimSeqEnd = src.theDimSeqEnd;
   theDimSeqStep = src.theDimSeqStep;
   theDimSeqDefault = src.theDimSeqDefault;
   theNoSubsets = src.theNoSubsets;
   theFixedWidth = src.theFixedWidth;
   theFixedHeight = src.theFixedHeight;
   theFormats = src.theFormats;
   theInheritedTimes = src.theInheritedTimes;
   theIsSequence = src.theIsSequence;
   theMinScaleHint = src.theMinScaleHint;
   theMaxScaleHint = src.theMaxScaleHint;
}

wmsRefPtr<wmsCapabilitiesState> wmsCapabilitiesState::child(unsigned int idx)
{
   if(idx < theChildren.size())
   {
      return theChildren[idx];
   }
   return NULL;
}

const wmsRefPtr<wmsCapabilitiesState> wmsCapabilitiesState::child(unsigned int idx)const
{
   if(idx < theChildren.size())
   {
      return theChildren[idx];
   }
   return NULL;
}


void wmsCapabilitiesState::addChild(wmsCapabilitiesState* child)
{
   theChildren.push_back(child);
}


bool wmsCapabilitiesState::getLatLonBoundingBox(double& minLat,
                                                double& minLon,
                                                double& maxLat,
                                                double& maxLon)const
{
   bool validChildBounds = false;
   double tempMinLat = 0.0;
   double tempMinLon = 0.0;
   double tempMaxLat = 0.0;
   double tempMaxLon = 0.0;
   if(hasChildren())
   {
      unsigned int idx = 0;
      for(idx = 0; idx < theChildren.size(); ++idx)
      {

         if(theChildren[idx].valid()&&
            theChildren[idx]->getLatLonBoundingBox(tempMinLat,
                                                   tempMinLon,
                                                   tempMaxLat,
                                                   tempMaxLon))
         {
            if(!validChildBounds)
            {
               minLat = tempMinLat;
               minLon = tempMinLon;
               maxLat = tempMaxLat;
               maxLon = tempMaxLon;
               validChildBounds = true;
            }
            else
            {
               if(tempMinLat < minLat)
               {
                  minLat = tempMinLat;
               }
               if(tempMinLon < minLon)
               {
                  minLon = tempMinLon;
               }
               if(tempMaxLat > maxLat)
               {
                  maxLat = tempMaxLat;
               }
               if(tempMaxLon > maxLon)
               {
                  maxLon = tempMaxLon;
               }
            }
         }
      }
   }

   if((theLatMin == "") ||
      (theLonMin == "") ||
      (theLatMax == "") ||
      (theLonMax == "") )
   {
      if(validChildBounds)
      {
         return true;
      }
   }

   tempMinLat = atof(theLatMin.c_str());
   tempMinLon = atof(theLonMin.c_str());
   tempMaxLat = atof(theLatMax.c_str());
   tempMaxLon = atof(theLonMax.c_str());

   if(tempMinLat < -90.0)
   {
      tempMinLat = -90.0;
   }
   if(tempMinLon < -180.0)
   {
      tempMinLon = -180.0;
   }
   if(tempMaxLat > 90.0)
   {
      tempMaxLat = 90.0;
   }
   if(tempMaxLon > 180.0)
   {
      tempMaxLon = 180.0;
   }
   if(validChildBounds)
   {
      if(tempMinLat < minLat)
      {
         minLat = tempMinLat;
      }
      if(tempMinLon < minLon)
      {
         minLon = tempMinLon;
      }
      if(tempMaxLat > maxLat)
      {
         maxLat = tempMaxLat;
      }
      if(tempMaxLon > maxLon)
      {
         maxLon = tempMaxLon;
      }
   }
   else
   {
      minLat = tempMinLat;
      minLon = tempMinLon;
      maxLat = tempMaxLat;
      maxLon = tempMaxLon;
   }
   return true;
}
