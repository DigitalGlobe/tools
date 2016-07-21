#ifndef ossimPlanetQtLegendAnimationPathItem_HEADER
#define ossimPlanetQtLegendAnimationPathItem_HEADER
#include <ossimPlanetQt/ossimPlanetQtLegendItem.h>
#include <osg/AnimationPath>
#include <osg/ref_ptr>

class ossimPlanetQtLegendAnimationPathItem : public ossimPlanetQtLegendItem
{
public:
   ossimPlanetQtLegendAnimationPathItem(QTreeWidgetItem* item,
                                  const QString& name);
   ossimPlanetQtLegendAnimationPathItem(QTreeWidget* treeWidget,
                           const QString& s);
   ossimPlanetQtLegendAnimationPathItem();
   
   void setAnimationPath(osg::ref_ptr<osg::AnimationPath> animationPath);
   osg::ref_ptr<osg::AnimationPath> animationPath();
   
   virtual void deleteLayer(bool deleteThis=false);
   virtual ossimRefPtr<ossimXmlNode> saveXml()const;
   virtual void loadXml(ossimRefPtr<ossimXmlNode> node,
                        std::vector<ossimPlanetOperation*>& activityList);

   virtual const osg::ref_ptr<ossimPlanetLookAt> getLookAt()const;
   virtual void setLookAt(osg::ref_ptr<ossimPlanetLookAt> lookAt);

   virtual const osg::ref_ptr<ossimPlanetExtents> getExtents()const;
   
   void getAnimationPathAsString(std::string& animationPath);
   void setAnimationPathFromString(const std::string& animationPath);
    
protected:
   osg::ref_ptr<osg::AnimationPath> theAnimationPath;
};

#endif
