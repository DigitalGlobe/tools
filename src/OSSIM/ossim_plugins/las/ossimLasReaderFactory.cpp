//----------------------------------------------------------------------------
//
// File: ossimLasReaderFactory.cpp
// 
// License:  See top level LICENSE.txt file
//
// Author:  David Burken
//
// Description: Reader factory for OSSIM LAS plugin.
//----------------------------------------------------------------------------
// $Id: ossimLasReaderFactory.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

#include "ossimLasReaderFactory.h"
#include "ossimLasReader.h"
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>

static const ossimTrace traceDebug("ossimLasReaderFactory:debug");

RTTI_DEF1(ossimLasReaderFactory,
          "ossimLasReaderFactory",
          ossimImageHandlerFactoryBase);

ossimLasReaderFactory* ossimLasReaderFactory::theInstance = 0;

ossimLasReaderFactory::~ossimLasReaderFactory()
{
   theInstance = 0;
}

ossimLasReaderFactory* ossimLasReaderFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimLasReaderFactory;
   }
   return theInstance;
}
   
ossimImageHandler* ossimLasReaderFactory::open(
   const ossimFilename& fileName, bool openOverview)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimLasReaderFactory::open(filename) DEBUG: entered..."
         << "\ntrying ossimLasReader"
         << std::endl;
   }
   
   ossimRefPtr<ossimImageHandler> reader = 0;

   reader = new ossimLasReader;
   reader->setOpenOverviewFlag(openOverview);
   if(reader->open(fileName) == false)
   {
      reader = 0;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimLasReaderFactory::open(filename) DEBUG: leaving..."
         << std::endl;
   }
   
   return reader.release();
}

ossimImageHandler* ossimLasReaderFactory::open(const ossimKeywordlist& kwl,
                                               const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimLasReaderFactory::open(kwl, prefix) DEBUG: entered..."
         << "Trying ossimLasReader"
         << std::endl;
   }

   ossimRefPtr<ossimImageHandler> reader = new ossimLasReader;
   if(reader->loadState(kwl, prefix) == false)
   {
      reader = 0;
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimLasReaderFactory::open(kwl, prefix) DEBUG: leaving..."
         << std::endl;
   }
   
   return reader.release();
}

ossimObject* ossimLasReaderFactory::createObject(
   const ossimString& typeName)const
{
   ossimRefPtr<ossimObject> result = 0;
   if(typeName == "ossimLasReader")
   {
      result = new ossimLasReader;
   }
   return result.release();
}

ossimObject* ossimLasReaderFactory::createObject(const ossimKeywordlist& kwl,
                                                 const char* prefix)const
{
   return this->open(kwl, prefix);
}
 
void ossimLasReaderFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(ossimString("ossimLasReader"));
}

void ossimLasReaderFactory::getSupportedExtensions(
   ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   extensionList.push_back(ossimString("las"));
}

ossimLasReaderFactory::ossimLasReaderFactory(){}

ossimLasReaderFactory::ossimLasReaderFactory(const ossimLasReaderFactory&){}

void ossimLasReaderFactory::operator=(const ossimLasReaderFactory&){}
