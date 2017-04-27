#include <ossimQt/ossimQtMainWindow.h>

// #include <qvariant.h>
// // #include <qapplication.h>
// #include <qimage.h>
// #include <qpixmap.h>

#include <QtGui/QDragEnterEvent>

#include <ossimQt/ossimQtMainWindowController.h>

/*
 *  Constructs a ossimQtMainWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
ossimQtMainWindow::ossimQtMainWindow(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
	theController = 0;
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtMainWindow::~ossimQtMainWindow()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtMainWindow::languageChange()
{
    retranslateUi(this);
}

void ossimQtMainWindow::dragEnterEvent(QDragEnterEvent* event)
{
  event->accept(theController->acceptsDragEvent(event));
}

void ossimQtMainWindow::dropEvent(QDropEvent* event)
{
  theController->dropEvent(event);
}

void ossimQtMainWindow::destroy()
{
    if(theController)
    {
	delete theController;
	theController=NULL;
    }
}

void ossimQtMainWindow::init()
{
    theController = new ossimQtMainWindowController(this);
}

ossimQtMainWindowController* ossimQtMainWindow::getController()
{
  return theController;
}

void ossimQtMainWindow::windowCascade_activated()
{
  theController->cascadeDisplays();
}


void ossimQtMainWindow::windowTile_activated()
{
  theController->tileDisplays();
}


void ossimQtMainWindow::windowMinimizeAll_activated()
{
  theController->minimizeAllDisplays();
}


void ossimQtMainWindow::windowCloseAll_activated()
{
  theController->closeAllDisplays();
}


void ossimQtMainWindow::ActionwindowMaximize_activated()
{
  theController->maximizeActiveDisplay();
}


void ossimQtMainWindow::projectExitAction_activated()
{
    theController->exitProject();
}


void ossimQtMainWindow::projectOpenAction_activated()
{
    theController->openProject();
}


void ossimQtMainWindow::projectSaveAction_activated()
{
    theController->saveProject();
}


void ossimQtMainWindow::projectSaveAsAction_activated()
{
    theController->saveAsProject();
}

void ossimQtMainWindow::projectNewAction_activated()
{
    theController->newProject();
}

void ossimQtMainWindow::projectCloseAction_activated()
{
    theController->closeProject();
}

void ossimQtMainWindow::utilitiesElevationManagerAction_activated()
{
  theController->openElevationManager();
}


void ossimQtMainWindow::utilitiesUnitConverterAction_activated()
{
  theController->openUnitConverter();
}


void ossimQtMainWindow::utilitiesDatumConverterAction_activated()
{
  theController->openDatumConverter();
}


void ossimQtMainWindow::layerImportFileAction_activated()
{

}


void ossimQtMainWindow::layerCombineLayersAction_activated()
{

}


void ossimQtMainWindow::layerOpenFileAction_activated()
{
  theController->openImageFileLayer();
}

void ossimQtMainWindow::layerOpenFromAction_activated()
{
  theController->openImageFileLayer(true);
}

void ossimQtMainWindow::displayLayerAction_activated()
{
}

bool ossimQtMainWindow::event(QEvent* evt)
{
	if(!theController) return Q3MainWindow::event(evt);

   if(!theController->event(evt))
   {
	   return Q3MainWindow::event(evt);
   }//QApplication::sendEvent(theController, evt);
   return true;
}


void ossimQtMainWindow::windowRestoreAllAction_activated()
{
    theController->restoreAllDisplays();
}


void ossimQtMainWindow::windowRefreshAllAction_activated()
{
    theController->refreshAllDisplays();
}


void ossimQtMainWindow::ossimQtMainWindow_usesTextLabelChanged( bool )
{

}


void ossimQtMainWindow::layerManager_activated()
{
    theController->displayLayerManager();
}



void ossimQtMainWindow::layerFusionColorNormalizedAction_activated()
{
    theController->colorNormalizedFusion();
}

void ossimQtMainWindow::layerFusionLocal_CorrelationAction_activated()
{
   if(theController)
   {
      theController->localCorrelationFusion();
   }
}

void ossimQtMainWindow::layerCombineBandMergeAction_activated()
{
    theController->bandMergeCombiner();
}


void ossimQtMainWindow::layerCombineBlendAction_activated()
{
    theController->blendCombiner();
}


void ossimQtMainWindow::layerCombineFeatherAction_activated()
{
    theController->featherCombiner();
}


void ossimQtMainWindow::layerCombineMosaicAction_activated()
{
    theController->simpleCombiner();
}

void ossimQtMainWindow::layerCombineOrthoAction_activated()
{
    theController->orthoCombiner();
}

void ossimQtMainWindow::layerElevationHillShadeAction_activated()
{
    theController->hillShade();
}


void ossimQtMainWindow::layerElevationNormalsAction_activated()
{
    theController->elevationNormals();
}


void ossimQtMainWindow::layerCorrectionTopographicAction_activated()
{
    theController->topographicCorrection();
}


void ossimQtMainWindow::layerHistogramMatchAction_activated()
{
    theController->histogramMatch();
}


void ossimQtMainWindow::vceNewChainAction_activated()
{
    theController->vceNewChain();
}


void ossimQtMainWindow::vceComponentDialogAction_activated()
{
   theController->vceComponentDialog();
}


void ossimQtMainWindow::projectPreferencesAction_activated()
{
    if(theController)
    {
	theController->preferences();
    }
}


void ossimQtMainWindow::helpAboutActionActivated()
{
    if (theController)
    {
	theController->displayAboutDialog();
    }
}


void ossimQtMainWindow::layerCombinerMaxActionActivated()
{
    if(theController)
    {
	theController->maxCombiner();
    }
}


void ossimQtMainWindow::layerCombinerFactoryActionActivated()
{
    if(theController)
    {
	theController->combinerFactory();
    }
}


void ossimQtMainWindow::layerCombinerClosestPtAction_activated()
{
    if(theController)
    {
	theController->closestToCenterCombiner();
    }

}


void ossimQtMainWindow::layerFusionLocalCorrelationAction_activated()
{
    if(theController)
    {
	theController->localCorrelationFusion();
    }
}


void ossimQtMainWindow::layerFusionSFIMAction_activated()
{
    if(theController)
    {
	theController->SFIMFusion();
    }
}


void ossimQtMainWindow::utilitiesGeometry_BuilderAction_activated()
{
    if(theController)
    {
	theController->openGeometryBuilder();
    }
}

void Ui_ossimQtMainWindow::setupUi(Q3MainWindow *ossimQtMainWindow)
{
   if (ossimQtMainWindow->objectName().isEmpty())
      ossimQtMainWindow->setObjectName(QString::fromUtf8("ossimQtMainWindow"));
   ossimQtMainWindow->resize(683, 665);
   ossimQtMainWindow->setAcceptDrops(true);
   projectOpenAction = new QAction(ossimQtMainWindow);
   projectOpenAction->setObjectName(QString::fromUtf8("projectOpenAction"));
   projectOpenAction->setName("projectOpenAction");
   projectOpenAction->setCheckable(false);
   projectOpenAction->setChecked(false);
   projectOpenAction->setEnabled(true);
   projectOpenAction->setIcon(QIcon());
   projectSaveAction = new QAction(ossimQtMainWindow);
   projectSaveAction->setObjectName(QString::fromUtf8("projectSaveAction"));
   projectSaveAction->setName("projectSaveAction");
   projectSaveAction->setEnabled(true);
   projectSaveAction->setIcon(QIcon());
   projectSaveAsAction = new QAction(ossimQtMainWindow);
   projectSaveAsAction->setObjectName(QString::fromUtf8("projectSaveAsAction"));
   projectSaveAsAction->setName("projectSaveAsAction");
   projectSaveAsAction->setEnabled(true);
   projectExitAction = new QAction(ossimQtMainWindow);
   projectExitAction->setObjectName(QString::fromUtf8("projectExitAction"));
   projectExitAction->setName("projectExitAction");
   editUndoAction = new QAction(ossimQtMainWindow);
   editUndoAction->setObjectName(QString::fromUtf8("editUndoAction"));
   editUndoAction->setName("editUndoAction");
   editUndoAction->setIcon(QIcon());
   editRedoAction = new QAction(ossimQtMainWindow);
   editRedoAction->setObjectName(QString::fromUtf8("editRedoAction"));
   editRedoAction->setName("editRedoAction");
   editRedoAction->setIcon(QIcon());
   editCutAction = new QAction(ossimQtMainWindow);
   editCutAction->setObjectName(QString::fromUtf8("editCutAction"));
   editCutAction->setName("editCutAction");
   editCutAction->setIcon(QIcon());
   editCopyAction = new QAction(ossimQtMainWindow);
   editCopyAction->setObjectName(QString::fromUtf8("editCopyAction"));
   editCopyAction->setName("editCopyAction");
   editCopyAction->setIcon(QIcon());
   editPasteAction = new QAction(ossimQtMainWindow);
   editPasteAction->setObjectName(QString::fromUtf8("editPasteAction"));
   editPasteAction->setName("editPasteAction");
   editPasteAction->setIcon(QIcon());
   editFindAction = new QAction(ossimQtMainWindow);
   editFindAction->setObjectName(QString::fromUtf8("editFindAction"));
   editFindAction->setName("editFindAction");
   editFindAction->setIcon(QIcon());
   helpContentsAction = new QAction(ossimQtMainWindow);
   helpContentsAction->setObjectName(QString::fromUtf8("helpContentsAction"));
   helpContentsAction->setName("helpContentsAction");
   helpContentsAction->setEnabled(false);
   helpIndexAction = new QAction(ossimQtMainWindow);
   helpIndexAction->setObjectName(QString::fromUtf8("helpIndexAction"));
   helpIndexAction->setName("helpIndexAction");
   helpIndexAction->setEnabled(false);
   helpAboutAction = new QAction(ossimQtMainWindow);
   helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
   helpAboutAction->setName("helpAboutAction");
   helpAboutAction->setEnabled(true);
   windowCascade = new QAction(ossimQtMainWindow);
   windowCascade->setObjectName(QString::fromUtf8("windowCascade"));
   windowCascade->setName("windowCascade");
   windowTile = new QAction(ossimQtMainWindow);
   windowTile->setObjectName(QString::fromUtf8("windowTile"));
   windowTile->setName("windowTile");
   windowMinimizeAll = new QAction(ossimQtMainWindow);
   windowMinimizeAll->setObjectName(QString::fromUtf8("windowMinimizeAll"));
   windowMinimizeAll->setName("windowMinimizeAll");
   windowCloseAll = new QAction(ossimQtMainWindow);
   windowCloseAll->setObjectName(QString::fromUtf8("windowCloseAll"));
   windowCloseAll->setName("windowCloseAll");
   windowMaximize = new QAction(ossimQtMainWindow);
   windowMaximize->setObjectName(QString::fromUtf8("windowMaximize"));
   windowMaximize->setName("windowMaximize");
   utilitiesDatumConverterAction = new QAction(ossimQtMainWindow);
   utilitiesDatumConverterAction->setObjectName(QString::fromUtf8("utilitiesDatumConverterAction"));
   utilitiesDatumConverterAction->setName("utilitiesDatumConverterAction");
   utilitiesDatumConverterAction->setEnabled(true);
   utilitiesElevationManagerAction = new QAction(ossimQtMainWindow);
   utilitiesElevationManagerAction->setObjectName(QString::fromUtf8("utilitiesElevationManagerAction"));
   utilitiesElevationManagerAction->setName("utilitiesElevationManagerAction");
   utilitiesElevationManagerAction->setEnabled(true);
   utilitiesUnitConverterAction = new QAction(ossimQtMainWindow);
   utilitiesUnitConverterAction->setObjectName(QString::fromUtf8("utilitiesUnitConverterAction"));
   utilitiesUnitConverterAction->setName("utilitiesUnitConverterAction");
   layerImportImageFileAction = new QAction(ossimQtMainWindow);
   layerImportImageFileAction->setObjectName(QString::fromUtf8("layerImportImageFileAction"));
   layerImportImageFileAction->setName("layerImportImageFileAction");
   layerImportImageFileAction->setEnabled(false);
   layerOpenFileAction = new QAction(ossimQtMainWindow);
   layerOpenFileAction->setObjectName(QString::fromUtf8("layerOpenFileAction"));
   layerOpenFileAction->setName("layerOpenFileAction");
   layerOpenFromAction = new QAction(ossimQtMainWindow);
   layerOpenFromAction->setObjectName(QString::fromUtf8("layerOpenFromAction"));
   layerOpenFromAction->setName("layerOpenFromAction");
   windowRestoreAllAction = new QAction(ossimQtMainWindow);
   windowRestoreAllAction->setObjectName(QString::fromUtf8("windowRestoreAllAction"));
   windowRestoreAllAction->setName("windowRestoreAllAction");
   windowRefreshAllAction = new QAction(ossimQtMainWindow);
   windowRefreshAllAction->setObjectName(QString::fromUtf8("windowRefreshAllAction"));
   windowRefreshAllAction->setName("windowRefreshAllAction");
   layerManager = new QAction(ossimQtMainWindow);
   layerManager->setObjectName(QString::fromUtf8("layerManager"));
   layerManager->setName("layerManager");
   projectPreferencesAction = new QAction(ossimQtMainWindow);
   projectPreferencesAction->setObjectName(QString::fromUtf8("projectPreferencesAction"));
   projectPreferencesAction->setName("projectPreferencesAction");
   projectPreferencesAction->setEnabled(true);
   projectNewAction = new QAction(ossimQtMainWindow);
   projectNewAction->setObjectName(QString::fromUtf8("projectNewAction"));
   projectNewAction->setName("projectNewAction");
   projectNewAction->setEnabled(true);
   projectCloseAction = new QAction(ossimQtMainWindow);
   projectCloseAction->setObjectName(QString::fromUtf8("projectCloseAction"));
   projectCloseAction->setName("projectCloseAction");
   layerHistogramMatchAction = new QAction(ossimQtMainWindow);
   layerHistogramMatchAction->setObjectName(QString::fromUtf8("layerHistogramMatchAction"));
   layerHistogramMatchAction->setName("layerHistogramMatchAction");
   vceNewChainAction = new QAction(ossimQtMainWindow);
   vceNewChainAction->setObjectName(QString::fromUtf8("vceNewChainAction"));
   vceNewChainAction->setName("vceNewChainAction");
   vceComponentDialogAction = new QAction(ossimQtMainWindow);
   vceComponentDialogAction->setObjectName(QString::fromUtf8("vceComponentDialogAction"));
   vceComponentDialogAction->setName("vceComponentDialogAction");
   layerCombinerClosestPtAction = new QAction(ossimQtMainWindow);
   layerCombinerClosestPtAction->setObjectName(QString::fromUtf8("layerCombinerClosestPtAction"));
   layerCombinerClosestPtAction->setName("layerCombinerClosestPtAction");

   /*
   layerCombineAction = new QAction(ossimQtMainWindow);
   layerCombineAction->setObjectName(QString::fromUtf8("layerCombineAction"));
   layerCombineAction->setName("layerCombineAction");
   layerFusionAction = new QAction(ossimQtMainWindow);
   layerFusionAction->setObjectName(QString::fromUtf8("layerFusionAction"));
   layerFusionAction->setName("layerFusionAction");   
   layerCorrectionAction = new QAction(ossimQtMainWindow);
   layerCorrectionAction->setObjectName(QString::fromUtf8("layerCorrectionAction"));
   layerCorrectionAction->setName("layerCorrectionAction");
   layerElevationAction = new QAction(ossimQtMainWindow);
   layerElevationAction->setObjectName(QString::fromUtf8("layerElevationAction"));   
   layerElevationAction->setName("layerElevationAction");
   */
   
   layerFusionLocalCorrelationAction = new QAction(ossimQtMainWindow);
   layerFusionLocalCorrelationAction->setObjectName(QString::fromUtf8("layerFusionLocalCorrelationAction"));
   layerFusionLocalCorrelationAction->setName("layerFusionLocalCorrelationAction");
   layerFusionSFIMAction = new QAction(ossimQtMainWindow);
   layerFusionSFIMAction->setObjectName(QString::fromUtf8("layerFusionSFIMAction"));
   layerFusionSFIMAction->setName("layerFusionSFIMAction");
   utilitiesGeometry_BuilderAction = new QAction(ossimQtMainWindow);
   utilitiesGeometry_BuilderAction->setObjectName(QString::fromUtf8("utilitiesGeometry_BuilderAction"));
   utilitiesGeometry_BuilderAction->setName("utilitiesGeometry_BuilderAction");
   utilitiesGeometry_BuilderAction->setEnabled(true);
   layerFusionActionGroup = new QActionGroup(ossimQtMainWindow);
   layerFusionActionGroup->setObjectName(QString::fromUtf8("layerFusionActionGroup"));
   layerFusionActionGroup->setName("layerFusionActionGroup");
   layerFusionColorNormalizedAction = new QAction(layerFusionActionGroup);
   layerFusionColorNormalizedAction->setObjectName(QString::fromUtf8("layerFusionColorNormalizedAction"));
   layerFusionColorNormalizedAction->setName("layerFusionColorNormalizedAction");
   layerFusionColorNormalizedAction->setCheckable(false);
   layerRadiometricCorrectionActionGroup = new QActionGroup(ossimQtMainWindow);
   layerRadiometricCorrectionActionGroup->setObjectName(QString::fromUtf8("layerRadiometricCorrectionActionGroup"));
   layerRadiometricCorrectionActionGroup->setName("layerRadiometricCorrectionActionGroup");
   layerCorrectionAtmosphericCorrectionAction = new QAction(layerRadiometricCorrectionActionGroup);
   layerCorrectionAtmosphericCorrectionAction->setObjectName(QString::fromUtf8("layerCorrectionAtmosphericCorrectionAction"));
   layerCorrectionAtmosphericCorrectionAction->setName("layerCorrectionAtmosphericCorrectionAction");
   layerCorrectionAtmosphericCorrectionAction->setCheckable(false);
   layerCorrectionAtmosphericCorrectionAction->setEnabled(false);
   layerCorrectionTopographicAction = new QAction(layerRadiometricCorrectionActionGroup);
   layerCorrectionTopographicAction->setObjectName(QString::fromUtf8("layerCorrectionTopographicAction"));
   layerCorrectionTopographicAction->setName("layerCorrectionTopographicAction");
   layerCorrectionTopographicAction->setCheckable(false);
   layerCorrectionTopographicAction->setEnabled(true);
   layerElevationActionGroup = new QActionGroup(ossimQtMainWindow);
   layerElevationActionGroup->setObjectName(QString::fromUtf8("layerElevationActionGroup"));
   layerElevationActionGroup->setName("layerElevationActionGroup");
   layerElevationHillShadeAction = new QAction(layerElevationActionGroup);
   layerElevationHillShadeAction->setObjectName(QString::fromUtf8("layerElevationHillShadeAction"));
   layerElevationHillShadeAction->setName("layerElevationHillShadeAction");
   layerElevationHillShadeAction->setCheckable(false);
   layerElevationHillShadeAction->setEnabled(true);
   layerElevationNormalsAction = new QAction(layerElevationActionGroup);
   layerElevationNormalsAction->setObjectName(QString::fromUtf8("layerElevationNormalsAction"));
   layerElevationNormalsAction->setName("layerElevationNormalsAction");
   layerElevationNormalsAction->setCheckable(false);
   layerCombineActionGroup = new QActionGroup(ossimQtMainWindow);
   layerCombineActionGroup->setObjectName(QString::fromUtf8("layerCombineActionGroup"));
   layerCombineActionGroup->setName("layerCombineActionGroup");
   layerCombineMosaicAction = new QAction(layerCombineActionGroup);
   layerCombineMosaicAction->setObjectName(QString::fromUtf8("layerCombineMosaicAction"));
   layerCombineMosaicAction->setName("layerCombineMosaicAction");
   layerCombineMosaicAction->setCheckable(false);
   layerCombineBlendAction = new QAction(layerCombineActionGroup);
   layerCombineBlendAction->setObjectName(QString::fromUtf8("layerCombineBlendAction"));
   layerCombineBlendAction->setName("layerCombineBlendAction");
   layerCombineBlendAction->setCheckable(false);
   layerCombineFeatherAction = new QAction(layerCombineActionGroup);
   layerCombineFeatherAction->setObjectName(QString::fromUtf8("layerCombineFeatherAction"));
   layerCombineFeatherAction->setName("layerCombineFeatherAction");
   layerCombineFeatherAction->setCheckable(false);
   layerCombineBandMergeAction = new QAction(layerCombineActionGroup);
   layerCombineBandMergeAction->setObjectName(QString::fromUtf8("layerCombineBandMergeAction"));
   layerCombineBandMergeAction->setName("layerCombineBandMergeAction");
   layerCombineBandMergeAction->setCheckable(false);
   layerCombineOrthoAction = new QAction(layerCombineActionGroup);
   layerCombineOrthoAction->setObjectName(QString::fromUtf8("layerCombineOrthoAction"));
   layerCombineOrthoAction->setName("layerCombineOrthoAction");
   layerCombineOrthoAction->setCheckable(true);
   layerCombinerMaxAction = new QAction(layerCombineActionGroup);
   layerCombinerMaxAction->setObjectName(QString::fromUtf8("layerCombinerMaxAction"));
   layerCombinerMaxAction->setName("layerCombinerMaxAction");
   layerCombinerMaxAction->setCheckable(false);
   layerCombinerFactoryAction = new QAction(layerCombineActionGroup);
   layerCombinerFactoryAction->setObjectName(QString::fromUtf8("layerCombinerFactoryAction"));
   layerCombinerFactoryAction->setName("layerCombinerFactoryAction");
   layerCombinerFactoryAction->setCheckable(false);
   widget = new QWidget(ossimQtMainWindow);
   widget->setObjectName(QString::fromUtf8("widget"));
   ossimQtMainWindow->setCentralWidget(widget);
   menubar = new QMenuBar(ossimQtMainWindow);
   menubar->setObjectName(QString::fromUtf8("menubar"));
   menubar->setGeometry(QRect(0, 0, 683, 33));
   fileMenu = new QMenu(menubar);
   fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
   PopupMenu = new QMenu(menubar);
   PopupMenu->setObjectName(QString::fromUtf8("PopupMenu"));
   layerCombineMenu = new QMenu(PopupMenu);
   layerCombineMenu->setObjectName(QString::fromUtf8("layerCombineMenu"));
   layerFusionMenu = new QMenu(PopupMenu);
   layerFusionMenu->setObjectName(QString::fromUtf8("layerFusionMenu"));
   
   layerCorrectionMenu = new QMenu(PopupMenu);
   layerCorrectionMenu->setObjectName(QString::fromUtf8("layerCorrectionMenu"));
   
   layerElevationMenu = new QMenu(PopupMenu);
   layerElevationMenu->setObjectName(QString::fromUtf8("layerElevationMenu"));
   
   PopupMenu_4 = new QMenu(menubar);
   PopupMenu_4->setObjectName(QString::fromUtf8("PopupMenu_4"));
   PopupMenu_5 = new QMenu(menubar);
   PopupMenu_5->setObjectName(QString::fromUtf8("PopupMenu_5"));
   PopupMenu_2 = new QMenu(menubar);
   PopupMenu_2->setObjectName(QString::fromUtf8("PopupMenu_2"));
   helpMenu = new QMenu(menubar);
   helpMenu->setObjectName(QString::fromUtf8("helpMenu"));

   menubar->addAction(fileMenu->menuAction());
   menubar->addAction(PopupMenu->menuAction());
   menubar->addAction(PopupMenu_4->menuAction());
   menubar->addAction(PopupMenu_5->menuAction());
   menubar->addAction(PopupMenu_2->menuAction());
   menubar->addAction(helpMenu->menuAction());
   
   fileMenu->addSeparator();
   fileMenu->addAction(layerOpenFileAction);
   fileMenu->addAction(layerOpenFromAction);
   fileMenu->addSeparator();
   fileMenu->addAction(projectOpenAction);
   fileMenu->addAction(projectCloseAction);
   fileMenu->addAction(projectNewAction);
   fileMenu->addAction(projectSaveAction);
   fileMenu->addAction(projectSaveAsAction);
   fileMenu->addSeparator();
   fileMenu->addAction(projectPreferencesAction);
   fileMenu->addSeparator();
   fileMenu->addAction(projectExitAction);
   
   PopupMenu->addAction(layerManager);
   PopupMenu->addSeparator();
   PopupMenu->addAction(layerImportImageFileAction);
   PopupMenu->addSeparator();
   PopupMenu->addAction(layerHistogramMatchAction);
   
