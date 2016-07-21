#include "ossimRegistrationMiscFactory.h"
#include "ossimImageCorrelator.h"
#include "ossimOutlierRejection.h"
#include "ossimModelOptimizer.h"
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimRegistrationMiscFactory, "ossimRegistrationMiscFactory", ossimObjectFactory);

ossimRegistrationMiscFactory* ossimRegistrationMiscFactory::theInstance = NULL;

ossimRegistrationMiscFactory* ossimRegistrationMiscFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimRegistrationMiscFactory;
   }

   return theInstance;
}

ossimObject* ossimRegistrationMiscFactory::createObject(const ossimString& typeName)const
{
   if(typeName == STATIC_TYPE_NAME(ossimImageCorrelator))
   {
      return new ossimImageCorrelator;
   }
   if(typeName == STATIC_TYPE_NAME(ossimOutlierRejection))
   {
      return new ossimOutlierRejection;
   }
   if(typeName == STATIC_TYPE_NAME(ossimModelOptimizer))
   {
      return new ossimModelOptimizer;
   }

   return (ossimObject*)NULL;
}

ossimObject* ossimRegistrationMiscFactory::createObject(const ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   const char* type = kwl.find(prefix,
                               ossimKeywordNames::TYPE_KW);
   ossimString copyPrefix = prefix;
   if(type)
   {
      ossimObject* object = createObject(ossimString(type));
      if(object)
      {
         object->loadState(kwl, copyPrefix.c_str());
      }
      return object;
   }
   return NULL;  
}

void ossimRegistrationMiscFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimImageCorrelator));
   typeList.push_back(STATIC_TYPE_NAME(ossimOutlierRejection));
   typeList.push_back(STATIC_TYPE_NAME(ossimModelOptimizer));
}
