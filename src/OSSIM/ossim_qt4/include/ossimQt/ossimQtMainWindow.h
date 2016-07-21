#ifndef OSSIMQTMAINWINDOW_H
#define OSSIMQTMAINWINDOW_H

#include <QtCore/QVariant>

class ossimQtMainWindowController;

#include <Qt3Support/Q3MainWindow>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>

class Ui_ossimQtMainWindow
{
public:
    QAction *projectOpenAction;
    QAction *projectSaveAction;
    QAction *projectSaveAsAction;
    QAction *projectExitAction;
    QAction *editUndoAction;
    QAction *editRedoAction;
    QAction *editCutAction;
    QAction *editCopyAction;
    QAction *editPasteAction;
    QAction *editFindAction;
    QAction *helpContentsAction;
    QAction *helpIndexAction;
    QAction *helpAboutAction;
    QAction *windowCascade;
    QAction *windowTile;
    QAction *windowMinimizeAll;
    QAction *windowCloseAll;
    QAction *windowMaximize;
    QAction *utilitiesDatumConverterAction;
    QAction *utilitiesElevationManagerAction;
    QAction *utilitiesUnitConverterAction;
    QAction *layerImportImageFileAction;
    QAction *layerOpenFileAction;
    QAction *layerOpenFromAction;
    QAction *windowRestoreAllAction;
    QAction *windowRefreshAllAction;
    QAction *layerManager;
    QAction *projectPreferencesAction;
    QAction *projectNewAction;
    QAction *projectCloseAction;
    QAction *layerHistogramMatchAction;
    QAction *vceNewChainAction;
    QAction *vceComponentDialogAction;
    QAction *layerCombinerClosestPtAction;
    QAction *layerCombineAction;
    QAction *layerFusionAction;
    QAction *layerCorrectionAction;
    QAction *layerElevationAction;
    QAction *layerFusionLocalCorrelationAction;
    QAction *layerFusionSFIMAction;
    QAction *utilitiesGeometry_BuilderAction;
    QActionGroup *layerFusionActionGroup;
    QAction *layerFusionColorNormalizedAction;
    QActionGroup *layerRadiometricCorrectionActionGroup;
    QAction *layerCorrectionAtmosphericCorrectionAction;
    QAction *layerCorrectionTopographicAction;
    QActionGroup *layerElevationActionGroup;
    QAction *layerElevationHillShadeAction;
    QAction *layerElevationNormalsAction;
    QActionGroup *layerCombineActionGroup;
    QAction *layerCombineMosaicAction;
    QAction *layerCombineBlendAction;
    QAction *layerCombineFeatherAction;
    QAction *layerCombineBandMergeAction;
    QAction *layerCombineOrthoAction;
    QAction *layerCombinerMaxAction;
    QAction *layerCombinerFactoryAction;
    QWidget *widget;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *PopupMenu;
    QMenu *layerCombineMenu;
    QMenu *layerFusionMenu;
    QMenu *layerCorrectionMenu;
    QMenu *layerElevationMenu;
    QMenu *PopupMenu_4;
    QMenu *PopupMenu_5;
    QMenu *PopupMenu_2;
    QMenu *helpMenu;

    void setupUi(Q3MainWindow *ossimQtMainWindow);
    void retranslateUi(Q3MainWindow *ossimQtMainWindow);
};

namespace Ui {
    class ossimQtMainWindow: public Ui_ossimQtMainWindow {};
} // namespace Ui
#include "ossimQtExport.h"
class  OSSIMQT_DLL ossimQtMainWindow : public Q3MainWindow, public Ui::ossimQtMainWindow
{
    Q_OBJECT

public:
    ossimQtMainWindow(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~ossimQtMainWindow();

    virtual ossimQtMainWindowController * getController();
    virtual bool event( QEvent * evt );

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
    ossimQtMainWindowController *theController;

protected slots:
    virtual void languageChange();

private:
    void destroy();
    void init();

};

#endif // OSSIMQTMAINWINDOW_H
