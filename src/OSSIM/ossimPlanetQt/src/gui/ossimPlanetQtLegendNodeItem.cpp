#include <ossimPlanetQt/ossimPlanetQtLegendNodeItem.h>
#include <QtGui/QApplication>
#include <ossimPlanetQt/ossimPlanetQtEvent.h>
#include <ossimPlanetQt/ossimPlanetQtLegend.h>
#include <QtGui/QApplication>

ossimPlanetQtLegendNodeItem::NodeCallback::NodeCallback(ossimPlanetQtLegendNodeItem* item)
:theItem(item)
{
}

void ossimPlanetQtLegendNodeItem::NodeCallback::propertyChanged(ossimPlanetNode* node,
																					 const ossimString& name)
{
	if(!theItem) return;
	if(theItem->legend())
	{
		QApplication::postEvent(theItem->legend(), new ossimPlanetQt::NodePropertyEvent(node, name));
	}
}

void ossimPlanetQtLegendNodeItem::NodeCallback::destructingNode(ossimPlanetNode* node)
{
}

void ossimPlanetQtLegendNodeItem::NodeCallback::needsRedraw(ossimPlanetNode* node)
{
}

void ossimPlanetQtLegendNodeItem::NodeCallback::nodeAdded(osg::Node* node)
{
	if(!theItem) return;
	ossimPlanetNode* n = dynamic_cast<ossimPlanetNode*>(node);
	if(n)
	{
		QApplication::postEvent(theItem->legend(),
										new ossimPlanetQt::NodeCallbackEvent(n, ossimPlanetQt::NodeCallbackEvent::NodeEventType_ADD));
	}
}

void ossimPlanetQtLegendNodeItem::NodeCallback::nodeRemoved(osg::Node* node)
{
	if(!theItem) return;
	ossimPlanetNode* n = dynamic_cast<ossimPlanetNode*>(node);
	if(n)
	{
      QApplication::postEvent(theItem->legend(),
                           new ossimPlanetQt::NodeCallbackEvent(n, ossimPlanetQt::NodeCallbackEvent::NodeEventType_REMOVE));
	}
}

ossimPlanetQtLegendNodeItem::ossimPlanetQtLegendNodeItem(QTreeWidgetItem* item,
																			const QString& name)
:ossimPlanetQtLegendItem(item, name)
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable|Qt::ItemIsEditable);
	theNodeCallback = new ossimPlanetQtLegendNodeItem::NodeCallback(this);
}

ossimPlanetQtLegendNodeItem::ossimPlanetQtLegendNodeItem(QTreeWidget* treeWidget,
																			const QString& s)
:ossimPlanetQtLegendItem(treeWidget, s)
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable|Qt::ItemIsEditable);
	theNodeCallback = new ossimPlanetQtLegendNodeItem::NodeCallback(this);
}

ossimPlanetQtLegendNodeItem::ossimPlanetQtLegendNodeItem()
:ossimPlanetQtLegendItem()
{
   setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable|Qt::ItemIsEditable);
	theNodeCallback = new ossimPlanetQtLegendNodeItem::NodeCallback(this);
}
ossimPlanetQtLegendNodeItem::~ossimPlanetQtLegendNodeItem()
{
	if(theLayerNode.valid())
	{
		theLayerNode->removeCallback(theNodeCallback.get());
	}
}

void ossimPlanetQtLegendNodeItem::deleteLayer(bool deleteThis)
{
   if(theLayerNode.valid())
   {
		theLayerNode->removeCallback(theNodeCallback.get());
      if(theLayerNode->getParent(0))
      {
         theLayerNode->getParent(0)->removeChild(theLayerNode.get());
      }
      theLayerNode=0;
   }
   
   if(deleteThis)
   {
      delete this;
   }
}

void ossimPlanetQtLegendNodeItem::changed()
{
   if(theLayerNode.valid())
   {
      bool checked = (checkState(0) !=Qt::Unchecked);
      if(checked!=theLayerNode->enableFlag())
      {
         theLayerNode->setEnableFlag(checked);
         theLayerNode->setRedrawFlag(true);
      }
   
      if((text(0)!="") &&(text(0) != QString(theLayerNode->name().c_str())))
      {
         theLayerNode->setName(text(0).toStdString());
      }
   }
}

ossimRefPtr<ossimXmlNode> ossimPlanetQtLegendNodeItem::saveXml()const
{
	return 0;
}

void ossimPlanetQtLegendNodeItem::loadXml(ossimRefPtr<ossimXmlNode> node,
														std::vector<ossimPlanetOperation*>& activityList)
{
}

const osg::ref_ptr<ossimPlanetLookAt> ossimPlanetQtLegendNodeItem::getLookAt()const
{
	if(theLookAt.valid())
	{
		return theLookAt;
	}
	if(theLayerNode.valid())
	{
		return theLayerNode->lookAt();
	}
	
	return 0;
}

void ossimPlanetQtLegendNodeItem::setLookAt(osg::ref_ptr<ossimPlanetLookAt> lookAt)
{
	theLookAt = lookAt;
}

const osg::ref_ptr<ossimPlanetExtents> ossimPlanetQtLegendNodeItem::getExtents()const
{
	return theExtents;
}

osg::ref_ptr<ossimPlanetNode> ossimPlanetQtLegendNodeItem::layer()
{
	return theLayerNode;
}

const osg::ref_ptr<ossimPlanetNode> ossimPlanetQtLegendNodeItem::layer()const
{
	return theLayerNode;
}

void ossimPlanetQtLegendNodeItem::setLayer(osg::ref_ptr<ossimPlanetNode> value)
{
	if(theLayerNode.valid())
	{
		theLayerNode->removeCallback(theNodeCallback.get());
	}
	theLayerNode = value;
	if(theLayerNode.valid())
	{
		setCheckState (0, theLayerNode->enableFlag()?Qt::Checked:Qt::Unchecked);
		setText(0, theLayerNode->name().c_str());
		theLayerNode->addCallback(theNodeCallback.get());
	}
}

void ossimPlanetQtLegendNodeItem::propertyChanged(const ossimString& name)
{
	if(name == "name")
	{
		if(theLayerNode.valid())
		{
			setText(0, theLayerNode->name().c_str());
		}
	}
	else if(name == "enableFlag")
	{
		setCheckState (0, theLayerNode->enableFlag()?Qt::Checked:Qt::Unchecked);
	}
}


