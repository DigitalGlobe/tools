#include <ossimPlanetQt/ossimPlanetQtActivityItem.h>
#include <ossimPlanetQt/ossimPlanetQtApplication.h>
#include <ossimPlanet/ossimPlanetTextureLayerRegistry.h>
#include <ossimPlanet/ossimPlanetConstants.h>
#include <ossimPlanetQt/ossimPlanetQtLegendTextureItem.h>
#include <ossimPlanetQt/ossimPlanetQtMainWindow.h>
#include <ossimPlanetQt/ossimPlanetQtLegend.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderBase.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <sstream>

ossimPlanetQtActivityItem::ossimPlanetQtActivityItem(QTreeWidget * parent, int type)
   :QTreeWidgetItem(parent, type),
    theState(ossimPlanetQtActivityItem::READY)
{
   theThread = 0;
   theInitializeGuiFlag = true;
}

ossimPlanetQtActivityItem::ossimPlanetQtActivityItem(QTreeWidgetItem * parent, int type)
   :QTreeWidgetItem(parent, type),
    theState(ossimPlanetQtActivityItem::READY)
{
   theThread = 0;
   theInitializeGuiFlag = true;
}

ossimPlanetQtActivityItem::ossimPlanetQtActivityItem(int type)
   :QTreeWidgetItem(type),
    theState(ossimPlanetQtActivityItem::READY)
{   
   theThread = 0;
   theInitializeGuiFlag = true;
}

void ossimPlanetQtActivityItem::setState(ossimPlanetQtActivityItem::State state)
{
   QMutexLocker lock(&theMutex);
   theState = state;
}

ossimPlanetQtActivityItem::State ossimPlanetQtActivityItem::state()const
{
   QMutexLocker lock(&theMutex);
   
   return theState;
}

const ossimString& ossimPlanetQtActivityItem::errorMessage()const
{
   return theErrorMessage;
}

void ossimPlanetQtActivityItem::setThread(QThread* thread)
{
   theThread = thread;
}

ossimPlanetQtOssimImageStagerActivity::ossimPlanetQtOssimImageStagerActivity(QTreeWidget * parent,
                                                                                 ossimPlanetOssimImageLayer* layer,
                                                                                 int type)
   :ossimPlanetQtActivityItem(parent, type),
    theLayer(layer)
{
   
}

ossimPlanetQtOssimImageStagerActivity::ossimPlanetQtOssimImageStagerActivity(QTreeWidgetItem * parent,
                                                                                 ossimPlanetOssimImageLayer* layer,
                                                                                 int type)
   :ossimPlanetQtActivityItem(parent, type),
    theLayer(layer)
{
}

ossimPlanetQtOssimImageStagerActivity::ossimPlanetQtOssimImageStagerActivity(ossimPlanetOssimImageLayer* layer,
                                                                                 ossimPlanetQtLegend* legend,
                                                                                 int type)
   :ossimPlanetQtActivityItem(type),
    theLayer(layer),
    theLegend(legend)
{
}

void ossimPlanetQtOssimImageStagerActivity::execute()
{
   theMutex.lock();
   theState = ossimPlanetQtActivityItem::EXECUTING;
   theMutex.unlock();
	
   if(theLayer->getHandler().valid()&&
		(theLayer->isStateSet(ossimPlanetTextureLayer_NO_OVERVIEWS)||
		 theLayer->isStateSet(ossimPlanetTextureLayer_NO_HISTOGRAMS)))
   {
		ossimRefPtr<ossimImageHandler> handler = (ossimImageHandler*)(theLayer->getHandler()->dup());

		if(theLayer->isStateSet(ossimPlanetTextureLayer_NO_OVERVIEWS))
		{
			buildOverviews(handler.get());
		}
		if(theLayer->isStateSet(ossimPlanetTextureLayer_NO_HISTOGRAMS)&&
			(!theLayer->getHandler()->getClassName().contains("OgrGdal")))
		{
			buildHistograms(handler.get());
		}
   }
}

