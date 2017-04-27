#include <ossimQt/ossimQtImageWindow.h>

#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossimQt/ossimQtImageWindowController.h>
#include <ossimQt/ossimQtSwipeScrollingImageWidget.h>

#include <QAction>
#include <QComboBox>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGui/QApplication>

/*
 *  Constructs a ossimQtImageWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
ossimQtImageWindow::ossimQtImageWindow(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : QMainWindow(parent, name, fl)
{
	theController = 0;
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtImageWindow::~ossimQtImageWindow()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtImageWindow::languageChange()
{
    retranslateUi(this);
}

void ossimQtImageWindow::init()
{
    theController = new ossimQtImageWindowController(this);
}

void ossimQtImageWindow::destroy()
{
    if(theController)
    {
	delete theController;
	theController = NULL;
    }
}

void ossimQtImageWindow::enterEvent(QEvent* evt)
{
    if(theController)
    {
	theController->enterEvent(evt);	
    }
}

void ossimQtImageWindow::leaveEvent(QEvent* evt)
{
    if(theController)
    {
	theController->leaveEvent(evt);
    }
}

void ossimQtImageWindow::addInputSource(ossimConnectableObject* obj)
{
   if(theController)
   {
      theController->addInput(obj);
   }
}

ossimQtSwipeScrollingImageWidget* ossimQtImageWindow::getImageWidget()
{
    return theImageWidget;
}

void ossimQtImageWindow::editImageGeometryAction_activated()
{
   if(theController)
   {
      theController->imageGeometry();
   }
}


void ossimQtImageWindow::editRedoAction_activated()
{

}


void ossimQtImageWindow::editUndoAction_activated()
{

}


void ossimQtImageWindow::editViewToolAction_activated()
{
   if(theController)
   {
      theController->viewTool();
   }
}


void ossimQtImageWindow::enhancementsBandSelectorAction_activated()
{
   if(theController)
   {
      theController->bandSelector();
   }
}

void ossimQtImageWindow::enhancementsBrightnessContrastAction_activated()
{
   if(theController)
   {
      theController->brightnessContrast();
   }
}

void ossimQtImageWindow::enhancementsHistogramOperationsAction_activated()
{
   if(theController)
   {
      theController->histogramOperation();
   }
}

void ossimQtImageWindow::enhancementsHsiAction_activated()
{
   if(theController)
   {
      theController->hsiAdjustments();
   }
}


void ossimQtImageWindow::fileExitAction_activated()
{
   if(theController)
   {
      theController->fileExit();
   }
}

void ossimQtImageWindow::fileSaveAsAction_activated()
{
   if(theController)
   {
      theController->igenExport();
   }
}


ossimQtImageWindowController* ossimQtImageWindow::controller()
{
    return theController;
}

bool ossimQtImageWindow::event(QEvent* e)
{
   if(!theController) return QMainWindow::event(e);
   if(!theController->event(e))
   {
      QMainWindow::event(e);
   }
   return true;
}

void ossimQtImageWindow::closeEvent(QCloseEvent* event)
{
   if(theController)
   {
      theController->closeEvent(event);
   }
}

void ossimQtImageWindow::resamplerActionGroup_selected( QAction * action)
{
   if(theController)
   {
      theController->setResampler(action->menuText());
   }
}

void ossimQtImageWindow::thePropagateButton_clicked()
{
   if(theController)
   {
      theController->propagateView();
   }
}


void ossimQtImageWindow::theFullResButton_clicked()
{
   if(theController)
   {
      theController->fullRes();
   }
}


void ossimQtImageWindow::theFitToWindowButton_clicked()
{
   if(theController)
   {
      theController->fitToWindow();
   }
}


void ossimQtImageWindow::thePanToTool_toggled( bool value)
{
   if(theController)
   {
      theController->panToToolButtonToggled(value);
   }
}

void ossimQtImageWindow::theZoomToolButton_toggled( bool value)
{
   if(theController)
   {
      theController->zoomToolButtonToggled(value);
   }
}



void ossimQtImageWindow::resamplerPropagateAction_activated()
{
   if(theController)
   {
      theController->propagateResampler();
   }
}


void ossimQtImageWindow::swipeAddLayerAction_activated()
{
   if(theController)
   {
      theController->swipeAddLayer();
   }
}


void ossimQtImageWindow::swipeRemoveLayerAction_activated()
{
   if(theController)
   {
      theController->swipeRemoveLayer();
   }
}


void ossimQtImageWindow::swipeBoxAction_activated()
{
   if(theController)
   {
      theController->swipeBox();
   }
}


void ossimQtImageWindow::swipeCircleAction_activated()
{
   if(theController)
   {
      theController->swipeCircle();
   }
}


void ossimQtImageWindow::swipeVerticalAction_activated()
{
   if(theController)
   {
      theController->swipeVertical();
   }
}


void ossimQtImageWindow::swipeHorizontalAction_activated()
{
   if(theController)
   {
      theController->swipeHorizontal();
   }
}


void ossimQtImageWindow::editImageChainAction_activated()
{
   if(theController)
   {
      theController->editImageChain();
   }
}

void ossimQtImageWindow::editImageInfoAction_activated()
{
   if(theController)
   {
      theController->editImageInfo();
   }
}

void ossimQtImageWindow::editLayersAction_activated()
{
   if(theController)
   {
      theController->editLayers();
   }
}


void ossimQtImageWindow::editParameterAdjustmentAction_activated()
{
   if(theController)
   {
      theController->geometryAdjustment();
   }
}


void ossimQtImageWindow::editVectorSourceAction_activated()
{
   if(theController)
   {
      theController->vectorSource();
   }
}


void ossimQtImageWindow::enhancementsFusionAction_activated()
{
   if(theController)
   {
      theController->fusion();
   }

}


void ossimQtImageWindow::theResamplerSelection_activated( const QString & value)
{
    if(theController)
    {
	theController->setResampler(value);
    }
}


void ossimQtImageWindow::editPositionInformationActionActivated()
{
    if(theController)
    {
	theController->showPositionInformation();
    }
}

void ossimQtImageWindow::editPositionQualityActionActivated()
{
    if(theController)
    {
	theController->showPositionQuality();
    }
}


void ossimQtImageWindow::refreshButtonClicked()
{
    if(theController)
    {
	theController->refreshWindow();
    }
}


void ossimQtImageWindow::enhancementTopographicCorrection_activated()
{
   if(theController)
   {
      theController->topographicCorrection();
   }

}

void Ui_ossimQtImageWindow::setupUi(QMainWindow *ossimQtImageWindow)
{
   if (ossimQtImageWindow->objectName().isEmpty())
      ossimQtImageWindow->setObjectName(QString::fromUtf8("ossimQtImageWindow"));

   ossimQtImageWindow->setEnabled(true);
   ossimQtImageWindow->resize(597, 545);
   ossimQtImageWindow->setMinimumSize(QSize(21, 290));
   ossimQtImageWindow->setFocusPolicy(Qt::StrongFocus);
   fileSaveAsAction = new QAction(ossimQtImageWindow);
   fileSaveAsAction->setObjectName(QString::fromUtf8("fileSaveAsAction"));
   fileSaveAsAction->setName("fileSaveAsAction");
   fileSaveAsAction->setEnabled(true);
   fileSaveAsAction->setIcon(QIcon());
   filePrintAction = new QAction(ossimQtImageWindow);
   filePrintAction->setObjectName(QString::fromUtf8("filePrintAction"));
   filePrintAction->setName("filePrintAction");
   filePrintAction->setEnabled(false);
   filePrintAction->setIcon(QIcon());
   fileExitAction = new QAction(ossimQtImageWindow);
   fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
   fileExitAction->setName("fileExitAction");
   editUndoAction = new QAction(ossimQtImageWindow);
   editUndoAction->setObjectName(QString::fromUtf8("editUndoAction"));
   editUndoAction->setName("editUndoAction");
   editUndoAction->setEnabled(false);
   editUndoAction->setIcon(QIcon());
   editRedoAction = new QAction(ossimQtImageWindow);
   editRedoAction->setObjectName(QString::fromUtf8("editRedoAction"));
   editRedoAction->setName("editRedoAction");
   editRedoAction->setEnabled(false);
   editRedoAction->setIcon(QIcon());
   helpContentsAction = new QAction(ossimQtImageWindow);
   helpContentsAction->setObjectName(QString::fromUtf8("helpContentsAction"));
   helpContentsAction->setName("helpContentsAction");
   helpContentsAction->setEnabled(false);
   helpIndexAction = new QAction(ossimQtImageWindow);
   helpIndexAction->setObjectName(QString::fromUtf8("helpIndexAction"));
   helpIndexAction->setName("helpIndexAction");
   helpIndexAction->setEnabled(false);
   helpAboutAction = new QAction(ossimQtImageWindow);
   helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
   helpAboutAction->setName("helpAboutAction");
   helpAboutAction->setEnabled(false);
   editViewToolAction = new QAction(ossimQtImageWindow);
   editViewToolAction->setObjectName(QString::fromUtf8("editViewToolAction"));
   editViewToolAction->setName("editViewToolAction");
   enhancementsHistogramOperationsAction = new QAction(ossimQtImageWindow);
   enhancementsHistogramOperationsAction->setObjectName(QString::fromUtf8("enhancementsHistogramOperationsAction"));
   enhancementsHistogramOperationsAction->setName("enhancementsHistogramOperationsAction");
   enhancementsHsiAction = new QAction(ossimQtImageWindow);
   enhancementsHsiAction->setObjectName(QString::fromUtf8("enhancementsHsiAction"));
   enhancementsHsiAction->setName("enhancementsHsiAction");
   enhancementsBrightnessContrastAction = new QAction(ossimQtImageWindow);
   enhancementsBrightnessContrastAction->setObjectName(QString::fromUtf8("enhancementsBrightnessContrastAction"));
   enhancementsBrightnessContrastAction->setName("enhancementsBrightnessContrastAction");
   igenExportAction = new QAction(ossimQtImageWindow);
   igenExportAction->setObjectName(QString::fromUtf8("igenExportAction"));
   igenExportAction->setName("igenExportAction");
   enhancementsBandSelectorAction = new QAction(ossimQtImageWindow);
   enhancementsBandSelectorAction->setObjectName(QString::fromUtf8("enhancementsBandSelectorAction"));
   enhancementsBandSelectorAction->setName("enhancementsBandSelectorAction");
   swipeAddLayerAction = new QAction(ossimQtImageWindow);
   swipeAddLayerAction->setObjectName(QString::fromUtf8("swipeAddLayerAction"));
   swipeAddLayerAction->setName("swipeAddLayerAction");
   swipeRemoveLayerAction = new QAction(ossimQtImageWindow);
   swipeRemoveLayerAction->setObjectName(QString::fromUtf8("swipeRemoveLayerAction"));
   swipeRemoveLayerAction->setName("swipeRemoveLayerAction");
   swipeRemoveLayerAction->setCheckable(false);
   swipeRemoveLayerAction->setEnabled(false);
   swipeHorizontalAction = new QAction(ossimQtImageWindow);
   swipeHorizontalAction->setObjectName(QString::fromUtf8("swipeHorizontalAction"));
   swipeHorizontalAction->setName("swipeHorizontalAction");
   swipeHorizontalAction->setCheckable(true);
   swipeHorizontalAction->setEnabled(false);
   swipeVerticalAction = new QAction(ossimQtImageWindow);
   swipeVerticalAction->setObjectName(QString::fromUtf8("swipeVerticalAction"));
   swipeVerticalAction->setName("swipeVerticalAction");
   swipeVerticalAction->setCheckable(true);
   swipeVerticalAction->setEnabled(false);
   swipeBoxAction = new QAction(ossimQtImageWindow);
   swipeBoxAction->setObjectName(QString::fromUtf8("swipeBoxAction"));
   swipeBoxAction->setName("swipeBoxAction");
   swipeBoxAction->setCheckable(true);
   swipeBoxAction->setEnabled(true);
   swipeCircleAction = new QAction(ossimQtImageWindow);
   swipeCircleAction->setObjectName(QString::fromUtf8("swipeCircleAction"));
   swipeCircleAction->setName("swipeCircleAction");
   swipeCircleAction->setCheckable(true);
   swipeCircleAction->setEnabled(false);
   editImageChainAction = new QAction(ossimQtImageWindow);
   editImageChainAction->setObjectName(QString::fromUtf8("editImageChainAction"));
   editImageChainAction->setName("editImageChainAction");
   editImageChainAction->setEnabled(true);
   editImageInfoAction = new QAction(ossimQtImageWindow);
   editImageInfoAction->setObjectName(QString::fromUtf8("editImageInfoAction"));
   editImageInfoAction->setName("editImageInfoAction");
   editImageInfoAction->setEnabled(true);
   editLayersAction = new QAction(ossimQtImageWindow);
   editLayersAction->setObjectName(QString::fromUtf8("editLayersAction"));
   editLayersAction->setName("editLayersAction");
   editLayersAction->setEnabled(true);
   editParameterAdjustmentAction = new QAction(ossimQtImageWindow);
   editParameterAdjustmentAction->setObjectName(QString::fromUtf8("editParameterAdjustmentAction"));
   editParameterAdjustmentAction->setName("editParameterAdjustmentAction");
   editVectorSourceAction = new QAction(ossimQtImageWindow);
   editVectorSourceAction->setObjectName(QString::fromUtf8("editVectorSourceAction"));
   editVectorSourceAction->setName("editVectorSourceAction");
   enhancementsFusionAction = new QAction(ossimQtImageWindow);
   enhancementsFusionAction->setObjectName(QString::fromUtf8("enhancementsFusionAction"));
   enhancementsFusionAction->setName("enhancementsFusionAction");
   editPositionInformationAction = new QAction(ossimQtImageWindow);
   editPositionInformationAction->setObjectName(QString::fromUtf8("editPositionInformationAction"));
   editPositionInformationAction->setName("editPositionInformationAction");
   editPositionQualityAction = new QAction(ossimQtImageWindow);
   editPositionQualityAction->setObjectName(QString::fromUtf8("editPositionQualityAction"));
   editPositionQualityAction->setName("editPositionQualityAction");
   enhancementTopographicCorrection = new QAction(ossimQtImageWindow);
   enhancementTopographicCorrection->setObjectName(QString::fromUtf8("enhancementTopographicCorrection"));
   enhancementTopographicCorrection->setName("enhancementTopographicCorrection");
   resamplerPropagateAction = new QAction(ossimQtImageWindow);
   resamplerPropagateAction->setObjectName(QString::fromUtf8("resamplerPropagateAction"));
   resamplerPropagateAction->setName("resamplerPropagateAction");
   widget = new QWidget(ossimQtImageWindow);
   widget->setObjectName(QString::fromUtf8("widget"));
   vboxLayout = new QVBoxLayout(widget);
   vboxLayout->setSpacing(0);
   vboxLayout->setMargin(0);
   vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
   vboxLayout->setContentsMargins(0, 0, 0, 0);
   theImageWidget = new ossimQtSwipeScrollingImageWidget(widget, "");
   theImageWidget->setObjectName(QString::fromUtf8("theImageWidget"));
   theImageWidget->setMouseTracking(true);

   vboxLayout->addWidget(theImageWidget);

   // menubar = new QMenuBar();
   // QMainWindow::setMenuBar(menubar);
   fileMenu = ossimQtImageWindow->menuBar()->addMenu(QObject::tr("&File"));
   fileMenu->addAction(fileSaveAsAction);
   fileMenu->addSeparator();
   fileMenu->addAction(filePrintAction);
   fileMenu->addSeparator();
   fileMenu->addAction(fileExitAction);

   editMenu = ossimQtImageWindow->menuBar()->addMenu(QObject::tr("&Edit"));
   editMenu->addAction(editUndoAction);
   editMenu->addAction(editRedoAction);
   editMenu->addSeparator();
   editMenu->addSeparator();
   editMenu->addAction(editViewToolAction);
   editMenu->addAction(editParameterAdjustmentAction);
   editMenu->addSeparator();
   editMenu->addAction(editImageChainAction);
   editMenu->addAction(editImageInfoAction);
   editMenu->addAction(editLayersAction);
   editMenu->addAction(editVectorSourceAction);
   editMenu->addSeparator();
   editMenu->addAction(editPositionInformationAction);
   editMenu->addAction(editPositionQualityAction);
   
   swipeMenu = ossimQtImageWindow->menuBar()->addMenu(QObject::tr("&Swipe"));
   swipeMenu->addAction(swipeAddLayerAction);
   swipeMenu->addAction(swipeRemoveLayerAction);
   swipeMenu->addAction(swipeHorizontalAction);
   swipeMenu->addAction(swipeVerticalAction);
   swipeMenu->addAction(swipeBoxAction);
   swipeMenu->addAction(swipeCircleAction);
   
   enhanceMenu = ossimQtImageWindow->menuBar()->addMenu(QObject::tr("E&nhancements"));
   enhanceMenu->addAction(enhancementsBandSelectorAction);
   enhanceMenu->addAction(enhancementsBrightnessContrastAction);
   enhanceMenu->addAction(enhancementTopographicCorrection);
   enhanceMenu->addAction(enhancementsHistogramOperationsAction);
   enhanceMenu->addAction(enhancementsHsiAction);
   enhanceMenu->addAction(enhancementsFusionAction);
   enhanceMenu->addAction(resamplerPropagateAction);
   
   helpMenu = ossimQtImageWindow->menuBar()->addMenu(QObject::tr("&Help"));
   helpMenu->addAction(helpContentsAction);
   helpMenu->addAction(helpIndexAction);
   helpMenu->addSeparator();
   helpMenu->addAction(helpAboutAction);

   ossimQtImageWindow->setCentralWidget(widget);

   Toolbar = new QToolBar();
   ossimQtImageWindow->addToolBar(Qt::TopToolBarArea, Toolbar);

   theZoomToolButton = new QToolButton();
   theZoomToolButton->setCheckable(true);
   Toolbar->addWidget(theZoomToolButton);

   thePanToToolButton = new QToolButton();
   thePanToToolButton->setCheckable(true);
   Toolbar->addWidget(thePanToToolButton);

   theFitToWindowButton = new QPushButton();
   Toolbar->addWidget(theFitToWindowButton);

   theFullResButton = new QPushButton();
   Toolbar->addWidget(theFullResButton);

   thePropagateButton = new QPushButton();
   Toolbar->addWidget(thePropagateButton);

   theRefreshButton = new QPushButton();
   Toolbar->addWidget(theRefreshButton);

   theResamplerSelection = new QComboBox();
   Toolbar->addWidget(theResamplerSelection);

   retranslateUi(ossimQtImageWindow);

   QObject::connect(editRedoAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editRedoAction_activated()));
   QObject::connect(editUndoAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editUndoAction_activated()));
   QObject::connect(editViewToolAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editViewToolAction_activated()));
   QObject::connect(enhancementsBandSelectorAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(enhancementsBandSelectorAction_activated()));
   QObject::connect(enhancementsHistogramOperationsAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(enhancementsHistogramOperationsAction_activated()));
   QObject::connect(enhancementsHsiAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(enhancementsHsiAction_activated()));
   QObject::connect(fileExitAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(fileExitAction_activated()));
   QObject::connect(fileSaveAsAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(fileSaveAsAction_activated()));
   QObject::connect(enhancementsBrightnessContrastAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(enhancementsBrightnessContrastAction_activated()));
   QObject::connect(theZoomToolButton, SIGNAL(toggled(bool)), ossimQtImageWindow, SLOT(theZoomToolButton_toggled(bool)));
   QObject::connect(thePropagateButton, SIGNAL(clicked()), ossimQtImageWindow, SLOT(thePropagateButton_clicked()));
   QObject::connect(theFullResButton, SIGNAL(clicked()), ossimQtImageWindow, SLOT(theFullResButton_clicked()));
   QObject::connect(theFitToWindowButton, SIGNAL(clicked()), ossimQtImageWindow, SLOT(theFitToWindowButton_clicked()));
   QObject::connect(thePanToToolButton, SIGNAL(toggled(bool)), ossimQtImageWindow, SLOT(thePanToTool_toggled(bool)));
   QObject::connect(swipeAddLayerAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(swipeAddLayerAction_activated()));
   QObject::connect(swipeRemoveLayerAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(swipeRemoveLayerAction_activated()));
   QObject::connect(swipeBoxAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(swipeBoxAction_activated()));
   QObject::connect(swipeCircleAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(swipeCircleAction_activated()));
   QObject::connect(swipeVerticalAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(swipeVerticalAction_activated()));
   QObject::connect(swipeHorizontalAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(swipeHorizontalAction_activated()));
   QObject::connect(editImageChainAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editImageChainAction_activated()));
   QObject::connect(editImageInfoAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editImageInfoAction_activated()));
   QObject::connect(editLayersAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editLayersAction_activated()));
   QObject::connect(editParameterAdjustmentAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editParameterAdjustmentAction_activated()));
   QObject::connect(editVectorSourceAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editVectorSourceAction_activated()));
   QObject::connect(enhancementsFusionAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(enhancementsFusionAction_activated()));
   QObject::connect(theResamplerSelection, SIGNAL(activated(QString)), ossimQtImageWindow, SLOT(theResamplerSelection_activated(QString)));
   QObject::connect(editPositionInformationAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editPositionInformationActionActivated()));
   QObject::connect(editPositionQualityAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(editPositionQualityActionActivated()));
   QObject::connect(theRefreshButton, SIGNAL(clicked()), ossimQtImageWindow, SLOT(refreshButtonClicked()));
   QObject::connect(resamplerPropagateAction, SIGNAL(activated()), ossimQtImageWindow, SLOT(resamplerPropagateAction_activated()));
   QObject::connect(enhancementTopographicCorrection, SIGNAL(activated()), ossimQtImageWindow, SLOT(enhancementTopographicCorrection_activated()));

   QMetaObject::connectSlotsByName(ossimQtImageWindow);
    
} // setupUi


void Ui_ossimQtImageWindow::retranslateUi(QMainWindow *ossimQtImageWindow)
{
   ossimQtImageWindow->setWindowTitle(QApplication::translate("ossimQtImageWindow", "OSSIM Image", 0, QApplication::UnicodeUTF8));
   fileSaveAsAction->setIconText(QApplication::translate("ossimQtImageWindow", "Save As", 0, QApplication::UnicodeUTF8));
   fileSaveAsAction->setText(QApplication::translate("ossimQtImageWindow", "Save &As...", 0, QApplication::UnicodeUTF8));
   fileSaveAsAction->setShortcut(QString());
   filePrintAction->setIconText(QApplication::translate("ossimQtImageWindow", "Print", 0, QApplication::UnicodeUTF8));
   filePrintAction->setText(QApplication::translate("ossimQtImageWindow", "&Print...", 0, QApplication::UnicodeUTF8));
   filePrintAction->setShortcut(QApplication::translate("ossimQtImageWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
   fileExitAction->setIconText(QApplication::translate("ossimQtImageWindow", "Exit", 0, QApplication::UnicodeUTF8));
   fileExitAction->setText(QApplication::translate("ossimQtImageWindow", "E&xit", 0, QApplication::UnicodeUTF8));
   fileExitAction->setShortcut(QString());
   editUndoAction->setIconText(QApplication::translate("ossimQtImageWindow", "Undo", 0, QApplication::UnicodeUTF8));
   editUndoAction->setText(QApplication::translate("ossimQtImageWindow", "&Undo", 0, QApplication::UnicodeUTF8));
   editUndoAction->setShortcut(QApplication::translate("ossimQtImageWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
   editRedoAction->setIconText(QApplication::translate("ossimQtImageWindow", "Redo", 0, QApplication::UnicodeUTF8));
   editRedoAction->setText(QApplication::translate("ossimQtImageWindow", "&Redo", 0, QApplication::UnicodeUTF8));
   editRedoAction->setShortcut(QApplication::translate("ossimQtImageWindow", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
   helpContentsAction->setIconText(QApplication::translate("ossimQtImageWindow", "Contents", 0, QApplication::UnicodeUTF8));
   helpContentsAction->setText(QApplication::translate("ossimQtImageWindow", "&Contents...", 0, QApplication::UnicodeUTF8));
   helpContentsAction->setShortcut(QString());
   helpIndexAction->setIconText(QApplication::translate("ossimQtImageWindow", "Index", 0, QApplication::UnicodeUTF8));
   helpIndexAction->setText(QApplication::translate("ossimQtImageWindow", "&Index...", 0, QApplication::UnicodeUTF8));
   helpIndexAction->setShortcut(QString());
   helpAboutAction->setIconText(QApplication::translate("ossimQtImageWindow", "About", 0, QApplication::UnicodeUTF8));
   helpAboutAction->setText(QApplication::translate("ossimQtImageWindow", "&About", 0, QApplication::UnicodeUTF8));
   helpAboutAction->setShortcut(QString());
   editViewToolAction->setIconText(QApplication::translate("ossimQtImageWindow", "View", 0, QApplication::UnicodeUTF8));
   editViewToolAction->setText(QApplication::translate("ossimQtImageWindow", "View", 0, QApplication::UnicodeUTF8));
   enhancementsHistogramOperationsAction->setIconText(QApplication::translate("ossimQtImageWindow", "Histogram operations", 0, QApplication::UnicodeUTF8));
   enhancementsHistogramOperationsAction->setText(QApplication::translate("ossimQtImageWindow", "Histogram operations", 0, QApplication::UnicodeUTF8));
   enhancementsHsiAction->setIconText(QApplication::translate("ossimQtImageWindow", "Hsi adjustments", 0, QApplication::UnicodeUTF8));
   enhancementsHsiAction->setText(QApplication::translate("ossimQtImageWindow", "Hsi adjustments", 0, QApplication::UnicodeUTF8));
   enhancementsBrightnessContrastAction->setIconText(QApplication::translate("ossimQtImageWindow", "Brightness contrast", 0, QApplication::UnicodeUTF8));
   enhancementsBrightnessContrastAction->setText(QApplication::translate("ossimQtImageWindow", "Brightness contrast", 0, QApplication::UnicodeUTF8));
   igenExportAction->setIconText(QApplication::translate("ossimQtImageWindow", "Image Generator", 0, QApplication::UnicodeUTF8));
   igenExportAction->setText(QApplication::translate("ossimQtImageWindow", "Image Generator", 0, QApplication::UnicodeUTF8));
   igenExportAction->setToolTip(QApplication::translate("ossimQtImageWindow", "Export using external igen application", 0, QApplication::UnicodeUTF8));
   enhancementsBandSelectorAction->setIconText(QApplication::translate("ossimQtImageWindow", "Band selection", 0, QApplication::UnicodeUTF8));
   enhancementsBandSelectorAction->setText(QApplication::translate("ossimQtImageWindow", "Band selection", 0, QApplication::UnicodeUTF8));
   swipeAddLayerAction->setIconText(QApplication::translate("ossimQtImageWindow", "Add layer", 0, QApplication::UnicodeUTF8));
   swipeAddLayerAction->setText(QApplication::translate("ossimQtImageWindow", "Add layer", 0, QApplication::UnicodeUTF8));
   swipeRemoveLayerAction->setIconText(QApplication::translate("ossimQtImageWindow", "Remove layer", 0, QApplication::UnicodeUTF8));
   swipeRemoveLayerAction->setText(QApplication::translate("ossimQtImageWindow", "Remove layer", 0, QApplication::UnicodeUTF8));
   swipeHorizontalAction->setIconText(QApplication::translate("ossimQtImageWindow", "Horizontal", 0, QApplication::UnicodeUTF8));
   swipeHorizontalAction->setText(QApplication::translate("ossimQtImageWindow", "Horizontal", 0, QApplication::UnicodeUTF8));
   swipeVerticalAction->setIconText(QApplication::translate("ossimQtImageWindow", "Vertical", 0, QApplication::UnicodeUTF8));
   swipeVerticalAction->setText(QApplication::translate("ossimQtImageWindow", "Vertical", 0, QApplication::UnicodeUTF8));
   swipeBoxAction->setIconText(QApplication::translate("ossimQtImageWindow", "Box", 0, QApplication::UnicodeUTF8));
   swipeBoxAction->setText(QApplication::translate("ossimQtImageWindow", "Box", 0, QApplication::UnicodeUTF8));
   swipeCircleAction->setIconText(QApplication::translate("ossimQtImageWindow", "Circle", 0, QApplication::UnicodeUTF8));
   swipeCircleAction->setText(QApplication::translate("ossimQtImageWindow", "Circle", 0, QApplication::UnicodeUTF8));
   editImageChainAction->setIconText(QApplication::translate("ossimQtImageWindow", "Image Chain", 0, QApplication::UnicodeUTF8));
   editImageChainAction->setText(QApplication::translate("ossimQtImageWindow", "Image Chain", 0, QApplication::UnicodeUTF8));
   editImageInfoAction->setIconText(QApplication::translate("ossimQtImageWindow", "Image Info", 0, QApplication::UnicodeUTF8));
   editImageInfoAction->setText(QApplication::translate("ossimQtImageWindow", "Image Info", 0, QApplication::UnicodeUTF8));
   editLayersAction->setIconText(QApplication::translate("ossimQtImageWindow", "Layers", 0, QApplication::UnicodeUTF8));
   editLayersAction->setText(QApplication::translate("ossimQtImageWindow", "Layers", 0, QApplication::UnicodeUTF8));
   editParameterAdjustmentAction->setIconText(QApplication::translate("ossimQtImageWindow", "Geometry adjustment", 0, QApplication::UnicodeUTF8));
   editParameterAdjustmentAction->setText(QApplication::translate("ossimQtImageWindow", "Geometry adjustment", 0, QApplication::UnicodeUTF8));
   editVectorSourceAction->setIconText(QApplication::translate("ossimQtImageWindow", "Vector", 0, QApplication::UnicodeUTF8));
   editVectorSourceAction->setText(QApplication::translate("ossimQtImageWindow", "Vector", 0, QApplication::UnicodeUTF8));
   enhancementsFusionAction->setIconText(QApplication::translate("ossimQtImageWindow", "Fusion", 0, QApplication::UnicodeUTF8));
   enhancementsFusionAction->setText(QApplication::translate("ossimQtImageWindow", "Fusion", 0, QApplication::UnicodeUTF8));
   editPositionInformationAction->setIconText(QApplication::translate("ossimQtImageWindow", "Position Information", 0, QApplication::UnicodeUTF8));
   editPositionInformationAction->setText(QApplication::translate("ossimQtImageWindow", "Position Information", 0, QApplication::UnicodeUTF8));
   editPositionQualityAction->setIconText(QApplication::translate("ossimQtImageWindow", "Position Quality", 0, QApplication::UnicodeUTF8));
   editPositionQualityAction->setText(QApplication::translate("ossimQtImageWindow", "Position Quality", 0, QApplication::UnicodeUTF8));
   enhancementTopographicCorrection->setIconText(QApplication::translate("ossimQtImageWindow", "Topographic correction", 0, QApplication::UnicodeUTF8));
   resamplerPropagateAction->setIconText(QApplication::translate("ossimQtImageWindow", "Propagate resampler", 0, QApplication::UnicodeUTF8));
   Toolbar->setLabel(QApplication::translate("ossimQtImageWindow", "Toolbar", 0, QApplication::UnicodeUTF8));
   theZoomToolButton->setText(QApplication::translate("ossimQtImageWindow", "Zoom", 0, QApplication::UnicodeUTF8));
   theZoomToolButton->setText(QApplication::translate("ossimQtImageWindow", "Zoom in", 0, QApplication::UnicodeUTF8));
   thePanToToolButton->setText(QApplication::translate("ossimQtImageWindow", "Pan", 0, QApplication::UnicodeUTF8));
   // Toolbar_2->setLabel(QApplication::translate("ossimQtImageWindow", "Toolbar_2", 0, QApplication::UnicodeUTF8));
   theFitToWindowButton->setText(QApplication::translate("ossimQtImageWindow", "Fit", 0, QApplication::UnicodeUTF8));
   theFullResButton->setText(QApplication::translate("ossimQtImageWindow", "Full res", 0, QApplication::UnicodeUTF8));
   thePropagateButton->setText(QApplication::translate("ossimQtImageWindow", "Propagate", 0, QApplication::UnicodeUTF8));
   theRefreshButton->setText(QApplication::translate("ossimQtImageWindow", "Refresh", 0, QApplication::UnicodeUTF8));
   theResamplerSelection->clear();
   theResamplerSelection->insertItems(0, QStringList()
                                      << QApplication::translate("ossimQtImageWindow", "Nearest neighbor", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Bilinear", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Cubic", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Bell", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Bessel", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Blackman", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Box", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Bspline", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Catrom", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Gaussian", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Hanning", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Hamming", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Hermite", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Lanczos", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Mitchell", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Quadtratic", 0, QApplication::UnicodeUTF8)
                                      << QApplication::translate("ossimQtImageWindow", "Sinc", 0, QApplication::UnicodeUTF8)
                                      );
   // fileMenu->setTitle(QApplication::translate("ossimQtImageWindow", "&File", 0, QApplication::UnicodeUTF8));
   // editMenu->setTitle(QApplication::translate("ossimQtImageWindow", "&Edit", 0, QApplication::UnicodeUTF8));
   // swipeMenu->setTitle(QApplication::translate("ossimQtImageWindow", "&Swipe", 0, QApplication::UnicodeUTF8));
   // enhanceMenu->setTitle(QApplication::translate("ossimQtImageWindow", "E&nhancements", 0, QApplication::UnicodeUTF8));
   // helpMenu->setTitle(QApplication::translate("ossimQtImageWindow", "&Help", 0, QApplication::UnicodeUTF8));
} // retranslateUi
