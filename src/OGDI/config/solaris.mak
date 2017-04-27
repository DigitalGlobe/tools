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
PLATFORM	= solaris
_SOLARIS	= 1
USE_TERMIO	= -DUSE_TERMIO

# 
# platform specific tools
#
AR		= /usr/ccs/bin/ar cr


#
# Compilation and linking flags
#
SHLIB_CFLAGS	= -fPIC
COMMON_CFLAGS	= $(OPTIMIZATION) -W -Wall -ansi -fPIC -D_SOLARIS=1 -DUNIX=1

SHLIB_LDFLAGS	= -shared -h $(@F)
COMMON_LDFLAGS  = $(OPTIMIZATION) 


UCB_STATICLIB = /usr/ucblib/libucb.a

#
# platform specific file locations
#
RPC_LINKLIB	= -lrpcsvc -lnsl -lsocket


#
# Endian definition, could be little or big
#

BIG_ENDIAN          = 1






