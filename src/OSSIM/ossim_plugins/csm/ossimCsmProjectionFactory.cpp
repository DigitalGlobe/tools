//*****************************************************************************
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains implementation of class ossimMapProjectionFactory
//
//*****************************************************************************
//  $Id: ossimCsmProjectionFactory.cpp 13037 2008-06-18 15:12:55Z gpotts $
#include <sstream>
#include "ossimCsmProjectionFactory.h"
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimTieGptSet.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include "ossimCsmSensorModel.h"
#include "ossimCsmSupport.h"

static ossimTrace traceDebug("ossimCsmProjectionFactory:debug");

ossimCsmProjectionFactory* ossimCsmProjectionFactory::theInstance = 0;

ossimCsmProjectionFactory* ossimCsmProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimCsmProjectionFactory;
   }

   return (ossimCsmProjectionFactory*) theInstance;
}

ossimProjection* ossimCsmProjectionFactory::createProjection(const ossimFilename& filename,
                                                             ossim_uint32 /* entryIdx */)const
{
    ossimRefPtr<ossimCsmSensorModel> model = new ossimCsmSensorModel;

   if(!model->setSensorModel(filename, ossimCsmSupport::instance()->csmPluginDirectory()))
    {
       model = 0;
    }

    return model.release();

}

ossimProjection* ossimCsmProjectionFactory::createProjection(const ossimKeywordlist &keywordList,
                                                             const char *prefix) const
{
   const char *lookup = keywordList.find(prefix, ossimKeywordNames::TYPE_KW);
   ossimProjection* result = 0;
   if(lookup)
   {
      result = createProjection(ossimString(lookup));
      if(result)
      {
         result->loadState(keywordList); 	

      }
   }
   return result;
}

ossimProjection* ossimCsmProjectionFactory::createProjection(const ossimString &name) const
{
   ossimProjection* result = 0;
   
   if(name == STATIC_TYPE_NAME(ossimCsmSensorModel))
   {
      result = new ossimCsmSensorModel();
   }
   
   return result;
}

ossimObject* ossimCsmProjectionFactory::createObject(const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimCsmProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
{
   return createProjection(kwl, prefix);
}

void ossimCsmProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimCsmSensorModel));
}
