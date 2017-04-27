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
// $Id: ossimDataManager.h 15647 2009-10-10 03:07:20Z gpotts $
#ifndef ossimDataManager_HEADER
#define ossimDataManager_HEADER
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
class ossimDataManagerPrivateListener;

class ossimDataManager : public ossimObject,
                         public ossimListenerManager
{
public:
   typedef std::map<ossimId, ossimRefPtr<ossimConnectableObject> > DataTableType;
   friend class ossimDataManagerPrivateListener;
   virtual ~ossimDataManager();
   ossimDataManager();

   /*!
    * The initial call should be the first id.
    * \param The initial value of the unused id.
    *        after call it will hold the next unused value.
    */
   void  makeUniqueIds();
  
   ossimConnectableObject::ConnectableObjectList createStandardProjectionChain(const char* filename);

  ossimConnectableObject* insertObject(const ossimString& classNameToInsert,
                                       const ossimId& objId);

   ossimConnectableObject* createRawChain(const ossimFilename& file, const ossimString& description=ossimString(""));
   ossimConnectableObject* createStandardNormalsChain(const ossimId& id);
   
   ossimConnectableObject* createStandardProjectionChain(ossimConnectableObject* obj);
   ossimConnectableObject* createStandardHistogramMatch(ossimConnectableObject::ConnectableObjectList& inputObjects,
						       ossimConnectableObject* target,
						       const ossimString& mosaicType);
   ossimConnectableObject* createStandardFusionChain(const ossimString& className,
						     ossimConnectableObject::ConnectableObjectList& objList);
   ossimConnectableObject* createStandardElevMosaic(const std::vector<ossimFilename>& inputs);
   ossimConnectableObject* createStandardElevMosaic(ossimConnectableObject::ConnectableObjectList& inputs);

   ossimConnectableObject* createStandardOrthoMosaic(const std::vector<ossimFilename>& inputs);
   ossimConnectableObject* createStandardOrthoMosaic(ossimConnectableObject::ConnectableObjectList& inputs);
   
   ossimConnectableObject* createStandardMosaic(ossimConnectableObject::ConnectableObjectList& inputs,
                                                const ossimString& mosaicClassName);
   ossimConnectableObject* createStandardMosaic(ossimConnectableObject::ConnectableObjectList& inputs,
                                                ossimConnectableObject* mosaicObj);
   
   void getAllObjects(ossimConnectableObject::ConnectableObjectList& result,
                      const RTTItypeid& typeId);
   void getAllObjects(ossimConnectableObject::ConnectableObjectList& result);
   void getAllElevationOrthos(ossimConnectableObject::ConnectableObjectList& result);
   
   ossimConnectableObject* getFirstObject();
   ossimConnectableObject* getFirstObject(const RTTItypeid& typeId);
   ossimConnectableObject* getNextObject();
   ossimConnectableObject* getNextObject(const RTTItypeid& typeId);

   
   ossimConnectableObject* getObject(const ossimId& id);
   ossimConnectableObject* getObject(const ossimConnectableObject* obj);
   ossimRefPtr<ossimConnectableObject> removeObject(const ossimId& id);
   ossimRefPtr<ossimConnectableObject> removeObject(const ossimConnectableObject* obj);
   void removeObjects(ossimConnectableObject::ConnectableObjectList& remove_these);
   void removeAll(ossimConnectableObject::ConnectableObjectList& result);
   void removeAll()
   {
      ossimConnectableObject::ConnectableObjectList result;
      removeAll(result);
   }
   void initializeAll();

   void deleteObject(ossimConnectableObject* obj);
   void deleteObject(const ossimId& id);
   void deleteObjects(ossimConnectableObject::ConnectableObjectList& objList);
   
   bool hasObjectsOfType(const std::vector<ossimString>& classNameList);

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
   
   ossimDataManager(const ossimDataManager& data)
      :ossimObject(data),
       ossimListenerManager()
      {}
   const ossimDataManager& operator = (const ossimDataManager& )
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

   DataTableType theDataTable;
   DataTableType::iterator theCurrentIter;
   ossimDataManagerPrivateListener* thePrivateListener;
TYPE_DATA  
};

#endif

