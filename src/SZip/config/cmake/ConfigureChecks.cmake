#-----------------------------------------------------------------------------
# Include all the necessary files for macros
#-----------------------------------------------------------------------------
INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)

#-----------------------------------------------------------------------------
# Always SET this for now IF we are on an OS X box
#-----------------------------------------------------------------------------
IF (APPLE)
  LIST(LENGTH CMAKE_OSX_ARCHITECTURES ARCH_LENGTH)
  IF(ARCH_LENGTH GREATER 1)
    set (CMAKE_OSX_ARCHITECTURES "" CACHE STRING "" FORCE)
    message(FATAL_ERROR "Building Universal Binaries on OS X is NOT supported by the HDF5 project. This is"
    "due to technical reasons. The best approach would be build each architecture in separate directories"
    "and use the 'lipo' tool to combine them into a single executable or library. The 'CMAKE_OSX_ARCHITECTURES'"
    "variable has been set to a blank value which will build the default architecture for this system.")
  ENDIF()
  SET (HDF_AC_APPLE_UNIVERSAL_BUILD 0)
ENDIF (APPLE)

#-----------------------------------------------------------------------------
# This MACRO checks IF the symbol exists in the library and IF it
# does, it appends library to the list.
#-----------------------------------------------------------------------------
SET (LINK_LIBS "")
MACRO (CHECK_LIBRARY_EXISTS_CONCAT LIBRARY SYMBOL VARIABLE)
  CHECK_LIBRARY_EXISTS ("${LIBRARY};${LINK_LIBS}" ${SYMBOL} "" ${VARIABLE})
  IF (${VARIABLE})
    SET (LINK_LIBS ${LINK_LIBS} ${LIBRARY})
  ENDIF (${VARIABLE})
ENDMACRO (CHECK_LIBRARY_EXISTS_CONCAT)

# ----------------------------------------------------------------------
# WINDOWS Hard code Values
# ----------------------------------------------------------------------

SET (WINDOWS)
IF (WIN32)
  IF (NOT UNIX AND NOT CYGWIN AND NOT MINGW)
    SET (WINDOWS 1)
  ENDIF (NOT UNIX AND NOT CYGWIN AND NOT MINGW)
ENDIF (WIN32)

IF (WINDOWS)
  SET (HAVE_LIBM 1)
  SET (HAVE_IO_H 1)
  SET (HAVE_SETJMP_H 1)
  SET (HAVE_STDDEF_H 1)
  SET (HAVE_SYS_STAT_H 1)
  SET (HAVE_SYS_TIMEB_H 1)
  SET (HAVE_SYS_TYPES_H 1)
  SET (HAVE_STRDUP 1)
  SET (HAVE_SYSTEM 1)
  SET (HAVE_DIFFTIME 1)
  SET (HAVE_LONGJMP 1)
  SET (STDC_HEADERS 1)
  SET (HAVE_GETHOSTNAME 1)
  SET (HAVE_TIMEZONE 1)
  SET (HAVE_FUNCTION 1)
ENDIF (WINDOWS)

# ----------------------------------------------------------------------
# END of WINDOWS Hard code Values
# ----------------------------------------------------------------------

IF (CYGWIN)
  SET (HAVE_LSEEK64 0)
ENDIF (CYGWIN)

#-----------------------------------------------------------------------------
#  Check for the math library "m"
#-----------------------------------------------------------------------------
IF (NOT WINDOWS)
  CHECK_LIBRARY_EXISTS_CONCAT ("m" random     HAVE_LIBM)
ENDIF (NOT WINDOWS)
CHECK_LIBRARY_EXISTS_CONCAT ("ws2_32" WSAStartup  HAVE_LIBWS2_32)
CHECK_LIBRARY_EXISTS_CONCAT ("wsock32" gethostbyname HAVE_LIBWSOCK32)
#CHECK_LIBRARY_EXISTS_CONCAT ("dl"     dlopen       HAVE_LIBDL)
CHECK_LIBRARY_EXISTS_CONCAT ("ucb"    gethostname  HAVE_LIBUCB)
CHECK_LIBRARY_EXISTS_CONCAT ("socket" connect      HAVE_LIBSOCKET)
CHECK_LIBRARY_EXISTS ("c" gethostbyname "" NOT_NEED_LIBNSL)

