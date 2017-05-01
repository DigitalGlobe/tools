
add_library(Qt5::WMFServicePlugin MODULE IMPORTED)

_populate_Multimedia_plugin_properties(WMFServicePlugin DEBUG "mediaservice/wmfengined.dll")

list(APPEND Qt5Multimedia_PLUGINS Qt5::WMFServicePlugin)
