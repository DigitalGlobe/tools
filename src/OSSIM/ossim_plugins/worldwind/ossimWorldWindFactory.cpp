//*******************************************************************
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//*******************************************************************
//  $Id: ossimWorldWindFactory.cpp 23664 2015-12-14 14:17:27Z dburken $

#include "ossimWorldWindFactory.h"
#include "ossimWorldWindHandler.h"
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>

static const ossimTrace traceDebug("ossimWorldWindFactory:debug");

RTTI_DEF1(ossimWorldWindFactory, "ossimWorldWindFactory", ossimImageHandlerFactoryBase);

ossimWorldWindFactory* ossimWorldWindFactory::theInstance = 0;
ossimWorldWindFactory::~ossimWorldWindFactory()
{
   theInstance = (ossimWorldWindFactory*)NULL;
}


ossimWorldWindFactory* ossimWorldWindFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimWorldWindFactory;
   }


   return theInstance;
}
   
ossimImageHandler* ossimWorldWindFactory::open( const ossimFilename& fileName,
                                                bool openOverview )const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimWorldWindFactory::open(filename) DEBUG: entered..." << std::endl;
   }
   
   ossimRefPtr<ossimImageHandler> result;
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimWorldWindFactory::open(filename) DEBUG:"
         << "\ntrying ossimWorldWindHandler"
         << std::endl;
   }
   
   // make sure you put this last
   //
   result = new ossimWorldWindHandler;
   result->setOpenOverviewFlag(openOverview);
   if(!result->open(fileName))
   {
      result = 0;
   }
   return result.release();
}

ossimImageHandler* ossimWorldWindFactory::open(const ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   ossimRefPtr<ossimImageHandler> result;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimWorldWindFactory::open(kwl, prefix) DEBUG: entered..." << std::endl;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimWorldWindFactory::open(kwl, prefix) DEBUG: trying ossimWorldWindHandler" << std::endl;
   }
   result = new ossimWorldWindHandler;
   if(!result->loadState(kwl, prefix))
   {
      result = 0;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimWorldWindFactory::open(kwl, prefix) DEBUG: leaving..." << std::endl;
   }
   
   return result.release();
}

ossimObject* ossimWorldWindFactory::createObject(const ossimString& typeName)const
{
   if(STATIC_TYPE_NAME(ossimWorldWindHandler) == typeName)
   {
      return new ossimWorldWindHandler();
   }
   
   return (ossimObject*)NULL;
}

ossimObject* ossimWorldWindFactory::createObject(const ossimKeywordlist& kwl,
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
 
void ossimWorldWindFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimWorldWindHandler));
}

void ossimWorldWindFactory::getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   extensionList.push_back("xml");
}

