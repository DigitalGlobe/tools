#
#  Zlib compression library makefile
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

#
# The names of the targets to build
#
TOBEGEN		= rpc
TARGETGEN	= $(ARCHGEN)

#
# Source files
#
SOURCES =	clnt_raw.c \
		xdr.c \
		clnt_tcp.c \
		clnt_udp.c \
		pmap_rmt.c \
		rpc_prot.c \
		svc_autu.c \
		svc_auth.c \
		svc_raw.c \
		svc_run.c \
		svc_tcp.c \
		svc_udp.c \
		xdr_mem.c \
		xdr_rec.c \
		auth_non.c \
		auth_uni.c \
		authunix.c \
		bindresv.c \
		clnt_gen.c \
		clnt_per.c \
		clnt_sim.c \
		get_myad.c \
		getrpcen.c \
		getrpcpo.c \
		pmap_cln.c \
		pmap_get.c \
		pmap_gma.c \
		pmap_pro.c \
		pmap_pr.c \
		rpc_call.c \
		rpc_comm.c \
		svc_simp.c \
		xdr_arra.c \
		xdr_floa.c \
		xdr_refe.c \
		xdr_stdi.c \
		svc.c \
		bcopy.c \
		nt.c

#
# Compilation flags
#
INCLUDES = $(CURRENT_INCLUDE) $(OGDI_INCLUDE) $(PROJ_INCLUDE)\
	   $(ZLIB_INCLUDE) $(GENERAL_INCLUDE) $(TCLTK_INCLUDE)
CFLAGS 	= $(INCLUDES) $(COMMON_CFLAGS) $(FLAGS_X86DEF)
LINK_LIBS= $(WIN_LINKLIB)


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
	$(MAKE) -f makeportmap.mak
	$(MAKE) -f makerpcinfo.mak

clean: default-clean

