#include <ossimPlanetQt/ossimPlanetQtMainWindow.h>
#include <ossimPlanetQt/ossimPlanetQtGlWidget.h>
#include <ossimPlanetQt/ossimPlanetQtLegend.h>
#include <ossimPlanetQt/ossimPlanetQtPreferences.h>
#include <ossimPlanetQt/ossimPlanetQtApplication.h>
#include <ossim/ossimVersion.h>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTextBrowser>
#include <QtGui/QWhatsThis>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <ossimPlanet/ossimPlanet.h>
//#include <ossimPlanet/ossimPlanetLand.h>
#include <iostream>
#include <sstream>
#include <ossimPlanet/ossimPlanetCloudLayer.h>
#include <ossimPlanet/ossimPlanetTerrain.h>
#include <ossimPlanet/ossimPlanetYahooGeocoder.h>
#include <ossimPlanet/ossimPlanetAction.h>
#include <ossimPlanet/ossimPlanetOssimImageLayer.h>
#include <ossimPlanet/ossimPlanetTextureLayerRegistry.h>
#include <ossimPlanet/ossimPlanetKmlLayerNode.h>
#include <ossimPlanetQt/ossimPlanetQtRuler.h>
#include <ossimPlanetQt/ossimPlanetQtWmsDialog.h>
#include <ossimPlanetQt/ossimPlanetQtLayerOperation.h>
#include <ossimPlanetQt/ossimPlanetQtActivityWindow.h>
#include <ossimPlanetQt/ossimPlanetQtApplication.h>
#include <ossimPlanetQt/ossimPlanetQtAboutDialog.h>
#include <ossimPlanetQt/ossimPlanetQtVersion.h>
#include <ossimPlanet/ossimPlanetWmsImageLayer.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimXmlDocument.h>
#include <QtCore/QDataStream>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <sstream>
#include <ossimPlanet/ossimPlanetKmlLayerNode.h>
#include <wms/wmsCurlMemoryStream.h>
#include <QtCore/QProcess>
#include <QtCore/QTemporaryFile>
#include <QtOpenGL/QGLFormat>
#include <ossimPlanetQt/ossimPlanetQtImage.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimMemoryImageSource.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossimPlanet/ossimPlanetIoSocketServerChannel.h>
#include <ossimPlanet/ossimPlanetIoSocket.h>
#include <ossimPlanetQt/ossimPlanetQtEvent.h>
#include <ossimPlanetQt/ossimPlanetQtDropDownDelegate.h>
#include <ossimPlanet/ossimPlanetDestinationCommandAction.h>
#include <ossimPlanet/ossimPlanetManipulator.h>
#include <ossimPlanet/ossimPlanetCache.h>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osg/io_utils>
static const int PREFERENCES_CLIENT_WIDGET_NAME_IDX   = 0;
static const int PREFERENCES_CLIENT_WIDGET_IP_IDX   = 1;
static const int PREFERENCES_CLIENT_WIDGET_PORT_IDX = 2;
static const int PREFERENCES_CLIENT_WIDGET_PORT_TYPE_IDX = 3;
static const int PREFERENCES_CLIENT_WIDGET_ENABLE_IDX = 4;
static const int PREFERENCES_CLIENT_WIDGET_SIZE = 5;

static const int PREFERENCES_SERVER_WIDGET_NAME_IDX   = 0;
static const int PREFERENCES_SERVER_WIDGET_IP_IDX   = 1;
static const int PREFERENCES_SERVER_WIDGET_PORT_IDX = 2;
static const int PREFERENCES_SERVER_WIDGET_PORT_TYPE_IDX = 3;
static const int PREFERENCES_SERVER_WIDGET_ENABLE_IDX = 4;
static const int PREFERENCES_SERVER_WIDGET_SIZE = 5;

static const int PREFERENCES_ARCHIVE_WIDGET_SOURCE_IDX		= 0;
static const int PREFERENCES_ARCHIVE_WIDGET_DESTINATION_IDX = 1;
static const int PREFERENCES_ARCHIVE_WIDGET_SIZE			= 2;


template <class charT, class traits = std::char_traits<charT> >
class ossimPlanetQtLogBufferStream : public std::basic_streambuf<charT, traits>
{
public:
   ossimPlanetQtLogBufferStream()
	:theMainWindow(0), 
	 theMessageType(ossimPlanetQt::MessageEvent::ERROR_MESSAGE_TYPE)
	 {}
	
	void setMessageType(ossimPlanetQt::MessageEvent::MessageType type)
	{
		theMessageType = type;
	}
	void setMainWindow(ossimPlanetQtMainWindow* w)
	{
		theMainWindow = w;
	}
protected:
   virtual int overflow(int c)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
		if(!traits::eq_int_type(c, traits::eof()))
		{
			theBuffer += ossimString(c);
		}
		
		return c;
	}
	
   virtual std::streamsize xsputn(const charT * pChar, std::streamsize n)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
		theBuffer += ossimString(pChar, pChar + n);
		return n;
	}
	
   virtual int sync()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
		if(theMainWindow)
		{
			if(theBuffer.size())
			{
				theBuffer = theBuffer.trim();
				QApplication::postEvent(theMainWindow, new ossimPlanetQt::MessageEvent(theBuffer.c_str(),
																											  theMessageType));
			}
		}
		theBuffer = "";
		return 0;
	}
private:
	mutable OpenThreads::ReentrantMutex theMutex;
	ossimPlanetQtMainWindow* theMainWindow;
	ossimPlanetQt::MessageEvent::MessageType theMessageType;
   ossimString theBuffer;
	
};

class ossimPlanetQtLogStream : public std::ostream
{
public:
	ossimPlanetQtLogStream(ossimPlanetQt::MessageEvent::MessageType type = ossimPlanetQt::MessageEvent::INFO_MESSAGE_TYPE) : std::ostream(&theLogBufferStream)
	{
		theLogBufferStream.setMessageType(type);
	}
   virtual ~ossimPlanetQtLogStream()
	{
		theLogBufferStream.pubsync();
	}
	void setMessageType(ossimPlanetQt::MessageEvent::MessageType type)
	{
		theLogBufferStream.setMessageType(type);
	}
	void setMainWindow(ossimPlanetQtMainWindow* w)
	{
		theLogBufferStream.setMainWindow(w);
	}
	
private:
   ossimPlanetQtLogBufferStream<char> theLogBufferStream;
   // Copy & assignment are undefined in iostreams
};

static ossimPlanetQtLogStream theQtErrorLogStream(ossimPlanetQt::MessageEvent::ERROR_MESSAGE_TYPE);
static ossimPlanetQtLogStream theQtWarnLogStream(ossimPlanetQt::MessageEvent::WARNING_MESSAGE_TYPE);
static ossimPlanetQtLogStream theQtDebugLogStream(ossimPlanetQt::MessageEvent::DEBUG_MESSAGE_TYPE);
static ossimPlanetQtLogStream theQtInfoLogStream(ossimPlanetQt::MessageEvent::INFO_MESSAGE_TYPE);

class ossimPlanetQtBrowser
{
public:
   static bool openUrl(const QString& url)
      {
#ifdef __MACOSX__
         QStringList args;
         args.push_back(url);
         
         return QProcess::startDetached("open",
                                        args);
         
         return true;
#elif  defined(__WIN32__) || defined(_WIN32)
         QStringList args;
         args.push_back(url);
		 return QProcess::startDetached(QString( "rundll32 url.dll,FileProtocolHandler \"%1\"").arg( url ));
#else
	ossimFilename file("/usr/bin/firefox");
        if(file.exists())
	{
         QStringList args;
         args.push_back(url);

         return QProcess::startDetached(file.c_str(),
                                        args);
	}
         return false;
#endif
      }
};


class ossimPlanetQtTextureLayerCallback : public ossimPlanetTextureLayerCallback
{
public:
   ossimPlanetQtTextureLayerCallback(ossimPlanetQtMainWindow* window)
      :theMainWindow(window)
      {
      }
   virtual void layerAdded(osg::ref_ptr<ossimPlanetTextureLayer> layer)
   {
		if(theMainWindow)
		{
			QApplication::postEvent(theMainWindow->legend(),
											new ossimPlanetQt::ImageLayerEvent(layer.get(), ossimPlanetQt::ImageLayerEvent::ADDED));
		}
   }
   virtual void layerRemoved(osg::ref_ptr<ossimPlanetTextureLayer> layer,
                             osg::ref_ptr<ossimPlanetTextureLayer> parent)
   {
		if(theMainWindow)
		{
			QApplication::postEvent(theMainWindow->legend(),
											new ossimPlanetQt::ImageLayerEvent(layer.get(), ossimPlanetQt::ImageLayerEvent::REMOVED));
		}
   }
   virtual void refreshExtent(osg::ref_ptr<ossimPlanetExtents> extent)
   {
   }
   virtual void propertyChanged(const ossimString& name,
                                const ossimPlanetTextureLayer* object){}
	

protected:
   ossimPlanetQtMainWindow* theMainWindow;
};

ossimPlanetQtMainWindow::NodeCallback::NodeCallback(ossimPlanetQtMainWindow* mainWindow)
:theMainWindow(mainWindow)
{
}

void ossimPlanetQtMainWindow::NodeCallback::nodeAdded(osg::Node* node)
{
	ossimPlanetNode* n = dynamic_cast<ossimPlanetNode*>(node);
	if(n)
	{
		QApplication::postEvent(theMainWindow->legend(),
										new ossimPlanetQt::NodeCallbackEvent(n, ossimPlanetQt::NodeCallbackEvent::NodeEventType_ADD));
	}
}
void ossimPlanetQtMainWindow::NodeCallback::nodeRemoved(osg::Node* node)
{
	ossimPlanetNode* n = dynamic_cast<ossimPlanetNode*>(node);
	if(n)
	{
		QApplication::postEvent(theMainWindow->legend(),
										new ossimPlanetQt::NodeCallbackEvent(n, ossimPlanetQt::NodeCallbackEvent::NodeEventType_REMOVE));
	}
}



ossimPlanetIoMainWindowMessageHandler::ossimPlanetIoMainWindowMessageHandler(ossimPlanetQtMainWindow* mainWindow)
   :theMainWindow(mainWindow)
{
}

bool ossimPlanetIoMainWindowMessageHandler::handleMessage(osg::ref_ptr<ossimPlanetMessage> message)
{
   ossimString test(message->data().begin(),
                    message->data().begin() + std::min((int)message->dataSize(), (int)30));
   if(test.contains("ossimPlanetQtLegend"))
   {
      ossimRefPtr<ossimXmlNode> node = new ossimXmlNode;
      std::istringstream in(ossimString(message->data().begin(),
                                       message->data().end()));
      if(node->read(in))
      {
         QApplication::postEvent(theMainWindow->legend(),
                                 new ossimPlanetQt::LegendXmlEvent(node.get()));
      }

      return true;
   }
   else if(test.contains("AnimationPath"))
   {
      QApplication::postEvent(theMainWindow->legend(),
                              new ossimPlanetQt::AnimationPathEvent(ossimString(message->data().begin(), message->data().end())));
      return true;
   }
   else if(test.contains("ossimPlanetQtKml"))
   {
      ossimString tempMessage(message->data().begin()+18,
                              message->data().end());
      ossimString filename;
      ossimString kml;

      kml = tempMessage.after(">");
      filename = tempMessage.before(">");

      ossimFilename kmlFile(ossimPlanetQtApplication::userSupportDirectory().toStdString());
      kmlFile = kmlFile.dirCat("kml");
      kmlFile.createDirectory();
      kmlFile = kmlFile.dirCat(filename);
      ofstream out(kmlFile.c_str());
      out.write((char*)(kml.c_str()), kml.size());
      out.close();
      if(kmlFile.exists())
      {
         QApplication::postEvent(theMainWindow->legend(),
                                 new ossimPlanetQt::KmlFileEvent(kmlFile));
      }
   }       
   else if(test.contains("ossimPlanetQtWms"))
   {
      std::vector<ossimPlanetQt::WmsSettingsEvent::WmsSettingsProperties> theSettingsList;
      
      ossimRefPtr<ossimXmlNode> node = new ossimXmlNode;
      std::istringstream in(ossimString(message->data().begin(),
                                       message->data().end()));
      if(node->read(in))
      {
         const vector<ossimRefPtr<ossimXmlNode> >&      childNodes = node->getChildNodes();
         ossim_uint32 idx = 0;
         for(idx = 0; idx < childNodes.size();++idx)
         {
            ossimString name;
            ossimString proxyHost;
            ossimString proxyPort;
            ossimString proxyUser;
            ossimString proxyPassword;
            ossimString url;
            ossimString cache;
            if(childNodes[idx]->getTag() == "Connection")
            {
               childNodes[idx]->getChildTextValue(name, "name");
               childNodes[idx]->getChildTextValue(proxyHost, "proxyHost");
               childNodes[idx]->getChildTextValue(proxyPort, "proxyPort");
               childNodes[idx]->getChildTextValue(proxyUser, "proxUser");
               childNodes[idx]->getChildTextValue(proxyPassword, "proxyPassword");
               childNodes[idx]->getChildTextValue(url, "url");
               theSettingsList.push_back(ossimPlanetQt::WmsSettingsEvent::WmsSettingsProperties(name,
                                                                                                url,
                                                                                                cache,
                                                                                                proxyHost,
                                                                                                proxyPort,
                                                                                                proxyUser,
                                                                                                proxyPassword));
                                                                                                
               // for now we will not carry along cache
//                childNodes[idx]->getChildTextValue(name, "cache");
            }
         }
         if(theSettingsList.size())
         {
            QApplication::postEvent(theMainWindow,
                                    new ossimPlanetQt::WmsSettingsEvent(theSettingsList));
         }
      }
      return true;
   }
//    else
//    {
//       std::cout << "Unhandled message = " << ossimString(message->data().begin(),
//                                                          message->data().end()) << std::endl;
//    }
   return false;
}

static int terrainTypeToInt(const std::string& type)
{
   if(type == "low")
   {
      return ossimPlanetTerrain::LOW_TEXTURE_DENSITY; // both elevation and texture are the same integer values
   }
   else if(type == "medium-low")
   {
      return ossimPlanetTerrain::MEDIUM_LOW_TEXTURE_DENSITY; // both elevation and texture are the same integer values
   }
   else if(type == "medium")
   {
      return ossimPlanetTerrain::MEDIUM_TEXTURE_DENSITY; // both elevation and texture are the same integer values
   }
   else if(type == "medium-high")
   {
      return ossimPlanetTerrain::MEDIUM_HIGH_TEXTURE_DENSITY; // both elevation and texture are the same integer values
   }
   else if(type == "high")
   {
      return ossimPlanetTerrain::HIGH_TEXTURE_DENSITY; // both elevation and texture are the same integer values
   }
   else if(type == "no-cull")
   {
      return ossimPlanetTerrain::NO_CULL; 
   }
   else if(type == "low-cull")
   {
      return ossimPlanetTerrain::LOW_CULL; 
   }
   else if(type == "medium-low-cull")
   {
      return ossimPlanetTerrain::MEDIUM_LOW_CULL; 
   }
   else if(type == "medium-cull")
   {
      return ossimPlanetTerrain::MEDIUM_CULL; 
   }
   else if(type == "medium-high-cull")
   {
      return ossimPlanetTerrain::MEDIUM_HIGH_CULL; 
   }
   else if(type == "high-cull")
   {
      return ossimPlanetTerrain::HIGH_CULL; 
   }
   return 0;
}

ossimPlanetQtMainWindow::ossimPlanetQtMainWindow(QWidget *parent)
   : QMainWindow(parent),
     theDefaultNavigationPortType("tcp"),
     theDefaultDataPortType("tcp")
{
   setupUi(this);
   theIoThread = 0;
   theLayerOperation = 0;
   theWmsWindow = 0;
   theRuler = 0;
   theActivityWindow = 0;
   theLargestLogMessageSize = 0;
   theInitializedFlag = false;
	theMessageLogDialog = new ossimPlanetQtMessageLog(this);
	theMessageLogDialog->setWindowFlags(theMessageLogDialog->windowFlags()|Qt::WindowStaysOnTopHint);

	theQtErrorLogStream.setMainWindow(this);
	theQtWarnLogStream.setMainWindow(this);
//	theQtDebugLogStream.setMainWindow(this);
//	theQtInfoLogStream.setMainWindow(this);

   thePlanet = new ossimPlanet;
   theViewerCallback = new ViewerCallback(this);
#if 1
	ossimSetNotifyStream(&theQtErrorLogStream, ossimNotifyFlags_FATAL);
	ossimSetNotifyStream(&theQtWarnLogStream, ossimNotifyFlags_WARN);
//	ossimSetNotifyStream(&theQtInfoLogStream, ossimNotifyFlags_NOTICE);
//	ossimSetNotifyStream(&theQtInfoLogStream, ossimNotifyFlags_INFO);
//	ossimSetNotifyStream(&theQtDebugLogStream, ossimNotifyFlags_DEBUG);
#endif
}

ossimPlanetQtMainWindow::~ossimPlanetQtMainWindow()
{
   theGlWidget->viewer()->removeCallback(theViewerCallback.get());
	theAnnotationLayer->removeCallback(theNodeCallback.get());
   theKmlLayer->removeCallback(theNodeCallback.get());
   theVideoLayer->removeCallback(theNodeCallback.get()); 
	theSousaLayer->removeCallback(theNodeCallback.get());
	
	thePlanet = 0;
	theGlWidget->viewer()->setSceneData(thePlanet.get());
   if(theIoThread.valid())
   {
      theIoThread->removeMessageHandler(theIoMessageHandler.get());
      if(theIoThread->isRunning())
      {
         theIoThread->cancel();
         while(theIoThread->isRunning());
      }
      theIoThread=0;
   }
   
   //theGlWidget->cancelPager();
	if(theActivityWindow)
	{
		theActivityWindow->close();
		
	}
}

osg::ref_ptr<ossimPlanet> ossimPlanetQtMainWindow::planet()
{
   return thePlanet;
}


const osg::ref_ptr<ossimPlanet> ossimPlanetQtMainWindow::planet()const
{
   return thePlanet;
}


const osg::ref_ptr<ossimPlanetKmlLayer> ossimPlanetQtMainWindow::kmlLayer()const
{
   return theKmlLayer.get();
}

osg::ref_ptr<ossimPlanetKmlLayer> ossimPlanetQtMainWindow::kmlLayer()
{
   return theKmlLayer.get();
}

const osg::ref_ptr<ossimPlanetVideoLayer> ossimPlanetQtMainWindow::videoLayer()const
{
   return theVideoLayer;
}

osg::ref_ptr<ossimPlanetAnnotationLayer> ossimPlanetQtMainWindow::annotationLayer()
{
   return theAnnotationLayer;
}

osg::ref_ptr<ossimPlanetVideoLayer> ossimPlanetQtMainWindow::videoLayer()
{
   return theVideoLayer;
}

ossim_uint32 ossimPlanetQtMainWindow::textureLayerGroupListSize()const
{
   return theTextureLayers.size();
}

osg::ref_ptr<ossimPlanetTextureLayerGroup> ossimPlanetQtMainWindow::textureLayerGroup(ossim_uint32 idx)
{
   if(idx < theTextureLayers.size())
   {
      return theTextureLayers[idx];
   }

   return 0;
}

const osg::ref_ptr<ossimPlanetTextureLayerGroup> ossimPlanetQtMainWindow::textureLayerGroup(ossim_uint32 idx)const
{
   if(idx < theTextureLayers.size())
   {
      return theTextureLayers[idx];
   }

   return 0;
}

