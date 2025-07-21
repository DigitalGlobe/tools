#----------------------------------------------------------------------
# File:    qxcppunit.pro
# Purpose: qmake config file for the QxCppUnit library.
#----------------------------------------------------------------------

TEMPLATE = lib

include(../../qxconfig.pro)

TARGET = $$QX_CPPUNITFILENAME   # From qxconfig

#----------------------------------------------------------------------
# OS Independent
#----------------------------------------------------------------------

QX_LIBDIR = ../../lib           # Location of Qx libraries
QX_INCDIR = ../../include/qxcppunit

RESOURCES   += resources/qxcppunit.qrc
DEPENDPATH  += $$QX_INCDIR
INCLUDEPATH += . $$DEPENDPATH ../../include $$(CPPUNIT)/include

#----------------------------------------------------------------------
# Libraries for linker.
#----------------------------------------------------------------------

dll {
    # LIBS += $$qxRunnerLibForLinker()
    # LIBS += $$cppUnitLibForLinker()
}

#----------------------------------------------------------------------
# MS Windows
#----------------------------------------------------------------------

win32 {
    QMAKE_POST_LINK = $$winCopyLib()
    debug: QMAKE_CXXFLAGS_DEBUG += $$compilerOptions()
}

win32:dll {
    DEFINES += QXCPPUNIT_DLL_BUILD
    DEFINES += QXRUNNER_DLL
    DEFINES += CPPUNIT_DLL
}

#----------------------------------------------------------------------
# Linux/Unix
#----------------------------------------------------------------------

unix {
    DESTDIR = $$QX_LIBDIR       # Override from qxconfig
}

#----------------------------------------------------------------------

HEADERS = \
        $$QX_INCDIR/cppunititem.h \
        $$QX_INCDIR/cppunitmodel.h \
        $$QX_INCDIR/qxcppunit_global.h \
        $$QX_INCDIR/testrunner.h

SOURCES = \
        cppunititem.cpp \
        cppunitmodel.cpp \
        testrunner.cpp
