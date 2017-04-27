cmake_minimum_required(VERSION 2.6)
project(HAWKNL)

# no ppc because it's untested (and do we need it at all?)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

file(GLOB_RECURSE HAWKNL_SOURCES ${HAWKNL_SOURCE_DIR}/src/*.c)
include_directories(${HAWKNL_SOURCE_DIR}/include)
add_library(NL SHARED ${HAWKNL_SOURCES})
target_link_libraries(NL ${PTHREAD_LIBRARY} wsock32 ws2_32.lib)

set_target_properties(NL PROPERTIES VERSION 1.7.1)

install(TARGETS NL DESTINATION lib)
install(DIRECTORY ${HAWKNL_SOURCE_DIR}/include/ DESTINATION include FILES_MATCHING PATTERN "*.h")
