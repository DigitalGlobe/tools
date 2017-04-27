#ifndef ossimPlanetQtActivityItem_HEADER
#define ossimPlanetQtActivityItem_HEADER
#include <QtGui/QTreeWidgetItem>
#include <QtCore/Qt>
#include <QtCore/QUrl>
#include <QtCore/QStringList>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QList>
#include <osg/ref_ptr>
#include <ossimPlanet/ossimPlanetOssimImageLayer.h>
#include <ossim/base/ossimProcessListener.h>
#include <ossim/base/ossimReferenced.h>
#include <ossimPlanet/ossimPlanetKml.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class ossimPlanetQtActivityWidget;
class ossimPlanetOssimImageLayer;
class ossimPlanetQtLegend;
class ossimPlanetQtLegendTextureItem;
class ossimImageHandler;

class OSSIMPLANETQT_DLL ossimPlanetQtActivityItem : public QObject, public QTreeWidgetItem
{
public:
   enum State
   {
      READY     = 0,
      EXECUTING = 1,
      FINISHED  = 2,
      CANCELED  = 4
   };
   ossimPlanetQtActivityItem(QTreeWidget* parent, int type = Type);
   ossimPlanetQtActivityItem(QTreeWidgetItem* parent, int type = Type);
   ossimPlanetQtActivityItem(int type = Type);
   virtual void execute()=0;
   virtual void updateGui()=0;
   void setThread(QThread* thread);
   void setState(ossimPlanetQtActivityItem::State state);
   ossimPlanetQtActivityItem::State state()const;
   const ossimString& errorMessage()const;
   
public:
   QThread*                         theThread;
   mutable QMutex                   theMutex;
   ossimPlanetQtActivityItem::State theState;
   ossimString                      theErrorMessage;
   bool                             theInitializeGuiFlag;
};

class OSSIMPLANETQT_DLL ossimPlanetQtOssimImageStagerActivity : public ossimPlanetQtActivityItem,
                                                public ossimProcessListener
{
public:
   ossimPlanetQtOssimImageStagerActivity(QTreeWidget * parent, ossimPlanetOssimImageLayer* layer, int type = Type);
   ossimPlanetQtOssimImageStagerActivity(QTreeWidgetItem * parent, ossimPlanetOssimImageLayer* layer, int type = Type);
   ossimPlanetQtOssimImageStagerActivity(ossimPlanetOssimImageLayer* layer, ossimPlanetQtLegend* legend, int type = Type);
	
   virtual void execute();
   virtual void updateGui();

   virtual void processProgressEvent(ossimProcessProgressEvent& event);
   
protected:
	void adjustToWriteableLocation(ossimFilename& file)const;
	void buildOverviews(ossimImageHandler* handler);
	void buildHistograms(ossimImageHandler* handler);
    osg::ref_ptr<ossimPlanetOssimImageLayer> theLayer;
    ossimPlanetQtLegend* theLegend;
    std::string          theStatus;
    ossimFilename        theFilename;   
  
};

class OSSIMPLANETQT_DLL ossimPlanetQtOpenImageFilesActivity : public ossimPlanetQtActivityItem
{
public:
   ossimPlanetQtOpenImageFilesActivity(const QList<QUrl>& urls,
                                       ossimPlanetQtLegend* legend);
   ossimPlanetQtOpenImageFilesActivity(const QStringList& urls,
                                       ossimPlanetQtLegend* legend);
   virtual void execute();
   virtual void updateGui();

protected:
   std::vector<ossimString>                            theFileList;
   std::vector<ossimString>                            theFileStatus;
   ossimPlanetQtLegend*                                theLegend;
   osg::ref_ptr<ossimPlanetExtents>                    theExtents;
   std::vector<osg::ref_ptr<ossimPlanetTextureLayer> > theLayers;
   ossimString                                         theStatus;
   bool                                                theInitializeGuiFlag;
   ossim_uint32                                        theCurrentFileIdx;
   ossim_uint32                                        theGuiFileItemIndex;
};

class OSSIMPLANETQT_DLL ossimPlanetQtOpenImageFileFromLayerActivity : public ossimPlanetQtActivityItem
{
public:
   ossimPlanetQtOpenImageFileFromLayerActivity(osg::ref_ptr<ossimPlanetOssimImageLayer> layer,
                                               const ossimFilename& name,
                                               ossim_int32 entry,
                                               ossimPlanetQtLegend* legend);
   virtual void execute();
   virtual void updateGui();
   void setItem(ossimPlanetQtLegendTextureItem* item);
protected:
	osg::ref_ptr<ossimPlanetOssimImageLayer> theLayer;
   osg::ref_ptr<ossimPlanetLookAt> theSavedLookAt;
   ossimString          theName;
   ossimString          theDescription;
   ossimFilename        theFilename;
   ossim_int32          theEntry;
   ossimPlanetQtLegend* theLegend;
   ossimPlanetQtLegendTextureItem* theItem;
   bool                 theInitializeGuiFlag;
   ossimString          theStatus;

};

class OSSIMPLANETQT_DLL ossimPlanetQtOpenImageFileFromLayerXmlActivity : public ossimPlanetQtActivityItem
{
public:
   ossimPlanetQtOpenImageFileFromLayerXmlActivity(osg::ref_ptr<ossimPlanetOssimImageLayer> layer,
                                                  ossimRefPtr<ossimXmlNode> node,
                                                  ossimPlanetQtLegend* legend);
   virtual void execute();
   virtual void updateGui();
   void setItem(ossimPlanetQtLegendTextureItem* item);
protected:
	osg::ref_ptr<ossimPlanetOssimImageLayer> theLayer;
   ossimRefPtr<ossimXmlNode> theXmlNode;
   ossimPlanetQtLegend* theLegend;
   ossimPlanetQtLegendTextureItem* theItem;
   bool                 theInitializeGuiFlag;
   ossimString          theStatus;
   ossimFilename        theFilename;
   ossimString          theName;
   ossimString          theDescription;

};

class OSSIMPLANETQT_DLL ossimPlanetQtMessageActivity : public ossimPlanetQtActivityItem
{
public:
   ossimPlanetQtMessageActivity(const std::list<QString>& messageList,
                                ossimPlanetQtLegend* legend);

   virtual void execute();
   virtual void updateGui();
   
protected:
   std::list<QString> theMessageList;
   ossimPlanetQtLegend* theLegend;
   std::list<osg::ref_ptr<ossimPlanetKmlObject> > theResultList;
};
#endif
