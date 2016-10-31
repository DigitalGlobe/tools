#----------------------------------------------------------------------
# File:    qxrunner.pro
# Purpose: qmake config file for the QxRunner library.
#----------------------------------------------------------------------

TEMPLATE = lib

include(../../qxconfig.pro)

TARGET = $$QX_RUNNERFILENAME    # From qxconfig

#----------------------------------------------------------------------
# OS Independent
#----------------------------------------------------------------------

#QX_LIBDIR = ../../lib           # Location of Qx libraries
QX_LIBDIR = $$OUT_PWD
message($$QX_LIBDIR)

RESOURCES   += resources/qxrunner.qrc
DEPENDPATH  += ../../include/qxrunner
INCLUDEPATH += . $$DEPENDPATH
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/include
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/lib
INCLUDEPATH += C:/DigitalGlobeTemp/tools/src/CppUnit/src/cppunit


#----------------------------------------------------------------------
# MS Windows
#----------------------------------------------------------------------

win32 {
    QMAKE_POST_LINK = $$winCopyLib()
    debug: QMAKE_CXXFLAGS_DEBUG += $$compilerOptions()
}

win32:dll {
    DEFINES += QXRUNNER_DLL_BUILD
}

#----------------------------------------------------------------------
# Linux/Unix
#----------------------------------------------------------------------

unix {
    DESTDIR = $$QX_LIBDIR       # Override from qxconfig
}

#----------------------------------------------------------------------

HEADERS = \
        ../../include/qxrunner/aboutdialog.h \
        ../../include/qxrunner/appsettings.h \
        ../../include/qxrunner/columnsdialog.h \
        ../../include/qxrunner/proxymodelcommon.h \
        ../../include/qxrunner/qxrunner_global.h \
        ../../include/qxrunner/resultsmodel.h \
        ../../include/qxrunner/resultsproxymodel.h \
        ../../include/qxrunner/resultsviewcontroller.h \
        ../../include/qxrunner/runner.h \
        ../../include/qxrunner/runneritem.h \
        ../../include/qxrunner/runnermodel.h \
        ../../include/qxrunner/runnermodelthread.h \
        ../../include/qxrunner/runnerproxymodel.h \
        ../../include/qxrunner/runnerviewcontroller.h \
        ../../include/qxrunner/runnerwindow.h \
        ../../include/qxrunner/runnerwindowclient.h \
        ../../include/qxrunner/settingsdialog.h \
        ../../include/qxrunner/statuswidget.h \
        ../../include/qxrunner/stoppingdialog.h \
        ../../include/qxrunner/utils.h \
        ../../include/qxrunner/viewcontrollercommon.h \

SOURCES = \
        aboutdialog.cpp \
        appsettings.cpp \
        columnsdialog.cpp \
        proxymodelcommon.cpp \
        qxrunner_global.cpp \
        resultsmodel.cpp \
        resultsproxymodel.cpp \
        resultsviewcontroller.cpp \
        runner.cpp \
        runneritem.cpp \
        runnermodel.cpp \
        runnermodelthread.cpp \
        runnerproxymodel.cpp \
        runnerviewcontroller.cpp \
        runnerwindow.cpp \
        runnerwindowclient.cpp \
        settingsdialog.cpp \
        statuswidget.cpp \
        stoppingdialog.cpp \
        utils.cpp \
        viewcontrollercommon.cpp

INTERFACES = \
        aboutdialog.ui \
        columnsdialog.ui \
        runnerwindow.ui \
        settingsdialog.ui \
        statuswidget.ui \
        stoppingdialog.ui
