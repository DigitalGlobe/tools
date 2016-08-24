#
#  Zlib compression library makefile
#
# Copyright (C) 1996 Logiciels et Applications Scientifiques (L.A.S.) Inc
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

#
# The names of the targets to build
#
TOBEGEN		= rpcgen
TARGETGEN	= $(PROGGEN)

#
# Source files
#
SOURCES = rpc_main.c rpc_hout.c rpc_cout.c rpc_parse.c rpc_scan.c \
	rpc_util.c rpc_svcout.c rpc_clntout.c rpc_tblout.c rpc_sample.c

#
# Compilation flags
#
INCLUDES = $(CURRENT_INCLUDE) $(OGDI_INCLUDE) $(PROJ_INCLUDE)\
	   $(ZLIB_INCLUDE) $(GENERAL_INCLUDE) $(TCLTK_INCLUDE)
CFLAGS 	= $(INCLUDES) $(COMMON_CFLAGS) $(FLAGS_X86DEF)
LINK_LIBS= $(WIN_LINKLIB) $(RPC_LINKLIB)

#
# Include the common configuration
#
include $(TOPDIR)/config/common.mak

#
# Primary target
#
all: MKOBJECTDIR
	@echo "making $(TARGETGEN)"
	$(MAKE) --directory $(OBJDIR) -f ../makefile PASS='depend' $(TARGETGEN)

clean: default-clean
