PROJECT(csmApi)
MESSAGE("**********************Setting up csmApi********************")
INCLUDE(OssimVersion)
INCLUDE(OssimUtilities)
INCLUDE(OssimCommonVariables)


IF(NOT APPLE)
  cmake_minimum_required(VERSION 2.6)
ELSE(NOT APPLE)
  cmake_minimum_required(VERSION 2.8)
ENDIF(NOT APPLE)

find_package(ossim)
find_package(OpenThreads)

MESSAGE( STATUS "OSSIM_INCLUDE_DIR         = ${OSSIM_INCLUDE_DIR}")
MESSAGE( STATUS "OSSIM_LIBRARIES           = ${OSSIM_LIBRARIES}")

################################################## BUILD THE LIBRARY #######################################
SET(LIB_NAME csmApi)

INCLUDE_DIRECTORIES(${${PROJECT_NAME}_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${${PROJECT_NAME}_SOURCE_DIR}/include/TSM)
INCLUDE_DIRECTORIES(${OSSIM_INCLUDE_DIR})
FILE(GLOB file_srcs RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/TSM/*.cpp")
FILE(GLOB file_headers "${CMAKE_CURRENT_SOURCE_DIR}/include/csmApi/*.h")
FILE(GLOB file_tsm_headers "${CMAKE_CURRENT_SOURCE_DIR}/include/csmApi/TSM/*.h")

SET(BUILD_OSSIM_FRAMEWORKS "OFF")
SET(INSTALL_INCLUDE_DIR "include/csmApi")

# Get the library suffix for lib or lib64.
get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)       
if(LIB64)
   set(INSTALL_LIBRARY_DIR "lib64")
else()
   set(INSTALL_LIBRARY_DIR "lib")
endif()

OSSIM_LINK_LIBRARY(${LIB_NAME} 
                   COMPONENT_NAME ossim 
                   TYPE ${OSSIM_USER_DEFINED_DYNAMIC_OR_STATIC}
                   HEADERS ${file_headers} 
                   SOURCE_FILES ${file_srcs} 
                   LIBRARIES ${OSSIM_LIBRARIES} ${OPENTHREADS_LIBRARY}
                   INSTALL_LIB VERSION_SYMLINKS 
                   ADDITIONAL_COMPILE_FLAGS "-DCSMAPI_LIBRARY -DTSM_LIBRARY")

# SET_TARGET_PROPERTIES(${LINK_NAME} PROPERTIES 
#                       FRAMEWORK FALSE)

install(FILES ${file_headers} DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT ossim) 
install(FILES ${file_tsm_headers} DESTINATION ${INSTALL_INCLUDE_DIR}/TSM COMPONENT ossim) 
####################################### SETUP THE UNINSTALL #################################
OSSIM_ADD_COMMON_MAKE_UNINSTALL()