ossimPlanetQtLegend* ossimPlanetQtMainWindow::legend()
{
   return thePlanetLegend;
}
const ossimPlanetQtLegend* ossimPlanetQtMainWindow::legend()const
{
   return thePlanetLegend;
}

ossimPlanetQtViewer* ossimPlanetQtMainWindow::glWidget()
{
   return theGlWidget;
}
ossimPlanetManipulator* ossimPlanetQtMainWindow::manipulator()
{
   return theManipulator.get();
}

ossimPlanetQtActivityWindow* ossimPlanetQtMainWindow::activityWindow()
{
   return theActivityWindow;
}

const ossimPlanetQtActivityWindow* ossimPlanetQtMainWindow::activityWindow()const
{
   return theActivityWindow;
}

void ossimPlanetQtMainWindow::saveSession(const QString& filename)
{
   ossimRefPtr<ossimXmlNode> manipulatorNode = new ossimXmlNode;
   ossimRefPtr<ossimXmlNode> viewNode        = new ossimXmlNode;
   ossimRefPtr<ossimXmlNode> orientationNode = new ossimXmlNode;
   ossimRefPtr<ossimXmlNode> posNode         = new ossimXmlNode;   
   ossimRefPtr<ossimXmlNode> rootNode = new ossimXmlNode;
   ossimRefPtr<ossimXmlDocument> document = new ossimXmlDocument;
   rootNode->setTag("ossimPlanetQt");
//    rootNode->setTag("object");
//    rootNode->addAttribute("type", "ossimPlanetQt");
   ossimRefPtr<ossimXmlNode> legendNode = thePlanetLegend->saveXml();
   rootNode->addChildNode(legendNode.get());
   osg::ref_ptr<ossimPlanetLookAt> currentCamera = theGlWidget->viewer()->currentCamera()->clone();
   manipulatorNode->setTag("ossimPlanetManipulator");
   manipulatorNode->addChildNode(viewNode.get());
   viewNode->setTag("View");
   viewNode->addChildNode("latitude", ossimString::toString(currentCamera->lat()));
   viewNode->addChildNode("longitude", ossimString::toString(currentCamera->lon()));
   viewNode->addChildNode("altitude", ossimString::toString(currentCamera->altitude()));

   viewNode->addChildNode("heading", ossimString::toString(currentCamera->heading()));
   viewNode->addChildNode("pitch", ossimString::toString(currentCamera->pitch()));
   viewNode->addChildNode("roll", ossimString::toString(currentCamera->roll()));
   
   rootNode->addChildNode(manipulatorNode);

   ofstream out(theSessionFilename.c_str());
   document->initRoot(rootNode.get());
   
   out << *document << std::endl;

   setWindowTitle(QString("ossimPlanet Session: ") + filename);
   
}

void ossimPlanetQtMainWindow::loadSession(const QString& filename)
{  
   ossimRefPtr<ossimXmlDocument> document = new ossimXmlDocument;
	thePlanetLegend->clear();

	if(theActivityWindow)
	{
		theActivityWindow->theActivityWidget->deleteAllActivities();
	}
   if(document->openFile(ossimFilename(filename.toStdString())))
   {
      theSessionFilename = ossimFilename(filename.toStdString());
      ossimRefPtr<ossimXmlNode> rootNode = document->getRoot();
      ossimString attributeValue;
      if(rootNode->getTag() == "ossimPlanetQt")
      {
//         std::vector<ossimRefPtr<ossimXmlNode> >&  childNodes = rootNode->getChildNodes();
         
         ossimRefPtr<ossimXmlNode> legend = rootNode->findFirstNode("ossimPlanetQtLegend");
         ossimRefPtr<ossimXmlNode> manipulator = rootNode->findFirstNode("ossimPlanetManipulator");
         if(legend.valid())
         {
            theTextureLayers[0]->removeLayers(0, theTextureLayers[0]->numberOfLayers());
            theTextureLayers[1]->removeLayers(0, theTextureLayers[1]->numberOfLayers());
            thePlanetLegend->populateLegend(legend.get());
         }
         if(manipulator.valid())
         {
            ossimRefPtr<ossimXmlNode> pos;
            ossimRefPtr<ossimXmlNode> orient;
            
            const vector<ossimRefPtr<ossimXmlNode> >& childNodes = manipulator->getChildNodes();
            
            if(childNodes.size() == 1)
            {
               ossimRefPtr<ossimXmlNode> lat     = manipulator->findFirstNode("View/latitude");
               ossimRefPtr<ossimXmlNode> lon     = manipulator->findFirstNode("View/longitude");
               ossimRefPtr<ossimXmlNode> hgt     = manipulator->findFirstNode("View/altitude");
               ossimRefPtr<ossimXmlNode> heading = manipulator->findFirstNode("View/heading");
               ossimRefPtr<ossimXmlNode> pitch   = manipulator->findFirstNode("View/pitch");
               ossimRefPtr<ossimXmlNode> roll    = manipulator->findFirstNode("View/roll");
               
               if(lat.valid(),
                  lon.valid(),
                  hgt.valid(),
                  heading.valid(),
                  pitch.valid(),
                  roll.valid())
               {
                  ossimString actionString = (ossimString(":navigator gotolatlonelevhpr ") +
                                              lat->getText() + " " +
                                              lon->getText() + " " +
                                              hgt->getText() + " " +
                                              heading->getText() + " " +
                                              pitch->getText() + " " +
                                              roll->getText() + " ");
                  ossimPlanetDestinationCommandAction(actionString).execute();
                  
               }
            }
         }
      }
      setWindowTitle(QString("ossimPlanet Session: ") + theSessionFilename);
   }
   else
   {
      // error
   }
}

void ossimPlanetQtMainWindow::dropEvent ( QDropEvent * event )
{
   thePlanetLegend->dropEvent(event);
}
 
void ossimPlanetQtMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
   thePlanetLegend->dragEnterEvent(event);
}


void ossimPlanetQtMainWindow::sendMessage(const std::string& id,
                                          const std::string& message,
                                          const ossimString& messageType)
{
   ossim_uint32 idx = 0;
   std::vector<char> tempMessage(message.begin(), message.end());
   for(idx = 0; idx < theClientList.size(); ++idx)
   {
      if(theClientList[idx]->thePortType == messageType)
      {
         theClientList[idx]->theIo->pushMessage(new ossimPlanetMessage(id, tempMessage), messageType=="Data");
      }
   }
}

void ossimPlanetQtMainWindow::addOperation(osg::ref_ptr<ossimPlanetOperation> operation)
{
	if(theActivityWindow)
	{
		theActivityWindow->theActivityWidget->addOperation(operation.get());
	}
}

bool ossimPlanetQtMainWindow::event(QEvent* e)
{
   switch(e->type())
   {
		case ossimPlanetQt::MESSAGE_EVENT_TYPE:
		{
         ossimPlanetQt::MessageEvent* evt = dynamic_cast<ossimPlanetQt::MessageEvent*>(e);
			if(evt)
			{
				// make sure the dialog exists and it's raised to top
				//
				switch(evt->messageType())
				{
					case ossimPlanetQt::MessageEvent::ERROR_MESSAGE_TYPE:
					{
//						QTextCharFormat format = theMessageLogDialog->theMessageLog->currentCharFormat();
//						format.setForeground(QBrush(QColor(255, 0, 0)));
//						theMessageLogDialog->theMessageLog->setCurrentCharFormat(format);
//						theMessageLogDialog->theMessageLog->append(QString("ERROR: ") + QString(evt->message()));
						theMessageLogDialog->addMessage(evt->message(),
																  ossimPlanetQtMessageLog::LOG_ERROR);
						theMessageLogDialog->raise();
						theMessageLogDialog->show();
						theMessageLogDialog->showLastMessage();
						break;
					}
					case ossimPlanetQt::MessageEvent::WARNING_MESSAGE_TYPE:
					{
						theMessageLogDialog->addMessage(QString("WARNING: ") + QString(evt->message()),
																  ossimPlanetQtMessageLog::LOG_WARNING);
						theMessageLogDialog->showLastMessage();
						break;
					}
					case ossimPlanetQt::MessageEvent::INFO_MESSAGE_TYPE:
					{
						theMessageLogDialog->addMessage(QString(evt->message()),
																  ossimPlanetQtMessageLog::LOG_INFO);

						theMessageLogDialog->showLastMessage();
						break;
					}
					case ossimPlanetQt::MessageEvent::DEBUG_MESSAGE_TYPE:
					{
						theMessageLogDialog->addMessage(QString(evt->message()),
						                                               ossimPlanetQtMessageLog::LOG_DEBUG);
						break;
					}
				}
			}
         if(evt->message().length() > theLargestLogMessageSize)
         {
            theLargestLogMessageSize = evt->message().length();
            theMessageLogDialog->theMessageLog->resizeColumnToContents(0);
         }
			return true;
		}
      case ossimPlanetQt::WMS_SETTINGS_EVENT_TYPE:
      {
         ossimPlanetQt::WmsSettingsEvent* evt = dynamic_cast<ossimPlanetQt::WmsSettingsEvent*>(e);
         if(evt)
         {
            QStringList keys = ossimPlanetQtApplication::wmsSettingsSubkeyList();//subkeyList(settings, basePath);
            QString   serversAdded;
            QString   serversUpdated;
            const std::vector<ossimPlanetQt::WmsSettingsEvent::WmsSettingsProperties>& propertyList = evt->settingsPropertyList();
            ossim_uint32 idx = 0;
            for(idx = 0; idx < propertyList.size();++idx)
            {
               if(!keys.contains(propertyList[idx].theName.c_str()))
               {
                  if(serversAdded == "")
                  {
                     serversAdded += propertyList[idx].theName.c_str();
                  }
                  else
                  {
                     serversAdded = serversAdded + ", " +  propertyList[idx].theName.c_str();
                  }
               }
               else
               {
                  if(serversUpdated == "")
                  {
                     serversUpdated += propertyList[idx].theName.c_str();
                  }
                  else
                  {
                     serversUpdated = serversUpdated + ", " +  propertyList[idx].theName.c_str();
                  }
               }
               QString path = propertyList[idx].theName.c_str();
               ossimPlanetQtApplication::writeWmsSettingsEntry(path + "/proxyhost", propertyList[idx].theProxyHost.c_str());
               ossimPlanetQtApplication::writeWmsSettingsEntry(path + "/proxyport", propertyList[idx].theProxyPort.c_str());
               ossimPlanetQtApplication::writeWmsSettingsEntry(path + "/proxyuser", propertyList[idx].theProxyUser.c_str());
               ossimPlanetQtApplication::writeWmsSettingsEntry(path + "/proxypassword", propertyList[idx].theProxyPassword.c_str());
               ossimPlanetQtApplication::writeWmsSettingsEntry(path + "/url", propertyList[idx].theUrl.c_str());
            }
            QString message;
            if(serversAdded!="")
            {
               message = "Servers Added: " + serversAdded;
               if(theWmsWindow)
               {
                  theWmsWindow->populateConnectionList();
               }
            }
            if(serversUpdated != "")
            {
               if(message!="")
               {
                  message +="\n";
               }
               message += "Servers Updated: " + serversUpdated;
            }
            
            if(message != "")
            {
               QMessageBox::information(this, tr("WMS proxies"),
                                        message);
            }
         }
         return true;
      }
      case QEvent::Show:
      {
   
         if(!theInitializedFlag)
         {
            init();
         }
         break;
      }
      case QEvent::Close:
      {
         if(theLayerOperation)
         {
            delete theLayerOperation;
            theLayerOperation = 0;
         }
         if(theWmsWindow)
         {
            delete theWmsWindow;
            theWmsWindow = 0;
         }
         if(theRuler)
         {
            delete theRuler;
            theRuler = 0;
         }
         if(theActivityWindow)
         {
            delete theActivityWindow;
            theActivityWindow = 0;
         }
         if(theMessageLogDialog)
         {
            delete theMessageLogDialog;
            theMessageLogDialog = 0;
         }
         break;
      }
      default:
      {
          break;
      } 
   }
   return QMainWindow::event(e);
}

void ossimPlanetQtMainWindow::on_viewBookmark_triggered( bool /*checked*/ )
{
   osg::ref_ptr<ossimPlanetLookAt> lookAt = theGlWidget->viewer()->currentLookAt()->clone();
   
   legend()->addBookmark(lookAt, "", "");// use default name and description
}

void ossimPlanetQtMainWindow::on_viewToggleFullScreen_triggered(bool )
{
   if(theIsFullScreen)
   {
      theGlWidget->setParent(tabWidget);
      tabWidget->widget(0)->layout()->addWidget(theGlWidget);
      if(theFullScreenFrame)
      {
         delete theFullScreenFrame;
      }
      theFullScreenFrame = 0;
      releaseKeyboard();
      theGlWidget->setFocus();
      theIsFullScreen = false;
      showNormal();
   }
   else
   {
       theFullScreenFrame = new QFrame();
       theFullScreenFrame->setFrameShadow(QFrame::Plain);
       theFullScreenFrame->setFrameShape(QFrame::NoFrame);
      tabWidget->widget(0)->layout()->removeWidget(theGlWidget);
      theGlWidget->setParent(theFullScreenFrame);
       theFullScreenFrame->showFullScreen();
      theGlWidget->showFullScreen();
       theGlWidget->setGeometry(0,
                                0,
                                theFullScreenGeometry.width(),
                                theFullScreenGeometry.height());
      grabKeyboard();
      theGlWidget->setFocus();
      theIsFullScreen = true;
   }
}

// void ossimPlanetQtMainWindow::on_viewSaveAsImage_triggered(bool checked)
// {
//    QImage image = theGlWidget->grabFrameBuffer();

//    image.save("/var/tmp/myimage.jpg", 0, 100);
// }
QString ossimPlanetQtMainWindow::getSaveFile(const QString& title, const QString& extension, const QString& filters, const QString& defaultDir)
{
   QFileDialog fileDialog(this, title, defaultDir);
   if(extension.size()>0)
   {
      fileDialog.setDefaultSuffix(extension);
   }
   fileDialog.setConfirmOverwrite(true);
   fileDialog.setAcceptMode(QFileDialog::AcceptSave);
   if(filters.size() > 0)
   {
      fileDialog.setFilter(filters);
   }
   if(extension.size() > 0)
   {
      fileDialog.setDefaultSuffix(extension);
   }
   fileDialog.setReadOnly(true);
   fileDialog.exec();
   QStringList files = fileDialog.selectedFiles();
   if(!files.empty())
   {
      return files[0];
   }
   return "";
}

void ossimPlanetQtMainWindow::on_fileSaveViewAsJpeg_triggered(bool )
{
   ossimFilename fileDir(ossimPlanetQtApplication::userDirectory().toStdString());
   if(!theLastViewSaveDirectory.empty())
   {
      fileDir = theLastViewSaveDirectory;
   }
   
   QString filename = getSaveFile("Save View As JPEG",
                                  "jpg",
                                  "*.jpg",
                                  fileDir.c_str());
   
   if(filename != "")
   {
      ossimRefPtr<ossimImageFileWriter> writer = ossimImageWriterFactoryRegistry::instance()->createWriter(ossimString("image/jpeg"));

      if(writer.valid())
      {
         ossimFilename file(filename.toStdString());
         theLastViewSaveDirectory = file.path();
         file = file.setExtension("jpg");
         ossimPlanetQtImage img(theGlWidget->grabFrameBuffer());
         ossimRefPtr<ossimImageData> data = img.getOssimImage();
         ossimRefPtr<ossimMemoryImageSource> memorySource = new ossimMemoryImageSource();
         memorySource->setImage(data);
         memorySource->initialize();
         ((ossimPropertyInterface*)writer.get())->setProperty("quality", "100");
         ((ossimPropertyInterface*)writer.get())->setProperty(ossimKeywordNames::CREATE_EXTERNAL_GEOMETRY_KW, "false");
         ((ossimPropertyInterface*)writer.get())->setProperty(ossimKeywordNames::CREATE_OVERVIEW_KW, "false");
         
         writer->setFilename(ossimFilename(file));
         writer->connectMyInputTo(0, memorySource.get());
         writer->execute();
      }
      else
      {
         QMessageBox::warning(this, tr("Save JPEG "),
                              tr("Unable to create a JPEG writer "));
         
      }
   }
}

void ossimPlanetQtMainWindow::on_fileSaveViewAsPng_triggered(bool )
{
   ossimFilename fileDir(ossimPlanetQtApplication::userDirectory().toStdString());
   if(!theLastViewSaveDirectory.empty())
   {
      fileDir = theLastViewSaveDirectory;
   }

   QString filename = getSaveFile("Save View As PNG",
                                  "png",
                                  "*.png",
                                  fileDir.c_str());
   
   if(filename != "")
   {
      ossimRefPtr<ossimImageFileWriter> writer = ossimImageWriterFactoryRegistry::instance()->createWriter(ossimString("image/png"));

      if(writer.valid())
      {
         ossimFilename file(filename.toStdString());
         theLastViewSaveDirectory = file.path();
         file = file.setExtension("png");
         ossimPlanetQtImage img(theGlWidget->grabFrameBuffer());
         ossimRefPtr<ossimImageData> data = img.getOssimImage();
         ossimRefPtr<ossimMemoryImageSource> memorySource = new ossimMemoryImageSource();
         memorySource->setImage(data);
         memorySource->initialize();
         ((ossimPropertyInterface*)writer.get())->setProperty("quality", "100");
         ((ossimPropertyInterface*)writer.get())->setProperty(ossimKeywordNames::CREATE_EXTERNAL_GEOMETRY_KW, "false");
         ((ossimPropertyInterface*)writer.get())->setProperty(ossimKeywordNames::CREATE_OVERVIEW_KW, "false");
         
         writer->setFilename(ossimFilename(file));
         writer->connectMyInputTo(0, memorySource.get());
         writer->execute();
      }
      else
      {
         QMessageBox::warning(this, tr("Save PNG "),
                              tr("Unable to create a PNG writer "));
         
      }
   }
}

void ossimPlanetQtMainWindow::on_fileSaveViewAsTiff_triggered(bool )
{
   ossimFilename fileDir(ossimPlanetQtApplication::userDirectory().toStdString());
   if(!theLastViewSaveDirectory.empty())
   {
      fileDir = theLastViewSaveDirectory;
   }

   QString filename = getSaveFile("Save View As TIFF",
                                  "tif",
                                  "*.tif",
                                  fileDir.c_str());
   
   if(filename != "")
   {
      ossimRefPtr<ossimImageFileWriter> writer = ossimImageWriterFactoryRegistry::instance()->createWriter(ossimString("image/tiff"));

      if(writer.valid())
      {
         ossimFilename file(filename.toStdString());
         theLastViewSaveDirectory = file.path();
         file = file.setExtension("tif");
         ossimPlanetQtImage img(theGlWidget->grabFrameBuffer());
         ossimRefPtr<ossimImageData> data = img.getOssimImage();
         ossimRefPtr<ossimMemoryImageSource> memorySource = new ossimMemoryImageSource();
         memorySource->setImage(data);
         memorySource->initialize();
         ((ossimPropertyInterface*)writer.get())->setProperty(ossimKeywordNames::CREATE_EXTERNAL_GEOMETRY_KW, "false");
         ((ossimPropertyInterface*)writer.get())->setProperty(ossimKeywordNames::CREATE_OVERVIEW_KW, "false");
         ((ossimPropertyInterface*)writer.get())->setProperty(ossimKeywordNames::CREATE_OVERVIEW_KW, "false");
         
         writer->setFilename(ossimFilename(file));
         writer->connectMyInputTo(0, memorySource.get());
         writer->execute();
      }
      else
      {
         QMessageBox::warning(this, tr("Save TIFF "),
                              tr("Unable to create a TIFF writer "));
         
      }
   }
}


