#ifndef ossimPlanetQtLegendVideoItem_HEADER
#define ossimPlanetQtLegendVideoItem_HEADER
#include <ossimPlanetQt/ossimPlanetQtLegendNodeItem.h>
#include <ossimPlanet/ossimPlanetVideoLayerNode.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <ossimPlanetQt/ossimPlanetQtVideoControlWidget.h>

class ossimPlanetQtVideoControlWidget;
class OSSIMPLANETQT_DLL ossimPlanetQtLegendVideoItem : public ossimPlanetQtLegendNodeItem
{
public:
   ossimPlanetQtLegendVideoItem(QTreeWidgetItem* item,
                              const QString& name=QString(""));
   ossimPlanetQtLegendVideoItem(QTreeWidget* treeWidget,
                              const QString& s = QString(""));
   ossimPlanetQtLegendVideoItem();
   virtual ~ossimPlanetQtLegendVideoItem();
   virtual void deleteLayer(bool deleteThis);
   virtual ossimRefPtr<ossimXmlNode> saveXml()const;
   virtual void loadXml(ossimRefPtr<ossimXmlNode> node,
                        std::vector<ossimPlanetOperation*>& activityList);

   virtual void propertyChanged(const ossimString& name);
	virtual void setLayer(osg::ref_ptr<ossimPlanetNode> node);
	const osg::ref_ptr<ossimPlanetLookAt> getLookAt()const;
protected:
	void updateLookAt();
	mutable osg::ref_ptr<ossimPlanetLookAt> theAutoUpdateLookAt;
	ossimPlanetVideoLayerNode* videoLayerNode();
	const ossimPlanetVideoLayerNode* videoLayerNode()const;
	
   QTreeWidgetItem*                   theVideoControlItem;
   ossimPlanetQtVideoControlWidget*   theVideoControlWidget;
};

#endif
