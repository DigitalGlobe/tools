//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: DataManager.h 13097 2008-06-27 20:00:22Z dburken $
#ifndef DataManager_HEADER
#define DataManager_HEADER
#include <ossim/base/ossimId.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimObjectDestructingEvent.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimListenerManager.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimConnectableContainer.h>
#include <vector>
#include <map>

class ossimConnectableObject;
class ossimImageChain;
class DataManagerPrivateListener;

class DataManager : public ossimObject,
                         public ossimListenerManager
{
public:
   friend class DataManagerPrivateListener;
   virtual ~DataManager();
   DataManager();

   /*!
    * The initial call should be the first id.
    * \param The initial value of the unused id.
    *        after call it will hold the next unused value.
    */
   void  makeUniqueIds();
  
   std::vector<ossimConnectableObject*> createStandardProjectionChain(const char* filename);

  ossimConnectableObject* insertObject(const ossimString& classNameToInsert,
				       const ossimId& objId);

   ossimConnectableObject* createRawChain(const ossimFilename& file, const ossimString& description=ossimString(""));
   ossimConnectableObject* createStandardNormalsChain(const ossimId& id);
   
   ossimConnectableObject* createStandardProjectionChain(ossimConnectableObject* obj);
  ossimConnectableObject* createStandardHistogramMatch(std::vector<ossimConnectableObject*>& inputObjects,
						       ossimConnectableObject* target,
						       const ossimString& mosaicType);
   ossimConnectableObject* createStandardFusionChain(const ossimString& className,
						     std::vector<ossimConnectableObject*>& objList);
   ossimConnectableObject* createStandardElevMosaic(const std::vector<ossimFilename>& inputs);
   ossimConnectableObject* createStandardElevMosaic(const std::vector<ossimConnectableObject*>& inputs);

   ossimConnectableObject* createStandardOrthoMosaic(const std::vector<ossimFilename>& inputs);
   ossimConnectableObject* createStandardOrthoMosaic(const std::vector<ossimConnectableObject*>& inputs);
   
   ossimConnectableObject* createStandardMosaic(const std::vector<ossimConnectableObject*>& inputs,
                                                const ossimString& mosaicClassName);
   ossimConnectableObject* createStandardMosaic(const std::vector<ossimConnectableObject*>& inputs,
                                                ossimConnectableObject* mosaicObj);
   
   void getAllObjects(std::vector<ossimConnectableObject*>& result,
                      const RTTItypeid& typeId);
   void getAllObjects(std::vector<ossimConnectableObject*>& result);
   void getAllElevationOrthos(std::vector<ossimConnectableObject*>& result);
   
   ossimConnectableObject* getFirstObject();
   ossimConnectableObject* getFirstObject(const RTTItypeid& typeId);
   ossimConnectableObject* getNextObject();
   ossimConnectableObject* getNextObject(const RTTItypeid& typeId);

   
   ossimConnectableObject* getObject(const ossimId& id);
   ossimConnectableObject* getObject(const ossimConnectableObject* obj);
   ossimConnectableObject* removeObject(const ossimId& id);
   ossimConnectableObject* removeObject(const ossimConnectableObject* obj);
   void removeAll(std::vector<ossimConnectableObject*>& result);
   void initializeAll();

   void deleteObject(ossimConnectableObject* obj);
   void deleteObject(const ossimId& id);
   void deleteObjects(std::vector<ossimConnectableObject*>& objList);
   
   bool hasObjectsOfType(const std::vector<ossimString>& classNameList)const;

   /*!
    * Duplicates the data object identified by id and adds it to the
    * manager
    */
   ossimId duplicate(const ossimId& id,
                     bool recurseInputs=false);

   /*!
    * Duplicates the data object identified by id and adds it to the
    * manager
    */
   ossimId duplicate(const ossimConnectableObject* obj,
                     bool recurseInputs=false);

   /*!
    * Duplicates the data object identified by id and adds it to the
    * manager
    */
   void duplicate(std::vector<ossimId>& result,
                  const std::vector<ossimId>& objList,
                  bool recurseInputs=false);
   
   ossimString getDescription(const ossimId& id)const;
   ossimString getDescription(const ossimConnectableObject* obj)const;
   
   void setDescription(const ossimId& id,
                       const ossimString& description);
   void setDescription(const ossimConnectableObject* obj,
                       const ossimString& description);
   
   void add(ossimConnectableObject* obj);
   void deleteAll();
   
   bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

  
protected:
   
   DataManager(const DataManager& data)
      :ossimObject(data),
       ossimListenerManager()
      {}
   const DataManager& operator = (const DataManager& )
      {
         return *this;
      }

   ossimFilename getHistogramFilename(ossimConnectableObject*)const;
   bool addAllObjects(std::map<ossimId, std::vector<ossimId> >& idMapping,
                      const ossimKeywordlist& kwl,
                      const char* prefix);
      
   bool connectAllObjects(const std::map<ossimId,
                          std::vector<ossimId> >& idMapping);
   void findInputConnectionIds(std::vector<ossimId>& result,
                               const ossimKeywordlist& kwl,
                               const char* prefix);

   /** @return true if connection is a shapefile reader, false if not. */ 
   bool isShapeFileConnection(ossimConnectableObject* obj) const;

   /**
    * @return true if keyword "shape_files_on_top" is found in preferences
    * and set to true, false if not.
    */
   bool shapeFilesOnTop()const;

   std::map<ossimId, ossimConnectableObject*> theDataTable;

   mutable std::map<ossimId, ossimConnectableObject*>::iterator theCurrentIter;
   DataManagerPrivateListener* thePrivateListener;
TYPE_DATA  
};

#endif

