::
:: This bat file doesn't use cd, all the paths are full paths.
:: with this convention this bat file is position independent
:: and it will be easier to move the place of somefiles.
::

@echo off
set ERRLEV=0

:CHECK_ENV
@call setenvvar.bat
@if errorlevel 1 (goto :END)

@call set_build_target.bat %*


::===========
:MAIN
@echo.
@echo Copy autoconfig.h
@del %FB_ROOT_PATH%\src\include\gen\autoconfig.h 2> nul
@copy %FB_ROOT_PATH%\src\include\gen\autoconfig_msvc.h %FB_ROOT_PATH%\src\include\gen\autoconfig.h > nul
@echo Creating directories
@rmdir /s /q %FB_GEN_DIR% 2>nul
:: Remove previously generated output, and recreate the directory hierarchy. Note the exceptions to the rule!
for %%v in ( alice burp dsql dudley gpre isql journal jrd misc msgs qli examples ) do (
  if NOT "%%v"=="journal" (@mkdir %FB_GEN_DIR%\%%v )
)

@rmdir /s /q %FB_GEN_DIR%\utilities 2>nul
@mkdir %FB_GEN_DIR%\utilities 2>nul
@mkdir %FB_GEN_DIR%\utilities\gstat 2>nul
@mkdir %FB_GEN_DIR%\utilities\gsec 2>nul

::=======
call :btyacc
if "%ERRLEV%"=="1" goto :END
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\btyacc.exe %FB_GEN_DIR%\ > nul

@echo Generating DSQL parser...
@call parse.bat
if "%ERRLEV%"=="1" goto :END

::=======
@echo.
@echo Building BLR Table
@call blrtable.bat

::=======
call :gpre_boot
if "%ERRLEV%"=="1" goto :END
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\gpre_boot.exe %FB_GEN_DIR% > nul

::=======
@echo Preprocessing the source files needed to build gbak_embed, gpre_embed and isql_embed...
@call preprocess.bat BOOT
::=======
call :gbak_embed
if "%ERRLEV%"=="1" goto :END

call :gpre_embed
if "%ERRLEV%"=="1" goto :END

call :isql_embed
if "%ERRLEV%"=="1" goto :END

@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\gbak_embed.exe %FB_GEN_DIR% > nul
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\gpre_embed.exe %FB_GEN_DIR% > nul
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\isql_embed.exe %FB_GEN_DIR% > nul
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\firebird\bin\fbembed.dll %FB_GEN_DIR% > nul

for %%v in ( icuuc30 icudt30 icuin30 ) do (
@copy %FB_ICU_SOURCE_BIN%\%%v.dll %FB_GEN_DIR% >nul 2>&1
)

::=======
@call :databases
::=======
@echo Preprocessing the entire source tree...
@call preprocess.bat
::=======
@call :msgs
if "%ERRLEV%"=="1" goto :END
@call :codes
if "%ERRLEV%"=="1" goto :END
::=======
@call create_msgs.bat msg
::=======
@call :NEXT_STEP
@goto :END


::===================
:: BUILD btyacc
:btyacc
@echo.
@echo Building btyacc (%FB_OBJ_DIR%)...

@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot btyacc_%FB_TARGET_PLATFORM%.log btyacc
if errorlevel 1 call :boot2 btyacc
goto :EOF

::===================
:: BUILD gpre_boot
:gpre_boot
@echo.
@echo Building gpre_boot (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot gpre_boot_%FB_TARGET_PLATFORM%.log gpre_boot
if errorlevel 1 goto :gpre_boot2
goto :EOF

::===================
:: Error gpre_boot
:gpre_boot2
echo.
echo Error building gpre_boot, see gpre_boot_%FB_TARGET_PLATFORM%.log
echo.
set ERRLEV=1
goto :EOF


::===================
:: BUILD gbak_embed
:gbak_embed
@echo.
@echo Building gbak_embed (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot gbak_embed_%FB_TARGET_PLATFORM%.log gbak_embed
if errorlevel 1 call :boot2 gbak_embed
@goto :EOF

::===================
:: BUILD gpre_embed
:gpre_embed
@echo.
@echo Building gpre_embed (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot gpre_embed_%FB_TARGET_PLATFORM%.log gpre_embed
if errorlevel 1 call :boot2 gpre_embed
@goto :EOF

::===================
:: BUILD isql_embed
:isql_embed
@echo.
@echo Building isql_embed (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot isql_embed_%FB_TARGET_PLATFORM%.log isql_embed
if errorlevel 1 call :boot2 isql_embed
@goto :EOF

::===================
:: ERROR boot
:boot2
echo.
echo Error building %1, see %1_%FB_TARGET_PLATFORM%.log
echo.
set ERRLEV=1
goto :EOF


::===================
:: BUILD messages
:msgs
@echo.
@echo Building build_msg (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot build_msg_%FB_TARGET_PLATFORM%.log build_msg
if errorlevel 1 goto :msgs2
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\build_msg\build_msg.exe   %FB_GEN_DIR%\ > nul
@goto :EOF
:msgs2
echo.
echo Error building build_msg, see build_msg_%FB_TARGET_PLATFORM%.log
echo.
set ERRLEV=1
goto :EOF

::===================
:: BUILD codes
:codes
@echo.
@echo Building codes (%FB_OBJ_DIR%)...
@call compile.bat %FB_ROOT_PATH%\builds\win32\%VS_VER%\Firebird2Boot codes_%FB_TARGET_PLATFORM%.log codes
if errorlevel 1 goto :codes2
@copy %FB_ROOT_PATH%\temp\%FB_OBJ_DIR%\codes\codes.exe   %FB_GEN_DIR%\ > nul
@goto :EOF
:codes2
echo.
echo Error building codes, see codes_%FB_TARGET_PLATFORM%.log
echo.
set ERRLEV=1
goto :EOF

::==============
:databases
@rmdir /s /q %FB_GEN_DIR%\dbs 2>nul
@mkdir %FB_GEN_DIR%\dbs 2>nul

@echo create database '%FB_GEN_DB_DIR%\dbs\security2.fdb'; | "%FB_GEN_DIR%\isql_embed" -q
@"%FB_GEN_DIR%\isql_embed" -q %FB_GEN_DB_DIR%/dbs/security2.fdb -i %FB_ROOT_PATH%\src\dbs\security.sql

@%FB_GEN_DIR%\gbak_embed -r %FB_ROOT_PATH%\builds\misc\metadata.gbak %FB_GEN_DB_DIR%/dbs/metadata.fdb

@call create_msgs.bat db

@%FB_GEN_DIR%\gbak_embed -r %FB_ROOT_PATH%\builds\misc\help.gbak %FB_GEN_DB_DIR%/dbs/help.fdb
@copy %FB_GEN_DIR%\dbs\metadata.fdb %FB_GEN_DIR%\dbs\yachts.lnk > nul

@goto :EOF


::==============
:NEXT_STEP
@echo.
@echo    You may now run make_all.bat [DEBUG] [CLEAN]
@echo.
@goto :EOF

:END

