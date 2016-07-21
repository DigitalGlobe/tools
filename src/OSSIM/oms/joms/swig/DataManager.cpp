//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: DataManager.cpp 13298 2008-07-25 18:27:49Z dburken $
#include <algorithm>

#include "DataManager.h"
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimIdManager.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimOrthoImageMosaic.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimCibCadrgTileSource.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimOrthoImageMosaic.h>
#include <ossim/imaging/ossimFusionCombiner.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimHistogramEqualization.h>
#include <ossim/imaging/ossimVpfTileSource.h>
#include <ossim/base/ossimProcessListener.h>


RTTI_DEF2(DataManager, "DataManager", ossimObject, ossimListenerManager);

class DataManagerPrivateListener : public ossimConnectableObjectListener,
   public ossimProcessListener
{
public:
   DataManagerPrivateListener(DataManager* manager)
      :theDataManager(manager)
      {
      }

   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);

   virtual void setDataManager(DataManager* dataManager)
      {
         theDataManager = dataManager;
      }
   DataManager* getDataManager()
      {
         return theDataManager;
      }
   const DataManager* getDataManager()const
      {
         return theDataManager;
      }
   virtual void processProgressEvent(ossimProcessProgressEvent& )
      {
//          if(qApp)
//          {
//             qApp->processEvents();
//          }
      }
   
protected:
   DataManager* theDataManager;
   
};

void DataManagerPrivateListener::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if(event.getObject()&&theDataManager)
   {
      ossimConnectableObject* connectable = PTR_CAST(ossimConnectableObject,
                                                     event.getObject());
      if(connectable)
      {
         theDataManager->removeObject(connectable);
      }
   }
}

DataManager::DataManager()
{
   theCurrentIter = theDataTable.begin();
   thePrivateListener = new DataManagerPrivateListener(this);
}

DataManager::~DataManager()
{
   deleteAll();
   if(thePrivateListener)
   {
      delete thePrivateListener;
      thePrivateListener = NULL;
   }
}

void  DataManager::makeUniqueIds()
{
   std::vector<ossimConnectableObject*> objs;
   ossim_uint32 i = 0; 
   getAllObjects(objs);
   
   
   for(i = 0; i < objs.size(); ++i)
   {
      ossimConnectableContainerInterface* containerInterface = PTR_CAST(ossimConnectableContainerInterface, objs[i]);
      if(containerInterface)
      {
         containerInterface->makeUniqueIds();
      }
      else
      {
         objs[i]->setId(ossimIdManager::instance()->generateId());
      }
   }
   theDataTable.clear();
   
   for(i = 0; i < objs.size(); ++i)
   {
      add(objs[i]);
   }
}

ossimConnectableObject* DataManager::insertObject(const ossimString& classNameToInsert,
						       const ossimId& objId)
{
  ossimConnectableObject* obj = getObject(objId);
  ossimConnectableObject* result = NULL;
  ossimConnectableObject* newObj = (ossimConnectableObject*)ossimObjectFactoryRegistry::instance()->createObject(classNameToInsert);
  ossimImageChain* defaultChain = PTR_CAST(ossimImageChain, obj);

  if(!newObj) return false;
  // check to see if its an image space filter to add
  //
  if(classNameToInsert == "ossimImageToPlaneNormalFilter")
    {
      ossimConnectableObject* objRenderer = obj->findObjectOfType("ossimImageRenderer",
							    ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT);
      if(objRenderer)
	{
	  ossimImageChain* chain = PTR_CAST(ossimImageChain, objRenderer->getOwner());
	  
	  if(chain)
	    {
	      if(PTR_CAST(ossimCacheTileSource, objRenderer->getInput()))
		{
		  chain->insertLeft(newObj,
				    objRenderer->getInput());
		  result = PTR_CAST(ossimImageChain, newObj->getOwner());
		}
	      else
		{
		  chain->insertLeft(newObj,
				    objRenderer);
		  result = PTR_CAST(ossimImageChain, newObj->getOwner());
		}
	    }
	}
      else
	{
	  if(defaultChain)
	    {
	      defaultChain->add(newObj);
	      result = defaultChain;
	    }
	}
    }
  else
    {
      if(defaultChain)
	{
	  defaultChain->add(newObj);
	  result = defaultChain;
	}
    }

  return result;
}


