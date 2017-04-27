TEMPLATE	= lib
LANGUAGE	= C++
QT += opengl
CONFIG	+= qt warn_on release

DEFINES	+= _FILE_OFFSET_BITS=64 _LARGE_FILES

HEADERS	+=  ../include/ossimPlanetQt/ossimPlanetQtAboutDialog.h \
	../include/ossimPlanetQt/ossimPlanetQtActivityItem.h \
	../include/ossimPlanetQt/ossimPlanetQtActivityWidget.h \
	../include/ossimPlanetQt/ossimPlanetQtActivityThreadQueueItem.h \
	../include/ossimPlanetQt/ossimPlanetQtActivityWindow.h \
	../include/ossimPlanetQt/ossimPlanetQtAlertLayer.h \
	../include/ossimPlanetQt/ossimPlanetQtApplication.h \
	../include/ossimPlanetQt/ossimPlanetQtDropDownDelegate.h \
	../include/ossimPlanetQt/ossimPlanetQtImage.h \
	../include/ossimPlanetQt/ossimPlanetQtLayerOperation.h \
	../include/ossimPlanetQt/ossimPlanetQtLegend.h \
        ../include/ossimPlanetQt/ossimPlanetQtLegendAnimationPathItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendLocationInfo.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendNodeItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendTextureItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendKmlItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendVideoItem.h \
	../include/ossimPlanetQt/ossimPlanetQtMainWindow.h \
	../include/ossimPlanetQt/ossimPlanetQtMessageLog.h \
	../include/ossimPlanetQt/ossimPlanetQtNewHttpConnection.h \
	../include/ossimPlanetQt/ossimPlanetQtOperation.h \
	../include/ossimPlanetQt/ossimPlanetQtPreferences.h \
	../include/ossimPlanetQt/ossimPlanetQtGlWidget.h \
	../include/ossimPlanetQt/ossimPlanetQtActionAdapter.h \
	../include/ossimPlanetQt/ossimPlanetQtRuler.h \
	../include/ossimPlanetQt/ossimPlanetQtVersion.h \
	../include/ossimPlanetQt/ossimPlanetQtVideoControlWidget.h \
	../include/ossimPlanetQt/ossimPlanetQtWmsDialog.h 

SOURCES	+= core/ossimPlanetQtImage.cpp \
	gui/ossimPlanetQtAboutDialog.cpp \
	gui/ossimPlanetQtActivityItem.cpp \
	gui/ossimPlanetQtActivityThread.cpp \
	gui/ossimPlanetQtActivityThreadQueueItem.cpp \
	gui/ossimPlanetQtActivityWindow.cpp \
	gui/ossimPlanetQtActivityWidget.cpp \
	gui/ossimPlanetQtAlertLayer.cpp \
	gui/ossimPlanetQtApplication.cpp \
	gui/ossimPlanetQtDropDownDelegate.cpp \
	gui/ossimPlanetQtLayerOperation.cpp \
	gui/ossimPlanetQtLegend.cpp \
        gui/ossimPlanetQtLegendAnimationPathItem.cpp \
	gui/ossimPlanetQtLegendItem.cpp \
	gui/ossimPlanetQtLegendTextureItem.cpp \
	gui/ossimPlanetQtLegendNodeItem.cpp \
	gui/ossimPlanetQtLegendKmlItem.cpp \
	gui/ossimPlanetQtLegendVideoItem.cpp \
	gui/ossimPlanetQtMainWindow.cpp \
	gui/ossimPlanetQtMessageLog.cpp \
	gui/ossimPlanetQtNewHttpConnection.cpp \
	gui/ossimPlanetQtOperation.cpp \
	gui/ossimPlanetQtPreferences.cpp \
	gui/ossimPlanetQtGlWidget.cpp \
	gui/ossimPlanetQtRuler.cpp \
	gui/ossimPlanetQtVideoControlWidget.cpp \
	gui/ossimPlanetQtWmsDialog.cpp 


