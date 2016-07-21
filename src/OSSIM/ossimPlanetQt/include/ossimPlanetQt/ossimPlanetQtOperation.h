#ifndef ossimPlanetQtOperation_HEADER
#define ossimPlanetQtOperation_HEADER
#include <ossimPlanet/ossimPlanetOperation.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <ossim/base/ossimProcessListener.h>
#include <ossimPlanet/ossimPlanetOssimImageLayer.h>
#include <ossim/base/ossimProcessListener.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimReferenced.h>
#include <ossimPlanet/ossimPlanetKml.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <OpenThreads/Mutex>
#include <QtCore/Qt>
#include <QtCore/QUrl>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QList>
#include <osg/ref_ptr>
class ossimPlanetQtLegend;
class ossimPlanetQtLegendTextureItem;
class ossimPlanetQtActivityWidget;
class ossimPlanetQtMainWindow;

class OSSIMPLANETQT_DLL  ossimPlanetQtOperation : public ossimPlanetOperation
{
public:
	ossimPlanetQtOperation();
   void setActivityWidget(ossimPlanetQtActivityWidget* w)
   {
      theWidget = w;
   }
   virtual void updateGui(){}
protected:
	void postMessage(const QString& msg,
						  int type)const;
	void adjustToWriteableLocation(ossimFilename& file)const;
	void adjustToStagerLocation(ossimFilename& file)const;
   ossimPlanetQtActivityWidget* theWidget;
};

class OSSIMPLANETQT_DLL ossimPlanetQtOssimImageStagerOperation : public ossimPlanetQtOperation,
                                                               public ossimProcessListener
{
public:
	ossimPlanetQtOssimImageStagerOperation(ossimPlanetOssimImageLayer* layer, ossimPlanetQtLegend* legned = 0);
   virtual void run();
	
protected:
   virtual void processProgressEvent(ossimProcessProgressEvent& event);
	void buildOverviews(ossimImageHandler* handler);
	void buildHistograms(ossimImageHandler* handler);
	osg::ref_ptr<ossimPlanetOssimImageLayer> theLayer;
	OpenThreads::Mutex theStagerMutex;
	ossimPlanetQtLegend* theLegend;
	ossimFilename        theFilename;   
   ossimString          theStagerMessage;
	ossimProcessInterface* theCurrentProcessInterface;
};

class OSSIMPLANETQT_DLL ossimPlanetQtOpenImageFileOperation : public ossimPlanetQtOperation
{
public:
	ossimPlanetQtOpenImageFileOperation(const QString& file,
													 ossimPlanetQtLegend* legend);
   virtual void run();
protected:
   std::vector<ossimString>                            theFileList;
   ossimPlanetQtLegend*                                theLegend;
   osg::ref_ptr<ossimPlanetExtents>                    theExtents;
   std::vector<osg::ref_ptr<ossimPlanetTextureLayer> > theLayers;
   bool                                                theInitializeGuiFlag;
   ossim_uint32                                        theCurrentFileIdx;
};

class OSSIMPLANETQT_DLL ossimPlanetQtOpenImageFileFromLayerOperation : public ossimPlanetQtOperation
{
public:
   ossimPlanetQtOpenImageFileFromLayerOperation(osg::ref_ptr<ossimPlanetOssimImageLayer> layer,
                                               const ossimFilename& name,
                                               ossim_int32 entry);
   virtual void run();
protected:
	osg::ref_ptr<ossimPlanetOssimImageLayer> theLayer;
   osg::ref_ptr<ossimPlanetLookAt> theSavedLookAt;
   ossimString          theName;
   ossimString          theDescription;
   ossimFilename        theFilename;
   ossim_int32          theEntry;
   bool                 theInitializeGuiFlag;
	
};

class OSSIMPLANETQT_DLL ossimPlanetQtOpenImageFileFromLayerXmlOperation : public ossimPlanetQtOperation
{
public:
   ossimPlanetQtOpenImageFileFromLayerXmlOperation(osg::ref_ptr<ossimPlanetOssimImageLayer> layer,
                                                  ossimRefPtr<ossimXmlNode> node,
                                                  ossimPlanetQtLegend* legend);
   virtual void run();
   void setItem(ossimPlanetQtLegendTextureItem* item);
protected:
	osg::ref_ptr<ossimPlanetOssimImageLayer> theLayer;
   ossimRefPtr<ossimXmlNode> theXmlNode;
   ossimPlanetQtLegend* theLegend;
   ossimPlanetQtLegendTextureItem* theItem;
   bool                 theInitializeGuiFlag;
   ossimFilename        theFilename;
   ossimString          theName;
   ossimString          theDescription;
	
};

#endif