void ossimPlanetQtOssimImageStagerActivity::adjustToWriteableLocation(ossimFilename& file)const
{
	if(file.path().isWriteable())
	{
		return;
	}
	ossimString drivePart;
	ossimString pathPart;
	ossimString filePart;
	ossimString extPart;
	
	file.split(drivePart, pathPart, filePart, extPart);
	
	ossimFilename stagingCache(ossimPlanetQtApplication::stagingCacheDirectory().toStdString().c_str());
	
	if(!stagingCache.empty())
	{
		stagingCache = stagingCache.dirCat(pathPart);
		if(!stagingCache.exists())
		{
			stagingCache.createDirectory();
		}
		stagingCache = stagingCache.dirCat(filePart);
		stagingCache += ossimFilename("." + extPart);
		
		file = stagingCache;
	}
}

void ossimPlanetQtOssimImageStagerActivity::buildOverviews(ossimImageHandler* handler)
{
	theMutex.lock();
	ossimRefPtr<ossimOverviewBuilderBase> builder = ossimOverviewBuilderFactoryRegistry::instance()->createBuilder("ossim_tiff_box");
	builder->setInputSource(handler);
	ossimFilename overviewLocation = handler->createDefaultOverviewFilename();
	theMutex.unlock();
	adjustToWriteableLocation(overviewLocation);
	builder->addListener((ossimProcessListener*)this);
	theFilename = overviewLocation;
	if(overviewLocation.path().isWriteable())
	{
		builder->setOutputFile(overviewLocation);
		builder->execute();
	}
	builder->removeListener((ossimProcessListener*)this);	

   if(overviewLocation.exists())
   {
      theLayer->setOverviewFile(overviewLocation);
      theLayer->dirtyExtents();
   }
}

void ossimPlanetQtOssimImageStagerActivity::buildHistograms(ossimImageHandler* handler)
{
	theMutex.lock();
	ossimFilename histogramFile = handler->createDefaultHistogramFilename();
	theMutex.unlock();
	adjustToWriteableLocation(histogramFile);
	ossimRefPtr<ossimImageHistogramSource> histoSource = new ossimImageHistogramSource;
	ossimRefPtr<ossimHistogramWriter> writer = new ossimHistogramWriter;
	histoSource->connectMyInputTo(0, handler);
	histoSource->enableSource();
	histoSource->setComputationMode(OSSIM_HISTO_MODE_FAST);
	writer->connectMyInputTo(0, histoSource.get());
	theFilename = histogramFile;

	writer->setFilename(histogramFile);
	writer->addListener((ossimProcessListener*)this);
	writer->execute();
	writer->removeListener((ossimProcessListener*)this);
	if(histogramFile.exists())
	{
		theLayer->setHistogramFile(histogramFile);
		theLayer->setHistogramStretchEnableFlag(true);
	}
}

void ossimPlanetQtOssimImageStagerActivity::processProgressEvent(ossimProcessProgressEvent& event)
{
   QMutexLocker lock(&theMutex);
   theStatus = ossimString::toString((ossim_uint32)(event.getPercentComplete())).string() +
      "% " + event.getMessage().c_str()+ "\n" +
      "file: " + theFilename.string() + "\n";
   
}

void ossimPlanetQtOssimImageStagerActivity::updateGui()
{
   QMutexLocker lock(&theMutex);

   if(theInitializeGuiFlag)
   {
      theInitializeGuiFlag = false;
      theStatus = theLayer->getHandler()->getFilename().string();
   }
   
   setText(0, theStatus.c_str());
   
   if(theState == ossimPlanetQtActivityItem::FINISHED)
   {
      theLegend->mainWindow()->refreshLandTextures(theLayer->getExtents()->clone(), ossimPlanetLandRefreshType_TEXTURE);
   }
}

ossimPlanetQtOpenImageFileFromLayerActivity::ossimPlanetQtOpenImageFileFromLayerActivity(osg::ref_ptr<ossimPlanetOssimImageLayer> layer,
                                                                                         const ossimFilename& name,
                                                                                         ossim_int32 entry,
                                                                                         ossimPlanetQtLegend* legend)
    :ossimPlanetQtActivityItem(),
     theLayer(layer),
     theFilename(name),
     theEntry(entry),
     theLegend(legend)
{
   theName = layer->getName();
   theDescription = layer->getDescription();
   theInitializeGuiFlag = true;
   theItem = 0;
   theSavedLookAt = layer->getLookAt();
}

