# Install script for directory: D:/Users/tim.tisler/tools/build/proj/data

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Users/tim.tisler/tools/build/proj/build/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/deformation_model.schema.json;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/projjson.schema.json;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/triangulation.schema.json;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/proj.ini;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/proj.db;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/world;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/other.extra;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/nad27;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/GL27;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/nad83;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/nad.lst;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/CH;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/ITRF2000;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/ITRF2008;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/ITRF2014;D:/Users/tim.tisler/tools/build/proj/build/install/share/proj/ITRF2020")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "D:/Users/tim.tisler/tools/build/proj/build/install/share/proj" TYPE FILE FILES
    "D:/Users/tim.tisler/tools/build/proj/data/deformation_model.schema.json"
    "D:/Users/tim.tisler/tools/build/proj/data/projjson.schema.json"
    "D:/Users/tim.tisler/tools/build/proj/data/triangulation.schema.json"
    "D:/Users/tim.tisler/tools/build/proj/data/proj.ini"
    "D:/users/tim.tisler/tools/build/proj/build/data/proj.db"
    "D:/Users/tim.tisler/tools/build/proj/data/world"
    "D:/Users/tim.tisler/tools/build/proj/data/other.extra"
    "D:/Users/tim.tisler/tools/build/proj/data/nad27"
    "D:/Users/tim.tisler/tools/build/proj/data/GL27"
    "D:/Users/tim.tisler/tools/build/proj/data/nad83"
    "D:/Users/tim.tisler/tools/build/proj/data/nad.lst"
    "D:/Users/tim.tisler/tools/build/proj/data/CH"
    "D:/Users/tim.tisler/tools/build/proj/data/ITRF2000"
    "D:/Users/tim.tisler/tools/build/proj/data/ITRF2008"
    "D:/Users/tim.tisler/tools/build/proj/data/ITRF2014"
    "D:/Users/tim.tisler/tools/build/proj/data/ITRF2020"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/users/tim.tisler/tools/build/proj/build/data/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
