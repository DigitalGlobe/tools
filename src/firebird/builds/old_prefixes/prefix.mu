# The contents of this file are subject to the Interbase Public
# License Version 1.0 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy
# of the License at http://www.Inprise.com/IPL.html
#
# Software distributed under the License is distributed on an
# "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
# or implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code was created by Inprise Corporation
# and its predecessors. Portions created by Inprise Corporation are
# Copyright (C) Inprise Corporation.
#
# All Rights Reserved.
# Contributor(s): ______________________________________.
.SUFFIXES: .c .e
.e.c:
	$(GPRE) $(GPRE_FLAGS) $<
.SUFFIXES: .bin .o .c
.c.o:
	$(CC) -c $(CFLAGS) $(VERSION_FLAG) $<
.c.bin:
	$(CC) -c $(CFLAGS) $(PIC_FLAGS) $(VERSION_FLAG) -o $*_temp.o $<
	mv $*_temp.o $*.bin


PROD_DEBUG_OBJECTS=	nodebug.o
PROD_SHRLIB_DIR=
PROD_VERSION_FLAG=	-DPROD_BUILD
PROD_CFLAGS=		-O -DHADES

DEV_DEBUG_OBJECTS=	grammar.o dbg.o dbt.o dmp.o
DEV_SHRLIB_DIR=		-L source/jrd
DEV_VERSION_FLAG=	-DDEV_BUILD
DEV_CFLAGS=		-g

DEBUG_OBJECTS=		$($(VERSION)_DEBUG_OBJECTS)
SHRLIB_DIR=		$($(VERSION)_SHRLIB_DIR)
VERSION_FLAG=		$($(VERSION)_VERSION_FLAG)
CFLAGS=			$($(VERSION)_CFLAGS) -Wf,-XNg1000

ACCESS_METHOD=		pipe
BACKEND_BRIDGE_MISC=	head5.o allp.o
BIN_PATH=		/usr/gds/bin
BRIDGE_MISC=		head5.o allp.o
COPT_JRD_SDW=           NO
DNET_SERVER=		dnet_server
DNET_SERVER_LIB=	dnet_server.a
EXAMPLES_DBS=		source/examples/
FORM_OBJECTS=		form.o
FORM_TRN_OBJECTS=	form_trn.o
FRED=			fred
GDS_LINK=		$(GDSSHR_LINK)
GDSLIB_BACKEND=		source/interbase/lib/gds_b.a
GDSLIB_LINK=		-Lsource/jrd -lgds_b -ldnet
GDSSHR=			source/interbase/lib/gds.a
GDSSHR_LINK=		-Lsource/jrd -lgds -ldnet
HLPDIR=			source/qli/
INET_LIBRARY=		inet_server.a
INTL=			intl
INTL_CFLAGS=		$(CFLAGS)
INTL_MISC=		$(INTL_OBJECTS)
INTL_TARGET=		intl_objects
IO_OBJECTS=		unix.o
LANG_OBJECTS=		
LANGUAGES=		cc cxx ndl make14 gdl1
LOCK_MANAGER=		manager
MARION_DB=		-d source/marion.gdb
NET_OBJECTS=		$(REMDIR)dnet.o
PIPE=			gds.a gds_pipe.a gds_pipe
PIPE_LIBRARY=		gds_pipe.a
PYXIS=			pyxis
PYXIS_MISC_OBJS=	$(PYXIS_MISC)
PYXIS_P_MISC_OBJS=	$(PYXIS_P_MISC)
PYXIS_OBJECTS=		pyxis_objects
PYXIS_MISC_OBJECTS=	$(PYXDIR)cdm.o $(PYXDIR)vt100.o
REG_HELP=		isc_ins_hlp.dat
REMOTE_GDSSHR=		$(GDSLIB)
REMOTE_GDSSHR_LINK=	$(SERVER_LINK)
SCREEN_LIBS=		-lcurses -ltermlib
SERVER_LINK=		$(GDSLIB_LINK)
SPECIAL_OPT=		source/special_opt
UTILITIES=		drop
WHYPS_O=		whyps.o

INET_SERVER_DEST=	source/interbase/bin/gds_inet_server
DNET_SERVER_DEST=	source/interbase/bin/gds_dnet_server
AMBX_SERVER_DEST=	source/interbase/bin/gds_server
INET_LIB_DEST=		source/interbase/lib/gds_inet_server.a
DNET_LIB_DEST=		source/interbase/lib/gds_dnet_server.a

SH=			sh -c
RM=			rm -f
CHMOD=			chmod
CHMOD_6=		chmod 666
CHMOD_7=		chmod 777
CHMOD_S7=		chmod 06777
MV=			mv -f
TOUCH=			touch
CP=			cp
ECHO=			echo
QUIET_ECHO=		@echo
CD=			cd
CAT=			cat
AR=			ar r
EXPAND_DBNAME=		@echo No need to expand...
COMPRESS_DBNAME=	@echo No need to compress...

ARCH_EXT=		.a
EXEC_EXT=

V3PRINTER=		source/lock/printv3.o

