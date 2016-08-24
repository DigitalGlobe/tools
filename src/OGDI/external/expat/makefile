#
#  Expat XML library makefile
#
# Copyright (C) 2001 Information Interoperability Institute (3i)
# Permission to use, copy, modify and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright notice appear in all copies, that
# both the copyright notice and this permission notice appear in
# supporting documentation, and that the name of 3i not be used 
# in advertising or publicity pertaining to distribution of the software 
# without specific, written prior permission.  3i makes no
# representations about the suitability of this software for any purpose.
# It is provided "as is" without express or implied warranty.
#

#
# The names of the targets to build
#
TOBEGEN		= expat_ogdi$(OGDI_VERSION)
TARGETGEN	= $(DYNAGEN)

#
# Source files
#
SOURCES =	xmlrole.c \
		xmlparse.c \
		xmltok.c
	    

#
# Include the common configuration
#
include $(TOPDIR)/config/common.mak

#
# Compilation flags
#
INCLUDES = $(CURRENT_INCLUDE)
CFLAGS 	= $(INCLUDES) $(COMMON_CFLAGS) \
	-DVERSION=\"expat_1.95.1\" -DPACKAGE="expat"

ifeq ($(BIG_ENDIAN),1)
CFLAGS 	:=    $(CFLAGS) -DWORDS_BIGENDIAN
endif

#
# Primary target
#
all: MKOBJECTDIR
	@echo "making $(TARGETGEN)"
	$(MAKE) --directory $(OBJDIR) -f ../makefile PASS='depend' $(TARGETGEN)

clean: default-clean
