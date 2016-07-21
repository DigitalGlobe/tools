#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include "ossimWorldWindFactory.h"
#include "ossimWorldWindExports.h"

static void setDescription(ossimString& description)
{
   description = "World Wind Plugin";
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
   OSSIM_WORLDWIND_DLL void ossimSharedLibraryInitialize(
      ossimSharedObjectInfo** info)
   {    
      myInfo.getDescription = (ossimSharedLibraryGetDescriptionPtr)getDescription;
      myInfo.getNumberOfClassNames = (ossimSharedLibraryGetNumberOfClassNamesPtr)getNumberOfClassNames;
      myInfo.getClassName = (ossimSharedLibraryGetClassNamesPtr)getClassName;
      
      *info = &myInfo;

      /* Register the readers... */
      ossimImageHandlerRegistry::instance()->registerFactory(ossimWorldWindFactory::instance());
      
     /* Register the writers... */
     
     setDescription(theDescription);
  }

   /* Note symbols need to be exported on windoze... */ 
   OSSIM_WORLDWIND_DLL void ossimSharedLibraryFinalize()
   {
      
   }
}
