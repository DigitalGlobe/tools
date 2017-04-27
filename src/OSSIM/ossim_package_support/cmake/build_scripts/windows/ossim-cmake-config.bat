::@echo off
::*************************************************************************************
::
::  Windows Batch file for initializing the CMAKE build system for building OSSIM 
::  with MicroSoft compiler.
::
::  This script must be copied into a dedicated build directory.
::  The script can then be invoked from that directory. DO NOT run this script out of
::  the ossim_package_support/cmake/build_scripts/windows.
::  
::  Optional arguments (in any order):
::
::    win32 | win64 | x64 (defaults to x64)
::    vs2005 | vs2010 (defaults vs2010)
::    ide | nmake (defaults to ide)
::
::  No environment variables need be defined prior to running this script. However,
::  OSSIM_DEV_HOME, OSSIM_DEPENDENCIES and OSSIM_BUILD_DIR need to be edited below.
::
::  Author: Oscar Kramer, GEOEYE (Aug, 2011)
::
::*************************************************************************************

:: Edit these to your local system:
set OSSIM_DEV_HOME=C:\Dev\osgeo\trunk
set OSSIM_DEPENDENCIES=C:\Dev\osgeo\ossim_dependencies
set OSSIM_BUILD_DIR=C:\Dev\osgeo\trunk\build

:: Default settings:
set PLATFORM=x64
set DEVENV=vs2010
set GEN_TYPE=IDE

:while_valid_arg
  if "%1"=="win32" set PLATFORM=Win32
  if "%1"=="win64" set PLATFORM=x64
  if "%1"=="vs2005" set DEVENV=vs2005
  if "%1"=="vs2008" set DEVENV=vs2008
  if "%1"=="nmake" set GEN_TYPE=NMAKE
  shift
if not "%1"=="" goto while_valid_arg

IF NOT DEFINED OSSIM_DEV_HOME (
  echo ERROR: OSSIM_DEV_HOME is not defined. Aborting
  GOTO ABORTED_END
)
IF NOT EXIST %OSSIM_DEV_HOME% (
  echo ERROR: OSSIM_DEV_HOME is not set to a valid path. Aborting
  GOTO ABORTED_END
)

IF NOT DEFINED OSSIM_DEPENDENCIES (
  echo ERROR: OSSIM_DEPENDENCIES is not defined. Aborting
  GOTO ABORTED_END
)
IF NOT EXIST %OSSIM_DEPENDENCIES% (
  echo ERROR: OSSIM_DEPENDENCIES is not set to a valid path. Aborting
  GOTO ABORTED_END
)

IF NOT EXIST %OSSIM_BUILD_DIR% MKDIR %OSSIM_BUILD_DIR%
pushd %OSSIM_BUILD_DIR%

:: Change all backslashes to forward slashes in path env vars:
set OSSIM_DEV_HOME=%OSSIM_DEV_HOME:\=/%
set OSSIM_DEPENDENCIES=%OSSIM_DEPENDENCIES:\=/%

echo.
echo Environment settings:
echo OSSIM_DEV_HOME = %OSSIM_DEV_HOME%
echo OSSIM_DEPENDENCIES = %OSSIM_DEPENDENCIES%
echo PLATFORM = %PLATFORM%
echo DEVENV = %DEVENV%
echo OSSIM_BUILD_DIR = %OSSIM_BUILD_DIR%
echo.

:: Create system specific subdirectory if needed:
if not exist %DEVENV% mkdir %DEVENV%
pushd %DEVENV%

:: Create platform and target specific subdirectory if needed:
if not exist %PLATFORM% mkdir %PLATFORM%
pushd %PLATFORM%
if not exist %GEN_TYPE% mkdir %GEN_TYPE%
pushd %GEN_TYPE%

