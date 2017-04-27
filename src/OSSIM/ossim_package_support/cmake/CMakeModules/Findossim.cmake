#---
# File: FindOSSIM.cmake
#
# Find OSSIM(Open Source Software Image Map) includes and libraries.
#
# This module defines:
# 
#  OSSIM_INCLUDE_DIR, Where to find ossimVersion.h, etc.
#  OSSIM_LIBRARIES, Libraries to link against to use OSSIM.
#  OSSIM_FOUND,  True if found, false if one of the above are not found.
# also defined, but not for general use are
#  OSSIM_LIBRARY, where to find the OSSIM library.
#---

#---
# Find include path:
#---
set(CMAKE_FIND_FRAMEWORK "LAST")
find_path(OSSIM_INCLUDE_DIR ossim/ossimVersion.h ossimVersion.h
   PATHS
      $ENV{OSSIM_DEV_HOME}/ossim/include
      $ENV{OSSIM_DEV_HOME}/ossim/latest/include
      $ENV{OSSIM_INSTALL_PREFIX}/include
      $ENV{OSSIM_DEV_HOME}/ossim
      $ENV{OSSIM_INSTALL_PREFIX}
   PATH_SUFFIXES 
      include
      lib
)

set(OSSIM_NAMES ${OSSIM_NAMES} ossim libossim)
find_library(OSSIM_LIBRARY NAMES ${OSSIM_NAMES}
   PATHS
      $ENV{OSSIM_INSTALL_PREFIX}/lib${LIBSUFFIX}
      $ENV{OSSIM_BUILD_DIR}/build_ossim/lib${LIBSUFFIX}
      $ENV{OSSIM_BUILD_DIR}/lib${LIBSUFFIX}
      $ENV{OSSIM_DEV_HOME}/build/build_ossim/lib${LIBSUFFIX}
      $ENV{OSSIM_DEV_HOME}/build/lib${LIBSUFFIX}
      $ENV{OSSIM_DEV_HOME}/ossim/lib${LIBSUFFIX}
      $ENV{OSSIM_INSTALL_PREFIX}
   PATH_SUFFIXES 
      lib
      Frameworks
)

#---
# This function sets OSSIM_FOUND if variables are valid.
#--- 
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args( OSSIM DEFAULT_MSG 
                                   OSSIM_LIBRARY 
                                   OSSIM_INCLUDE_DIR )

if(OSSIM_FOUND)
   set( OSSIM_LIBRARIES ${OSSIM_LIBRARY} )
   set( OSSIM_INCLUDES  ${OSSIM_INCLUDE_DIR} )
else( OSSIM_FOUND )
   if( NOT OSSIM_FIND_QUIETLY )
      message( WARNING "Could not find OSSIM" )
   endif( NOT OSSIM_FIND_QUIETLY )
endif(OSSIM_FOUND)

if( NOT OSSIM_FIND_QUIETLY )
   message( STATUS "OSSIM_INCLUDE_DIR=${OSSIM_INCLUDE_DIR}" )
   message( STATUS "OSSIM_LIBRARY=${OSSIM_LIBRARY}" )
endif( NOT OSSIM_FIND_QUIETLY )

MARK_AS_ADVANCED(OSSIM_INCLUDES OSSIM_INCLUDE_DIR OSSIM_LIBRARY)
