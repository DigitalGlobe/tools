#include <ossimPlanetQt/ossimPlanetQtOperation.h>
#include <ossimPlanetQt/ossimPlanetQtApplication.h>
#include <ossimPlanetQt/ossimPlanetQtEvent.h>
#include <ossimPlanetQt/ossimPlanetQtLegend.h>
#include <ossimPlanetQt/ossimPlanetQtMainWindow.h>
#include <ossimPlanetQt/ossimPlanetQtActivityWidget.h>
#include <QtGui/QApplication>
#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderBase.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
ossimPlanetQtOperation::ossimPlanetQtOperation()
:theWidget(0)
{
}

void ossimPlanetQtOperation::postMessage(const QString& msg,
													  int type)const
{
	if(theWidget)
	{
		QWidget* root = theWidget;
		while(root->parentWidget())
		{
			root = root->parentWidget();
		}
		QApplication::postEvent(root, new ossimPlanetQt::MessageEvent(msg,
																						  (ossimPlanetQt::MessageEvent::MessageType)type));
	}
}

void ossimPlanetQtOperation::adjustToWriteableLocation(ossimFilename& file)const
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

void ossimPlanetQtOperation::adjustToStagerLocation(ossimFilename& file)const
{
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

ossimPlanetQtOssimImageStagerOperation::ossimPlanetQtOssimImageStagerOperation(ossimPlanetOssimImageLayer* layer, 
                                                                               ossimPlanetQtLegend* legend)
:ossimPlanetQtOperation(),
theLayer(layer),
theLegend(legend),
theCurrentProcessInterface(0)
{
   setStatus("Ready to stage image " + theLayer->getHandler()->getFilename());
}

void ossimPlanetQtOssimImageStagerOperation::run()
{
   if(theLayer->getHandler().valid()&&
		(theLayer->isStateSet(ossimPlanetTextureLayer_NO_OVERVIEWS)||
		 theLayer->isStateSet(ossimPlanetTextureLayer_NO_HISTOGRAMS)))
   {
		ossimRefPtr<ossimImageHandler> handler = (ossimImageHandler*)(theLayer->getHandler()->dup());
		
		if(theLayer->isStateSet(ossimPlanetTextureLayer_NO_OVERVIEWS))
		{
         theStagerMessage = "Building overview for file: " + handler->getFilename() + "\n";
			buildOverviews(handler.get());
		}
		if(theLayer->isStateSet(ossimPlanetTextureLayer_NO_HISTOGRAMS)&&
			(!theLayer->getHandler()->getClassName().contains("OgrGdal")))
		{
         theStagerMessage = "Building histogram for file: " + handler->getFilename() + "\n";
			buildHistograms(handler.get());
		}
   }
}

void ossimPlanetQtOssimImageStagerOperation::processProgressEvent(ossimProcessProgressEvent& event)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theStagerMutex);
	if(state() == ossimPlanetOperation::CANCELED_STATE)
	{
		if(theCurrentProcessInterface)
		{
			theCurrentProcessInterface->abort();
		}
	}
   setStatus(theStagerMessage + ossimString::toString((ossim_uint32)(event.getPercentComplete())) +
             "% " + event.getMessage().c_str());
}


void ossimPlanetQtOssimImageStagerOperation::buildOverviews(ossimImageHandler* handler)
{
	ossimRefPtr<ossimOverviewBuilderBase> builder = ossimOverviewBuilderFactoryRegistry::instance()->createBuilder("ossim_tiff_box");
	builder->setInputSource(handler);
	theFilename = handler->getFilename();
	ossimFilename overviewFile = handler->createDefaultOverviewFilename();
	adjustToWriteableLocation(overviewFile);
	builder->addListener((ossimProcessListener*)this);
	theFilename = overviewFile;
	if(overviewFile.path().isWriteable())
	{
		builder->setOutputFile(overviewFile);
		theCurrentProcessInterface = builder.get();
		builder->execute();
	}
	builder->removeListener((ossimProcessListener*)this);	
	if(overviewFile.exists())
	{
		theLayer->setOverviewFile(overviewFile);
	}
	theCurrentProcessInterface = 0;
}