void ossimPlanetQtMainWindow::on_fileSaveAnimationPath_triggered(bool)
{
}

void ossimPlanetQtMainWindow::on_fileLoadAnimationPath_triggered(bool)
{
}

void ossimPlanetQtMainWindow::on_viewStartRecordingAnimationPath_triggered(bool)
{
   theManipulator->startRecording();
}

void ossimPlanetQtMainWindow::on_viewStopRecordingAnimationPath_triggered(bool)
{
   theManipulator->stopRecording();
   osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath;
   ostringstream out;
   theManipulator->saveRecording(out);
   istringstream in(out.str());
   animationPath->read(in);
   thePlanetLegend->addAnimationPath(animationPath);
}

void ossimPlanetQtMainWindow::on_viewPlayAnimationPath_triggered(bool)
{
   theManipulator->playRecording();
}


void ossimPlanetQtMainWindow::on_viewGotoLatLon_triggered( bool )
{
   bool ok;
   QString text = QInputDialog::getText(this,
      QString("Goto Lat Lon"),
      QString("Enter Lat Lon and optional height separated by spaces:"),
      QLineEdit::Normal,
      QString::null, &ok);
   if ( ok && !text.isEmpty() )
   {
      std::istringstream in(text.toStdString());
      ossimString lat, lon, height;
      in >> lat >> lon >> height;
      lat = lat.trim();
      lon = lon.trim();
      height = height.trim();
      if(height.empty())
      {
         ossimPlanetDestinationCommandAction(":navigator gotolatlonnadir " + lat + " " + lon).execute(); 
      }
      else
      {
         ossimPlanetDestinationCommandAction(":navigator gotolatlonelevnadir " + lat + " " + lon + " " + height).execute(); 
      }
   }   
}

void ossimPlanetQtMainWindow::on_viewGotoAddress_triggered( bool )
{
   bool ok;
   QString text = QInputDialog::getText(this, tr("GeoEncoder Dialog"),
                                        tr("Enter Address:"), QLineEdit::Normal,
                                        QDir::home().dirName(), &ok);
   if (ok && !text.isEmpty())
   {
      std::vector<osg::ref_ptr<ossimPlanetGoecoder::ossimPlanetGeocoderLocation> > result;
      osg::ref_ptr<ossimPlanetYahooGeocoder> geocoder = new ossimPlanetYahooGeocoder;
      
      geocoder->getLocationFromAddress(result,
                                       ossimString(text.toStdString()));
      if(result.size())
      {
         osg::ref_ptr<ossimPlanetLookAt> look = new ossimPlanetLookAt;
         ossimGpt gpt = result[0]->getLocation();
         look->setLat(gpt.latd());
         look->setLon(gpt.lond());
         look->setPitch(45);
         look->setRange(20000);
         ossimPlanetDestinationCommandAction(look->toNavigationString()).execute();
      }
   }
}

void ossimPlanetQtMainWindow::on_fileQuit_triggered(bool )
{
   if(theLayerOperation)
   {
      delete theLayerOperation;
      theLayerOperation = 0;
   }
   if(theWmsWindow)
   {
      delete theWmsWindow;
      theWmsWindow = 0;
   }
   if(theActivityWindow)
   {
      delete theActivityWindow;
      theActivityWindow = 0;
   }
   if(theMessageLogDialog)
   {
      delete theMessageLogDialog;
      theMessageLogDialog = 0;
   }
   if(theRuler)
   {
      delete theRuler;
      theRuler = 0;
   }
   close();
}

void ossimPlanetQtMainWindow::on_fileOpenImage_triggered( bool )
{
   ossimImageHandlerFactoryBase::UniqueStringList stringList;
   
   ossimImageHandlerRegistry::instance()->getSupportedExtensions(stringList);

   ossim_uint32 idx = 0;
   QString extString = "Images(";
   for(idx = 0; idx < stringList.size(); ++idx)
   {
      extString += ("*."+stringList[idx]).c_str();
      if((idx+1) != stringList.size())
      {
         extString += " ";
      }
   }
   extString += ");;Vectors(*shp );;All Files(*)";

   
   QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                         "open file dialog",
                                                         ossimPlanetQtApplication::currentOpenImageDirectory(),
                                                         extString);
   if(filenames.size() > 0)
   {
      ossimFilename lastFile;
      ossim_uint32 fileIdx = 0;
      for(fileIdx = 0; fileIdx < (ossim_uint32)filenames.size();++fileIdx)
      {
         ossimPlanetQtOpenImageFileOperation* activity = new ossimPlanetQtOpenImageFileOperation(filenames[fileIdx],
                                                                                                  thePlanetLegend);
         theActivityWindow->theActivityWidget->addOperation(activity);
         lastFile = ossimFilename(filenames[fileIdx].toStdString()).path();
      }
      if(!lastFile.empty())
      {
         ossimPlanetQtApplication::setCurrentOpenImageDirectory(lastFile.c_str());
      }
   }
}

void ossimPlanetQtMainWindow::on_fileOpenVideo_triggered( bool /*checked*/)
{
	QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                         "open file dialog",
                                                         ossimPlanetQtApplication::currentOpenVideoDirectory(),
                                                         "Videos(*.mpg *.mpeg *.mp4 *.m2p *.mpv *.m2v)");
   if(filenames.size() > 0)
   {
      ossim_uint32 fileIdx = 0;
      ossimFilename lastFile;
      for(fileIdx = 0; fileIdx < (ossim_uint32)filenames.size();++fileIdx)
      {
         ossimFilename file(filenames[fileIdx].toStdString());
         lastFile = file.path(); 
			videoLayer()->add(file);
      }
      ossimPlanetQtApplication::setCurrentOpenVideoDirectory(lastFile.c_str());
   }
}

void ossimPlanetQtMainWindow::on_filePreferences_triggered(bool )
{
   if(thePreferences)
   {
      thePreferences->show();
      thePreferences->raise();
   }
   else
   {
      thePreferences = new ossimPlanetQtPreferences(0);
      thePreferences->setAttribute(Qt::WA_DeleteOnClose);      
      thePreferences->show();
      thePreferences->theAutoHighlightLineOfSiteFlag->setChecked( theAutoHighlightLineOfSiteFlag);
      connect(thePreferences,
              SIGNAL(destroyed(QObject*)),
              this,
              SLOT(preferencesDestroyed(QObject*)));
      connect(thePreferences->theStagingCacheDirectory,
              SIGNAL(textChanged(const QString&)),
              this,
              SLOT(setStagingCacheDirectory(const QString&)));
      connect(thePreferences->theStagingCacheDirectoryButton,
              SIGNAL(clicked(bool)),
              this,
              SLOT(stagingCacheDirectoryButtonClicked(bool)));
      connect(thePreferences->theAutoHistogramStretchSetting,
              SIGNAL(activated(const QString&)),
              this,
              SLOT(setAutoHistogramStretchSetting(const QString&)));
      connect(thePreferences->theWmsTimeout,
              SIGNAL(textChanged(const QString&)),
              this,
              SLOT(setWmsTimeout(const QString&)));
      connect(thePreferences->theHudEnabled,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setHudEnabledFlag(bool)));
      connect(thePreferences->theAutomaticStagingEnabled,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setAutomaticStagingEnabled(bool)));
      
	  // added by russc 4/30/08
	  connect(thePreferences->archiveMappingEnabled,
			  SIGNAL(clicked(bool)),
			  this,
			  SLOT(setArchiveMappingEnabledFlag(bool)));
      connect(thePreferences->archiveMappingWidget,
              SIGNAL(itemChanged( QTableWidgetItem* )),
              this,
              SLOT(archiveMappingWidgetItemChanged(QTableWidgetItem*)));
      connect(thePreferences->archiveAddButton,
              SIGNAL(clicked(bool)),
              this,
              SLOT(archiveAddButtonClicked(bool)));
      connect(thePreferences->archiveRemoveButton,
              SIGNAL(clicked(bool)),
              this,
              SLOT(archiveRemoveButtonClicked(bool)));
      
      connect(thePreferences->theSousaIdentitySetButton,
              SIGNAL(clicked(bool)),
              this,
              SLOT(sousaIdentitySetButtonClicked(bool)));
      connect(thePreferences->theSousaServerConnectButton,
              SIGNAL(clicked(bool)),
              this,
              SLOT(sousaServerConnectButtonClicked(bool)));
      connect(thePreferences->theSousaServerAutoConnectOnStart,
              SIGNAL(clicked(bool)),
              this,
              SLOT(sousaServerAutoConnectOnStartClicked(bool)));
      
      connect(thePreferences->theAutoHighlightLineOfSiteFlag,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setAutoHighlightLineOfSiteFlag(bool)));              
      connect(thePreferences->theElevationEnabled,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setElevationEnabledFlag(bool)));
      connect(thePreferences->theMeshDetail,
              SIGNAL(valueChanged(int)),
              this,
              SLOT(setMeshDetailType(int)));
      connect(thePreferences->theTextureDetail,
              SIGNAL(valueChanged(int)),
              this,
              SLOT(setTextureDetailType(int)));
      connect(thePreferences->theTerrainCullingAmount,
              SIGNAL(valueChanged(int)),
              this,
              SLOT(setTerrainCullingType(int)));
      
      connect(thePreferences->theElevationMultiplier,
              SIGNAL(currentIndexChanged(const QString&)),
              this,
              SLOT(setHeightExageration(const QString&)));
      connect(thePreferences->theServerPortWidget,
              SIGNAL(itemChanged( QTableWidgetItem* )),
              this,
              SLOT(serverPortItemChanged(QTableWidgetItem*)));
      connect(thePreferences->theAddPortButton,
              SIGNAL(clicked ( bool )),
              this,
              SLOT(addPort(bool)));
      connect(thePreferences->theDeletePortButton,
              SIGNAL(clicked ( bool )),
              this,
              SLOT(deletePort(bool)));
       connect(thePreferences->theAddClientPortButton,
              SIGNAL(clicked ( bool )),
              this,
              SLOT(addClientPort(bool)));
      connect(thePreferences->theDeleteClientPortButton,
              SIGNAL(clicked ( bool )),
              this,
              SLOT(deleteClientPort(bool)));
      connect(thePreferences->theEnableAllClientPortButton,
              SIGNAL(clicked ( bool )),
              this,
              SLOT(enableAllClientPorts(bool)));
      connect(thePreferences->theDisableAllClientPortButton,
              SIGNAL(clicked ( bool )),
              this,
              SLOT(disableAllClientPorts(bool)));
      connect(thePreferences->theClientPortWidget,
             SIGNAL(itemChanged( QTableWidgetItem* )),
              this,
              SLOT(clientItemChanged(QTableWidgetItem*)));
      connect(thePreferences->theViewTransmitRate,
             SIGNAL(textChanged( const QString& )),
              this,
              SLOT(viewTransmitTextChanged(const QString&)));
      connect(thePreferences->theAutoViewSyncFlag,
              SIGNAL(stateChanged(int)),
              this,
              SLOT(autoViewSyncFlagChanged(int)));

      connect(thePreferences->theEphemerisEnabled,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setEphemerisEnabledFlag(bool)));
      connect(thePreferences->theEphemerisAutoUpdateToCurrentTime,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setEphemerisAutoUpdateToCurrentTimeFlag(bool)));
      connect(thePreferences->theSkyEnabled,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setSkyEnabledFlag(bool)));
      connect(thePreferences->theMoonEnabled,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setMoonEnabledFlag(bool)));
      connect(thePreferences->theSunLightingEnabled,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setSunlightEnabledFlag(bool)));
      connect(thePreferences->theVisibility,
              SIGNAL(valueChanged(int)),
              this,
              SLOT(setPercentVisibility(int)));
      
      connect(thePreferences->theCloudsEnabled,
              SIGNAL(clicked(bool)),
              this,
              SLOT(setCloudsEnabledFlag(bool)));
      connect(thePreferences->theCloudAmount,
              SIGNAL(valueChanged(int)),
              this,
              SLOT(setCloudAmount(int)));
      connect(thePreferences->theCloudSharpness,
              SIGNAL(valueChanged(int)),
              this,
              SLOT(setCloudSharpness(int)));
      
      thePreferences->theClientPortWidget->setHorizontalHeaderItem(PREFERENCES_CLIENT_WIDGET_NAME_IDX, new QTableWidgetItem("Name"));
      thePreferences->theClientPortWidget->setHorizontalHeaderItem(PREFERENCES_CLIENT_WIDGET_IP_IDX, new QTableWidgetItem("IP Address"));
      thePreferences->theClientPortWidget->setHorizontalHeaderItem(PREFERENCES_CLIENT_WIDGET_PORT_IDX, new QTableWidgetItem("Port"));
      thePreferences->theClientPortWidget->setHorizontalHeaderItem(PREFERENCES_CLIENT_WIDGET_PORT_TYPE_IDX, new QTableWidgetItem("Type"));
      
      thePreferences->theServerPortWidget->setHorizontalHeaderItem(PREFERENCES_SERVER_WIDGET_NAME_IDX, new QTableWidgetItem("Name"));
      thePreferences->theServerPortWidget->setHorizontalHeaderItem(PREFERENCES_SERVER_WIDGET_IP_IDX, new QTableWidgetItem("IP Address"));
      thePreferences->theServerPortWidget->setHorizontalHeaderItem(PREFERENCES_SERVER_WIDGET_PORT_IDX, new QTableWidgetItem("Port"));
      thePreferences->theServerPortWidget->setHorizontalHeaderItem(PREFERENCES_SERVER_WIDGET_PORT_TYPE_IDX, new QTableWidgetItem("Type"));
      
	  // added by russc
	  thePreferences->archiveMappingWidget->setHorizontalHeaderItem(PREFERENCES_ARCHIVE_WIDGET_SOURCE_IDX, new QTableWidgetItem("Source"));
	  thePreferences->archiveMappingWidget->setHorizontalHeaderItem(PREFERENCES_ARCHIVE_WIDGET_DESTINATION_IDX, new QTableWidgetItem("Destination"));

      populatePreferences();
   }
}

void ossimPlanetQtMainWindow::on_fileOpenWms_triggered(bool )
{
   if(theWmsWindow)
   {
      theWmsWindow->show();
      theWmsWindow->raise();
   }
   else
   {
      theWmsWindow = new ossimPlanetQtWmsDialog(0);
      theWmsWindow->setAttribute(Qt::WA_DeleteOnClose);
      connect(theWmsWindow,
              SIGNAL(addButtonHit(ossimPlanetQtWmsDialog*,
                                  ossimRefPtr<ossimWmsCapabilitiesDocument>,
                                  ossimPlanetQtWmsDialog::SelectedLayerInfoListType&)),
              this,
              SLOT(wmsAddButtonHit(ossimPlanetQtWmsDialog*,
                                   ossimRefPtr<ossimWmsCapabilitiesDocument>,
                                   ossimPlanetQtWmsDialog::SelectedLayerInfoListType&)));
      connect(theWmsWindow,
              SIGNAL(destroyed(QObject*)),
              this,
              SLOT(wmsDialogDestroyed(QObject*)));
      connect(theWmsWindow->theWmsSynchButton,
              SIGNAL(clicked(bool)),
              this,
              SLOT(wmsSyncCurrentSettings()));
      connect(theWmsWindow->theWmsSynchAllButton,
              SIGNAL(clicked(bool)),
              this,
              SLOT(wmsSyncAllSettings()));
      theWmsWindow->show();
   }
}

void ossimPlanetQtMainWindow::on_fileOpenKml_triggered( bool )
{
   QString extString = "Annotations(*.kml *.kmz *.KML *.KMZ)";
   
   QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                         "Open Kml file dialog",
                                                         ossimPlanetQtApplication::currentOpenKmlDirectory(),
                                                         extString);
   if(filenames.size() > 0)
   {
      ossim_uint32 idx = 0;

      ossimFilename lastFile;
      for(idx = 0; idx < (ossim_uint32)filenames.size(); ++idx)
      {
         ossimFilename kmlFilename(filenames[idx].toStdString());
         kmlLayer()->addKml(kmlFilename);
         lastFile = kmlFilename.path();
      }
      if(!lastFile.empty())
      {
         ossimPlanetQtApplication::setCurrentOpenKmlDirectory(lastFile.c_str());
      }
   }
}

void ossimPlanetQtMainWindow::on_openSession_triggered(bool )
{
   QString sessionDir = ossimPlanetQtApplication::sessionDirectory();
   QString filename = QFileDialog::getOpenFileName(this,
                                                   "Open Session",
                                                   ossimPlanetQtApplication::sessionDirectory(),
                                                   "*.session");
   
   if(filename != "")
   {
      loadSession(filename);
      ossimPlanetQtApplication::setCurrentOpenSessionDirectory(ossimFilename(filename.toStdString()).path().c_str());
   }
}

void ossimPlanetQtMainWindow::on_saveSession_triggered(bool)
{
   ossimRefPtr<ossimXmlNode> rootNode = new ossimXmlNode;
   ossimFilename sessionDir(ossimPlanetQtApplication::sessionDirectory().toStdString());
   if(!sessionDir.exists())
   {
      sessionDir.createDirectory();
   }
   if(theSessionFilename == "NONAME")
   {
      QString filename = QFileDialog::getSaveFileName(this,
                                                      "Save Session",
                                                      sessionDir.c_str(),
                                                      ".session");
      if(filename != "")
      {
         theSessionFilename = ossimFilename(filename.toStdString());
      }
   }
   if(theSessionFilename != "NONAME")
   {
      theSessionFilename.setExtension("session");
      saveSession(theSessionFilename.c_str());
      ossimPlanetQtApplication::setCurrentOpenSessionDirectory(theSessionFilename.path().c_str());
   }
}

void ossimPlanetQtMainWindow::on_saveSessionAs_triggered(bool)
{
   ossimRefPtr<ossimXmlNode> rootNode = new ossimXmlNode;
   ossimFilename sessionDir(ossimPlanetQtApplication::sessionDirectory().toStdString());
   if(!sessionDir.exists())
   {
      theCurrentOpenSessionPath.createDirectory();
   }

   QString filename = QFileDialog::getSaveFileName(this,
                                                   "Save Session",
                                                   sessionDir.c_str(),
                                                   ".session");
   if(filename != "")
   {
      theSessionFilename = ossimFilename(filename.toStdString());
   }

   if(theSessionFilename != "NONAME")
   {
      theSessionFilename.setExtension("session");
      saveSession(theSessionFilename.c_str());
      setWindowTitle(QString("ossimPlanet Session: ") + theSessionFilename);
      ossimPlanetQtApplication::setCurrentOpenSessionDirectory(theSessionFilename.path().c_str());
   }
}

void ossimPlanetQtMainWindow::on_editLayerOperation_triggered(bool)
{
#if 0
   if(!thePlanet.valid()||
      !thePlanet->land().valid())
   {
      return;
   }
   if(!thePlanet->land()->shadersInitialized())
   {
      return;
   }
   if(theLayerOperation)
   {
      theLayerOperation->show();
      theLayerOperation->raise();
   }
   else
   {
      theLayerOperation = new ossimPlanetQtLayerOperation(0);
//       theLayerOperation->setWindowFlags(theLayerOperation->windowFlags()|Qt::WindowStaysOnTopHint);
      theLayerOperation->setAttribute(Qt::WA_DeleteOnClose);
//      theLayerOperation->setLand(planet()->land().get());
      connect(theLayerOperation->theCloseButton,
              SIGNAL(clicked(bool)),
              this,
              SLOT(layerOperationCloseButtonClicked(bool)));
      theLayerOperation->show();
   }
#endif
}

void ossimPlanetQtMainWindow::on_actionActivity_triggered(bool)
{
   theActivityWindow->show();
   theActivityWindow->raise();
}

