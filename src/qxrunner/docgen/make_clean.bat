@echo off
REM
REM make_clean.bat
REM

call :MAKE_CLEAN qxrunner
call :MAKE_CLEAN qxcppunit
REM call :MAKE_CLEAN qxrunner_all

goto EOF


REM
REM Subroutines
REM

:MAKE_CLEAN

cd ..\doc\%1\html

del *.hhc
del *.hhk
del *.hhp
del *.chm

cd ..\..\..\docgen


:EOF
