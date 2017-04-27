//*******************************************************************
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//*******************************************************************
//  $Id: ossimContribPluginInit.cpp 23664 2015-12-14 14:17:27Z dburken $
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include "ossimContribExports.h"
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include "ossimContribImageSourceFactory.h"
#include <ossim/base/ossimString.h>

extern "C"
{
   static ossimSharedObjectInfo theContribInfo;
   static ossimString theContribDescription;
   static std::vector<ossimString> theContribObjectList;

   static const char* getContribDescription()
   {
      return theContribDescription.c_str();
   }

   static int getContribNumberOfClassNames()
   {
      return (int)theContribObjectList.size();
   }

   static const char* getContribClassName(int idx)
   {
      if(idx < (int)theContribObjectList.size())
      {
         return theContribObjectList[idx].c_str();
      }
      
      return 0;
   }

   /* Note symbols need to be exported on windoze... */ 
   OSSIM_CONTRIB_DLL void ossimSharedLibraryInitialize(ossimSharedObjectInfo** info)
   {
      theContribInfo.getDescription = getContribDescription;
      theContribInfo.getNumberOfClassNames = getContribNumberOfClassNames;
      theContribInfo.getClassName = getContribClassName;

      theContribDescription = "Shared Library for Contributions from the OSSIM community\n\n"
                               "Contributors:\n\n"
                               "   Apogee Imaging International\n"
                               "      Contributions include the CFAR filter.";
      ossimImageSourceFactoryRegistry::instance()->registerFactory(ossimContribImageSourceFactory::instance());

      if(!theContribObjectList.size())
      {
         ossimImageSourceFactoryRegistry::instance()->getTypeNameList(theContribObjectList);
      }
      *info = &theContribInfo;
  }

   /* Note symbols need to be exported on windoze... */ 
  OSSIM_CONTRIB_DLL void ossimSharedLibraryFinalize()
  {
     ossimImageSourceFactoryRegistry::instance()->unregisterFactory(ossimContribImageSourceFactory::instance());
  }
}

