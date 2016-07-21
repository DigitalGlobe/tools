//*******************************************************************
// Copyright (C) 2009 Garrett Potts.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//*******************************************************************
//  $Id$

#include "ossimNitroImageHandlerFactory.h"
#include "ossimNitroHandler.h"
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>

static const ossimTrace traceDebug("ossimNitroImageHandlerFactory:debug");

RTTI_DEF1(ossimNitroImageHandlerFactory, "ossimNitroImageHandlerFactory", ossimImageHandlerFactoryBase);

ossimNitroImageHandlerFactory* ossimNitroImageHandlerFactory::theInstance = 0;
ossimNitroImageHandlerFactory::~ossimNitroImageHandlerFactory()
{
   theInstance = (ossimNitroImageHandlerFactory*)NULL;
}


ossimNitroImageHandlerFactory* ossimNitroImageHandlerFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimNitroImageHandlerFactory;
   }

   return theInstance;
}
   
ossimImageHandler* ossimNitroImageHandlerFactory::open(const ossimFilename& fileName)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimNitroImageHandlerFactory::open(filename) DEBUG: entered..." << std::endl;
   }
   
   ossimImageHandler* result = NULL;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitroImageHandlerFactory::open(filename) DEBUG:"
         << "\ntrying ossimGdalTileSource"
         << std::endl;
   }
   
   // make sure you put this last
   //
   result = new ossimNitroHandler;
   if(result->open(fileName))
   {
      return result;
   }
   delete result;
   result = 0;
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitroImageHandlerFactory::open(filename) DEBUG: leaving..."
         << std::endl;
   }
   
   return result;
}

ossimImageHandler* ossimNitroImageHandlerFactory::open(const ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   ossimImageHandler* result = (ossimImageHandler*)NULL;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimNitroImageHandlerFactory::open(kwl, prefix) DEBUG: entered..." << std::endl;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimNitroImageHandlerFactory::open(kwl, prefix) DEBUG: trying ossimNitroHandler" << std::endl;
   }
   result = new ossimNitroHandler;
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;
   result = (ossimImageHandler*)NULL;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimNitroImageHandlerFactory::open(kwl, prefix) DEBUG: leaving..." << std::endl;
   }
   
   return result;
}

ossimObject* ossimNitroImageHandlerFactory::createObject(const ossimString& typeName)const
{
   if(STATIC_TYPE_NAME(ossimNitroHandler) == typeName)
   {
      return new ossimNitroHandler();
   }
   
   return (ossimObject*)NULL;
}

ossimObject* ossimNitroImageHandlerFactory::createObject(const ossimKeywordlist& kwl,
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
 
void ossimNitroImageHandlerFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimNitroHandler));
}

void ossimNitroImageHandlerFactory::getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   extensionList.push_back("ntf");
   extensionList.push_back("nsf");
   extensionList.push_back("nitf");
}

