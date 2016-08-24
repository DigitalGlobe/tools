#
#  Copyright (c) 1997 Logiciels et Applications Scientifiques (L.A.S.) Inc.
#  Il est strictement interdit de publier ou de devoiler le contenu de ce
#  programme sans avoir prealablement obtenu la permission de L.A.S. Inc.
#  It is strictly forbidden to publish or divulge the content of
#  these programs without the prior permission of L.A.S. Inc.
#

TOBEGEN	= vpf
TARGETGEN=$(ARCHGEN)

SOURCES = vpfselec.c \
	vpftidx.c \
	vpfwrite.c \
	vpftable.c \
	vpfrelat.c \
	vpfprim.c \
	vpfquery.c \
	get_feat.c \
	vpfdproj.c \
	coorgeom.c \
	vpfspx.c \
	set.c \
	linklist.c \
	reduce2.c \
	vpfread.c \
	vpfprop.c \
	strfunc.c \
	musedir.c

INCLUDES = $(CURRENT_INCLUDE) $(GENERAL_INCLUDE) $(VPF_INCLUDE)

CFLAGS 	= $(INCLUDES) $(COMMON_CFLAGS) $(UNIX_DEFINE) \
	-DSYS_BIG_ENDIAN=$(BIG_ENDIAN)

include $(TOPDIR)/config/common.mak

ifeq ($(HAVE_ICONV),yes)
CFLAGS := $(CFLAGS) -DHAVE_ICONV
endif

all: MKOBJECTDIR 
	$(MAKE) --directory $(OBJDIR) -f ../makefile PASS='depend' $(TARGETGEN)

clean: default-clean
