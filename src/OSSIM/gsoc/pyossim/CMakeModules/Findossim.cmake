# - Find ossim library
# Find the native ossim includes and library
# This module defines
#  OSSIM_INCLUDE_DIR, where to find tiff.h, etc.
#  OSSIM_LIBRARIES, libraries to link against to use OSSIM.
#  OSSIM_FOUND, If false, do not try to use OSSIM.
# also defined, but not for general use are
#  OSSIM_LIBRARY, where to find the OSSIM library.
SET(CMAKE_FIND_FRAMEWORK "LAST")
FIND_PATH(OSSIM_INCLUDE_DIR ossim/ossimVersion.h ossimVersion.h
	HINTS 
		$ENV{OSSIM_DEV_HOME}/ossim/include
		$ENV{OSSIM_DEV_HOME}/ossim/lib
		$ENV{OSSIM_INSTALL_PREFIX}/include
	PATHS
		$ENV{OSSIM_DEV_HOME}/ossim
		$ENV{OSSIM_INSTALL_PREFIX}
    	PATH_SUFFIXES 
		lib
		include
)

SET(OSSIM_NAMES ${OSSIM_NAMES} ossim libossim)
FIND_LIBRARY(OSSIM_LIBRARY NAMES ${OSSIM_NAMES}
	HINTS 
		$ENV{OSSIM_DEV_HOME}/ossim/lib
	PATHS
		$ENV{OSSIM_DEV_HOME}/ossim/lib
		$ENV{OSSIM_INSTALL_PREFIX}
                $ENV{OSSIM_DEV_HOME}/build/lib
    	PATH_SUFFIXES 
		lib
		Frameworks
)

# handle the QUIETLY and REQUIRED arguments and set OSSIM_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OSSIM  DEFAULT_MSG  OSSIM_LIBRARY  OSSIM_INCLUDE_DIR)

IF(OSSIM_FOUND)
  SET( OSSIM_LIBRARIES ${OSSIM_LIBRARY} )
  SET( OSSIM_INCLUDES  ${OSSIM_INCLUDE_DIR} )
ENDIF(OSSIM_FOUND)

MARK_AS_ADVANCED(OSSIM_INCLUDES  OSSIM_INCLUDE_DIR OSSIM_LIBRARY)
