$Id: README.txt 11230 2007-06-18 12:36:11Z dburken $

*******************************    NOTE(s)     ***************************
For a working version of "imagelinker" and "iview" please use the 
"ossim_qt" module not this module "ossim_qt4. (D. Burken)

This module "ossim_qt4" contains code from the "ossim_qt" module that is 
being ported from QT3 to QT4.  



README file for "Open Source Software Image Map (OSSIM) QT" library and 
applications.

Web site:  http://www.ossim.org/

Build instructions:

For most unix type platforms via command line:
1) Requirements:
   - ossim core library
   - qt development library version 4.3 or greater.

2) Environment variables:
   - OSSIM_DEV_HOME (This should be the directory where thing are under.) 
   - OSSIM_HOME
   - OSSIM_QT_HOME
   - QTDIR

tcsh/csh users (replace "software" with your path):
setenv QTDIR /usr
setenv OSSIM_DEV_HOME /software
setenv OSSIM_HOME /software/ossim
setenv OSSIM_QT_HOME /software/ossim_qt

bash/sh users:
set QTDIR=/usr
set OSSIM_DEV_HOME=/software
set OSSIM_HOME=/software/ossim
set OSSIM_QT_HOME=/software/ossim_qt
export QTDIR OSSIM_DEV_HOME OSSIM_HOME OSSIM_QT_HOME

3) To build:
cd /software/ossim_qt
qmake ossim_qt.pro
make
make install

You should now have two applications "iview" and "imagelinker" in the 
ossim_qt/bin directory.

NOTE:
If you get a qmake error that it cannot deduce the SPEC you can 


