# - Find TIFF library
# Find the native TIFF includes and library
# This module defines
#  TIFF_INCLUDE_DIR, where to find tiff.h, etc.
#  TIFF_LIBRARIES, libraries to link against to use TIFF.
#  TIFF_FOUND, If false, do not try to use TIFF.
# also defined, but not for general use are
#  TIFF_LIBRARY, where to find the TIFF library.

#=============================================================================
# Copyright 2002-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(TIFF_INCLUDE_DIR tiff.h
          PATHS 
          ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include 
          /usr/local/include/tiff
          /usr/include/
          /usr/include/tiff)


SET(TIFF_NAMES ${TIFF_NAMES} tiff libtiff_i libtiff tiff3 libtiff3)

# Added x86_64-linux-gnu path for Ubuntu install
FIND_LIBRARY(TIFF_LIBRARY 
             NAMES ${TIFF_NAMES} 
             PATHS 
             ${CMAKE_INSTALL_PREFIX}/lib
             /usr/local/lib
             /usr/lib 
             /usr/lib/x86_64-linux-gnu)

# handle the QUIETLY and REQUIRED arguments and set TIFF_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TIFF  DEFAULT_MSG  TIFF_LIBRARY  TIFF_INCLUDE_DIR)

IF(TIFF_FOUND)
  SET( TIFF_LIBRARIES ${TIFF_LIBRARY} )
ENDIF(TIFF_FOUND)

MARK_AS_ADVANCED(TIFF_INCLUDE_DIR TIFF_LIBRARY)