void ossimPlanetQtMainWindow::on_actionMessageLog_triggered(bool)
{
	theMessageLogDialog->show();
	theMessageLogDialog->raise();
}

void ossimPlanetQtMainWindow::on_viewRefreshTextures_triggered(bool)
{
   ossimPlanetTerrain* terrain = theGlWidget->viewer()->terrainLayer();
   if(terrain)
   {
      terrain->refreshImageLayers();
   }
#if 0
   if(thePlanet->land().valid())
   {
      thePlanet->land()->resetGraph(0, ossimPlanetLandRefreshType_TEXTURE);
   }
#endif
}

void ossimPlanetQtMainWindow::on_viewRefreshAll_triggered(bool)
{
   ossimPlanetTerrain* terrain = theGlWidget->viewer()->terrainLayer();
   if(terrain)
   {
      terrain->resetGraph();
   }
   //thePlanet->land()->resetGraph();
}

void ossimPlanetQtMainWindow::on_viewSyncLocation_triggered(bool)
{
   osg::ref_ptr<ossimPlanetLookAt> lookAt = theGlWidget->viewer()->currentLookAt()->clone();

   std::ostringstream out;
   
   out << setprecision(20) << ":navigator gotolatlonelevhpr " << lookAt->lat() << " "
       << lookAt->lon()    << " " << lookAt->altitude()        << " "           << lookAt->heading() 
       << " " << lookAt->pitch() << " " << lookAt->roll();

   sendMessage("View", out.str(), "Navigation");
}

void ossimPlanetQtMainWindow::on_toolsRuler_triggered( bool  )
{
   if(!theRuler)
   {
      theRuler = new ossimPlanetQtRuler(this);
      theRuler->setWindowFlags(theRuler->windowFlags()|Qt::WindowStaysOnTopHint);
      theRuler->setAttribute(Qt::WA_DeleteOnClose);
      theRuler->setGlWidget(theGlWidget);
      theRuler->setPlanet(thePlanet.get());
      connect(theRuler->theCloseButton,
              SIGNAL(clicked(bool)),
              this,
              SLOT(rulerCloseButtonClicked(bool)));
      theRuler->show();
   }
   else
   {
      theRuler->show();
      theRuler->raise();
   }
}

void ossimPlanetQtMainWindow::on_helpAbout_triggered(bool)
{
   ossimPlanetQtAboutDialog* aboutDialog = new ossimPlanetQtAboutDialog(this);
   aboutDialog->setAttribute(Qt::WA_DeleteOnClose);

   QString version = "OSSIM Planetary Visualization\n\n";
   
   version = version + ("Version " + QString(ossimString::toString(OSSIMPLANETQT_MAJOR_VERSION).c_str()) + "." +
                        QString(ossimString::toString(OSSIMPLANETQT_MINOR_VERSION).c_str()) + "." +
                        QString(ossimString::toString(OSSIMPLANETQT_PATCH_VERSION).c_str()) + " " +
                        QString(OSSIMPLANETQT_BUILD_STRING) + " " +
                        QString(OSSIM_BUILD_DATE));
   
   aboutDialog->theVersionInformation->setText(version);
   aboutDialog->show();
}

void ossimPlanetQtMainWindow::archiveMappingWidgetItemChanged(QTableWidgetItem* item)
{
	int row = thePreferences->archiveMappingWidget->row(item);
	int col = thePreferences->archiveMappingWidget->column(item);

	ossimPlanetArchiveMapping opam;
	if( (row > -1) && (row < (int)theArchive->getMappingList().size()) )
	{
		// dont need this line of code but we'll see
		opam = theArchive->getMappingList()[row];
	}

	switch(col)
	{
		case PREFERENCES_ARCHIVE_WIDGET_SOURCE_IDX:
		{
			if((row > -1)&&(row < (int)theArchive->getMappingList().size()))
			{
				ossimString src = ossimString(thePreferences->theServerPortWidget->item(row, col)->text().toStdString());
				theArchive->getMappingList()[row].setSource(src); 
				//opam.setSource(archiveMappingList[row].getSource().c_str());
			}
			break;
		}
		case PREFERENCES_ARCHIVE_WIDGET_DESTINATION_IDX:
		{
			ossimString dest = ossimString(thePreferences->archiveMappingWidget->item(row, col)->text().toStdString());
			theArchive->getMappingList()[row].setDestination(dest); 
			//opam.setSource(archiveMappingList[row].getDestination().c_str());            
			break;
		}
	}
	saveArchiveMappingPreferences();
	thePreferences->archiveMappingWidget->resizeColumnsToContents();
}

void ossimPlanetQtMainWindow::archiveAddButtonClicked(bool)
{
	thePreferences->archiveMappingWidget->blockSignals(true);

	ossimString src = thePreferences->archiveLineEdit_Src->text().toStdString();
	ossimString dest = thePreferences->archiveLineEdit_Dest->text().toStdString();

	ossimPlanetArchiveMapping opam = ossimPlanetArchiveMapping();

	// src can be empty, dest cant
	if( !dest.empty() )
	{
		opam.setSource(src);
		opam.setDestination(dest);

		theArchive->addMapping(opam);

		QString num = "Size: " + theArchive->getMappingList().size();
		thePreferences->archiveLineEdit_Src->setText(num);
		populateArchiveMappingPreferences();
		saveArchiveMappingPreferences();
	}

	thePreferences->archiveLineEdit_Src->clear();
	thePreferences->archiveLineEdit_Dest->clear();
	thePreferences->archiveMappingWidget->blockSignals(false);
}

void ossimPlanetQtMainWindow::archiveRemoveButtonClicked(bool)
{
	thePreferences->archiveMappingWidget->blockSignals(true);
	QList<QTableWidgetItem *> items = thePreferences->archiveMappingWidget->selectedItems();
	int idx = 0;
	int count = items.size() / 2;
	int row = 0;

	ossimPlanetArchiveMapping opam;

	ossimFilename src;
	ossimFilename dest;

	if( count == 1 )
	{
		row = thePreferences->archiveMappingWidget->row(items[idx]);
		if((row > -1)&&(row < (int)theArchive->getMappingList().size()))
		{
			opam = theArchive->getMappingList()[row];
			theArchive->removeMapping(opam);
		}
	}
	else if( count > 1 )
	{
		for( idx = count-1; idx >= 0; idx-- )
		{
			row = items[idx]->row();
			if( (row > -1) && (row < (int)theArchive->getMappingList().size()) )
			{
				opam = theArchive->getMappingList()[row];
				theArchive->removeMapping(opam);
			}
		}
	}

	populateArchiveMappingPreferences();
	saveArchiveMappingPreferences();
	thePreferences->archiveMappingWidget->blockSignals(false);
}

void ossimPlanetQtMainWindow::sousaIdentitySetButtonClicked(bool )
{
   if(theSousaLayer.valid())
   {
      ossimString username = thePreferences->theSousaUserName->text().toStdString();
      ossimString domain = thePreferences->theSousaDomain->text().toStdString();
      
      theSousaLayer->setIdentity(username, domain);
      saveSousaConnectionPreferences();
   }
}

void ossimPlanetQtMainWindow::sousaServerConnectButtonClicked(bool)
{
   if(theSousaLayer.valid())
   {
      ossimString ip, port;
      ip = thePreferences->theSousaServerIp->text().toStdString();
      port = thePreferences->theSousaServerPort->text().toStdString();
      
      theSousaLayer->setConnection(ip, ip, port, theDefaultDataPortType);
      saveSousaConnectionPreferences();
   }
}

void ossimPlanetQtMainWindow::sousaServerAutoConnectOnStartClicked(bool)
{
   saveSousaConnectionPreferences();
}

void ossimPlanetQtMainWindow::layerOperationCloseButtonClicked(bool)
{
   theLayerOperation->close();
   theLayerOperation=0;
}
void ossimPlanetQtMainWindow::rulerCloseButtonClicked(bool)
{
   theRuler->close();
   theRuler=0;
}

void ossimPlanetQtMainWindow::closeActivity(bool)
{
   theActivityWindow->close();
}

void ossimPlanetQtMainWindow::deleteSelectedActivities(bool)
{
	theActivityWindow->theActivityWidget->deleteSelectedActivities();
}

void ossimPlanetQtMainWindow::wmsDialogDestroyed(QObject* obj)
{
   if(theWmsWindow == obj)
   {
      theWmsWindow = 0;
   }
}

void ossimPlanetQtMainWindow::preferencesDestroyed(QObject* obj)
{
   if((thePreferences == obj)||!obj)
   {
      if(thePreferences)
      {
         thePreferences = 0;
      }
   }
}

void ossimPlanetQtMainWindow::wmsAddButtonHit(ossimPlanetQtWmsDialog* wms,
                                              ossimRefPtr<ossimWmsCapabilitiesDocument> capabilities,
                                              ossimPlanetQtWmsDialog::SelectedLayerInfoListType& layers)
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < layers.size(); ++idx)
   {
      osg::ref_ptr<ossimPlanetWmsImageLayer> wmsLayer = new ossimPlanetWmsImageLayer;
      wmsUrl url(layers[idx].theUrl.string());
      ossimString tempProtocol;
      ossimString tempServer;
      ossimString tempPath;
      ossimString tempOptions;
      url.splitUrl(tempProtocol, tempServer, tempPath, tempOptions);
      tempOptions = tempOptions.trim("&"); // make sure there are no trailing &
      if(!tempOptions.empty())
      {
         tempOptions = tempOptions + "&";
      }
      if(layers[idx].theStyle.empty())
      {
         tempOptions = tempOptions + "BGCOLOR=0x000000&LAYERS="+layers[idx].theLayer->getName()+"&STYLES="+"&FORMAT="+layers[idx].theImageFormat;
      }
      else
      {
         tempOptions = tempOptions + "BGCOLOR=0x000000&LAYERS="+layers[idx].theLayer->getName()+"&STYLES="+layers[idx].theStyle+"&FORMAT="+layers[idx].theImageFormat; 
      }
      url.mergeUrl(tempProtocol, tempServer, tempPath, tempOptions);
      wmsLayer->setServer(url.url());
      wmsLayer->setCacheDirectory(layers[idx].theRootCacheDir);
      double minLat = -90.0;
      double maxLat = 90.0;
      double minLon = -180.0;
      double maxLon = 180.0;
      const ossimRefPtr<ossimWmsBoundingBox> box = layers[idx].theLayer->findBoundingBox();
      if(box.valid())
      {
         if(box->isGeographic())
         {
            minLat = box->getMinY();
            minLon = box->getMinX();
            maxLat = box->getMaxY();
            maxLon = box->getMaxX();
         }
      }
      wmsLayer->getExtents()->setMinMaxLatLon(minLat, minLon, maxLat, maxLon);

      if(layers[idx].theLayer->getScaleHint().valid())
      {
         wmsLayer->getExtents()->setMinMaxScale(layers[idx].theLayer->getScaleHint()->getMin(),
                                                layers[idx].theLayer->getScaleHint()->getMax());
      }
      wmsLayer->dirtyExtents();
      wmsLayer->setEnableFlag(false);
      wmsLayer->setName(layers[idx].theDescription);
      wmsLayer->setProxyHost(layers[idx].theProxyHost);
      wmsLayer->setProxyPort(layers[idx].theProxyPort);
      wmsLayer->setProxyUser(layers[idx].theProxyUser);
      wmsLayer->setProxyPassword(layers[idx].theProxyPassword);
      
      wmsLayer->resetLookAt();
		
      thePlanetLegend->addTextureLayerToTop(wmsLayer.get());
      refreshLandTextures(wmsLayer->getExtents().get(),
                          ossimPlanetLandRefreshType_TEXTURE);
   }
}

void ossimPlanetQtMainWindow::wmsSyncCurrentSettings()
{
   
   QString connectionName = ossimPlanetQtApplication::readWmsSettingsEntry("selected");

   std::ostringstream out;

   out << "<ossimPlanetQtWms>"
       << wmsSettingsToXml(connectionName)
       << "</ossimPlanetQtWms>";

   sendMessage("", out.str(), "Data");
}

void ossimPlanetQtMainWindow::wmsSyncAllSettings()
{
   QStringList keys = ossimPlanetQtApplication::wmsSettingsSubkeyList();
   std::ostringstream out;
   out << "<ossimPlanetQtWms>";

   QStringList::Iterator it = keys.begin();
   while(it!=keys.end())
   {
      out << wmsSettingsToXml(*it);
      ++it;
   }
   out << "</ossimPlanetQtWms>";
  
   sendMessage("", out.str(), "Data");
}

std::string ossimPlanetQtMainWindow::wmsSettingsToXml(const QString& connectionName)const
{
   QSettings settings;
   
   QString key            = connectionName;
   QString connStringParts= ossimPlanetQtApplication::readWmsSettingsEntry(key+"/url");
   QString proxyHost = ossimPlanetQtApplication::readWmsSettingsEntry(key+"/proxyhost");
   QString proxyPort = ossimPlanetQtApplication::readWmsSettingsEntry(key+"/proxyport");
   QString proxyUser = ossimPlanetQtApplication::readWmsSettingsEntry(key+"/proxyuser");
   QString proxyPassword = ossimPlanetQtApplication::readWmsSettingsEntry(key+"/proxypassword");
   QString url           = ossimPlanetQtApplication::readWmsSettingsEntry(key + "/url");
   QString cache         = ossimPlanetQtApplication::readWmsSettingsEntry(key+"/cache");

   std::ostringstream out;

   out << "<Connection>"
       << "<name>"<<connectionName.toStdString() << "</name>"
       << "<proxyHost>"<<proxyHost.toStdString() << "</proxyHost>"
       << "<proxyPort>"<<proxyPort.toStdString() << "</proxyPort>"
       << "<proxyUser>"<<proxyUser.toStdString() << "</proxyUser>"
       << "<proxyPassword>"<<proxyPassword.toStdString() << "</proxyPassword>"
       << "<url>"<<url.toStdString()<<"</url>"
       << "<cache>"<<cache.toStdString() <<"</cache>"
       << "</Connection>";
   
   return out.str();
}

void ossimPlanetQtMainWindow::init()
{
   theCloudSeed     = time(0);
   theMaxVisibility = 500000;
   theInitializedFlag = true;
	theNodeCallback = new ossimPlanetQtMainWindow::NodeCallback(this);
   theAutoViewSyncRateInSeconds = ossimPlanetQtApplication::autoViewSyncRateInSeconds();
   theAutoViewSyncFlag = ossimPlanetQtApplication::autoViewSyncFlag();
   theLastAutoViewSyncTick = 0;
   setAcceptDrops(true);	   
   theUpdateHiglightsFlag = true;
   theAutoHighlightLineOfSiteFlag = ossimPlanetQtApplication::autoHighlightLineOfSitFlag();
   theWmsWindow = 0;
   thePreferences    = 0;
   theLayerOperation = 0; 
   theRuler          = 0; 
   QHBoxLayout* layout = new QHBoxLayout;  

   //tabWidget->setLayout(layout);
   tabWidget->widget(0)->setLayout(layout);
   QGLFormat format;
#if (defined(Q_WS_MAC) || defined(Q_WS_WIN))
   format.setSwapInterval(1);
#else
   if(format.swapInterval() >= 0)
   {
      format.setSwapInterval(1);
   }
#endif
   format.setOverlay(false);
   theGlWidget = new ossimPlanetQtViewer(format, tabWidget->widget(0));
   ossimPlanetViewer* viewer = new ossimPlanetViewer;
   viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
   theGlWidget->setViewer(viewer);
   theGlWidget->setMouseTracking(true);
//   tabWidget->setContentsMargins(0, 0, 0, 0);
   layout->addWidget(theGlWidget);
   theGlWidget->setFocusPolicy(Qt::StrongFocus);
   thePlanet = new ossimPlanet;
   //thePlanet->setupDefaults();
   theKmlLayer   = new ossimPlanetKmlLayer;
   
   theVideoLayer   = new ossimPlanetVideoLayer;
   theHudLayer     = new ossimPlanetLatLonHud;
   theHudLayer->setAutoUpdateFlag(true);
   theSousaLayer   = new ossimPlanetSousaLayer;
   theAnnotationLayer = new ossimPlanetAnnotationLayer;
	
   
   
   ossimPlanetTerrain* terrain = new ossimPlanetTerrain();
   terrain->initElevation();
   osg::ref_ptr<ossimPlanetGrid> grid = new ossimPlanetAdjustableCubeGrid(ossimPlanetAdjustableCubeGrid::MEDIUM_CAP);  
   terrain->setGrid(grid.get());
   terrain->setElevationDensityType(ossimPlanetTerrain::MEDIUM_LOW_ELEVATION_DENSITY);
   ossimPlanetMemoryImageCache* cache = new ossimPlanetMemoryImageCache;
   cache->setMinMaxCacheSizeInMegaBytes(16, 20);
   terrain->setElevationMemoryCache(cache);
   thePlanet->addChild(terrain);
   thePlanet->addChild(theAnnotationLayer.get());
   thePlanet->addChild(theKmlLayer.get());
   thePlanet->addChild(theVideoLayer.get());
   thePlanet->addChild(theSousaLayer.get());
   thePlanet->addChild(theHudLayer.get());
   theManipulator = new ossimPlanetManipulator();
   osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;
   keyswitchManipulator->addMatrixManipulator( '1', "Standard", theManipulator.get() );
//   keyswitchManipulator->addMatrixManipulator( '2', "Trackball", new osgGA::TrackballManipulator() );
//   keyswitchManipulator->addMatrixManipulator( '3', "Flight", new osgGA::FlightManipulator() );
//   keyswitchManipulator->addMatrixManipulator( '4', "Drive", new osgGA::DriveManipulator() );
//   keyswitchManipulator->addMatrixManipulator( '5', "Terrain", new osgGA::TerrainManipulator() );
   theGlWidget->viewer()->setCameraManipulator(keyswitchManipulator.get());
   theGlWidget->viewer()->addEventHandler( new osgGA::StateSetManipulator(theGlWidget->viewer()->getCamera()->getOrCreateStateSet()) );
   theGlWidget->viewer()->addEventHandler(new osgViewer::StatsHandler);
   theGlWidget->viewer()->setSceneData(thePlanet.get());
   theGlWidget->viewer()->addCallback(theViewerCallback.get());

   theTextureLayers.resize(2);
   theGlWidget->viewer()->terrainLayer()->setNumberOfTextureLayers(2);
   
   theTextureLayers[0] = new ossimPlanetTextureLayerGroup();
   theTextureLayers[0]->setName("Image Operation Layer");
   theTextureLayers[0]->setDescription("Top layer for various applications of GPU algorithms");

   theTextureLayers[1] = new ossimPlanetTextureLayerGroup();
   theTextureLayers[1]->setName("Image Layers");
   theTextureLayers[1]->setDescription("Root Image layer used to populate the planetary viewer");
   theLayerListener = new ossimPlanetQtTextureLayerCallback(this);
   theTextureLayers[1]->removeLayers(0, theTextureLayers[1]->numberOfLayers());
   theGlWidget->viewer()->terrainLayer()->setTextureLayer(0, theTextureLayers[1].get());
   theGlWidget->viewer()->terrainLayer()->setTextureLayer(1, theTextureLayers[0].get());
//    thePlanet->land()->setReferenceLayer(theTextureLayers[1].get());
//   thePlanet->land()->overlayLayers()->removeLayers(0, thePlanet->land()->overlayLayers()->numberOfLayers());
//   thePlanet->land()->overlayLayers()->addTop(theTextureLayers[0].get());
   theFullScreenFrame = 0;

   thePlanetLegend->setMainWindow(this);
   theActivityWindow = new ossimPlanetQtActivityWindow(this);
	theActivityWindow->setWindowFlags(theActivityWindow->windowFlags()|Qt::WindowStaysOnTopHint);

   setFocusPolicy(Qt::StrongFocus);

   connect(theActivityWindow->theCloseButton, SIGNAL(clicked(bool)), this, SLOT(closeActivity(bool)));
   connect(theActivityWindow->theDeleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteSelectedActivities(bool)));

