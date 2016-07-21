###############################################################################
#
# CMake module to search for Oracle client library (OCI)
#
# On success, the macro sets the following variables:
# ORACLE_FOUND       = if the library found
# ORACLE_LIBRARY     = full path to the library
# ORACLE_LIBRARIES   = full path to the library
# ORACLE_INCLUDE_DIR = where to find the library headers also defined,
#                       but not for general use are
# ORACLE_VERSION     = version of library which was found, e.g. "1.2.5"
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
# Developed with inspiration from Petr Vanek <petr@scribus.info>
# who wrote similar macro for TOra - http://torasql.com/
#
# Module source: http://github.com/mloskot/workshop/tree/master/cmake/
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
###############################################################################

if(ORACLE_INCLUDE_DIR AND (ORACLE_LIBRARIES OR ORACLE_LIBRARY))
  # Already in cache, be silent
  set(ORACLE_FIND_QUIETLY TRUE)
endif()

if(NOT ORACLE_FIND_QUIETLY)
  message(STATUS "Searching for Oracle ${Oracle_FIND_VERSION}+ OCI client library")
endif()

if(NOT DEFINED ENV{ORACLE_HOME})
  message(FATAL_ERROR "Missing environment variable ORACLE_HOME with location of Oracle installation")
endif()

set(ORACLE_HOME $ENV{ORACLE_HOME})

find_path(ORACLE_INCLUDE_DIR
  oci.h
  PATHS
  ${ORACLE_HOME}/rdbms/public
  ${ORACLE_HOME}/include
  ${ORACLE_HOME}/sdk/include
  ${ORACLE_HOME}/OCI/include )

set(ORACLE_OCI_NAMES clntsh libclntsh oci)
set(ORACLE_NNZ_NAMES nnz10 libnnz10 nnz11 libnnz11 ociw32)
set(ORACLE_OCCI_NAMES libocci occi oraocci10 oraocci11)

set(ORACLE_LIB_DIR 
  ${ORACLE_HOME}/lib
  ${ORACLE_HOME}/OCI/lib/MSVC)

find_library(ORACLE_OCI_LIBRARY  NAMES ${ORACLE_OCI_NAMES} PATHS ${ORACLE_LIB_DIR})
if (NOT MSVC)
find_library(ORACLE_OCCI_LIBRARY NAMES ${ORACLE_OCCI_NAMES} PATHS ${ORACLE_LIB_DIR})
find_library(ORACLE_NNZ_LIBRARY NAMES ${ORACLE_NNZ_NAMES} PATHS ${ORACLE_LIB_DIR})
endif(NOT MSVC)

set(ORACLE_LIBRARY ${ORACLE_OCI_LIBRARY} ${ORACLE_OCCI_LIBRARY} ${ORACLE_NNZ_LIBRARY})

if(APPLE)
  set(ORACLE_OCIEI_NAMES libociei ociei)

  find_library(ORACLE_OCIEI_LIBRARY
    NAMES libociei ociei
    PATHS ${ORACLE_LIB_DIR})

  if(ORACLE_OCIEI_LIBRARY)
    set(ORACLE_LIBRARY ${ORACLE_LIBRARY} ${ORACLE_OCIEI_LIBRARY})
  else(ORACLE_OCIEI_LIBRARY)
    message(STATUS "libociei.dylib is not found. It may cause crash if you are building BUNDLE")
  endif()
endif()

set(ORACLE_LIBRARIES ${ORACLE_LIBRARY})

# Handle the QUIETLY and REQUIRED arguments and set ORACLE_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ORACLE DEFAULT_MSG ORACLE_LIBRARY ORACLE_INCLUDE_DIR)
