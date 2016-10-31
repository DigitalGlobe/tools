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

#QX_LIBDIR = ../../lib           # Location of Qx libraries
QX_LIBDIR = $$OUT_PWD
message($$QX_LIBDIR)

RESOURCES   += resources/qxcppunit.qrc
DEPENDPATH  += ../../include/qxcppunit
INCLUDEPATH += . $$DEPENDPATH ../../include $$(CPPUNIT)/include
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/include
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/lib
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/src/cppunit
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/src/cppunit/config


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
        ../../include/qxcppunit/cppunititem.h \
        ../../include/qxcppunit/cppunitmodel.h \
        ../../include/qxcppunit/qxcppunit_global.h \
        ../../include/qxcppunit/testrunner.h


SOURCES = \
        cppunititem.cpp \
        cppunitmodel.cpp \
        testrunner.cpp

DISTFILES += \
    resources/qxcppunit_64x64.png \