//   theTimerId = startTimer(1000);
   theDesktopWidget = new QDesktopWidget;
  
  ossim_int32 numScreens = theDesktopWidget->numScreens();
  if(numScreens)
  {
     theFullScreenGeometry = theDesktopWidget->screenGeometry(0);
  }
//   for(int i = 0; i < numScreens; ++i)
//   {
//      QRect rect = theDesktopWidget->screenGeometry(i);
//      theFullScreenGeometry = theFullScreenGeometry.united(rect);
//   }
//    int w = theFullScreenGeometry.width();
//    int h = theFullScreenGeometry.height();
//    if(w > 4096)
//    {
//       w = 4096;
//    }
//    if(h > 4096)
//    {
//       h = 4096;
//    }
//    theFullScreenGeometry.setRect(theFullScreenGeometry.x(),
//                                  theFullScreenGeometry.y(),
//                                  w, h);
   theIsFullScreen = false;
   theSessionFilename = "NONAME";
   setWindowTitle(QString("ossimPlanet Session: ") + theSessionFilename);
   theIoThread     = new ossimPlanetIoThread;
   theIoMessageHandler = new ossimPlanetIoMainWindowMessageHandler(this);
   theIoThread->addMessageHandler(theIoMessageHandler.get());
//    theServerThread = new ossimPlanetServerThread;
//    theServerThread->addMessageHandler(new ossimPlanetQtViewMessageHandler());
//    theServerThread->addMessageHandler(new ossimPlanetQtUaimMessageHandler(theKmlLayer.get()));
//    theServerThread->setQueueMessagesFlag(true);
//    theClientThread = new ossimPlanetClientThread;
   connect(theGlWidget,
           SIGNAL(signalDropEvent(QDropEvent*)),
           this,
           SLOT(glWidgetDropEvent(QDropEvent*)));
   connect(theGlWidget,
           SIGNAL(signalMouseMoveEvent(QMouseEvent*)),
           this,
           SLOT(glWidgetMouseMoveEvent(QMouseEvent*)));
    connect(theGlWidget,
            SIGNAL(signalMouseReleaseEvent(QMouseEvent*)),
            this,
            SLOT(glWidgetMouseReleaseEvent(QMouseEvent*)));
   connect(theGlWidget,
           SIGNAL(signalDragEnterEvent(QDragEnterEvent*)),
           this,
           SLOT(glWidgetDragEnterEvent(QDragEnterEvent*)));


   // now initialize defaults
   //

   theHudLayer->setCompassTexture(ossimPlanetQtApplication::compassRing());
//   theHudLayer->setCompassTexture(ossimPlanetQtApplication::compassRing(),
//                                  ossimPlanetQtApplication::compassRose());
   
   theHudLayer->setEnableFlag(ossimPlanetQtApplication::hudEnabled());
   theGlWidget->viewer()->terrainLayer()->setElevationEnabledFlag(ossimPlanetQtApplication::elevEnabled());
   theGlWidget->viewer()->terrainLayer()->setElevationExaggeration(ossimPlanetQtApplication::elevExag());
//   thePlanet->land()->setElevationEnabledFlag(ossimPlanetQtApplication::elevEnabled());
//   thePlanet->land()->setHeightExag(ossimPlanetQtApplication::elevExag());
//   thePlanet->land()->setElevationPatchSize(ossimPlanetQtApplication::elevEstimate());
//   thePlanet->land()->setMaxLevelDetail(ossimPlanetQtApplication::levelOfDetail());
//   thePlanet->land()->setElevationCacheDir(ossimPlanetQtApplication::elevCacheDir());
   ///  NHV TEST
//   thePlanet->land()->setSplitMetricRatio(ossimPlanetQtApplication::splitMetricRatio());
//   thePlanet->land()->setMipMappingFlag(ossimPlanetQtApplication::mipMapping());
   std::deque<osg::ref_ptr<ossimPlanetTextureLayer> >& referenceImages = ossimPlanetQtApplication::referenceImages();

   for(ossimPlanetQtApplication::ReferenceImageListType::iterator iter = referenceImages.begin();
       iter != referenceImages.end(); ++iter)
   {
      theTextureLayers[1]->addTop(iter->get());
   }
   theTextureLayers[1]->sortByGsd();
//   thePlanet->land()->resetGraph();
   legend()->populate();
	// we will listen after we populte
   
   // added by russc 5/6/08
   // before we load the archiveMappings
   // create theArchive pointer
   theArchive = new ossimPlanetArchive();

   loadArchiveMappingFromPreferences();
   loadServerPortFromPreferences();
   loadClientPortFromPreferences();
   theGlWidget->viewer()->terrainLayer()->setElevationDensityType((ossimPlanetTerrain::ElevationDensityType)terrainTypeToInt(ossimPlanetQtApplication::elevationDetailType()));
   theGlWidget->viewer()->terrainLayer()->setTextureDensityType((ossimPlanetTerrain::TextureDensityType)terrainTypeToInt(ossimPlanetQtApplication::textureDetailType()));
   theGlWidget->viewer()->terrainLayer()->setCullAmountType((ossimPlanetTerrain::CullAmountType)terrainTypeToInt(ossimPlanetQtApplication::cullAmountType()));
//   thePlanet->land()->setPathnameAndRegister(":land");
   theAnnotationLayer->setPathnameAndRegister(":annotation");
   theSousaLayer->setPathnameAndRegister(":idolbridge");
   theSousaLayer->setArchive(theArchive);
   
   
	ossimString user = ossimPlanetQtApplication::sousaIdentityUserName().toStdString();
	ossimString domain = ossimPlanetQtApplication::sousaIdentityDomain().toStdString();
	if(user.empty())
	{
		user = ossimEnvironmentUtility::instance()->getUserName();
		ossimPlanetQtApplication::setSousaIdentityUserName(user.c_str());
	}
	theSousaLayer->setIdentity(user, domain);
	
   if(ossimPlanetQtApplication::sousaConnectionAutoConnectOnStart())
   {
      ossimString ip = ossimPlanetQtApplication::sousaConnectionIp().toStdString();
      ossimString port = ossimPlanetQtApplication::sousaConnectionPort().toStdString();
      if(!ip.empty()&&!port.empty())
      {
         theSousaLayer->setConnection(ip, ip, port, theDefaultDataPortType);
      }
   }
   
   setEphemerisEnabledFlag(ossimPlanetQtApplication::ephemerisEnabled());
   setEphemerisAutoUpdateToCurrentTimeFlag(ossimPlanetQtApplication::ephemerisAutoUpdateToCurrentTimeFlag());
   
   theTextureLayers[0]->addCallback(theLayerListener);
   theTextureLayers[1]->addCallback(theLayerListener);
	theAnnotationLayer->addCallback(theNodeCallback.get());
   theKmlLayer->addCallback(theNodeCallback.get());
   theVideoLayer->addCallback(theNodeCallback.get()); 
	theSousaLayer->addCallback(theNodeCallback.get());

}

void ossimPlanetQtMainWindow::populatePreferences()
{
   if(planet().valid())
   {
      thePreferences->theWmsTimeout->blockSignals(true);
      thePreferences->theHudEnabled->blockSignals(true);
      thePreferences->theAutomaticStagingEnabled->blockSignals(true);
      thePreferences->archiveMappingEnabled->blockSignals(true);
      thePreferences->theElevationEnabled->blockSignals(true);
      thePreferences->theMeshDetail->blockSignals(true);
      thePreferences->theTextureDetail->blockSignals(true);
      thePreferences->theTerrainCullingAmount->blockSignals(true);
      thePreferences->theEphemerisEnabled->blockSignals(true);
      thePreferences->theEphemerisAutoUpdateToCurrentTime->blockSignals(true);
      thePreferences->theSunLightingEnabled->blockSignals(true);
      thePreferences->theSkyEnabled->blockSignals(true);
      thePreferences->theMoonEnabled->blockSignals(true);
      thePreferences->theVisibility->blockSignals(true);
      thePreferences->theCloudsEnabled->blockSignals(true);
      thePreferences->theCloudAmount->blockSignals(true);
      thePreferences->theCloudSharpness->blockSignals(true);
      thePreferences->theAutoViewSyncFlag->blockSignals(true);
      thePreferences->theViewTransmitRate->blockSignals(true);
      thePreferences->theStagingCacheDirectory->blockSignals(true);
      thePreferences->theAutoHistogramStretchSetting->blockSignals(true);
      thePreferences->theAutoHistogramStretchSetting->addItem("None");
      thePreferences->theAutoHistogramStretchSetting->addItem("Linear Auto Min Max");
      thePreferences->theAutoHistogramStretchSetting->addItem("1 Standard Deviation");
      thePreferences->theAutoHistogramStretchSetting->addItem("2 Standard Deviation");
      thePreferences->theAutoHistogramStretchSetting->addItem("3 Standard Deviation");
      
      QString mode = ossimPlanetQtApplication::autoHistogramStretchMode();
      if(mode == "None")
      {
         thePreferences->theAutoHistogramStretchSetting->setCurrentIndex(0);
      }
      else if(mode == "Linear Auto Min Max")
      {
         thePreferences->theAutoHistogramStretchSetting->setCurrentIndex(1);
      }
      else if(mode == "1 Standard Deviation")
      {
         thePreferences->theAutoHistogramStretchSetting->setCurrentIndex(2);
      }
      else if(mode == "2 Standard Deviation")
      {
         thePreferences->theAutoHistogramStretchSetting->setCurrentIndex(3);
      }
      else if(mode == "3 Standard Deviation")
      {
         thePreferences->theAutoHistogramStretchSetting->setCurrentIndex(4);
      }
      
      thePreferences->theStagingCacheDirectory->setText(ossimPlanetQtApplication::stagingCacheDirectory());
      thePreferences->theWmsTimeout->setText(QString::number((ossim_uint32)ossimPlanetQtApplication::wmsNetworkTimeoutInSeconds()));
      thePreferences->theAutoViewSyncFlag->setCheckState(theAutoViewSyncFlag?Qt::Checked:Qt::Unchecked);
      thePreferences->theViewTransmitRate->setText(ossimString::toString(theAutoViewSyncRateInSeconds).trim('0').c_str());
      //      int cacheSizeInMegabytes = planet()->land()->getCacheSize()/(1024*1024);
      
      thePreferences->theHudEnabled->setChecked(theHudLayer->enableFlag());
      
      thePreferences->theAutomaticStagingEnabled->setChecked(ossimPlanetQtApplication::automaticStagingEnabledFlag());
      // todo: add a parameter within planet to use 	 
      
      // archive mapping
      theArchive->setArchiveMappingEnabledFlag(ossimPlanetQtApplication::archiveMappingEnabled());
      thePreferences->archiveMappingEnabled->setChecked(ossimPlanetQtApplication::archiveMappingEnabled());
      thePreferences->theElevationEnabled->setChecked(theGlWidget->viewer()->terrainLayer()->elevationEnabledFlag());
      thePreferences->theMeshDetail->setValue(terrainTypeToInt(ossimPlanetQtApplication::elevationDetailType()));
      thePreferences->theTextureDetail->setValue(terrainTypeToInt(ossimPlanetQtApplication::textureDetailType()));
//      thePreferences->theMeshDetail->setValue((int)theGlWidget->viewer()->terrainLayer()->elevationDensityType());
//      thePreferences->theTextureDetail->setValue((int)theGlWidget->viewer()->terrainLayer()->textureDensityType());
      thePreferences->theTerrainCullingAmount->setValue(terrainTypeToInt(ossimPlanetQtApplication::cullAmountType()));
      if(theGlWidget->viewer()->terrainLayer())
      {
         thePreferences->theElevationMultiplier->setCurrentIndex(((int)theGlWidget->viewer()->terrainLayer()->elevationExaggeration())-1);
      }
      thePreferences->theEphemerisEnabled->setChecked(ossimPlanetQtApplication::ephemerisEnabled());
      thePreferences->theEphemerisAutoUpdateToCurrentTime->setChecked(ossimPlanetQtApplication::ephemerisAutoUpdateToCurrentTimeFlag());
      thePreferences->theSunLightingEnabled->setChecked(ossimPlanetQtApplication::sunlightEnabled());
      thePreferences->theSkyEnabled->setChecked(ossimPlanetQtApplication::skyEnabled());
      thePreferences->theMoonEnabled->setChecked(ossimPlanetQtApplication::moonEnabled());
      thePreferences->theVisibility->setMinimum(0);
      thePreferences->theVisibility->setMaximum(100);
      thePreferences->theVisibility->setValue(ossimPlanetQtApplication::percentVisibility());
      
      thePreferences->theCloudAmount->setMinimum(-50);
      thePreferences->theCloudAmount->setMaximum(200);
      thePreferences->theCloudSharpness->setMinimum(70);
      thePreferences->theCloudSharpness->setMaximum(99);
      thePreferences->theCloudAmount->setTracking(false);
      thePreferences->theCloudSharpness->setTracking(false);
      thePreferences->theCloudsEnabled->setChecked(ossimPlanetQtApplication::cloudsEnabled());
      thePreferences->theCloudAmount->setValue(ossimPlanetQtApplication::cloudCoverage());
      thePreferences->theCloudSharpness->setValue(ossimPlanetQtApplication::cloudSharpness()*100);
      
      populateServerPortPreferences();
      populateClientPortPreferences();
	   populateArchiveMappingPreferences();
      populateSousaConnectionPreferences();
      thePreferences->theWmsTimeout->blockSignals(false);
      thePreferences->theHudEnabled->blockSignals(false);
      thePreferences->theAutomaticStagingEnabled->blockSignals(false);
	   thePreferences->archiveMappingEnabled->blockSignals(false);
      thePreferences->theElevationEnabled->blockSignals(false);
      thePreferences->theMeshDetail->blockSignals(false);
      thePreferences->theTextureDetail->blockSignals(false);
      thePreferences->theTerrainCullingAmount->blockSignals(false);
      thePreferences->theEphemerisEnabled->blockSignals(false);
      thePreferences->theEphemerisAutoUpdateToCurrentTime->blockSignals(false);
      thePreferences->theSunLightingEnabled->blockSignals(false);
      thePreferences->theSkyEnabled->blockSignals(false);
      thePreferences->theMoonEnabled->blockSignals(false);
      thePreferences->theVisibility->blockSignals(false);
      thePreferences->theCloudsEnabled->blockSignals(false);
      thePreferences->theCloudAmount->blockSignals(false);
      thePreferences->theCloudSharpness->blockSignals(false);
      thePreferences->theAutoViewSyncFlag->blockSignals(false);
      thePreferences->theViewTransmitRate->blockSignals(false);
      thePreferences->theStagingCacheDirectory->blockSignals(false);
      thePreferences->theAutoHistogramStretchSetting->blockSignals(false);
   }
}

void ossimPlanetQtMainWindow::saveServerPortPreferences()
{
   ossimPlanetQtApplication::removePreferenceSetting("servers");
   ossim_uint32 idx = 0;
   for(idx = 0; idx <theServerList.size();++idx )
   {
      QString key("servers/");
      key = key + QString("%1").arg(idx) + "/";
      ossimPlanetQtApplication::writePreferenceSetting(key + "name", theServerList[idx]->theName.c_str());
      ossimPlanetQtApplication::writePreferenceSetting(key + "host", theServerList[idx]->theHost.c_str());
      ossimPlanetQtApplication::writePreferenceSetting(key + "port", theServerList[idx]->thePort.c_str());
      ossimPlanetQtApplication::writePreferenceSetting(key + "port-type", theServerList[idx]->thePortType.c_str());
      ossimPlanetQtApplication::writePreferenceSetting(key + "enable-flag", ossimString::toString(theServerList[idx]->theIo->enableFlag()).c_str());
   }
}

void ossimPlanetQtMainWindow::saveClientPortPreferences()
{
   ossimPlanetQtApplication::removePreferenceSetting("clients");
   ossim_uint32 idx = 0;
   for(idx = 0; idx <theClientList.size();++idx )
   {
      QString key("clients/");
      key = key + QString("%1").arg(idx) + "/";
      ossimPlanetQtApplication::writePreferenceSetting(key + "name", theClientList[idx]->theName.c_str());
      ossimPlanetQtApplication::writePreferenceSetting(key + "host", theClientList[idx]->theHost.c_str());
      ossimPlanetQtApplication::writePreferenceSetting(key + "port", theClientList[idx]->thePort.c_str());
      ossimPlanetQtApplication::writePreferenceSetting(key + "port-type", theClientList[idx]->thePortType.c_str());
      ossimPlanetQtApplication::writePreferenceSetting(key + "enable-flag", ossimString::toString(theClientList[idx]->theIo->enableFlag()).c_str());
   }
}

void ossimPlanetQtMainWindow::saveArchiveMappingPreferences()
{
	ossimPlanetQtApplication::removePreferenceSetting("Sousa/archiveMappings");
	ossim_uint32 idx = 0;
	QString src;
	QString dest;

	for(idx = 0; idx < theArchive->getMappingList().size(); ++idx )
	{
		QString key("Sousa/archiveMappings/");
		key = key + QString("%1").arg(idx) + "/";
		src = theArchive->getMappingList()[idx].getSource().c_str();
		dest = theArchive->getMappingList()[idx].getDestination().c_str();
		ossimPlanetQtApplication::writePreferenceSetting(key + "source", src);
		ossimPlanetQtApplication::writePreferenceSetting(key + "destination", dest);
	}
}

void ossimPlanetQtMainWindow::saveSousaConnectionPreferences()
{
   ossimPlanetQtApplication::setSousaIdentityUserName(thePreferences->theSousaUserName->text());
   ossimPlanetQtApplication::setSousaIdentityDomain(thePreferences->theSousaDomain->text());
   ossimPlanetQtApplication::setSousaConnectionAutoConnectOnStart(thePreferences->theSousaServerAutoConnectOnStart->checkState()==Qt::Checked);
   ossimPlanetQtApplication::setSousaConnectionName(thePreferences->theSousaServerIp->text());
   ossimPlanetQtApplication::setSousaConnectionIp(thePreferences->theSousaServerIp->text());
   ossimPlanetQtApplication::setSousaConnectionPort(thePreferences->theSousaServerPort->text());
   ossimPlanetQtApplication::setSousaConnectionPortType(theDefaultDataPortType.c_str());
   
}

void ossimPlanetQtMainWindow::loadServerPortFromPreferences()
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx <theServerList.size();++idx )
   {
      theIoThread->removeIo(theServerList[idx]->theIo.get());
   }
   theServerList.clear();
   QStringList  serverList = ossimPlanetQtApplication::preferenceSettingsSubkeyList("servers");
   QStringList::iterator it = serverList.begin();
   while(it != serverList.end())
   {
      QString key = "servers/" + (*it);
      QString name = ossimPlanetQtApplication::readPreferenceSetting(key + "/name");
      QString host = ossimPlanetQtApplication::readPreferenceSetting(key + "/host");
      QString port = ossimPlanetQtApplication::readPreferenceSetting(key + "/port");
      QString portType = ossimPlanetQtApplication::readPreferenceSetting(key + "/port-type");
      QString enableFlag = ossimPlanetQtApplication::readPreferenceSetting(key + "/enable-flag");
      osg::ref_ptr<ossimPlanetQtMainWindow::SocketInfo> socketInfo = new ossimPlanetQtMainWindow::SocketInfo(name.toStdString(),
                                                                                                             host.toStdString(),
                                                                                                             port.toStdString(),
                                                                                                             portType.toStdString());
      ossimPlanetIoSocketServerChannel* server = new ossimPlanetIoSocketServerChannel();
      server->setSocket(host.toStdString(),
                        ossimString(port.toStdString()).toInt32(),
                        portType=="Navigation"?theDefaultNavigationPortType:theDefaultDataPortType);
      server->setEnableFlag(ossimString(enableFlag.toStdString()).toBool());
      socketInfo->theIo = server;
      theServerList.push_back(socketInfo.get());
      theIoThread->addIo(socketInfo->theIo.get());
      if(thePreferences)
      {
         populateServerPortPreferences();
      }
      ++it;
   }
