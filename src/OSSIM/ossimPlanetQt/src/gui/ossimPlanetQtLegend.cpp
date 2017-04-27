#include <ossimPlanetQt/ossimPlanetQtLegend.h>
#include <ossimPlanetQt/ossimPlanetQtLegendItem.h>
#include <ossimPlanetQt/ossimPlanetQtLegendTextureItem.h>
#include <ossimPlanetQt/ossimPlanetQtMainWindow.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimGzStream.h>
#include <QtCore/Qt>
#include <QtGui/QFont>
#include <QtGui/QTextDocument>
#include <QtGui/QTextEdit>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidgetItemIterator>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <iostream>
#include <QtGui/QMessageBox>
#include <QtGui/QAction>
#include <QtCore/QUrl>
#include <ossimPlanet/ossimPlanetKml.h>
#include <ossimPlanet/ossimPlanetKmlLayer.h>
#include <ossimPlanet/ossimPlanetTextureLayerGroup.h>
#include <ossimPlanet/ossimPlanetTextureLayerRegistry.h>
#include <ossimPlanet/ossimPlanetAction.h>
#include <ossimPlanetQt/ossimPlanetQtActivityWindow.h>
#include <ossimPlanetQt/ossimPlanetQtLegendKmlItem.h>
#include <ossimPlanetQt/ossimPlanetQtLegendVideoItem.h>
#include <ossimPlanetQt/ossimPlanetQtLegendNodeItem.h>
#include <ossimPlanetQt/ossimPlanetQtLegendAnimationPathItem.h>
#include <ossimPlanetQt/ossimPlanetQtGlWidget.h>
#include <ossimPlanet/ossimPlanetDestinationCommandAction.h>
#include <ossimPlanet/ul.h>
#ifdef check
#undef check
#endif
#include <QtGui/QItemDelegate>
#include <stack>
struct LegendPopupMenuInfoType
{
   const char* theMenuItem;
   int         theMenuItemId; 
};

enum LegendPopupMenuIdType
{
	LegendPopupMenuId_GROUP   = 0,
	LegendPopupMenuId_EDIT,
	LegendPopupMenuId_DELETE,
	LegendPopupMenuId_ENABLE,
	LegendPopupMenuId_DISABLE,
	LegendPopupMenuId_LOOKAT_SET_TO_DEFAULT,
	LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_VIEW,
	LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_ORIENTATION,
	LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_RANGE,
	LegendPopupMenuId_STRETCH_NONE,
	LegendPopupMenuId_STRETCH_LINEAR_AUTO_MIN_MAX,
	LegendPopupMenuId_STRETCH_1STD,
	LegendPopupMenuId_STRETCH_2STD,
	LegendPopupMenuId_STRETCH_3STD,
	LegendPopupMenuId_TOUR_SELECTED_ITEMS,
	LegendPopupMenuId_CLEAR_DISK_CACHE,
	LegendPopupMenuId_REFRESH,
	LegendPopupMenuId_SYNCH,
};


static const LegendPopupMenuInfoType legendMenuInfo[] = 
{ 
	{"Group", (int)LegendPopupMenuId_GROUP},
	{"Edit", (int)LegendPopupMenuId_EDIT},
	{"Delete", (int)LegendPopupMenuId_DELETE},
	{"Enable", (int)LegendPopupMenuId_ENABLE},
	{"Disable", (int)LegendPopupMenuId_DISABLE},
	{"To Default View", (int)LegendPopupMenuId_LOOKAT_SET_TO_DEFAULT},
	{"To Current View", (int)LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_VIEW},
	{"To Current Orientation", (int)LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_ORIENTATION},
	{"To Current Range", (int)LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_RANGE},
	{"None", (int)LegendPopupMenuId_STRETCH_NONE},
	{"Linear Auto Min Max", (int)LegendPopupMenuId_STRETCH_LINEAR_AUTO_MIN_MAX},
	{"1 Standard Deviation", (int)LegendPopupMenuId_STRETCH_1STD},
	{"2 Standard Deviation", (int)LegendPopupMenuId_STRETCH_2STD},
	{"3 Standard Deviation", (int)LegendPopupMenuId_STRETCH_3STD},
	{"Tour Selected Items", (int)LegendPopupMenuId_TOUR_SELECTED_ITEMS},
	{"Clear Disk Cache", (int)LegendPopupMenuId_CLEAR_DISK_CACHE},
	{"Refresh", (int)LegendPopupMenuId_REFRESH},
	{"Synch", (int)LegendPopupMenuId_SYNCH},
};

ossimPlanetQtLegend::ossimPlanetQtLegend(QWidget * parent)
   :QTreeWidget(parent),
    theMainWindow(0),
//     theLandTextureRefreshFlag(true),
    theMouseDragFlag(false),
    theMousePressFlag(false),
    theItemBeingMoved(0),
    theLocationItem(0),
    theVideoItem(0),
    theAnimationPathRootItem(0)
{

//   setAcceptDrops(true);
   setSortingEnabled(false);
   setDragEnabled(false);
   setAutoScroll(true);
   QFont f("Arial", 10, QFont::Normal);
   setFont(f);
   setColumnCount(2);
   header()->setHidden(1);
   setRootIsDecorated(true);
   theItemPopup = new QMenu();
//   const LegendPopupMenuInfoType* legendMenuPtr = legendMenuInfo;

   // setup the popup menu
   thePopupMenuActions.push_back(theItemPopup->addAction(legendMenuInfo[LegendPopupMenuId_GROUP].theMenuItem));
   thePopupMenuActions.push_back(theItemPopup->addAction(legendMenuInfo[LegendPopupMenuId_EDIT].theMenuItem));
   thePopupMenuActions.push_back(theItemPopup->addAction(legendMenuInfo[LegendPopupMenuId_DELETE].theMenuItem));
   thePopupMenuActions.push_back(theItemPopup->addAction(legendMenuInfo[LegendPopupMenuId_ENABLE].theMenuItem));
   thePopupMenuActions.push_back(theItemPopup->addAction(legendMenuInfo[LegendPopupMenuId_DISABLE].theMenuItem));
   theItemPopup->addSeparator();
   QMenu* menu = theItemPopup->addMenu("Set Selected Layers Look At");
   thePopupMenuActions.push_back(menu->addAction(legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_DEFAULT].theMenuItem));
   thePopupMenuActions.push_back(menu->addAction(legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_VIEW].theMenuItem));
   thePopupMenuActions.push_back(menu->addAction(legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_ORIENTATION].theMenuItem));
   thePopupMenuActions.push_back(menu->addAction(legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_RANGE].theMenuItem));
   
   QMenu* menuHistogram = theItemPopup->addMenu("Histogram Stretch");
   thePopupMenuActions.push_back(menuHistogram->addAction(legendMenuInfo[LegendPopupMenuId_STRETCH_NONE].theMenuItem));
   thePopupMenuActions.push_back(menuHistogram->addAction(legendMenuInfo[LegendPopupMenuId_STRETCH_LINEAR_AUTO_MIN_MAX].theMenuItem));
   thePopupMenuActions.push_back(menuHistogram->addAction(legendMenuInfo[LegendPopupMenuId_STRETCH_1STD].theMenuItem));
   thePopupMenuActions.push_back(menuHistogram->addAction(legendMenuInfo[LegendPopupMenuId_STRETCH_2STD].theMenuItem));
   thePopupMenuActions.push_back(menuHistogram->addAction(legendMenuInfo[LegendPopupMenuId_STRETCH_3STD].theMenuItem));
   
   thePopupMenuActions.push_back(theItemPopup->addAction(legendMenuInfo[LegendPopupMenuId_TOUR_SELECTED_ITEMS].theMenuItem));
   thePopupMenuActions.push_back(theItemPopup->addAction(legendMenuInfo[LegendPopupMenuId_CLEAR_DISK_CACHE].theMenuItem));
   thePopupMenuActions.push_back(theItemPopup->addAction(legendMenuInfo[LegendPopupMenuId_REFRESH].theMenuItem));
   thePopupMenuActions.push_back(theItemPopup->addAction(legendMenuInfo[LegendPopupMenuId_SYNCH].theMenuItem));
   
   connect(theItemPopup, SIGNAL(triggered(QAction*)), this, SLOT(popupMenuTriggered(QAction*)));
   connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem * , int  )),
           this, SLOT(doubleClick(QTreeWidgetItem * , int  )));
   
   connect(this, SIGNAL(itemChanged(QTreeWidgetItem * , int  )),
           this, SLOT(changed(QTreeWidgetItem * , int  )));

   connect(this, SIGNAL(itemExpanded(QTreeWidgetItem * )),
           this, SLOT(expanded(QTreeWidgetItem * )));

   connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem * )),
           this, SLOT(collapsed(QTreeWidgetItem * )));
   theVersion = "1";
   setSelectionMode(QAbstractItemView::ExtendedSelection);
}

ossimPlanetQtLegend::~ossimPlanetQtLegend()
{
   theMainWindow = 0;
}

void ossimPlanetQtLegend::setMainWindow(ossimPlanetQtMainWindow* mainWindow)
{
   theMainWindow = mainWindow;
   populate();
}