void ossimPlanetQtOpenImageFileFromLayerActivity::execute()
{
   theMutex.lock();
   theState = ossimPlanetQtActivityItem::EXECUTING;
   theErrorMessage = "";
   theStatus = "Opening " + theFilename;
   theMutex.unlock();

   if(theLayer.valid())
   {
      if(theLayer->openImage(theFilename, theEntry))
      {
         ossimPlanetOssimImageLayer* imageLayer = dynamic_cast<ossimPlanetOssimImageLayer*>(theLayer.get());
         if(imageLayer)
         {
            QString stretchMode = ossimPlanetQtApplication::autoHistogramStretchMode();
            if(!stretchMode.isEmpty())
            {
               imageLayer->setHistogramStretchMode(stretchMode.toStdString());
               if(stretchMode!="None")
               {
                  imageLayer->setHistogramStretchEnableFlag(true);
               }
               else
               {
                  imageLayer->setHistogramStretchEnableFlag(false);
               }
            }
         }
         if(theLayer->getHandler().valid())
         {
				ossimString drivePart;
				ossimString pathPart;
				ossimString filePart;
				ossimString extPart;
				ossimFilename overviewFile = imageLayer->getHandler()->createDefaultOverviewFilename();
				ossimFilename histogramFile = imageLayer->getHandler()->createDefaultHistogramFilename();
				
            if(!overviewFile.exists())
            {
					ossimFilename cachedFile = overviewFile;
               
               cachedFile.split(drivePart, pathPart, filePart, extPart);
               ossimFilename stagingCache(ossimPlanetQtApplication::stagingCacheDirectory().toStdString().c_str());
               stagingCache = stagingCache.dirCat(filePart);
               stagingCache += ossimFilename("." + extPart);
               if(stagingCache.exists())
               {
                  theLayer->getHandler()->openOverview(stagingCache);
                  theLayer->dirtyExtents();
               }
            }
				if(!histogramFile.exists())
				{
					ossimFilename cachedFile = histogramFile;
               cachedFile.split(drivePart, pathPart, filePart, extPart);
               ossimFilename stagingCache(ossimPlanetQtApplication::stagingCacheDirectory().toStdString().c_str());
               stagingCache = stagingCache.dirCat(filePart);
               stagingCache += ossimFilename("." + extPart);
               if(stagingCache.exists())
               {
                  theLayer->setHistogramFile(stagingCache);
               }
				}
         }
      }
      if(!theName.empty())
      {
         theLayer->setName(theName);
      }
      if(!theDescription.empty())
      {
         theLayer->setDescription(theDescription);
      }
      if(theSavedLookAt.valid())
      {
         theLayer->setLookAt(theSavedLookAt);
      }
      else
      {
         theLayer->resetLookAt();
      }
   }
}

void ossimPlanetQtOpenImageFileFromLayerActivity::updateGui()
{
   QMutexLocker lock(&theMutex);
   
   if(theInitializeGuiFlag)
   {
      theStatus = "Ready to open " + theFilename;
      setText(0, theStatus.c_str());
      theInitializeGuiFlag = false;
      treeWidget()->scrollToItem(this);
   }
   setText(0, theStatus.c_str());
   
   if(theState == ossimPlanetQtActivityItem::FINISHED)
   {
      if(theItem)
      {
         theLegend->mainWindow()->refreshLandTextures(theLayer->getExtents(), ossimPlanetLandRefreshType_TEXTURE);
         theItem->updateLayerInfo();
         if(theItem->parent())
         {
            if(!theItem->parent()->isExpanded())
            {
               theItem->parent()->setExpanded(true);
            }
         }
      }
   }
}

void ossimPlanetQtOpenImageFileFromLayerActivity::setItem(ossimPlanetQtLegendTextureItem* item)
{
   theItem = item;
}