//    ossim_uint32 size = serverList.size();
}

void ossimPlanetQtMainWindow::loadClientPortFromPreferences()
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx <theClientList.size();++idx )
   {
      theIoThread->removeIo(theClientList[idx]->theIo.get());
   }
   theClientList.clear();
   QStringList  clientList = ossimPlanetQtApplication::preferenceSettingsSubkeyList("clients");
   QStringList::iterator it = clientList.begin();
   while(it != clientList.end())
   {
      QString key = "clients/" + (*it);
      QString name = ossimPlanetQtApplication::readPreferenceSetting(key + "/name");
      QString host = ossimPlanetQtApplication::readPreferenceSetting(key + "/host");
      QString port = ossimPlanetQtApplication::readPreferenceSetting(key + "/port");
      QString portType = ossimPlanetQtApplication::readPreferenceSetting(key + "/port-type");
      QString enableFlag = ossimPlanetQtApplication::readPreferenceSetting(key + "/enable-flag");
      osg::ref_ptr<ossimPlanetQtMainWindow::SocketInfo> socketInfo = new ossimPlanetQtMainWindow::SocketInfo(name.toStdString(),
                                                                                                             host.toStdString(),
                                                                                                             port.toStdString(),
                                                                                                             portType.toStdString());
      ossimPlanetIoSocket* client = new ossimPlanetIoSocket();
      client->setAutoReconnectFlag(true);
      //client->setAutoCloseOnPeerShutdownFlag(false);

      // for now we will make this false.  We need to figure out how to do oberservers on
      // the TCP type sockets, until ten we will bring all client sockets up in disabled mode
      //
      client->setEnableFlag(false);//ossimString(enableFlag.toStdString()).toBool());
      if(portType == "Data")
      {
         client->setMaxBytesToSendPerIo(64*1024);
      }
      client->setSocket(host.toStdString(),
                        ossimString(port.toStdString()).toInt32(),
                        portType=="Navigation"?theDefaultNavigationPortType:theDefaultDataPortType);
      socketInfo->theIo = client;
      theClientList.push_back(socketInfo.get());
      theIoThread->addIo(socketInfo->theIo.get());
      if(thePreferences)
      {
         populateClientPortPreferences();
      }
      ++it;
   }
}

void ossimPlanetQtMainWindow::loadArchiveMappingFromPreferences()
{
	theArchive->getMappingList().clear();
	QStringList  mappingList = ossimPlanetQtApplication::preferenceSettingsSubkeyList("Sousa/archiveMappings");
	QStringList::iterator it = mappingList.begin();

	ossimString source;
	ossimString destination;
	QString key;
	QString src;
	QString dest;
	ossimPlanetArchiveMapping opam;

	while(it != mappingList.end())
	{
		key = "Sousa/archiveMappings/" + (*it);
		src = ossimPlanetQtApplication::readPreferenceSetting(key + "/source");
		dest = ossimPlanetQtApplication::readPreferenceSetting(key + "/destination");

		source = ossimString(src.toStdString());
		destination = ossimString(dest.toStdString());
		opam = ossimPlanetArchiveMapping(source, destination);
		theArchive->addMapping(opam);
		++it;
	}
   theArchive->setArchiveMappingEnabledFlag(ossimPlanetQtApplication::archiveMappingEnabled());

   populateArchiveMappingPreferences();
}


void ossimPlanetQtMainWindow::populateSousaConnectionPreferences()
{
   thePreferences->theSousaUserName->blockSignals(true);
   thePreferences->theSousaDomain->blockSignals(true);
   thePreferences->theSousaServerAutoConnectOnStart->blockSignals(true);
   thePreferences->theSousaServerIp->blockSignals(true);
   thePreferences->theSousaServerIp->blockSignals(true);
   thePreferences->theSousaServerPort->blockSignals(true);

   thePreferences->theSousaUserName->setText(ossimPlanetQtApplication::sousaIdentityUserName());
   thePreferences->theSousaDomain->setText(ossimPlanetQtApplication::sousaIdentityDomain());
   thePreferences->theSousaServerAutoConnectOnStart->setCheckState(ossimPlanetQtApplication::sousaConnectionAutoConnectOnStart()?Qt::Checked:Qt::Unchecked);
   thePreferences->theSousaServerIp->setText(ossimPlanetQtApplication::sousaConnectionName());
   thePreferences->theSousaServerIp->setText(ossimPlanetQtApplication::sousaConnectionIp());
   thePreferences->theSousaServerPort->setText(ossimPlanetQtApplication::sousaConnectionPort());

   thePreferences->theSousaUserName->blockSignals(false);
   thePreferences->theSousaDomain->blockSignals(false);
   thePreferences->theSousaServerAutoConnectOnStart->blockSignals(false);
   thePreferences->theSousaServerIp->blockSignals(false);
   thePreferences->theSousaServerIp->blockSignals(false);
   thePreferences->theSousaServerPort->blockSignals(false);
}

void ossimPlanetQtMainWindow::populateArchiveMappingPreferences()
{
	// this will need to be sync'd too
	if( !thePreferences ) return;

	ossim_uint32 index = 0;
	thePreferences->archiveMappingWidget->blockSignals(true);
	thePreferences->archiveMappingWidget->clear();
	thePreferences->archiveMappingWidget->setColumnCount(PREFERENCES_ARCHIVE_WIDGET_SIZE);
	thePreferences->archiveMappingWidget->setRowCount(theArchive->getMappingList().size());
	thePreferences->archiveMappingWidget->setHorizontalHeaderItem(PREFERENCES_ARCHIVE_WIDGET_SOURCE_IDX, new QTableWidgetItem("        Source        "));
	thePreferences->archiveMappingWidget->setHorizontalHeaderItem(PREFERENCES_ARCHIVE_WIDGET_DESTINATION_IDX, new QTableWidgetItem("         Destination         "));
	// loop thru the mappings list
	ossimFilename src;
	ossimFilename dest;
	ossimPlanetArchiveMapping opam;
	for(index = 0; index < theArchive->getMappingList().size(); ++index)
	{
		opam = theArchive->getMappingList()[index];
		src = opam.getSource();
		dest = opam.getDestination();

		thePreferences->archiveMappingWidget->setItem(index, PREFERENCES_ARCHIVE_WIDGET_SOURCE_IDX,
													new QTableWidgetItem(src.c_str()));

		thePreferences->archiveMappingWidget->setItem(index, PREFERENCES_ARCHIVE_WIDGET_DESTINATION_IDX,
													new QTableWidgetItem(dest.c_str()));
      
		thePreferences->archiveMappingWidget->setVerticalHeaderItem(index, new QTableWidgetItem(""));
	}
	if(theArchive->getMappingList().size()>0)
	{
		thePreferences->archiveMappingWidget->resizeColumnsToContents();
	}
	thePreferences->archiveMappingWidget->blockSignals(false);
}

void ossimPlanetQtMainWindow::populateServerPortPreferences()
{
   if(!thePreferences) return;
   ossim_uint32 idx = 0;
   thePreferences->theServerPortWidget->blockSignals(true);
   thePreferences->theServerPortWidget->clear();
   thePreferences->theServerPortWidget->setColumnCount(PREFERENCES_SERVER_WIDGET_SIZE);
   thePreferences->theServerPortWidget->setRowCount(theServerList.size());
   thePreferences->theServerPortWidget->setHorizontalHeaderItem(PREFERENCES_SERVER_WIDGET_NAME_IDX, new QTableWidgetItem("        Name        "));
   thePreferences->theServerPortWidget->setHorizontalHeaderItem(PREFERENCES_SERVER_WIDGET_IP_IDX, new QTableWidgetItem("           Ip           "));
   thePreferences->theServerPortWidget->setHorizontalHeaderItem(PREFERENCES_SERVER_WIDGET_PORT_IDX, new QTableWidgetItem("        Port        "));
   thePreferences->theServerPortWidget->setHorizontalHeaderItem(PREFERENCES_SERVER_WIDGET_PORT_TYPE_IDX, new QTableWidgetItem("            Type            "));
   thePreferences->theServerPortWidget->setHorizontalHeaderItem(PREFERENCES_SERVER_WIDGET_ENABLE_IDX, new QTableWidgetItem("       Enabled       "));
   QStringList typeList;
   typeList.push_back("Navigation");
   typeList.push_back("Data");
   ossimPlanetIoSocketServerChannel* socket = 0;
   for(idx = 0; idx <theServerList.size();++idx )
   {
      socket = static_cast<ossimPlanetIoSocketServerChannel*>(theServerList[idx]->theIo.get());
      thePreferences->theServerPortWidget->setItem(idx, PREFERENCES_SERVER_WIDGET_NAME_IDX,
                                                   new QTableWidgetItem(theServerList[idx]->theName.c_str()));
      thePreferences->theServerPortWidget->setItem(idx, PREFERENCES_SERVER_WIDGET_IP_IDX,
                                                   new QTableWidgetItem(theServerList[idx]->theHost.c_str()));
      thePreferences->theServerPortWidget->setItem(idx, PREFERENCES_SERVER_WIDGET_PORT_IDX,
                                                   new QTableWidgetItem(theServerList[idx]->thePort.c_str()));
      thePreferences->theServerPortWidget->setItem(idx, PREFERENCES_SERVER_WIDGET_PORT_TYPE_IDX,
                                                   new QTableWidgetItem(theServerList[idx]->thePortType.c_str()));
      thePreferences->theServerPortWidget->setItem(idx, PREFERENCES_SERVER_WIDGET_ENABLE_IDX,
                                                   new QTableWidgetItem(""));
      thePreferences->theServerPortWidget->item(idx, PREFERENCES_SERVER_WIDGET_ENABLE_IDX)->setCheckState(theServerList[idx]->theIo->enableFlag()?Qt::Checked:Qt::Unchecked);
      ossimPlanetQtDropDownDelegate* delegate = new ossimPlanetQtDropDownDelegate;
      delegate->setChoices(typeList);
      thePreferences->theServerPortWidget->setItemDelegateForColumn(PREFERENCES_SERVER_WIDGET_PORT_TYPE_IDX, delegate);

      thePreferences->theServerPortWidget->setVerticalHeaderItem(idx, new QTableWidgetItem(""));
      

      if(socket)
      {
         QColor color(0,0,0);
         if(socket->handle() < 0)
         {
            color = QColor(255,0,0);
         }
         thePreferences->theServerPortWidget->item(idx, PREFERENCES_SERVER_WIDGET_NAME_IDX)->setForeground(QBrush(color));
         thePreferences->theServerPortWidget->item(idx, PREFERENCES_SERVER_WIDGET_IP_IDX)->setForeground(QBrush(color));
         thePreferences->theServerPortWidget->item(idx, PREFERENCES_SERVER_WIDGET_PORT_IDX)->setForeground(QBrush(color));
         thePreferences->theServerPortWidget->item(idx, PREFERENCES_SERVER_WIDGET_PORT_TYPE_IDX)->setForeground(QBrush(color));
         thePreferences->theServerPortWidget->item(idx, PREFERENCES_SERVER_WIDGET_ENABLE_IDX)->setForeground(QBrush(color));
      }
   }
   if(theServerList.size()>0)
   {
      thePreferences->theServerPortWidget->resizeColumnsToContents();
   }
   thePreferences->theServerPortWidget->blockSignals(false);
}

void ossimPlanetQtMainWindow::populateClientPortPreferences()
{
   ossim_uint32 idx = 0;
   thePreferences->theClientPortWidget->blockSignals(true);
   thePreferences->theClientPortWidget->clear();
   thePreferences->theClientPortWidget->setColumnCount(PREFERENCES_CLIENT_WIDGET_SIZE);
   thePreferences->theClientPortWidget->setRowCount(theClientList.size());
   thePreferences->theClientPortWidget->setHorizontalHeaderItem(PREFERENCES_CLIENT_WIDGET_NAME_IDX, new QTableWidgetItem("        Name        "));
   thePreferences->theClientPortWidget->setHorizontalHeaderItem(PREFERENCES_CLIENT_WIDGET_IP_IDX, new QTableWidgetItem("     IP Address     "));
   thePreferences->theClientPortWidget->setHorizontalHeaderItem(PREFERENCES_CLIENT_WIDGET_PORT_IDX, new QTableWidgetItem("        Port        "));
   thePreferences->theClientPortWidget->setHorizontalHeaderItem(PREFERENCES_CLIENT_WIDGET_PORT_TYPE_IDX, new QTableWidgetItem("            Type            "));
   thePreferences->theClientPortWidget->setHorizontalHeaderItem(PREFERENCES_CLIENT_WIDGET_ENABLE_IDX, new QTableWidgetItem("       Enabled       "));
   QStringList typeList;
   typeList.push_back("Navigation");
   typeList.push_back("Data");
   ossimPlanetIoSocket* socket = 0;
   for(idx = 0; idx <theClientList.size();++idx )
   {
      socket = static_cast<ossimPlanetIoSocket*>(theClientList[idx]->theIo.get());
      thePreferences->theClientPortWidget->setItem(idx, PREFERENCES_CLIENT_WIDGET_NAME_IDX,
                                                   new QTableWidgetItem(theClientList[idx]->theName.c_str()));
      thePreferences->theClientPortWidget->setItem(idx, PREFERENCES_CLIENT_WIDGET_IP_IDX,
                                                   new QTableWidgetItem(theClientList[idx]->theHost.c_str()));
      thePreferences->theClientPortWidget->setItem(idx, PREFERENCES_CLIENT_WIDGET_PORT_IDX,
                                                   new QTableWidgetItem(theClientList[idx]->thePort.c_str()));
      thePreferences->theClientPortWidget->setItem(idx, PREFERENCES_CLIENT_WIDGET_PORT_TYPE_IDX,
                                                   new QTableWidgetItem(theClientList[idx]->thePortType.c_str()));
      thePreferences->theClientPortWidget->setItem(idx, PREFERENCES_CLIENT_WIDGET_ENABLE_IDX,
                                                   new QTableWidgetItem(""));
      thePreferences->theClientPortWidget->item(idx,PREFERENCES_CLIENT_WIDGET_ENABLE_IDX)->setCheckState(theClientList[idx]->theIo->enableFlag()?Qt::Checked:Qt::Unchecked);
      
      ossimPlanetQtDropDownDelegate* delegate = new ossimPlanetQtDropDownDelegate;
      delegate->setChoices(typeList);
      thePreferences->theClientPortWidget->setItemDelegateForColumn(PREFERENCES_CLIENT_WIDGET_PORT_TYPE_IDX, delegate);
      thePreferences->theClientPortWidget->setVerticalHeaderItem(idx, new QTableWidgetItem(""));
      
      if(socket)//check for handle problems
      {
         QColor color(0,0,0);
         if(socket->handle() < 0)
         {
            color = QColor(255,0,0);
         }
         thePreferences->theClientPortWidget->item(idx, PREFERENCES_CLIENT_WIDGET_NAME_IDX)->setForeground(QBrush(color));
         thePreferences->theClientPortWidget->item(idx, PREFERENCES_CLIENT_WIDGET_IP_IDX)->setForeground(QBrush(color));
         thePreferences->theClientPortWidget->item(idx, PREFERENCES_CLIENT_WIDGET_PORT_IDX)->setForeground(QBrush(color));
         thePreferences->theClientPortWidget->item(idx, PREFERENCES_CLIENT_WIDGET_PORT_TYPE_IDX)->setForeground(QBrush(color));
         thePreferences->theClientPortWidget->item(idx, PREFERENCES_CLIENT_WIDGET_ENABLE_IDX)->setForeground(QBrush(color));
     }
   }
   if(theClientList.size()>0)
   {
      thePreferences->theClientPortWidget->resizeColumnsToContents();
   }
   thePreferences->theClientPortWidget->blockSignals(false);
}


void ossimPlanetQtMainWindow::setWmsTimeout(const QString& value)
{
   bool ok=true;
   ossim_uint32 v = value.toLong(&ok);
   if(ok)
   {
      ossimPlanetQtApplication::setWmsNetworkTimeoutInSeconds(v);
   }
}

void ossimPlanetQtMainWindow::setStagingCacheDirectory(const QString& value)
{
   ossimPlanetQtApplication::setStagingCacheDirectory(value);
}

void ossimPlanetQtMainWindow::setAutoHistogramStretchSetting(const QString& text)
{
   ossimPlanetQtApplication::setAutoHistogramStretchMode(text);
}

void ossimPlanetQtMainWindow::stagingCacheDirectoryButtonClicked(bool checked)
{
   QString dir = QFileDialog::getExistingDirectory(thePreferences, 
                                                   "Please select the staging cache directory", 
                                                   ossimPlanetQtApplication::stagingCacheDirectory());
   if(!dir.isEmpty())
   {
      setStagingCacheDirectory(dir);
      if(thePreferences)
      {
         thePreferences->theStagingCacheDirectory->setText(ossimPlanetQtApplication::stagingCacheDirectory());
      }
   }
}

void ossimPlanetQtMainWindow::setHudEnabledFlag(bool flag)
{
   ossimPlanetQtApplication::setHudFlag(flag);
   if(theHudLayer->enableFlag() != flag)
   {
      theHudLayer->setEnableFlag(flag);
   }
}

void ossimPlanetQtMainWindow::setAutomaticStagingEnabled(bool flag)
{
   ossimPlanetQtApplication::setAutomaticStagingEnabledFlag(flag);
}

void ossimPlanetQtMainWindow::setArchiveMappingEnabledFlag(bool flag)
{
	ossimPlanetQtApplication::setArchiveMappingEnabledFlag(flag);

	if( thePreferences->archiveMappingEnabled->isChecked() == true )
	{
		thePreferences->archiveLineEdit_Src->setEnabled(true);
		thePreferences->archiveLineEdit_Dest->setEnabled(true);
		thePreferences->archiveAddButton->setEnabled(true);
		thePreferences->archiveRemoveButton->setEnabled(true);
		thePreferences->archiveMappingWidget->setEnabled(true);
		
		theArchive->setArchiveMappingEnabledFlag(true);
	}
	else
	{
		thePreferences->archiveLineEdit_Src->setEnabled(false);
		thePreferences->archiveLineEdit_Dest->setEnabled(false);
		thePreferences->archiveAddButton->setEnabled(false);
		thePreferences->archiveRemoveButton->setEnabled(false);
		thePreferences->archiveMappingWidget->setEnabled(false);

		theArchive->setArchiveMappingEnabledFlag(false);
	}
}
void ossimPlanetQtMainWindow::setAutoHighlightLineOfSiteFlag(bool flag)
{
   ossimPlanetQtApplication::setAutoHighlightLineOfSitFlag(flag);

   theAutoHighlightLineOfSiteFlag = flag;
}