void ossimPlanetQtLegend::populate()
{
   clear();
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lockTemp(theNodeIndexMapMutex);
		OpenThreads::ScopedLock<OpenThreads::Mutex> lockTemp2(theTextureLayerIndexMapMutex);
		theNodeIndexMap.clear();
		theTextureLayerIndexMap.clear();
	}
   if(!theMainWindow) return;
   
   ossim_uint32 size = theMainWindow->textureLayerGroupListSize();
   ossim_uint32 idx = 0;
   for(idx = 0; idx < size; ++idx)
   {
      const osg::ref_ptr<ossimPlanetTextureLayerGroup> layer = theMainWindow->textureLayerGroup(idx);
      if(layer.valid())
      {
         ossimPlanetQtLegendTextureItem* item = new ossimPlanetQtLegendTextureItem(this);
         item->setLayer(layer.get());

      }
   }
   
   theLocationItem          = new ossimPlanetQtLegendKmlItem(this, "Location");
   theVideoItem             = new ossimPlanetQtLegendVideoItem(this, "Video");
   theAnimationPathRootItem = new QTreeWidgetItem(this);
   theAnimationPathRootItem->setText(0, "Animation Paths");
   
   resizeColumnToContents(0);
   resizeColumnToContents(1);
}

void ossimPlanetQtLegend::populateLegend(ossimRefPtr<ossimXmlNode> legendNode)
{
   mainWindow()->iothread()->setPauseFlag(true);
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
   {  
      OpenThreads::ScopedLock<OpenThreads::Mutex> lockTemp(theNodeIndexMapMutex);
		OpenThreads::ScopedLock<OpenThreads::Mutex> lockTemp2(theTextureLayerIndexMapMutex);
      theNodeIndexMap.clear();
		theTextureLayerIndexMap.clear();
  }
   
   if(!legendNode.valid()) return;
   blockSignals(true);
   clear();
   theLocationItem = 0;
   theAnimationPathRootItem= 0;
   ossimPlanetQtLegendTextureItem* imageOperationLayer = new ossimPlanetQtLegendTextureItem(this, "Image Operation Layer");
   ossimPlanetQtLegendTextureItem* imageLayers          = new ossimPlanetQtLegendTextureItem(this, "Image Layers");
   theLocationItem          = new ossimPlanetQtLegendKmlItem(this, "Location");
   theVideoItem             = new ossimPlanetQtLegendVideoItem(this, "Video");
   theAnimationPathRootItem = new QTreeWidgetItem(this);
   theAnimationPathRootItem->setText(0, "Animation Paths");
   
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = legendNode->getChildNodes();
   std::vector<ossimPlanetOperation*> activityList;
   if(childNodes.size())
   {
      ossim_uint32 idx = 0;
      ossim_uint32 textureLayerCount = 0;
      ossimString attribute;
      ossimString type;
      for(idx = 0; idx < childNodes.size(); ++idx)
      {
         type = childNodes[idx]->getTag();
         if((type == "ossimPlanetTextureLayerGroup")&&
            (textureLayerCount < theMainWindow->textureLayerGroupListSize()))
         {
            const osg::ref_ptr<ossimPlanetTextureLayerGroup> layer = theMainWindow->textureLayerGroup(idx);
            if(idx == 0)
            {
               imageOperationLayer->setLayer(layer.get());
               imageOperationLayer->loadXml(childNodes[idx], activityList);
            }
            else if(idx == 1)
            {
               imageLayers->setLayer(layer.get());
               imageLayers->loadXml(childNodes[idx], activityList);
            }
            ++textureLayerCount;
         }
         else if(type == "ossimPlanetKml")
         {
            theLocationItem->loadXml(childNodes[idx], activityList);
         }
         else if(type == "ossimPlanetVideoLayer")
         {
            theVideoItem->loadXml(childNodes[idx], activityList);
         }
         else if(type == "AnimationPath")
         {
            ossimPlanetQtLegendAnimationPathItem* item = new ossimPlanetQtLegendAnimationPathItem(theAnimationPathRootItem, "");
            item->loadXml(childNodes[idx], activityList);
         }
      }
   }
   // quick hack for locations not being loaded until I have a full load and save for them
   if(activityList.size())
   {
      ossim_int32 idx = activityList.size()-1;
      for(;idx>=0; --idx)
      {
         mainWindow()->activityWindow()->theActivityWidget->addOperation(activityList[idx]);
         
      }
      activityList.clear();
   }
   resizeColumnToContents(0);
   resizeColumnToContents(1);
   blockSignals(false);
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lockTemp(theNodeIndexMapMutex);
      QTreeWidgetItemIterator locationIter(theLocationItem);
      while(*locationIter)
      {
         ossimPlanetQtLegendNodeItem* item = dynamic_cast<ossimPlanetQtLegendNodeItem*>(*locationIter);
         if(item)
         {
            if(item->layer().valid())
            {
					theNodeIndexMap[item->layer().get()] = item;
            }
         }
         ++locationIter;
      }      
      QTreeWidgetItemIterator videoIter(theVideoItem);
      while(*videoIter)
      {
         ossimPlanetQtLegendNodeItem* item = dynamic_cast<ossimPlanetQtLegendNodeItem*>(*videoIter);
         if(item)
         {
            if(item->layer().valid())
            {
					theNodeIndexMap[item->layer().get()] = item;
            }
         }
         ++videoIter;
      }      
   }
   mainWindow()->iothread()->setPauseFlag(false);
}

void ossimPlanetQtLegend::addTextureLayerToTop(osg::ref_ptr<ossimPlanetTextureLayer> layer)
{
   ossimPlanetQtLegendTextureItem* root = referenceTextureItem();
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock2(theTextureLayerIndexMapMutex);
		if(theTextureLayerIndexMap.find(layer.get()) != theTextureLayerIndexMap.end())
		{
			return;
		}
	}
   ossimPlanetTextureLayerGroup* group = root->layer()->asGroup();
   if(group)
   {
      group->addTop(layer.get());
   }
}
#if 0
ossimPlanetQtLegendTextureItem* ossimPlanetQtLegend::addTextureLayerToTop(osg::ref_ptr<ossimPlanetTextureLayer> layer)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
   ossimPlanetQtLegendTextureItem* root = referenceTextureItem();

   if(root)
   {
      return addTextureLayerToTop(layer, root);
   }

   return 0;
}
#endif
void ossimPlanetQtLegend::addAnimationPath(osg::ref_ptr<osg::AnimationPath> path)
{
   ossimPlanetQtLegendAnimationPathItem* animationPath = new ossimPlanetQtLegendAnimationPathItem(theAnimationPathRootItem, "Animation Path");
   animationPath->setAnimationPath(path);
}

void ossimPlanetQtLegend::addAnimationPath(ossimRefPtr<ossimXmlNode> path)
{
   ossimPlanetQtLegendAnimationPathItem* item = new ossimPlanetQtLegendAnimationPathItem(theAnimationPathRootItem, "");
   std::vector<ossimPlanetOperation*> activityList;
   item->loadXml(path, activityList);
}

#if 0


ossimPlanetQtLegendTextureItem* ossimPlanetQtLegend::addTextureLayerAfterItem(osg::ref_ptr<ossimPlanetTextureLayer> layer,
                                                                              ossimPlanetQtLegendTextureItem* item)
{
   ossimPlanetQtLegendTextureItem* i = 0;
   ossimPlanetTextureLayerGroup* group = item->layer()->getParent(0);
   if(group&&item->parent())
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock2(theTextureLayerIndexMapMutex);
		if(theTextureLayerIndexMap.find(layer.get()) != theTextureLayerIndexMap.end())
		{
			return 0;
		}
      i = new ossimPlanetQtLegendTextureItem();
      group->addAfterLayer(item->layer(), layer.get());
      item->parent()->insertChild(item->parent()->indexOfChild(item)+1,
                                  i);
      i->setLayer(layer.get());
		theTextureLayerIndexMap[layer.get()] = i;
      resizeColumnToContents(0);
      resizeColumnToContents(1);
   }
   else
   {
      return addTextureLayerToTop(layer);
   }

   return i;
}

ossimPlanetQtLegendTextureItem* ossimPlanetQtLegend::addTextureLayerToTop(osg::ref_ptr<ossimPlanetTextureLayer> layer,
																								  ossimPlanetQtLegendTextureItem* item)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock2(theTextureLayerIndexMapMutex);
	if(theTextureLayerIndexMap.find(layer.get()) != theTextureLayerIndexMap.end())
	{
		return 0;
	}
   ossimPlanetQtLegendTextureItem* i = new ossimPlanetQtLegendTextureItem();
   ossimPlanetTextureLayerGroup* group = item->layer()->asGroup();
   if(group)
   {
      group->addTop(layer.get());
      item->insertChild(0, i);
      i->setLayer(layer.get());
		theTextureLayerIndexMap[layer.get()] = i;
   }
   else
   {
      delete i;
      i = 0;
   }
   resizeColumnToContents(0);
   resizeColumnToContents(1);
	
   return i;
}
ossimPlanetQtLegendTextureItem* ossimPlanetQtLegend::addTextureLayerToBottom(osg::ref_ptr<ossimPlanetTextureLayer> layer,
                                                                             ossimPlanetQtLegendTextureItem* item)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock2(theTextureLayerIndexMapMutex);
	if(theTextureLayerIndexMap.find(layer.get()) != theTextureLayerIndexMap.end())
	{
		return 0;
	}
   ossimPlanetQtLegendTextureItem* i = new ossimPlanetQtLegendTextureItem();
   ossimPlanetTextureLayerGroup* group = item->layer()->asGroup();
   if(group)
   {
      group->addBottom(layer.get());
      item->addChild(i);
      i->setLayer(layer.get());
		theTextureLayerIndexMap[layer.get()] = i;
   }
   else
   {
      delete i;
      i = 0;
   }
   resizeColumnToContents(0);
   resizeColumnToContents(1);
	
   return i;
}