std::vector<ossimConnectableObject*> DataManager::createStandardProjectionChain(const char* filename)
{
    std::vector<ossimConnectableObject*> result;

    ossimConnectableObject* obj = NULL;
    
    ossimImageHandler* ih = ossimImageHandlerRegistry::instance()->open(ossimFilename(filename));
    if(ih)
    {
       ossim_uint32 idx = 0;
       obj = createStandardProjectionChain(ih);
       if(obj)
       {
          result.push_back(obj);
       }
       else
       {
          delete ih;
          ih = NULL;
       }
       
       if((ih->getNumberOfEntries() > 1))
       {
          std::vector<ossim_uint32> entryList;
          ih->getEntryList(entryList);
          for(idx = 1; idx < ih->getNumberOfEntries(); ++idx)
          {
             ossimImageHandler* tempIh
                = PTR_CAST(ossimImageHandler,
                           (ossimImageHandlerRegistry::instance()->
                            createObject(ih->getClassName())));
             
             if(tempIh)
             {
                if(tempIh->open(ih->getFilename(),
                                entryList[idx]))
                {
                   
                   obj = createStandardProjectionChain(tempIh);
                   if(obj)
                   {
                      result.push_back(obj);
                   }
                   else
                   {
                      delete tempIh;
                      tempIh = NULL;
                   }
                }
             }
          }
       }
          
    }
    
    return result;
}

ossimConnectableObject* DataManager::createStandardProjectionChain(ossimConnectableObject* obj)
{
    ossimKeywordlist kwl;
    ossimImageChain* chainResult = NULL;
    ossimString defaultDescription;
    ossimImageHandler* vecSource = NULL;
    if(!vecSource)
    {
       vecSource = PTR_CAST(ossimVpfTileSource, obj);
       if(vecSource)
       {
          kwl.add("type",
                  "ossimImageChain",
                  true);
          kwl.add("id",
                  0,
                  true);
          kwl.add("object10.type",
                  "ossimCacheTileSource",
                  true);
          kwl.add("object10.id",
                  10,
                  true);
          kwl.add("object10.input_connection1",
                  -1,
                  true);
          defaultDescription = "Vector chain: " + vecSource->getFilename();
       }
       else if(obj->canCastTo("ossimOgrGdalTileSource"))
       {
          vecSource = PTR_CAST(ossimImageHandler, obj);
          kwl.add("type",
                  "ossimImageChain",
                  true);
          kwl.add("id",
                  0,
                  true);
          kwl.add("object10.type",
                  "ossimCacheTileSource",
                  true);
          kwl.add("object10.id",
                  10,
                  true);
          kwl.add("object10.input_connection1",
                  -1,
                  true);
          defaultDescription = "Vector chain: " + vecSource->getFilename();
          
       }
    }
    if(!vecSource)
    {
       bool needsBandSelector = true;
       
       ossimImageHandler* imageHandler = PTR_CAST(ossimImageHandler, obj);
       if(imageHandler)
       {
          if (imageHandler->getNumberOfOutputBands() == 1)
          {
             needsBandSelector = false;
          }
          defaultDescription = "Image chain: " + imageHandler->getFilename();
       }
       else
       {
          defaultDescription = obj->getClassName() + " chain";
       }
	
       kwl.add("type",
               "ossimImageChain",
               true);
	
       kwl.add("id",
               0,
               true);

       ossim_int32 nextConnetionId = 0;
       
       if (needsBandSelector)
       {
          kwl.add("object5.type",
                  "ossimBandSelector",
                  true);
          kwl.add("object5.id",
                  5,
                  true);
          kwl.add("object5.input_connection1",
                  0,
                  true);
          nextConnetionId = 5;
       }

       kwl.add("object6.type",
               "ossimHistogramRemapper",
               true);
       kwl.add("object6.id",
               6,
               true);
       kwl.add("object6.input_connection1",
               nextConnetionId,
               true);

       kwl.add("object10.type",
               "ossimCacheTileSource",
               true);
       kwl.add("object10.id",
               10,
               true);
       kwl.add("object10.input_connection1",
               6,
               true);

       if(obj->getClassName()=="ossimWorldWindHandler")
       {
          kwl.add("object20.max_levels_to_compute",
                  0,
                  true);
       }
       kwl.add("object20.type",
               "ossimImageRenderer",
               true);
       kwl.add("object20.id",
               20,
               true);
       kwl.add("object20.input_connection1",
               10,
               true);
	
       kwl.add("object30.type",
               "ossimCacheTileSource",
               true);
       kwl.add("object30.id",
               30,
               true);
       kwl.add("object30.input_connection1",
               20,
               true);
    }

    ossimObject* objResult = ossimObjectFactoryRegistry::instance()->createObject(kwl);
    chainResult      = PTR_CAST(ossimImageChain, objResult);
    if(chainResult)
    {
	chainResult->makeUniqueIds();
	if(obj)
	{
	    chainResult->addLast(PTR_CAST(ossimConnectableObject, obj));
	}
        chainResult->setDescription(defaultDescription);
        chainResult->initialize();
	add(chainResult);
    }
    else if(objResult)
    {
	delete objResult;
    }
    
    return chainResult;
}

