cmake_minimum_required(VERSION 2.8)

include_directories(${ZLIB_INCLUDE_DIR})

option(BUILD_SHARED_LIBS  "Build shared libs." ON)

set(SRCS
  ioapi.c
  mztools.c
  unzip.c
  zip.c )

set(INCS
  crypt.h
  ioapi.h
  mztools.h
  unzip.h
  zip.h )

if(WIN32)
  list(APPEND SRCS iowin32.c)
  list(APPEND INCS iowin32.h)
endif()

message(STATUS "ZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIR}")
message(STATUS "ZLIB_LIBRARY=${ZLIB_LIBRARY}")

add_library(minizip ${SRCS})

target_link_libraries(minizip ${ZLIB_LIBRARY})

install(FILES ${INCS}
   DESTINATION include/minizip)

install(TARGETS minizip
   DESTINATION lib)

 
