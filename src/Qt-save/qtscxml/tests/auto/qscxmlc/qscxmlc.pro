QT = core testlib scxml-private
CONFIG += testcase console c++11
CONFIG -= app_bundle

TARGET = tst_qscxmlc
TEMPLATE = app

RESOURCES += tst_qscxmlc.qrc

include(../../../tools/qscxmlc/qscxmlc.pri)

INCLUDEPATH += ../../../tools/qscxmlc/

SOURCES += \
    tst_qscxmlc.cpp