void ossimPlanetQtMainWindow::setElevationEnabledFlag(bool flag)
{
   ossimPlanetQtApplication::setElevFlag(flag);
   theGlWidget->viewer()->terrainLayer()->setElevationEnabledFlag(flag);
   theGlWidget->viewer()->terrainLayer()->refreshElevationLayers();
#if 0
   if(planet()->land()->getElevationEnabledFlag() != flag)
   {
      planet()->land()->setElevationEnabledFlag(flag);
      
      refreshLandTextures();
   }
#endif
}


void ossimPlanetQtMainWindow::setTextureDetailType(int value)
{
   ossimString valueAsString = "medium";
   switch(value)
   {
      case ossimPlanetTerrain::LOW_TEXTURE_DENSITY:
      {
         valueAsString = "low";
         break;
      }
      case ossimPlanetTerrain::MEDIUM_LOW_TEXTURE_DENSITY:
      {
         valueAsString = "medium-low";
         break;
      }
      case ossimPlanetTerrain::MEDIUM_TEXTURE_DENSITY:
      {
         valueAsString = "medium";
         break;
      }
      case ossimPlanetTerrain::MEDIUM_HIGH_TEXTURE_DENSITY:
      {
         valueAsString = "medium-high";
         break;
      }
      case ossimPlanetTerrain::HIGH_TEXTURE_DENSITY:
      {
         valueAsString = "high";
         break;
      }
      default:
      {
         value =  ossimPlanetTerrain::MEDIUM_TEXTURE_DENSITY;
      }
   }
   ossimPlanetQtApplication::setTextureDetailType(valueAsString);
   theGlWidget->viewer()->terrainLayer()->setTextureDensityType((ossimPlanetTerrain::TextureDensityType)value);
   theGlWidget->viewer()->terrainLayer()->refreshImageLayers();
}

void ossimPlanetQtMainWindow::setMeshDetailType(int value)
{
   ossimString valueAsString = "medium";
   switch(value)
   {
      case ossimPlanetTerrain::LOW_ELEVATION_DENSITY:
      {
         valueAsString = "low";
         break;
      }
      case ossimPlanetTerrain::MEDIUM_LOW_ELEVATION_DENSITY:
      {
         valueAsString = "medium-low";
         break;
      }
      case ossimPlanetTerrain::MEDIUM_ELEVATION_DENSITY:
      {
         valueAsString = "medium";
         break;
      }
      case ossimPlanetTerrain::MEDIUM_HIGH_ELEVATION_DENSITY:
      {
         valueAsString = "medium-high";
         break;
      }
      case ossimPlanetTerrain::HIGH_ELEVATION_DENSITY:
      {
         valueAsString = "high";
         break;
      }
      default:
      {
         value =  ossimPlanetTerrain::MEDIUM_ELEVATION_DENSITY;
      }
   }
   ossimPlanetQtApplication::setElevationDetailType(valueAsString);
   theGlWidget->viewer()->terrainLayer()->setElevationDensityType((ossimPlanetTerrain::ElevationDensityType)value);
   theGlWidget->viewer()->terrainLayer()->refreshElevationLayers();
}

void ossimPlanetQtMainWindow::setTerrainCullingType(int value)
{
   ossimString valueAsString = "high";
   switch(value)
   {
      case ossimPlanetTerrain::NO_CULL:
      {
         valueAsString = "no-cull";
         break;
      }
      case ossimPlanetTerrain::LOW_CULL:
      {
         valueAsString = "low-cull";
         break;
      }
      case ossimPlanetTerrain::MEDIUM_LOW_CULL:
      {
         valueAsString = "medium-low-cull";
         break;
      }
      case ossimPlanetTerrain::MEDIUM_CULL:
      {
         valueAsString = "medium-cull";
         break;
      }
      case ossimPlanetTerrain::MEDIUM_HIGH_CULL:
      {
         valueAsString = "medium-high-cull";
         break;
      }
      case ossimPlanetTerrain::HIGH_CULL:
      {
         valueAsString = "high-cull";
         break;
      }
      default:
      {
         valueAsString = "high-cull";
         value = ossimPlanetTerrain::HIGH_CULL;
         break;
      }
   }
   ossimPlanetQtApplication::setCullAmountType(valueAsString);
   theGlWidget->viewer()->terrainLayer()->setCullAmountType((ossimPlanetTerrain::CullAmountType)value);
}

void ossimPlanetQtMainWindow::setHeightExageration(const QString& height)
{
   double value = ossimString(height.toStdString()).toDouble();
   ossimPlanetQtApplication::setElevExag(value);
  
   if(theGlWidget->viewer()->terrainLayer())
   {
      double value = ossimString(height.toStdString()).toDouble();
      theGlWidget->viewer()->terrainLayer()->setElevationExaggeration(value);
      theGlWidget->viewer()->terrainLayer()->refreshElevationLayers();
   }
#if 0
   double value = ossimString(height.toStdString()).toDouble();
   ossimPlanetQtApplication::setElevExag(value);
   if(planet()->land()->getHeightExag() != value)
   {
      planet()->land()->setHeightExag(value);
      refreshLandTextures();
   }
#endif
}

void ossimPlanetQtMainWindow::setEphemerisEnabledFlag(bool flag)
{
   ossimPlanetQtApplication::setEphemerisEnabledFlag(flag);
   int members = ephemerisMembers();
   if(flag)
   {
      if(!theGlWidget->viewer()->ephemeris())
      {
         theGlWidget->viewer()->addEphemeris(members);
         theGlWidget->viewer()->ephemeris()->setAutoUpdateToCurrentTimeFlag(true);
         theGlWidget->viewer()->ephemeris()->setAutoUpdateToCurrentTimeFlag(ossimPlanetQtApplication::ephemerisAutoUpdateToCurrentTimeFlag());
     }
      setPercentVisibility(ossimPlanetQtApplication::percentVisibility());
      setCloudsEnabledFlag(ossimPlanetQtApplication::cloudsEnabled());
   }
   else
   {
      theGlWidget->viewer()->removeEphemeris();
   }
   
   theGlWidget->viewer()->requestRedraw();
}

void ossimPlanetQtMainWindow::setEphemerisAutoUpdateToCurrentTimeFlag(bool flag)
{
   ossimPlanetQtApplication::setEphemerisAutoUpdateToCurrentTimeFlag(flag);
   if(theGlWidget->viewer()->ephemeris())
   {
      theGlWidget->viewer()->ephemeris()->setAutoUpdateToCurrentTimeFlag(flag);
   }
}

void ossimPlanetQtMainWindow::setSkyEnabledFlag(bool flag)
{
   ossimPlanetQtApplication::setSkyEnabledFlag(flag);
   if(theGlWidget->viewer()->ephemeris())
   {
      theGlWidget->viewer()->ephemeris()->setMembers(ephemerisMembers());
      theGlWidget->viewer()->requestRedraw();
   }
}
void ossimPlanetQtMainWindow::setMoonEnabledFlag(bool flag)
{
   ossimPlanetQtApplication::setMoonEnabledFlag(flag);
   if(theGlWidget->viewer()->ephemeris())
   {
      theGlWidget->viewer()->ephemeris()->setMembers(ephemerisMembers());
      theGlWidget->viewer()->requestRedraw();
   }
}

void ossimPlanetQtMainWindow::setSunlightEnabledFlag(bool flag)
{
   ossimPlanetQtApplication::setSunlightEnabledFlag(flag);
   if(theGlWidget->viewer()->ephemeris())
   {
      theGlWidget->viewer()->ephemeris()->setMembers(ephemerisMembers());
      theGlWidget->viewer()->requestRedraw();
   }
}

void ossimPlanetQtMainWindow::setPercentVisibility(int percentValue)
{
   ossimPlanetQtApplication::setPercentVisibility(percentValue);
   if(theGlWidget->viewer()->ephemeris())
   {
      if(percentValue == 100)
      {
         theGlWidget->viewer()->ephemeris()->setFogEnableFlag(false);
      }
      else
      {
         double t = static_cast<double>(percentValue)/100.0;
         theGlWidget->viewer()->ephemeris()->setFogEnableFlag(true);
         theGlWidget->viewer()->ephemeris()->setVisibility(t*theMaxVisibility);
      }
   }
   theGlWidget->viewer()->requestRedraw();
}

void ossimPlanetQtMainWindow::setCloudsEnabledFlag(bool flag)
{
   ossimPlanetQtApplication::setCloudsEnabledFlag(flag);
   if(theGlWidget->viewer()->ephemeris())
   {
      theCloudSeed = time(0);
      if(flag)
      {
         if(theGlWidget->viewer()->ephemeris()->numberOfCloudLayers() < 1)
         {
            theGlWidget->viewer()->ephemeris()->createGlobalCloud(0,
                                                                  20000,
                                                                  32,
                                                                  theCloudSeed,
                                                                  ossimPlanetQtApplication::cloudCoverage(),
                                                                  ossimPlanetQtApplication::cloudSharpness());
            theGlWidget->viewer()->ephemeris()->cloudLayer(0)->setMaxAltitudeToShowClouds(40000);
            theGlWidget->viewer()->ephemeris()->cloudLayer(0)->setScale(2);
         }
         else
         {
            theGlWidget->viewer()->ephemeris()->cloudLayer(0)->setEnableFlag(flag);
         }
      }
      else if(theGlWidget->viewer()->ephemeris()->numberOfCloudLayers() > 0)
      {
         theGlWidget->viewer()->ephemeris()->cloudLayer(0)->setEnableFlag(flag);
//         theGlWidget->viewer()->ephemeris()->removeClouds(0, theGlWidget->viewer()->ephemeris()->numberOfCloudLayers());
      }
   }
   theGlWidget->viewer()->requestRedraw();
}

void ossimPlanetQtMainWindow::setCloudAmount(int value)
{
   ossimPlanetQtApplication::setCloudCoverage(value);
   if(theGlWidget->viewer()->ephemeris())
   {
      if(theGlWidget->viewer()->ephemeris()->numberOfCloudLayers()>0)
      {
         theGlWidget->viewer()->ephemeris()->cloudLayer(0)->updateTexture(theCloudSeed,
                                                                          ossimPlanetQtApplication::cloudCoverage(),
                                                                          ossimPlanetQtApplication::cloudSharpness());
      }
   }
   theGlWidget->viewer()->requestRedraw();
}
void ossimPlanetQtMainWindow::setCloudSharpness(int value)
{
   ossimPlanetQtApplication::setCloudSharpness(static_cast<double>(value)/100.0);
   if(theGlWidget->viewer()->ephemeris())
   {
      if(theGlWidget->viewer()->ephemeris()->numberOfCloudLayers()>0)
      {
         if(theGlWidget->viewer()->ephemeris()->numberOfCloudLayers()>0)
         {
            theGlWidget->viewer()->ephemeris()->cloudLayer(0)->updateTexture(theCloudSeed,
                                                                             ossimPlanetQtApplication::cloudCoverage(),
                                                                             ossimPlanetQtApplication::cloudSharpness());
         }
      }
   }
   theGlWidget->viewer()->requestRedraw();
}

void ossimPlanetQtMainWindow::gotoLocationNadir(double lat, double lon, double height)
{
   ossimString actionString = ":navigator gotolatlonelevnadir ";
   actionString += (ossimString::toString(lat) + " " +
                    ossimString::toString(lon) + " " +
                    ossimString::toString(height));
   
   ossimPlanetDestinationCommandAction(actionString.c_str()).execute();
   
}

void ossimPlanetQtMainWindow::refreshLandTextures(osg::ref_ptr<ossimPlanetExtents> extents,
                                                  ossimPlanetLandRefreshType refreshType)
{
   ossimPlanetTerrain* terrain = theGlWidget->viewer()->terrainLayer();
   if(terrain)
   {
      terrain->refreshImageLayers();
   }
#if 0
   if(thePlanet.valid())
   {
      thePlanet->land()->resetGraph(extents.get(), refreshType);
   }
#endif
}

const osg::ref_ptr<ossimPlanetLookAt> ossimPlanetQtMainWindow::getLookAt()const
{
	return theGlWidget->viewer()->currentLookAt()->clone();
}

void ossimPlanetQtMainWindow::keyPressEvent ( QKeyEvent * event )
{
   if(theFullScreenFrame)
   {
      ((QObject*)theGlWidget)->event(event);
   }
}

void ossimPlanetQtMainWindow::keyReleaseEvent ( QKeyEvent * event )
{
   if((event->modifiers() == Qt::ControlModifier)&&
      (event->key()==Qt::Key_F))
   {
      on_viewToggleFullScreen_triggered();
      event->accept();
   }
	else if(event->key()==Qt::Key_R)
	{
		on_viewRefreshTextures_triggered();
      event->accept();
	}
   if((event->modifiers() == Qt::ControlModifier)&&
      (event->key()==Qt::Key_B))
   {
      on_viewBookmark_triggered();
   }
   else if(theFullScreenFrame)
   {
      ((QObject*)theGlWidget)->event(event);
   }
   
}

void ossimPlanetQtMainWindow::timerEvent ( QTimerEvent * event )
{
   if(event->timerId() == theTimerId)
   {
//       std::cout << "<update,cull,draw> = " << (int)(theGlWidget->getUpdateTimeInSeconds()*1000) << ", "
//                 << (int)(theGlWidget->getCullTimeInSeconds()*1000) << ", "
//                 << (int)(theGlWidget->getDrawTimeInSeconds()*1000) << "\n";
      
//      std::ostringstream out;
//       double megabyte = 1024*1024;
//       osg::ref_ptr<ossimPlanetTextureLayer::Stats> stats1 = theTextureLayers[0]->getStats()->clone();
//       osg::ref_ptr<ossimPlanetTextureLayer::Stats> stats2 = theTextureLayers[1]->getStats()->clone();
//       ossim_uint64 textureSize      = (stats1->totalTextureSize()+stats2->totalTextureSize());
//       ossim_uint64 bytesTransferred =  (stats1->bytesTransferred()+stats2->bytesTransferred());
//      out << "Frames Per Second: " << theGlWidget->getFramesPerSecond();
//           << "   Total Texture Size = " << (textureSize/megabyte) << " MB\t" 
//           << "Total transferred = " << (bytesTransferred/megabyte) << " MB\t"
//           << "Percentage = " <<  (((double)bytesTransferred/(double)textureSize)*100.0) << "%";
//      statusBar()->showMessage(out.str().c_str());

//      ossimString message;
//      std::list<QString> messageList;
//       while(theServerThread->nextMessage(message))
//       {
//          messageList.push_back(message.c_str());
//       }
      
//       if(!messageList.empty())
//       {
//          theActivityWindow->theActivityWidget->addActivity(new ossimPlanetQtMessageActivity(messageList,
//                                                                                             thePlanetLegend));
//       }

      if(theUpdateHiglightsFlag&&theAutoHighlightLineOfSiteFlag)
      {
         osg::Vec3d los = thePlanet->getLineOfSiteLatLonHeightPoint();
         if(!ossim::isnan(los[0])&&
            !ossim::isnan(los[1])&&
            !ossim::isnan(los[2]))
         {
            thePlanetLegend->selectLayersContainingPosition(los[0], los[1], los[2]);
//             std::vector<QTreeWidgetItem*> items;
//             thePlanetLegend->getLayersContainingPosition(items, los[0], los[1], los[2]);
            
//             if(items.size())
//             {
//                for(std::vector<QTreeWidgetItem*>::iterator iterItems = items.begin();
//                    iterItems != items.end();
//                    ++iterItems)
//                {
//                   (*iterItems)->setSelected(true);
//                }
//             }
            theUpdateHiglightsFlag = false;
         }
         else
         {
            thePlanetLegend->unselectItems();
         }
      }
   }
}

void ossimPlanetQtMainWindow::serverPortItemChanged(QTableWidgetItem* item)
{
   int row = thePreferences->theServerPortWidget->row(item);
   int col = thePreferences->theServerPortWidget->column(item);
   ossimPlanetIoSocketServerChannel* socket = 0;
   if((row>-1)&&(row<(int)theServerList.size()))
   {
      socket = dynamic_cast<ossimPlanetIoSocketServerChannel*>(theServerList[row]->theIo.get());
   }
   if(socket)
   {
      switch(col)
      {
         case PREFERENCES_SERVER_WIDGET_NAME_IDX:
         {
            if((row > -1)&&(row < (int)theServerList.size()))
            {
               theServerList[row]->theName = thePreferences->theServerPortWidget->item(row, col)->text().toStdString();
               socket->setName(theServerList[row]->theName);
            }
            break;
         }
         case PREFERENCES_SERVER_WIDGET_IP_IDX:
         {
            theServerList[row]->theHost = thePreferences->theServerPortWidget->item(row, col)->text().toStdString();
            socket->setSocket(theServerList[row]->theHost,
                              theServerList[row]->thePort.toInt32(),
                              ((theServerList[row]->thePortType=="Navigation"?theDefaultNavigationPortType:theDefaultDataPortType)));
            break;
         }
         case PREFERENCES_SERVER_WIDGET_PORT_IDX:
         {
            theServerList[row]->thePort = thePreferences->theServerPortWidget->item(row, col)->text().toStdString();
            socket->setSocket("",
                              theServerList[row]->thePort.toInt32(),
                              ((theServerList[row]->thePortType=="Navigation"?theDefaultNavigationPortType:theDefaultDataPortType)));
            break;
         }
            
         case PREFERENCES_SERVER_WIDGET_PORT_TYPE_IDX:
         {
            theServerList[row]->thePortType = thePreferences->theServerPortWidget->item(row, col)->text().toStdString();
            socket->setSocket(theServerList[row]->theHost,
                              theServerList[row]->thePort.toInt32(),
                              ((theServerList[row]->thePortType=="Navigation"?theDefaultNavigationPortType:theDefaultDataPortType)));
            break;
         }
         case PREFERENCES_SERVER_WIDGET_ENABLE_IDX:
         {
            if(thePreferences->theServerPortWidget->item(row, col)->checkState() == Qt::Unchecked)
            {
               theServerList[row]->theIo->setEnableFlag(false);
            }
            else
            {
               theServerList[row]->theIo->setEnableFlag(true);
            }
         }
      }
      QColor color(0,0,0);
      if(socket->handle() < 0)
      {
         color = QColor(255,0,0);
      }
      thePreferences->theServerPortWidget->item(row, PREFERENCES_SERVER_WIDGET_NAME_IDX)->setForeground(QBrush(color));
      thePreferences->theServerPortWidget->item(row, PREFERENCES_SERVER_WIDGET_IP_IDX)->setForeground(QBrush(color));
      thePreferences->theServerPortWidget->item(row, PREFERENCES_SERVER_WIDGET_PORT_IDX)->setForeground(QBrush(color));
      thePreferences->theServerPortWidget->item(row, PREFERENCES_SERVER_WIDGET_PORT_TYPE_IDX)->setForeground(QBrush(color));
      thePreferences->theServerPortWidget->item(row, PREFERENCES_SERVER_WIDGET_ENABLE_IDX)->setForeground(QBrush(color));
   }
   saveServerPortPreferences();
   thePreferences->theServerPortWidget->resizeColumnsToContents();
}

void ossimPlanetQtMainWindow::addPort(bool)
{
   thePreferences->theServerPortWidget->blockSignals(true);
   osg::ref_ptr<ossimPlanetQtMainWindow::SocketInfo> socketInfo = new ossimPlanetQtMainWindow::SocketInfo("My Server", "localhost", "8000", "Navigation");
   ossimPlanetIoSocketServerChannel* server = new ossimPlanetIoSocketServerChannel();
   server->setSocket("localhost", 8000, theDefaultNavigationPortType);
   socketInfo->theIo = server;
   theServerList.push_back(socketInfo.get());
   theIoThread->addIo(socketInfo->theIo.get());
   populateServerPortPreferences();
   saveServerPortPreferences();
   thePreferences->theServerPortWidget->blockSignals(false);
}

