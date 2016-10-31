@REM
@REM make_html.bat
@REM

call :MAKE_HTML qxrunner
call :MAKE_HTML qxcppunit
REM call :MAKE_HTML qxrunner_all

goto EOF


REM
REM Subroutines
REM

:MAKE_HTML

doxygen %1.doxyfile


:EOF
