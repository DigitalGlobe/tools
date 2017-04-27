#ifndef ossimPlanetQtLegendItem_HEADER
#define ossimPlanetQtLegendItem_HEADER
#include <QtGui/QTreeWidgetItem>
#include <ossim/base/ossimXmlNode.h>
#include <osg/ref_ptr>
#include <ossimPlanet/ossimPlanetLookAt.h>
#include <ossimPlanet/ossimPlanetExtents.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class ossimPlanetOperation;
class ossimPlanetQtLegend;
class OSSIMPLANETQT_DLL ossimPlanetQtLegendItem : public QTreeWidgetItem
                                                  
{
public:
   ossimPlanetQtLegendItem(QTreeWidgetItem* item,
                           const QString& name);
   ossimPlanetQtLegendItem(QTreeWidget* treeWidget,
                           const QString& s);
   ossimPlanetQtLegendItem();
   enum DRAG_ACTION
   {
      REORDER, //change order of items (drag with left mouse button)
      INSERT, //insert an item into another one (drag with middle mouse button)
      NO_ACTION //do nothing
   };

   const ossimPlanetQtLegend* legend() const;
   ossimPlanetQtLegend* legend();
   virtual void deleteLayer(bool deleteThis=false)=0;
   virtual void changed();

   virtual ossimRefPtr<ossimXmlNode> saveXml()const=0;
   virtual void loadXml(ossimRefPtr<ossimXmlNode> node,
                        std::vector<ossimPlanetOperation*>& activityList)=0;

   virtual const osg::ref_ptr<ossimPlanetLookAt> getLookAt()const=0;
   virtual void setLookAt(osg::ref_ptr<ossimPlanetLookAt> lookAt)=0;

   virtual const osg::ref_ptr<ossimPlanetExtents> getExtents()const=0;
   virtual void propertyChanged(const ossimString& /*name*/){}
   
protected:
   virtual void recursiveAdd(QTreeWidgetItem* parent,
                             const ossimRefPtr<ossimXmlNode> node);
};

#endif
