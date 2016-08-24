# Copyright (C) 1996 Her Majesty the Queen in Right of Canada.
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

# Solaris configuration file

#
# Read in the configuration common to all Unix
#
include $(TOPDIR)/config/unix.mak

#
# platform specific Symbols
#
PLATFORM	= linux
_SOLARIS	= 1
USE_TERMIO	= -DUSE_TERMIO

# 
# platform specific tools
#
AR		= ar cr


#
# Compilation and linking flags
#
SHLIB_CFLAGS	= -fPIC
COMMON_CFLAGS	= $(OPTIMIZATION) -W -Wall -ansi -fPIC -DUNIX=1 \
		-D_BSD_SOURCE -D_LINUX

SHLIB_LDFLAGS	= -shared 
COMMON_LDFLAGS  = $(OPTIMIZATION) 

RPC_INCLUDES = -DHAVE_STD_RPC_INCLUDES


UCB_STATICLIB = 

TCL_INCLUDE = -I/usr/include/tcl8.3
TCL_LINKLIB =

#
# platform specific file locations
#
#RPC_LINKLIB	= -lrpcsvc -lnsl -lsocket
#RPC_LINKLIB	=	-ldl -ldbmalloc
RPC_LINKLIB	=	-ldl

#
# Endian definition, could be little or big
#

BIG_ENDIAN          = 0






