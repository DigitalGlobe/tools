###
# $Id: ossimplanet.pro 16846 2010-03-09 12:18:40Z gpotts $
#
###
target= ossimplanet
TEMPLATE	= app
LANGUAGE	= C++
CONFIG	+= qt warn_on release 

DEFINES	+= _FILE_OFFSET_BITS=64 _LARGE_FILES HAS_OPEN_THREADS=1 HAVE_CONFIG_H MPI_SETTING=0 GEOTIFF_ENABLED FREETYPE_SETTING=1

INCLUDEPATH	+= ../../include 
SOURCES	= main.cpp
target.path = ../../bin
RESOURCES = ../ossimPlanetQt.qrc

win32 {
INCLUDEPATH += ..\..\..\ossim\include ..\..\..\ossimPlanet\include ..\..\..\libwms\include ..\..\..\ossim_dependencies\include   
TEMPLATE = vcapp
  LIBS	=  -L..\..\lib -L$(OSSIM_HOME)\lib -L$(OSSIM_HOME)\projects\vs\release\lib
LIBS += -L$(OSSIM_DEPENDENCIES)\lib  
LIBS += -L..\..\..\ossim_dependencies\lib  
LIBS += -lossimPlanetQt1 -losgText -losgGA -losgDB -losgUtil -losg -lossim -llibwms -lossimPlanet -lOpenThreads 
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

else:mac {
  QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.5.sdk
  QMAKE_LFLAGS += -F$(OSSIM_BUILD) -F/Library/Frameworks -F$(OSSIM_DEV_HOME)/ossimPlanetQt/src/build/Release
  QMAKE_CFLAGS += -F$(OSSIM_BUILD) -mmacosx-version-min=10.5
  QMAKE_CXXFLAGS += -F$(OSSIM_BUILD) -mmacosx-version-min=10.5
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  QMAKE_CLEAN += 
  LIBS += -framework ossimPlanetQt -framework ossimPlanet -framework ossim -framework wms -framework osgDB -framework osgGA -framework osgUtil -framework osgText -framework osgViewer -framework osg  -framework OpenThreads
  target.path = $(OSSIM_BUILD)
}

else:unix {

  ###
  # Relies upon OSSIM_DEV_HOME environment variable being set.  Also this uses
  # OpenThreads.  Take out/edit as necessary.
  ###

  ###
  # LOCAL_ROOT A place where your local compiled stuff is installed like
  # libtiff, geotiff, OpenSceneGraph and so on...
  ###
#  LOCAL_ROOT=/usr/local

  ###
  # DESTDIR Where the executable goes when you type "make".
  ###
  DESTDIR = ../../bin

  ###
  # INSTALL_PREFIX Point to install location for libraries.
  ### 
 # INSTALL_PREFIX=/usr/local
  isEmpty(INSTALL_PREFIX){
      INSTALL_PREFIX=/usr/local
  }

  ###
  # The make install target like "/usr/local"
  ###
  target.path = $$INSTALL_PREFIX/bin
  INSTALLS += target

  QMAKE_CLEAN += ossimplanet ../../bin/ossimplanet
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj

  INCLUDEPATH += $$(OSSIM_DEV_HOME)/ossimPlanetQt/src $$(OSSIM_DEV_HOME)/ossimPlanet/include $$(OSSIM_DEV_HOME)/ossim/include $$LOCAL_ROOT/include

  ###
  # LIBS with predator
  ###
  # LIBS = -L$$(OSSIM_DEV_HOME)/ossimPlanetQt/lib -L$$(OSSIM_DEV_HOME)/ossim/lib -L$$LOCAL_ROOT/lib -lossimPlanetQt -lossimPlanet -lossimPredator -lavformat -lavcodec -lavdevice -lavutil

  ###
  # LIBS without predator
  ###
  LIBS = -L$$(OSSIM_DEV_HOME)/ossimPlanetQt/lib -L$$(OSSIM_DEV_HOME)/ossimPlanet/lib/Linux64 -L$$(OSSIM_DEV_HOME)/libwms/lib/Linux64 -L$$(OSSIM_DEV_HOME)/OpenSceneGraph-2.8.2/lib -L$$(OSSIM_DEV_HOME)/ossim/lib -L$$LOCAL_ROOT/lib -lossimPlanetQt -lossimPlanet -lwms
}

# INSTALLS = target
