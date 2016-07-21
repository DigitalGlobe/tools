PROJECT(ossim)
SET(CMAKE_MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/CMakeModules;$ENV{OSSIM_DEV_HOME}/ossim/CMakeModules;${CMAKE_MODULE_PATH}")
IF(NOT OSSIM_BUILD_ADDITIONAL_DIRECTORIES)
   SET(OSSIM_BUILD_ADDITIONAL_DIRECTORIES "" CACHE PATH "Specify full paths separated by ; to additional libraries you wish to include into the ossim build framework" FORCE)
ENDIF(NOT OSSIM_BUILD_ADDITIONAL_DIRECTORIES)
INCLUDE(OssimVersion)
INCLUDE(OssimUtilities)
INCLUDE(OssimCommonVariables)
MESSAGE( STATUS "OSSIM_INCLUDE_DIR               = ${${PROJECT_NAME}_SOURCE_DIR}" )

#set (SRTM_3ARC "${CMAKE_CURRENT_BINARY_DIR}/srtm_3arc.zip")
#set (SRTM_3ARC_URL "http://download.osgeo.org/ossim/dependencies/elevation/srtm_3arc.zip")

#IF(INSTALL_SRTM)

#  IF (NOT EXISTS "${SRTM_3ARC}")
#     MESSAGE( STATUS "DOWNLOAD SRTM FROM (This may take a few minutes)........${SRTM_3ARC_URL} TO LOCATION: ${SRTM_3ARC}" )
#     file(DOWNLOAD  "${SRTM_3ARC_URL}" "${SRTM_3ARC}")
#  endif()
#ENDIF(INSTALL_SRTM)


IF(NOT APPLE)
  cmake_minimum_required(VERSION 2.6)
ELSE(NOT APPLE)
    cmake_minimum_required(VERSION 2.8)
ENDIF(NOT APPLE)


if(NOT OSSIM_DEV_HOME)
   MESSAGE(FATAL_ERROR "Please pass in or define the OSSIM_DEV_HOME root path where all modules are located. This includes ossim, ossimPlanet, libwms, ams, etc.")
ENDIF(NOT OSSIM_DEV_HOME)

#IF(NOT APPLE)
#  cmake_minimum_required(VERSION 2.6)
#ELSE(NOT APPLE)
#  cmake_minimum_required(VERSION 2.8)
#ENDIF(NOT APPLE)

IF(EXISTS "$ENV{OSSIM_DEPENDENCIES}")
   SET(CMAKE_INCLUDE_PATH "$ENV{OSSIM_DEPENDENCIES}/include")
   SET(CMAKE_LIBRARY_PATH "$ENV{OSSIM_DEPENDENCIES}/lib")
   SET(CMAKE_FRAMEWORK_PATH "$ENV{OSSIM_DEPENDENCIES}/Frameworks")
ENDIF(EXISTS "$ENV{OSSIM_DEPENDENCIES}")

IF(NOT CMAKE_INSTALL_PREFIX)
    SET(CMAKE_INSTALL_PREFIX "$ENV{OSSIM_INSTALL_PREFIX}" CACHE STRING "Cache the install prefix" FORCE)
ENDIF(NOT CMAKE_INSTALL_PREFIX)

#######################################################################################################
#  Initialize the libraries to be the cmake targets and initialize the includes to the proper dev paths
#######################################################################################################

IF(NOT OSSIM_LIBRARY)
   SET(OSSIM_LIBRARY "ossim" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIM_LIBRARY)

IF(NOT OSSIM_HOME)
   SET(OSSIM_HOME "${OSSIM_DEV_HOME}/ossim" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIM_HOME)

IF(NOT OSSIM_INCLUDE_DIR)
   SET(OSSIM_INCLUDE_DIR "${OSSIM_HOME}/include" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIM_INCLUDE_DIR)

IF(NOT OSSIMC_LIBRARY)
   SET(OSSIMC_LIBRARY "ossimc" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIMC_LIBRARY)

IF(NOT OSSIMC_INCLUDE_DIR)
   SET(OSSIMC_INCLUDE_DIR "${OSSIM_DEV_HOME}/ossimc/include" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIMC_INCLUDE_DIR)

IF(NOT OSSIMPLANET_LIBRARY)
   SET(OSSIMPLANET_LIBRARY "ossimPlanet" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIMPLANET_LIBRARY)

IF(NOT OSSIMPLANET_INCLUDE_DIR)
   SET(OSSIMPLANET_INCLUDE_DIR "${OSSIM_DEV_HOME}/ossimPlanet/include" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIMPLANET_INCLUDE_DIR)

IF(NOT OSSIMPLANETQT_LIBRARY)
   SET(OSSIMPLANETQT_LIBRARY "ossimPlanetQt" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIMPLANETQT_LIBRARY)

IF(NOT OSSIMPLANETQT_INCLUDE_DIR)
   SET(OSSIMPLANETQT_INCLUDE_DIR "${OSSIM_DEV_HOME}/ossimPlanetQt/include" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIMPLANETQT_INCLUDE_DIR)

IF(NOT OSSIMQT_LIBRARY)
   SET(OSSIMQT_LIBRARY "ossimQt" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIMQT_LIBRARY)

IF(NOT OSSIMQT_INCLUDE_DIR)
   SET(OSSIMQT_INCLUDE_DIR "${OSSIM_DEV_HOME}//include" CACHE STRING "Caching" FORCE)
ENDIF(NOT OSSIMQT_INCLUDE_DIR)

IF(NOT OSSIMPREDATOR_LIBRARY AND BUILD_OSSIMPREDATOR)
    SET(OSSIMPREDATOR_LIBRARY "ossimPredator" CACHE STRING "Caching" FORCE)
ENDIF()
 

IF(NOT OSSIMPREDATOR_INCLUDE_DIR AND BUILD_OSSIMPREDATOR)
    SET(OSSIMPREDATOR_INCLUDE_DIR "${OSSIM_DEV_HOME}/ossimPredator/include" CACHE STRING "Caching" FORCE)
ENDIF()

IF(NOT CSMAPI_LIBRARY)
   SET(CSMAPI_LIBRARY "csmApi" CACHE STRING "Caching" FORCE)
ENDIF()

IF(NOT CSMAPI_INCLUDE_DIR)
   SET(CSMAPI_INCLUDE_DIR "${OSSIM_DEV_HOME}/csmApi/include" CACHE STRING "Caching" FORCE)
ENDIF(NOT CSMAPI_INCLUDE_DIR)

IF(NOT OMS_LIBRARY)
   SET(OMS_LIBRARY "oms" CACHE STRING "Caching" FORCE)
ENDIF(NOT OMS_LIBRARY)

IF(NOT OMS_INCLUDE_DIR)
   SET(OMS_INCLUDE_DIR "${OSSIM_DEV_HOME}/oms/coms/include" CACHE STRING "Caching" FORCE)
ENDIF(NOT OMS_INCLUDE_DIR)

IF(NOT WMS_LIBRARY)
   SET(WMS_LIBRARY "wms" CACHE STRING "Caching" FORCE)
ENDIF(NOT WMS_LIBRARY)

IF(NOT WMS_INCLUDE_DIR)
   SET(WMS_INCLUDE_DIR "$ENV{OSSIM_DEV_HOME}/libwms/include" CACHE STRING "Caching" FORCE)
ENDIF(NOT WMS_INCLUDE_DIR)


############################### Add subdirectories to build based on BUILD variables ###########################
IF(BUILD_WMS)
   add_subdirectory(${OSSIM_DEV_HOME}/libwms ${CMAKE_CURRENT_BINARY_DIR}/wms)
ENDIF()
add_subdirectory(${OSSIM_HOME} ${CMAKE_CURRENT_BINARY_DIR}/ossim) 
add_subdirectory(${OSSIM_DEV_HOME}/ossim_plugins ${CMAKE_CURRENT_BINARY_DIR}/ossim_plugins)

IF(BUILD_OSSIMPREDATOR AND EXISTS ${OSSIM_DEV_HOME}/ossimPredator)
   add_subdirectory(${OSSIM_DEV_HOME}/ossimPredator ${CMAKE_CURRENT_BINARY_DIR}/ossimPredator)
ENDIF()

IF(BUILD_OMS AND EXISTS ${OSSIM_DEV_HOME}/oms)
   add_subdirectory(${OSSIM_DEV_HOME}/oms ${CMAKE_CURRENT_BINARY_DIR}/oms)
ENDIF()

IF(BUILD_OSSIMC AND EXISTS ${OSSIM_DEV_HOME}/ossimc)
   add_subdirectory(${OSSIM_DEV_HOME}/ossimc ${CMAKE_CURRENT_BINARY_DIR}/ossimc)
ENDIF()


IF(BUILD_OSSIMJNI AND EXISTS ${OSSIM_DEV_HOME}/ossimjni)
   add_subdirectory(${OSSIM_DEV_HOME}/ossimjni ${CMAKE_CURRENT_BINARY_DIR}/ossimjni)
ENDIF()

IF(BUILD_OSSIMPLANET)
   add_subdirectory(${OSSIM_DEV_HOME}/ossimPlanet ${CMAKE_CURRENT_BINARY_DIR}/ossimPlanet)
ENDIF(BUILD_OSSIMPLANET)

IF(BUILD_OSSIMPLANET AND BUILD_OSSIMPLANETQT)
add_subdirectory(${OSSIM_DEV_HOME}/ossimPlanetQt ${CMAKE_CURRENT_BINARY_DIR}/ossimPlanetQt)
ENDIF(BUILD_OSSIMPLANET AND BUILD_OSSIMPLANETQT)

IF(BUILD_OSSIMQT4)
     add_subdirectory(${OSSIM_DEV_HOME}/ossim_qt4 ${CMAKE_CURRENT_BINARY_DIR}/ossimQt4)
ENDIF(BUILD_OSSIMQT4)

IF(BUILD_OSSIMGUI)
     add_subdirectory(${OSSIM_DEV_HOME}/ossimGui ${CMAKE_CURRENT_BINARY_DIR}/ossimGui)
ENDIF(BUILD_OSSIMGUI)

IF(OSSIM_BUILD_ADDITIONAL_DIRECTORIES)
    FOREACH(it IN LISTS OSSIM_BUILD_ADDITIONAL_DIRECTORIES)
        MESSAGE(STATUS "**********************Adding additional subdirectory ${it}*************************")
        SUBDIRS(${it})
    ENDFOREACH(it)
ENDIF()

IF(BUILD_CSMAPI)
    add_subdirectory(${OSSIM_DEV_HOME}/csmApi ${CMAKE_CURRENT_BINARY_DIR}/csmApi)
ENDIF()

############################## Doxygen installation for current ossim core moduel ###########################################
FIND_PACKAGE(Doxygen)

MACRO(OSSIM_GENERATE_DOXYGEN  DOXYGEN_OUTPUT_DIRECTORY CONFIGURE_FILE WORKING_DIRECTORY DOXYGEN_INPUT BASE_TARGET_NAME CLASS_PREFIX_TO_IGNORE) 
   
     SET(DOXYGEN_IGNORE_PREFIX "${CLASS_PREFIX_TO_IGNORE}")

     IF(NOT EXISTS "${DOXYGEN_OUTPUT_DIRECTORY}")
         make_directory("${DOXYGEN_OUTPUT_DIRECTORY}")
     ENDIF()
     CONFIGURE_FILE("${CONFIGURE_FILE}"
                    "${DOXYGEN_INPUT}" @ONLY IMMEIDATE)

     ADD_CUSTOM_COMMAND(
       OUTPUT ${DOXYGEN_OUTPUT_DIRECTORY}
       COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
       COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
       COMMAND ${CMAKE_COMMAND} -E echo "Done."
       WORKING_DIRECTORY ${WORKING_DIRECTORY}
       DEPENDS ${DOXYGEN_INPUT}
     )

     ADD_CUSTOM_TARGET(${BASE_TARGET_NAME}-doc ALL DEPENDS ${DOXYGEN_OUTPUT_DIRECTORY})

     ADD_CUSTOM_TARGET(${BASE_TARGET_NAME}-doc-forced
       COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
       COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
       COMMAND ${CMAKE_COMMAND} -E echo "Done."
       WORKING_DIRECTORY "${WORKING_DIRECTORY}")
ENDMACRO(OSSIM_GENERATE_DOXYGEN)

IF (DOXYGEN_FOUND)
  OPTION(OSSIM_BUILD_DOXYGEN "Set to ON to generate oxygen documentation files" OFF)
  IF(OSSIM_BUILD_DOXYGEN)

     SET(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doc/ossim-${OSSIM_VERSION}/ossimApi")
     OSSIM_GENERATE_DOXYGEN("${DOXYGEN_OUTPUT_DIRECTORY}"
                      "${OSSIM_DEV_HOME}/ossim/Doxyfile.in"
                      "${OSSIM_DEV_HOME}/ossim"
                      "${CMAKE_BINARY_DIR}/OssimDoxyfile"
                      "ossim-api"
                      "ossim")
     FILE(GLOB ossim_docs "${DOXYGEN_OUTPUT_DIRECTORY}/html/*")
     INSTALL(FILES ${ossim_docs} DESTINATION "share/ossim/${OSSIM_VERSION}/docs/doxygen/ossimapi/html" COMPONENT ossim-doc)

     IF(BUILD_OSSIMPLANET)
        SET(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doc/ossim-${OSSIM_VERSION}/ossimPlanetApi")
        OSSIM_GENERATE_DOXYGEN("${DOXYGEN_OUTPUT_DIRECTORY}"
                         "${OSSIM_DEV_HOME}/ossim/Doxyfile.in"
                         "${OSSIM_DEV_HOME}/ossimPlanet"
                         "${CMAKE_BINARY_DIR}/OssimPlanetDoxyfile"
                         "ossimPlanet-api"
                         "ossimPlanet")
        FILE(GLOB ossim_docs "${DOXYGEN_OUTPUT_DIRECTORY}/html/*")
        INSTALL(FILES ${ossim_docs} DESTINATION "share/ossim/${OSSIM_VERSION}/docs/doxygen/ossimPlanetApi/html" COMPONENT ossim-doc)
     ENDIF(BUILD_OSSIMPLANET)

   ENDIF(OSSIM_BUILD_DOXYGEN)
ENDIF (DOXYGEN_FOUND)

############################## Setup shared file installation ##############################
INSTALL(FILES ${OSSIM_DEV_HOME}/ossim_package_support/geoids/geoid1996/egm96.grd DESTINATION "share/ossim/geoids/geoid1996" COMPONENT ossim)

# Fonts removed for rpms
# INSTALL(FILES ${OSSIM_DEV_HOME}/ossim_package_support/fonts/arial.ttf DESTINATION "share/ossim/fonts" COMPONENT ossim)
# INSTALL(FILES ${OSSIM_DEV_HOME}/ossim_package_support/fonts/times.ttf DESTINATION "share/ossim/fonts" COMPONENT ossim)

FILE(GLOB ossim_cmake_modules ${OSSIM_DEV_HOME}/ossim_package_support/cmake/CmakeModules/*.cmake* )
INSTALL(FILES ${ossim_cmake_modules} DESTINATION share/ossim/${OSSIM_VERSION}/CMakeModules COMPONENT ossim)

############################## Setup the packages ###############################################################
# present the packaging option only if we have the cpack command defined (effectively >= 2.6.0)
IF(CMAKE_CPACK_COMMAND)
    OPTION(BUILD_OSSIM_PACKAGES "Set to ON to generate CPack configuration files and packaging targets" OFF)
    IF(BUILD_OSSIM_PACKAGES)
      INCLUDE(OssimCPack)
    ENDIF()
ENDIF()
SET(PKGCONFIG_FILES
ossim
)

FOREACH(PKGCONFIG_FILE ${PKGCONFIG_FILES})
  CONFIGURE_FILE(${OSSIM_DEV_HOME}/${PROJECT_NAME}/packaging/pkgconfig/${PKGCONFIG_FILE}.pc.in
    ${${PROJECT_NAME}_BINARY_DIR}/packaging/pkgconfig/${PKGCONFIG_FILE}.pc
    @ONLY
    )
  INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/packaging/pkgconfig/${PKGCONFIG_FILE}.pc DESTINATION lib${LIBSUFFIX}/pkgconfig COMPONENT ossim)
ENDFOREACH(PKGCONFIG_FILE)


#####################################################################################################
# We may want to add this as a build step instead so it can capture different output directories
# Will comment out for now until I figure this out.  This is used to symlink the pre built frameworks to the build
# dir so you can run things within Xcode
#
#####################################################################################################
#IF(APPLE)
#    IF(MAKE_APPENDS_BUILD_TYPE)
#        IF(OSSIM_DEPENDENCIES)
#             IF(NOT EXISTS ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/OpenThreads.framework AND EXISTS ${OSSIM_DEPENDENCIES}/Frameworks)
#                  FILE(GLOB framework_files "${OSSIM_DEPENDENCIES}/Frameworks/*.framework")
#                  foreach(x ${framework_files})
#                     execute_process(COMMAND ln -s  ${x} .
#                                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/)
#                  endforeach(x)
#             ENDIF()
#        ENDIF()
#    ENDIF()
#ENDIF()


############################ setup common INSTALL support files #############################################
# SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS "YES")
include(InstallRequiredSystemLibraries)
################################### SETUP Uninstall TARGETS ####################################################
OSSIM_ADD_COMMON_MAKE_UNINSTALL()

MESSAGE( STATUS "CMAKE_BUILD_TYPE                = ${CMAKE_BUILD_TYPE}" )
MESSAGE( STATUS "CMAKE_C_COMPILER                = ${CMAKE_C_COMPILER}" )
MESSAGE( STATUS "CMAKE_CXX_COMPILER              = ${CMAKE_CXX_COMPILER}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS                 = ${CMAKE_CXX_FLAGS}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG           = ${CMAKE_CXX_FLAGS_DEBUG}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE         = ${CMAKE_CXX_FLAGS_RELEASE}" )
MESSAGE( STATUS "CMAKE_CXX_DEFINES_DEBUG         = ${CXX_DEFINES}" )
MESSAGE( STATUS "CMAKE_CXX_DEFINES_RELEASE       = ${CMAKE_CXX_DEFINES}" )
MESSAGE( STATUS "CMAKE_DEBUG_POSTFIX             = ${CMAKE_DEBUG_POSTFIX}" )
MESSAGE( STATUS "CMAKE_INCLUDE_PATH              = ${CMAKE_INCLUDE_PATH}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX            = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "CMAKE_SHARED_LINKER_FLAGS       = ${CMAKE_SHARED_LINKER_FLAGS}" )
MESSAGE( STATUS "CMAKE_EXE_LINKER_FLAGS          = ${CMAKE_EXE_LINKER_FLAGS}" )
MESSAGE( STATUS "COMPILE_DEFINITIONS_DEBUG       = ${COMPILE_DEFINITIONS_DEBUG}" )
MESSAGE( STATUS "COMPILE_DEFINITIONS_RELEASE     = ${COMPILE_DEFINITIONS_RELEASE}" )
MESSAGE( STATUS "KAKADU_ROOT_SRC                 = ${KAKADU_ROOT_SRC}")
MESSAGE( STATUS "KAKADU_AUX_LIBRARY              = ${KAKADU_AUX_LIBRARY}")
MESSAGE( STATUS "KAKADU_LIBRARY                  = ${KAKADU_LIBRARY}")
MESSAGE( STATUS "MPI_LIBRARY                     = ${MPI_LIBRARIES}" )
MESSAGE( STATUS "MPI_INCLUDE                     = ${MPI_INCLUDE_PATH}" )
MESSAGE( STATUS "OPENTHREADS_LIBRARY             = ${OPENTHREADS_LIBRARY}" )
MESSAGE( STATUS "OPENTHREADS_INCLUDE             = ${OPENTHREADS_INCLUDE_DIR}" )
MESSAGE( STATUS "OSSIM_COMPILE_WITH_FULL_WARNING = ${OSSIM_COMPILE_WITH_FULL_WARNING}" )
MESSAGE( STATUS "OSSIM_COMMON_COMPILER_FLAGS     = ${OSSIM_COMMON_COMPILER_FLAGS}" )
MESSAGE( STATUS "OSSIM_LIBRARY                   = ${OSSIM_LIBRARY}" )
MESSAGE( STATUS "OSSIM_INCLUDE_DIR               = ${OSSIM_INCLUDE_DIR}" )
MESSAGE( STATUS "TIFF_LIBRARY                    = ${TIFF_LIBRARIES}" )
MESSAGE( STATUS "TIFF_INCLUDE                    = ${TIFF_INCLUDE_DIR}" )
MESSAGE( STATUS "ZLIB_LIBRARY                    = ${ZLIB_LIBRARIES}" )
MESSAGE( STATUS "ZLIB_INCLUDE                    = ${ZLIB_INCLUDE_DIR}" )
MESSAGE( STATUS "BUILD_OMS                       = ${BUILD_OMS}" )
MESSAGE( STATUS "BUILD_OSSIM                     = ${BUILD_OSSIM}" )
MESSAGE( STATUS "BUILD_OSSIM_FREETYPE_SUPPORT    = ${BUILD_OSSIM_FREETYPE_SUPPORT}" )
MESSAGE( STATUS "BUILD_OSSIMGUI                  = ${BUILD_OSSIMGUI}" )
MESSAGE( STATUS "BUILD_OSSIMJNI                  = ${BUILD_OSSIMJNI}" )
MESSAGE( STATUS "BUILD_OSSIMQT4                  = ${BUILD_OSSIMQT4}" )
MESSAGE( STATUS "BUILD_OSSIM_MPI_SUPPORT         = ${BUILD_OSSIM_MPI_SUPPORT}" )
MESSAGE( STATUS "BUILD_OSSIMPLANET               = ${BUILD_OSSIMPLANET}" )
MESSAGE( STATUS "BUILD_OSSIMPLANETQT             = ${BUILD_OSSIMPLANETQT}" )
MESSAGE( STATUS "BUILD_OSSIMPREDATOR             = ${BUILD_OSSIMPREDATOR}" )
MESSAGE( STATUS "BUILD_WMS                       = ${BUILD_WMS}" )
MESSAGE( STATUS "BUILD_LIBRARY_DIR               = ${BUILD_LIBRARY_DIR}" )
MESSAGE( STATUS "BUILD_RUNTIME_DIR               = ${BUILD_RUNTIME_DIR}" )
MESSAGE( STATUS "Use OSSIM_BUILD_ADDITIONAL_DIRECTORIES to add other cmake builds." )
