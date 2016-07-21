set(LIB_NAME ossimlibraw_plugin)
MESSAGE( "************** LIBRARY SETUP FOR ossimlibraw_plugin******************")

# Include ossim cmake stuff:
include(OssimVersion)
include(OssimCommonVariables)
include(OssimUtilities)

SET(LIBRAW_SRC LibRaw-0.9.0)
find_package(JPEG)
find_package(TIFF)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${LIBRAW_SRC} ${JPEG_INCLUDE_DIR} ${TIFF_INCLUDE_DIR})

FILE(GLOB OSSIMPLUGIN_SRCS ${LIBRAW_SRC}/internal/*.cpp ${LIBRAW_SRC}/src/*.cpp *.cpp)
FILE(GLOB OSSIMPLUGIN_HEADERS ${LIBRAW_SRC}/internal/*.h *.h ${LIBRAW_SRC}/libraw/*.h)

####
# OSSIMPLUGINSMAKINGDLL controls dll linkage on windows.  
# Adding this sets OSSIM_PLUGINS_DLL #define TO "__declspec(dllexport)".
###
add_definitions("-DOSSIMPLUGINSMAKINGDLL")

# Adjust the install library path:
if(OSSIM_INSTALL_PLUGINS_WITH_VERSION)
   set(INSTALL_LIBRARY_DIR   "${INSTALL_LIBRARY_DIR}/ossim-${OSSIM_VERSION}/plugins")
else(OSSIM_INSTALL_PLUGINS_WITH_VERSION)
   set(INSTALL_LIBRARY_DIR   "${INSTALL_LIBRARY_DIR}/ossim/plugins")
endif(OSSIM_INSTALL_PLUGINS_WITH_VERSION)

OSSIM_LINK_LIBRARY(${LIB_NAME}
                   COMPONENT_NAME ossim TYPE "${OSSIM_PLUGIN_LINK_TYPE}"
		   LIBRARIES ${OSSIM_LIBRARIES} ${OPENTHREADS_LIBRARY} ${JPEG_LIBRARY} ${TIFF_LIBRARY}
                   HEADERS "${OSSIMPLUGIN_HEADERS}"
		   SOURCE_FILES "${OSSIMPLUGIN_SRCS}" ADDITIONAL_COMPILE_FLAGS "-DLIBRAW_NOTHREADS -DLIBRAW_NODLL"
                   INSTALL_LIB)


