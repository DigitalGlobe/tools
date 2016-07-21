#---
# File ossimjni/cpp/CMakeLists.txt
# $Id: CMakeLists.txt 19748 2011-06-12 15:34:56Z dburken $
#---

message("************** begin ossimjni/cpp project cmake setup **************")

project(ossimjni)

cmake_minimum_required(VERSION 2.8)

find_package(ossim)
find_package(OpenThreads)

message( STATUS "OSSIM_INCLUDE_DIR       = ${OSSIM_INCLUDE_DIR}")
message( STATUS "OSSIM_LIBRARIES         = ${OSSIM_LIBRARIES}")
message( STATUS "OPENTHREADS_LIBRARY     = ${OPENTHREADS_LIBRARY}")
message( STATUS "OPENTHREADS_INCLUDE_DIR = ${OPENTHREADS_INCLUDE_DIR}")

############################## BUILD THE LIBRARY ##############################

set(LIB_NAME ossimjni)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${OSSIM_INCLUDE_DIR})
include_directories(${OPENTHREADS_INCLUDE_DIR})

file(GLOB OSSIMJNI_SRCS RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/src/ossimjni/*.cpp")
file(GLOB OSSIMJNI_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/ossimjni/*.h")

set(INSTALL_INCLUDE_DIR "include/ossimjni")
OSSIM_LINK_LIBRARY(${LIB_NAME} COMPONENT_NAME ossim TYPE "${OSSIM_USER_DEFINED_DYNAMIC_OR_STATIC}" HEADERS ${OSSIMJNI_HEADERS} SOURCE_FILES ${OSSIMJNI_SRCS} 
                   LIBRARIES ${OSSIM_LIBRARY} ${OPENTHREADS_LIBRARY}
                   INSTALL_LIB INSTALL_HEADERS VERSION_SYMLINKS ADDITIONAL_COMPILE_FLAGS "-DOSSIMJNI_LIBRARY -D__STDC_CONSTANT_MACROS")

##################################### SETUP TEST APPLICATION BUILDS #############################
if(BUILD_OSSIM_TEST_APPS)
   
endif(BUILD_OSSIM_TEST_APPS)

message("************** end ossimjni/cpp project cmake setup **************")