FORMS	= gui/ui/ossimPlanetQtAboutDialog.ui \
	  gui/ui/ossimPlanetQtActivityWindow.ui \
	  gui/ui/ossimPlanetQtLayerOperation.ui \
	  gui/ui/ossimPlanetQtMainWindow.ui \
	  gui/ui/ossimPlanetQtMessageLog.ui \
	  gui/ui/ossimPlanetQtNewHttpConnection.ui \
	  gui/ui/ossimPlanetQtPreferences.ui \ 
	  gui/ui/ossimPlanetQtRuler.ui \ 
	  gui/ui/ossimPlanetQtVideoControlWidget.ui \
	  gui/ui/ossimPlanetQtWmsDialog.ui 

###
# $Id: src.pro 16958 2010-04-02 19:41:41Z gpotts $
#
# NOTE:  
# You should set environment variables for include paths and linking:
# "OSSIM_HOME" 
# "OSSIM_QT_HOME"
# "LIBTIFF_HOME" Needed if tiff includes are not in standard place like 
#                /usr/include.
###


UI_HEADERS_DIR= ../include/ossimPlanetQt/
UI_SOURCE_DIR= gui/ui
MOC_DIR = moc
OBJECTS_DIR = obj

INCLUDEPATH += ../include
###
# $Id: src.pro 16958 2010-04-02 19:41:41Z gpotts $
#
# NOTE:  
# You should set environment variables for include paths and linking:
# "OSSIM_HOME"
# "OSSIM_QT_HOME"
# For windows all you need is
# EARTH_PACKAGE
###
VERSION         = 1.8.4
TARGET		= ossimPlanetQt
DESTDIR		= ../lib

win32 {
INCLUDEPATH += ..\..\ossim\include ..\..\ossimPlanet\include ..\..\libwms\include  ..\..\ossim_dependencies\include
TEMPLATE = vclib
  DEFINES += OSSIMPLANETQT_LIBRARY
 DEFINES +=  NOMINMAX
#  QMAKE_CXXFLAGS = -EHsc -GR
  LIBS	=  -L$(OSSIM_HOME)\lib -L$(OSSIM_HOME)\projects\vs\release\lib
LIBS += -L$(OSSIM_DEPENDENCIES)\lib  
LIBS += -losgText -losgGA -losgDB -losgViewer -losgUtil -losg -lossim -llibwms -lossimPlanet -lOpenThreads 
#  INCLUDEPATH += c:/msys/1.0/local/include/ossim c:/Devel/svn/ossim/ossimPlanet/include c:/msys/1.0/local/include
#  DESTDIR = $(EARTH_PACKAGE)\ossim_qt\lib
#  DEPENDPATH += $(EARTH_PACKAGE)\ossim\include
}

