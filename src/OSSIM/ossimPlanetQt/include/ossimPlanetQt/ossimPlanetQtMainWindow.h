#ifndef ossimPlanetQtMainWindow_HEADER
#define ossimPlanetQtMainWindow_HEADER
#define _WINSOCKAPI_
#include <ossimPlanetQt/ui_ossimPlanetQtMainWindow.h>
#include <QtGui/QMainWindow>
#include <QtGui/QKeyEvent>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFrame>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <ossimPlanet/ossimPlanet.h>
#include <ossimPlanet/ossimPlanetTextureLayerGroup.h>
#include <ossimPlanet/ossimPlanetConstants.h>
#include <ossimPlanet/ossimPlanetExtents.h>
#include <ossimPlanet/ossimPlanetIo.h>
#include <ossimPlanet/ossimPlanetIoThread.h>
#include <ossimPlanet/ossimPlanetKmlLayer.h>
#include <ossimPlanet/ossimPlanetLatLonHud.h>
#include <ossimPlanet/ossimPlanetSousaLayer.h>
#include <ossimPlanet/ossimPlanetManipulator.h>
#include <ossimPlanet/ossimPlanetVideoLayer.h>
#include <ossimPlanet/ossimPlanetAnnotationLayer.h>
#include <ossimPlanet/ossimPlanetSousaLayer.h>
#include <ossimPlanet/ossimPlanetLookAt.h>
#include <ossimPlanet/ossimPlanetOperation.h>
#include <list>
#include <ossim/base/ossimWms.h>
#include <ossimPlanetQt/ossimPlanetQtWmsDialog.h>
#include <ossimPlanetQt/ossimPlanetQtMessageLog.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <ossimPlanet/ossimPlanetIoMessageHandler.h>
#include <osg/Timer>
#include <ossimPlanet/ossimPlanetArchive.h>
#include <ossimPlanet/ossimPlanetViewer.h>


class ossimPlanetQtViewer;
class ossimPlanetQtLegend;
class ossimPlanetQtPreferences;
class ossimPlanetQtLayerOperation;
class ossimPlanetQtWmsDialog;
class ossimPlanetQtRuler;
class ossimPlanetQtActivityWindow;
class ossimPlanetQtAboutDialog;
class QTableWidgetItem;
class ossimPlanetQtMainWindow;

class ossimPlanetIoMainWindowMessageHandler : public ossimPlanetIoMessageHandler
{
public:
   ossimPlanetIoMainWindowMessageHandler(ossimPlanetQtMainWindow* mainWindow);
   virtual bool handleMessage(osg::ref_ptr<ossimPlanetMessage> message);

protected:
   ossimPlanetQtMainWindow* theMainWindow;
};

class OSSIMPLANETQT_DLL ossimPlanetQtMainWindow : public QMainWindow, public Ui::ossimPlanetQtMainWindow
{
   Q_OBJECT
public:
   class ViewerCallback : public ossimPlanetViewerCallback
      {
      public:
         ViewerCallback(ossimPlanetQtMainWindow* main):theMainWindow(main){}
         virtual void viewChanged(ossimPlanetViewer* /*viewer*/){
            theMainWindow->viewChanged();
         }

      protected:
         ossimPlanetQtMainWindow* theMainWindow;
      };
	class NodeCallback : public ossimPlanetNodeCallback
		{
		public:
			NodeCallback(ossimPlanetQtMainWindow* mw);
			void setMainWindow(ossimPlanetQtMainWindow* mw){theMainWindow=mw;}
			ossimPlanetQtMainWindow* mainWindow(){return theMainWindow;}
			virtual void nodeAdded(osg::Node* node);
			virtual void nodeRemoved(osg::Node* node);
			
		protected:
			ossimPlanetQtMainWindow* theMainWindow;
		};
	
