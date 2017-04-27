#
# Copyright (C) 1997 Logiciels et Applications Scientifiques (L.A.S.) Inc
# Permission to use, copy, modify and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright notice appear in all copies, that
# both the copyright notice and this permission notice appear in
# supporting documentation, and that the name of L.A.S. Inc not be used 
# in advertising or publicity pertaining to distribution of the software 
# without specific, written prior permission. L.A.S. Inc. makes no
# representations about the suitability of this software for any purpose.
# It is provided "as is" without express or implied warranty.
#

TOBEGEN	= rpf
TARGETGEN=$(DYNAGEN)

SOURCES = rpf.c utils.c object.c 

INCLUDES = $(CURRENT_INCLUDE) $(GENERAL_INCLUDE) $(OGDI_INCLUDE) \
	   $(GRASS_INCLUDE) $(PROJ_INCLUDE)

CFLAGS 	= $(INCLUDES) $(COMMON_CFLAGS)

LINK_LIBS= $(OGDI_LINKLIB) $(VPF_STATICLIB) $(GLUTIL_STATICLIB) $(WIN_LINKLIB) $(MATH_LINKLIB)

include $(TOPDIR)/config/common.mak

all:  MKOBJECTDIR datadict.h
	$(MAKE) --directory $(OBJDIR) -f ../makefile PASS='depend' $(TARGETGEN)

clean: default-clean