ossimPlanetQtOpenImageFileFromLayerXmlActivity::ossimPlanetQtOpenImageFileFromLayerXmlActivity(osg::ref_ptr<ossimPlanetOssimImageLayer> layer,
																															  ossimRefPtr<ossimXmlNode> node,
																															  ossimPlanetQtLegend* legend)
    :ossimPlanetQtActivityItem(),
     theLayer(layer),
     theXmlNode(node),
     theLegend(legend)
{
   ossimRefPtr<ossimXmlNode> filenameNode = theXmlNode->findFirstNode("filename");
   ossimRefPtr<ossimXmlNode> name = theXmlNode->findFirstNode("name");
   ossimRefPtr<ossimXmlNode> description = theXmlNode->findFirstNode("desciption");
   if(filenameNode.valid())
   {
      theFilename = filenameNode->getText();
   }
   if(name.valid())
   {
      theName = name->getText();
   }
   else
   {
      theName = theFilename.file();
   }
   if(description.valid()) theDescription = description->getText();
   theInitializeGuiFlag = true;
   theItem = 0;
}

void ossimPlanetQtOpenImageFileFromLayerXmlActivity::execute()
{
   theMutex.lock();
   theState = ossimPlanetQtActivityItem::EXECUTING;
   theErrorMessage = "";
   theStatus = "Opening " + theFilename;
   theMutex.unlock();

   if(theLayer.valid())
   {
      theLayer->loadXml(theXmlNode);
      if(!theLayer->getLookAt().valid())
      {
         theLayer->resetLookAt();
      }
   }
}

void ossimPlanetQtOpenImageFileFromLayerXmlActivity::updateGui()
{
   QMutexLocker lock(&theMutex);
   
   if(theInitializeGuiFlag)
   {
      theStatus = "Ready to open " + theFilename;
      setText(0, theStatus.c_str());
      theInitializeGuiFlag = false;
      treeWidget()->scrollToItem(this);
   }
   
   setText(0, theStatus.c_str());
//    if(theItem)
//    {
//       theItem->setText(0, theStatus.c_str());
//    }
   
   if(theState == ossimPlanetQtActivityItem::FINISHED)
   {
      if(theItem)
      {
         theLayer->setName(theName);
         theLayer->setDescription(theDescription);
         theLegend->mainWindow()->refreshLandTextures(theLayer->getExtents(), ossimPlanetLandRefreshType_TEXTURE);
         theItem->updateLayerInfo();
      }
   }
}

void ossimPlanetQtOpenImageFileFromLayerXmlActivity::setItem(ossimPlanetQtLegendTextureItem* item)
{
   theItem = item;
}

ossimPlanetQtOpenImageFilesActivity::ossimPlanetQtOpenImageFilesActivity(const QList<QUrl>& urls,
                                                                         ossimPlanetQtLegend* legend)
   :ossimPlanetQtActivityItem(),
    theLegend(legend)
{
   QList<QUrl>::const_iterator iter = urls.begin();
   while(iter != urls.end())
   {
      theFileList.push_back(ossimString(iter->toLocalFile().toStdString().c_str())); 
      ++iter;
   }
   theCurrentFileIdx   = 0;
   theGuiFileItemIndex = 0;
}

ossimPlanetQtOpenImageFilesActivity::ossimPlanetQtOpenImageFilesActivity(const QStringList& stringList,
                                                                         ossimPlanetQtLegend* legend)
   :ossimPlanetQtActivityItem(),
    theLegend(legend)
{
   QStringList::const_iterator iter = stringList.begin();
   while(iter != stringList.end())
   {
      theFileList.push_back(iter->toStdString()); 
      ++iter;
   }
   theCurrentFileIdx   = 0;
   theGuiFileItemIndex = 0;
}

