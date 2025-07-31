# Copyright (C) 2001 Her Majesty the Queen in Right of Canada.
# Permission to use, copy, modify and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright notice appear in all copies, that
# both the copyright notice and this permission notice appear in
# supporting documentation, and that the name of Her Majesty the Queen
# in Right  of Canada not be used in advertising or publicity pertaining
# to distribution of the software without specific, written prior
# permission.  Her Majesty the Queen in Right of Canada makes no
# representations about the suitability of this software for any purpose.
# It is provided "as is" without express or implied warranty.

# Generic unix configuration file, processed by configure to make 
# platform specific.

#
# Read in the configuration common to all Unix
#
include /config/unix.mak

#
# platform specific Symbols
#
PLATFORM= 
_SOLARIS= 1
USE_TERMIO= -DUSE_TERMIO

# 
# platform specific tools
#
AR= ar cr


#
# Compilation and linking flags
#
SHLIB_CFLAGS= -fPIC
COMMON_CFLAGS=  -Wall -O2  -Wall -DUNIX=1

SHLIB_LDFLAGS= -shared 
COMMON_LDFLAGS  =  

RPC_INCLUDES = -DHAVE_STD_RPC_INCLUDES

UCB_STATICLIB = 

TCL_INCLUDE = -I/usr/include/tcl
TCL_LINKLIB =

#
#Install Locations
#
prefix= /usr
exec_prefix = 
INST_INCLUDE= /include
INST_LIB= /lib
INST_BIN= /bin

#
# platform specific file locations
#
#RPC_LINKLIB= -lrpcsvc -lnsl -lsocket
#RPC_LINKLIB=-ldl -ldbmalloc
RPC_LINKLIB=-ldl 

#
# Endian definition, could be little or big
#
BIG_ENDIAN          = 0

#
# Handle internal/external PROJ.4 library usage.
#

PROJ_SETTING=external

ifeq (,external)
PROJ_INCLUDE :=-I/usr/include
PROJ_STATICLIB := -lproj
endif

#
# Handle internal/external ZLIB library usage.
#

ZLIB_SETTING=external

ifeq (,external)
ZLIB_INCLUDE :=-I/usr/include
ZLIB_LINKLIB := -lz
endif

#
# Handle internal/external/disabled "Expat" library handling.
#

EXPAT_SETTING=external

ifeq (,external)
EXPAT_INCLUDE := -I/usr/include
EXPAT_LINKLIB := -lexpat
endif

ifeq (,disabled)
EXPAT_INCLUDE := -DEXPAT_DISABLED
EXPAT_LINKLIB := 
endif


