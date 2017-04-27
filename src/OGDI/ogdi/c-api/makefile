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
#

TOBEGEN	= ogdi

ifeq ($(TARGET),Linux)
TARGETGEN=$(SHRDGEN)
else
TARGETGEN=$(DYNAGEN)
endif

SOURCES = ecs_dyna.c ecsregex.c ecssplit.c \
          ecsassoc.c ecshash.c ecstile.c server.c ecsdist.c \
          ecslist.c ecsinfo.c ecsgeo.c ecs_xdr.c ecs_xdrz.c\
          matrix.c client.c ecs_capabilities.c

INCLUDES = $(CURRENT_INCLUDE) $(GENERAL_INCLUDE) $(OGDI_INCLUDE) \
		$(GRASS_INCLUDE) $(ZLIB_INCLUDE) $(PROJ_INCLUDE) \
		$(EXPAT_INCLUDE)

CFLAGS 	= $(INCLUDES) $(COMMON_CFLAGS) -DMODULES_PATH="\"$(INST_LIB)/ogdi/\""

LINK_LIBS= $(RPC_LINKLIB) $(ZLIB_LINKLIB) $(EXPAT_LINKLIB) $(WIN_LINKLIB) \
		$(PROJ_STATICLIB) $(MATH_LINKLIB)

EXTRA_INSTALL_TARGETS	=	install-so-link

include $(TOPDIR)/config/common.mak

all: MKOBJECTDIR ecs_xdr.c
	$(MAKE) --directory $(OBJDIR) -f ../makefile PASS='depend' $(TARGETGEN)
	$(COPY_LOCAL)

#
#	Because the RPCGEN process doesn't work too well on many
#	platforms we disable it by default, and make people do a "make rpcgen"
#	to regenerate ecs_xdr.c explicitly.
#
#ecs_xdr.c: ../include/ecs.x

rpcgen:
	-$(RM) -f ecs.x
	-$(RM) -f $@.raw
	-$(RM) -f $@
	$(FILECOPY) ../include/ecs.x ecs.x
	rpcgen -C -c -o $@.raw ecs.x
	perl ../scripts/xdr_clean.pl -o $@ $@.raw
	$(RM) -f ecs.x

clean: default-clean

