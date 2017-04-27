#include <ossimPlanetQt/ossimPlanetQtLegendTextureItem.h>
#include <ossimPlanetQt/ossimPlanetQtApplication.h>
#include <ossimPlanet/ossimPlanetTextureLayerGroup.h>
#include <ossimPlanet/ossimPlanetOssimImageLayer.h>
#include <ossimPlanet/ossimPlanetWmsImageLayer.h>
#include <ossimPlanetQt/ossimPlanetQtLegend.h>
#include <ossimPlanetQt/ossimPlanetQtApplication.h>
#include <ossimPlanetQt/ossimPlanetQtOperation.h>
#include <ossimPlanetQt/ossimPlanetQtMainWindow.h>
#include <ossim/base/ossimEnvironmentUtility.h>
#include <QtGui/QApplication>
#include <ossimPlanetQt/ossimPlanetQtEvent.h>

ossimPlanetQtLegendTextureItem::Callback::Callback(ossimPlanetQtLegendTextureItem* item)
:theItem(item)
{
}

void ossimPlanetQtLegendTextureItem::Callback::layersAdded(std::vector<osg::ref_ptr<ossimPlanetTextureLayer> >& layers)
{
	ossim_uint32 idx = 0;
	
	for(idx = 0; idx < layers.size(); ++idx)
	{
	}
}

void ossimPlanetQtLegendTextureItem::Callback::layersRemoved(std::vector<osg::ref_ptr<ossimPlanetTextureLayer> >& layers)
{
	ossim_uint32 idx = 0;
	
	for(idx = 0; idx < layers.size(); ++idx)
	{
	}
}

void ossimPlanetQtLegendTextureItem::Callback::propertyChanged(const ossimString& name,
                                                               const ossimPlanetTextureLayer* layer)
{
   if(theItem)
   {
      ossimPlanetQtLegend* legend = theItem->legend();
     if(legend)
      {
         QApplication::postEvent(legend,
                                 new ossimPlanetQt::LegendItemProperty(name.c_str(),
                                                                       theItem));
      }
   }
}

void ossimPlanetQtLegendTextureItem::StagerCallback::finished(ossimPlanetOperation* /*operation*/)
{
   if(theItem)
   {
      theItem->theStagerMutex.lock();
      theItem->theStager = 0;
      theItem->theStagerMutex.unlock();
   }
}

void ossimPlanetQtLegendTextureItem::StagerCallback::canceled(ossimPlanetOperation* operation)
{
   finished(operation);
}

ossimPlanetQtLegendTextureItem::ossimPlanetQtLegendTextureItem(QTreeWidgetItem* item,
                                                               const QString& name)
   :ossimPlanetQtLegendItem(item, name)
{
   
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable|Qt::ItemIsEditable);
   setCheckState (0, Qt::Checked);
   theCallback = new ossimPlanetQtLegendTextureItem::Callback(this);
//    QIcon myIcon(ossimPlanetQtApplication::themePath().dirCat("mActionFolder.png").c_str());
//    setIcon(0, myIcon);
}

ossimPlanetQtLegendTextureItem::ossimPlanetQtLegendTextureItem(QTreeWidget* treeWidget,
                                                               const QString& name)
   :ossimPlanetQtLegendItem(treeWidget, name)
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
   setCheckState (0, Qt::Checked);
   theCallback = new ossimPlanetQtLegendTextureItem::Callback(this);
//    QIcon myIcon(ossimPlanetQtApplication::themePath().dirCat("mActionFolder.png").c_str());
//    setIcon(0, myIcon);
}

ossimPlanetQtLegendTextureItem::ossimPlanetQtLegendTextureItem()
   :ossimPlanetQtLegendItem()
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable|Qt::ItemIsEditable);
   setCheckState (0, Qt::Checked);
   theCallback = new ossimPlanetQtLegendTextureItem::Callback(this);
//    QIcon myIcon(ossimPlanetQtApplication::themePath().dirCat("mActionFolder.png").c_str());
//    setIcon(0, myIcon);
}

ossimPlanetQtLegendTextureItem::~ossimPlanetQtLegendTextureItem()
{
   theCallback->setItem(0);
   if(theLayer.valid())
   {
      theLayer->removeCallback(theCallback.get());
   }
}

void ossimPlanetQtLegendTextureItem::setLayer(osg::ref_ptr<ossimPlanetTextureLayer> layer)
{
   if(theLayer.valid())
   {
      theLayer->removeCallback(theCallback.get());
   }
   theLayer = layer;
   if(theLayer.valid())
   {
      theLayer->addCallback(theCallback.get());
   }
	if(legend())
	{
		legend()->addIndexMapping(layer.get(), this);
	}

   populateItem();
}

