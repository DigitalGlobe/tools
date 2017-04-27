::
:: Preprocess all .epp files to .cpp
:: ---------------------------------
::
::  To do:
::
::  o Better/Some documentation as to what this does
::  o Add some logging/diagnostics, so we can see what is happening
::    and track errors.
::  o License boiler plate, copyright and authorship acknowledgements
::

@echo off

::===========
:MAIN

@call setenvvar.bat

@if "%1"=="BOOT" (set BOOTBUILD=1) else (set BOOTBUILD=0)
@echo.
@if "%BOOTBUILD%"=="1" (call :BOOT_PROCESS) else (call :MASTER_PROCESS)
@set BOOTBUILD=
@set GPRE=
@goto :END

::===========
:PREPROCESS
@echo Processing %1/%2.epp
@echo Calling GPRE for %1/%2.epp
@if "%3"=="" (call :GPRE_M %1 %2) else (call :GPRE_GDS %1 %2 %3 %4)

@if not exist %FB_GEN_DIR%\%1\%2.cpp (
    @move %FB_GEN_DIR%\preprocessing.cpp %FB_GEN_DIR%\%1\%2.cpp
) else (
    @fc %FB_GEN_DIR%\preprocessing.cpp %FB_GEN_DIR%\%1\%2.cpp >nul
    @if errorlevel 1 @move %FB_GEN_DIR%\preprocessing.cpp %FB_GEN_DIR%\%1\%2.cpp
)

@echo.
@goto :EOF

::===========
:GPRE_M
@%GPRE% -n -m -raw %FB_ROOT_PATH%\src\%1\%2.epp %FB_GEN_DIR%\preprocessing.cpp -b %FB_GEN_DB_DIR%/dbs/
@goto :EOF

::===========
:GPRE_GDS
@%GPRE% -n -raw -ids %3 %4 %FB_ROOT_PATH%\src\%1\%2.epp %FB_GEN_DIR%\preprocessing.cpp -b %FB_GEN_DB_DIR%/dbs/
goto :EOF

::===========
:BOOT_PROCESS
@echo.
@set GPRE=%FB_GEN_DIR%\gpre_boot -lang_internal
@for %%i in (array, blob) do @call :PREPROCESS dsql %%i
@for %%i in (metd, DdlNodes) do @call :PREPROCESS dsql %%i -gds_cxx
@for %%i in (gpre_meta) do @call :PREPROCESS gpre %%i
@for %%i in (backup, restore) do @call :PREPROCESS burp %%i
@for %%i in (extract, isql, show) do @call :PREPROCESS isql %%i
@for %%i in (dba) do @call :PREPROCESS utilities/gstat %%i

@set GPRE=%FB_GEN_DIR%\gpre_boot
@for %%i in (alice_meta) do @call :PREPROCESS alice %%i
@for %%i in (array, blob) do @call :PREPROCESS dsql %%i
@for %%i in (metd, DdlNodes) do @call :PREPROCESS dsql %%i -gds_cxx
@for %%i in (gpre_meta) do @call :PREPROCESS gpre %%i
@for %%i in (dfw, dpm, dyn, dyn_def, dyn_del, dyn_mod, dyn_util, fun, grant, ini, met, pcmet, scl) do @call :PREPROCESS jrd %%i -gds_cxx
@for %%i in (stats) do @call :PREPROCESS utilities %%i
@goto :EOF

::===========
:MASTER_PROCESS
@set GPRE=%FB_GEN_DIR%\gpre_embed
@for %%i in (alice_meta) do @call :PREPROCESS alice %%i
@for %%i in (backup, restore) do @call :PREPROCESS burp %%i
@for %%i in (array, blob) do @call :PREPROCESS dsql %%i
@for %%i in (metd) do @call :PREPROCESS dsql %%i -gds_cxx -cxx
@for %%i in (DdlNodes) do @call :PREPROCESS dsql %%i -gds_cxx
@for %%i in (exe, extract) do @call :PREPROCESS dudley %%i
@for %%i in (gpre_meta) do @call :PREPROCESS gpre %%i
@for %%i in (dfw, dpm, dyn, dyn_def, dyn_del, dyn_mod, dyn_util, fun, grant, ini, met, pcmet, scl) do @call :PREPROCESS jrd %%i -gds_cxx
@for %%i in (codes) do @call :PREPROCESS misc %%i
@for %%i in (build_file) do @call :PREPROCESS msgs %%i
@for %%i in (help, meta, proc, show) do @call :PREPROCESS qli %%i
@for %%i in (extract, isql, show) do @call :PREPROCESS isql %%i
@for %%i in (dba) do @call :PREPROCESS utilities/gstat %%i
@for %%i in (security) do @call :PREPROCESS utilities/gsec %%i
@for %%i in (stats) do @call :PREPROCESS utilities %%i
@goto :EOF

:END