   ossimPlanetQtMainWindow(QWidget *parent = 0);
   virtual ~ossimPlanetQtMainWindow();
   osg::ref_ptr<ossimPlanet> planet();
   const osg::ref_ptr<ossimPlanet> planet()const;
   const osg::ref_ptr<ossimPlanetKmlLayer> kmlLayer()const;
   const osg::ref_ptr<ossimPlanetVideoLayer> videoLayer()const;
   osg::ref_ptr<ossimPlanetKmlLayer> kmlLayer();
   osg::ref_ptr<ossimPlanetVideoLayer> videoLayer();
   osg::ref_ptr<ossimPlanetAnnotationLayer> annotationLayer();
   ossim_uint32 textureLayerGroupListSize()const;
   osg::ref_ptr<ossimPlanetTextureLayerGroup> textureLayerGroup(ossim_uint32 idx=0);
   const osg::ref_ptr<ossimPlanetTextureLayerGroup> textureLayerGroup(ossim_uint32 idx=0)const;
   ossimPlanetQtLegend* legend();
   const ossimPlanetQtLegend* legend()const;
   ossimPlanetQtViewer* glWidget();
   ossimPlanetManipulator* manipulator();
   ossimPlanetQtActivityWindow* activityWindow();
   const ossimPlanetQtActivityWindow* activityWindow()const;
   
   void gotoLocationNadir(double lat, double lon, double height);
   void refreshLandTextures(osg::ref_ptr<ossimPlanetExtents> extents = 0,
                            ossimPlanetLandRefreshType refreshType=ossimPlanetLandRefreshType_PRUNE);
   const osg::ref_ptr<ossimPlanetLookAt> getLookAt()const;
   
   void saveSession(const QString& filename);
   void loadSession(const QString& filename);
   virtual void dropEvent ( QDropEvent * event );   
   virtual void dragEnterEvent(QDragEnterEvent *event);

   osg::ref_ptr<ossimPlanetIoThread> iothread()
   {
      return theIoThread;
   }
   void sendMessage(const std::string& id,
                    const std::string& message,
                    const ossimString& messageType);
	void addOperation(osg::ref_ptr<ossimPlanetOperation> operation);
   virtual bool event(QEvent* e);
	
public slots:
   void on_actionActivity_triggered(bool checked = false);
   void on_actionMessageLog_triggered(bool checked = false);
   void wmsSyncCurrentSettings();
   void wmsSyncAllSettings();
   void on_viewToggleFullScreen_triggered(bool checked = false);
 
private slots:
   void on_fileSaveViewAsJpeg_triggered(bool checked = false);
   void on_fileSaveViewAsPng_triggered(bool checked = false);
   void on_fileSaveViewAsTiff_triggered(bool checked = false);
   void on_fileSaveAnimationPath_triggered(bool checked = false);
   void on_fileLoadAnimationPath_triggered(bool checked = false);
   void on_viewBookmark_triggered( bool checked = false );
   void on_viewGotoLatLon_triggered( bool checked = false );
   void on_viewGotoAddress_triggered( bool checked = false );
   void on_viewStartRecordingAnimationPath_triggered( bool checked = false );
   void on_viewStopRecordingAnimationPath_triggered( bool checked = false );
   void on_viewPlayAnimationPath_triggered( bool checked = false );
   void on_fileQuit_triggered( bool checked = false );
   void on_fileOpenImage_triggered( bool checked = false );
   void on_fileOpenVideo_triggered( bool checked = false );
   void on_fileOpenWms_triggered( bool checked = false );
   void on_fileOpenKml_triggered( bool checked = false );
   void on_openSession_triggered( bool checked = false );
   void on_saveSession_triggered( bool checked = false );
   void on_saveSessionAs_triggered( bool checked = false );
   void on_filePreferences_triggered(bool checked = false);
   void on_editLayerOperation_triggered(bool checked = false);
   void on_viewRefreshTextures_triggered(bool checked = false);
   void on_viewRefreshAll_triggered(bool checked = false);
   void on_viewSyncLocation_triggered(bool checked = false);
   void on_toolsRuler_triggered( bool checked = false );
   void on_helpAbout_triggered( bool checked = false );
   void serverPortItemChanged(QTableWidgetItem* item);
   void addPort(bool checked = false);
   void deletePort(bool checked = false);
   void addClientPort(bool checked = false);
   void deleteClientPort(bool checked = false);
   void clientItemChanged(QTableWidgetItem* item);
   void enableAllClientPorts(bool checked = false);
   void disableAllClientPorts(bool checked = false);
   void viewTransmitTextChanged(const QString& value);
   void autoViewSyncFlagChanged(int state);

