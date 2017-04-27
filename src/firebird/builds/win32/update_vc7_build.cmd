::@echo off
:: This is the script to convert MSVC8 build to MSVC7 build using VSPC utility
:: http://sourceforge.net/projects/vspc/
::

copy /y msvc8\*.vcproj msvc7
copy /y msvc8\*.sln msvc7
copy /y msvc8\*.vsprops msvc7
vspc VS2005 VS2003 msvc7\firebird2.sln
vspc VS2005 VS2003 msvc7\Firebird2Boot.sln
vspc VS2005 VS2003 msvc7\Firebird2_Examples.sln

call adjust_vc7_files.cmd
