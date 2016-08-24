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
# It is provided "as is" without express or implied warranty
#

TOBEGEN	= ogdi_info
TARGETGEN=$(PROGGEN)

SOURCES = ogdi_info.c

INCLUDES = $(CURRENT_INCLUDE) $(OGDI_INCLUDE) $(PROJ_INCLUDE)\
	   $(ZLIB_INCLUDE) $(GENERAL_INCLUDE) $(TCLTK_INCLUDE)

CFLAGS 	= $(INCLUDES) $(COMMON_CFLAGS) $(FLAGS_X86DEF)

LINK_LIBS= $(OGDI_LINKLIB) $(ZLIB_LINKLIB) $(RPC_LINKLIB) \
           $(PROJ_STATICLIB) $(MATH_LINKLIB) \
	   $(LIBC_LINKLIB) $(UCB_STATICLIB) $(DL_LINKLIB) \
	   $(WIN_LINKLIB)

include $(TOPDIR)/config/common.mak

all: MKOBJECTDIR ogdi_info.c
	$(MAKE) --directory $(OBJDIR) -f ../makefile PASS='depend' $(TARGETGEN)

clean: default-clean