osg::ref_ptr<ossimPlanetTextureLayer> ossimPlanetQtLegendTextureItem::layer()
{
   return theLayer;
}

const osg::ref_ptr<ossimPlanetTextureLayer> ossimPlanetQtLegendTextureItem::layer()const
{
   return theLayer;
}


void ossimPlanetQtLegendTextureItem::updateLayerInfo()
{
   if(!theLayer.valid()) return;
   ossimString name = theLayer->getName();
   if(theLayer->getEnableFlag())
   {
      setCheckState (0, Qt::Checked);
      
   }
   else
   {
      setCheckState (0, Qt::Unchecked);
   }
   theStagerMutex.lock();
   if((theLayer->isStateSet(ossimPlanetTextureLayer_NO_OVERVIEWS)||
		theLayer->isStateSet(ossimPlanetTextureLayer_NO_HISTOGRAMS))&&
      ossimPlanetQtApplication::automaticStagingEnabledFlag())
   {
      if(!theStager.valid())
      {
         ossimPlanetOssimImageLayer* imageLayer = dynamic_cast<ossimPlanetOssimImageLayer*>(layer().get());
         if(imageLayer)
         {
            if(legend()&&legend()->mainWindow())
            {
					theStager = new ossimPlanetQtOssimImageStagerOperation(imageLayer,
																							 legend());
					legend()->mainWindow()->addOperation(theStager.get());
					ossimPlanetQtApplication::postEvent(legend()->mainWindow(),
																	new ossimPlanetQt::MessageEvent(QString("Staging layer ") + QString(layer()->name().c_str()),
																											  ossimPlanetQt::MessageEvent::INFO_MESSAGE_TYPE));
            }
         }
      }
   }
   theStagerMutex.unlock();
   if(theLayer->isStateSet(ossimPlanetTextureLayer_NO_SOURCE_DATA))
   {
      setForeground(0, QBrush(QColor(255, 0, 0)));
   }
   else
   {
      setForeground(0, QBrush(QColor(0, 0, 0)));
   }
   setText(0, name.c_str());
   if(!theLayer->asGroup())
   {
      clearItem();
      ossimRefPtr<ossimXmlNode> metadata = new ossimXmlNode;
      theLayer->getMetadata(metadata);
      
      ossim_uint32 idx = 0;
      ossim_uint32 idxMax = 0;
      const std::vector<ossimRefPtr<ossimXmlNode> >& childNodes = metadata->getChildNodes();
      idxMax = childNodes.size();
      for(idx = 0; idx < idxMax; ++idx)
      {
         recursiveAdd(this,
                      childNodes[idx]);
      }
   }
}


void ossimPlanetQtLegendTextureItem::populateItem()
{
   clearItem();
   if(!theLayer.valid()) return;
   theCallback->setEnableFlag(false);
   updateLayerInfo();
   
   if(theLayer.valid())
   {
      osg::ref_ptr<ossimPlanetTextureLayerGroup> group = theLayer->asGroup();

      if(group.valid())
      {
         unsigned int n = group->numberOfLayers();
         unsigned int idx = 0;
         for(idx = 0; idx < n; ++idx)
         {
            ossimPlanetQtLegendTextureItem* item = new ossimPlanetQtLegendTextureItem(this);
            item->setLayer(group->layer(idx));
         }
      }
   }
   theCallback->setEnableFlag(true);
}

void ossimPlanetQtLegendTextureItem::deleteLayer(bool deleteThis)
{
   if(!theLayer.valid()) return;
   theLayer->removeCallback(theCallback.get());

	legend()->removeIndexMapping(theLayer.get(), false);
	
   if(theLayer->getParent(0))
   {
      theLayer->getParent(0)->removeLayer(theLayer.get());
   }
	if(deleteThis)
	{
		delete this;
	}
}

void ossimPlanetQtLegendTextureItem::changed()
{
   if(!theLayer.valid())
   {
      return;
   }
   theCallback->setEnableFlag(false);
   if(text(0).toStdString() != "")
   {
      theLayer->setName(text(0).toStdString());
   }
   bool checked = (checkState(0) !=Qt::Unchecked);
   if(checked != theLayer->getEnableFlag())
   {
      theLayer->setEnableFlag(checked);
//      theLayer->notifyRefreshExtent(theLayer->getExtents());
//       if(legend()->landTextureRefreshFlag())
      {
//         legend()->mainWindow()->refreshLandTextures(theLayer->getExtents().get(),
//                                                     ossimPlanetLandRefreshType_TEXTURE);
      }
   }
   theCallback->setEnableFlag(true);
}

