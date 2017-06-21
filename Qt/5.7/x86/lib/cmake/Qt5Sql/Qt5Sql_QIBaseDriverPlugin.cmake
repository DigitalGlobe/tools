
add_library(Qt5::QIBaseDriverPlugin MODULE IMPORTED)

_populate_Sql_plugin_properties(QIBaseDriverPlugin RELEASE "sqldrivers/qsqlibase.dll")

list(APPEND Qt5Sql_PLUGINS Qt5::QIBaseDriverPlugin)
