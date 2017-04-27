//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  David Burken
//
// Description: Factory for OSSIM Portable Network Graphics (PNG) reader
// (tile source).
//----------------------------------------------------------------------------
// $Id: ossimLibRawReaderFactory.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include "ossimLibRawReaderFactory.h"
#include "ossimLibRawReader.h"
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>

static const ossimTrace traceDebug("ossimLibRawReaderFactory:debug");

RTTI_DEF1(ossimLibRawReaderFactory,
          "ossimLibRawReaderFactory",
          ossimImageHandlerFactoryBase);

ossimLibRawReaderFactory* ossimLibRawReaderFactory::theInstance = 0;

ossimLibRawReaderFactory::~ossimLibRawReaderFactory()
{
   theInstance = 0;
}

ossimLibRawReaderFactory* ossimLibRawReaderFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimLibRawReaderFactory;
   }
   return theInstance;
}
   
ossimImageHandler* ossimLibRawReaderFactory::open(
   const ossimFilename& fileName, bool openOverview)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimLibRawReaderFactory::open(filename) DEBUG: entered..."
         << "\ntrying ossimLibRawReader"
         << std::endl;
   }
   
   ossimRefPtr<ossimImageHandler> reader = new ossimLibRawReader;
   reader->setOpenOverviewFlag(openOverview);
   if(reader->open(fileName) == false)
   {
      reader = 0;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimLibRawReaderFactory::open(filename) DEBUG: leaving..."
         << std::endl;
   }
   
   return reader.release();
}

ossimImageHandler* ossimLibRawReaderFactory::open(const ossimKeywordlist& kwl,
                                               const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimLibRawReaderFactory::open(kwl, prefix) DEBUG: entered..."
         << "Trying ossimLibRawReader"
         << std::endl;
   }

   ossimRefPtr<ossimImageHandler> reader = new ossimLibRawReader;
   if(reader->loadState(kwl, prefix) == false)
   {
      reader = 0;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimLibRawReaderFactory::open(kwl, prefix) DEBUG: leaving..."
         << std::endl;
   }
   
   return reader.release();
}

ossimObject* ossimLibRawReaderFactory::createObject(
   const ossimString& typeName)const
{
   ossimRefPtr<ossimObject> result = 0;
   if(typeName == "ossimLibRawReader")
   {
      result = new ossimLibRawReader;
   }
   return result.release();
}

ossimObject* ossimLibRawReaderFactory::createObject(const ossimKeywordlist& kwl,
                                                 const char* prefix)const
{
   return this->open(kwl, prefix);
}
 
void ossimLibRawReaderFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(ossimString("ossimLibRawReader"));
}

void ossimLibRawReaderFactory::getSupportedExtensions(
   ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
}

ossimLibRawReaderFactory::ossimLibRawReaderFactory(){}

ossimLibRawReaderFactory::ossimLibRawReaderFactory(const ossimLibRawReaderFactory&){}

void ossimLibRawReaderFactory::operator=(const ossimLibRawReaderFactory&){}