ossimConnectableObject* DataManager::createStandardHistogramMatch(
   std::vector<ossimConnectableObject*>& inputObjects,
   ossimConnectableObject* target,
   const ossimString& mosaicType)
{
   ossimImageChain* result = NULL;
   if(inputObjects.size() < 1)
   {
      return result;
   }
   ossimConnectableObject* mosaic =
      (ossimConnectableObject*)ossimObjectFactoryRegistry::instance()->
      createObject(mosaicType);
   if(!mosaic)
   {
      return result;
   }
   if(target == NULL)
   {
      target = inputObjects[0];
   }
   if(!target)
   {
      return result;
   }
   std::vector<ossimImageChain*> inputEqualizationChains;
   ossimString description;
   ossimImageChain* mosaicChain = new ossimImageChain;
   mosaicChain->addChild(mosaic);
   ossimFilename targetFilename = getHistogramFilename(target);
   const ossim_uint32 SIZE = inputObjects.size();

   for(ossim_uint32 idx = 0; idx < SIZE; ++idx)
   {
      ossimImageChain* equalizationChain = new ossimImageChain;
      ossimHistogramEqualization* equalization =
         new ossimHistogramEqualization;
      ossimHistogramEqualization* target = new ossimHistogramEqualization;
      
      target->setInverseFlag(true);
      target->setHistogram(targetFilename);

      equalization->setInverseFlag(false);
      ossimFilename histogramFile = getHistogramFilename(inputObjects[idx]);
      
      equalization->setHistogram(histogramFile);
      equalizationChain->addChild(equalization);
      equalizationChain->addChild(target);
      equalizationChain->connectMyInputTo(inputObjects[idx]);
      description = inputObjects[idx]->getDescription();
      description = description.replaceAllThatMatch("[a-zA-Z]*:",
       					    "Equalization chain:");
      equalizationChain->setDescription(description);
      add(equalizationChain);
      mosaicChain->connectMyInputTo(equalizationChain);
   }
   
   ossimHistogramRemapper* remapper = new ossimHistogramRemapper;
   remapper->openHistogram(targetFilename);
   description = "Histogram match mosaic";
   mosaicChain->setDescription(description);
   mosaicChain->addChild((ossimConnectableObject*)ossimObjectFactoryRegistry::instance()->createObject(ossimString("ossimCacheTileSource")));
   mosaicChain->addChild(remapper);
   result = mosaicChain;
   add(mosaicChain);
  
   return result;
}


ossimConnectableObject* DataManager::createStandardFusionChain(const ossimString& className,
								    std::vector<ossimConnectableObject*>& objList)
{
  ossimKeywordlist kwl;
  ossimImageChain* result = NULL;
  ossimString defaultDescription = className + " chain";
  ossimConnectableObject* fusionObject = (ossimConnectableObject*)ossimObjectFactoryRegistry::instance()->createObject(className);
  
  if(fusionObject)
    {
	kwl.add("type",
		"ossimImageChain",
		true);
	
	kwl.add("id",
		0,
		true);
	
	
	kwl.add("object10.type",
		"ossimCacheTileSource",
		true);

	kwl.add("object10.id",
		10,
		true);
	kwl.add("object10.input_connection1",
		-1,
		true);

	ossimObject* objResult = ossimObjectFactoryRegistry::instance()->createObject(kwl);
	result      = PTR_CAST(ossimImageChain, objResult);
	if(result)
	  {
	    result->addLast(fusionObject);
	    result->makeUniqueIds();
	    result->setDescription(defaultDescription);
	    
	    result->connectMyInputTo(objList[0]);
	    result->connectMyInputTo(objList[1]);
	    result->initialize();
	    add(result);
	  }
	else if(objResult)
	  {
	    delete objResult;
	    delete fusionObject;
	  }
    }
  return result;
}


ossimConnectableObject* DataManager::createRawChain(const ossimFilename& file, const ossimString& description)
{
   ossimImageHandler* ih = ossimImageHandlerRegistry::instance()->open(file);
   ossimConnectableObject* result = NULL;
   ossimString defaultDescription = description;
   if(ih)
   {
      ossimImageChain* chain = new ossimImageChain;
      chain->add(ih);
      if(defaultDescription == "")
      {
         defaultDescription = file;
      }
      chain->setDescription(defaultDescription);
      add(chain);
      result = chain;
   }

   return result;
}


ossimConnectableObject* DataManager::createStandardNormalsChain(const ossimId& id)
{
   ossimConnectableObject* result = NULL;
   ossimConnectableObject* input = getObject(id);

   if(input)
   {
      ossimImageChain* chain = new ossimImageChain;
      ossimConnectableObject* normals = (ossimConnectableObject*)ossimObjectFactoryRegistry::instance()->createObject(ossimString("ossimImageToPlaneNormalFilter"));
      if(normals)
      {
         chain->add(normals);
         chain->setDescription(ossimString("Normals chain"));
         chain->connectMyInputTo(input);
         add(chain);
      }
      else
      {
         delete chain;
         chain = NULL;
      }
      
      result = chain;
   }
   
   return result;
}


ossimConnectableObject* DataManager::createStandardElevMosaic(const std::vector<ossimFilename>& inputs)
{
   std::vector<ossimConnectableObject*> inputObjects;
   for(int idx = 0; idx < (int)inputs.size();++idx)
   {
      ossimConnectableObject* obj = createRawChain(inputs[idx]);
      if(obj)
      {
         inputObjects.push_back(obj);
      }
   }
   return createStandardElevMosaic(inputObjects);
}

