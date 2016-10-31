#----------------------------------------------------------------------
# File:    qxrunnerdemo.pro
# Purpose: qmake config file for the QxRunner library demo program.
#----------------------------------------------------------------------

TEMPLATE = app

include(../../qxconfig.pro)

TARGET = qxrunnerdemo

#----------------------------------------------------------------------
# OS Independent
#----------------------------------------------------------------------

#QX_LIBDIR = ../../lib           # Location of Qx libraries
QX_LIBDIR = $$OUT_PWD
message($$QX_LIBDIR)

INCLUDEPATH += . ../../include
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/include
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/lib
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/src/cppunit

#----------------------------------------------------------------------
# Libraries for linker.
#----------------------------------------------------------------------

LIBS += $$qxRunnerLibForLinker()

#----------------------------------------------------------------------
# MS Windows
#----------------------------------------------------------------------

win32 {
    RES_FILE = qxrunnerdemo.res
    debug: QMAKE_CXXFLAGS_DEBUG += $$compilerOptions()
}

win32:use_dll {
    DEFINES += QXRUNNER_DLL
}

#----------------------------------------------------------------------
# Linux/Unix
#----------------------------------------------------------------------

unix: {
    # NOP
}

#----------------------------------------------------------------------

HEADERS = \
        demoitem.h \
        demomodel.h

SOURCES = \
        demoitem.cpp \
        demomodel.cpp \
        main.cpp
