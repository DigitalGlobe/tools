#include <ossimPlanetQt/ossimPlanetQtLegendKmlItem.h>
//#include <ossimPlanet/ossimPlanetLand.h>
#include <ossimPlanet/ossimPlanetGeoRefModel.h>
#include <ossimPlanetQt/ossimPlanetQtLegend.h>
#include <ossimPlanetQt/ossimPlanetQtMainWindow.h>

ossimPlanetQtLegendKmlItem::ossimPlanetQtLegendKmlItem(QTreeWidgetItem* item,
                                                       const QString& name)
   :ossimPlanetQtLegendNodeItem(item, name)
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
   setCheckState (0, Qt::Checked);
}

ossimPlanetQtLegendKmlItem::ossimPlanetQtLegendKmlItem(QTreeWidget* treeWidget,
                                                       const QString& s)
   :ossimPlanetQtLegendNodeItem(treeWidget, s)
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
   setCheckState (0, Qt::Checked);
}

ossimPlanetQtLegendKmlItem::ossimPlanetQtLegendKmlItem()
   :ossimPlanetQtLegendNodeItem()
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
   setCheckState (0, Qt::Checked);
}

void ossimPlanetQtLegendKmlItem::setLayer(osg::ref_ptr<ossimPlanetNode> kml)
{
	ossimPlanetQtLegendNodeItem::setLayer(kml.get());
   if(kmlLayerNode())
   {
      ossimPlanetKmlFeature* feature = dynamic_cast<ossimPlanetKmlFeature*>(kmlLayerNode()->kmlObject().get());
      
      if(feature)
      {
         setText(0, feature->name().c_str());
         osg::ref_ptr<ossimPlanetGeoRefModel> model = legend()->mainWindow()->planet()->model();
         if(feature->lookAt().valid()&&
				feature->lookAt()->lookAt().valid())
         {
            setLookAt(feature->lookAt()->lookAt());
            if(theLookAt->altitudeMode() == ossimPlanetAltitudeMode_CLAMP_TO_GROUND)
            {
               if(model.valid())
               {
                  theLookAt->setAltitude(model->getHeightAboveEllipsoid(theLookAt->lat(),
                                                                        theLookAt->lon()));
               }
            }
            else if(theLookAt->altitudeMode() == ossimPlanetAltitudeMode_RELATIVE_TO_GROUND)
            {
               if(model.valid())
               {
                  theLookAt->setAltitude(model->getHeightAboveEllipsoid(theLookAt->lat(),
                                                                        theLookAt->lon()));
               }               
            }
         }
         else
         {
            double lat, lon, h;
            if(feature->getCenter(lat, lon, h))
            {
               theLookAt = new ossimPlanetLookAt;
               theLookAt->setAll(lat, lon,
                                 model->getHeightAboveEllipsoid(lat,lon),
                                 0, 45, 0, 4000);
             }
         }
      }
      else
      {
			ossimPlanetKmlLayerNode* kmlNode = dynamic_cast<ossimPlanetKmlLayerNode*>(kml.get());
			ossimPlanetKml* kmlRoot = 0;
			if(kmlNode)
			{
				kmlRoot = dynamic_cast<ossimPlanetKml*>(kmlNode->kmlObject().get());
			}
         if(kmlRoot)
         {
            setText(0, kmlRoot->filename().file().c_str()); 
         }
      }
      ossim_uint32 idx = 0;
      ossim_uint32 bound = kml->getNumChildren();
      for(idx = 0; idx < bound; ++idx)
      {
         ossimPlanetKmlLayerNode* layerNode = dynamic_cast<ossimPlanetKmlLayerNode*>(kml->getChild(idx));
         if(layerNode)
         {
            ossimPlanetQtLegendKmlItem* item = new ossimPlanetQtLegendKmlItem(this, "");
            item->setLayer(layerNode);
         }
      }
   }
   else
   {
      theLookAt = 0;
   }
}

void ossimPlanetQtLegendKmlItem::changed()
{
   if(kmlLayerNode())
   {
      bool checked = (checkState(0) !=Qt::Unchecked);
      if(checked!=kmlLayerNode()->enableFlag())
      {
         kmlLayerNode()->setEnableFlag(checked);
         ossimPlanetLayer* layer = ossimPlanetLayer::findLayer(kmlLayerNode());
         if(layer)
         {
            layer->setRedrawFlag(true);
         }
      }
   }
}


ossimRefPtr<ossimXmlNode> ossimPlanetQtLegendKmlItem::saveXml()const
{
   ossimRefPtr<ossimXmlNode> childNode = new ossimXmlNode;
   
   childNode->setTag("ossimPlanetKml");
   childNode->addChildNode("name", text(0).toStdString());
   
   std::cout << "NEED TO IMPLEMENT ossimPlanetQtLegendKmlItem::saveXml()!!!" << std::endl;

   return childNode.get();
}

void ossimPlanetQtLegendKmlItem::loadXml(ossimRefPtr<ossimXmlNode> node,
                                         std::vector<ossimPlanetOperation*>& activityList)
{
   ossimRefPtr<ossimXmlNode> enabled = node->findFirstNode("enableFlag");
   ossimRefPtr<ossimXmlNode> name    = node->findFirstNode("name");
   if(!enabled.valid())
   {
      enabled =  node->findFirstNode("enabled");
   }
   if(enabled.valid())
   {
   }

   if(name.valid())
   {
      setText(0, name->getText().c_str());
   }
   std::cout << "NEED TO IMPLEMENT ossimPlanetQtLegendKmlItem::loadXml()!!!" << std::endl;
}

ossimPlanetKmlLayerNode* ossimPlanetQtLegendKmlItem::kmlLayerNode()
{
	return dynamic_cast<ossimPlanetKmlLayerNode*>(theLayerNode.get());
}

const ossimPlanetKmlLayerNode* ossimPlanetQtLegendKmlItem::kmlLayerNode()const
{
	return dynamic_cast<const ossimPlanetKmlLayerNode*>(theLayerNode.get());
}
