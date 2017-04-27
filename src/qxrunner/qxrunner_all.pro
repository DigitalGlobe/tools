#----------------------------------------------------------------------
# File:    qxrunner_all.pro
# Purpose: qmake config file to create all Qx libraries and demo
#          programs. Set the CONFIG variable in the particular config
#          files to build in a specific mode. 
#----------------------------------------------------------------------

TEMPLATE = subdirs

SUBDIRS = src/qxrunner \
          src/qxcppunit \
          examples/qxrunnerdemo \
          examples/qxcppunitdemo