ossimRefPtr<ossimXmlNode> ossimPlanetQtLegendTextureItem::saveXml()const
{
   ossimRefPtr<ossimXmlNode> result;
   if(theLayer.valid())
   {
      theCallback->setEnableFlag(false);
      result  = theLayer->saveXml(false);
//       if(theLookAt.valid())
//       {
//          ossimRefPtr<ossimXmlNode> lookAtOverride = theLookAt->saveXml();
//          lookAtOverride->setTag("ossimPlanetLookAtOverride");
//          result->addChildNode(lookAtOverride);
//       }
      if(theLayer->asGroup())
      {
         ossim_uint32 idx = 0;
         ossim_uint32 idxBound = childCount();

         for(idx = 0; idx < idxBound; ++idx)
         {
            if(child(idx))
            {
               ossimPlanetQtLegendTextureItem* item = dynamic_cast<ossimPlanetQtLegendTextureItem*>(child(idx));
               if(item)
               {
                  result->addChildNode(item->saveXml());
               }
            }
         }
      }
      theCallback->setEnableFlag(true);
   }

   return result;
}

void ossimPlanetQtLegendTextureItem::loadXml(ossimRefPtr<ossimXmlNode> node,
                                             std::vector<ossimPlanetOperation*>& activityList)
{
   clearItem();
   theCallback->setEnableFlag(false);
   ossimRefPtr<ossimXmlNode> enabled = node->findFirstNode("enableFlag");
   ossimRefPtr<ossimXmlNode> name = node->findFirstNode("name");
   if(!enabled.valid())
   {
      enabled =  node->findFirstNode("enabled");
   }
   if(enabled.valid())
   {
      if(theLayer.valid())
      {
         theLayer->setEnableFlag(enabled->getText().toBool());
      }
   }
   if(name.valid())
   {
      if(theLayer.valid())
      {
         theLayer->setName(name->getText());
      }
      setText(0, name->getText().c_str());
   }
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   if(childNodes.size())
   {
      ossim_uint32 idx = 0;
      for(idx = 0; idx < childNodes.size(); ++idx)
      {
         addXml(childNodes[idx], activityList);
      }
   }
   theCallback->setEnableFlag(true);
}

