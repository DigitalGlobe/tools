#ifndef ossimPlanetQtLegendTextureItem_HEADER
#define ossimPlanetQtLegendTextureItem_HEADER
#include <ossimPlanetQt/ossimPlanetQtLegendItem.h>
#include <ossimPlanetQt/ossimPlanetQtEvent.h>
#include <QtGui/QApplication>
#include <ossimPlanet/ossimPlanetTextureLayer.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <ossimPlanet/ossimPlanetOperation.h>

class OSSIMPLANETQT_DLL ossimPlanetQtLegendTextureItem : public ossimPlanetQtLegendItem
{
public:

   class Callback : public ossimPlanetTextureLayerCallback
   {
   public:
      Callback(ossimPlanetQtLegendTextureItem* item);
      virtual void propertyChanged(const ossimString& name, 
                                   const ossimPlanetTextureLayer* layer);
		virtual void layersAdded(std::vector<osg::ref_ptr<ossimPlanetTextureLayer> >& layers);
		virtual void layersRemoved(std::vector<osg::ref_ptr<ossimPlanetTextureLayer> >& layers);
		void setItem(ossimPlanetQtLegendTextureItem* item)
      {
         theItem = item;
      }
   protected:
      ossimPlanetQtLegendTextureItem* theItem;
   };
   
   class StagerCallback : public ossimPlanetOperationCallback
   {
   public:
      StagerCallback(ossimPlanetQtLegendTextureItem* item)
      :theItem(item)
      {
      }
      virtual void finished(ossimPlanetOperation* operation);
      virtual void canceled(ossimPlanetOperation* operation);
      
   protected:
      ossimPlanetQtLegendTextureItem* theItem;
   };
   friend class StagerCallback;
      
   ossimPlanetQtLegendTextureItem(QTreeWidgetItem* item,
                                  const QString& name = QString(""));
   ossimPlanetQtLegendTextureItem(QTreeWidget* treeWidget,
                                  const QString& name = QString(""));
   ossimPlanetQtLegendTextureItem();
   virtual ~ossimPlanetQtLegendTextureItem();
   void setLayer(osg::ref_ptr<ossimPlanetTextureLayer> layer);
   osg::ref_ptr<ossimPlanetTextureLayer> layer();
   const osg::ref_ptr<ossimPlanetTextureLayer> layer()const;

   void populateItem();
   virtual void changed();
   void deleteLayer(bool deleteThis=false);

   void updateLayerInfo();
   virtual ossimRefPtr<ossimXmlNode> saveXml()const;
   virtual void loadXml(ossimRefPtr<ossimXmlNode> node,
                        std::vector<ossimPlanetOperation*>& activityList);
   virtual void addXml(ossimRefPtr<ossimXmlNode> node,
                       std::vector<ossimPlanetOperation*>& activityList,
                       bool startAtTopFlag=false);

   virtual const osg::ref_ptr<ossimPlanetLookAt> getLookAt()const;
   virtual void setLookAt(osg::ref_ptr<ossimPlanetLookAt> lookAt);

   virtual const osg::ref_ptr<ossimPlanetExtents> getExtents()const;
   virtual void propertyChanged(const ossimString& name);

protected:
   void clearItem();
   osg::ref_ptr<ossimPlanetTextureLayer> theLayer;
   osg::ref_ptr<ossimPlanetQtLegendTextureItem::Callback> theCallback;
   
   OpenThreads::Mutex theStagerMutex;
   osg::ref_ptr<StagerCallback> theStagerCallback;
   osg::ref_ptr<ossimPlanetOperation> theStager;
   
/*    osg::ref_ptr<ossimPlanetLookAt> theLookAt; */

};
#endif
