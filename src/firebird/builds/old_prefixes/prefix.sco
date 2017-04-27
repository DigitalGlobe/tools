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
	$(CC) -c $(PIC_FLAGS) $(VERSION_FLAG) -Fo$*.bin $<


PROD_DEBUG_OBJECTS=	nodebug.o
PROD_SHRLIB_DIR=	-Lsource/jrd
PROD_VERSION_FLAG=	-DPROD_BUILD
PROD_CFLAGS=		-O -quiet -DHADES

DEV_DEBUG_OBJECTS=	grammar.o dbg.o dbt.o dmp.o
DEV_SHRLIB_DIR=		-Lsource/jrd
DEV_VERSION_FLAG=	-DDEV_BUILD
DEV_CFLAGS=		-g -quiet

DEBUG_OBJECTS=		$($(VERSION)_DEBUG_OBJECTS)
SHRLIB_DIR=		$($(VERSION)_SHRLIB_DIR)
VERSION_FLAG=		$($(VERSION)_VERSION_FLAG)
CFLAGS=			$($(VERSION)_CFLAGS) -D$(SYSTEM)

ACCESS_METHOD=		gdslib.sco pipe
BACKEND_BRIDGE_MISC=	head5.o allp.o
BIN_PATH=		/usr/gds/bin
BRIDGE_MISC=		head5.o allp.o
COPT_DSQL_PASS1=	NO
COPT_DUDLEY_PARSE=	NO
COPT_JRD_CVT=		NO
COPT_QLI_HELP=		NO
DSQL_P_OBJECTS=		dsql_p_objects
EXAMPLES_DBS=		source/examples/
FORM_OBJECTS=		form.o
FORM_TRN_OBJECTS=	form_trn.o
FRED=			fred
FUNCSHR=		source/interbase/lib/gdsf_s
GDS_LINK=		$(SHRLIB_DIR) -lgds -lsocket
GDSLIB_BACKEND=		source/interbase/lib/gds_b.a
GDSLIB_LINK=		-link -Lsource/jrd -lgds_b -lrpc -lsocket -lcrypt_i
GDSSHR=			source/interbase/lib/gds_s
GDSSHR_LINK=		-link $(SHRLIB_DIR) -lgds_s -lgdsf_s -lgds_pyxis -lrpc -lyp -lrpc -lsocket -lcrypt_i
HLPDIR=			source/qli/
INCLUDES=		include_sco
INET_LIBRARY=		inet_server.a
INTL=			intl
INTL_CFLAGS=		$(CFLAGS)
INTL_MISC=		$(INTL_OBJECTS)
INTL_P_OBJS=		intl
INTL_P_MISC=		$(INTL_P_OBJECTS)
INTL_PIC_FLAGS=		$(PIC_FLAGS)
INTL_TARGET=		intl_objects intl_p_objs
IO_OBJECTS=		unix.o
IO_P_OBJECTS=		unix.bin
JRD_MISC_OBJECTS=	stubs.o
JRD_P_MISC_OBJECTS=	nodebug.bin shrinit.bin stubs.bin
LANG_OBJECTS=		ada.o
LANGUAGES=		cc cxx ndl make8 gdl1 ada sco_ada
LINKABLE_LIBS=		burplib.a isqllib.a qlilib.a
LOCK_MANAGER=		manager
MARION_DB=		-d source/marion.gdb
MUISQL=                 muisql
MUISQL_MU_LIB=          -L/usr/gds/qa_tools/lib -lmu
PIC_FLAGS=		$(CFLAGS) -DSHLIB_DEFS
PIPE=			gds.a gds_pipe.a gds_pipe
PIPE_LIBRARY=		gds_pipe.a
PYXIS=			pyxis
PYXIS_MISC_OBJS=	$(PYXIS_MISC)
PYXIS_P_MISC_OBJS=	$(PYXIS_P_MISC)
PYXIS_OBJECTS=		pyxis_objects
PYXIS_MISC_OBJECTS=	$(PYXDIR)cdm.o $(PYXDIR)vt100.o
REG_HELP=		isc_ins.hlp
REMOTE_GDSSHR=		$(GDSLIB)
REMOTE_GDSSHR_LINK=	$(GDSLIB_LINK)
REMOTE_P_OBJS=		rem_p_objects
SCO_FUNCSHR=		$(FUNCSHR)
SCO_GDSSHR=		$(GDSSHR)
SCO_OBJS_BURPLIB=	$(JRD_LINK)
SCO_OBJS_ISQLLIB=	$(JRD_LINK)
SCO_OBJS_QLILIB=	$(JRD_LINK_OBJECTS)
SCO_OBJS_REMLIB=	$(REMOTE_OBJECTS) $(JRD_LINK)
SCO_SOCKET_LIB=		-lsocket
SCREEN_LIBS=		-lcurses
SERVER_LINK=		$(GDSSHR_LINK)
SETUP_ISC=		ISC_USER=sysdba; ISC_PASSWORD=masterkey; export ISC_USER ISC_PASSWORD;
SPECIAL_OPT=		source/special_opt
UTILITIES=		drop
WAL_P_OBJS=		wal_p_objects

INET_SERVER_DEST=	source/interbase/bin/gds_inet_srvr
DNET_SERVER_DEST=	source/interbase/bin/gds_dnet_srvr
AMBX_SERVER_DEST=	source/interbase/bin/gds_server
INET_LIB_DEST=		source/interbase/lib/gds_inet_svr.a
DNET_LIB_DEST=		source/interbase/lib/gds_dnet_svr.a

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