void ossimPlanetQtOpenImageFilesActivity::execute()
{
   theMutex.lock();
   theState = ossimPlanetQtActivityItem::EXECUTING;
   theCurrentFileIdx = 0;
   theErrorMessage = "";
   theExtents = 0;
   theLayers.clear();
   theFileStatus.clear();
   theMutex.unlock();
   
   ossim_uint32 fileTotal = theFileList.size();
   while(theCurrentFileIdx < fileTotal)
   {
      theMutex.lock();
      theStatus = (ossimString::toString(theCurrentFileIdx+1) + "/" +
                   ossimString::toString(fileTotal) + "\n" +
                   ossimString("Current file: ") + theFileList[theCurrentFileIdx]);
                   
      theMutex.unlock();
      osg::ref_ptr<ossimPlanetTextureLayer> textureLayer = ossimPlanetTextureLayerRegistry::instance()->createLayer(theFileList[theCurrentFileIdx]);
      
      if(textureLayer.valid()&&!textureLayer->isStateSet(ossimPlanetTextureLayer_NO_GEOM))
      {
         if(!textureLayer->getLookAt().valid())
         {
            textureLayer->resetLookAt();
         }
         ossimPlanetOssimImageLayer* imageLayer = dynamic_cast<ossimPlanetOssimImageLayer*>(textureLayer.get());
         if(imageLayer)
         {
            QString stretchMode = ossimPlanetQtApplication::autoHistogramStretchMode();
            if(!stretchMode.isEmpty())
            {
               imageLayer->setHistogramStretchMode(stretchMode.toStdString());
               if(stretchMode!="None")
               {
                  imageLayer->setHistogramStretchEnableFlag(true);
               }
               else
               {
                  imageLayer->setHistogramStretchEnableFlag(false);
               }
            }
         }
         theMutex.lock();
         theLayers.push_back(textureLayer.get());
         theFileStatus.push_back("Loaded " + theFileList[theCurrentFileIdx]);
         theMutex.unlock();
         
      }
      else if(!textureLayer.valid())
      {
         theMutex.lock();
         theFileStatus.push_back(ossimString("ERROR: Unable to open file ") + theFileList[theCurrentFileIdx]);
         theErrorMessage += theFileStatus[theCurrentFileIdx] + "\n";
         theMutex.unlock();
      }
      else if(textureLayer->isStateSet(ossimPlanetTextureLayer_NO_GEOM))
      {
         theMutex.lock();
         theFileStatus.push_back(ossimString("ERROR: No Geometry for file ") + theFileList[theCurrentFileIdx]);
         theErrorMessage += theFileStatus[theCurrentFileIdx] + "\n";
         theMutex.unlock();
      }
      else
      {
         theMutex.lock();
         theFileStatus.push_back(ossimString("ERROR: Unable to open file ") + theFileList[theCurrentFileIdx]);  
         theErrorMessage += theFileStatus[theCurrentFileIdx] + "\n";
         theMutex.unlock();
      }
      theMutex.lock();
      ++theCurrentFileIdx;
      theMutex.unlock();
   }
}

void ossimPlanetQtOpenImageFilesActivity::updateGui()
{
   QMutexLocker lock(&theMutex);
   
   if(theInitializeGuiFlag)
   {
      theStatus = "Ready to process " + ossimString::toString((ossim_uint32)theFileList.size()) + " files\n";
      
      ossim_uint32 idx = 0;
      ossim_uint32 maxIdx = theFileList.size();
      for(idx = 0; idx < maxIdx; ++idx)
      {
         QTreeWidgetItem* fileItem = new QTreeWidgetItem();
         fileItem->setText(0, theFileList[idx].c_str());
         fileItem->setTextColor(0, QColor(255, 0, 0));
         addChild(fileItem);
      }
      theInitializeGuiFlag = false;
      treeWidget()->scrollToItem(this);
   }
   else
   {
      for(;theGuiFileItemIndex < theCurrentFileIdx; ++theGuiFileItemIndex)
      {
         QTreeWidgetItem* i = child(theGuiFileItemIndex);
         if(i)
         {
            if(ossimString(theFileStatus[theGuiFileItemIndex].begin(),
                           theFileStatus[theGuiFileItemIndex].begin() + 5) != "ERROR")
            {
               i->setText(0, theFileStatus[theGuiFileItemIndex].c_str());
               i->setTextColor(0, QColor(0, 255, 0));
            }
            else
            {
               i->setText(0, theFileStatus[theGuiFileItemIndex].c_str());
            }
         }
      }
   }
   setText(0, theStatus.c_str());
   
   if(theLayers.size())
   {
      ossim_int32 idx = 0;
      for(idx = ((ossim_int32)theLayers.size()-1); idx >=0 ; --idx)
      {
         if(theExtents.valid())
         {
            theExtents->combine(theLayers[idx]->getExtents().get());
         }
         else
         {
            theExtents = theLayers[idx]->getExtents()->clone();
         }
         //theLegend->addTextureLayerToTop(theLayers[idx].get());
      }
      theLayers.clear();
      if(theState == ossimPlanetQtActivityItem::FINISHED)
      {
//          theLegend->setLandTextureRefreshFlag(true);   
//          theLegend->mainWindow()->refreshLandTextures(theExtents.get(), ossimPlanetLandRefreshType_TEXTURE);
         theExtents = 0;
      }
   }
}