void ossimPlanetQtLegendTextureItem::addXml(ossimRefPtr<ossimXmlNode> node,
                                            std::vector<ossimPlanetOperation*>& activityList,
                                            bool startAtTopFlag)
{
   ossimString type = node->getTag();
   ossimRefPtr<ossimXmlNode> enabled = node->findFirstNode("enableFlag");
   ossimRefPtr<ossimXmlNode> name    = node->findFirstNode("name");
   ossimRefPtr<ossimXmlNode> lookAtNode = node->findFirstNode("ossimPlanetLookAt");
	theLayer->blockCallbacks(true);
   if(type == "ossimPlanetTextureLayerGroup")
   {
      osg::ref_ptr<ossimPlanetTextureLayerGroup> layer = new ossimPlanetTextureLayerGroup;
      if(startAtTopFlag)
      {
         theLayer->asGroup()->addTop(layer.get());
      }
      else
      {
         theLayer->asGroup()->addBottom(layer.get());
      }
      if(name.valid())
      {
         layer->setName(name->getText());
      }
      else
      {
         layer->setName(layer->getClassName());
      }
      if(enabled.valid())
      {
         layer->setEnableFlag(enabled->getText().toBool());
      }
      if(lookAtNode.valid())
      {
         osg::ref_ptr<ossimPlanetLookAt> lookAt = new ossimPlanetLookAt;
         if(lookAt->loadXml(lookAtNode))
         {
            layer->setLookAt(lookAt);
         }
      }
      ossimPlanetQtLegendTextureItem* item = new ossimPlanetQtLegendTextureItem();
      if(startAtTopFlag)
      {
         this->insertChild(0, item);
      }
      else
      {
         this->addChild(item);
      }
      item->setLayer(layer.get());
      item->loadXml(node, activityList);
      if(!layer->getLookAt().valid())
      {
         layer->resetLookAt();
      }
      if(!item->parent()->isExpanded())
      {
         item->parent()->setExpanded(true);
      }            
   }
   else if(type == "ossimPlanetOssimImageLayer")
   {
      osg::ref_ptr<ossimPlanetOssimImageLayer> layer = new ossimPlanetOssimImageLayer;
      layer->blockCallbacks(true);
      if(startAtTopFlag)
      {
         theLayer->asGroup()->addTop(layer.get());
      }
      else
      {
         theLayer->asGroup()->addBottom(layer.get());
      }
      ossimPlanetQtLegendTextureItem* item = new ossimPlanetQtLegendTextureItem();//(this);
      if(startAtTopFlag)
      {
         this->insertChild(0, item);
      }
      else
      {
         this->addChild(item);
      }
      item->setLayer(layer.get());
      if(!item->parent()->isExpanded())
      {
         item->parent()->setExpanded(true);
      }
      if(name.valid())
      {
         layer->setName(name->getText());
         item->setText(0, name->getText().c_str());
      }
      
//                   ossimPlanetQtOpenImageFileFromLayerXmlActivity* activity = new ossimPlanetQtOpenImageFileFromLayerXmlActivity(layer.get(),
//                                                                                                                                 ossimFilename(filenameNode->getText()),
//                                                                                                                                 entryNode->getText().toInt32(),
//                                                                                                                                 legend());
      ossimPlanetQtOpenImageFileFromLayerXmlOperation* activity = new ossimPlanetQtOpenImageFileFromLayerXmlOperation(layer.get(),
                                                                                                                      node,
                                                                                                                      legend());
      activity->setItem(item);
      activityList.push_back(activity);
      
      item->loadXml(node, activityList);
      if(!item->parent()->isExpanded())
      {
         item->parent()->setExpanded(true);
      }
      layer->blockCallbacks(false);
  }
   else if(type == "ossimPlanetWmsImageLayer")
   {
      // Need to move all of this to the core planet
      ossimRefPtr<ossimXmlNode> cacheDirectory       = node->findFirstNode("cacheDirectory");
      if(cacheDirectory.valid())
      {
         ossimFilename testDir(cacheDirectory->getText());
         if(!testDir.exists())
         {
            testDir.createDirectory();
         }
         if(!testDir.exists()||!testDir.isWriteable())
         {
            ossimFilename file = ossimFilename(ossimPlanetQtApplication::defaultWmsCacheDir().toStdString());
            cacheDirectory->setText(file.c_str());
         }
      }
     
      osg::ref_ptr<ossimPlanetWmsImageLayer> layer = new ossimPlanetWmsImageLayer;
      if(layer->loadXml(node))
      {
         ossimRefPtr<ossimXmlNode> cacheDirectory       = node->findFirstNode("cacheDirectory");
         ossimPlanetQtLegendTextureItem* item = new ossimPlanetQtLegendTextureItem();//(this);
         if(startAtTopFlag)
         {
            this->insertChild(0, item);
         }
         else
         {
            this->addChild(item);
         }
        if(layer->getName() == "")
         {
            layer->setName(ossimString("WMS: ") +
                           ossimString(layer->getServer()));
         }
         item->setLayer(layer.get());
         if(!item->parent()->isExpanded())
         {
            item->parent()->setExpanded(true);
         }
         if(startAtTopFlag)
         {
            theLayer->asGroup()->addTop(layer.get());
         }
         else
         {
            theLayer->asGroup()->addBottom(layer.get());
         }
         item->loadXml(node, activityList);
         if(!item->parent()->isExpanded())
         {
            item->parent()->setExpanded(true);
         }
      }
   }
	theLayer->blockCallbacks(false);
#if 0
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   if(childNodes.size())
   {
      ossim_uint32 idx = 0;
      ossimString attribute;
      ossimString type;
      for(idx = 0; idx < childNodes.size(); ++idx)
      {
         type = childNodes[idx]->getTag();
         ossimRefPtr<ossimXmlNode> enabled = childNodes[idx]->findFirstNode("enableFlag");
          ossimRefPtr<ossimXmlNode> name    = childNodes[idx]->findFirstNode("name");
         ossimRefPtr<ossimXmlNode> lookAtNode = childNodes[idx]->findFirstNode("ossimPlanetLookAt");
         if(!enabled.valid())
         {
            enabled =  childNodes[idx]->findFirstNode("enabled");
         }
         if(type == "ossimPlanetTextureLayerGroup")
         {
            osg::ref_ptr<ossimPlanetTextureLayerGroup> layer = new ossimPlanetTextureLayerGroup;
            theLayer->asGroup()->addBottom(layer.get());

            if(name.valid())
            {
               layer->setName(name->getText());
            }
            else
            {
               layer->setName(layer->getClassName());
            }
            if(enabled.valid())
            {
               layer->setEnableFlag(enabled->getText().toBool());
            }
            if(lookAtNode.valid())
            {
               osg::ref_ptr<ossimPlanetLookAt> lookAt = new ossimPlanetLookAt;
               if(lookAt->loadXml(lookAtNode))
               {
                  layer->setLookAt(lookAt);
               }
            }
            ossimPlanetQtLegendTextureItem* item = new ossimPlanetQtLegendTextureItem(this);
            item->setLayer(layer.get());
            item->loadXml(childNodes[idx], activityList);
            if(!layer->getLookAt().valid())
            {
               layer->resetLookAt();
            }
            if(!item->parent()->isExpanded())
            {
               item->parent()->setExpanded(true);
            }            
         }
         else if(type == "ossimPlanetOssimImageLayer")
         {
            osg::ref_ptr<ossimPlanetOssimImageLayer> layer = new ossimPlanetOssimImageLayer;
            theLayer->asGroup()->addBottom(layer.get());
            ossimPlanetQtLegendTextureItem* item = new ossimPlanetQtLegendTextureItem(this);
            item->setLayer(layer.get());
            if(!item->parent()->isExpanded())
            {
               item->parent()->setExpanded(true);
            }
            if(name.valid())
            {
               layer->setName(name->getText());
               item->setText(0, name->getText().c_str());
            }
            
            ossimPlanetQtOpenImageFileFromLayerXmlActivity* activity = new ossimPlanetQtOpenImageFileFromLayerXmlActivity(layer.get(),
                                                                                                                          childNodes[idx],
                                                                                                                          legend());
            activity->setItem(item);
            activityList.push_back(activity);
            item->loadXml(childNodes[idx], activityList);
            if(!item->parent()->isExpanded())
            {
               item->parent()->setExpanded(true);
            }
         }
         else if(type == "ossimPlanetWmsImageLayer")
         {
            // Need to move all of this to the core planet
            
            osg::ref_ptr<ossimPlanetWmsImageLayer> layer = new ossimPlanetWmsImageLayer;
            if(layer->loadXml(childNodes[idx]))
            {
               ossimPlanetQtLegendTextureItem* item = new ossimPlanetQtLegendTextureItem(this);
               if(layer->getName() == "")
               {
                  layer->setName(ossimString("WMS: ") +
                                 ossimString(layer->getServer()));
               }
               item->setLayer(layer.get());
               if(!item->parent()->isExpanded())
               {
                  item->parent()->setExpanded(true);
               }
               theLayer->asGroup()->addBottom(layer.get());
               item->loadXml(childNodes[idx], activityList);
               if(!item->parent()->isExpanded())
               {
                  item->parent()->setExpanded(true);
               }
            }
         }
      }
   }
#endif
}