IF (NOT NOT_NEED_LIBNSL)
  CHECK_LIBRARY_EXISTS_CONCAT ("nsl"    gethostbyname  HAVE_LIBNSL)
ENDIF (NOT NOT_NEED_LIBNSL)


SET (USE_INCLUDES "")
IF (WINDOWS)
  SET (USE_INCLUDES ${USE_INCLUDES} "windows.h")
ENDIF (WINDOWS)
#-----------------------------------------------------------------------------
# Check IF header file exists and add it to the list.
#-----------------------------------------------------------------------------
MACRO (CHECK_INCLUDE_FILE_CONCAT FILE VARIABLE)
  CHECK_INCLUDE_FILES ("${USE_INCLUDES};${FILE}" ${VARIABLE})
  IF (${VARIABLE})
    SET (USE_INCLUDES ${USE_INCLUDES} ${FILE})
  ENDIF (${VARIABLE})
ENDMACRO (CHECK_INCLUDE_FILE_CONCAT)

#-----------------------------------------------------------------------------
#  Check for the existence of certain header files
#-----------------------------------------------------------------------------
CHECK_INCLUDE_FILE_CONCAT ("globus/common.h" HAVE_GLOBUS_COMMON_H)
CHECK_INCLUDE_FILE_CONCAT ("io.h"            HAVE_IO_H)
CHECK_INCLUDE_FILE_CONCAT ("mfhdf.h"         HAVE_MFHDF_H)
CHECK_INCLUDE_FILE_CONCAT ("pdb.h"           HAVE_PDB_H)
CHECK_INCLUDE_FILE_CONCAT ("pthread.h"       HAVE_PTHREAD_H)
CHECK_INCLUDE_FILE_CONCAT ("setjmp.h"        HAVE_SETJMP_H)
CHECK_INCLUDE_FILE_CONCAT ("srbclient.h"     HAVE_SRBCLIENT_H)
CHECK_INCLUDE_FILE_CONCAT ("stddef.h"        HAVE_STDDEF_H)
CHECK_INCLUDE_FILE_CONCAT ("stdint.h"        HAVE_STDINT_H)
CHECK_INCLUDE_FILE_CONCAT ("string.h"        HAVE_STRING_H)
CHECK_INCLUDE_FILE_CONCAT ("strings.h"       HAVE_STRINGS_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/ioctl.h"     HAVE_SYS_IOCTL_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/proc.h"      HAVE_SYS_PROC_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/resource.h"  HAVE_SYS_RESOURCE_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/socket.h"    HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/stat.h"      HAVE_SYS_STAT_H)
IF (CMAKE_SYSTEM_NAME MATCHES "OSF")
  CHECK_INCLUDE_FILE_CONCAT ("sys/sysinfo.h" HAVE_SYS_SYSINFO_H)
ELSE (CMAKE_SYSTEM_NAME MATCHES "OSF")
  SET (HAVE_SYS_SYSINFO_H "" CACHE INTERNAL "" FORCE)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "OSF")