ossimConnectableObject* DataManager::createStandardElevMosaic(const std::vector<ossimConnectableObject*>& inputs)
{
   if(!inputs.size())
   {
      return NULL;
   }
   ossimImageChain* chainResult = NULL;
   ossimString defaultDescription="ossimOrthoImageMosaic";;
   ossimKeywordlist kwl;
   
   kwl.add("type",
           "ossimImageChain",
           true);
   
   kwl.add("id",
           0,
           true);
   
   kwl.add("object1.type",
           "ossimOrthoImageMosaic",
           true);
   kwl.add("object1.id",
           1,
           true);
   
   kwl.add("object10.type",
           "ossimCacheTileSource",
           true);
   kwl.add("object10.id",
           10,
           true);
   kwl.add("object10.input_connection1",
           1,
           true);
   
   kwl.add("object20.type",
           "ossimImageRenderer",
           true);
   kwl.add("object20.id",
           20,
           true);
   kwl.add("object20.input_connection1",
           10,
           true);
   
   kwl.add("object30.type",
           "ossimCacheTileSource",
           true);
   kwl.add("object30.id",
           30,
           true);
   kwl.add("object30.input_connection1",
           20,
           true);
   
    ossimObject* objResult = ossimObjectFactoryRegistry::instance()->createObject(kwl);
    chainResult      = PTR_CAST(ossimImageChain, objResult);
    if(chainResult)
    {
	chainResult->makeUniqueIds();
        chainResult->setDescription(defaultDescription);
        for(int idx = 0; idx < (int)inputs.size();++idx)
        {
           chainResult->connectMyInputTo(inputs[idx]);
        }
	add(chainResult);
    }
    else if(objResult)
    {
	delete objResult;
    }
    
    return chainResult;
}


ossimConnectableObject* DataManager::createStandardOrthoMosaic(const std::vector<ossimFilename>& inputs)
{
   std::vector<ossimConnectableObject*> inputObjects;
   for(int idx = 0; idx < (int)inputs.size();++idx)
   {
      ossimConnectableObject* obj = createRawChain(inputs[idx]);
      if(obj)
      {
         inputObjects.push_back(obj);
      }
   }
   return createStandardOrthoMosaic(inputObjects);
}

ossimConnectableObject* DataManager::createStandardOrthoMosaic(const std::vector<ossimConnectableObject*>& inputs)
{
   if(!inputs.size())
   {
      return NULL;
   }
   std::vector<ossimConnectableObject*> inputList;
   int idx = 0;
   for(idx = 0; idx < (int)inputs.size(); ++idx)
   {
     ossimImageChain* chain = PTR_CAST(ossimImageChain, inputs[idx]);
     if(chain)
       {
	 ossimConnectableObject* renderer = chain->findFirstObjectOfType("ossimImageRenderer",
									 false);
	 if(renderer)
	   {
	     
	     ossimConnectableObject* image = chain->findFirstObjectOfType("ossimImageHandler",
									  false);
	     if(image)
	       {
		 ossimImageChain* newChain = new ossimImageChain;
		 ossimConnectableObject* dupIh = (ossimConnectableObject*)image->dup();
		 newChain->addChild(dupIh);
		 ossimString description = chain->getDescription();
		 description = description.replaceAllThatMatch("[a-zA-Z]*:",
							       "Ortho chain:");
		 newChain->setDescription(description);
		 add(newChain);
		 inputList.push_back(newChain);
	       }
	   }
	 else
	   {
	     inputList.push_back(chain);
	   }
       }
   }
   if(!inputList.size())
     {
       return (ossimConnectableObject*)NULL;
     }

   ossimImageChain* chainResult = NULL;
   ossimString defaultDescription="ossimOrthoImageMosaic";;
   ossimKeywordlist kwl;
   
   kwl.add("type",
           "ossimImageChain",
           true);
   
   kwl.add("id",
           0,
           true);
   
   kwl.add("object1.type",
           "ossimOrthoImageMosaic",
           true);
   kwl.add("object1.id",
           1,
           true);
   
   kwl.add("object5.type",
           "ossimBandSelector",
           true);
   kwl.add("object5.id",
           5,
           true);
   kwl.add("object5.input_connection1",
           1,
           true);
   
   kwl.add("object10.type",
           "ossimCacheTileSource",
           true);
   kwl.add("object10.id",
           10,
           true);
   kwl.add("object10.input_connection1",
           5,
           true);
   
   kwl.add("object20.type",
           "ossimImageRenderer",
           true);
   kwl.add("object20.id",
           20,
           true);
   kwl.add("object20.input_connection1",
           10,
           true);
   
   kwl.add("object30.type",
           "ossimCacheTileSource",
           true);
   kwl.add("object30.id",
           30,
           true);
   kwl.add("object30.input_connection1",
           20,
           true);
   
    ossimObject* objResult = ossimObjectFactoryRegistry::instance()->createObject(kwl);
    chainResult      = PTR_CAST(ossimImageChain, objResult);
    if(chainResult)
    {
	chainResult->makeUniqueIds();
        chainResult->setDescription(defaultDescription);
        for(idx = 0; idx < (int)inputList.size();++idx)
        {
           chainResult->connectMyInputTo(inputList[idx]);
        }
	add(chainResult);
    }
    else if(objResult)
    {
	delete objResult;
    }
    
    return chainResult;
}

