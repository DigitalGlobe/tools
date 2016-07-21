###
# $Id$
#
# File: makedefs.pri
# 
# Included by qt project files to set common things.
###

###
# LOCAL_ROOT also used for linking things like geotiff and OpenThread which
# are typically installed in /usr/local or /opt/local
###
unix:LOCAL_ROOT=/usr/local
macx:LOCAL_ROOT=/opt/local

###
# Edit to set the destination directory for "make install" target.
###
INSTALL_PREFIX=$$(OSSIM_HOME)/../ossim_qt4
