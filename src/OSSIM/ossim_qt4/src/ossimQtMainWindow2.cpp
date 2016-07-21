//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt.
//
// Author:  David Burken
//
// Description: Rewrite of the Qt3 imagelinker main window written by
// Garrett Potts.
//
// $Id$
//----------------------------------------------------------------------------


#include <QtGui/QDragEnterEvent>
#include <QtGui/QApplication>

#include "ossimQtMainWindowController.h"
#include "ossimQtMainWindow2.h"

/*
 *  Constructs a ossimQtMainWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
ossimQtMainWindow2::ossimQtMainWindow2(QWidget* parent,
                                     Qt::WindowFlags fl)
   : QMainWindow(parent, fl)
{
   init();

   
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtMainWindow2::~ossimQtMainWindow2()
{
   destroy();
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtMainWindow2::languageChange()
{
   // retranslateUi(this);
}

void ossimQtMainWindow2::dragEnterEvent(QDragEnterEvent* event)
{
   event->accept(theController->acceptsDragEvent(event));
}

void ossimQtMainWindow2::dropEvent(QDropEvent* event)
{
   theController->dropEvent(event);
}

void ossimQtMainWindow2::destroy()
{
   if(theController)
   {
      delete theController;
      theController=0;
   }
}

void ossimQtMainWindow2::init()
{
   // theController = new ossimQtMainWindowController(this);
}

ossimQtMainWindowController* ossimQtMainWindow2::getController()
{
   return theController;
}

void ossimQtMainWindow2::windowCascade_activated()
{
   theController->cascadeDisplays();
}

void ossimQtMainWindow2::windowTile_activated()
{
   theController->tileDisplays();
}

void ossimQtMainWindow2::windowMinimizeAll_activated()
{
   theController->minimizeAllDisplays();
}

void ossimQtMainWindow2::windowCloseAll_activated()
{
   theController->closeAllDisplays();
}

void ossimQtMainWindow2::ActionwindowMaximize_activated()
{
   theController->maximizeActiveDisplay();
}

void ossimQtMainWindow2::projectExitAction_activated()
{
   theController->exitProject();
}

void ossimQtMainWindow2::projectOpenAction_activated()
{
   theController->openProject();
}

void ossimQtMainWindow2::projectSaveAction_activated()
{
   theController->saveProject();
}

void ossimQtMainWindow2::projectSaveAsAction_activated()
{
   theController->saveAsProject();
}

void ossimQtMainWindow2::projectNewAction_activated()
{
   theController->newProject();
}

void ossimQtMainWindow2::projectCloseAction_activated()
{
   theController->closeProject();
}

void ossimQtMainWindow2::utilitiesElevationManagerAction_activated()
{
   theController->openElevationManager();
}

void ossimQtMainWindow2::utilitiesUnitConverterAction_activated()
{
   theController->openUnitConverter();
}

void ossimQtMainWindow2::utilitiesDatumConverterAction_activated()
{
   theController->openDatumConverter();
}

void ossimQtMainWindow2::layerImportFileAction_activated()
{

}

void ossimQtMainWindow2::layerCombineLayersAction_activated()
{

}

void ossimQtMainWindow2::layerOpenFileAction_activated()
{
   theController->openImageFileLayer();
}

void ossimQtMainWindow2::layerOpenFromAction_activated()
{
   theController->openImageFileLayer(true);
}

void ossimQtMainWindow2::displayLayerAction_activated()
{
}

void ossimQtMainWindow2::customEvent(QCustomEvent* evt)
{
   QApplication::sendEvent(theController, evt);
}

void ossimQtMainWindow2::windowRestoreAllAction_activated()
{
   theController->restoreAllDisplays();
}

void ossimQtMainWindow2::windowRefreshAllAction_activated()
{
   theController->refreshAllDisplays();
}

void ossimQtMainWindow2::ossimQtMainWindow_usesTextLabelChanged( bool )
{

}

void ossimQtMainWindow2::layerManager_activated()
{
   theController->displayLayerManager();
}

void ossimQtMainWindow2::layerFusionColorNormalizedAction_activated()
{
   theController->colorNormalizedFusion();
}

void ossimQtMainWindow2::layerFusionLocal_CorrelationAction_activated()
{
   if(theController)
   {
      theController->localCorrelationFusion();
   }
}

void ossimQtMainWindow2::layerCombineBandMergeAction_activated()
{
   theController->bandMergeCombiner();
}

void ossimQtMainWindow2::layerCombineBlendAction_activated()
{
   theController->blendCombiner();
}

void ossimQtMainWindow2::layerCombineFeatherAction_activated()
{
   theController->featherCombiner();
}

void ossimQtMainWindow2::layerCombineMosaicAction_activated()
{
   theController->simpleCombiner();
}

void ossimQtMainWindow2::layerCombineOrthoAction_activated()
{
   theController->orthoCombiner();
}

void ossimQtMainWindow2::layerElevationHillShadeAction_activated()
{
   theController->hillShade();
}

void ossimQtMainWindow2::layerElevationNormalsAction_activated()
{
   theController->elevationNormals();
}

void ossimQtMainWindow2::layerCorrectionTopographicAction_activated()
{
   theController->topographicCorrection();
}

void ossimQtMainWindow2::layerHistogramMatchAction_activated()
{
   theController->histogramMatch();
}

void ossimQtMainWindow2::vceNewChainAction_activated()
{
   theController->vceNewChain();
}

void ossimQtMainWindow2::vceComponentDialogAction_activated()
{
   theController->vceComponentDialog();
}

void ossimQtMainWindow2::projectPreferencesAction_activated()
{
   if(theController)
   {
      theController->preferences();
   }
}

void ossimQtMainWindow2::helpAboutActionActivated()
{
   if (theController)
   {
      theController->displayAboutDialog();
   }
}

void ossimQtMainWindow2::layerCombinerMaxActionActivated()
{
   if(theController)
   {
      theController->maxCombiner();
   }
}

void ossimQtMainWindow2::layerCombinerFactoryActionActivated()
{
   if(theController)
   {
      theController->combinerFactory();
   }
}

void ossimQtMainWindow2::layerCombinerClosestPtAction_activated()
{
   if(theController)
   {
      theController->closestToCenterCombiner();
   }
}

void ossimQtMainWindow2::layerFusionLocalCorrelationAction_activated()
{
   if(theController)
   {
      theController->localCorrelationFusion();
   }
}

void ossimQtMainWindow2::layerFusionSFIMAction_activated()
{
   if(theController)
   {
      theController->SFIMFusion();
   }
}