   // added by russc 4/30/08
   void archiveMappingWidgetItemChanged(QTableWidgetItem* item);
   void archiveAddButtonClicked(bool checked = false);
   void archiveRemoveButtonClicked(bool checked = false);
   void sousaIdentitySetButtonClicked(bool checked = false);
   void sousaServerConnectButtonClicked(bool checked = false);
   void sousaServerAutoConnectOnStartClicked(bool checked = false);
protected slots:
   void preferencesDestroyed(QObject* obj);
   void wmsAddButtonHit(ossimPlanetQtWmsDialog* wms,
                        ossimRefPtr<ossimWmsCapabilitiesDocument> capabilities,
                        ossimPlanetQtWmsDialog::SelectedLayerInfoListType& layers);
   void layerOperationCloseButtonClicked(bool checked=false);
   void rulerCloseButtonClicked(bool checked=false);
   void closeActivity(bool checked = false);
   void deleteSelectedActivities(bool checked = false);
   void wmsDialogDestroyed(QObject* obj);
/*    void newMessage(QString message, */
/*                    int port); */
/*    void geospatialReadServer(); */

public slots:
   void setWmsTimeout(const QString& text);
   void setStagingCacheDirectory(const QString& text);
   void setAutoHistogramStretchSetting(const QString& text);
   void stagingCacheDirectoryButtonClicked(bool checked = false);
   void setHudEnabledFlag(bool flag = false);
   void setAutomaticStagingEnabled(bool flag = false);
   void setArchiveMappingEnabledFlag(bool flag = false);
   void setAutoHighlightLineOfSiteFlag(bool flag = false);
   void setElevationEnabledFlag(bool flag = false);
   void setTextureDetailType(int value);
   void setMeshDetailType(int value);
   void setTerrainCullingType(int value);
   void setHeightExageration(const QString& height);
   
   void setEphemerisEnabledFlag(bool flag);
   void setEphemerisAutoUpdateToCurrentTimeFlag(bool flag);
   void setSkyEnabledFlag(bool flag);
   void setMoonEnabledFlag(bool flag);
   void setSunlightEnabledFlag(bool flag);
   void setPercentVisibility(int value);
   void setCloudsEnabledFlag(bool flag);
   void setCloudAmount(int value);
   void setCloudSharpness(int value);
   void viewChanged();
   void glWidgetDropEvent(QDropEvent * event);
   void glWidgetDragEnterEvent(QDragEnterEvent *event);
   void glWidgetMouseMoveEvent(QMouseEvent* event);
   void glWidgetMouseReleaseEvent(QMouseEvent* event);
protected:
   class SocketInfo : public osg::Referenced
   {
   public:
      SocketInfo(const std::string& name="",
                 const std::string& host="",
                 const std::string& port="",
                 const std::string& portType="")
         :theName(name),
         theHost(host),
         thePort(port),
         thePortType(portType),
         theValidFlag(false)
         {
         }

      ossimString theName;
      ossimString theHost;
      ossimString thePort;
      ossimString thePortType;
      osg::ref_ptr<ossimPlanetIo> theIo;
      bool theValidFlag;
   };

   void loadServerPortFromPreferences();
   void loadClientPortFromPreferences();
   void loadArchiveMappingFromPreferences();
   void saveServerPortPreferences();
   void saveClientPortPreferences();
   void saveArchiveMappingPreferences();
   void saveSousaConnectionPreferences();
   
