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

RESOURCES   += resources/qxcppunit.qrc
DEPENDPATH  += ../../include/qxcppunit
INCLUDEPATH += . $$DEPENDPATH ../../include $$(CPPUNIT)/include

#----------------------------------------------------------------------
# Libraries for linker.
#----------------------------------------------------------------------

dll {
    LIBS += $$qxRunnerLibForLinker()
    LIBS += $$cppUnitLibForLinker()
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
        cppunititem.h \
        cppunitmodel.h \
        qxcppunit_global.h \
        testrunner.h

SOURCES = \
        cppunititem.cpp \
        cppunitmodel.cpp \
        testrunner.cpp
