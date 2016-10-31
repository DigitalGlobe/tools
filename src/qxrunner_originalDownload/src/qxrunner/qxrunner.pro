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

QX_LIBDIR = ../../lib           # Location of Qx libraries

RESOURCES   += resources/qxrunner.qrc
DEPENDPATH  += ../../include/qxrunner
INCLUDEPATH += . $$DEPENDPATH

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
        aboutdialog.h \
        appsettings.h \
        columnsdialog.h \
        proxymodelcommon.h \
        qxrunner_global.h \
        resultsmodel.h \
        resultsproxymodel.h \
        resultsviewcontroller.h \
        runner.h \
        runneritem.h \
        runnermodel.h \
        runnermodelthread.h \
        runnerproxymodel.h \
        runnerviewcontroller.h \
        runnerwindow.h \
        runnerwindowclient.h \
        settingsdialog.h \
        statuswidget.h \
        stoppingdialog.h \
        utils.h \
        viewcontrollercommon.h

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
