//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  David Burken
//
// Description: OSSIM Portable Network Graphics (PNG) plugin initialization
// code.
//
//----------------------------------------------------------------------------
// $Id: ossimLibRawPluginInit.cpp 11046 2007-05-25 18:03:03Z gpotts $

#include <ossim/plugin/ossimSharedObjectBridge.h>
#include "ossimPluginConstants.h"
#include "ossimLibRawReaderFactory.h"
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <libraw/libraw.h>
static void setDescription(ossimString& description)
{
   description = "LIBRAW reader plugin version: " + ossimString(libraw_version()) + "\nSupports " + ossimString::toString(libraw_cameraCount()) + " camera formats:\n\n";
   
   static const char**         cameraList = libraw_cameraList();
   int                  count = libraw_cameraCount();
   
   int idx = 0;
   
   for(idx = 0; idx < count; ++idx)
   {
      if(cameraList[idx])
      {
         description += ossimString(cameraList[idx]);
         description += "\n";
      }
   }

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
                                                       ossimSharedObjectInfo** info, 
                                                       const char* /*options*/)
   {    
      myInfo.getDescription = getDescription;
      myInfo.getNumberOfClassNames = getNumberOfClassNames;
      myInfo.getClassName = getClassName;
      
      *info = &myInfo;
      
      /* Register the readers... */
      ossimImageHandlerRegistry::instance()->
        registerFactory(ossimLibRawReaderFactory::instance());
      
      setDescription(theDescription);
  }

   /* Note symbols need to be exported on windoze... */ 
  OSSIM_PLUGINS_DLL void ossimSharedLibraryFinalize()
  {
     ossimImageHandlerRegistry::instance()->
        unregisterFactory(ossimLibRawReaderFactory::instance());
  }
}
