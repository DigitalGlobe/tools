@echo off


:: Set env vars
@call setenvvar.bat

@call set_build_target.bat %* icu

::==========
:: MAIN

@echo Building ICU %FB_OBJ_DIR% for %FB_TARGET_PLATFORM%

if %MSVC_VERSION% GEQ 8 (
    @call compile.bat %FB_ROOT_PATH%\extern\icu\source\allinone\allinone_%MSVC_VERSION% make_icu_%FB_TARGET_PLATFORM%.log
) else (
    @call compile.bat %FB_ROOT_PATH%\extern\icu\source\allinone\allinone make_icu_%FB_TARGET_PLATFORM%.log
)

if errorlevel 1 call :ERROR build failed - see make_icu_%FB_TARGET_PLATFORM%.log for details
@goto :EOF



:ERROR
::====
@echo.
@echo   An error occurred while running make_icu.bat -
@echo     %*
@echo.
set ERRLEV=1
cancel_script > nul 2>&1
::End of ERROR
::------------
@goto :EOF