ossimConnectableObject* DataManager::createStandardMosaic(const std::vector<ossimConnectableObject*>& inputs,
							       const ossimString& mosaicClassName)
{
  ossimConnectableObject* connectable = (ossimConnectableObject*)(ossimObjectFactoryRegistry::instance()->createObject(mosaicClassName));
  if(connectable)
    {
      return createStandardMosaic(inputs, connectable);
    }
  return NULL;
}

ossimConnectableObject* DataManager::createStandardMosaic(
   const std::vector<ossimConnectableObject*>& inputs,
   ossimConnectableObject* obj)
{
   ossimKeywordlist kwl;
   ossimImageChain* chainResult = 0;
   ossimString defaultDescription=obj->getDescription();
   kwl.add("type",
           "ossimImageChain",
           true);
   
   kwl.add("id",
           0,
           true);
   
//     kwl.add("object5.type",
// 	    "ossimBandSelector",
// 	    true);
//     kwl.add("object5.id",
// 	    5,
// 	    true);
//     kwl.add("object5.input_connection1",
// 	    -1,
// 	    true);

   kwl.add("object10.type",
           "ossimCacheTileSource",
           true);
   kwl.add("object10.id",
           10,
           true);
   kwl.add("object10.input_connection1",
           -1,
           true);
   
   if(defaultDescription == "")
   {
      defaultDescription = obj->getClassName();
   }
   
   ossimObject* objResult = ossimObjectFactoryRegistry::instance()->createObject(kwl);
   chainResult            = PTR_CAST(ossimImageChain, objResult);
   if(chainResult)
   {
      chainResult->makeUniqueIds();
      if(obj)
      {
         chainResult->addLast(PTR_CAST(ossimConnectableObject, obj));
      }
      chainResult->setDescription(defaultDescription);
      
      bool shpTopFlag = shapeFilesOnTop();
      
      for(int idx = 0; idx < (int) inputs.size(); ++idx)
      {
         chainResult->connectMyInputTo(inputs[idx]);
         if (shpTopFlag)
         {
            if (isShapeFileConnection(inputs[idx]))
            {
               chainResult->moveInputToTop(inputs[idx]->getId());
            }
         }
         
      }
      add(chainResult);
   }
   else if(objResult)
   {
      delete objResult;
   }
   
   return chainResult;
}


void DataManager::add(ossimConnectableObject* obj)
{
   if(obj&&!getObject(obj))
   {
      theDataTable.insert(make_pair(obj->getId(), obj));
      ossimString tempDesc = obj->getDescription();
      if(tempDesc == "")
      {
         tempDesc = obj->getClassName();
      }
      obj->addListener((ossimConnectableObjectListener*)thePrivateListener);
      ossimConnectableObject::ConnectableObjectList result;
      
      obj->findAllInputsOfType(result,
                               "ossimImageRenderer",
                               false,
                               true);
      if(result.size())
      {
         ossim_uint32 idx = 0;
         for(idx = 0; idx < result.size(); ++idx)
         {
            if(result[idx].valid())
            {
               result[idx]->addListener((ossimProcessListener*)thePrivateListener);
            }
         }
      }
   }
}

ossimConnectableObject* DataManager::getObject(const ossimId& id)
{
   std::map<ossimId, ossimConnectableObject*>::iterator current = theDataTable.find(id);

   if(current != theDataTable.end())
   {
      return (*current).second;
   }
   
   return (ossimConnectableObject*)NULL;
}

ossimConnectableObject* DataManager::getObject(const ossimConnectableObject* obj)
{
   return getObject(obj->getId());
}

void DataManager::getAllObjects(std::vector<ossimConnectableObject*>& result,
                                            const RTTItypeid& typeId)
{
   std::map<ossimId, ossimConnectableObject*>::iterator current = theDataTable.begin();

   while(current != theDataTable.end())
   {
      if(typeId.can_cast(TYPE_INFO((*current).second)))
      {
         result.push_back((*current).second);
      }
      
      ++current;
   }
}

void DataManager::getAllObjects(std::vector<ossimConnectableObject*>& result)
{
   std::map<ossimId, ossimConnectableObject*>::iterator current = theDataTable.begin();

   while(current != theDataTable.end())
   {
      if((*current).second)
      {
         result.push_back((*current).second);
      }
      ++current;
   }
}

