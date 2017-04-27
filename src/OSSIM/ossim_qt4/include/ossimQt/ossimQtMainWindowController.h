//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtMainWindowController.h 11811 2007-10-05 19:42:38Z dburken $
#ifndef ossimQtMainWindowController_HEADER
#define ossimQtMainWindowController_HEADER
#include <vector>
  // #include <qobject.h>

#include <QtCore/QObject>
#include <QtCore/QCustomEvent>

#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>


class ossimQtGeometryBuilderDialog;
class ossimQtMainWindow;
class ossimDataManager;
class ossimQtMainWindowWorkspace;
class ossimQtDatumConverterDialog;
class ossimQtUnitConverterDialog;
class ossimQtElevationManagerDialog;
class ossimQtDataManagerDialog;
class ossimConnectableObject;
class QDragEnterEvent;
class QDropEvent;
class ossimQtProject;
class ossimQtVceComponentDialog;
class ossimImageHandler;

class ossimQtMainWindowController:public QObject
{
  Q_OBJECT

public:
  ossimQtMainWindowController(ossimQtMainWindow* mainWindow);
  virtual ~ossimQtMainWindowController();

  virtual bool destroyController();

  // basic layer loading
  //
  virtual void openImageFile(const char* filename);
  virtual void openImageFileLayer(bool openFrom=false);
  virtual void displayLayer(ossimConnectableObject* connectable, bool fitToWindowFlag=false);
  virtual void displayLayer(ossim_int32 id, bool fitToWindowFlag=false);
  
  virtual void displayLayerManager();

  

  // drag and drop events
  //
  bool acceptsDragEvent(QDragEnterEvent* event);
  void dropEvent(QDropEvent* event);
  
   virtual bool event(QEvent* e);
   
protected:

   /**
    * Checks the object which should be an ossimImageChain for a valid
    * geometry.  If no good, will prompt the user to set to a default
    * projection.
    */
   virtual void checkForImageGeometry(ossimConnectableObject* obj) const;

   /**
    * This will check to see if image projection is affected by elevation; if
    * so, then checks to see if there is coverage for the upper left image
    * corner and issues a popup warning to user if not.
    *
    * @param obj This should be a "StandardProjectionChain" from a file
    * open.
    */
   void checkElevation(ossimConnectableObject* obj) const;

   /*!
    * Checks for overviews and prompts user if none are present asking to
    * build overviews.  Brings up build overview dialog if user request it.
    */
   virtual void checkForOverviews(ossimConnectableObject* obj) const;

   void buildOverViews(ossimImageHandler* ih) const;
   
   ossimQtMainWindow*             theMainWindow;
   std::vector<ossimQtProject*>   theProjectList;
   ossimQtProject*                theCurrentProject;
   ossimQtElevationManagerDialog* theElevManagerDialog;
   ossimQtUnitConverterDialog*    theUnitConverterDialog;
   ossimQtDatumConverterDialog*   theDatumConverterDialog;
   ossimQtGeometryBuilderDialog*  theGeometryBuilderDialog;
   ossimQtDataManagerDialog*      theLayerManager;
   ossimQtVceComponentDialog*     theVceComponentDialog;
   
protected slots:
   void vceComponentDialogDestroyed();
   void elevationManagerDestroyed();
   void unitConverterDestroyed();
   void datumConverterDestroyed();
   void layerManagerDestroyed();
   void geometryBuilderDialogDestroyed();

public slots:
   virtual void initializeMenus();

   virtual void preferences();

   // vce options
   virtual void vceNewChain();
   virtual void vceComponentDialog();
   
   // display manipulation interface
   virtual void cascadeDisplays();
   virtual void tileDisplays();
   virtual void minimizeAllDisplays();
   virtual void restoreAllDisplays();
   virtual void refreshAllDisplays();
   virtual void closeAllDisplays();
   virtual void maximizeActiveDisplay();
   
   // the utilities interface
   virtual void openElevationManager();
   virtual void openUnitConverter();
   virtual void openDatumConverter();
   virtual void openGeometryBuilder();
   
   // hiogram match
   virtual void histogramMatch();
   
   // fusion layers
   //
   virtual void colorNormalizedFusion();
   virtual void localCorrelationFusion();
   virtual void SFIMFusion();
   
   // combiner layers
   //
   
   virtual void orthoCombiner();
   virtual void simpleCombiner();
   virtual void blendCombiner();
   virtual void bandMergeCombiner();
   virtual void featherCombiner();
   virtual void maxCombiner();
   virtual void closestToCenterCombiner();
   virtual void combinerFactory();
   // correction layers
   //
   virtual void topographicCorrection();
   
   // elevation layers
   //
   virtual void hillShade();
   virtual void elevationNormals();
   
   // project menu
   //
   virtual void openProject();
   virtual void saveProject();
   virtual void saveAsProject();
   virtual void newProject();
   virtual void closeProject();
   virtual void exitProject();

   virtual void displayAboutDialog();
   
protected slots:

   virtual void applyHistogramMatch(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   
   // fusion slots for layer chooser
   //
   virtual void applyColorNormalizedFusion(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyLocalCorrelationFusion(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applySFIMFusion(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyFusionCombiner(const ossimString& className,
                                    ossimConnectableObject::ConnectableObjectList& selectedObjects);
   
   
   // combiner slots for layer chooser
   //
   virtual void applySimpleCombiner(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyBandMergeCombiner(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyFeatherCombiner(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyBlendCombiner(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyOrthoCombiner(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyMaxCombiner(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyClosestToCenterCombiner(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyCombiner(const ossimString& className,
                              ossimConnectableObject::ConnectableObjectList& selectedObjects);
   
   // elevation slots for layer chooser
   //
   virtual void applyHillShade(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   virtual void applyElevationNormals(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   
   // correction slots for layer chooser
   //
   virtual void applyTopographicCorrection(ossimConnectableObject::ConnectableObjectList& selectedObjects);
   
signals:
   void closeFusionSelector();
   void closeCombinerSelector();
   void closeNormalSelector();
   void closeHistogramMatchSelector();


protected:
   virtual void openProject(const ossimFilename& projectFile);
   
};

#endif