ossimPlanetQtLegendTextureItem* ossimPlanetQtLegend::addTextureLayerBeforeItem(osg::ref_ptr<ossimPlanetTextureLayer> layer,
                                                                               ossimPlanetQtLegendTextureItem* item)
{
   ossimPlanetQtLegendTextureItem* i = 0;
   ossimPlanetTextureLayerGroup* group = item->layer()->getParent(0);
   if(group&&item->parent())
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock2(theTextureLayerIndexMapMutex);
		if(theTextureLayerIndexMap.find(layer.get()) != theTextureLayerIndexMap.end())
		{
			return 0;
		}
      i = new ossimPlanetQtLegendTextureItem();
      group->addBeforeLayer(item->layer(),
                            layer.get());
      item->parent()->insertChild(item->parent()->indexOfChild(item),
                                  i);
      i->setLayer(layer.get());
		theTextureLayerIndexMap[layer.get()] = i;
      resizeColumnToContents(0);
      resizeColumnToContents(1);
   }
   else
   {
      return addTextureLayerToTop(layer, referenceTextureItem());
   }
	
   return i;
}
#endif

void ossimPlanetQtLegend::addKmlNode(osg::ref_ptr<ossimPlanetKmlLayerNode> kml)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
   if(theLocationItem)
   {
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock2(theNodeIndexMapMutex);
      ossimPlanetQtLegendKmlItem* item = new ossimPlanetQtLegendKmlItem(theLocationItem);
		theNodeIndexMap.insert(std::make_pair(kml.get(), item));
      item->setLayer(kml.get());
   }  
}


void ossimPlanetQtLegend::removeKmlNodeFromLegend(osg::ref_ptr<ossimPlanetKmlLayerNode> node)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
   QTreeWidgetItemIterator iter(theLocationItem);
   while(*iter)
   {
      ossimPlanetQtLegendKmlItem* kmlItem = dynamic_cast<ossimPlanetQtLegendKmlItem*>(*iter);
      if(kmlItem)
      {
         if(kmlItem->layer().get() == node.get())
         {
            if(kmlItem->parent())
            {
               QTreeWidgetItem* it = kmlItem->parent()->takeChild(kmlItem->parent()->indexOfChild(kmlItem));
               if(it) delete it;
               return;
            }
            else
            {
               QTreeWidgetItem* it = theLocationItem->takeChild(theLocationItem->indexOfChild(kmlItem));
               if(it) delete it;
               return;
            }
         }
      }
      ++iter;
   }
}

void ossimPlanetQtLegend::addVideoNode(osg::ref_ptr<ossimPlanetVideoLayerNode> node)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
   if(theVideoItem)
   {
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theNodeIndexMapMutex);
      ossimPlanetQtLegendVideoItem* item = new ossimPlanetQtLegendVideoItem(theVideoItem);
		theNodeIndexMap[node.get()] = item;
      item->setLayer(node.get());
   }  
}

void ossimPlanetQtLegend::removeVideoNodeFromLegend(osg::ref_ptr<ossimPlanetVideoLayerNode> node)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(thePopulateLegendMutex);
   QTreeWidgetItemIterator iter(theVideoItem);
   while(*iter)
   {
      ossimPlanetQtLegendVideoItem* videoItem = dynamic_cast<ossimPlanetQtLegendVideoItem*>(*iter);
      if(videoItem)
      {
         if(videoItem->layer().get() == node.get())
         {
            if(videoItem->parent())
            {
               QTreeWidgetItem* it = videoItem->parent()->takeChild(videoItem->parent()->indexOfChild(videoItem));
               if(it) delete it;
               return;
            }
            else
            {
               QTreeWidgetItem* it = theVideoItem->takeChild(theVideoItem->indexOfChild(videoItem));
               if(it) delete it;
               return;
            }
         }
      }
      ++iter;
   }
}

void ossimPlanetQtLegend::addNode(osg::ref_ptr<ossimPlanetNode> node)
{
	if(!node.valid()) return;
	ossimPlanetKmlLayerNode* kmlNode = dynamic_cast<ossimPlanetKmlLayerNode*>(node.get());
	if(kmlNode)
	{
		addKmlNode(kmlNode);
		return;
	}
	ossimPlanetVideoLayerNode* videoNode = dynamic_cast<ossimPlanetVideoLayerNode*>(node.get());
	if(videoNode)
	{
		addVideoNode(videoNode);
		return;
	}
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock1(thePopulateLegendMutex);
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock2(theNodeIndexMapMutex);
   
	ossimPlanetQtLegendNodeItem* item = new ossimPlanetQtLegendNodeItem(getParentNodeItem(node.get(), theLocationItem));
	theNodeIndexMap[node.get()] = item;
	item->setLayer(node.get());
}

void ossimPlanetQtLegend::addBookmark(osg::ref_ptr<ossimPlanetLookAt> lookAt, const QString& name, const QString& description )
{
   ossimPlanetAnnotationPlacemark* placemark = new ossimPlanetAnnotationPlacemark(osg::Vec3d(lookAt->lat(), lookAt->lon(), lookAt->altitude()),
                                                                                  lookAt->altitudeMode(),
                                                                                  ossimString("Bookmark"),
                                                                                  ossimString("Bookmark")); 
   mainWindow()->annotationLayer()->addChild(placemark);
   placemark->setLookAt(lookAt.get());
   mainWindow()->annotationLayer()->stagingThreadQueue()->add(new ossimPlanetAnnotationLayer::Stager(placemark));
}


void ossimPlanetQtLegend::removeNodeFromLegend(osg::ref_ptr<ossimPlanetNode> node, bool removeNodeFromLayerFlag)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock1(thePopulateLegendMutex);
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock2(theNodeIndexMapMutex);
   std::stack<ossimPlanetQtLegendNodeItem*> itemStack;
   if(node.valid())
   {
      NodeIndexMapType::iterator iter = theNodeIndexMap.find(node.get());
      if(iter != theNodeIndexMap.end())
      {
         if(iter->second->childCount() > 0)
         {
            std::stack<ossimPlanetQtLegendNodeItem*> needToTraverseStack;
            ossim_uint32 idx = 0;
            needToTraverseStack.push(iter->second);
            while(!needToTraverseStack.empty())
            {
               ossimPlanetQtLegendNodeItem* current = needToTraverseStack.top();
               needToTraverseStack.pop();
               itemStack.push(current);
               for(idx = 0; idx < (ossim_uint32)current->childCount(); ++idx)
               {
                  ossimPlanetQtLegendNodeItem* child = dynamic_cast<ossimPlanetQtLegendNodeItem*>(current->child(idx));
                  if(child)
                  {
                     if(child->childCount()>0)
                     {
                        needToTraverseStack.push(child);
                     }
                     else
                     {
                        itemStack.push(child);
                     }
                  }
               }
            }
            //QTreeWidgetItemIterator childIter(iter->second->child(0));
            //while(*childIter)
            //{
            //   ossimPlanetQtLegendNodeItem* test  = dynamic_cast<ossimPlanetQtLegendNodeItem*>(*childIter);
            //   if(test)
            //   {
            //      std::cout << test->text(0).toStdString() << std::endl;
            //      itemStack.push(test);
            //   }
            //   ++childIter;
           // }
         }
         else
         {
            itemStack.push(iter->second);
         }
         if(removeNodeFromLayerFlag)
         {
            while(!itemStack.empty())
            {
               if(itemStack.top()->layer().get())
               {
                  NodeIndexMapType::iterator tempIter = theNodeIndexMap.find(itemStack.top()->layer().get());
                  if(tempIter!=theNodeIndexMap.end())
                  {
                     theNodeIndexMap.erase(tempIter);
                  }
               }
               itemStack.top()->deleteLayer(true);
               itemStack.pop();
            }
         }
         else
         {
            while(!itemStack.empty())
            {
               if(itemStack.top()->layer().get())
               {
                  NodeIndexMapType::iterator tempIter = theNodeIndexMap.find(itemStack.top()->layer().get());
                  if(tempIter!=theNodeIndexMap.end())
                  {
                     theNodeIndexMap.erase(tempIter);
                  }
               }
               QTreeWidgetItem* it = itemStack.top()->parent()->takeChild(itemStack.top()->parent()->indexOfChild(itemStack.top()));
               if(it) delete it;
               itemStack.pop();
            }
         }
      }
   }
}

