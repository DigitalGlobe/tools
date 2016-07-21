###
# $Id: src.pro 16826 2010-03-08 18:47:32Z gpotts $
###
QT += qt3support
UI_DIR 		= ui
MOC_DIR 	= moc
OBJECTS_DIR 	= obj
UI_HEADERS_DIR  = ../include/ossimQt/
UI_SOURCE_DIR   = ui
VERSION         = 1.8.4
TARGET		= ossimQt
DESTDIR		= ../lib
TEMPLATE	= lib
LANGUAGE	= C++
CONFIG		+= qt warn_on release
# DEFINES		+= -DQT3_SUPPORT _FILE_OFFSET_BITS=64 _LARGE_FILES
DEFINES		+= QT3_SUPPORT

HEADERS	= ../include/ossimQt/ossimDataManager.h \
          ../include/ossimQt/ossimQtAboutDialogBase.h \
          ../include/ossimQt/ossimQtAdjustableParameterDialogController.h \
          ../include/ossimQt/ossimQtAdjustableParameterDialog.h \
          ../include/ossimQt/ossimQtAnnotatorBase.h \
          ../include/ossimQt/ossimQtApplicationUtility.h \
          ../include/ossimQt/ossimQtBandSelectorController.h \
          ../include/ossimQt/ossimQtBandSelectorDialog.h \
          ../include/ossimQt/ossimQtBrightnessContrastDialog.h \
          ../include/ossimQt/ossimQtCombinerEditorDialog.h \
          ../include/ossimQt/ossimQtCombinerEditorDialogController.h \
          ../include/ossimQt/ossimQtDataManagerDialog.h \
          ../include/ossimQt/ossimQtDataManagerListBox.h \
          ../include/ossimQt/ossimQtDatumConverterDialog.h \
          ../include/ossimQt/ossimQtDisplayUtility.h \
          ../include/ossimQt/ossimQtEditorWidgetManager.h \
          ../include/ossimQt/ossimQtElevationManagerController.h \
          ../include/ossimQt/ossimQtElevationManagerDialog.h \
          ../include/ossimQt/ossimQtEvent.h \
          ../include/ossimQt/ossimQtEventIds.h \
          ../include/ossimQt/ossimQtGeometryBuilderDialogController.h \
          ../include/ossimQt/ossimQtGeometryBuilderDialog.h \
          ../include/ossimQt/ossimQtHistogramBuilderDialog.h \
          ../include/ossimQt/ossimQtHistogramController.h \
          ../include/ossimQt/ossimQtHistogramDialog.h \
          ../include/ossimQt/ossimQtHistogramWidget.h \
          ../include/ossimQt/ossimQtHsiRemapperController.h \
          ../include/ossimQt/ossimQtHsiRemapperDialog.h \
          ../include/ossimQt/ossimQtIcpDialog.h \
          ../include/ossimQt/ossimQtIgenController.h \
          ../include/ossimQt/ossimQtIgenDialog.h \
          ../include/ossimQt/ossimQtImageChainEditorController.h \
          ../include/ossimQt/ossimQtImageChainEditorDialog.h \
          ../include/ossimQt/ossimQtImageInfoController.h \
          ../include/ossimQt/ossimQtImageInfoDialog.h \
          ../include/ossimQt/ossimQtImagelinkerAboutDialog.h \
          ../include/ossimQt/ossimQtImageWindow.h \
          ../include/ossimQt/ossimQtImageWindowControllerEventFilter.h \
          ../include/ossimQt/ossimQtImageWindowController.h \
          ../include/ossimQt/ossimQtInformationDialog.h \
          ../include/ossimQt/ossimQtIviewAboutDialog.h \
          ../include/ossimQt/ossimQtLayerChooser.h \
          ../include/ossimQt/ossimQtLayerEditorController.h \
          ../include/ossimQt/ossimQtLayerEditor.h \
          ../include/ossimQt/ossimQtMainWindowController.h \
          ../include/ossimQt/ossimQtMainWindow.h \
          ../include/ossimQt/ossimQtMainWindowWorkspace.h \
          ../include/ossimQt/ossimQtMatrixPropertyController.h \
          ../include/ossimQt/ossimQtMatrixPropertyDialog.h \
          ../include/ossimQt/ossimQtOverviewBuilderDialog.h \
          ../include/ossimQt/ossimQtPluginsDialog.h \
          ../include/ossimQt/ossimQtPositionQualityDialog.h \
          ../include/ossimQt/ossimQtPreferencesDialogController.h \
          ../include/ossimQt/ossimQtPreferencesDialog.h \
          ../include/ossimQt/ossimQtProgressDialog.h \
          ../include/ossimQt/ossimQtProject.h \
          ../include/ossimQt/ossimQtPropertyBooleanItem.h \
          ../include/ossimQt/ossimQtPropertyColorItem.h \
          ../include/ossimQt/ossimQtPropertyContainerItem.h \
          ../include/ossimQt/ossimQtPropertyDateItem.h \
          ../include/ossimQt/ossimQtPropertyDialog.h \
          ../include/ossimQt/ossimQtPropertyDialogController.h \
          ../include/ossimQt/ossimQtPropertyFactory.h \
          ../include/ossimQt/ossimQtPropertyFilenameItem.h \
          ../include/ossimQt/ossimQtPropertyFontItem.h \
          ../include/ossimQt/ossimQtPropertyItem.h \
          ../include/ossimQt/ossimQtPropertyListView.h \
          ../include/ossimQt/ossimQtPropertyMatrixItem.h \
          ../include/ossimQt/ossimQtPropertyNumericItem.h \
          ../include/ossimQt/ossimQtPropertyStringItem.h \
          ../include/ossimQt/ossimQtPropertyStringListItem.h \
          ../include/ossimQt/ossimQtPropertyTextItem.h \
          ../include/ossimQt/ossimQtQuadProjectionController.h \
          ../include/ossimQt/ossimQtQuadProjectionDialog.h \
          ../include/ossimQt/ossimQtRLevelFilterDialog.h \
          ../include/ossimQt/ossimQtRoiAnnotator.h \
          ../include/ossimQt/ossimQtRoiRectAnnotator.h \
          ../include/ossimQt/ossimQtScrollingImageWidget.h \
          ../include/ossimQt/ossimQtSingleImageWindow.h \
          ../include/ossimQt/ossimQtSliderTableItem.h \
          ../include/ossimQt/ossimQtStaticTileImageCache.h \
          ../include/ossimQt/ossimQtStringListPropertyController.h \
          ../include/ossimQt/ossimQtStringListPropertyDialog.h \
          ../include/ossimQt/ossimQtSwipeScrollingImageWidget.h \
          ../include/ossimQt/ossimQtTopographicCorrectionDialogController.h \
          ../include/ossimQt/ossimQtTopographicCorrectionDialog.h \
          ../include/ossimQt/ossimQtUnitConverterDialog.h \
          ../include/ossimQt/ossimQtVectorEditorDialogController.h \
          ../include/ossimQt/ossimQtVectorEditorDialog.h \
          ../include/ossimQt/ossimQtViewDialog.h \
          ../include/ossimQt/ossimQtViewDialogController.h \
          ../include/ossimQt/ossimQWidgetEditorFactory.h \
          ../include/ossimQt/ossimQWidgetFactoryInterface.h \
          ../include/ossimQt/ossimQWidgetFactoryRegistry.h

