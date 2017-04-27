#ifndef ossimPlanetQtLegendKmlItem_HEADER
#define ossimPlanetQtLegendKmlItem_HEADER
#include <ossimPlanetQt/ossimPlanetQtLegendNodeItem.h>
#include <ossimPlanet/ossimPlanetKmlLayerNode.h>
#include <ossimPlanet/ossimPlanetKml.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class OSSIMPLANETQT_DLL ossimPlanetQtLegendKmlItem : public ossimPlanetQtLegendNodeItem
{
public:
   ossimPlanetQtLegendKmlItem(QTreeWidgetItem* item,
                              const QString& name=QString(""));
   ossimPlanetQtLegendKmlItem(QTreeWidget* treeWidget,
                              const QString& s = QString(""));
   ossimPlanetQtLegendKmlItem();

   void setLayer(osg::ref_ptr<ossimPlanetNode> kml);
   virtual void changed();
   virtual ossimRefPtr<ossimXmlNode> saveXml()const;
   virtual void loadXml(ossimRefPtr<ossimXmlNode> node,
                        std::vector<ossimPlanetOperation*>& activityList);
   
protected:
	ossimPlanetKmlLayerNode* kmlLayerNode();
	const ossimPlanetKmlLayerNode* kmlLayerNode()const;
};

#endif
