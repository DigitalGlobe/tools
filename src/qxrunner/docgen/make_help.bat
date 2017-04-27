@echo off
REM
REM make_help.bat
REM

call copy_design.bat

call :MAKE_AND_COPY_HTMLHELP qxrunner
call :MAKE_AND_COPY_HTMLHELP qxcppunit
REM call :MAKE_AND_COPY_HTMLHELP qxrunner_all

goto EOF


REM
REM Subroutines
REM

:MAKE_AND_COPY_HTMLHELP

cd ..\doc\%1\html

"C:\Programme\HTML Help Workshop\hhc.exe" index.hhp

copy index.chm ..\%1.chm

cd ..\..\..\docgen


:EOF