SOURCES	= ossimDataManager.cpp \
          ossimQtAboutDialogBase.cpp \
          ossimQtAdjustableParameterDialogController.cpp \
          ossimQtAdjustableParameterDialog.cpp \
          ossimQtAnnotatorBase.cpp \
          ossimQtApplicationUtility.cpp \
          ossimQtBandSelectorController.cpp \
          ossimQtBandSelectorDialog.cpp \
          ossimQtBrightnessContrastDialog.cpp \
          ossimQtCombinerEditorDialog.cpp \
          ossimQtCombinerEditorDialogController.cpp \
          ossimQtDataManagerDialog.cpp \
          ossimQtDataManagerListBox.cpp \
          ossimQtDatumConverterDialog.cpp \
          ossimQtDisplayUtility.cpp \
          ossimQtEditorWidgetManager.cpp \
          ossimQtElevationManagerController.cpp \
          ossimQtElevationManagerDialog.cpp \
          ossimQtEvent.cpp \
          ossimQtGeometryBuilderDialogController.cpp \
          ossimQtGeometryBuilderDialog.cpp \
          ossimQtHistogramBuilderDialog.cpp \
          ossimQtHistogramController.cpp \
          ossimQtHistogramDialog.cpp \
          ossimQtHistogramWidget.cpp \
          ossimQtHsiRemapperController.cpp \
          ossimQtHsiRemapperDialog.cpp \
          ossimQtIcpDialog.cpp \
          ossimQtIgenController.cpp \
          ossimQtIgenDialog.cpp \
          ossimQtImageChainEditorController.cpp \
          ossimQtImageChainEditorDialog.cpp \
          ossimQtImageInfoController.cpp \
          ossimQtImageInfoDialog.cpp \
          ossimQtImagelinkerAboutDialog.cpp \
          ossimQtImageWindow.cpp \
          ossimQtImageWindowControllerEventFilter.cpp \
          ossimQtImageWindowController.cpp \
          ossimQtInformationDialog.cpp \
          ossimQtIviewAboutDialog.cpp \
          ossimQtLayerChooser.cpp \
          ossimQtLayerEditorController.cpp \
          ossimQtLayerEditor.cpp \
          ossimQtMainWindowController.cpp \
          ossimQtMainWindow.cpp \
          ossimQtMainWindowWorkspace.cpp \
          ossimQtMatrixPropertyController.cpp \
          ossimQtMatrixPropertyDialog.cpp \
          ossimQtOverviewBuilderDialog.cpp \
          ossimQtPluginsDialog.cpp \
          ossimQtPositionQualityDialog.cpp \
          ossimQtPreferencesDialogController.cpp \
          ossimQtPreferencesDialog.cpp \
          ossimQtProgressDialog.cpp \
          ossimQtProject.cpp \
          ossimQtPropertyBooleanItem.cpp \
          ossimQtPropertyColorItem.cpp \
          ossimQtPropertyContainerItem.cpp \
          ossimQtPropertyDateItem.cpp \
          ossimQtPropertyDialog.cpp \
          ossimQtPropertyDialogController.cpp \
          ossimQtPropertyFactory.cpp \
          ossimQtPropertyFilenameItem.cpp \
          ossimQtPropertyFontItem.cpp\
          ossimQtPropertyItem.cpp \
          ossimQtPropertyListView.cpp \
          ossimQtPropertyMatrixItem.cpp \
          ossimQtPropertyNumericItem.cpp \
          ossimQtPropertyStringItem.cpp \
          ossimQtPropertyStringListItem.cpp \
          ossimQtPropertyTextItem.cpp \
          ossimQtQuadProjectionController.cpp \
          ossimQtQuadProjectionDialog.cpp \
          ossimQtRLevelFilterDialog.cpp \
          ossimQtRoiAnnotator.cpp \
          ossimQtRoiRectAnnotator.cpp \
          ossimQtScrollingImageWidget.cpp \
          ossimQtSingleImageWindow.cpp \
          ossimQtSliderTableItem.cpp \
          ossimQtStaticTileImageCache.cpp \
          ossimQtStringListPropertyDialog.cpp \
          ossimQtStringListPropertyController.cpp \
          ossimQtSwipeScrollingImageWidget.cpp \
          ossimQtTopographicCorrectionDialogController.cpp \
          ossimQtTopographicCorrectionDialog.cpp \
          ossimQtUnitConverterDialog.cpp \
          ossimQtVectorEditorDialogController.cpp \
          ossimQtVectorEditorDialog.cpp \
          ossimQtViewDialog.cpp \
          ossimQtViewDialogController.cpp \
          ossimQWidgetEditorFactory.cpp \
          ossimQWidgetFactoryInterface.cpp \
          ossimQWidgetFactoryRegistry.cpp
 
