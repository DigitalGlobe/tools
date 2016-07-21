#include <ossimPlanetQt/ossimPlanetQtLegendVideoItem.h>
#include <ossimPlanet/ossimPlanetVideoLayer.h>
#include <ossimPlanetQt/ossimPlanetQtLegend.h>
#include <ossimPlanet/ossimPlanetLookAt.h>
#include <QtGui/QApplication>

ossimPlanetQtLegendVideoItem::ossimPlanetQtLegendVideoItem(QTreeWidgetItem* item,
                                                           const QString& name)
   :ossimPlanetQtLegendNodeItem(item, name)
{
   theLookAt = new ossimPlanetLookAt;
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable|Qt::ItemIsEditable);
   setCheckState (0, Qt::Checked);
   theVideoControlItem = 0;
   theVideoControlWidget = 0;
   
  
}
ossimPlanetQtLegendVideoItem::ossimPlanetQtLegendVideoItem(QTreeWidget* treeWidget,
                                                         const QString& s)
   :ossimPlanetQtLegendNodeItem(treeWidget, s)
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
   setCheckState (0, Qt::Checked);
   theVideoControlItem = 0;
   theVideoControlWidget = 0;
}

ossimPlanetQtLegendVideoItem::ossimPlanetQtLegendVideoItem()
   :ossimPlanetQtLegendNodeItem()
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
   setCheckState (0, Qt::Checked);
   theVideoControlItem   = 0;
   theVideoControlWidget = 0;
}

ossimPlanetQtLegendVideoItem::~ossimPlanetQtLegendVideoItem()
{
}

void ossimPlanetQtLegendVideoItem::setLayer(osg::ref_ptr<ossimPlanetNode> node)
{
	ossimPlanetQtLegendNodeItem::setLayer(node.get());
   if(!theVideoControlItem)
   {
      theVideoControlItem = new QTreeWidgetItem(this);
      theVideoControlItem->setText(0, "");
      theVideoControlItem->setFlags(Qt::ItemIsEnabled);
      theVideoControlWidget = new ossimPlanetQtVideoControlWidget;
      
      theVideoControlItem->setSizeHint(0, theVideoControlWidget->sizeHint());
      legend()->setItemWidget(theVideoControlItem, 0, theVideoControlWidget);

   }
   if(videoLayerNode())
   {
      theVideoControlWidget->theReferenceTimeSlider->setRange(0, (int)videoLayerNode()->duration());
      theVideoControlWidget->theReferenceTimeSlider->setValue((int)videoLayerNode()->referenceTime());
		videoLayerNode()->lookAt(*theLookAt);
   }
   theVideoControlWidget->setVideoLayerNode(videoLayerNode());
   theVideoControlWidget->setCallback(theNodeCallback.get());
}

void ossimPlanetQtLegendVideoItem::deleteLayer(bool deleteThis)
{
	theVideoControlWidget->setVideoLayerNode(0);
	if(videoLayerNode())
	{
		videoLayerNode()->pause();
	}
	ossimPlanetQtLegendNodeItem::deleteLayer(deleteThis);
}

ossimRefPtr<ossimXmlNode> ossimPlanetQtLegendVideoItem::saveXml()const
{
   ossimRefPtr<ossimXmlNode> childNode = new ossimXmlNode;
   
   childNode->setTag("ossimPlanetVideoLayer");
   childNode->addChildNode("name", text(0).toStdString());
   
   std::cout << "NEED TO IMPLEMENT ossimPlanetQtLegendVideoItem::saveXml()!!!" << std::endl;

   return childNode.get();
}

void ossimPlanetQtLegendVideoItem::loadXml(ossimRefPtr<ossimXmlNode> node,
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
   std::cout << "NEED TO IMPLEMENT ossimPlanetQtLegendVideoItem::loadXml()!!!" << std::endl;
}

ossimPlanetVideoLayerNode* ossimPlanetQtLegendVideoItem::videoLayerNode()
{
	return dynamic_cast<ossimPlanetVideoLayerNode*>(theLayerNode.get());
}
const ossimPlanetVideoLayerNode* ossimPlanetQtLegendVideoItem::videoLayerNode()const
{
	return dynamic_cast<const ossimPlanetVideoLayerNode*>(theLayerNode.get());
}

void ossimPlanetQtLegendVideoItem::propertyChanged(const ossimString& name)
{
	if(name == "referenceTime")
	{
		if(videoLayerNode()&&
			theVideoControlWidget)
		{
			ossim_float64 newTime = videoLayerNode()->referenceTime();
			int newValue = (int)newTime;
			if(theVideoControlWidget->theReferenceTimeSlider->sliderPosition()!=newValue)
			{
				theVideoControlWidget->theReferenceTimeSlider->blockSignals(true);
				theVideoControlWidget->theReferenceTimeSlider->setValue(newValue);
				theVideoControlWidget->theReferenceTimeSlider->blockSignals(false);
			}
		}
	}
}

const osg::ref_ptr<ossimPlanetLookAt> ossimPlanetQtLegendVideoItem::getLookAt()const
{
	if(!theAutoUpdateLookAt.valid()) theAutoUpdateLookAt = new ossimPlanetLookAt();
	if(videoLayerNode())
	{
		videoLayerNode()->lookAt(*theAutoUpdateLookAt);
		theAutoUpdateLookAt->setRoll(0.0);
		theAutoUpdateLookAt->setPitch(fabs(theLookAt->pitch()));
	}
	
	return theAutoUpdateLookAt.get();
}