ossimPlanetQtLegendTextureItem* ossimPlanetQtLegend::referenceTextureItem()
{
   QTreeWidgetItemIterator iter(this);
   ossimPlanetQtLegendTextureItem* result = 0;
   while(*iter)
   {
      ossimPlanetQtLegendTextureItem* test  = dynamic_cast<ossimPlanetQtLegendTextureItem*>(*iter);

      if(test&&!test->parent())
      {
         result = test;
      }
      ++iter;
   }
   return result;
}


void ossimPlanetQtLegend::doubleClick(QTreeWidgetItem * item, int  /*columnNumber*/)
{
   ossimPlanetQtLegendItem* legendItem = dynamic_cast<ossimPlanetQtLegendItem*>(item);
   if(legendItem)
   {
      ossimPlanetQtLegendAnimationPathItem* animationItem = dynamic_cast<ossimPlanetQtLegendAnimationPathItem*>(legendItem);

      if(!animationItem)
      {
         const osg::ref_ptr<ossimPlanetLookAt> lookAt = legendItem->getLookAt();
         if(lookAt.valid())
         {
            ossimPlanetDestinationCommandAction(lookAt->toNavigationString()).execute();
         }
      }
      else
      {
         std::string animationPath;

         animationItem->getAnimationPathAsString(animationPath);

         istringstream in(animationPath);
         mainWindow()->manipulator()->loadRecording(in);
         mainWindow()->manipulator()->playRecording();
      }
   }
}

ossimPlanetQtMainWindow* ossimPlanetQtLegend::mainWindow()
{
   return theMainWindow;
}

// void ossimPlanetQtLegend::setLandTextureRefreshFlag(bool flag)
// {
//    theLandTextureRefreshFlag = flag;
// }

void ossimPlanetQtLegend::mouseDoubleClickEvent ( QMouseEvent * event )
{
   event->accept();
   QTreeWidgetItem* item = itemAt(event->pos());
   doubleClick(item, 0);
#if 0
   ossimPlanetQtLegendItem* legendItem = dynamic_cast<ossimPlanetQtLegendItem*>(item);
   if(legendItem)
   {
      const osg::ref_ptr<ossimPlanetLookAt> lookAt = legendItem->getLookAt();

      if(lookAt.valid())
      {
         ossimPlanetAction(lookAt->toNavigationString()).execute();
      }
   }
#endif
}

ossimRefPtr<ossimXmlNode> ossimPlanetQtLegend::saveXml()const
{
   ossimRefPtr<ossimXmlNode> result = new ossimXmlNode;

   result->setTag("ossimPlanetQtLegend");
//    result->setTag("object");
//    result->addAttribute("type", "ossimPlanetQtLegend");
   result->addAttribute("version", theVersion);

   int count = topLevelItemCount();
   int idx = 0;
   for(idx = 0; idx < count; ++idx)
   {
      ossimPlanetQtLegendItem* legendItem  = dynamic_cast<ossimPlanetQtLegendItem*>(topLevelItem(idx));
      
      if(legendItem)
      {
         ossimRefPtr<ossimXmlNode> node = legendItem->saveXml();
         if(node.valid())
         {
            result->addChildNode(node.get());
         }
      }
   }
   count = theAnimationPathRootItem->childCount();
   for(idx = 0; idx < count; ++idx)
   {
      ossimPlanetQtLegendItem* legendItem  = dynamic_cast<ossimPlanetQtLegendItem*>(theAnimationPathRootItem->child(idx));
      
      if(legendItem)
      {
         ossimRefPtr<ossimXmlNode> node = legendItem->saveXml();
         if(node.valid())
         {
            result->addChildNode(node.get());
         }
      }
   }
   
   return result.get();
}

void ossimPlanetQtLegend::loadXml(ossimRefPtr<ossimXmlNode> legendNode)
{
   populateLegend(legendNode.get());
}

void ossimPlanetQtLegend::addXml(ossimRefPtr<ossimXmlNode> node)
{
   blockSignals(true);
   ossimString type;
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   std::vector<ossimPlanetOperation*> activityList;
   ossimPlanetQtLegendTextureItem* imageLayers = referenceTextureItem();
   if(childNodes.size())
   {
      ossim_uint32 idx = 0;
      for(idx = 0; idx < childNodes.size(); ++idx)
      {
         type = childNodes[idx]->getTag();
         if(imageLayers&&
            ((type == "ossimPlanetTextureLayerGroup")||
             (type == "ossimPlanetOssimImageLayer")||
             (type == "ossimPlanetWmsImageLayer")))
         {
            imageLayers->addXml(childNodes[idx], activityList, true);
         }
//          else if(type == "ossimPlanetKml")
//          {
//             imageLayers>loadXml(childNodes[idx], activityList);
//          }
         else if(type == "AnimationPath")
         {
            ossimPlanetQtLegendAnimationPathItem* item = new ossimPlanetQtLegendAnimationPathItem(theAnimationPathRootItem, "");
            item->loadXml(childNodes[idx], activityList);
         }         
      }
   }
   if(activityList.size())
   {
      ossim_int32 idx = activityList.size()-1;
      for(;idx>=0; --idx)
      {
         mainWindow()->activityWindow()->theActivityWidget->addOperation(activityList[idx]);
         
      }
      activityList.clear();
   }
   resizeColumnToContents(0);
   resizeColumnToContents(1);
   blockSignals(false);
}

void ossimPlanetQtLegend::updateTextureLayer(ossimRefPtr<ossimPlanetTextureLayer> layer)
{
   if(!layer.valid()) return;
   QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::All);;
   while(*iter)
   {
      QTreeWidgetItem* item = *iter;
      ossimPlanetQtLegendTextureItem* textureItem = dynamic_cast<ossimPlanetQtLegendTextureItem*>(item);

      if(textureItem->layer().get() == layer.get())
      {
         textureItem->updateLayerInfo();
         return;
      }
      ++iter;
   }
}


// bool ossimPlanetQtLegend::landTextureRefreshFlag()const
// {
//    return theLandTextureRefreshFlag;
// }

void ossimPlanetQtLegend::changed(QTreeWidgetItem * item, int  /*columnNumber*/)
{
   ossimPlanetQtLegendItem* legendItem = dynamic_cast<ossimPlanetQtLegendItem*>(item);

   if(legendItem)
   {
      legendItem->changed();
   }
}

void ossimPlanetQtLegend::deleteSelectedItems()
{
   QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::Selected);
//    bool needRefresh = true;
   osg::ref_ptr<ossimPlanetExtents> extents = 0;
   while(*iter)
   {
      QTreeWidgetItem* item = *iter;
      ossimPlanetQtLegendItem* legendItem = dynamic_cast<ossimPlanetQtLegendItem*>(item);
      ++iter;
      if(legendItem&&(legendItem->parent()))
      {
         ossimPlanetQtLegendTextureItem* textureItem = dynamic_cast<ossimPlanetQtLegendTextureItem*>(legendItem);

         if(textureItem)
         {
            if(extents.valid())
            {
               extents->combine(textureItem->layer()->getExtents().get());
            }
            else
            {
               extents = textureItem->layer()->getExtents()->clone();
            }
         }
			ossimPlanetQtLegendNodeItem* nodeItem = dynamic_cast<ossimPlanetQtLegendNodeItem*>(legendItem);
			if(nodeItem)
			{
				if(nodeItem->layer().valid())
				{
					removeNodeFromLegend(nodeItem->layer().get(), true);
				}
				else
				{
					legendItem->deleteLayer(true);
				}
			}
			else
			{
				legendItem->deleteLayer(true);
			}
      }
   }
//    if(needRefresh)
//    {
//       theMainWindow->refreshLandTextures(extents.get(),
//                                          ossimPlanetPagedLandLodRefreshType_TEXTURE);
//    }
}

void ossimPlanetQtLegend::setEnableFlagOnSelectedItems(bool flag)
{
   QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::Selected);
   bool currentlyChecked = false;
   while(*iter)
   {
      currentlyChecked = (*iter)->checkState(0)!=Qt::Unchecked;
      if(currentlyChecked != flag)
      {
         (*iter)->setCheckState(0, flag?Qt::Checked:Qt::Unchecked);
      }
      ++iter;
   }
}

void ossimPlanetQtLegend::editSelectedItem()
{
   QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::Selected);
   osg::ref_ptr<ossimPlanetExtents> extents = 0;
   if(*iter)
   {
      editItem(*iter);
   }
}

void ossimPlanetQtLegend::clearDiskCacheOnSelectedItems()
{
   QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::Selected);
   while(*iter)
   {
      QTreeWidgetItem* item = *iter;
      ossimPlanetQtLegendTextureItem* legendItem = dynamic_cast<ossimPlanetQtLegendTextureItem*>(item);
      ++iter;
      if(legendItem)
      {
         ossimPlanetWmsImageLayer* wmsLayer = dynamic_cast<ossimPlanetWmsImageLayer*>(legendItem->layer().get());

         if(wmsLayer)
         {
            wmsLayer->clearDiskCache();
         }
      }
   }
}