FORMS	=\
#	ui/ossimQtAdjustableParameter.ui \
#	ui/ossimQtBandSelectorDialog.ui \
#	ui/ossimQtCombinerEditorDialog.ui \
#	ui/ossimQtDataManagerDialog.ui \
#	ui/ossimQtDatumConverterDialog.ui \
#	ui/ossimQtElevationManager.ui \
#	ui/ossimQtGeometryBuilderDialog.ui \
#	ui/ossimQtHistogramDialog.ui \
#	ui/ossimQtHsiRemapperDialog.ui \
#	ui/ossimQtIgenDialog.ui \
#	ui/ossimQtImageChainEditor.ui \
#	ui/ossimQtImageInfoDialog.ui \
#	ui/ossimQtImageWindow.ui \
#	ui/ossimQtLayerChooser.ui \
#	ui/ossimQtLayerEditorDialog.ui \
#	ui/ossimQtMainWindow.ui \
#	ui/ossimQtPreferencesDialog.ui \
#	ui/ossimQtQuadProjectionDialog.ui \
#	ui/ossimQtTopographicCorrectionDialog.ui \
	ui/ossimQtUnitConverterDialog.ui 
#	ui/ossimQtVectorEditorDialog.ui \
#	ui/ossimQtViewDialog.ui 