void DataManager::getAllElevationOrthos(std::vector<ossimConnectableObject*>& result)
{
   ossimConnectableObject* current = getFirstObject();

   while(current)
   {
      ossimImageChain* chain = PTR_CAST(ossimImageChain, current);
      if(chain)
      {
         ossimOrthoImageMosaic* mosaic = PTR_CAST(ossimOrthoImageMosaic, chain->getLastObject());
         if(mosaic)
         {
            ossimImageChain* orthoChain = PTR_CAST(ossimImageChain, mosaic->getInput(0));
            if(orthoChain)
            {
               if(orthoChain->getNumberOfOutputBands() == 1)
               {
                  result.push_back(current);
               }
            }
         }
      }
      current = getNextObject();
   }
}

ossimConnectableObject* DataManager::getFirstObject()
{
   theCurrentIter = theDataTable.begin();

   if(theCurrentIter != theDataTable.end())
   {
      return (*theCurrentIter).second;
   }

   return (ossimConnectableObject*)NULL;
}

ossimConnectableObject* DataManager::getFirstObject(const RTTItypeid& typeId)
{
   theCurrentIter = theDataTable.begin();

   while(theCurrentIter != theDataTable.end())
   {
      if(typeId.can_cast(TYPE_INFO((*theCurrentIter).second)))
      {
         return (*theCurrentIter).second;
      }
      ++theCurrentIter;
   }

   return (ossimConnectableObject*)NULL;
}

ossimConnectableObject* DataManager::getNextObject()
{
   if(theCurrentIter != theDataTable.end())
   {
      ++theCurrentIter;
   }
   if(theCurrentIter != theDataTable.end())
   {
      return (*theCurrentIter).second;
   }
   return (ossimConnectableObject*)NULL;   
}

ossimConnectableObject* DataManager::getNextObject(const RTTItypeid& typeId)
{
   // advance pointer
   if(theCurrentIter!=theDataTable.end())
   {
      ++theCurrentIter;
   }

   // find the next object of the passed in type.
   while(theCurrentIter != theDataTable.end())
   {
      if(typeId.can_cast(TYPE_INFO((*theCurrentIter).second)))
      {
         return (*theCurrentIter).second;
      }
      ++theCurrentIter;
   }   
   
   return (ossimConnectableObject*)NULL;   
}

ossimConnectableObject* DataManager::removeObject(const ossimId& id)
{
   ossimConnectableObject* result = NULL;
   std::map<ossimId, ossimConnectableObject*>::iterator current = theDataTable.find(id);

   if(current != theDataTable.end())
   {
      result = (*current).second;
      theDataTable.erase(current);
      if(result)
      {
         result->removeListener((ossimConnectableObjectListener*)thePrivateListener);
         ossimConnectableObject::ConnectableObjectList  resultVector;
         
         result->findAllInputsOfType(resultVector,
                                     "ossimImageRenderer",
                                     false,
                                     true);
         if(resultVector.size())
         {
            ossim_uint32 idx = 0;
            for(idx = 0; idx < resultVector.size(); ++idx)
            {
               resultVector[idx]->removeListener((ossimProcessListener*)thePrivateListener);
            }
         }
      }
   }
   
   return result;
}

ossimConnectableObject* DataManager::removeObject(const ossimConnectableObject* obj)
{
   return removeObject(obj->getId());
}

void DataManager::removeAll(std::vector<ossimConnectableObject*>& result)
{
   int i = 0;
   std::map<ossimId, ossimConnectableObject*>::iterator current = theDataTable.begin();
   std::vector<ossimConnectableObject*> tempResult;
   
   while(current != theDataTable.end())
   {
      tempResult.push_back((*current).second);
      ++current;
   }
   for(i = 0; i < (int)tempResult.size(); ++i)
   {
      removeObject(tempResult[i]);
      result.push_back(tempResult[i]);
   }
   
}

void DataManager::deleteObject(ossimConnectableObject* obj)
{
   if(obj)
   {
      if(removeObject(obj))
      {
         delete obj;
      }
   }
}

void DataManager::deleteObject(const ossimId& id)
{
   ossimConnectableObject* obj = removeObject(id);
   
   if(obj)
   {
      delete obj;
   }
}

void DataManager::deleteObjects(std::vector<ossimConnectableObject*>& objList)
{
   int idx = 0;
   
   for(idx = 0; idx < (int)objList.size(); ++idx)
   {
      removeObject(objList[idx]);
      if(objList[idx])
      {
         delete objList[idx];
         objList[idx] = NULL;
      }
   }
}

bool DataManager::hasObjectsOfType(const std::vector<ossimString>& classNameList)const
{
   std::map<ossimId, ossimConnectableObject*>::const_iterator iter = theDataTable.begin();
   ossim_uint32 idx = 0;
   while(iter!=theDataTable.end())
   {
      for(idx = 0; idx < classNameList.size(); ++idx)
      {
         if(((*iter).second)->findObjectOfType(classNameList[idx], ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT))
         {
            return true;
         }
      }
      
      ++iter;
   }

   return false;
}


