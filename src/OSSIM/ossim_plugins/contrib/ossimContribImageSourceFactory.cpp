#include "ossimContribImageSourceFactory.h"
#include "ossimCFARFilter.h"
#include "ossimSPOTNatural.h"
#include "ossimComplexAbs.h"
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimContribImageSourceFactory, "ossimContribImageSourceFactory", ossimContribImageSourceFactory);

ossimContribImageSourceFactory* ossimContribImageSourceFactory::theInstance = 0;

ossimContribImageSourceFactory* ossimContribImageSourceFactory::instance()
{
   if(!theInstance)
   {
     theInstance = new ossimContribImageSourceFactory;
   }

   return theInstance;
}

ossimObject* ossimContribImageSourceFactory::createObject(const ossimString& typeName)const
{
   if(typeName == STATIC_TYPE_NAME(ossimCFARFilter))
   {
      return new ossimCFARFilter;
   }
   if(typeName == STATIC_TYPE_NAME(ossimSPOTNatural))
   {
      return new ossimSPOTNatural;
   }
   if(typeName == STATIC_TYPE_NAME(ossimComplexAbs))
   {
      return new ossimComplexAbs;
   }

   return (ossimObject*)NULL;
}

ossimObject* ossimContribImageSourceFactory::createObject(const ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   const char* type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   ossimObject* result = NULL;
   if(type)
   {
      result = createObject(ossimString(type));
      if(result)
      {
         result->loadState(kwl, prefix);
      }
   }

   return result;
}

void ossimContribImageSourceFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimCFARFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimSPOTNatural));
   typeList.push_back(STATIC_TYPE_NAME(ossimComplexAbs));
}

ossimContribImageSourceFactory::~ossimContribImageSourceFactory()
{
   ossimImageSourceFactoryRegistry::instance()->unregisterFactory(theInstance);
   theInstance = 0;
}
