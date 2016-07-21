//***
// Written by: Kenneth Melero (kmelero@sanz.com)
// ***

#include "../ossimPluginConstants.h"
#include "ossimNdfReaderFactory.h"
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>

static void setDescription(ossimString& description)
{
   description = "NLAPS Data Format (NDF) Reader Plugin\n\n";
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
   OSSIM_PLUGINS_DLL void ossimSharedLibraryInitialize(ossimSharedObjectInfo** info)
   {    
      myInfo.getDescription = getDescription;
      myInfo.getNumberOfClassNames = getNumberOfClassNames;
      myInfo.getClassName = getClassName;
      
      *info = &myInfo;
      
      /**
       * Register the reader...
       */
      ossimImageHandlerRegistry::instance()->
         registerFactory(ossimNdfReaderFactory::instance(), false);
      
      setDescription(theDescription);
   }
   
   /* Note symbols need to be exported on windoze... */ 
   OSSIM_PLUGINS_DLL void ossimSharedLibraryFinalize()
   {
      ossimImageHandlerRegistry::instance()->unregisterFactory(ossimNdfReaderFactory::instance());
   }
}