CHECK_INCLUDE_FILE_CONCAT ("sys/time.h"      HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE_CONCAT ("time.h"          HAVE_TIME_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/timeb.h"     HAVE_SYS_TIMEB_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/types.h"     HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE_CONCAT ("unistd.h"        HAVE_UNISTD_H)
CHECK_INCLUDE_FILE_CONCAT ("stdlib.h"        HAVE_STDLIB_H)
CHECK_INCLUDE_FILE_CONCAT ("memory.h"        HAVE_MEMORY_H)
CHECK_INCLUDE_FILE_CONCAT ("dlfcn.h"         HAVE_DLFCN_H)
CHECK_INCLUDE_FILE_CONCAT ("features.h"      HAVE_FEATURES_H)
CHECK_INCLUDE_FILE_CONCAT ("inttypes.h"      HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE_CONCAT ("netinet/in.h"    HAVE_NETINET_IN_H)

IF (NOT CYGWIN)
  CHECK_INCLUDE_FILE_CONCAT ("winsock2.h"      H4_HAVE_WINSOCK_H)
ENDIF (NOT CYGWIN)

# IF the c compiler found stdint, check the C++ as well. On some systems this
# file will be found by C but not C++, only do this test IF the C++ compiler
# has been initialized (e.g. the project also includes some c++)
IF (HAVE_STDINT_H AND CMAKE_CXX_COMPILER_LOADED)
  CHECK_INCLUDE_FILE_CXX ("stdint.h" HAVE_STDINT_H_CXX)
  IF (NOT HAVE_STDINT_H_CXX)
    SET (HAVE_STDINT_H "" CACHE INTERNAL "Have includes HAVE_STDINT_H")
    SET (USE_INCLUDES ${USE_INCLUDES} "stdint.h")
  ENDIF (NOT HAVE_STDINT_H_CXX)
ENDIF (HAVE_STDINT_H AND CMAKE_CXX_COMPILER_LOADED)

#-----------------------------------------------------------------------------
#  Check for large file support
#-----------------------------------------------------------------------------

# The linux-lfs option is deprecated.
SET (LINUX_LFS 0)

SET (HDF_EXTRA_FLAGS)
IF (NOT WINDOWS)
  # Linux Specific flags
  SET (HDF_EXTRA_FLAGS -D_POSIX_SOURCE -D_BSD_SOURCE)
  OPTION (HDF_ENABLE_LARGE_FILE "Enable support for large (64-bit) files on Linux." ON)
  IF (HDF_ENABLE_LARGE_FILE)
    SET (msg "Performing TEST_LFS_WORKS")
    TRY_RUN (TEST_LFS_WORKS_RUN   TEST_LFS_WORKS_COMPILE
        ${CMAKE_BINARY_DIR}
        ${HDF_RESOURCES_DIR}/HDFTests.c
        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=-DTEST_LFS_WORKS
        OUTPUT_VARIABLE OUTPUT
    )
    IF (TEST_LFS_WORKS_COMPILE)
      IF (TEST_LFS_WORKS_RUN  MATCHES 0)
        SET (TEST_LFS_WORKS 1 CACHE INTERNAL ${msg})
    SET (LARGEFILE 1)
        SET (HDF_EXTRA_FLAGS ${HDF_EXTRA_FLAGS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE)
        MESSAGE (STATUS "${msg}... yes")
      ELSE (TEST_LFS_WORKS_RUN  MATCHES 0)
        SET (TEST_LFS_WORKS "" CACHE INTERNAL ${msg})
        MESSAGE (STATUS "${msg}... no")
        FILE (APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
              "Test TEST_LFS_WORKS Run failed with the following output and exit code:\n ${OUTPUT}\n"
        )
      ENDIF (TEST_LFS_WORKS_RUN  MATCHES 0)
    ELSE (TEST_LFS_WORKS_COMPILE )
      SET (TEST_LFS_WORKS "" CACHE INTERNAL ${msg})
      MESSAGE (STATUS "${msg}... no")
      FILE (APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
          "Test TEST_LFS_WORKS Compile failed with the following output:\n ${OUTPUT}\n"
      )
    ENDIF (TEST_LFS_WORKS_COMPILE)
  ENDIF (HDF_ENABLE_LARGE_FILE)
  SET (CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS} ${HDF_EXTRA_FLAGS})
ENDIF (NOT WINDOWS)

ADD_DEFINITIONS (${HDF_EXTRA_FLAGS})

# For other tests to use the same libraries
SET (CMAKE_REQUIRED_LIBRARIES ${LINK_LIBS})

#-----------------------------------------------------------------------------
# Check for some functions that are used
#
CHECK_FUNCTION_EXISTS (alarm             HAVE_ALARM)
CHECK_FUNCTION_EXISTS (fork              HAVE_FORK)
CHECK_FUNCTION_EXISTS (frexpf            HAVE_FREXPF)
CHECK_FUNCTION_EXISTS (frexpl            HAVE_FREXPL)

CHECK_FUNCTION_EXISTS (gethostname       HAVE_GETHOSTNAME)
CHECK_FUNCTION_EXISTS (getpwuid          HAVE_GETPWUID)
CHECK_FUNCTION_EXISTS (getrusage         HAVE_GETRUSAGE)
CHECK_FUNCTION_EXISTS (lstat             HAVE_LSTAT)

CHECK_FUNCTION_EXISTS (rand_r            HAVE_RAND_R)
CHECK_FUNCTION_EXISTS (random            HAVE_RANDOM)
CHECK_FUNCTION_EXISTS (setsysinfo        HAVE_SETSYSINFO)

CHECK_FUNCTION_EXISTS (signal            HAVE_SIGNAL)
CHECK_FUNCTION_EXISTS (longjmp           HAVE_LONGJMP)
CHECK_FUNCTION_EXISTS (setjmp            HAVE_SETJMP)
CHECK_FUNCTION_EXISTS (siglongjmp        HAVE_SIGLONGJMP)
CHECK_FUNCTION_EXISTS (sigsetjmp         HAVE_SIGSETJMP)
CHECK_FUNCTION_EXISTS (sigaction         HAVE_SIGACTION)
CHECK_FUNCTION_EXISTS (sigprocmask       HAVE_SIGPROCMASK)

CHECK_FUNCTION_EXISTS (snprintf          HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS (srandom           HAVE_SRANDOM)
CHECK_FUNCTION_EXISTS (strdup            HAVE_STRDUP)
CHECK_FUNCTION_EXISTS (symlink           HAVE_SYMLINK)
CHECK_FUNCTION_EXISTS (system            HAVE_SYSTEM)

CHECK_FUNCTION_EXISTS (tmpfile           HAVE_TMPFILE)
CHECK_FUNCTION_EXISTS (vasprintf         HAVE_VASPRINTF)
CHECK_FUNCTION_EXISTS (waitpid           HAVE_WAITPID)

CHECK_FUNCTION_EXISTS (vsnprintf         HAVE_VSNPRINTF)
CHECK_FUNCTION_EXISTS (ioctl             HAVE_IOCTL)
#CHECK_FUNCTION_EXISTS (gettimeofday      HAVE_GETTIMEOFDAY)
CHECK_FUNCTION_EXISTS (difftime          HAVE_DIFFTIME)
CHECK_FUNCTION_EXISTS (fseeko            HAVE_FSEEKO)
CHECK_FUNCTION_EXISTS (ftello            HAVE_FTELLO)
CHECK_FUNCTION_EXISTS (fseeko64          HAVE_FSEEKO64)
CHECK_FUNCTION_EXISTS (ftello64          HAVE_FTELLO64)
CHECK_FUNCTION_EXISTS (fstat64           HAVE_FSTAT64)
CHECK_FUNCTION_EXISTS (stat64            HAVE_STAT64)

#-----------------------------------------------------------------------------
#  Since gettimeofday is not defined any where standard, lets look in all the
#  usual places. On MSVC we are just going to use ::clock()
#-----------------------------------------------------------------------------
IF (NOT MSVC)
  IF ("HAVE_TIME_GETTIMEOFDAY" MATCHES "^HAVE_TIME_GETTIMEOFDAY$")
    TRY_COMPILE (HAVE_TIME_GETTIMEOFDAY
        ${CMAKE_BINARY_DIR}
        ${HDF_RESOURCES_DIR}/GetTimeOfDayTest.c
        COMPILE_DEFINITIONS -DTRY_TIME_H
        OUTPUT_VARIABLE OUTPUT
    )
    IF (HAVE_TIME_GETTIMEOFDAY STREQUAL "TRUE")
      SET (HAVE_TIME_GETTIMEOFDAY "1" CACHE INTERNAL "HAVE_TIME_GETTIMEOFDAY")
      SET (HAVE_GETTIMEOFDAY "1" CACHE INTERNAL "HAVE_GETTIMEOFDAY")
    ENDIF (HAVE_TIME_GETTIMEOFDAY STREQUAL "TRUE")
  ENDIF ("HAVE_TIME_GETTIMEOFDAY" MATCHES "^HAVE_TIME_GETTIMEOFDAY$")

  IF ("HAVE_SYS_TIME_GETTIMEOFDAY" MATCHES "^HAVE_SYS_TIME_GETTIMEOFDAY$")
    TRY_COMPILE (HAVE_SYS_TIME_GETTIMEOFDAY
        ${CMAKE_BINARY_DIR}
        ${HDF_RESOURCES_DIR}/GetTimeOfDayTest.c
        COMPILE_DEFINITIONS -DTRY_SYS_TIME_H
        OUTPUT_VARIABLE OUTPUT
    )
    IF (HAVE_SYS_TIME_GETTIMEOFDAY STREQUAL "TRUE")
      SET (HAVE_SYS_TIME_GETTIMEOFDAY "1" CACHE INTERNAL "HAVE_SYS_TIME_GETTIMEOFDAY")
      SET (HAVE_GETTIMEOFDAY "1" CACHE INTERNAL "HAVE_GETTIMEOFDAY")
    ENDIF (HAVE_SYS_TIME_GETTIMEOFDAY STREQUAL "TRUE")
  ENDIF ("HAVE_SYS_TIME_GETTIMEOFDAY" MATCHES "^HAVE_SYS_TIME_GETTIMEOFDAY$")

  IF (NOT HAVE_SYS_TIME_GETTIMEOFDAY AND NOT HAVE_GETTIMEOFDAY AND NOT MSVC)
    MESSAGE (STATUS "---------------------------------------------------------------")
    MESSAGE (STATUS "Function 'gettimeofday()' was not found. SZIP will use its")
    MESSAGE (STATUS "  own implementation.. This can happen on older versions of")
    MESSAGE (STATUS "  MinGW on Windows. Consider upgrading your MinGW installation")
    MESSAGE (STATUS "  to a newer version such as MinGW 3.12")
    MESSAGE (STATUS "---------------------------------------------------------------")
  ENDIF (NOT HAVE_SYS_TIME_GETTIMEOFDAY AND NOT HAVE_GETTIMEOFDAY AND NOT MSVC)
ENDIF (NOT MSVC)

# Check for Symbols
CHECK_SYMBOL_EXISTS (tzname "time.h" HAVE_DECL_TZNAME)

#-----------------------------------------------------------------------------
#
#-----------------------------------------------------------------------------
IF (NOT WINDOWS)
  CHECK_SYMBOL_EXISTS (TIOCGWINSZ "sys/ioctl.h" HAVE_TIOCGWINSZ)
  CHECK_SYMBOL_EXISTS (TIOCGETD   "sys/ioctl.h" HAVE_TIOCGETD)
ENDIF (NOT WINDOWS)


# For other other specific tests, use this MACRO.
MACRO (HDF_FUNCTION_TEST OTHER_TEST)
  IF ("${OTHER_TEST}" MATCHES "^${OTHER_TEST}$")
    SET (MACRO_CHECK_FUNCTION_DEFINITIONS "-D${OTHER_TEST} ${CMAKE_REQUIRED_FLAGS}")
    SET (OTHER_TEST_ADD_LIBRARIES)
    IF (CMAKE_REQUIRED_LIBRARIES)
      SET (OTHER_TEST_ADD_LIBRARIES "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    ENDIF (CMAKE_REQUIRED_LIBRARIES)

    FOREACH (def ${HDF_EXTRA_TEST_DEFINITIONS})
      SET (MACRO_CHECK_FUNCTION_DEFINITIONS "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D${def}=${${def}}")
    ENDFOREACH (def)

    FOREACH (def
        HAVE_SYS_TIME_H
        HAVE_UNISTD_H
        HAVE_SYS_TYPES_H
        HAVE_SYS_SOCKET_H
    )
      IF ("${def}")
        SET (MACRO_CHECK_FUNCTION_DEFINITIONS "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D${def}")
      ENDIF ("${def}")
    ENDFOREACH (def)

    IF (LARGEFILE)
      SET (MACRO_CHECK_FUNCTION_DEFINITIONS
          "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE"
      )
    ENDIF (LARGEFILE)

    #MESSAGE (STATUS "Performing ${OTHER_TEST}")
    TRY_COMPILE (${OTHER_TEST}
        ${CMAKE_BINARY_DIR}
        ${HDF_RESOURCES_DIR}/HDFTests.c
        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
        "${OTHER_TEST_ADD_LIBRARIES}"
        OUTPUT_VARIABLE OUTPUT
    )
    IF (${OTHER_TEST})
      SET (${OTHER_TEST} 1 CACHE INTERNAL "Other test ${FUNCTION}")
      MESSAGE (STATUS "Performing Other Test ${OTHER_TEST} - Success")
    ELSE (${OTHER_TEST})
      MESSAGE (STATUS "Performing Other Test ${OTHER_TEST} - Failed")
      SET (${OTHER_TEST} "" CACHE INTERNAL "Other test ${FUNCTION}")
      FILE (APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
          "Performing Other Test ${OTHER_TEST} failed with the following output:\n"
          "${OUTPUT}\n"
      )
    ENDIF (${OTHER_TEST})
  ENDIF ("${OTHER_TEST}" MATCHES "^${OTHER_TEST}$")
ENDMACRO (HDF_FUNCTION_TEST)

#-----------------------------------------------------------------------------
# Check a bunch of other functions
#-----------------------------------------------------------------------------
IF (NOT WINDOWS)
  FOREACH (test
      TIME_WITH_SYS_TIME
      STDC_HEADERS
      HAVE_TM_ZONE
      HAVE_STRUCT_TM_TM_ZONE
      HAVE_ATTRIBUTE
      HAVE_FUNCTION
      HAVE_TM_GMTOFF
#      HAVE_TIMEZONE
      HAVE_STRUCT_TIMEZONE
      HAVE_STAT_ST_BLOCKS
      HAVE_FUNCTION
      SYSTEM_SCOPE_THREADS
      HAVE_SOCKLEN_T
      DEV_T_IS_SCALAR
      HAVE_OFF64_T
      GETTIMEOFDAY_GIVES_TZ
      VSNPRINTF_WORKS
      HAVE_C99_FUNC
      HAVE_C99_DESIGNATED_INITIALIZER
      CXX_HAVE_OFFSETOF
  )
    HDF_FUNCTION_TEST (${test})
  ENDFOREACH (test)
    IF (NOT CYGWIN AND NOT MINGW)
      HDF_FUNCTION_TEST (HAVE_TIMEZONE)
#      HDF_FUNCTION_TEST (HAVE_STAT_ST_BLOCKS)
    ENDIF (NOT CYGWIN AND NOT MINGW)
ENDIF (NOT WINDOWS)

#-----------------------------------------------------------------------------
# Look for 64 bit file stream capability
#-----------------------------------------------------------------------------
IF (HAVE_OFF64_T)
  CHECK_FUNCTION_EXISTS (lseek64           HAVE_LSEEK64)
  CHECK_FUNCTION_EXISTS (fseek64           HAVE_FSEEK64)
ENDIF (HAVE_OFF64_T)