void ossimPlanetQtOssimImageStagerOperation::buildHistograms(ossimImageHandler* handler)
{
	ossimFilename histogramFile = handler->createDefaultHistogramFilename();
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
	theCurrentProcessInterface = writer.get();
	writer->execute();
	writer->removeListener((ossimProcessListener*)this);
	theCurrentProcessInterface = 0;
	if(histogramFile.exists())
	{
		theLayer->setHistogramFile(histogramFile);
		theLayer->setHistogramStretchEnableFlag(true);
	}
}

ossimPlanetQtOpenImageFileOperation::ossimPlanetQtOpenImageFileOperation(const QString& file,
                                                                         ossimPlanetQtLegend* legend)
:ossimPlanetQtOperation(),
theLegend(legend)
{
	theFileList.push_back(ossimString(file.toStdString().c_str())); 
   setStatus("ready to open " + file.toStdString());
}

void ossimPlanetQtOpenImageFileOperation::run()
{
   theCurrentFileIdx = 0;
   theExtents = 0;
   theLayers.clear();
   ossim_uint32 fileTotal = theFileList.size();
   while(theCurrentFileIdx < fileTotal)
   {
      ossimString baseOpeningStatus = ossimString("opening file: ") + theFileList[theCurrentFileIdx];
      setStatus(baseOpeningStatus);
      QApplication::postEvent(theWidget, new ossimPlanetQtActivityWidget::QtOperationEvent(this, 
                                                                                           ossimPlanetQtActivityWidget::STATUS_TYPE));
      // do not gorup them we will open individually
      osg::ref_ptr<ossimPlanetTextureLayer> textureLayer = ossimPlanetTextureLayerRegistry::instance()->createLayer(theFileList[theCurrentFileIdx], false);
      if(textureLayer.valid()&&!textureLayer->isStateSet(ossimPlanetTextureLayer_NO_GEOM))
      {
         if(!textureLayer->getLookAt().valid())
         {
            textureLayer->resetLookAt();
         }
         osg::ref_ptr<ossimPlanetOssimImageLayer> imageLayer = dynamic_cast<ossimPlanetOssimImageLayer*>(textureLayer.get());
         if(imageLayer.valid())
         {
            ossim_uint32 entryIdx = 0;
            ossim_uint32 numberOfEntries = imageLayer->getNumberOfEntries();
            do
            {
               if(!imageLayer->isStateSet(ossimPlanetTextureLayer_NO_GEOM)&&
                  !imageLayer->isStateSet(ossimPlanetTextureLayer_NO_SOURCE_DATA))
               {
                  ossimString baseOpeningStatus = ossimString("opening file: ") + theFileList[theCurrentFileIdx] + "\nentry: " + ossimString::toString(entryIdx);
                  setStatus(baseOpeningStatus);
                  QApplication::postEvent(theWidget, new ossimPlanetQtActivityWidget::QtOperationEvent(this, 
                                                                                                       ossimPlanetQtActivityWidget::STATUS_TYPE));
                  QString stretchMode = ossimPlanetQtApplication::autoHistogramStretchMode();
                  if(imageLayer->getHandler().valid())
                  {
                     ossimFilename histogramFile = imageLayer->getHandler()->createDefaultHistogramFilename();
                     ossimFilename overviewFile  = imageLayer->getHandler()->createDefaultOverviewFilename();
                     adjustToStagerLocation(histogramFile);
                     adjustToStagerLocation(overviewFile);
                     
                     if(imageLayer->isStateSet(ossimPlanetTextureLayer_NO_HISTOGRAMS)&&
                        histogramFile.exists())
                     {
                        imageLayer->setHistogramFile(histogramFile);
                     }
                     if(imageLayer->isStateSet(ossimPlanetTextureLayer_NO_OVERVIEWS)&&
                        overviewFile.exists())
                     {
                        imageLayer->setOverviewFile(overviewFile);
                     }
                  }
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
                  if(theLegend)
                  {
                     QApplication::postEvent(theLegend, new ossimPlanetQt::ImageLayerEvent(imageLayer.get(),
                                                                                           ossimPlanetQt::ImageLayerEvent::ADD));
                  }
               }
               ++entryIdx;
               if(entryIdx < numberOfEntries)
               {
                  imageLayer = new ossimPlanetOssimImageLayer();
                  imageLayer->openImage(theFileList[theCurrentFileIdx], entryIdx);
               }
            }while(entryIdx < numberOfEntries);
            setStatus(ossimString("finished  opening file: ") + theFileList[theCurrentFileIdx]);
            QApplication::postEvent(theWidget, new ossimPlanetQtActivityWidget::QtOperationEvent(this, 
                                                                                                 ossimPlanetQtActivityWidget::STATUS_TYPE));
         }
      }
      else if(!textureLayer.valid())
      {
			postMessage(QString("Unable to open file ") + QString(theFileList[theCurrentFileIdx].c_str()),
							ossimPlanetQt::MessageEvent::ERROR_MESSAGE_TYPE);
      }
      else if(textureLayer->isStateSet(ossimPlanetTextureLayer_NO_GEOM))
      {
			postMessage(QString("No geometry found for file ") + QString(theFileList[theCurrentFileIdx].c_str()),
							ossimPlanetQt::MessageEvent::ERROR_MESSAGE_TYPE);
      }
      else
      {
			postMessage(QString("Unable to open file ") + QString(theFileList[theCurrentFileIdx].c_str()),
							ossimPlanetQt::MessageEvent::ERROR_MESSAGE_TYPE);
      }
      ++theCurrentFileIdx;
   }
}