ossimId DataManager::duplicate(const ossimId& id, bool recurseInputs)
{
   ossimConnectableObject* obj = getObject(id);
   
   if(!obj)
   {
      return ossimId(ossimId::INVALID_ID);
   }

   ossimKeywordlist kwl;
   int i;
   obj->saveState(kwl);
   ossimObject* tempObj = ossimObjectFactoryRegistry::instance()->createObject(kwl);
   
   ossimConnectableObject* connectable = PTR_CAST(ossimConnectableObject,
                                                  tempObj);

   if(connectable)
   {
      ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                           connectable);
      if(inter)
      {
         inter->makeUniqueIds();
      }
      else
      {
         connectable->setId(ossimIdManager::instance()->generateId());
      }

      add(connectable);

      if(recurseInputs)
      {
         for(i = 0; i < (int)obj->getNumberOfInputs(); ++i)
         {
            if(obj->getInput(i))
            {
               ossimId dupId = duplicate(obj->getInput(i)->getId(),
                                         recurseInputs);
               if(dupId != ossimId(ossimId::INVALID_ID))
               {
                  ossimConnectableObject* tempObj2 = getObject(dupId);
                  connectable->connectMyInputTo(tempObj2);
               }
            }
         }
      }
      else
      {
         for(i = 0; i < (int)obj->getNumberOfInputs(); ++i)
         {
            connectable->connectMyInputTo(obj->getInput(i));
         }
      }
   }
   else
   {
      if(tempObj)
      {
         delete tempObj;
         
         return ossimId(ossimId::INVALID_ID);
      }
   }
   
   return connectable->getId();
}

ossimId DataManager::duplicate(const ossimConnectableObject* obj, bool recurseInputs)
{
   if(obj)
   {
      return duplicate(obj->getId(), recurseInputs);
   }
   return ossimId(ossimId::INVALID_ID);
}

void DataManager::duplicate(std::vector<ossimId>& result,
                                        const std::vector<ossimId>& objList,
                                        bool recurseInputs)
{
   for(unsigned int i = 0; i < objList.size(); ++i)
   {
      result.push_back(duplicate(objList[i], recurseInputs));
   }
}

ossimString DataManager::getDescription(const ossimId& id)const
{
   std::map<ossimId, ossimConnectableObject*>::const_iterator desc = theDataTable.find(id);

   if(desc != theDataTable.end())
   {
      return (*desc).second->getDescription();
   }

   return ossimString("");
}

ossimString DataManager::getDescription(const ossimConnectableObject* obj)const
{
   if(obj)
   {
      return getDescription(obj->getId());
   }

   return "";
}

void DataManager::setDescription(const ossimId& id,
                                      const ossimString& description)
{
   std::map<ossimId, ossimConnectableObject*>::iterator desc = theDataTable.find(id);
   if(desc != theDataTable.end())
   {
      ossimString oldDescription = (*desc).second->getDescription();
      
      (*desc).second->setDescription(description);
   }
}

void DataManager::setDescription(const ossimConnectableObject* obj,
                                      const ossimString& description)
{
   if(obj)
   {
      setDescription(obj->getId(), description);
   }
}

void DataManager::deleteAll()
{
   std::vector<ossimConnectableObject*> result;
   removeAll(result);

   for(int idx = 0; idx < (int)result.size(); ++idx)
   {
      if(result[idx])
      {
         delete result[idx];
         result[idx] = NULL;
      }
   }
   result.clear();
}


void DataManager::initializeAll()
{
   ossimConnectableObject* current = getFirstObject();

   while(current)
   {
      ossimSource* src = PTR_CAST(ossimSource, current);
      if(src)
      {
         src->initialize();
      }
      current = getNextObject();
   }
}

bool DataManager::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   deleteAll();
   
   ossimString copyPrefix = prefix;
   
   std::map<ossimId, std::vector<ossimId> > idMapping;
   bool result = addAllObjects(idMapping, kwl, prefix);
   if(!result)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING DataManager::loadState: "
         << "problems adding sources" << std::endl;
   }
   
   result = connectAllObjects(idMapping);
   initializeAll();
   if(!result)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING DataManager::loadState: "
         << "problems connecting sources" << std::endl;
   }

   return result;
}
   
bool DataManager::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   
   bool result = true;
   std::map<ossimId, ossimConnectableObject*>::const_iterator current = theDataTable.begin();
   
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "DataManager",
           true);
   ossim_int32 childIndex = 1;
   while(current != theDataTable.end())
   {
      ossimString newPrefix = ( (ossimString(prefix) + 
                                 ossimString("object") + 
                                 ossimString::toString(childIndex) + "."));
      if((*current).second)
      {
	 bool test = ((*current).second)->saveState(kwl, newPrefix);
	 if(!test)
         {
            result = false;
         }

      }
      ++childIndex;
      ++current;
   }

   return result;
}

