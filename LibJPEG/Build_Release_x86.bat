::-----------------------------------------------------------------------------
::
:: Build_Debug_x86.bat
::
:: Summary : Builds the 32-bit version of Libjpeg.
::
::-----------------------------------------------------------------------------

@ECHO OFF

SETLOCAL

::-----------------------------------------------------------------------------
:: global variables

    ::-------------------------------------------------------------------------
    :: the path that holds intermediate build results
    SET PATH_BUILD=%CD%\x86\build
    ::-------------------------------------------------------------------------
    :: the current directory
    SET PATH_CURRENT=%CD%
    ::-------------------------------------------------------------------------
    :: the path that holds usable build results
    SET PATH_DISTRIBUTION=%CD%\x86\dist
    ::-------------------------------------------------------------------------
    :: the path that contains the source code
    SET PATH_SRC=%CD%\x86\src
    ::-------------------------------------------------------------------------

::-----------------------------------------------------------------------------
:: initialize the include directory
set INCLUDE=%INCLUDE%;c:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include

:: recreate the build directory, if it exists
ECHO Recreating build directory...
ECHO %PATH_BUILD%
IF EXIST %PATH_BUILD% RMDIR /S /Q %PATH_BUILD%
MKDIR %PATH_BUILD%

:: recreate the distribution directory
ECHO Recreating distribution directory...
IF EXIST %PATH_DISTRIBUTION% RMDIR /S /Q %PATH_DISTRIBUTION%
MKDIR %PATH_DISTRIBUTION%

:: copy the source code into the build directory (necessary because
::     cannot specify Nmake output path)
XCOPY %PATH_SRC%\* %PATH_BUILD% /E /Q /S

:: change to the build directory
CD /D %PATH_BUILD%

:: choose the configuration file
COPY jconfig.vc jconfig.h /v

:: build the library
"%VS140COMNTOOLS%\..\..\VC\bin\nmake.exe" /C /f makefile.vc nodebug=1

:: copy the build results
COPY *.exe %PATH_DISTRIBUTION% /v
COPY *.lib %PATH_DISTRIBUTION% /v

:: change to the current directory
CD /D %PATH_CURRENT%
::-----------------------------------------------------------------------------
    
ENDLOCAL