:: Interpret the target system and set up environment:
IF %DEVENV%==vs2005 (
  IF %PLATFORM%==Win32 (
    set TARGET_SYSTEM="Visual Studio 8 2005"
    call "C:\Program Files (x86)\Microsoft Visual Studio 8\VC\vcvarsall" x86
  ) ELSE (
    set TARGET_SYSTEM="Visual Studio 8 2005 Win64"
    call "C:\Program Files (x86)\Microsoft Visual Studio 8\VC\vcvarsall" x64
  )
) 
IF %DEVENV%==vs2008 (
  IF %PLATFORM%==Win32 (
    set TARGET_SYSTEM="Visual Studio 9 Win32"
    call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall" x86
  ) ELSE (
    set TARGET_SYSTEM="Visual Studio 9 Win64"
    call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall" x64
  )
) 
IF %DEVENV%==vs2010 (
  IF %PLATFORM%==Win32 (
    set TARGET_SYSTEM="Visual Studio 10 Win32"
    call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall" x86
  ) ELSE (
    set TARGET_SYSTEM="Visual Studio 10 Win64"
    call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall" x64
  )
)

set DEPLIBDIR=%OSSIM_DEPENDENCIES%/lib/x64
set DEPINCDIR=%OSSIM_DEPENDENCIES%/include

