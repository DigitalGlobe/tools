PROJECT(ossimPredator)
Message("################## Setting up ossimPredator library #########################")

INCLUDE(OssimVersion)
INCLUDE(OssimUtilities)
INCLUDE(OssimCommonVariables)

IF(NOT APPLE)
  cmake_minimum_required(VERSION 2.6)
ELSE(NOT APPLE)
  cmake_minimum_required(VERSION 2.8)
ENDIF(NOT APPLE)

INCLUDE_DIRECTORIES(${${PROJECT_NAME}_SOURCE_DIR}/include)

# Stores list of libs to link with. Initialized throughout.
set( dependentLibs )

# FFMPEG - Required:
find_package( FFmpeg )
if( FFMPEG_FOUND )
   include_directories( ${FFMPEG_INCLUDE_DIRS} )
   set( dependentLibs ${dependentLibs} ${FFMPEG_LIBRARIES} )
else( FFMPEG_FOUND )
   message( FATAL_ERROR "Could not find required ffmpeg package!" )
endif( FFMPEG_FOUND )

# OpenThreads - Required:
find_package( OpenThreads )
if( OPENTHREADS_FOUND )
   include_directories( ${OPENTHREADS_INCLUDE_DIR} )
   set( dependentLibs ${dependentLibs} ${OPENTHREADS_LIBRARY} )
else( OPENTHREADS_FOUND )
   message( FATAL_ERROR "Could not find required OpenThreads package!" )
endif( OPENTHREADS_FOUND )

# Ossim - Required:
find_package( ossim )
if( OSSIM_FOUND )
   include_directories( ${OSSIM_INCLUDE_DIR} )
   set( dependentLibs ${dependentLibs} ${OSSIM_LIBRARY} )
else( OSSIM_FOUND )
   message( FATAL_ERROR "Could not find required Ossim package!" )
endif( OSSIM_FOUND )

# Zlib - Required:
find_package( ZLIB )
if( ZLIB_FOUND )
   include_directories( ${ZLIB_INCLUDE_DIR} )
   set( dependentLibs ${dependentLibs} ${ZLIB_LIBRARY} )
else( ZLIB_FOUND )
   message( FATAL_ERROR "Could not find required Zlib package!" )
endif( ZLIB_FOUND )

# bzip2 - Optional
find_package( BZIP2 )
if( BZIP2_FOUND )
   include_directories( ${BZIP2_INCLUDE_DIR} )
   set( dependentLibs ${dependentLibs} ${BZIP2_LIBRARY} )
else( BZIP2_FOUND )
   message( WARNING "Could not find bzip2 package! Consider installing this as it will soon become a required package." )
endif( BZIP2_FOUND )

MESSAGE( STATUS "OSSIM_INCLUDE_DIR=${OSSIM_INCLUDE_DIR}")
MESSAGE( STATUS "OSSIM_LIBRARIES =${OSSIM_LIBRARIES}")
MESSAGE( STATUS "FFMPEG_LIBRARIES = ${FFMPEG_LIBRARIES}")
MESSAGE( STATUS "FFMPEG_INCLUDE_DIRS = ${FFMPEG_INCLUDE_DIRS}")
MESSAGE( STATUS "OPENTHREADS_LIBRARY = ${OPENTHREADS_LIBRARY}")
MESSAGE( STATUS "OPENTHREADS_INCLUDE_DIR = ${OPENTHREADS_INCLUDE_DIR}")
MESSAGE( STATUS "ZLIB_LIBRARY        = ${ZLIB_LIBRARY}" )
MESSAGE( STATUS "ZLIB_INCLUDE        = ${ZLIB_INCLUDE_DIR}" )
MESSAGE( STATUS "BZIP2_LIBRARY        = ${BZIP2_LIBRARY}" )
MESSAGE( STATUS "BZIP2_INCLUDE        = ${BZIP2_INCLUDE_DIR}" )
MESSAGE( STATUS "dependent libraries= ${dependentLibraries}" )

################################################## BUILD THE LIBRARY #######################################
SET(LIB_NAME ossimPredator)


INCLUDE_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/include)
INCLUDE_DIRECTORIES(${FFMPEG_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENTHREADS_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OSSIM_INCLUDE_DIR})

IF(BZIP2_FOUND)
   INCLUDE_DIRECTORIES(${BZIP2_INCLUDE_DIR})
ELSE(BZIP2_FOUND)
   SET(BZIP2_LIBRARIES "")
ENDIF(BZIP2_FOUND)

FILE(GLOB OSSIMPREDATOR_SRCS RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB OSSIMPREDATOR_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/ossimPredator/*.h")

set(INSTALL_INCLUDE_DIR "include/ossimPredator")
OSSIM_LINK_LIBRARY(${LIB_NAME} COMPONENT_NAME ossim TYPE "${OSSIM_USER_DEFINED_DYNAMIC_OR_STATIC}" HEADERS ${OSSIMPREDATOR_HEADERS} SOURCE_FILES ${OSSIMPREDATOR_SRCS} 
                   LIBRARIES ${dependentLibs}
                   INSTALL_LIB INSTALL_HEADERS VERSION_SYMLINKS ADDITIONAL_COMPILE_FLAGS "-DOSSIMPREDATOR_LIBRARY")
set(INCLUDE_DIR "include")

####################################### SETUP THE UNINSTALL #################################
OSSIM_ADD_COMMON_MAKE_UNINSTALL()

message("################## end of ossimPredator library setup #########################")
