#ifndef ossimPlanetQtLegendNodeItem_HEADER
#define ossimPlanetQtLegendNodeItem_HEADER
#include <QtGui/QTreeWidgetItem>
#include <ossim/base/ossimXmlNode.h>
#include <osg/ref_ptr>
#include <ossimPlanet/ossimPlanetNode.h>
#include <ossimPlanet/ossimPlanetLookAt.h>
#include <ossimPlanet/ossimPlanetExtents.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <ossimPlanetQt/ossimPlanetQtLegendItem.h>

class ossimPlanetOperation;
class ossimPlanetQtLegend;
class OSSIMPLANETQT_DLL ossimPlanetQtLegendNodeItem : public ossimPlanetQtLegendItem
{
public:
	class NodeCallback : public ossimPlanetNodeCallback
		{
		public:
			NodeCallback(ossimPlanetQtLegendNodeItem* item);
			virtual void propertyChanged(ossimPlanetNode* node,
												  const ossimString& name);
			virtual void destructingNode(ossimPlanetNode* node);
			virtual void needsRedraw(ossimPlanetNode* node);
			virtual void nodeAdded(osg::Node* /*node*/);
			virtual void nodeRemoved(osg::Node* /*node*/);
			
		protected:
			ossimPlanetQtLegendNodeItem* theItem;
		};
	
   ossimPlanetQtLegendNodeItem(QTreeWidgetItem* item,
                           const QString& name="");
   ossimPlanetQtLegendNodeItem(QTreeWidget* treeWidget,
                           const QString& s="");
   ossimPlanetQtLegendNodeItem();
	virtual ~ossimPlanetQtLegendNodeItem();
   virtual void deleteLayer(bool deleteThis=false);
	virtual void changed();
   virtual ossimRefPtr<ossimXmlNode> saveXml()const;
   virtual void loadXml(ossimRefPtr<ossimXmlNode> node,
                        std::vector<ossimPlanetOperation*>& activityList);
	const osg::ref_ptr<ossimPlanetLookAt> getLookAt()const;
   virtual void setLookAt(osg::ref_ptr<ossimPlanetLookAt> lookAt);
   virtual const osg::ref_ptr<ossimPlanetExtents> getExtents()const;
   
	osg::ref_ptr<ossimPlanetNode> layer();
	const osg::ref_ptr<ossimPlanetNode> layer()const;
	virtual void setLayer(osg::ref_ptr<ossimPlanetNode> value);
	virtual void propertyChanged(const ossimString& name);
protected:
	osg::ref_ptr<ossimPlanetNode>   theLayerNode;
	mutable osg::ref_ptr<ossimPlanetLookAt> theLookAt;
	mutable osg::ref_ptr<ossimPlanetExtents> theExtents;
	
	osg::ref_ptr<ossimPlanetQtLegendNodeItem::NodeCallback> theNodeCallback;
};

#endif