ossimPlanetQtMessageActivity::ossimPlanetQtMessageActivity(const std::list<QString>& messageList,
                                                           ossimPlanetQtLegend* legend)
   :theMessageList(messageList),
    theLegend(legend)
{
}

void ossimPlanetQtMessageActivity::execute()
{
   theMutex.lock();
   theState = ossimPlanetQtActivityItem::EXECUTING;
   theMutex.unlock();

   while(!theMessageList.empty())
   {
 //     QString& msg = theMessageList.front();

//       if(msg.startsWith("<tcpipevent", Qt::CaseInsensitive))
//       {
//          msg = QString("<uaimAlert>") + msg + QString("</uaimAlert>");
//          std::istringstream in(msg.toStdString());
         
//          ossimRefPtr<ossimXmlNode> node = new ossimXmlNode;
//          if(node->read(in))
//          {
//             std::vector<ossimRefPtr<ossimXmlNode> > nodeList;
//             node->findChildNodes("tcpipEvent", nodeList);
//             ossim_uint32 listSize = nodeList.size();
//             if(listSize)
//             {
//                ossim_uint32 idx = 0;
//                for(;idx < listSize; ++idx)
//                {
//                   ossimRefPtr<ossimXmlNode> geoCenterNode = nodeList[idx]->findFirstNode("geoCenter");
//                   if(geoCenterNode.valid())
//                   {
//                      ossimString latString;
//                      ossimString lonString;
//                      ossimString altitudeString;
//                      ossimString eventId;
//                      ossimString userId;
//                      nodeList[idx]->getChildTextValue(eventId, "eventID");
//                      nodeList[idx]->getChildTextValue(userId, "userID");
//                      if(geoCenterNode->getChildTextValue(latString, "latitude")&&
//                         geoCenterNode->getChildTextValue(lonString, "longitude")&&
//                         geoCenterNode->getChildTextValue(altitudeString, "altitude"))
//                      {
//                         osg::ref_ptr<ossimPlanetKmlPlacemark> placemark = new ossimPlanetKmlPlacemark;
//                         osg::ref_ptr<ossimPlanetKmlRegion>       region = new ossimPlanetKmlRegion;
//                         osg::ref_ptr<ossimPlanetKmlLatLonAltBox> llBox  = new ossimPlanetKmlLatLonAltBox;
//                         double lat = latString.toDouble();
//                         double lon = lonString.toDouble();
//                         placemark->setName(userId+": "+eventId);
//                         placemark->setMetaData(msg.toStdString().c_str());
//                         region->setBounds(lat, lat, lon, lon);
//                         placemark->setRegion(region.get());
//                         theMutex.lock();
//                         theResultList.push_back(placemark.get());
//                         theMutex.unlock();
//                      }
//                   }
//                }
//             }
//          }
//       }
      // process message
      theMessageList.pop_front();
   }
}

void ossimPlanetQtMessageActivity::updateGui()
{
   theMutex.lock();
   if(!theResultList.empty())
   {
      int count = 0;
      while(!theResultList.empty()&&(count < 5))
      {
         osg::ref_ptr<ossimPlanetKmlObject> result = theResultList.front();
         //theLegend->addLocation(result->clone());
         theLegend->mainWindow()->kmlLayer()->addKml(result.get());
         theResultList.pop_front();
         ++count;
      }
   }
   theMutex.unlock();
   
   if(theState == ossimPlanetQtActivityItem::FINISHED)
   {
   }   
}