bool DataManager::addAllObjects(std::map<ossimId, std::vector<ossimId> >& idMapping,
                                    const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   ossimString copyPrefix = prefix;
   ossim_int32 index = 0;

   std::vector<ossimId> inputConnectionIds;

   ossimString regExpression =  ossimString("^(") + copyPrefix + "object[0-9]+.)";
   std::vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   ossim_int32 numberOfSources = keys.size();//kwl.getNumberOfSubstringKeys(regExpression);

   int offset = (copyPrefix+"object").size();
   int idx = 0;
   std::vector<int> theNumberList(numberOfSources);
   for(idx = 0; idx < (int)theNumberList.size();++idx)
     {
       ossimString numberStr(keys[idx].begin() + offset,
			     keys[idx].end());
       theNumberList[idx] = numberStr.toInt();
     }
   std::sort(theNumberList.begin(), theNumberList.end());
   for(idx=0;idx < (int)theNumberList.size();++idx)
   {
      ossimString newPrefix = copyPrefix;
      newPrefix += ossimString("object");
      newPrefix += ossimString::toString(theNumberList[idx]);
      newPrefix += ossimString(".");
      ossimObject* object = ossimObjectFactoryRegistry::instance()->createObject(kwl,
                                                                                 newPrefix.c_str());
      if(object)
      {
         ossimConnectableObject* connectable = PTR_CAST(ossimConnectableObject, object);
         if(connectable)
         {
            // we did find a source so include it in the count
            ossimId id = connectable->getId();
            inputConnectionIds.clear();

            findInputConnectionIds(inputConnectionIds,
                                   kwl,
                                   newPrefix);
            if(inputConnectionIds.size() != 0)
            {
               idMapping.insert(std::make_pair(id, inputConnectionIds));
            }
            add(connectable);
         }
         else
         {
            delete object;
            object = NULL;
         }
      }
      else
      {
      }      
      ++index;
   }
   return true;
}

void DataManager::findInputConnectionIds(std::vector<ossimId>& result,
                                              const ossimKeywordlist& kwl,
                                              const char* prefix)
{
   ossimString copyPrefix = prefix;
   ossim_uint32 idx = 0;
   
   ossimString regExpression =  ossimString("^") + ossimString(prefix) + "input_connection[0-9]+";
   std::vector<ossimString> keys =
      kwl.getSubstringKeyList( regExpression );
   
   ossim_int32 offset = (copyPrefix+"input_connection").size();
   ossim_uint32 numberOfKeys = keys.size();
   std::vector<int> theNumberList(numberOfKeys);
   for(idx = 0; idx < theNumberList.size();++idx)
   {
      ossimString numberStr(keys[idx].begin() + offset,
                            keys[idx].end());
      theNumberList[idx] = numberStr.toInt();
   }
   std::sort(theNumberList.begin(), theNumberList.end());
   copyPrefix += ossimString("input_connection");
   for(idx=0;idx < theNumberList.size();++idx)
   {
      const char* lookup = kwl.find(copyPrefix,ossimString::toString(theNumberList[idx]));
      if(lookup)
      {
         ossim_int32 id = ossimString(lookup).toInt32();
         result.push_back(ossimId(id));
      }
   }
}

bool DataManager::connectAllObjects(const std::map<ossimId, std::vector<ossimId> >& idMapping)
{
   if(idMapping.size())
   {
      std::map<ossimId, std::vector<ossimId> >::const_iterator iter = idMapping.begin();

      while(iter != idMapping.end())
      {
         ossimConnectableObject* currentObject = getObject((*iter).first);

         if(currentObject)
         {
            ossim_int32 upperBound = (*iter).second.size();
            for(ossim_int32 index = 0; index < upperBound; ++index)
            {
               ossimConnectableObject* inputObject = getObject((*iter).second[index]);
						   
               currentObject->connectMyInputTo(index, inputObject);
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL DataManager::connectAllObjects: Could not find "
               << (*iter).first << std::endl;
            return false;
         }
         ++iter;
      }
   }
   return true;
}


ossimFilename DataManager::getHistogramFilename(
   ossimConnectableObject* connectable)const
{
  ossimFilename result = "";
  if(connectable)
  {
     ossimImageHandler* handler = (ossimImageHandler*)(connectable->findObjectOfType("ossimImageHandler",
                                                                                     ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT,
                                                                                     true));
     if(handler)
     {
        result = handler->createDefaultHistogramFilename();
     }
  }
  
  return result;
}

bool DataManager::isShapeFileConnection(
   ossimConnectableObject* obj) const
{
   bool result = false;
   
   ossimImageHandler* handler = (ossimImageHandler*)
      (obj->findObjectOfType(
          "ossimImageHandler",
          ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT,
          true));

   if (handler)
   {
      if (handler->getClassName() == "ossimOgrGdalTileSource")
      {
         result = true;
      }
   }
   return result;
}

bool DataManager::shapeFilesOnTop() const
{
   bool result = false;
   const char* lookup =
      ossimPreferences::instance()->findPreference("shape_files_on_top");
   if (lookup)
   {
      ossimString os = lookup;
      result = os.toBool();
   }
   return result;
}