//   PopupMenu->addAction(layerCombineAction);
   PopupMenu->addAction(layerCombineMenu->menuAction());
   
   
//   PopupMenu->addAction(layerFusionAction);
   PopupMenu->addAction(layerFusionMenu->menuAction());
   
//   PopupMenu->addAction(layerCorrectionAction);
   PopupMenu->addAction(layerCorrectionMenu->menuAction());

   
//   PopupMenu->addAction(layerElevationAction);
   PopupMenu->addAction(layerElevationMenu->menuAction());
   
   layerCombineMenu->addAction(layerCombineMosaicAction);
   layerCombineMenu->addAction(layerCombineBlendAction);
   layerCombineMenu->addAction(layerCombineFeatherAction);
   layerCombineMenu->addAction(layerCombineBandMergeAction);
   layerCombineMenu->addAction(layerCombineOrthoAction);
   layerCombineMenu->addAction(layerCombinerMaxAction);
   layerCombineMenu->addAction(layerCombinerClosestPtAction);
   layerCombineMenu->addAction(layerCombinerFactoryAction);
   layerFusionMenu->addAction(layerFusionColorNormalizedAction);
   layerFusionMenu->addAction(layerFusionLocalCorrelationAction);
   layerFusionMenu->addAction(layerFusionSFIMAction);

   layerCorrectionMenu->addAction(layerCorrectionAtmosphericCorrectionAction);
   layerCorrectionMenu->addAction(layerCorrectionTopographicAction);
   layerElevationMenu->addAction(layerElevationHillShadeAction);

   
   layerElevationMenu->addAction(layerElevationNormalsAction);
   PopupMenu_4->addAction(vceNewChainAction);
   PopupMenu_4->addAction(vceComponentDialogAction);
   PopupMenu_5->addAction(utilitiesDatumConverterAction);
   PopupMenu_5->addAction(utilitiesElevationManagerAction);
   PopupMenu_5->addAction(utilitiesUnitConverterAction);
   PopupMenu_5->addAction(utilitiesGeometry_BuilderAction);
   PopupMenu_2->addAction(windowCascade);
   PopupMenu_2->addAction(windowTile);
   PopupMenu_2->addAction(windowMinimizeAll);
   PopupMenu_2->addAction(windowRestoreAllAction);
   PopupMenu_2->addAction(windowMaximize);
   PopupMenu_2->addSeparator();
   PopupMenu_2->addAction(windowRefreshAllAction);
   PopupMenu_2->addAction(windowCloseAll);
   helpMenu->addAction(helpContentsAction);
   helpMenu->addAction(helpIndexAction);
   helpMenu->addSeparator();
   helpMenu->addAction(helpAboutAction);

   retranslateUi(ossimQtMainWindow);
   QObject::connect(windowCascade, SIGNAL(activated()), ossimQtMainWindow, SLOT(windowCascade_activated()));
   QObject::connect(windowCascade, SIGNAL(activated()), ossimQtMainWindow, SLOT(windowCascade_activated()));
   QObject::connect(windowTile, SIGNAL(activated()), ossimQtMainWindow, SLOT(windowTile_activated()));
   QObject::connect(windowMinimizeAll, SIGNAL(activated()), ossimQtMainWindow, SLOT(windowMinimizeAll_activated()));
   QObject::connect(windowCloseAll, SIGNAL(activated()), ossimQtMainWindow, SLOT(windowCloseAll_activated()));
   QObject::connect(windowMaximize, SIGNAL(activated()), ossimQtMainWindow, SLOT(ActionwindowMaximize_activated()));
   QObject::connect(projectExitAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(projectExitAction_activated()));
   QObject::connect(projectOpenAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(projectOpenAction_activated()));
   QObject::connect(projectSaveAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(projectSaveAction_activated()));
   QObject::connect(projectSaveAsAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(projectSaveAsAction_activated()));
   QObject::connect(utilitiesElevationManagerAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(utilitiesElevationManagerAction_activated()));
   QObject::connect(utilitiesUnitConverterAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(utilitiesUnitConverterAction_activated()));
   QObject::connect(utilitiesDatumConverterAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(utilitiesDatumConverterAction_activated()));
   QObject::connect(layerImportImageFileAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerImportFileAction_activated()));
   QObject::connect(layerOpenFileAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerOpenFileAction_activated()));
   QObject::connect(layerOpenFromAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerOpenFromAction_activated()));
   QObject::connect(windowRestoreAllAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(windowRestoreAllAction_activated()));
   QObject::connect(windowRefreshAllAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(windowRefreshAllAction_activated()));
   QObject::connect(ossimQtMainWindow, SIGNAL(usesTextLabelChanged(bool)), ossimQtMainWindow, SLOT(ossimQtMainWindow_usesTextLabelChanged(bool)));
   QObject::connect(layerManager, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerManager_activated()));
   QObject::connect(projectNewAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(projectNewAction_activated()));
   QObject::connect(projectCloseAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(projectCloseAction_activated()));
   QObject::connect(layerFusionColorNormalizedAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerFusionColorNormalizedAction_activated()));
   QObject::connect(layerCombineBandMergeAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerCombineBandMergeAction_activated()));
   QObject::connect(layerCombineBlendAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerCombineBlendAction_activated()));
   QObject::connect(layerCombineFeatherAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerCombineFeatherAction_activated()));
   QObject::connect(layerCombineMosaicAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerCombineMosaicAction_activated()));
   QObject::connect(layerElevationHillShadeAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerElevationHillShadeAction_activated()));
   QObject::connect(layerElevationNormalsAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerElevationNormalsAction_activated()));

   QObject::connect(layerCorrectionTopographicAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerCorrectionTopographicAction_activated()));

   QObject::connect(layerCombineOrthoAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerCombineOrthoAction_activated()));
   QObject::connect(layerHistogramMatchAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerHistogramMatchAction_activated()));
   QObject::connect(vceNewChainAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(vceNewChainAction_activated()));
   QObject::connect(vceComponentDialogAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(vceComponentDialogAction_activated()));
   QObject::connect(projectPreferencesAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(projectPreferencesAction_activated()));
   QObject::connect(helpAboutAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(helpAboutActionActivated()));
   QObject::connect(layerCombinerMaxAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerCombinerMaxActionActivated()));
   QObject::connect(layerCombinerFactoryAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerCombinerFactoryActionActivated()));
   QObject::connect(layerCombinerClosestPtAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerCombinerClosestPtAction_activated()));
   QObject::connect(layerFusionLocalCorrelationAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerFusionLocalCorrelationAction_activated()));
   QObject::connect(layerFusionSFIMAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(layerFusionSFIMAction_activated()));
   QObject::connect(utilitiesGeometry_BuilderAction, SIGNAL(activated()), ossimQtMainWindow, SLOT(utilitiesGeometry_BuilderAction_activated()));

   QMetaObject::connectSlotsByName(ossimQtMainWindow);
} // setupUi

