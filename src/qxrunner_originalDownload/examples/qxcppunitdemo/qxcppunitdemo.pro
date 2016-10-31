#----------------------------------------------------------------------
# File:    qxcppunitdemo.pro
# Purpose: qmake config file for the QxCppUnit library demo program.
#----------------------------------------------------------------------

TEMPLATE = app

include(../../qxconfig.pro)

TARGET = qxcppunitdemo

#----------------------------------------------------------------------
# OS Independent
#----------------------------------------------------------------------

QX_LIBDIR = ../../lib           # Location of Qx libraries

INCLUDEPATH += . ../../include $$(CPPUNIT)/include

#----------------------------------------------------------------------
# Libraries for linker.
#----------------------------------------------------------------------

LIBS += $$qxCppUnitLibForLinker()
LIBS += $$qxRunnerLibForLinker()
LIBS += $$cppUnitLibForLinker()

#----------------------------------------------------------------------
# MS Windows
#----------------------------------------------------------------------

win32 {
    RES_FILE = qxcppunitdemo.res
    debug: QMAKE_CXXFLAGS_DEBUG += $$compilerOptions()
}

win32:use_dll {
    DEFINES += QXCPPUNIT_DLL
    DEFINES += QXRUNNER_DLL
    DEFINES += CPPUNIT_DLL
}

#----------------------------------------------------------------------
# Linux/Unix
#----------------------------------------------------------------------

unix: {
    # NOP
}

#----------------------------------------------------------------------

SOURCES = \
        testexamples1.cpp \
        testexamples2.cpp \
        testexamples3.cpp \
        main.cpp
