#include "ossimUnitTestImageSourceFactory.h"
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include "ossimUnitTestPolyCutter.h"

RTTI_DEF1(ossimUnitTestImageSourceFactory, "ossimUnitTestImageSourceFactory", ossimImageSourceFactoryBase);

ossimUnitTestImageSourceFactory* ossimUnitTestImageSourceFactory::theInstance=0;
static ossimTrace traceDebug("ossimUnitTestImageSourceFactory:debug");

ossimUnitTestImageSourceFactory::ossimUnitTestImageSourceFactory()
{
   theInstance = this;
}

ossimUnitTestImageSourceFactory::ossimUnitTestImageSourceFactory(const ossimUnitTestImageSourceFactory& rhs)
{
   theInstance = this;
}

const ossimUnitTestImageSourceFactory& ossimUnitTestImageSourceFactory::operator=(ossimUnitTestImageSourceFactory&)
{
   return *this;
}

ossimUnitTestImageSourceFactory::~ossimUnitTestImageSourceFactory()
{
   theInstance = NULL;
   ossimObjectFactoryRegistry::instance()->unregisterFactory(this);
}

ossimUnitTestImageSourceFactory* ossimUnitTestImageSourceFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimUnitTestImageSourceFactory;
   }

   return theInstance;
}

ossimObject* ossimUnitTestImageSourceFactory::createObject(const ossimString& name)const
{
   if(name==STATIC_TYPE_NAME(ossimUnitTestPolyCutter))
   {
      return new ossimUnitTestPolyCutter;
   }
   return 0;
}

ossimObject* ossimUnitTestImageSourceFactory::createObject(const ossimKeywordlist& kwl,
                                                   const char* prefix)const
{
   static const char* MODULE = "ossimUnitTestImageSourceFactory::createObject";
   
   ossimString copyPrefix = prefix;
   ossimObject* result = NULL;
   
   if(traceDebug())
   {
      CLOG << "looking up type keyword for prefix = " << copyPrefix << std::endl;
   }

   const char* lookup = kwl.find(copyPrefix, "type");
   if(lookup)
   {
      ossimString name = lookup;
      result           = createObject(name);
      
      if(result)
      {
         if(traceDebug())
         {
            CLOG << "found source " << result->getClassName() << " now loading state" << std::endl;
         }
         result->loadState(kwl, copyPrefix.c_str());
      }
      else
      {
         if(traceDebug())
         {
            CLOG << "type not found " << lookup << std::endl;
         }
      }
   }
   else
   {
      if(traceDebug())
      {
         CLOG << "type keyword not found" << std::endl;
      }
   }
   return result;
}

void ossimUnitTestImageSourceFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back("ossimUnitTestPolyCutter");
}
