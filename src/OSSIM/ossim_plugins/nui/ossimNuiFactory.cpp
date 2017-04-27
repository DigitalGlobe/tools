//*******************************************************************
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//*******************************************************************
//  $Id: ossimNuiFactory.cpp 23664 2015-12-14 14:17:27Z dburken $

#include "ossimNuiFactory.h"
#include "ossimNuiHandler.h"
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <nuiSDK.h>

static const ossimTrace traceDebug("ossimNuiFactory:debug");

RTTI_DEF1(ossimNuiFactory, "ossimNuiFactory", ossimImageHandlerFactoryBase);

ossimNuiFactory* ossimNuiFactory::theInstance = 0;
ossimNuiFactory::~ossimNuiFactory()
{
   theInstance = (ossimNuiFactory*)NULL;
}


ossimNuiFactory* ossimNuiFactory::instance()
{
   
   if(!theInstance)
   {
      theInstance = new ossimNuiFactory;
   }
   
   return theInstance;
}
   
ossimImageHandler* ossimNuiFactory::open(const ossimFilename& fileName)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimNuiFactory::open(filename) DEBUG: entered..." << std::endl;
   }
   
   ossimRefPtr<ossimImageHandler> result;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNuiFactory::open(filename) DEBUG:"
         << "\ntrying ossimNuiHandler"
         << std::endl;
   }
   
   // make sure you put this last
   //
   result = new ossimNuiHandler;
   if(!result->open(fileName))
   {
      result = 0;
   }
   
   return result.release();
}

ossimImageHandler* ossimNuiFactory::open(const ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   ossimRefPtr<ossimImageHandler> result;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimNuiFactory::open(kwl, prefix) DEBUG: entered..." << std::endl;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimNuiFactory::open(kwl, prefix) DEBUG: trying ossimNuiHandler" << std::endl;
   }
   result = new ossimNuiHandler;
   if(!result->loadState(kwl, prefix))
   {
      result = 0;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimNuiFactory::open(kwl, prefix) DEBUG: leaving..." << std::endl;
   }
   
   return result.release();
}

ossimObject* ossimNuiFactory::createObject(const ossimString& typeName)const
{
   if(STATIC_TYPE_NAME(ossimNuiHandler) == typeName)
   {
      return new ossimNuiHandler();
   }
   
   return (ossimObject*)NULL;
}

ossimObject* ossimNuiFactory::createObject(const ossimKeywordlist& kwl,
                                                    const char* prefix)const
{
   ossimObject* result = (ossimObject*)NULL;
   const char* type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(type)
   {
      if (ossimString(type).trim() == STATIC_TYPE_NAME(ossimImageHandler))
      {
         const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
         if (lookup)
         {
            // Call the open that takes a filename...
            result = this->open(kwl, prefix);
         }
      }
      else
      {
         result = createObject(ossimString(type));
         if(result)
         {
            result->loadState(kwl, prefix);
         }
      }
   }

   return result;
}
 
void ossimNuiFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimNuiHandler));
}

void ossimNuiFactory::getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   extensionList.push_back("nui");
}