void ossimPlanetQtLegend::groupSelectedItems()
{
   // for now I will only support grouping of texture items
   QList<QTreeWidgetItem*> items = selectedItems();
   QList<QTreeWidgetItem*>::iterator iter = items.begin();
   while(iter != items.end())
   {
      if(dynamic_cast<ossimPlanetQtLegendTextureItem*>(*iter))
      {
         if((*iter)->parent())
         {
            if((*iter)->parent()->isSelected())
            {
               (*iter)->parent()->setSelected(false);
            }
         }
      }
      else
      {
         (*iter)->setSelected(false);
      }
      
      ++iter;
   }
   
   items = selectedItems();
   if(items.size())
   {
      iter = items.begin();
		ossimPlanetQtLegendTextureItem* parentItem = dynamic_cast<ossimPlanetQtLegendTextureItem*>((*iter)->parent());
		ossimPlanetQtLegendTextureItem* item = dynamic_cast<ossimPlanetQtLegendTextureItem*>(*iter);
		ossimPlanetQtLegendTextureItem* newItem = new ossimPlanetQtLegendTextureItem();
		if(!parentItem||!item||!item->layer())
		{
			return;
		}
		osg::ref_ptr<ossimPlanetTextureLayerGroup> parentGroup = item->layer()->parent(0);
		if(!parentGroup) return;
		osg::ref_ptr<ossimPlanetTextureLayerGroup> group = new ossimPlanetTextureLayerGroup;
		parentGroup->blockCallbacks(true);
		parentGroup->addBeforeLayer(item->layer().get(), group.get());
		parentGroup->blockCallbacks(false);
		while(iter != items.end())
		{
			
			ossimPlanetQtLegendTextureItem* item = dynamic_cast<ossimPlanetQtLegendTextureItem*>(*iter);
			
			if(item)
			{
				osg::ref_ptr<ossimPlanetTextureLayer> layer = item->layer();
				removeIndexMapping(layer.get(), true);
				layer->detachFromParents();
				
				group->addBottom(layer.get());
			}
			++iter;
		}
		ossim_int32 idx = parentGroup->findLayerIndex(group.get());
		
		parentItem->insertChild(idx, newItem);
		group->dirtyExtents();
		group->updateExtents();
		parentGroup->dirtyExtents();
		parentGroup->updateExtents();
		newItem->setLayer(group.get());
	}
}

void ossimPlanetQtLegend::setLookAtForSelectedItems(osg::ref_ptr<ossimPlanetLookAt> lookAt)
{
   QList<QTreeWidgetItem*> items = selectedItems();
   QList<QTreeWidgetItem*>::iterator iter = items.begin();
   while(iter != items.end())
   {
      ossimPlanetQtLegendItem* legendItem = dynamic_cast<ossimPlanetQtLegendItem*>(*iter);
      if(legendItem)
      {
         if(lookAt.valid())
         {
            legendItem->setLookAt(lookAt->clone());
         }
         else
         {
            legendItem->setLookAt(0);
         }
      }
      ++iter;
   }
}

void ossimPlanetQtLegend::setLookAtToCurrentOrientationForSelectedItems(const osg::ref_ptr<ossimPlanetLookAt> lookAt)
{
   if(!lookAt.valid()) return;
   QList<QTreeWidgetItem*> items = selectedItems();
   QList<QTreeWidgetItem*>::iterator iter = items.begin();
   while(iter != items.end())
   {
      ossimPlanetQtLegendItem* legendItem = dynamic_cast<ossimPlanetQtLegendItem*>(*iter);
      if(legendItem)
      {
         osg::ref_ptr<ossimPlanetLookAt> look = legendItem->getLookAt();
         
         if(look.valid())
         {
            osg::ref_ptr<ossimPlanetLookAt> newLook = look->clone();
            newLook->setHeading(lookAt->heading());
            newLook->setPitch(lookAt->pitch());
            newLook->setRoll(lookAt->roll());
            legendItem->setLookAt(newLook);
         }
      }
      ++iter;
   }
}

void ossimPlanetQtLegend::setLookAtToCurrentRangeForSelectedItems(double range)
{
   QList<QTreeWidgetItem*> items = selectedItems();
   QList<QTreeWidgetItem*>::iterator iter = items.begin();
   while(iter != items.end())
   {
      ossimPlanetQtLegendItem* legendItem = dynamic_cast<ossimPlanetQtLegendItem*>(*iter);
      if(legendItem)
      {
         osg::ref_ptr<ossimPlanetLookAt> look = legendItem->getLookAt();
         
         if(look.valid())
         {
            osg::ref_ptr<ossimPlanetLookAt> newLook = look->clone();
            newLook->setRange(range);
            legendItem->setLookAt(newLook);
         }
      }
      ++iter;
   }
   
}


void ossimPlanetQtLegend::setHistogramStretchAndMode(ossimPlanetTextureLayerGroup* group,
                                                     bool enabledFlag,
                                                     const std::string& mode)
{
   if(group)
   {
      ossim_uint32 idx = 0;
      ossim_uint32 layers = group->numberOfLayers();
      for(;idx < layers; ++idx)
      {
         osg::ref_ptr<ossimPlanetTextureLayer> layer =  group->layer(idx);
         if(layer.valid())
         {
            ossimPlanetOssimImageLayer*   imageLayer      = dynamic_cast<ossimPlanetOssimImageLayer*>(layer.get());
            ossimPlanetTextureLayerGroup* imageLayerGroup = dynamic_cast<ossimPlanetTextureLayerGroup*>(layer.get());
            if(imageLayer)
            {
               imageLayer->setHistogramStretchEnableFlag(enabledFlag);
               imageLayer->setHistogramStretchMode(mode);
            }
            else if(imageLayerGroup)
            {
               setHistogramStretchAndMode(imageLayerGroup, enabledFlag, mode);
            }
         }
      }
   }
}

void ossimPlanetQtLegend::setHistogrmaStretchModeForSelectedItems(const QString& mode)
{
   QList<QTreeWidgetItem*> items = selectedItems();
   QList<QTreeWidgetItem*>::iterator iter = items.begin();
   bool stretchEnableFlag = mode!="None";
   std::string stretchMode = mode.toStdString();
   while(iter != items.end())
   {
      ossimPlanetQtLegendTextureItem* textureItem = dynamic_cast<ossimPlanetQtLegendTextureItem*>(*iter);
      if(textureItem)
      {
         ossimPlanetOssimImageLayer*   imageLayer = dynamic_cast<ossimPlanetOssimImageLayer*>(textureItem->layer().get());
         ossimPlanetTextureLayerGroup* imageLayerGroup = dynamic_cast<ossimPlanetTextureLayerGroup*>(textureItem->layer().get());
         if(imageLayer)
         {
            imageLayer->setHistogramStretchEnableFlag(stretchEnableFlag);
            imageLayer->setHistogramStretchMode(stretchMode);
         }
         else if(imageLayerGroup)
         {
            setHistogramStretchAndMode(imageLayerGroup, stretchEnableFlag, stretchMode);
         }
      }
      ++iter;
   }
}

void ossimPlanetQtLegend::synchSelectedItems()
{
   QList<QTreeWidgetItem*> items = selectedItems();
   QList<QTreeWidgetItem*>::iterator iter = items.begin();
   
   while(iter != items.end())
   {
      if(dynamic_cast<ossimPlanetQtLegendTextureItem*>(*iter))
      {
         if((*iter)->parent())
         {
            if((*iter)->parent()->isSelected())
            {
               (*iter)->parent()->setSelected(false);
            }
         }
      }
      else
      {
         (*iter)->setSelected(false);
      }
      
      ++iter;
   }
   iter = items.begin();
   ossimRefPtr<ossimXmlNode> result = new ossimXmlNode;
   result->setTag("ossimPlanetQtLegend");
   std::vector<ossimFilename> kmlFiles;
   while(iter != items.end())
   {
      ossimPlanetQtLegendItem* item = dynamic_cast<ossimPlanetQtLegendItem*>(*iter);
      if(item)
      {
         ossimPlanetQtLegendKmlItem* kmlItem = dynamic_cast<ossimPlanetQtLegendKmlItem*>(item);
         if(kmlItem)
         {
				ossimPlanetKmlLayerNode* kml = dynamic_cast<ossimPlanetKmlLayerNode*>( kmlItem->layer().get());
				osg::ref_ptr<ossimPlanetKmlObject> obj;
				if(kml)
				{
					obj = kml->kmlObject();
				}
            if(obj.valid())
            {
               ossimPlanetKmlObject* root = ossimPlanetKmlObject::getRoot(obj.get());
//                if(root&&(root != obj.get()))
//                {
//                   ossimRefPtr<ossimXmlNode> tempNode = new ossimXmlNode;
//                   obj->write(tempNode);
//                   std::ostringstream out;
                  
//                   out << *tempNode;
//                   mainWindow()->sendMessage(out.str(), "Data");
//                }
//                else if(root)
               if(root)
               {
                  ossimPlanetKml* kml = dynamic_cast<ossimPlanetKml*>(root);
                  if(!kml->isCompressed())
                  {
                     ossimFilename name = kml->filename();
//                      if(name.ext() != "kmz")
//                      {
//                         name.file();
                        if(std::find(kmlFiles.begin(), kmlFiles.end(), name.file()) == kmlFiles.end())
                        {
                           kmlFiles.push_back(name);
                        }
//                      }
                  }
               }
            }
         }
         else
         {
            result->addChildNode(item->saveXml());
         }
      }
      
      ++iter;
   }
   {
      std::ostringstream out;
      
      out << *result;
      mainWindow()->sendMessage("", out.str(), "Data");
   }
   
   if(!kmlFiles.empty())
   {
      ossim_uint32 idx = 0;
      for(idx = 0; idx < kmlFiles.size(); ++idx)
      {
         std::ifstream in(kmlFiles[idx].c_str());
         if(!in.fail())
         {
            std::vector<char> byteBuffer(kmlFiles[idx].fileSize());
            in.read((char*)(&byteBuffer.front()), byteBuffer.size());
            ossim_int64 bytesRead = in.gcount();
            
            std::ostringstream out;
            out << "<ossimPlanetQtKml " << kmlFiles[idx].file() << ">";
            out.write((char*)(&byteBuffer.front()), bytesRead);
            mainWindow()->sendMessage("", out.str(), "Data");
         }
      }
   }
   
}
void ossimPlanetQtLegend::unselectItems()
{
   QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::All);
   while(*iter)
   {
      (*iter)->setSelected(false);
      ++iter;
   }
}

