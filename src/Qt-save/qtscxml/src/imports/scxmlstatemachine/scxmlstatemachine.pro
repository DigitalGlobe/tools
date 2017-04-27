TARGET = scxml
TARGETPATH = QtScxml

QT = scxml qml-private core-private

SOURCES = \
    $$PWD/plugin.cpp \
    $$PWD/statemachineloader.cpp

HEADERS = \
    $$PWD/statemachineloader.h

load(qml_plugin)

OTHER_FILES += plugins.qmltypes qmldir
