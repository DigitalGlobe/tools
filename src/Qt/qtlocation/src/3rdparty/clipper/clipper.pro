TARGET = clipper

CONFIG += staticlib
CONFIG += exceptions


load(qt_helper_lib)

# workaround for QTBUG-31586
contains(QT_CONFIG, c++11): CONFIG += c++11

*-g++* {
    QMAKE_CXXFLAGS += -O3 -ftree-vectorize -ffast-math -funsafe-math-optimizations -Wno-error=return-type
}

HEADERS += clipper.h
SOURCES += clipper.cpp
