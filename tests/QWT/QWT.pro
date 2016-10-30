QT += core gui
#QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = QWT
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QWTDIR = ../../src/QWT

LIBSDIR32 = ../../sdk/x86
LIBSDIR64 = ../../sdk/x64

INCLUDEPATH += ../../src/CppUnit/include
INCLUDEPATH += ../../src/CppUnit/lib
INCLUDEPATH += $$QWTDIR/src

CONFIG( debug, debug|release ) {
    # debug
    contains(QT_ARCH, i386) {
        LIBS += -L/../../sdk/x86 -lcppunit_d
        LIBS += -L$$LIBSDIR32/lib -lqwt_d
    }  else {
        LIBS += -L/../../sdk/x64 -lcppunit_d
        LIBS += -L$$LIBSDIR64/lib -lqwt_d
    }
} else {
    # release
    contains(QT_ARCH, i386) {
        LIBS += -L/../../sdk/x86 -lcppunit
        LIBS += -L$$LIBSDIR32/lib -lqwt
    } else {
        LIBS += -L/../../sdk/x64 -lcppunit
        LIBS += -L$$LIBSDIR64/lib -lqwt
    }
}


SOURCES += \
    RunApp.cpp \
    QWTRoutines.cpp \
    TestQWTRoutines.cpp

HEADERS += \
    QWTRoutines.h