ossimPlanetQtOpenImageFileFromLayerOperation::ossimPlanetQtOpenImageFileFromLayerOperation(osg::ref_ptr<ossimPlanetOssimImageLayer> layer,
																														 const ossimFilename& name,
																														 ossim_int32 entry)
:ossimPlanetQtOperation(),
theLayer(layer),
theFilename(name),
theEntry(entry)
{
	
}

void ossimPlanetQtOpenImageFileFromLayerOperation::run()
{
   setStatus("Opening " + theFilename);
	
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
				ossimFilename histogramFile = imageLayer->getHandler()->createDefaultHistogramFilename();
				ossimFilename overviewFile  = imageLayer->getHandler()->createDefaultOverviewFilename();
				
            if(!overviewFile.exists())
            {
					adjustToStagerLocation(overviewFile);
               if(overviewFile.exists())
               {
                  theLayer->getHandler()->openOverview(overviewFile);
                  theLayer->dirtyExtents();
               }
            }
				if(!histogramFile.exists())
				{
					adjustToStagerLocation(histogramFile);
               if(histogramFile.exists())
               {
                  theLayer->setHistogramFile(histogramFile);
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

ossimPlanetQtOpenImageFileFromLayerXmlOperation::ossimPlanetQtOpenImageFileFromLayerXmlOperation(osg::ref_ptr<ossimPlanetOssimImageLayer> layer,
                                                                                                 ossimRefPtr<ossimXmlNode> node,
                                                                                                 ossimPlanetQtLegend* legend)
:ossimPlanetQtOperation(),
theLayer(layer),
theXmlNode(node),
theLegend(legend)
{
   ossimRefPtr<ossimXmlNode> filenameNode = theXmlNode->findFirstNode("filename");
   ossimRefPtr<ossimXmlNode> name         = theXmlNode->findFirstNode("name");
   ossimRefPtr<ossimXmlNode> description  = theXmlNode->findFirstNode("desciption");
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

void ossimPlanetQtOpenImageFileFromLayerXmlOperation::run()
{
   setStatus("Opening image " + theFilename);
	
   if(theLayer.valid())
   {
      theLayer->loadXml(theXmlNode);
      if(!theLayer->getLookAt().valid())
      {
         theLayer->resetLookAt();
      }
   }
}
void ossimPlanetQtOpenImageFileFromLayerXmlOperation::setItem(ossimPlanetQtLegendTextureItem* item)
{
	theItem = item;
}

