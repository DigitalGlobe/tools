#-------------------------------------------------------------------------------
MACRO (SZIP_SET_LIB_OPTIONS libtarget libname libtype)
  # message (STATUS "${libname} libtype: ${libtype}")
  HDF_SET_LIB_OPTIONS (${libtarget} ${libname} ${libtype})

  IF (${libtype} MATCHES "SHARED")
    IF (WIN32)
      SET (LIBSZIP_VERSION ${SZIP_PACKAGE_VERSION_MAJOR})
    ELSE (WIN32)
      SET (LIBSZIP_VERSION ${SZIP_PACKAGE_VERSION})
    ENDIF (WIN32)
    SET_TARGET_PROPERTIES (${libtarget} PROPERTIES VERSION ${LIBSZIP_VERSION})
    SET_TARGET_PROPERTIES (${libtarget} PROPERTIES SOVERSION ${LIBSZIP_VERSION})
  ENDIF (${libtype} MATCHES "SHARED")

  #-- Apple Specific install_name for libraries
  IF (APPLE)
    OPTION (SZIP_BUILD_WITH_INSTALL_NAME "Build with library install_name set to the installation path" OFF)
    IF (SZIP_BUILD_WITH_INSTALL_NAME)
      SET_TARGET_PROPERTIES(${libtarget} PROPERTIES
          LINK_FLAGS "-current_version ${SZIP_PACKAGE_VERSION} -compatibility_version ${SZIP_PACKAGE_VERSION}"
          INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
          BUILD_WITH_INSTALL_RPATH ${SZIP_BUILD_WITH_INSTALL_NAME}
      )
    ENDIF (SZIP_BUILD_WITH_INSTALL_NAME)
  ENDIF (APPLE)

ENDMACRO (SZIP_SET_LIB_OPTIONS)

