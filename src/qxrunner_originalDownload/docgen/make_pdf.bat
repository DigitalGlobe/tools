@echo off
REM
REM make_pdf.bat
REM

call :MAKE_AND_COPY_PDF qxrunner
call :MAKE_AND_COPY_PDF qxcppunit
REM call :MAKE_AND_COPY_PDF qxrunner_all

goto EOF


REM
REM Subroutines
REM

:MAKE_AND_COPY_PDF

cd doc\latex\%1

copy ..\..\..\style.sty .
copy ..\..\..\index.sty .
copy ..\..\..\TBD!!!!!!.jpg .\banner.jpg

pdflatex refman.tex

makeindex -c -s index.sty refman.idx 

REM Multiple pdflatex for proper TOC creation

pdflatex refman.tex
pdflatex refman.tex
pdflatex refman.tex

copy refman.pdf ..\..\pdf\%1.pdf

cd ..\..\..


:EOF
