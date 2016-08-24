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

# Darwin (MacOS X) configuration file

#
# Read in the configuration common to all Unix
#
include $(TOPDIR)/config/unix.mak

#
# platform specific Symbols
#
PLATFORM	= darwin

# 
# platform specific tools
#
AR		= ar cr

#
# Compilation and linking flags
#
SHLIB_CFLAGS	= -fno-common
COMMON_CFLAGS	= $(OPTIMIZATION) -fno-common

SHLIB_LDFLAGS	= -dynamiclib
COMMON_LDFLAGS  = $(OPTIMIZATION)


#
# File name extensions
#
SHLIB_EXT	= dylib

UCB_STATICLIB = 

#
# Endian definition
#

BIG_ENDIAN          = 1


#
# make rules
#
$(ARCHGEN): $(OBJECTS)
	@echo Making archive file: $@
	$(AR) $@ $^ 
	ranlib $@
	@echo $@ made successfully ...
	
$(DYNAGEN): $(OBJECTS)
	@echo Making shared library: $@
	$(SHLIB_LD) $(SHLIB_LDFLAGS) $(COMMON_LDFLAGS) -o $@ $^ $(LINK_LIBS) 
	@echo $@ made successfully ...
