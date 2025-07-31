# Install script for directory: D:/Users/tim.tisler/tools/build/proj/include/proj

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/proj" TYPE FILE FILES
    "D:/Users/tim.tisler/tools/build/proj/include/proj/util.hpp"
    "D:/Users/tim.tisler/tools/build/proj/include/proj/metadata.hpp"
    "D:/Users/tim.tisler/tools/build/proj/include/proj/common.hpp"
    "D:/Users/tim.tisler/tools/build/proj/include/proj/coordinates.hpp"
    "D:/Users/tim.tisler/tools/build/proj/include/proj/crs.hpp"
    "D:/Users/tim.tisler/tools/build/proj/include/proj/datum.hpp"
    "D:/Users/tim.tisler/tools/build/proj/include/proj/coordinatesystem.hpp"
    "D:/Users/tim.tisler/tools/build/proj/include/proj/coordinateoperation.hpp"
    "D:/Users/tim.tisler/tools/build/proj/include/proj/io.hpp"
    "D:/Users/tim.tisler/tools/build/proj/include/proj/nn.hpp"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/users/tim.tisler/tools/build/proj/build/include/proj/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
