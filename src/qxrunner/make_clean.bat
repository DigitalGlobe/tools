@echo off
REM
REM make_clean.bat
REM
REM Removes files in the lib directory and all temporary files/directories.

del /F/S/Q lib\*.*

del qxrunner_all.ncb
del/A:H *.suo
del Makefile*

rmdir /S/Q .\doc\qxrunner
rmdir /S/Q .\doc\qxcppunit
REM rmdir /S/Q .\doc\html
REM rmdir /S/Q .\doc\latex

call :MAKE_CLEAN src\qxrunner
call :MAKE_CLEAN examples\qxrunnerdemo
call :MAKE_CLEAN src\qxcppunit
call :MAKE_CLEAN examples\qxcppunitdemo

goto EOF


REM
REM Subroutines
REM

:MAKE_CLEAN

del .\%1\*.ncb
del .\%1\*.pdb
del/A:H .\%1\*.suo
del .\%1\*.vcproj.*.user
del .\%1\Makefile*

rmdir /S/Q .\%1\debug
rmdir /S/Q .\%1\debug_dll
rmdir /S/Q .\%1\release
rmdir /S/Q .\%1\release_dll
rmdir /S/Q .\%1\generatedfiles


:EOF