void ossimPlanetQtLegend::selectLayersContainingPosition(double lat, double lon, double /*hgt*/)
{
   QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::All);
   while(*iter)
   {
      ossimPlanetQtLegendTextureItem* item = dynamic_cast<ossimPlanetQtLegendTextureItem*>(*iter);

      if(item)
      {
         osg::ref_ptr<ossimPlanetExtents> extents = item->getExtents();
         if(extents.valid())
         {
            double minLat=extents->getMinLat(),
               minLon=extents->getMinLon(),
               maxLat=extents->getMaxLat(),
               maxLon=extents->getMaxLon();
            if(lat >= minLat && lat <= maxLat && lon >=minLon && lon <=maxLon)
            {
               if(!item->isSelected())
               {
                  item->setSelected(true);
               }
            }
            else
            {
               if(item->isSelected())
               {
                  item->setSelected(false);
               }
               
            }
         }
         else
         {
            item->setSelected(false);
         }
      }
      else
      {
         (*iter)->setSelected(false);
      }
      ++iter;
   }
}

bool ossimPlanetQtLegend::event(QEvent* e)
{
   switch(e->type())
   {
      case ossimPlanetQt::ANIMATION_EVENT_TYPE:
      {
         ossimPlanetQt::AnimationPathEvent* pathEvent = dynamic_cast<ossimPlanetQt::AnimationPathEvent*>(e);
         if(pathEvent)
         {
            ossimString animationPath;
            ossim_uint32 idx = 0;
            ossimRefPtr<ossimXmlNode> node = new ossimXmlNode;
            for(idx = 0; idx < pathEvent->animationPathCount();++idx)
            {
               pathEvent->animationPath(animationPath, idx);
               if(!animationPath.empty())
               {
                  std::istringstream in(animationPath);
                  if(node->read(in))
                  {
                     addAnimationPath(node);
                  }
               }
            }
         }
         return true;
      }
      case ossimPlanetQt::LEGEND_XML_EVENT_TYPE:
      {
         ossimPlanetQt::LegendXmlEvent* xmlEvent = dynamic_cast<ossimPlanetQt::LegendXmlEvent*>(e);
         if(xmlEvent)
         {
            addXml(xmlEvent->xmlNode());
         }
         return true;
      }
      case ossimPlanetQt::KML_FILE_EVENT_TYPE:
      {
         ossimPlanetQt::KmlFileEvent* kmlEvent = dynamic_cast<ossimPlanetQt::KmlFileEvent*>(e);
         if(kmlEvent)
         {
            ossimFilename kmlFilename = kmlEvent->filename();
            bool isKmlFile = false;
            if(kmlFilename.ext() == "kmz")
            {
               isKmlFile = true;
            }
            else
            {
               std::ifstream in(kmlFilename.c_str(), std::ios::binary|std::ios::in);
               if(!in.fail())
               {
                  char buf[1024];
                  in.read(buf, 1024);
                  if(in.gcount())
                  {
                     if(ossimString(buf,
                                    buf + in.gcount()).contains("<kml"))
                     {
                        isKmlFile = true;
                     }
                  }
               }
            }
            if(isKmlFile)
            {
               mainWindow()->kmlLayer()->addKml(kmlFilename);
            }
         }
      }
      case ossimPlanetQt::NODE_CALLBACK_EVENT_TYPE:
      {
         ossimPlanetQt::NodeCallbackEvent* callbackEvent = dynamic_cast<ossimPlanetQt::NodeCallbackEvent*>(e);

         if(callbackEvent)
         {
            if(callbackEvent->nodeEventType() == ossimPlanetQt::NodeCallbackEvent::NodeEventType_ADD)
            {
					addNode(callbackEvent->node());
            }
            else if(callbackEvent->nodeEventType() ==  ossimPlanetQt::NodeCallbackEvent::NodeEventType_REMOVE)
            {
					removeNodeFromLegend(callbackEvent->node());  
           }
         }
         return true;
      }
      case ossimPlanetQt::IMAGE_LAYER_EVENT:
      {
         ossimPlanetQt::ImageLayerEvent* imageLayerEvent = dynamic_cast<ossimPlanetQt::ImageLayerEvent*>(e);
         if(imageLayerEvent)
         {
            switch(imageLayerEvent->actionType())
            {
               case ossimPlanetQt::ImageLayerEvent::ADD:
               {
						ossimPlanetQtLegendTextureItem* root = referenceTextureItem();
						
						if(root)
						{
							ossimPlanetTextureLayerGroup* group = root->layer()->asGroup();
							if(group)
							{
								group->addTop(imageLayerEvent->layer().get());
							}
						}
						break;
               }
					case ossimPlanetQt::ImageLayerEvent::ADDED:
               {
						bool indexedFlag = false;
						{
						  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theTextureLayerIndexMapMutex);
						
							indexedFlag = theTextureLayerIndexMap.find(imageLayerEvent->layer().get()) != theTextureLayerIndexMap.end();
						}
						if(!indexedFlag)
						{
							ossimPlanetTextureLayerGroup* parent = imageLayerEvent->layer()->parent(0);
							if(parent)
							{
								TextureLayerIndexMapType::iterator iter = theTextureLayerIndexMap.find(parent);
								ossimPlanetQtLegendTextureItem* root = referenceTextureItem();

								if(iter!=theTextureLayerIndexMap.end())
								{
									root = iter->second;
								}
								ossimPlanetQtLegendTextureItem* i = new ossimPlanetQtLegendTextureItem();
								root->insertChild(0, i);
								i->setLayer(imageLayerEvent->layer().get());
								resizeColumnToContents(0);
								resizeColumnToContents(1);
							}
						}
						break;
					}
					case ossimPlanetQt::ImageLayerEvent::REMOVED:
               {
						//OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theTextureLayerIndexMapMutex);
						//std::cout << "DOING REMOVED EVENT" << std::endl;
						removeIndexMapping(imageLayerEvent->layer().get(), true);
						break;
					}
            }
         }
         return true;
      }
      case ossimPlanetQt::NODE_PROPERTY_EVENT_TYPE:
		{
			ossimPlanetQt::NodePropertyEvent* nodePropertyEvent = dynamic_cast<ossimPlanetQt::NodePropertyEvent*>(e);
			if(nodePropertyEvent&&nodePropertyEvent->node())
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theNodeIndexMapMutex);
				NodeIndexMapType::iterator iter = theNodeIndexMap.find(nodePropertyEvent->node());
				if(iter!=theNodeIndexMap.end())
				{
					iter->second->propertyChanged(nodePropertyEvent->name());
				}
				return true;
			}
			break;
		}
      case ossimPlanetQt::LEGEND_ITEM_PROPERTY_EVENT:
      {
         ossimPlanetQt::LegendItemProperty* itemProperty = dynamic_cast<ossimPlanetQt::LegendItemProperty*>(e);
         if(itemProperty&&itemProperty->item())
         {
            itemProperty->item()->propertyChanged(itemProperty->name());
         }
         return true;
      }
      default:
      {
         break;
      }
   }
   
   return QTreeWidget::event(e);
}

void ossimPlanetQtLegend::removeIndexMapping(osg::ref_ptr<ossimPlanetTextureLayer> layer,
															bool deleteGuiItem)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theTextureLayerIndexMapMutex);
	TextureLayerIndexMapType::iterator iter = theTextureLayerIndexMap.find(layer.get());
	if(iter != theTextureLayerIndexMap.end())
	{
		if(deleteGuiItem)
		{
			delete iter->second;
		}
		theTextureLayerIndexMap.erase(iter);
	}
}

void ossimPlanetQtLegend::addIndexMapping(osg::ref_ptr<ossimPlanetTextureLayer> layer,
														ossimPlanetQtLegendTextureItem* item)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theTextureLayerIndexMapMutex);
	if(layer.valid() && item)
	{
		theTextureLayerIndexMap[layer.get()] = item;
	}
}