###
# For INSTALL_PREFIX  and LOCAL_ROOT for "make" and "make install" target
###
include(../makedefs.pri)
target.path = $$INSTALL_PREFIX/lib
INSTALLS += target

win32 {
INCLUDEPATH += $(OSSIM_HOME)\include $(OSSIM_HOME)\..\ossim_qt4\include $(OSSIM_DEPENDENCIES)\include $(QT_HOME)\include
TEMPLATE = vclib
DEFINES +=  NOMINMAX
#QMAKE_CXXFLAGS = -EHsc -GR
DEFINES += OSSIMQT_LIBRARY
LIBS	=  -L$(OSSIM_HOME)\lib 
LIBS += -lossim  
}

else:macx {
  DEFINES += MACOSX
  LIBS += -L$(OSSIM_HOME)/lib/ossim -lossim -flat_namespace -undefined suppress
  INCLUDEPATH += $(OSSIM_HOME)/include /opt/local/include 
  DESTDIR = $(OSSIM_HOME)/../ossim_qt4/lib
  DEPENDPATH += $(OSSIM_HOME)/include
}

else:unix {
  ###
  # Relies upon OSSIM_DEV_HOME environment variable being set.
  # LOCAL_ROOT is set in top level makedefs.pri file.  
  # OpenThread and geotiff should be installed in $$LOCAL_ROOT
  # Take out/edit as necessary.
  ###

  INCLUDEPATH += $$(OSSIM_HOME)/include $$(OSSIM_HOME)/../ossim_qt4/include $$LOCAL_ROOT/include

  LIBS += -L$$(OSSIM_HOME)/lib -L$$LOCAL_ROOT/lib -lossim -lOpenThreads -lgeotiff -ltiff
  
  ###
  # DESTDIR Where the libraries go when you type "make".
  ###
  DESTDIR = ../lib

  ###
  # For "make clean" so we clean out the libraries in the DESTDIR.
  ###
  QMAKE_CLEAN += ../lib/lib*

  DEPENDPATH += $$(OSSIM_HOME)/include
}
