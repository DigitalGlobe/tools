//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  Mingjie Su
//
// Description: Factory for OSSIM HDF reader using Hdf4 and Hdf5 libraries.
//----------------------------------------------------------------------------
// $Id: ossimHdfReaderFactory.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

#include "ossimHdfReaderFactory.h"
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageHandler.h>
#include "ossimHdf4Reader.h"

static const ossimTrace traceDebug("ossimHdfReaderFactory:debug");

class ossimImageHandler;

RTTI_DEF1(ossimHdfReaderFactory,
          "ossimHdfReaderFactory",
          ossimImageHandlerFactoryBase);

ossimHdfReaderFactory* ossimHdfReaderFactory::theInstance = 0;

ossimHdfReaderFactory::~ossimHdfReaderFactory()
{
   theInstance = 0;
}

ossimHdfReaderFactory* ossimHdfReaderFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimHdfReaderFactory;
   }
   return theInstance;
}

ossimImageHandler* ossimHdfReaderFactory::open(const ossimFilename& fileName,
                                               bool openOverview)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHdfReaderFactory::open(filename, openOverview) DEBUG: entered..."
         << std::endl;
   }

   ossimRefPtr<ossimImageHandler> reader = 0;

   if ( hasExcludedExtension(fileName) == false )
   {
      if ( !reader.valid()) //try hdf4 reader
      {
         // cout << "Calling ossimHdf4Reader ***********************" << endl;
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "trying ossimHdf4Reader..."
               << std::endl;
         }
         reader = new ossimHdf4Reader;
         reader->setOpenOverviewFlag(openOverview);
         if(reader->open(fileName) == false)
         {
            reader = 0;
         }
      }
    
      if(!reader.valid())
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "Could not open any handler..."
               << std::endl;
         }
      }  
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHdfReaderFactory::open(filename, openOverview) DEBUG: leaving..."
         << std::endl;
   }

   return reader.release();
}

ossimImageHandler* ossimHdfReaderFactory::open(const ossimKeywordlist& kwl,
                                               const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHdfReaderFactory::open(kwl, prefix) DEBUG: entered..."
         << "Trying ossimKakaduNitfReader"
         << std::endl;
   }

   ossimString type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   ossimRefPtr<ossimImageHandler> reader;
   if(type)
   {
      if( (type == "ossimHdf4Reader") ||
          (type == "ossimImageHandler") )
      {
         reader = new ossimHdf4Reader();
      }
   }
   if(reader.valid())
   {
      if(!reader->loadState(kwl, prefix))
      {
         reader = 0;
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHdfReaderFactory::open(kwl, prefix) DEBUG: leaving..."
         << std::endl;
   }

   return reader.release();
}

ossimObject* ossimHdfReaderFactory::createObject(
   const ossimString& typeName)const
{
   ossimRefPtr<ossimObject> result = 0;
   if(typeName == STATIC_TYPE_NAME(ossimHdf4Reader))
   {
      result = new ossimHdf4Reader;
   }
   return result.release();
}

ossimObject* ossimHdfReaderFactory::createObject(
   const ossimKeywordlist& kwl,
   const char* prefix)const
{
   return this->open(kwl, prefix);
}

void ossimHdfReaderFactory::getTypeNameList(
   std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimHdf4Reader));
}

void ossimHdfReaderFactory::getSupportedExtensions(
   ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   extensionList.push_back(ossimString("h4"));
   extensionList.push_back(ossimString("he4"));
   extensionList.push_back(ossimString("hdf4"));
}

bool ossimHdfReaderFactory::hasExcludedExtension(
   const ossimFilename& file) const
{
   bool result = true;
   ossimString ext = file.ext().downcase();
   //only include the file with those extension and exclude any other files
   if ( ext == "hdf" || ext == "h4" || ext == "hdf4" || 
      ext == "he4" || ext == "hdf5" || ext == "he5" || ext == "h5" || 
      ext == "l1r")
   {
      result = false;
   }
   return result;
}

ossimHdfReaderFactory::ossimHdfReaderFactory(){}

ossimHdfReaderFactory::ossimHdfReaderFactory(const ossimHdfReaderFactory&){}

void ossimHdfReaderFactory::operator=(const ossimHdfReaderFactory&){}