void ossimPlanetQtLegend::refreshSelectedItems()
{
   if(!theMainWindow) return;
   QTreeWidgetItemIterator iter(this, QTreeWidgetItemIterator::Selected);
   while(*iter)
   {
      QTreeWidgetItem* item = *iter;
      ossimPlanetQtLegendTextureItem* legendItem = dynamic_cast<ossimPlanetQtLegendTextureItem*>(item);
      ++iter;
      if(legendItem)
      {
         legendItem->layer()->notifyRefreshExtent(legendItem->layer()->getExtents());
//          theMainWindow->refreshLandTextures(legendItem->layer()->getExtents().get(),
//                                             ossimPlanetPagedLandLodRefreshType_TEXTURE);         
      }
   }
}

void ossimPlanetQtLegend::keyPressEvent ( QKeyEvent * event )
{
   switch(event->key())
   {
      case Qt::Key_Delete:
      {
         if(QMessageBox::question(this, "Delete Items", "Do you wish to delete the currently\nselected items?", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
         {
            deleteSelectedItems();
         }
         break;
      }
   }
}

void ossimPlanetQtLegend::mousePressEvent(QMouseEvent *e)
{
   if (e->button() == Qt::LeftButton)
   {
      theLastPressPos = e->pos();
      theMouseDragFlag = false;
      theMousePressFlag = true;
//       e->setAccepted(false);
   }
   else if(e->button() == Qt::RightButton)
   {
      thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_NONE].theMenuItemId]->setEnabled(false);
      thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_LINEAR_AUTO_MIN_MAX].theMenuItemId]->setEnabled(false);
      thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_1STD].theMenuItemId]->setEnabled(false);
      thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_2STD].theMenuItemId]->setEnabled(false);
      thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_3STD].theMenuItemId]->setEnabled(false);
      QTreeWidgetItem *destItem = itemAt(e->pos());
      ossimPlanetQtLegendTextureItem* destination = dynamic_cast<ossimPlanetQtLegendTextureItem*>(destItem);
      ossimPlanetQtLegendAnimationPathItem* animationPathItem = dynamic_cast<ossimPlanetQtLegendAnimationPathItem*>(destItem);
      if(destination)
      {
         ossimPlanetOssimImageLayer* imageLayer = dynamic_cast<ossimPlanetOssimImageLayer*>(destination->layer().get());
         if(dynamic_cast<ossimPlanetWmsImageLayer*>(destination->layer().get()))
         {
            thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_CLEAR_DISK_CACHE].theMenuItemId]->setEnabled(true);
         }
         else
         {
            thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_CLEAR_DISK_CACHE].theMenuItemId]->setEnabled(false);
         }
         if(!(destination->flags()&Qt::ItemIsEditable))
         {
            thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_EDIT].theMenuItemId]->setEnabled(false);
            
         }
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_TOUR_SELECTED_ITEMS].theMenuItemId]->setEnabled(false);
         theItemPopup->popup(QPoint(e->globalX(),
                                    e->globalY()));
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_DEFAULT].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_VIEW].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_ORIENTATION].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_RANGE].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_NONE].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_LINEAR_AUTO_MIN_MAX].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_1STD].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_2STD].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_3STD].theMenuItemId]->setEnabled(true);
         if(imageLayer)
         {
            thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_NONE].theMenuItemId]->setEnabled(true);
            thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_STRETCH_LINEAR_AUTO_MIN_MAX].theMenuItemId]->setEnabled(true);
         }
      }
      else if(animationPathItem)
      {
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_CLEAR_DISK_CACHE].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_TOUR_SELECTED_ITEMS].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_ENABLE].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_DISABLE].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_REFRESH].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_DEFAULT].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_VIEW].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_ORIENTATION].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_RANGE].theMenuItemId]->setEnabled(false);
         
         theItemPopup->popup(QPoint(e->globalX(),
                                    e->globalY()));
      }
      else
      {
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_CLEAR_DISK_CACHE].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_TOUR_SELECTED_ITEMS].theMenuItemId]->setEnabled(false);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_ENABLE].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_DISABLE].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_REFRESH].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_DEFAULT].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_VIEW].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_ORIENTATION].theMenuItemId]->setEnabled(true);
         thePopupMenuActions[legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_RANGE].theMenuItemId]->setEnabled(true);
         
         theItemPopup->popup(QPoint(e->globalX(),
                                    e->globalY()));
         
      }
//       QTreeWidgetItem* item = itemAt(e->pos());
//       setCurrentItem(item);
//       e->setAccepted(true);
      theMouseDragFlag = false;
      theMousePressFlag = false;
   }
      QTreeWidget::mousePressEvent(e);
   
}

void ossimPlanetQtLegend::mouseMoveEvent(QMouseEvent * /*e*/)
{
    if(theMousePressFlag)
    {
       // set the cursor one time and say we are dragging
       theMouseDragFlag  = true;
       theMousePressFlag = false;

	// remember item we've pressed as the one being moved
	// and where it was originally
// 	QTreeWidgetItem* item = itemAt(theLastPressPos);
// 	if(item)
	{
// 	    theItemBeingMoved = item; 
	    setCursor(Qt::SizeVerCursor);
	}
    }
}

void ossimPlanetQtLegend::mouseReleaseEvent(QMouseEvent * e)
{
  setCursor(QCursor(Qt::ArrowCursor));

  if(e->button() == Qt::LeftButton)//theItemBeingMoved)
  {
     if(theMouseDragFlag)
     {
        QTreeWidgetItem *destItem = itemAt(e->pos());
        ossimPlanetQtLegendTextureItem* destination = dynamic_cast<ossimPlanetQtLegendTextureItem*>(destItem);
        
        if(destination)
        {
           moveSelectedTexturesTo(destination);
        }
        theMouseDragFlag = false;
     }
  }
  else if(e->button() == Qt::RightButton)
  {
//      e->setAccepted(true);
  }
  else
  {
//      e->setAccepted(false);
//      QTreeWidget::mouseReleaseEvent(e);
  }
  QTreeWidget::mouseReleaseEvent(e);
}

void ossimPlanetQtLegend::moveTextureItemFromTo(ossimPlanetQtLegendTextureItem* itemBeingMoved,
                                                ossimPlanetQtLegendTextureItem* destination)
{
   if(!itemBeingMoved||!destination) return;
   bool itemBeingMovedExpanded = isItemExpanded(itemBeingMoved);
   if(itemBeingMoved->parent()&&(itemBeingMoved != destination))
   {
      ossimPlanetQtLegendTextureItem* itemBeingMovedParent = dynamic_cast<ossimPlanetQtLegendTextureItem*>(itemBeingMoved->parent());
      ossimPlanetQtLegendTextureItem* destinationParent    = dynamic_cast<ossimPlanetQtLegendTextureItem*>(destination->parent());
      bool needRefreshing = true;
      if(itemBeingMovedParent&&destinationParent)
      {
         itemBeingMovedParent->takeChild(itemBeingMovedParent->indexOfChild(itemBeingMoved));
         int idx = destinationParent->indexOfChild(destination);
         
         if((idx >=0) && (idx < destinationParent->childCount()))
         {
            if(isItemExpanded(destination))
            {
               destination->insertChild(0, itemBeingMoved);
               itemBeingMovedParent->layer()->asGroup()->removeLayer(itemBeingMoved->layer(), false);
               destination->layer()->asGroup()->addTop(itemBeingMoved->layer().get(), false);
               destination->layer()->asGroup()->notifyRefreshExtent(itemBeingMoved->layer()->getExtents());
            }
            else
            {
               if((idx+1) >= destinationParent->childCount()) // adding to the end of the list
               {
                 destinationParent->addChild(itemBeingMoved);
                  itemBeingMovedParent->layer()->asGroup()->removeLayer(itemBeingMoved->layer(), false);
                  destinationParent->layer()->asGroup()->addBottom(itemBeingMoved->layer().get(), false);
               }
               else // inserting a child
               {
                  itemBeingMovedParent->layer()->asGroup()->removeLayer(itemBeingMoved->layer(), false);
                  destinationParent->insertChild(idx+1, itemBeingMoved);
                  destinationParent->layer()->asGroup()->addAfterLayer(destination->layer().get(),
                                                                       itemBeingMoved->layer().get(), false);
                  destinationParent->layer()->asGroup()->notifyRefreshExtent(itemBeingMoved->layer()->getExtents());
               }
            }
         }
      }
      else if(!destination->parent())// it's a root item
      {
         if(itemBeingMovedParent->takeChild(itemBeingMovedParent->indexOfChild(itemBeingMoved))==itemBeingMoved)
         {
            destination->insertChild(0, itemBeingMoved);
            itemBeingMovedParent->layer()->asGroup()->removeLayer(itemBeingMoved->layer(), false);
            destination->layer()->asGroup()->addTop(itemBeingMoved->layer().get(), false);
            destination->layer()->asGroup()->notifyRefreshExtent(itemBeingMoved->layer()->getExtents());
         }
      }
      else
      {
         needRefreshing = false;
      }
      if(itemBeingMovedExpanded)
      {
         setItemExpanded(itemBeingMoved, itemBeingMovedExpanded);
      }
   }
}

