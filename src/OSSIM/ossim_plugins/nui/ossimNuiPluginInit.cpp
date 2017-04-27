#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include "ossimNuiFactory.h"
#include "ossimNuiExports.h"

static void setDescription(ossimString& description)
{
   description = "This plugin supports loaders for Pixia's NUI file format.";
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
   OSSIM_NUI_DLL void ossimSharedLibraryInitialize(
      ossimSharedObjectInfo** info)
   {    
      myInfo.getDescription = (ossimSharedLibraryGetDescriptionPtr)getDescription;
      myInfo.getNumberOfClassNames = (ossimSharedLibraryGetNumberOfClassNamesPtr)getNumberOfClassNames;
      myInfo.getClassName = (ossimSharedLibraryGetClassNamesPtr)getClassName;
      
      *info = &myInfo;

      /* Register the readers... */
      ossimImageHandlerRegistry::instance()->registerFactory(ossimNuiFactory::instance());
      
     /* Register the writers... */
     
     setDescription(theDescription);
  }

   /* Note symbols need to be exported on windoze... */ 
   OSSIM_NUI_DLL void ossimSharedLibraryFinalize()
   {
      
   }
}
