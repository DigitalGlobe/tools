cmake_minimum_required (VERSION 2.8)
PROJECT (SZIP_SRC C)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (${SZIP_SRC_SOURCE_DIR})
INCLUDE_DIRECTORIES (${CMAKE_BINARY_DIR})

#-----------------------------------------------------------------------------
# Define szip Library
#-----------------------------------------------------------------------------
SET (SZIP_SRCS
    ${SZIP_SRC_SOURCE_DIR}/rice.c
    ${SZIP_SRC_SOURCE_DIR}/sz_api.c
    ${SZIP_SRC_SOURCE_DIR}/encoding.c
)

SET (SZIP_HDRS
    ${SZIP_SRC_SOURCE_DIR}/ricehdf.h
)

SET (SZIP_PUBLIC_HEADERS
    ${SZIP_SRC_SOURCE_DIR}/szip_adpt.h
    ${SZIP_SRC_SOURCE_DIR}/szlib.h
)

ADD_LIBRARY (${SZIP_LIB_TARGET} ${LIB_TYPE} ${SZIP_SRCS} ${SZIP_HDRS} ${SZIP_PUBLIC_HEADERS})
#SET_TARGET_PROPERTIES (${SZIP_LIB_TARGET}
#  PROPERTIES
#      LINK_FLAGS "-version-info 2:0:0"
#) 
SET_TARGET_PROPERTIES(${SZIP_LIB_TARGET} PROPERTIES DEFINE_SYMBOL ${SZIP_LIB_CORENAME}_EXPORTS)
TARGET_LINK_LIBRARIES (${SZIP_LIB_TARGET} ${LINK_LIBS})
SET_GLOBAL_VARIABLE (SZIP_LIBRARIES_TO_EXPORT ${SZIP_LIB_TARGET})
SZIP_SET_LIB_OPTIONS (${SZIP_LIB_TARGET} ${SZIP_LIB_NAME} ${LIB_TYPE})

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install 
#-----------------------------------------------------------------------------
INSTALL (
    FILES
        ${SZIP_PUBLIC_HEADERS}
    DESTINATION
        ${SZIP_INSTALL_INCLUDE_DIR}
    COMPONENT
        headers
)

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
IF (SZIP_EXPORTED_TARGETS)
  INSTALL (
      TARGETS
          ${SZIP_LIB_TARGET}
      EXPORT
          ${SZIP_EXPORTED_TARGETS}
      LIBRARY DESTINATION ${SZIP_INSTALL_LIB_DIR} COMPONENT libraries
      ARCHIVE DESTINATION ${SZIP_INSTALL_LIB_DIR} COMPONENT libraries
      RUNTIME DESTINATION ${SZIP_INSTALL_BIN_DIR} COMPONENT libraries
  )
ENDIF (SZIP_EXPORTED_TARGETS)
