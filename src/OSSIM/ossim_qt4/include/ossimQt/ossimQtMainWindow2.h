//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt.
//
// Author:  David Burken
//
// Description: Rewrite of the Qt3 imagelinker main window written by
// Garrett Potts.
//
// $Id: ossimQtMainWindow2.h 11056 2007-05-26 18:38:19Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimQtMainWindow2_HEADER 
#define ossimQtMainWindow2_HEADER

#include <QtGui/QMainWindow>

class QCustomEvent;
class ossimQtMainWindowController;

class ossimQtMainWindow2 : public QMainWindow
{
   Q_OBJECT

public:

   ossimQtMainWindow2(QWidget* parent = 0,
                      Qt::WindowFlags fl = Qt::WType_TopLevel);
   
   virtual ~ossimQtMainWindow2();

   virtual ossimQtMainWindowController * getController();

public slots:
    virtual void dragEnterEvent( QDragEnterEvent * event );
    virtual void dropEvent( QDropEvent * event );
    virtual void windowCascade_activated();
    virtual void windowTile_activated();
    virtual void windowMinimizeAll_activated();
    virtual void windowCloseAll_activated();
    virtual void ActionwindowMaximize_activated();
    virtual void projectExitAction_activated();
    virtual void projectOpenAction_activated();
    virtual void projectSaveAction_activated();
    virtual void projectSaveAsAction_activated();
    virtual void projectNewAction_activated();
    virtual void projectCloseAction_activated();
    virtual void utilitiesElevationManagerAction_activated();
    virtual void utilitiesUnitConverterAction_activated();
    virtual void utilitiesDatumConverterAction_activated();
    virtual void layerImportFileAction_activated();
    virtual void layerCombineLayersAction_activated();
    virtual void layerOpenFileAction_activated();
    virtual void layerOpenFromAction_activated();
    virtual void displayLayerAction_activated();
    virtual void customEvent( QCustomEvent * evt );
    virtual void windowRestoreAllAction_activated();
    virtual void windowRefreshAllAction_activated();
    virtual void ossimQtMainWindow_usesTextLabelChanged( bool );
    virtual void layerManager_activated();
    virtual void layerFusionColorNormalizedAction_activated();
    virtual void layerFusionLocal_CorrelationAction_activated();
    virtual void layerCombineBandMergeAction_activated();
    virtual void layerCombineBlendAction_activated();
    virtual void layerCombineFeatherAction_activated();
    virtual void layerCombineMosaicAction_activated();
    virtual void layerCombineOrthoAction_activated();
    virtual void layerElevationHillShadeAction_activated();
    virtual void layerElevationNormalsAction_activated();
    virtual void layerCorrectionTopographicAction_activated();
    virtual void layerHistogramMatchAction_activated();
    virtual void vceNewChainAction_activated();
    virtual void vceComponentDialogAction_activated();
    virtual void projectPreferencesAction_activated();
    virtual void helpAboutActionActivated();
    virtual void layerCombinerMaxActionActivated();
    virtual void layerCombinerFactoryActionActivated();
    virtual void layerCombinerClosestPtAction_activated();
    virtual void layerFusionLocalCorrelationAction_activated();
    virtual void layerFusionSFIMAction_activated();
    virtual void utilitiesGeometry_BuilderAction_activated();

protected:
    ossimQtMainWindowController* theController;

protected slots:
    virtual void languageChange();

private:
    void destroy();
    void init();

    
};
#endif /* #ifndef ossimQtMainWindow2_HEADER */
