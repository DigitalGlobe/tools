//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  Mingjie Su
//
// Description: OSSIM HDF plugin initialization
// code.
//
//----------------------------------------------------------------------------
// $Id: ossimHdfPluginInit.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/support_data/ossimInfoFactoryRegistry.h>

#include "../ossimPluginConstants.h"
#include "ossimHdfReaderFactory.h"
#include "ossimHdfInfoFactory.h"
#include "ossimHdfProjectionFactory.h"

static void setDescription(ossimString& description)
{
   description = "Hdf reader plugin\n\n";
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
         registerFactory(ossimHdfReaderFactory::instance());

      /* Register hdf info factoy... */
      ossimInfoFactoryRegistry::instance()->
         registerFactory(ossimHdfInfoFactory::instance());

      /* Register hdf projection factoy... */
      ossimProjectionFactoryRegistry::instance()->
         registerFactoryToFront(ossimHdfProjectionFactory::instance());

      setDescription(theDescription);
   }

   /* Note symbols need to be exported on windoze... */ 
   OSSIM_PLUGINS_DLL void ossimSharedLibraryFinalize()
   {
      ossimImageHandlerRegistry::instance()->
         unregisterFactory(ossimHdfReaderFactory::instance());

      ossimInfoFactoryRegistry::instance()->
         unregisterFactory(ossimHdfInfoFactory::instance());

      ossimProjectionFactoryRegistry::instance()->
         unregisterFactory(ossimHdfProjectionFactory::instance());
   }
}
