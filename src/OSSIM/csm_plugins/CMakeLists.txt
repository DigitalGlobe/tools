PROJECT(csm_plugins)
MESSAGE("**********************Setting up csm_plugins********************")
INCLUDE(OssimVersion)
INCLUDE(OssimUtilities)
INCLUDE(OssimCommonVariables)

OPTION(OSSIMPLANET_ENABLE_EPHEMERIS "Set to ON enable ephemeris support if GPSTK is found.  Use OFF to not enable ephemeris." ON)

IF(NOT APPLE)
  cmake_minimum_required(VERSION 2.6)
ELSE(NOT APPLE)
  cmake_minimum_required(VERSION 2.8)
ENDIF(NOT APPLE)

find_package(OpenThreads)
find_package(ossim)
find_package(csmApi)

INCLUDE_DIRECTORIES(${OSSIM_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OPENTHREADS_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CSMAPI_INCLUDE_DIRS})

MESSAGE( STATUS "OSSIM_INCLUDE_DIR         = ${OSSIM_INCLUDE_DIR}")
MESSAGE( STATUS "OSSIM_LIBRARIES           = ${OSSIM_LIBRARIES}")
MESSAGE( STATUS "CSMAPI_INCLUDE_DIR         = ${CSMAPI_INCLUDE_DIRS}")
MESSAGE( STATUS "CSMAPI_LIBRARIES           = ${CSMAPI_LIBRARIES}")

################################################## BUILD THE LIBRARY #######################################

SET(BUILD_OSSIM_FRAMEWORKS "OFF")
SET(OSSIM_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
SET(INSTALL_LIBRARY_DIR "lib/csmPlugins")
SET(BUILD_RUNTIME_DIR "csmPlugins")

STRING(COMPARE EQUAL "." "${CMAKE_CFG_INTDIR}" CMAKE_GENERATOR_TEST)
IF(CMAKE_GENERATOR_TEST)
   SET(BUILD_LIBRARY_DIR "csmPlugins/${CMAKE_BUILD_TYPE}")
   SET(BUILD_RUNTIME_DIR "csmPlugins/${CMAKE_BUILD_TYPE}")
ENDIF()


SET(LIB_NAME csmbuckeye_plugin)
FILE(GLOB file_srcs RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/src/buckeye/*.cpp")
FILE(GLOB file_headers "${CMAKE_CURRENT_SOURCE_DIR}/src/buckeye/*.h")

OSSIM_LINK_LIBRARY(${LIB_NAME} COMPONENT_NAME ossim TYPE "${OSSIM_USER_DEFINED_DYNAMIC_OR_STATIC}" HEADERS ${file_headers} SOURCE_FILES ${file_srcs} 
                   LIBRARIES ${OSSIM_LIBRARIES} ${CSMAPI_LIBRARIES}  ${OPENTHREADS_LIBRARY}
                   INSTALL_LIB )

####################################### SETUP THE UNINSTALL #################################
OSSIM_ADD_COMMON_MAKE_UNINSTALL()