void ossimPlanetQtLegend::moveSelectedTexturesTo(ossimPlanetQtLegendTextureItem* destination)
{
   if(!destination) return;
   // 
   if(destination->isSelected())
   {
      // for now we will not allow this.
   }
   else
   {
      // find actual items to move.  So we have to make sure we remove all item correctly
      // and avoid circular problems.  So if a child and it's parent is selscted this will be a no no.
      //
      QList<QTreeWidgetItem*> itemsToMove = selectedItems();
      QList<QTreeWidgetItem*>::iterator iter = itemsToMove.begin();
      while(iter != itemsToMove.end())
      {
         if(dynamic_cast<ossimPlanetQtLegendTextureItem*>(*iter))
         {
            if((*iter)->parent())
            {
               if((*iter)->parent()->isSelected())
               {
                  (*iter)->parent()->setSelected(false);
               }
            }
         }
         else
         {
            (*iter)->setSelected(false);
         }
         
         ++iter;
      }
      itemsToMove = selectedItems();
      iter = itemsToMove.begin();
      ossimPlanetQtLegendTextureItem* to = destination;
      while(iter != itemsToMove.end())
      {
         ossimPlanetQtLegendTextureItem* from = dynamic_cast<ossimPlanetQtLegendTextureItem*>(*iter);
         if(from)
         {
            moveTextureItemFromTo(from, to);
            if(to->parent() || to->isExpanded())
            {
               to = from;
            }
         }
         (*iter)->setSelected(true);
         ++iter;
      }
   }
}

ossimPlanetQtLegendNodeItem* ossimPlanetQtLegend::getParentNodeItem(ossimPlanetNode* node, ossimPlanetQtLegendNodeItem* defaultValue)
{
   ossimPlanetQtLegendNodeItem* result = defaultValue;
   if(node)
   {
      ossimPlanetNode* parentNode = 0;
      if((node->getNumParents()>0)&&node->getParent(0))
      {
         parentNode  = dynamic_cast<ossimPlanetNode*>(node->getParent(0));
      }
      NodeIndexMapType::iterator iter = theNodeIndexMap.end();
      if(parentNode)
      {
         iter = theNodeIndexMap.find(parentNode);
         if(iter != theNodeIndexMap.end())
         {
            if(iter->second)
            {
               result = iter->second;
            }
         }
      }
   }
   
   return result;
}

void ossimPlanetQtLegend::dropEvent ( QDropEvent * event )
{
	if(event->mimeData()->hasUrls())
   {
      osg::ref_ptr<ossimPlanetExtents> extents = 0;
      event->acceptProposedAction();
      QList<QUrl> urls = event->mimeData()->urls();
      std::vector<ossimFilename> sessionFiles;
      std::vector<ossimFilename> kmlFiles;
      std::vector<ossimFilename> plwCityFiles;
      std::vector<ossimFilename> videoFiles;
      QList<QUrl>::iterator i = urls.begin();
      
      while(i != urls.end())
      {         

         ossimFilename file(i->toLocalFile().toStdString().c_str());
         ossimString ext = file.ext();
         ext = ext.downcase();
         if((ext == "kml")||
            (ext == "kmz"))         
         {
            kmlFiles.push_back(file);
            i = urls.erase(i);
         }
         else if((ext == "mpeg") ||
                 (ext == "mpg") ||
                 (ext == "mp4")||
                 (ext == "m2p")||
                 (ext == "mpv")||
                 (ext == "m2v"))
         {
            videoFiles.push_back(file);
            i = urls.erase(i);
         }
         else if(file.empty())
         {
            i = urls.erase(i);
         }
         else
         {
            
            ifstream in(file.c_str(), std::ios::binary|std::ios::in);
            if(in)
            {
               char buf[1000];
               in.read(buf, 1000);
               int count = in.gcount();
               if(count > 0)
               {
                  ossimString header(buf, buf+count);
                  
                  if(header.contains("<ossimPlanetQt>"))
                  {
                     sessionFiles.push_back(ossimFilename(file.c_str()));
                     i = urls.erase(i);
                  }
                  else if(ossimString(header.begin(),
                                      header.begin() + 12) == "origin_WGS84")
                  {
                     plwCityFiles.push_back(ossimFilename(file.c_str()));
                     i = urls.erase(i);
                  }
                  else
                  {
                     ++i;
                  }
               }
               else
               {
                  ++i;
               }
               in.close();
            }
            else
            {
               ++i;
            }
         }
      }
		i = urls.begin();
		while(i != urls.end())
		{
			mainWindow()->activityWindow()->theActivityWidget->addOperation(new ossimPlanetQtOpenImageFileOperation(i->toLocalFile(), this)); 
			++i;
		}
		
      ossim_uint32 idx = 0;
      for(idx = 0; idx < kmlFiles.size(); ++idx)
      {
         ossimFilename kmlFilename = kmlFiles[idx];
         bool isKmlFile = false;
         ossimString ext = kmlFilename.ext();
         ext = ext.downcase();
         if(ext == "kmz")
         {
            isKmlFile = true;
         }
         else
         {
            std::ifstream in(kmlFilename.c_str(), std::ios::binary|std::ios::in);
            if(!in.fail())
            {
               char buf[1024];
               in.read(buf, 1024);
               if(in.gcount())
               {
                  if(ossimString(buf,
                                 buf + in.gcount()).contains("<kml"))
                  {
                     isKmlFile = true;
                  }
               }
            }
         }
         if(isKmlFile)
         {
            mainWindow()->kmlLayer()->addKml(kmlFilename);
         }
      }
#if 0
      for(idx = 0; idx < plwCityFiles.size(); ++idx)
      {
         mainWindow()->kmlLayer()->addCityDatabase(plwCityFiles[idx]);
      }
#endif
      for(idx = 0; idx < videoFiles.size();++idx)
      {
         mainWindow()->videoLayer()->add(videoFiles[idx]);
      }
   }
   else
   {
      if(event->mimeData()->hasText())
      {
      }
   }
}

void ossimPlanetQtLegend::dragEnterEvent(QDragEnterEvent *event)
{
   if (event->mimeData()->hasUrls()||
       event->mimeData()->hasText())
   {
     event->acceptProposedAction();
   }
}

void ossimPlanetQtLegend::popupMenuTriggered(QAction *action)
{
   if(action->text() == legendMenuInfo[LegendPopupMenuId_EDIT].theMenuItem)
   {
      editSelectedItem();
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_DELETE].theMenuItem)
   {
      if(QMessageBox::question(this, "Delete Items", "Do you wish to delete the currently\nselected items?", QMessageBox::Yes| QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
      {
         deleteSelectedItems();
      }
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_CLEAR_DISK_CACHE].theMenuItem)
   {
      if(QMessageBox::question(this, "Clear Disk Cache ", "Do you wish to clear any associated disk cache for this layer?", QMessageBox::Yes| QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
      {
         clearDiskCacheOnSelectedItems();
      }      
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_REFRESH].theMenuItem)
   {
      refreshSelectedItems();
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_GROUP].theMenuItem)
   {
      groupSelectedItems();
   }
   else if(action->text() ==  legendMenuInfo[LegendPopupMenuId_ENABLE].theMenuItem)
   {
      setEnableFlagOnSelectedItems(true);
   }
   else if(action->text() ==  legendMenuInfo[LegendPopupMenuId_DISABLE].theMenuItem)
   {
      setEnableFlagOnSelectedItems(false);
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_DEFAULT].theMenuItem)
   {
      setLookAtForSelectedItems(0);
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_VIEW].theMenuItem)
   {
      setLookAtForSelectedItems(theMainWindow->getLookAt()->clone());	
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_ORIENTATION].theMenuItem)
   {
      setLookAtToCurrentOrientationForSelectedItems(theMainWindow->getLookAt()->clone());
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_LOOKAT_SET_TO_CURRENT_RANGE].theMenuItem)
   {
      setLookAtToCurrentRangeForSelectedItems(theMainWindow->getLookAt()->range());     
   }
   else if((action->text() == legendMenuInfo[LegendPopupMenuId_STRETCH_LINEAR_AUTO_MIN_MAX].theMenuItem)||
           (action->text() == legendMenuInfo[LegendPopupMenuId_STRETCH_NONE].theMenuItem)||
           (action->text() == legendMenuInfo[LegendPopupMenuId_STRETCH_1STD].theMenuItem)||
           (action->text() == legendMenuInfo[LegendPopupMenuId_STRETCH_2STD].theMenuItem)||
           (action->text() == legendMenuInfo[LegendPopupMenuId_STRETCH_3STD].theMenuItem))
   {
      setHistogrmaStretchModeForSelectedItems(action->text());
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_TOUR_SELECTED_ITEMS].theMenuItem)
   {
   }
   else if(action->text() == legendMenuInfo[LegendPopupMenuId_SYNCH].theMenuItem)
   {
      synchSelectedItems();     
   }
}

void ossimPlanetQtLegend::expanded(QTreeWidgetItem * /*item*/)
{
   resizeColumnToContents(0);
   resizeColumnToContents(1);
}

void ossimPlanetQtLegend::collapsed(QTreeWidgetItem * /*item*/)
{
   resizeColumnToContents(0);
   resizeColumnToContents(1);
}