else:mac {

CONFIG += x86 x86_64

  QMAKE_LFLAGS += -F$(OSSIM_BUILD) -F/Library/Frameworks 
  QMAKE_CFLAGS += -F$(OSSIM_BUILD) 
  QMAKE_CXXFLAGS += -F$(OSSIM_BUILD) 
  LIBS += -framework ossimPlanet -framework ossim -framework wms -framework osgDB -framework osgGA -framework osgUtil -framework osgText -framework osgViewer -framework osg  -framework OpenThreads
 QMAKE_LFLAGS_SONAME  = -Wl,-install_name,@executable_path/../Frameworks/
  DEFINES += __USE_OSX_AGL_IMPLEMENTATION__ 
  CONFIG += lib_bundle
  FRAMEWORK_HEADERS.version = Versions
  FRAMEWORK_HEADERS.files +=../include/ossimPlanetQt/ossimPlanetQtApplication.h \
        ../include/ossimPlanetQt/ossimPlanetQtAboutDialog.h \
	../include/ossimPlanetQt/ossimPlanetQtActivityItem.h \
	../include/ossimPlanetQt/ossimPlanetQtActivityWidget.h \
	../include/ossimPlanetQt/ossimPlanetQtActivityThreadQueueItem.h \
	../include/ossimPlanetQt/ossimPlanetQtActivityWindow.h \
	../include/ossimPlanetQt/ossimPlanetQtAlertLayer.h \
	../include/ossimPlanetQt/ossimPlanetQtApplication.h \
	../include/ossimPlanetQt/ossimPlanetQtDropDownDelegate.h \
	../include/ossimPlanetQt/ossimPlanetQtImage.h \
	../include/ossimPlanetQt/ossimPlanetQtLayerOperation.h \
	../include/ossimPlanetQt/ossimPlanetQtLegend.h \
        ../include/ossimPlanetQt/ossimPlanetQtLegendAnimationPathItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendLocationInfo.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendNodeItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendTextureItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendKmlItem.h \
	../include/ossimPlanetQt/ossimPlanetQtLegendVideoItem.h \
	../include/ossimPlanetQt/ossimPlanetQtMainWindow.h \
	../include/ossimPlanetQt/ossimPlanetQtMessageLog.h \
	../include/ossimPlanetQt/ossimPlanetQtNewHttpConnection.h \
	../include/ossimPlanetQt/ossimPlanetQtOperation.h \
	../include/ossimPlanetQt/ossimPlanetQtPreferences.h \
	../include/ossimPlanetQt/ossimPlanetQtGlWidget.h \
	../include/ossimPlanetQt/ossimPlanetQtActionAdapter.h \
	../include/ossimPlanetQt/ossimPlanetQtRuler.h \
	../include/ossimPlanetQt/ossimPlanetQtVersion.h \
	../include/ossimPlanetQt/ossimPlanetQtVideoControlWidget.h \
	../include/ossimPlanetQt/ossimPlanetQtWmsDialog.h \
        ../include/ossimPlanetQt/ui_ossimPlanetQtMainWindow.h \
        ../include/ossimPlanetQt/ui_ossimPlanetQtPreferences.h
  FRAMEWORK_HEADERS.path = Headers
  QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
  TARGET=ossimPlanetQt
  target.path = $(OSSIM_BUILD)
}

else:unix {
  ###
  # Relies upon OSSIM_DEV_HOME environment variable being set.  Also this uses
  # OpenThreads.  Take out/edit as necessary.
  # 
  # You can edit "INSTALL_PREFIX" to point to install location for libraries.
  ###
  isEmpty(INSTALL_PREFIX){
      INSTALL_PREFIX=/usr/local
  }

  ###
  # LOCAL_ROOT A place where your local compiled stuff is installed like
  # libtiff, geotiff, OpenSceneGraph and so on...
  ###
  #LOCAL_ROOT=/usr/local

  # uncomment the next line if INSTALL_HERE defined
  # LOCAL_ROOT=$$(INSTALL_HERE)
  ###
  # DESTDIR Where the libraries go when you type "make".
  ###
  DESTDIR = ../lib

  ###
  # For "make clean" so we clean out the libraries in the DESTDIR.
  ###
  QMAKE_CLEAN += ../lib/lib*

  # uncomment next line if you have INSTALL_HERE defined
  # INSTALL_PREFIX=$$(INSTALL_HERE)
  ###
  # The make install target like "/usr/local"
  ###
  target.path = $$INSTALL_PREFIX/lib
  INSTALLS += target

  INCLUDEPATH += $$(OSSIM_DEV_HOME)/ossimPlanet/include $$(OSSIM_DEV_HOME)/ossim/include $$(LOCAL_ROOT)/include 

  LIBS += -L$$(OSSIM_DEV_HOME)/ossimPlanet/lib -L$$(OSSIM_DEV_HOME)/ossimPlanet/lib/Linux64 -L$$(OSSIM_DEV_HOME)/libwms/lib/Linux64 -L$$(OSSIM_DEV_HOME)/OpenSceneGraph-2.8.2/lib -L$$(OSSIM_DEV_HOME)/ossim/lib -L$$LOCAL_ROOT/lib -lossimPlanet -lwms -losgDB -losgGA -losgUtil -losgText -losg  -lossim -lOpenThreads -lgeotiff -ltiff -lossimPredator

  DEPENDPATH += $$(OSSIM_DEV_HOME)/ossimPlanet/include $$(OSSIM_DEV_HOME)/ossim/include
}