void ossimPlanetQtLegendTextureItem::clearItem()
{
   QList<QTreeWidgetItem *> children = takeChildren();
   QList<QTreeWidgetItem*>::Iterator iter = children.begin();

   while(iter != children.end())
   {
      delete *iter;
      ++iter;
   }

   children.clear();
}

const osg::ref_ptr<ossimPlanetLookAt> ossimPlanetQtLegendTextureItem::getLookAt()const
{
//    if(theLookAt.valid()) return theLookAt;
   if(theLayer.valid())
   {
		if(theLayer->getLookAt().valid())
		{
			return theLayer->getLookAt();
		}
		else
		{
			double lat, lon, len;
			theLayer->getCenterLatLonLength(lat, lon, len);
			
			osg::ref_ptr<ossimPlanetLookAt> lookAt = new ossimPlanetLookAt;
			
			lookAt->setLat(lat);
			lookAt->setLon(lon);
			lookAt->setRange(len);
			
			return lookAt;
		}
   }
   return 0;
}

void ossimPlanetQtLegendTextureItem::setLookAt(osg::ref_ptr<ossimPlanetLookAt> lookAt)
{
   theCallback->setEnableFlag(false);
   if(!lookAt.valid())
   {
      theLayer->resetLookAt();
   }
   else
   {
      theLayer->setLookAt(lookAt);
   }
   theCallback->setEnableFlag(true);
}

const osg::ref_ptr<ossimPlanetExtents> ossimPlanetQtLegendTextureItem::getExtents()const
{
   if(theLayer.valid())
   {
      return theLayer->getExtents();
   }

   return 0;
}

void ossimPlanetQtLegendTextureItem::propertyChanged(const ossimString& name)
{
	if(!theLayer.valid())
	{
		return;
	}
	if(name =="name")
	{
		setText(0, theLayer->name().c_str());
	}
	else if(name == "enableFlag")
	{
		if(theLayer->enableFlag())
		{
			setCheckState (0, Qt::Checked);
			
		}
		else
		{
			setCheckState (0, Qt::Unchecked);
		}
	}
	else 
	{
		theCallback->setEnableFlag(false);
		updateLayerInfo();
		theCallback->setEnableFlag(true);
	}
}

