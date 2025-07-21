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
QX_INCDIR = ../../include/qxrunner

RESOURCES   += resources/qxrunner.qrc
DEPENDPATH  += $$QX_INCDIR
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
        $$QX_INCDIR/aboutdialog.h \
        $$QX_INCDIR/appsettings.h \
        $$QX_INCDIR/columnsdialog.h \
        $$QX_INCDIR/proxymodelcommon.h \
        $$QX_INCDIR/qxrunner_global.h \
        $$QX_INCDIR/resultsmodel.h \
        $$QX_INCDIR/resultsproxymodel.h \
        $$QX_INCDIR/resultsviewcontroller.h \
        $$QX_INCDIR/runner.h \
        $$QX_INCDIR/runneritem.h \
        $$QX_INCDIR/runnermodel.h \
        $$QX_INCDIR/runnermodelthread.h \
        $$QX_INCDIR/runnerproxymodel.h \
        $$QX_INCDIR/runnerviewcontroller.h \
        $$QX_INCDIR/runnerwindow.h \
        $$QX_INCDIR/runnerwindowclient.h \
        $$QX_INCDIR/settingsdialog.h \
        $$QX_INCDIR/statuswidget.h \
        $$QX_INCDIR/stoppingdialog.h \
        $$QX_INCDIR/utils.h \
        $$QX_INCDIR/viewcontrollercommon.h

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

FORMS = \
        aboutdialog.ui \
        columnsdialog.ui \
        runnerwindow.ui \
        settingsdialog.ui \
        statuswidget.ui \
        stoppingdialog.ui