void Ui_ossimQtMainWindow::retranslateUi(Q3MainWindow *ossimQtMainWindow)
{
   ossimQtMainWindow->setWindowTitle(QApplication::translate("ossimQtMainWindow", "Imagelinker", 0, QApplication::UnicodeUTF8));
   projectOpenAction->setIconText(QApplication::translate("ossimQtMainWindow", "Open Project", 0, QApplication::UnicodeUTF8));
   projectOpenAction->setText(QApplication::translate("ossimQtMainWindow", "Open Project...", 0, QApplication::UnicodeUTF8));
   projectSaveAction->setIconText(QApplication::translate("ossimQtMainWindow", "Save Project", 0, QApplication::UnicodeUTF8));
   projectSaveAction->setText(QApplication::translate("ossimQtMainWindow", "&Save Project", 0, QApplication::UnicodeUTF8));
   projectSaveAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
   projectSaveAsAction->setIconText(QApplication::translate("ossimQtMainWindow", "Save As", 0, QApplication::UnicodeUTF8));
   projectSaveAsAction->setText(QApplication::translate("ossimQtMainWindow", "Save &Project As...", 0, QApplication::UnicodeUTF8));
   projectSaveAsAction->setShortcut(QString());
   projectExitAction->setIconText(QApplication::translate("ossimQtMainWindow", "Exit", 0, QApplication::UnicodeUTF8));
   projectExitAction->setText(QApplication::translate("ossimQtMainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
   projectExitAction->setShortcut(QString());
   editUndoAction->setIconText(QApplication::translate("ossimQtMainWindow", "Undo", 0, QApplication::UnicodeUTF8));
   editUndoAction->setText(QApplication::translate("ossimQtMainWindow", "&Undo", 0, QApplication::UnicodeUTF8));
   editUndoAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
   editRedoAction->setIconText(QApplication::translate("ossimQtMainWindow", "Redo", 0, QApplication::UnicodeUTF8));
   editRedoAction->setText(QApplication::translate("ossimQtMainWindow", "&Redo", 0, QApplication::UnicodeUTF8));
   editRedoAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
   editCutAction->setIconText(QApplication::translate("ossimQtMainWindow", "Cut", 0, QApplication::UnicodeUTF8));
   editCutAction->setText(QApplication::translate("ossimQtMainWindow", "&Cut", 0, QApplication::UnicodeUTF8));
   editCutAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
   editCopyAction->setIconText(QApplication::translate("ossimQtMainWindow", "Copy", 0, QApplication::UnicodeUTF8));
   editCopyAction->setText(QApplication::translate("ossimQtMainWindow", "C&opy", 0, QApplication::UnicodeUTF8));
   editCopyAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
   editPasteAction->setIconText(QApplication::translate("ossimQtMainWindow", "Paste", 0, QApplication::UnicodeUTF8));
   editPasteAction->setText(QApplication::translate("ossimQtMainWindow", "&Paste", 0, QApplication::UnicodeUTF8));
   editPasteAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
   editFindAction->setIconText(QApplication::translate("ossimQtMainWindow", "Find", 0, QApplication::UnicodeUTF8));
   editFindAction->setText(QApplication::translate("ossimQtMainWindow", "&Find...", 0, QApplication::UnicodeUTF8));
   editFindAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
   helpContentsAction->setIconText(QApplication::translate("ossimQtMainWindow", "Contents", 0, QApplication::UnicodeUTF8));
   helpContentsAction->setText(QApplication::translate("ossimQtMainWindow", "&Contents...", 0, QApplication::UnicodeUTF8));
   helpContentsAction->setShortcut(QString());
   helpIndexAction->setIconText(QApplication::translate("ossimQtMainWindow", "Index", 0, QApplication::UnicodeUTF8));
   helpIndexAction->setText(QApplication::translate("ossimQtMainWindow", "&Index...", 0, QApplication::UnicodeUTF8));
   helpIndexAction->setShortcut(QString());
   helpAboutAction->setIconText(QApplication::translate("ossimQtMainWindow", "About", 0, QApplication::UnicodeUTF8));
   helpAboutAction->setText(QApplication::translate("ossimQtMainWindow", "&About", 0, QApplication::UnicodeUTF8));
   helpAboutAction->setShortcut(QString());
   windowCascade->setIconText(QApplication::translate("ossimQtMainWindow", "Cascade", 0, QApplication::UnicodeUTF8));
   windowCascade->setText(QApplication::translate("ossimQtMainWindow", "Cascade", 0, QApplication::UnicodeUTF8));
   windowTile->setIconText(QApplication::translate("ossimQtMainWindow", "Tile", 0, QApplication::UnicodeUTF8));
   windowTile->setText(QApplication::translate("ossimQtMainWindow", "Tile", 0, QApplication::UnicodeUTF8));
   windowMinimizeAll->setIconText(QApplication::translate("ossimQtMainWindow", "Minimize all", 0, QApplication::UnicodeUTF8));
   windowMinimizeAll->setText(QApplication::translate("ossimQtMainWindow", "Minimize all", 0, QApplication::UnicodeUTF8));
   windowCloseAll->setIconText(QApplication::translate("ossimQtMainWindow", "Close all", 0, QApplication::UnicodeUTF8));
   windowCloseAll->setText(QApplication::translate("ossimQtMainWindow", "Close all", 0, QApplication::UnicodeUTF8));
   windowMaximize->setIconText(QApplication::translate("ossimQtMainWindow", "Maximize", 0, QApplication::UnicodeUTF8));
   windowMaximize->setText(QApplication::translate("ossimQtMainWindow", "Maximize", 0, QApplication::UnicodeUTF8));
   utilitiesDatumConverterAction->setIconText(QApplication::translate("ossimQtMainWindow", "Datum converter", 0, QApplication::UnicodeUTF8));
   utilitiesDatumConverterAction->setText(QApplication::translate("ossimQtMainWindow", "Datum converter", 0, QApplication::UnicodeUTF8));
   utilitiesElevationManagerAction->setIconText(QApplication::translate("ossimQtMainWindow", "Elevation manager", 0, QApplication::UnicodeUTF8));
   utilitiesElevationManagerAction->setText(QApplication::translate("ossimQtMainWindow", "Elevation manager", 0, QApplication::UnicodeUTF8));
   utilitiesUnitConverterAction->setIconText(QApplication::translate("ossimQtMainWindow", "Unit converter", 0, QApplication::UnicodeUTF8));
   utilitiesUnitConverterAction->setText(QApplication::translate("ossimQtMainWindow", "Unit converter", 0, QApplication::UnicodeUTF8));
   layerImportImageFileAction->setIconText(QApplication::translate("ossimQtMainWindow", "Import image file...", 0, QApplication::UnicodeUTF8));
   layerImportImageFileAction->setText(QApplication::translate("ossimQtMainWindow", "Import image file...", 0, QApplication::UnicodeUTF8));
   layerOpenFileAction->setIconText(QApplication::translate("ossimQtMainWindow", "&Open Image", 0, QApplication::UnicodeUTF8));
   layerOpenFileAction->setText(QApplication::translate("ossimQtMainWindow", "&Open Image...", 0, QApplication::UnicodeUTF8));
   layerOpenFileAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
   layerOpenFromAction->setIconText(QApplication::translate("ossimQtMainWindow", "&Open Image From", 0, QApplication::UnicodeUTF8));
   layerOpenFromAction->setText(QApplication::translate("ossimQtMainWindow", "&Open Image From...", 0, QApplication::UnicodeUTF8));
   layerOpenFromAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
   windowRestoreAllAction->setIconText(QApplication::translate("ossimQtMainWindow", "Restore all", 0, QApplication::UnicodeUTF8));
   windowRestoreAllAction->setText(QApplication::translate("ossimQtMainWindow", "Restore all", 0, QApplication::UnicodeUTF8));
   windowRefreshAllAction->setIconText(QApplication::translate("ossimQtMainWindow", "Refresh all", 0, QApplication::UnicodeUTF8));
   windowRefreshAllAction->setText(QApplication::translate("ossimQtMainWindow", "Refresh all", 0, QApplication::UnicodeUTF8));
   layerManager->setIconText(QApplication::translate("ossimQtMainWindow", "Manager", 0, QApplication::UnicodeUTF8));
   layerManager->setText(QApplication::translate("ossimQtMainWindow", "Manager", 0, QApplication::UnicodeUTF8));
   projectPreferencesAction->setIconText(QApplication::translate("ossimQtMainWindow", "Preferences", 0, QApplication::UnicodeUTF8));
   projectPreferencesAction->setText(QApplication::translate("ossimQtMainWindow", "Preferences", 0, QApplication::UnicodeUTF8));
   projectNewAction->setIconText(QApplication::translate("ossimQtMainWindow", "New Project", 0, QApplication::UnicodeUTF8));
   projectNewAction->setText(QApplication::translate("ossimQtMainWindow", "New Project", 0, QApplication::UnicodeUTF8));
   projectNewAction->setShortcut(QApplication::translate("ossimQtMainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
   projectCloseAction->setIconText(QApplication::translate("ossimQtMainWindow", "&Close Project", 0, QApplication::UnicodeUTF8));
   projectCloseAction->setText(QApplication::translate("ossimQtMainWindow", "&Close Project", 0, QApplication::UnicodeUTF8));
   layerHistogramMatchAction->setIconText(QApplication::translate("ossimQtMainWindow", "Histogram match", 0, QApplication::UnicodeUTF8));
   layerHistogramMatchAction->setText(QApplication::translate("ossimQtMainWindow", "Histogram match", 0, QApplication::UnicodeUTF8));
   vceNewChainAction->setIconText(QApplication::translate("ossimQtMainWindow", "New chain", 0, QApplication::UnicodeUTF8));
   vceNewChainAction->setText(QApplication::translate("ossimQtMainWindow", "New chain", 0, QApplication::UnicodeUTF8));
   vceComponentDialogAction->setIconText(QApplication::translate("ossimQtMainWindow", "Components", 0, QApplication::UnicodeUTF8));
   vceComponentDialogAction->setText(QApplication::translate("ossimQtMainWindow", "Components", 0, QApplication::UnicodeUTF8));
   layerCombinerClosestPtAction->setIconText(QApplication::translate("ossimQtMainWindow", "will use the pixel closest to the center of the overlapping region", 0, QApplication::UnicodeUTF8));
   layerCombinerClosestPtAction->setText(QApplication::translate("ossimQtMainWindow", "Closest to center", 0, QApplication::UnicodeUTF8));
   
   /*
   layerCombineAction->setIconText(QApplication::translate("ossimQtMainWindow", "Combine", 0, QApplication::UnicodeUTF8));
   layerCombineAction->setText(QApplication::translate("ossimQtMainWindow", "Combine", 0, QApplication::UnicodeUTF8));
   layerFusionAction->setIconText(QApplication::translate("ossimQtMainWindow", "Fusion", 0, QApplication::UnicodeUTF8));
   layerFusionAction->setText(QApplication::translate("ossimQtMainWindow", "Fusion", 0, QApplication::UnicodeUTF8));
   layerCorrectionAction->setIconText(QApplication::translate("ossimQtMainWindow", "Correction", 0, QApplication::UnicodeUTF8));
   layerCorrectionAction->setText(QApplication::translate("ossimQtMainWindow", "Correction", 0, QApplication::UnicodeUTF8));
   layerElevationAction->setIconText(QApplication::translate("ossimQtMainWindow", "Elevation", 0, QApplication::UnicodeUTF8));
   layerElevationAction->setText(QApplication::translate("ossimQtMainWindow", "Elevation", 0, QApplication::UnicodeUTF8));
   */
   
   layerFusionLocalCorrelationAction->setIconText(QApplication::translate("ossimQtMainWindow", "Local Correlation", 0, QApplication::UnicodeUTF8));
   layerFusionLocalCorrelationAction->setText(QApplication::translate("ossimQtMainWindow", "Local Correlation", 0, QApplication::UnicodeUTF8));
   layerFusionSFIMAction->setIconText(QApplication::translate("ossimQtMainWindow", "SFIM", 0, QApplication::UnicodeUTF8));
   utilitiesGeometry_BuilderAction->setIconText(QApplication::translate("ossimQtMainWindow", "Geometry Builder", 0, QApplication::UnicodeUTF8));
   layerFusionColorNormalizedAction->setIconText(QApplication::translate("ossimQtMainWindow", "Color normalized", 0, QApplication::UnicodeUTF8));
   layerFusionColorNormalizedAction->setText(QApplication::translate("ossimQtMainWindow", "Color normalized", 0, QApplication::UnicodeUTF8));
   layerFusionColorNormalizedAction->setToolTip(QApplication::translate("ossimQtMainWindow", "Color normalized fusion algorithm", 0, QApplication::UnicodeUTF8));
   layerCorrectionAtmosphericCorrectionAction->setIconText(QApplication::translate("ossimQtMainWindow", "Atmospheric", 0, QApplication::UnicodeUTF8));
   layerCorrectionAtmosphericCorrectionAction->setText(QApplication::translate("ossimQtMainWindow", "Atmospheric", 0, QApplication::UnicodeUTF8));
   layerCorrectionTopographicAction->setIconText(QApplication::translate("ossimQtMainWindow", "Topographic", 0, QApplication::UnicodeUTF8));
   layerCorrectionTopographicAction->setText(QApplication::translate("ossimQtMainWindow", "Topographic", 0, QApplication::UnicodeUTF8));
   layerElevationHillShadeAction->setIconText(QApplication::translate("ossimQtMainWindow", "Hillshade", 0, QApplication::UnicodeUTF8));
   layerElevationHillShadeAction->setText(QApplication::translate("ossimQtMainWindow", "Hillshade", 0, QApplication::UnicodeUTF8));
   layerElevationNormalsAction->setIconText(QApplication::translate("ossimQtMainWindow", "Normals", 0, QApplication::UnicodeUTF8));
   layerElevationNormalsAction->setText(QApplication::translate("ossimQtMainWindow", "Normals", 0, QApplication::UnicodeUTF8));
   layerCombineMosaicAction->setIconText(QApplication::translate("ossimQtMainWindow", "Mosaic", 0, QApplication::UnicodeUTF8));
   layerCombineMosaicAction->setText(QApplication::translate("ossimQtMainWindow", "Mosaic", 0, QApplication::UnicodeUTF8));
   layerCombineMosaicAction->setToolTip(QApplication::translate("ossimQtMainWindow", "Simple mosaic", 0, QApplication::UnicodeUTF8));
   layerCombineBlendAction->setIconText(QApplication::translate("ossimQtMainWindow", "Blend", 0, QApplication::UnicodeUTF8));
   layerCombineBlendAction->setText(QApplication::translate("ossimQtMainWindow", "Blend", 0, QApplication::UnicodeUTF8));
   layerCombineBlendAction->setToolTip(QApplication::translate("ossimQtMainWindow", "Blend", 0, QApplication::UnicodeUTF8));
   layerCombineFeatherAction->setIconText(QApplication::translate("ossimQtMainWindow", "Feather", 0, QApplication::UnicodeUTF8));
   layerCombineFeatherAction->setText(QApplication::translate("ossimQtMainWindow", "Feather", 0, QApplication::UnicodeUTF8));
   layerCombineFeatherAction->setToolTip(QApplication::translate("ossimQtMainWindow", "Feather", 0, QApplication::UnicodeUTF8));
   layerCombineBandMergeAction->setIconText(QApplication::translate("ossimQtMainWindow", "Merge bands", 0, QApplication::UnicodeUTF8));
   layerCombineBandMergeAction->setText(QApplication::translate("ossimQtMainWindow", "Merge bands", 0, QApplication::UnicodeUTF8));
   layerCombineOrthoAction->setIconText(QApplication::translate("ossimQtMainWindow", "Ortho", 0, QApplication::UnicodeUTF8));
   layerCombineOrthoAction->setText(QApplication::translate("ossimQtMainWindow", "Ortho", 0, QApplication::UnicodeUTF8));
   layerCombinerMaxAction->setIconText(QApplication::translate("ossimQtMainWindow", "Max", 0, QApplication::UnicodeUTF8));
   layerCombinerMaxAction->setText(QApplication::translate("ossimQtMainWindow", "Max", 0, QApplication::UnicodeUTF8));
   layerCombinerFactoryAction->setIconText(QApplication::translate("ossimQtMainWindow", "Factory...", 0, QApplication::UnicodeUTF8));
   layerCombinerFactoryAction->setText(QApplication::translate("ossimQtMainWindow", "Factory...", 0, QApplication::UnicodeUTF8));
   fileMenu->setTitle(QApplication::translate("ossimQtMainWindow", "&File", 0, QApplication::UnicodeUTF8));
   PopupMenu->setTitle(QApplication::translate("ossimQtMainWindow", "&Layer", 0, QApplication::UnicodeUTF8));
   layerCombineMenu->setTitle(QApplication::translate("ossimQtMainWindow", "Combine", 0, QApplication::UnicodeUTF8));
   layerFusionMenu->setTitle(QApplication::translate("ossimQtMainWindow", "Fusion", 0, QApplication::UnicodeUTF8));
   layerCorrectionMenu->setTitle(QApplication::translate("ossimQtMainWindow", "Correction", 0, QApplication::UnicodeUTF8));
   layerElevationMenu->setTitle(QApplication::translate("ossimQtMainWindow", "Elevation", 0, QApplication::UnicodeUTF8));
   PopupMenu_4->setTitle(QApplication::translate("ossimQtMainWindow", "Vce", 0, QApplication::UnicodeUTF8));
   PopupMenu_5->setTitle(QApplication::translate("ossimQtMainWindow", "&Utilities", 0, QApplication::UnicodeUTF8));
   PopupMenu_2->setTitle(QApplication::translate("ossimQtMainWindow", "&Window", 0, QApplication::UnicodeUTF8));
   helpMenu->setTitle(QApplication::translate("ossimQtMainWindow", "&Help", 0, QApplication::UnicodeUTF8));
} // retranslateUi
