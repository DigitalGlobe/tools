###
# $Id: imagelinker.pro 11920 2007-10-29 02:12:56Z dburken $
#
# NOTE:  
# You should set environment variables for include paths and linking:
# "OSSIM_HOME" 
# "OSSIM_QT_HOME"
# "EARTH_PACKAGE"
#
# Example qmake on linux:
# /usr/lib/qt4/bin/qmake-qt4 -unix -o Makefile imagelinker.pro
###
QT 	+= qt3support
DEFINES	+= QT3_SUPPORT

TEMPLATE	= app
LANGUAGE	= C++
CONFIG	+= qt warn_on release
RESOURCES = ../app.qrc

target = imagelinker
DESTDIR = ../../bin

###
# For make install target.
###
include(../../makedefs.pri)
target.path = $$INSTALL_PREFIX/bin
INSTALLS += target

INCLUDEPATH	+= ../../src ../../src/.ui ../../src/.moc

SOURCES	= imagelinker.cpp

win32 {
  QMAKE_CLEAN += imagelinker.e* imagelinker.l ..\..\bin\imagelinker.*  
INCLUDEPATH += ..\..\include ..\..\..\ossim\include ..\..\..\ossimPlanet\include ..\..\..\libwms\include ..\..\..\ossim_dependencies\include  
TEMPLATE = vcapp
LIBS += -L$(OSSIM_HOME)\lib 
LIBS	+=  -L..\..\lib  
LIBS += -lossimQt1 -lossim
CONFIG += embed_manifest_exe
DEFINES +=  NOMINMAX
#  QMAKE_CLEAN += ossimplanet.e* ossimplanet.l* ..\..\bin\ossimplanet.*  
#  DEFINES += OSSIMUSINGDLL
#  QMAKE_CXXFLAGS = -EHsc -GR

#  LIBS = -L$(EARTH_PACKAGE)/earth_package_solution/lib -L$(EARTH_PACKAGE)/ossim_qt/lib ../../lib/ossim_qt.lib -lossim -lopengl32 -lglu32 -lopengl32 -lglu32

#target.path = $(EARTH_PACKAGE)\earth_package_solution\bin
#DEPENDPATH += $(EARTH_PACKAGE)\ossim_qt\src $(EARTH_PACKAGE)\ossim\include
#INCLUDEPATH += $(EARTH_PACKAGE)\ossim\include

}

else:macx {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  QMAKE_CLEAN += imagelinker ../../bin/imagelinker
   LIBS = -bind_at_load -L$(OSSIM_DEV_HOME)/ossim_qt/lib -L$(OSSIM_DEV_HOME)/ossim/lib/ossim -L/opt/local/lib -lossim_qt -lossim
  INCLUDEPATH += $(OSSIM_DEV_HOME)/ossim/include /opt/local/include
  target.path = $(OSSIM_DEV_HOME)/ossim_qt4/bin
  DEPENDPATH += $(OSSIM_DEV_HOME)/ossim_qt4/include $(OSSIM_DEV_HOME)/ossim/include
  INCLUDEPATH	+= $(OSSIM_DEV_HOME)/ossim/include $(OSSIM_DEV_HOME)/ossim_qt4/include
  target.path = $(OSSIM_DEV_HOME)/ossim_qt/bin
}

else:unix {
###
# Relies upon OSSIM_DEV_HOME environment variable being set.
# LOCAL_ROOT is set in top level makedefs.pri file.  
# OpenThread and geotiff should be installed in $LOCAL_ROOT
# Take out/edit as necessary.
###

QMAKE_CLEAN += imagelinker ../../bin/imagelinker
UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj



LIBS = -L$$(OSSIM_DEV_HOME)/ossim_qt4/lib -L$$(OSSIM_DEV_HOME)/ossim/lib -L$$LOCAL_ROOT/lib -lossimQt -lossim -lOpenThreads -lgeotiff -ltiff

DEFINES += OSSIMQT_USE_WINDOWS_STYLE=1

DEPENDPATH += $$(OSSIM_DEV_HOME)/ossim_qt4/src $$(OSSIM_DEV_HOME)/ossim/include

INCLUDEPATH	= $$(OSSIM_DEV_HOME)/ossim_qt4/include $$(OSSIM_DEV_HOME)/ossim/include $$LOCAL_ROOT/include

}

