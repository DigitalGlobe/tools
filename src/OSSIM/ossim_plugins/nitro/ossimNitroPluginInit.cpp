//*******************************************************************
// Copyright (C) 2005 David Burken, all rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//*******************************************************************
//  $Id: ossimGdalPluginInit.cpp 13341 2008-07-29 17:27:02Z dburken $
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossimPluginConstants.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include "ossimNitroImageHandlerFactory.h"

static void setDescription(ossimString& description)
{
   description = "NITRO Plugin\n\n";
}


extern "C"
{
   ossimSharedObjectInfo  myInfo;
   ossimString theDescription;
   std::vector<ossimString> theObjList;
   const char* getDescription()
   {
      return theDescription.c_str();
   }
   int getNumberOfClassNames()
   {
      return (int)theObjList.size();
   }
   const char* getClassName(int idx)
   {
      if(idx < (int)theObjList.size())
      {
         return theObjList[0].c_str();
      }
      return (const char*)0;
   }

   /* Note symbols need to be exported on windoze... */ 
   OSSIM_PLUGINS_DLL void ossimSharedLibraryInitialize(
      ossimSharedObjectInfo** info)
   {    
      myInfo.getDescription = getDescription;
      myInfo.getNumberOfClassNames = getNumberOfClassNames;
      myInfo.getClassName = getClassName;
      
      *info = &myInfo;

      /* Register the readers... */
     ossimImageHandlerRegistry::instance()->
        registerFactory(ossimNitroImageHandlerFactory::instance(), true);

#if 0
     /* Register the writers... */
     ossimImageWriterFactoryRegistry::instance()->
        registerFactory(ossimGdalImageWriterFactory::instance());

     /* Register the overview builder factory. */
     ossimOverviewBuilderFactoryRegistry::instance()->
        registerFactory(ossimGdalOverviewBuilderFactory::instance());

     ossimProjectionFactoryRegistry::instance()->
        registerFactory(ossimGdalProjectionFactory::instance());

     /* Register generic objects... */
     ossimObjectFactoryRegistry::instance()->
        registerFactory(ossimGdalObjectFactory::instance());
#endif
     setDescription(theDescription);
  }

   /* Note symbols need to be exported on windoze... */ 
  OSSIM_PLUGINS_DLL void ossimSharedLibraryFinalize()
  {
     ossimImageHandlerRegistry::instance()->
        unregisterFactory(ossimNitroImageHandlerFactory::instance());
#if 0

     ossimImageWriterFactoryRegistry::instance()->
        unregisterFactory(ossimGdalImageWriterFactory::instance());

     ossimOverviewBuilderFactoryRegistry::instance()->
        unregisterFactory(ossimGdalOverviewBuilderFactory::instance());

     ossimProjectionFactoryRegistry::instance()->unregisterFactory(ossimGdalProjectionFactory::instance());

     ossimObjectFactoryRegistry::instance()->
        unregisterFactory(ossimGdalObjectFactory::instance());
 
#endif
  }
}

