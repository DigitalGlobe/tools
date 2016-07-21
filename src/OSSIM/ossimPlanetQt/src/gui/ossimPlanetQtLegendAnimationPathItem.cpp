#include <ossimPlanetQt/ossimPlanetQtLegendAnimationPathItem.h>
#include <sstream>
#include <ossimPlanet/ossimPlanetAction.h>

ossimPlanetQtLegendAnimationPathItem::ossimPlanetQtLegendAnimationPathItem(QTreeWidgetItem* item,
                                                               const QString& name)
   :ossimPlanetQtLegendItem(item, name)
{
   setFlags(Qt::ItemIsEditable| Qt::ItemIsEnabled| Qt::ItemIsSelectable);
}


ossimPlanetQtLegendAnimationPathItem::ossimPlanetQtLegendAnimationPathItem(QTreeWidget* treeWidget,
                                                               const QString& s)
   :ossimPlanetQtLegendItem(treeWidget, s)
{
   setFlags(Qt::ItemIsEditable| Qt::ItemIsEnabled| Qt::ItemIsSelectable);
}

ossimPlanetQtLegendAnimationPathItem::ossimPlanetQtLegendAnimationPathItem()
   :ossimPlanetQtLegendItem()
{
   setFlags(Qt::ItemIsEditable| Qt::ItemIsEnabled| Qt::ItemIsSelectable);
}

void ossimPlanetQtLegendAnimationPathItem::deleteLayer(bool deleteThis)
{
   if(deleteThis)
   {
      delete this;
   }
}

ossimRefPtr<ossimXmlNode> ossimPlanetQtLegendAnimationPathItem::saveXml()const
{
   ossimRefPtr<ossimXmlNode> result = new ossimXmlNode;
   
   result->setTag("AnimationPath");
   result->addChildNode("name",
                        text(0).toStdString());
  
   std::ostringstream out;
   if(theAnimationPath.valid()&&!theAnimationPath->empty())
   {
      theAnimationPath->write(out);

      result->addChildNode("path",
                           out.str());
   }

   return result;
}

void ossimPlanetQtLegendAnimationPathItem::loadXml(ossimRefPtr<ossimXmlNode> node,
                                             std::vector<ossimPlanetOperation*>& activityList)
{
   if(!node.valid()) return;
   ossimString tempValue;

   if(node->getChildTextValue(tempValue, "name"))
   {
      setText(0, tempValue.c_str());
   }
   if(node->getChildTextValue(tempValue, "path"))
   {   
      setAnimationPathFromString(tempValue);
   }  
}

const osg::ref_ptr<ossimPlanetLookAt> ossimPlanetQtLegendAnimationPathItem::getLookAt()const
{
   return 0;
}

void ossimPlanetQtLegendAnimationPathItem::setLookAt(osg::ref_ptr<ossimPlanetLookAt> lookAt)
{
}

const osg::ref_ptr<ossimPlanetExtents> ossimPlanetQtLegendAnimationPathItem::getExtents()const
{
   return 0;
}

void ossimPlanetQtLegendAnimationPathItem::setAnimationPath(osg::ref_ptr<osg::AnimationPath> animationPath)
{
   theAnimationPath = animationPath;
}

osg::ref_ptr<osg::AnimationPath> ossimPlanetQtLegendAnimationPathItem::animationPath()
{
   return theAnimationPath;
}

void ossimPlanetQtLegendAnimationPathItem::getAnimationPathAsString(std::string& animationPath)
{
   std::ostringstream out;

   animationPath = "";
   if(theAnimationPath.valid()&&(!theAnimationPath->empty()))
   {
      theAnimationPath->write(out);
      animationPath = out.str();
   }
}

void ossimPlanetQtLegendAnimationPathItem::setAnimationPathFromString(const std::string& animationPath)
{
   std::istringstream in(animationPath);
   
   if(!theAnimationPath)
   {
      theAnimationPath = new osg::AnimationPath;
   }
   else
   {
      theAnimationPath->getTimeControlPointMap().clear();
   }

   theAnimationPath->read(in);
}