   virtual void keyPressEvent ( QKeyEvent * event );
   virtual void keyReleaseEvent ( QKeyEvent * event );
/*    void createLegend(); */
   void populatePreferences();
   void populateServerPortPreferences();
   void populateClientPortPreferences();
   void populateSousaConnectionPreferences();
   // added by russc
   void populateArchiveMappingPreferences();

/*    osg::Node* createBounds(const ossimPlanetTextureLayerExtents& extents)const; */
   virtual void timerEvent ( QTimerEvent * event );
   QString getSaveFile(const QString& title, const QString& extension, const QString& filters="", const QString& defaultDir = "");
   std::string wmsSettingsToXml(const QString& connectionName)const;
   void init();
   int ephemerisMembers();
   ossimPlanetQtViewer* theGlWidget;
   osg::ref_ptr<ossimPlanet> thePlanet;
   std::vector<osg::ref_ptr<ossimPlanetTextureLayerGroup> > theTextureLayers;
   osg::ref_ptr<ossimPlanetTextureLayerGroup> theOverlayGroup;
   QFrame*              theFullScreenFrame;
   ossimPlanetQtPreferences* thePreferences;
   ossimPlanetQtLayerOperation* theLayerOperation;
   ossimPlanetQtRuler*          theRuler;
   ossimPlanetQtActivityWindow* theActivityWindow;
   ossim_uint32 theLargestLogMessageSize;
   ossimPlanetQtWmsDialog*      theWmsWindow;   
	ossimPlanetQtMessageLog*     theMessageLogDialog;
   QDesktopWidget*              theDesktopWidget;
   int                          theTimerId;
   QRect                        theFullScreenGeometry;
   bool                         theIsFullScreen;
   ossimFilename                theSessionFilename;
   bool                         theSessionDirtyFlag;
   double                       theAutoViewSyncRateInSeconds;
   double                       theAutoViewSyncFlag;
   osg::Timer_t                 theLastAutoViewSyncTick;
   osg::Vec3d                   theStartPoint;
   osg::Vec3d                   theEndPoint;
   osg::ref_ptr<ossimPlanetKmlLayer>         theKmlLayer;
   osg::ref_ptr<ossimPlanetVideoLayer>       theVideoLayer;
   osg::ref_ptr<ossimPlanetSousaLayer>       theSousaLayer;
   osg::ref_ptr<ossimPlanetAnnotationLayer>  theAnnotationLayer;
   osg::ref_ptr<ossimPlanetLatLonHud>        theHudLayer;
   osg::ref_ptr<ossimPlanetTextureLayerCallback> theLayerListener;
   osg::ref_ptr<ViewerCallback> theViewerCallback;
   osg::ref_ptr<ossimPlanetManipulator> theManipulator;
   osg::ref_ptr<ossimPlanetIoThread>                               theIoThread;
   std::vector<osg::ref_ptr<ossimPlanetQtMainWindow::SocketInfo> > theServerList;
   std::vector<osg::ref_ptr<ossimPlanetQtMainWindow::SocketInfo> > theClientList;
  
   mutable bool theUpdateHiglightsFlag;
   bool theAutoHighlightLineOfSiteFlag;

   ossimFilename theLastViewSaveDirectory;

   osg::ref_ptr<ossimPlanetIoMainWindowMessageHandler> theIoMessageHandler;
   
   // added by russc 5/6/08
   osg::ref_ptr<ossimPlanetArchive> theArchive;

   bool theInitializedFlag;
	osg::ref_ptr<ossimPlanetQtMainWindow::NodeCallback> theNodeCallback;
   
   ossimString theDefaultNavigationPortType;
   ossimString theDefaultDataPortType;
   
   ossimFilename theCurrentOpenImagePath;
   ossimFilename theCurrentOpenVideoPath;
   ossimFilename theCurrentOpenKmlPath;
   ossimFilename theCurrentOpenSessionPath;
   ossimFilename theLastOpenedPath;
   
   double theMaxVisibility;
   ossim_uint64 theCloudSeed;
};

#endif
