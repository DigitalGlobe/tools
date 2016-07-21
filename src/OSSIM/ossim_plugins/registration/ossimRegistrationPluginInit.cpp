//*******************************************************************
// Copyright (C) 2005 David Burken, all rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//*******************************************************************
//  $Id: ossimRegistrationPluginInit.cpp 23664 2015-12-14 14:17:27Z dburken $
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include "ossimRegistrationExports.h"
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include "ossimRegistrationImageSourceFactory.h"
#include "ossimRegistrationMiscFactory.h"

static void setDescription(ossimString& description)
{
   description = "Registration Plugin\n\n";
}

extern "C"
{
   static ossimSharedObjectInfo  myInfo;
   static ossimString theDescription;
   static std::vector<ossimString> theObjList;
   static const char* getDescription()
   {
      return theDescription.c_str();
   }
   static int getNumberOfClassNames()
   {
      return (int)theObjList.size();
   }
   static const char* getClassName(int idx)
   {
      if(idx < (int)theObjList.size())
      {
         return theObjList[0].c_str();
      }
      return (const char*)0;
   }

   /* Note symbols need to be exported on windoze... */ 
   OSSIM_REGISTRATION_DLL void ossimSharedLibraryInitialize(
                                                            ossimSharedObjectInfo** info, 
                                                            const char* options)
   {
      myInfo.getDescription = getDescription;
      myInfo.getNumberOfClassNames = getNumberOfClassNames;
      myInfo.getClassName = getClassName;
      setDescription(theDescription);
      ossimImageSourceFactoryRegistry::instance()->registerFactory(ossimRegistrationImageSourceFactory::instance());
      ossimObjectFactoryRegistry::instance()->registerFactory(ossimRegistrationMiscFactory::instance());

      if(!theObjList.size())
      {
         ossimRegistrationImageSourceFactory::instance()->getTypeNameList(theObjList);
         ossimRegistrationMiscFactory::instance()->getTypeNameList(theObjList);
      }
      *info = &myInfo;
  }

   /* Note symbols need to be exported on windoze... */ 
  OSSIM_REGISTRATION_DLL void ossimSharedLibraryFinalize()
  {
     ossimImageSourceFactoryRegistry::instance()->unregisterFactory(ossimRegistrationImageSourceFactory::instance());
     ossimObjectFactoryRegistry::instance()->unregisterFactory(ossimRegistrationMiscFactory::instance());
     
  }
}

