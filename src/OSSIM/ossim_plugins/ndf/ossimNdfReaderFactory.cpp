//***
// Written by: Kenneth Melero (kmelero@sanz.com)
//
// $Id$
// ***

#include "ossimNdfReaderFactory.h"
#include "ossimNdfTileSource.h"
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>

static const ossimTrace traceDebug("ossimNdfReaderFactory:debug");

RTTI_DEF1(ossimNdfReaderFactory, "ossimNdfReaderFactory", ossimImageHandlerFactoryBase);

ossimNdfReaderFactory* ossimNdfReaderFactory::theInstance = 0;

ossimNdfReaderFactory::~ossimNdfReaderFactory()
{
   theInstance = 0;
}

ossimNdfReaderFactory* ossimNdfReaderFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimNdfReaderFactory;
   }
   return theInstance;
}

ossimImageHandler* ossimNdfReaderFactory::open(const ossimFilename& fileName,
                                               bool openOverview) const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNdfReaderFactory::open(filename) DEBUG: entered..."
         << "\nTrying ossimNdfTileSource"
         << std::endl;
   }
   
   ossimRefPtr<ossimImageHandler> reader = new ossimNdfTileSource;
   reader->setOpenOverviewFlag(openOverview);
   if(reader->open(fileName) == false)
   {
      reader = 0;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNdfReaderFactory::open(filename) DEBUG: exit status = "
         << (reader.get()?"success":"failure")
         << std::endl;
   }
   
   return reader.release();
}

ossimImageHandler* ossimNdfReaderFactory::open(const ossimKeywordlist& kwl,
                                               const char* prefix) const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNdfReaderFactory::open(kwl, prefix) DEBUG: entered..."
         << "Trying ossimNdfTileSource"
         << std::endl;
   }
   
   ossimRefPtr<ossimImageHandler> reader = new ossimNdfTileSource;
   if(reader->loadState(kwl, prefix) == false)
   {
      reader = 0;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNdfReaderFactory::open(kwl, prefix) DEBUG: leaving..."
         << std::endl;
   }
   
   return reader.release();
}

ossimObject* ossimNdfReaderFactory::createObject(const ossimString& typeName) const
{
   if(typeName == "ossimNdfTileSource")
   {
      return new ossimNdfTileSource;
   }
   return 0;
}

ossimObject* ossimNdfReaderFactory::createObject(const ossimKeywordlist& kwl,
                                                 const char* prefix) const
{
   return this->open(kwl, prefix);
}
 
void ossimNdfReaderFactory::getTypeNameList(std::vector<ossimString>& typeList) const
{
   typeList.push_back(ossimString("ossimNdfTileSource"));
}

void ossimNdfReaderFactory::getSupportedExtensions(
   ossimImageHandlerFactoryBase::UniqueStringList& extensionList) const
{
   extensionList.push_back(ossimString("h1"));
   extensionList.push_back(ossimString("h2"));
   extensionList.push_back(ossimString("h3"));
   extensionList.push_back(ossimString("hd"));
}

ossimNdfReaderFactory::ossimNdfReaderFactory() {}

ossimNdfReaderFactory::ossimNdfReaderFactory(const ossimNdfReaderFactory&) {}

void ossimNdfReaderFactory::operator=(const ossimNdfReaderFactory&) {}