:: Here are the cmake options. Everything is about setting variables except the 
:: the last line (the business end). Make sure that source path points to the
:: ossim_package_support/cmake directory containing the top-level CMakeLists.txt
set CMAKE_PARAMETERS=^
-D%PLATFORM%_USE_MP=ON ^
-DBUILD_CSMAPI=OFF ^
-DBUILD_LIBRARY_DIR=lib ^
-DBUILD_OMS=OFF ^
-DBUILD_OSSIM=ON ^
-DBUILD_OSSIMQT4=OFF ^
-DBUILD_OSSIMGUI=OFF ^
-DBUILD_OSSIM_PLUGIN=ON ^
-DBUILD_OSSIMCONTRIB_PLUGIN=OFF ^
-DBUILD_OSSIMCSM_PLUGIN=OFF ^
-DBUILD_OSSIMGDAL_PLUGIN=ON ^
-DBUILD_OSSIMGEOPDF_PLUGIN=ON ^
-DBUILD_OSSIMKAKADU_PLUGIN=ON ^
-DBUILD_OSSIMLIBRAW_PLUGIN=OFF ^
-DBUILD_OSSIMMDA_PLUGIN=ON ^
-DBUILD_OSSIMMRSID_PLUGIN=OFF ^
-DBUILD_OSSIMNDF_PLUGIN=ON ^
-DBUILD_OSSIMNUI_PLUGIN=OFF ^
-DBUILD_OSSIMPNG_PLUGIN=ON ^
-DBUILD_OSSIMREGISTRATION_PLUGIN=OFF ^
-DBUILD_OSSIM_MPI_SUPPORT=0 ^
-DBUILD_OSSIMPLANET=OFF ^
-DBUILD_OSSIMPLANETQT=OFF ^
-DBUILD_OSSIMPREDATOR=OFF ^
-DBUILD_OSSIM_TEST_APPS=ON ^
-DBUILD_RUNTIME_DIR=bin ^
-DBUILD_SHARED_LIBS=ON ^
-DBUILD_WMS=off ^
-DCMAKE_INCLUDE_PATH=%DEPINCDIR% ^
-DCMAKE_INSTALL_PREFIX=%OSSIM_DEPENDENCIES%/^
-DCMAKE_LIBRARY_PATH=%DEPLIBDIR% ^
-DCMAKE_MODULE_PATH=%OSSIM_DEV_HOME%/ossim_package_support/CMakeModules ^
-DKAKADU_ROOT_SRC=%OSSIM_DEPENDENCIES%/include/kakadu ^
-DKAKADU_LIBRARY=%DEPLIBDIR%/kdu_v64.lib ^
-DKAKADU_AUX_LIBRARY=%DEPLIBDIR%/kdu_a64.lib ^
-DMRSID_LIBRARY=%DEPLIBDIR%/lti_dsdk.lib ^
-DMRSID_INCLUDE_DIR=%DEPINCDIR%/lti_geo_ESDK-7.0.0.2167 ^
-DMRSID_ESDK_LIBRARY=%DEPLIBDIR%/lti_esdk.lib ^
-DMRSID_GDAL_LIBRARY=%DEPLIBDIR%/lti_gdal.lib ^
-DMRSID_LIDAR_LIBRARY=%DEPLIBDIR%/lti_lidar_dsdk.lib ^
-DMRSID_CDLL_LIBRARY=%DEPLIBDIR%/lti_dsdk_cdll.lib ^
-DOPENTHREADS_LIBRARY=%DEPLIBDIR%/OpenThreadsx64.lib ^
-DPNG_LIBRARY=%DEPLIBDIR%/libpng.lib ^
-DPNG_PNG_INCLUDE_DIR=%DEPINCDIR%/png ^
-DTIFF_LIBRARY=%DEPLIBDIR%/libtiff_i.lib ^
-DTIFF_INCLUDE_DIR=%DEPINCDIR%/tiff ^
-DGDAL_LIBRARY=%DEPLIBDIR%/gdal_i.lib ^
-DGDAL_INCLUDE_DIR=%DEPINCDIR%/gdal ^
-DPROJ4_LIBRARY=%DEPLIBDIR%/proj.lib ^
-DPODOFO_INCLUDE_DIR=%DEPINCDIR% ^
-DPODOFO_LIBRARY=%DEPLIBDIR%/podofo.lib ^
-DEXPAT_INCLUDE_DIR=%DEPINCDIR%/expat ^
-DEXPAT_LIBRARY=%DEPLIBDIR%/podofo.lib ^
-DGEOTIFF_LIBRARY=%DEPLIBDIR%/geotiff_i.lib ^
-DGEOTIFF_INCLUDE_DIR=%DEPINCDIR%/geotiff ^
-DFFTW3_LIBRARY=%DEPLIBDIR%/libfftw3-3.lib ^
-DJPEG_LIBRARY=%DEPLIBDIR%/libjpeg.lib ^
-DJPEG_INCLUDE_DIR=%DEPINCDIR%/jpeg8a ^
-DFREETYPE_LIBRARY=%DEPLIBDIR%/freetype237.lib ^
-DFREETYPE_INCLUDE_DIRS=%DEPINCDIR%/freetype ^
-DZLIB_LIBRARY=%DEPLIBDIR%/zlib.lib ^
-DZLIB_INCLUDE_DIR=%DEPINCDIR%/zlib ^
-DOSSIM_COMPILE_WITH_FULL_WARNING=ON ^
-DOSSIM_DEPENDENCIES=%OSSIM_DEPENDENCIES%^
-DOSSIM_DEV_HOME=%OSSIM_DEV_HOME% ^
-DBUILD_SHARED_LIBS=ON ^
%OSSIM_DEV_HOME%/ossim_package_support/cmake

:: Finally the cmake commands themselves:
echo TARGET_SYSTEM = %TARGET_SYSTEM%
IF %GEN_TYPE%==IDE (
  echo Generating IDE solution and project files...
  cmake -G %TARGET_SYSTEM% %CMAKE_PARAMETERS%
) ELSE (
  echo Generating nmake makefiles...
  cmake -G "NMake Makefiles" %CMAKE_PARAMETERS%
)

:: Restore environment
popd
popd
popd
popd
set OSSIM_DEV_HOME=%OSSIM_DEV_HOME:/=\%
set OSSIM_DEPENDENCIES=%OSSIM_DEPENDENCIES:/=\%
echo Successfully configured build environment.
goto FINISHED

:ABORTED_END
echo Script terminated prematurely...

:FINISHED
pause