void ossimPlanetQtMainWindow::deletePort(bool)
{
   thePreferences->theServerPortWidget->blockSignals(true);
   QList<QTableWidgetItem *> items = thePreferences->theServerPortWidget->selectedItems();
//    QList<QTableWidgetItem *>::iterator iter = items.begin();
   int count = items.size();
   int idx = 0;
   if(count > 0)
   {
      for(idx = count-1; idx > -1;--idx)
      {
         int row = thePreferences->theServerPortWidget->row(items[idx]);
         if((row > -1)&&(row < (int)theServerList.size()))
         {
            if(theIoThread->removeIo(theServerList[row]->theIo.get()))
            {
               theServerList.erase(theServerList.begin() + row);
            }
         }
      }
      populateServerPortPreferences();
   }
   saveServerPortPreferences();
   thePreferences->theServerPortWidget->blockSignals(false);
}

void ossimPlanetQtMainWindow::addClientPort(bool)
{
   thePreferences->theClientPortWidget->blockSignals(true);
   osg::ref_ptr<ossimPlanetQtMainWindow::SocketInfo> socketInfo = new ossimPlanetQtMainWindow::SocketInfo("My Socket", "", "8000", "Navigation");
   ossimPlanetIoSocket* socket = new ossimPlanetIoSocket();
   socket->setAutoReconnectFlag(true);
   //socket->setAutoCloseOnPeerShutdownFlag(false);
   socketInfo->theIo = socket;
   theClientList.push_back(socketInfo.get());
   theIoThread->addIo(socketInfo->theIo.get());
   populateClientPortPreferences();
   saveClientPortPreferences();
   thePreferences->theClientPortWidget->blockSignals(false);
}

void ossimPlanetQtMainWindow::deleteClientPort(bool)
{
   thePreferences->theClientPortWidget->blockSignals(true);
   QList<QTableWidgetItem *> items = thePreferences->theClientPortWidget->selectedItems();
   int count = items.size();
   int idx = 0;
   if(count > 0)
   {
      for(idx = count-1; idx > -1;--idx)
      {
         int row = thePreferences->theClientPortWidget->row(items[idx]);
         if((row > -1)&&(row < (int)theClientList.size()))
         {
            if(theIoThread->removeIo(theClientList[row]->theIo.get()))
            {
               theClientList.erase(theClientList.begin() + row);
            }
         }
      }
      populateClientPortPreferences();
   }
   saveClientPortPreferences();
   thePreferences->theClientPortWidget->blockSignals(false);
}

void ossimPlanetQtMainWindow::clientItemChanged(QTableWidgetItem* item)
{
   int row = thePreferences->theClientPortWidget->row(item);
   int col = thePreferences->theClientPortWidget->column(item);
   ossimPlanetIoSocket* socket = 0;
   
   if((row>-1)&&(row<(int)theClientList.size()))
   {
      socket = dynamic_cast<ossimPlanetIoSocket*>(theClientList[row]->theIo.get());
   }
   if(socket)
   {
      if(col == PREFERENCES_CLIENT_WIDGET_NAME_IDX) // name changed
      {
         if((row > -1)&&(row < (int)theClientList.size()))
         {
            theClientList[row]->theName = thePreferences->theClientPortWidget->item(row, 0)->text().toStdString();
            socket->setName(theClientList[row]->theName);
         }
      }
      else if(col == PREFERENCES_CLIENT_WIDGET_ENABLE_IDX) // enable disable flag
      {
         if(thePreferences->theClientPortWidget->item(row, col)->checkState() == Qt::Unchecked)
         {
            theClientList[row]->theIo->setEnableFlag(false);
         }
         else
         {
            theClientList[row]->theIo->setEnableFlag(true);
         }

      }
      else
      {
         ossimString test;
         theClientList[row]->theName     = thePreferences->theClientPortWidget->item(row, PREFERENCES_CLIENT_WIDGET_NAME_IDX)->text().toStdString();
         theClientList[row]->theHost     = thePreferences->theClientPortWidget->item(row, PREFERENCES_CLIENT_WIDGET_IP_IDX)->text().toStdString();
         theClientList[row]->thePort     = thePreferences->theClientPortWidget->item(row, PREFERENCES_CLIENT_WIDGET_PORT_IDX)->text().toStdString();
         theClientList[row]->thePortType = thePreferences->theClientPortWidget->item(row, PREFERENCES_CLIENT_WIDGET_PORT_TYPE_IDX)->text().toStdString();
         socket->setName(theClientList[row]->theName);
         test = theDefaultNavigationPortType;
         if(theClientList[row]->thePortType == "Data")
         {
            test = theDefaultDataPortType;
         }
         if(test == "tcp")
         {
            socket->setMaxBytesToSendPerIo(64*1024);
         }
         else
         {
            socket->setMaxBytesToSendPerIo(4*1024);
         }
         socket->setSocket(theClientList[row]->theHost,
                           theClientList[row]->thePort.toInt32(),
                           test);
      }
      QColor color(0,0,0);
      if(socket->handle() < 0)
      {
         color = QColor(255,0,0);
      }
      thePreferences->theClientPortWidget->item(row, PREFERENCES_CLIENT_WIDGET_NAME_IDX)->setForeground(QBrush(color));
      thePreferences->theClientPortWidget->item(row, PREFERENCES_CLIENT_WIDGET_IP_IDX)->setForeground(QBrush(color));
      thePreferences->theClientPortWidget->item(row, PREFERENCES_CLIENT_WIDGET_PORT_IDX)->setForeground(QBrush(color));
      thePreferences->theClientPortWidget->item(row, PREFERENCES_CLIENT_WIDGET_PORT_TYPE_IDX)->setForeground(QBrush(color));
      thePreferences->theClientPortWidget->item(row, PREFERENCES_CLIENT_WIDGET_ENABLE_IDX)->setForeground(QBrush(color));
   }
   thePreferences->theClientPortWidget->resizeColumnsToContents();
   saveClientPortPreferences();
}

void ossimPlanetQtMainWindow::enableAllClientPorts(bool)
{
   bool needUpdating = false;
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theClientList.size(); ++idx)
   {
      if(!theClientList[idx]->theIo->enableFlag())
      {
         theClientList[idx]->theIo->setEnableFlag(true);
         theClientList[idx]->theIo->openIo();
         needUpdating = true;
      }
   }
   if(needUpdating)
   {
      populateClientPortPreferences();
      saveClientPortPreferences();
   }
}

void ossimPlanetQtMainWindow::disableAllClientPorts(bool)
{
   bool needUpdating = false;
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theClientList.size(); ++idx)
   {
      if(theClientList[idx]->theIo->enableFlag())
      {
         theClientList[idx]->theIo->setEnableFlag(false);
         theClientList[idx]->theIo->closeIo();
         needUpdating = true;
      }
   }
   if(needUpdating)
   {
      populateClientPortPreferences();
      saveClientPortPreferences();
   }
}


void ossimPlanetQtMainWindow::viewTransmitTextChanged(const QString& value)
{
   bool ok;
   double tempValue = value.toDouble(&ok);
   theAutoViewSyncRateInSeconds = value.toDouble(&ok);
   if(ok)
   {
      ossimPlanetQtApplication::setAutoViewSyncRateInSeconds(tempValue);
      theAutoViewSyncRateInSeconds = tempValue;
   }
}

void ossimPlanetQtMainWindow::autoViewSyncFlagChanged(int state)
{
   theAutoViewSyncFlag = (state==Qt::Unchecked?false:true);
   ossimPlanetQtApplication::setAutoViewSyncFlag(theAutoViewSyncFlag);
}

void ossimPlanetQtMainWindow::viewChanged()
{
   theUpdateHiglightsFlag = true;
   osg::ref_ptr<ossimPlanetLookAt> camera = theGlWidget->viewer()->currentCamera()->clone();
   osg::ref_ptr<ossimPlanetLookAt> lookAt = theGlWidget->viewer()->currentLookAt()->clone();
   
   if(theAutoViewSyncFlag)
   {
      if(theLastAutoViewSyncTick == 0)
      {
         theLastAutoViewSyncTick = osg::Timer::instance()->tick();
      }
      else if(osg::Timer::instance()->delta_s(theLastAutoViewSyncTick, osg::Timer::instance()->tick()) >= theAutoViewSyncRateInSeconds)
      {
         std::ostringstream out;
            
         out << setprecision(20)
             << "<Set target=\"" << ":navigator\" vref=\"wgs84\">"
             <<   "<Camera>"
             <<      "<longitude>" << camera->lon() << "</longitude>"
             <<      "<latitude>"  << camera->lat() << "</latitude>"
             <<      "<altitude>" << camera->altitude() << "</altitude>"
             <<      "<heading>"   << camera->heading() << "</heading>"
             <<      "<pitch>"     << camera->pitch() << "</pitch>"
             <<      "<roll>"      << camera->roll() << "</roll>"
             <<      "<altitudeMode>" << camera->modeToString(camera->altitudeMode()) << "</altitudeMode>"
             <<   "</Camera>";
             if(lookAt.valid())
             {
                out << lookAt->toKml();
             }
         
         
           out  << "</Set>";
//         out << setprecision(20) << ":navigator setlatlonelevhpr " << lat << " "
//             << lon << " " << height << " " << heading << " " << pitch << " "
//             << roll << std::endl;
         sendMessage("View", out.str(), "Navigation");
         theLastAutoViewSyncTick = 0;
      }
   }
   else
   {
      theLastAutoViewSyncTick = 0;
   }
   if(theUpdateHiglightsFlag&&theAutoHighlightLineOfSiteFlag)
   {
      osg::Vec3d los(lookAt->lat(), lookAt->lon(), lookAt->altitude());
      if(!ossim::isnan(los[0])&&
         !ossim::isnan(los[1])&&
         !ossim::isnan(los[2]))
      {
         thePlanetLegend->selectLayersContainingPosition(los[0], los[1], los[2]);
//             std::vector<QTreeWidgetItem*> items;
//             thePlanetLegend->getLayersContainingPosition(items, los[0], los[1], los[2]);
            
//             if(items.size())
//             {
//                for(std::vector<QTreeWidgetItem*>::iterator iterItems = items.begin();
//                    iterItems != items.end();
//                    ++iterItems)
//                {
//                   (*iterItems)->setSelected(true);
//                }
//             }
         theUpdateHiglightsFlag = false;
      }
      else
      {
         thePlanetLegend->unselectItems();
      }
   }
//    if(theClientThread->getNumberOfConnections()>0)
//    {
//       ostringstream out;
      
//       out << setprecision(20) << ":navigator setlatlonelevhpr " << lat << " "
//           << lon << " " << height << " " << heading << " " << pitch << " "
//           << roll << std::endl;
      
//       theClientThread->broadcastMessage(out.str().c_str());
//    }
}

void ossimPlanetQtMainWindow::glWidgetDropEvent(QDropEvent * event)
{
   thePlanetLegend->dropEvent(event);
}

void ossimPlanetQtMainWindow::glWidgetDragEnterEvent(QDragEnterEvent *event)
{
   thePlanetLegend->dragEnterEvent(event);
}

void ossimPlanetQtMainWindow::glWidgetMouseMoveEvent(QMouseEvent* event)
{
}

class MyBrowser : public QTextBrowser
{
public:
   class NodeCallback : public ossimPlanetNodeCallback
   {
   public:
      NodeCallback(MyBrowser* browser)
      :theBrowser(browser)
      {
         
      }
      virtual void propertyChanged(ossimPlanetNode* node,
                                   const ossimString& name)
      {
         if(theBrowser && (name == "description")&&(theBrowser->theNode == node))
         {
            QApplication::postEvent(theBrowser,
                                    new ossimPlanetQt::NodePropertyEvent(node, name));

         }
      }
      virtual void destructingNode(ossimPlanetNode* node)
      {
         if(theBrowser)
         {
            if(theBrowser->theNode == node)
            {
               theBrowser->theNode = 0;
               if(theBrowser->parent())
               {
                  theBrowser->parent()->deleteLater();
               }
            }
         }
      }
      void setBrowser(MyBrowser* browser)
      {
         theBrowser = browser;
      }
   protected:
      MyBrowser* theBrowser;
   };
   friend class NodeCallbck;
   MyBrowser(QWidget* parent,
             ossimPlanetNode* node)
      :QTextBrowser(parent),
       theNode(node)
      {
         theCallback = new NodeCallback(this);
         if(theNode)
         {
            theNode->addCallback(theCallback.get());
         }
      }
   virtual ~MyBrowser()
   {
      if(theNode)
      {
         theNode->removeCallback(theCallback.get());
         theNode = 0;
      }
   }
   virtual bool event(QEvent* e)
   {
      if(theNode)
      {
			ossimPlanetQt::ossimPlanetQtEventId eventId = static_cast<ossimPlanetQt::ossimPlanetQtEventId>(e->type());
         switch(eventId)
         {
            case ossimPlanetQt::NODE_PROPERTY_EVENT_TYPE:
            {
               ossimPlanetQt::NodePropertyEvent* propEvent = dynamic_cast<ossimPlanetQt::NodePropertyEvent*>(e);
               if(propEvent)
               {
                  if(propEvent->name() == "description")
                  {
                     ossimString description;
                     theNode->description(description);
                     setHtml(description.c_str());
                  }
               }
               
               return true;
            }
				default:
				{
					break;
				}
         }
      }
      return QTextBrowser::event(e);
   }
   bool loadFile(QImage& image, const ossimFilename& file)
      {
         ossimRefPtr<ossimImageHandler> ih = ossimImageHandlerRegistry::instance()->open(file);
         ossimRefPtr<ossimScalarRemapper> remapper = new ossimScalarRemapper;
         if(ih.valid())
         {
            remapper->connectMyInputTo(0, ih.get());
            ossimRefPtr<ossimImageData> imageData = remapper->getTile(remapper->getBoundingRect());
            if((imageData.valid())&&imageData->getBuf()&&
               (imageData->getDataObjectStatus()!=OSSIM_EMPTY))
            {
               image = QImage(imageData->getWidth(),
                              imageData->getHeight(),
                              QImage::Format_RGB32);

               memset(image.bits(), '\0', image.numBytes());

               ossim_uint8* bands[3];
               if(imageData->getNumberOfBands() < 3)
               {
                  bands[0] = (ossim_uint8*)imageData->getBuf(0);
                  bands[1] = (ossim_uint8*)imageData->getBuf(0);
                  bands[2] = (ossim_uint8*)imageData->getBuf(0);
               }
               else
               {
                  bands[0] = (ossim_uint8*)imageData->getBuf(0);
                  bands[1] = (ossim_uint8*)imageData->getBuf(1);
                  bands[2] = (ossim_uint8*)imageData->getBuf(2);
                  
               }
               if(bands[0]&&bands[1]&&bands[2])
               {
                  ossim_uint32* bits = (ossim_uint32*)image.bits();
                  ossim_uint32 idx = 0;
                  ossim_uint32 size = imageData->getWidth()*imageData->getHeight();
                  if(bits)
                  {
                     for(idx = 0; idx < size; ++idx)
                     {
                        *bits = (ossim_uint32)((0xff<<24)|
                                               ( (*bands[0])<<16 )|
                                               ( (*bands[1])<<8) |
                                               ( (*bands[2])));
                        ++bits;
                        ++bands[0];
                        ++bands[1];
                        ++bands[2];
                     }
                  }
               }
            }
            return true;
         }
         
         return false;
      }
   virtual QVariant loadResource ( int type, const QUrl & name )
      {
         ossimFilename file;
         if(type == QTextDocument::ImageResource)
         {
            QImage image(QSize(128, 128), QImage::Format_RGB32);
            memset(image.bits(), '\0', image.numBytes());
            ossimFilename filenameToProcess(name.toString().toStdString());
            if(name.scheme() == "http")
            {
               QTemporaryFile tempFile;
               tempFile.open();
               wmsCurlMemoryStream s(name.toString().toStdString());
               QImage correctImage;
               ossimFilename tempFilename(tempFile.fileName().toStdString());
               if(s.download(tempFilename))
               {
                  loadFile(image, tempFilename);
               }
            }
            else if(filenameToProcess.exists())
            {
               loadFile(image, filenameToProcess);
            }
            return QVariant::fromValue(image);
         }
         else if(type == QTextDocument::HtmlResource)
         {
            if((name.scheme() == "http")||
               (name.scheme() == ""))
            {
               if(!ossimPlanetQtBrowser::openUrl(name.toString()))
               {
               }
               
               return QVariant(document()->toHtml());
            }
         }
         return QTextBrowser::loadResource(type, name);
      }
   
protected:
   ossimPlanetNode* theNode;
   osg::ref_ptr<NodeCallback> theCallback;
};

void ossimPlanetQtMainWindow::glWidgetMouseReleaseEvent(QMouseEvent* event)
{

   std::vector<osg::ref_ptr<ossimPlanetViewer::PickObject> > pickedObjects;
   
   if(theGlWidget->viewer()->pickAtWindowCoordinate(pickedObjects, event->x(), theGlWidget->height() - event->y()))
   {
      ossim_uint32 idx = 0;
      for(idx = 0; idx < pickedObjects.size();++idx)
      {
         ossimPlanetNode* layer = pickedObjects[idx]->firstPlanetNode();
         if(layer)
         {
            QDialog* dialog = new QDialog;
            QString s = layer->description().c_str();
			if(s.isEmpty())
			{
				ossimPlanetKmlLayerNode* layerNode = dynamic_cast<ossimPlanetKmlLayerNode*>(layer);
				if(layerNode)
				{
					ossimPlanetKmlFeature* kmlFeature = dynamic_cast<ossimPlanetKmlFeature*>(layerNode->kmlObject().get());
					if(kmlFeature)
					{
						const ossimXmlNode* extendedData = kmlFeature->extendedData();
						ossimXmlNode::ChildListType dataList;
						if(extendedData)
						{
							extendedData->findChildNodes("Data", dataList);
							ossim_uint32 idx = 0;
							if(dataList.size())
							{
								s += "<table border='1'>";
							}
							ossim_uint32 maxCount = ossim::min((ossim_uint64)dataList.size(),
								                               (ossim_uint64)20);
							for(idx = 0; idx < maxCount; ++idx)
							{
								s += "<tr>";
								s += QString("<td>") + dataList[idx]->getAttributeValue("name") + "</td>";
								s += QString("<td>") + dataList[idx]->getChildTextValue("value") + "</td>";
								s += "</tr>";
							}
							if(dataList.size())
							{
								s += "</table>";
							}
						}
					}
				}
			}
            if(!s.isEmpty())
            {
               if(!s.isEmpty())
               {
                  QTextBrowser* browser = new MyBrowser(dialog, layer);
                  browser->setHtml(s);
                  browser->resize(512, 512);
                  dialog->resize(512, 512);
                  dialog->setAttribute(Qt::WA_DeleteOnClose);      
                  dialog->setWindowFlags(dialog->windowFlags()|Qt::WindowStaysOnTopHint);
                  dialog->show();
               }
            }
            return;
         }
      }
   }
}

int ossimPlanetQtMainWindow::ephemerisMembers()
{
   int members = 0;
   if(ossimPlanetQtApplication::sunlightEnabled())
   {
      members |= ossimPlanetEphemeris::SUN_LIGHT;
   }
   if(ossimPlanetQtApplication::skyEnabled())
   {
      members |= ossimPlanetEphemeris::SKY;
   }
   if(ossimPlanetQtApplication::moonEnabled())
   {
      members |= ossimPlanetEphemeris::MOON;
   }
   if(ossimPlanetQtApplication::percentVisibility() < 100)
   {
      members |= ossimPlanetEphemeris::FOG;
   }
   return members;
}
