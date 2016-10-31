@echo off
REM
REM copy_design.bat
REM

call :COPY_ARTEFACTS qxrunner
call :COPY_ARTEFACTS qxcppunit
REM call :COPY_ARTEFACTS qxrunner_all

goto EOF


REM
REM Subroutines
REM

:COPY_ARTEFACTS

cd ..\doc\%1\html
copy ..\..\..\docgen\tab*.gif .
copy ..\..\..\docgen\tabs.css .

cd ..\..\..\docgen


:EOF
