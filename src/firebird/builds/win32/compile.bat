@echo off
::==============
:: compile.bat solution, output, [projects...]
::   Note: MSVC7/8 don't accept more than one project
::
::   Note2: Our MSVC8/9 projects create object files in temp/$platform/$config
::     but we call devenv with $config|$platform (note variable in reverse order
::      and odd syntax.) This extended syntax for devenv does not seem to be
::      supported in MSVC7 (despite documentation to the contrary.)

setlocal
set solution=%1.sln
set output=%2
set projects=

@if "%FB_DBG%"=="" (
	set config=release
) else (
	set config=debug
)

if %MSVC_VERSION% GEQ 10 (
	set config=/p:Configuration=%config% /p:platform=%FB_TARGET_PLATFORM%
) else (
    if %MSVC_VERSION% GEQ 8 (
	    set config=%config%|%FB_TARGET_PLATFORM%
    )
)

if "%VS_VER_EXPRESS%"=="1" (
	set exec=vcexpress
) else (
    if %MSVC_VERSION% GEQ 10 (
	    set exec=msbuild
	) else (
        set exec=devenv
    )
)

shift
shift

:loop_start


if "%1" == "" goto loop_end

if "%exec%" == "msbuild" (
    set solution=%FB_ROOT_PATH%\builds\win32\%VS_VER%\%1.vcxproj
    set projects=
) else (
    set projects=%projects% /project %1
)

shift
goto loop_start

:loop_end

if "%exec%" == "msbuild" (
    set output=/filelogger /flp:logfile=%output%
    set config=%config% /p:optimize=false /p:DisabledWarnings=4267
) else (
    set output=/OUT %output%
)

echo %exec% %solution% %projects% %FB_CLEAN% %config% %output%
%exec% %solution% %projects% %FB_CLEAN% %config% %output%

endlocal

goto :EOF
