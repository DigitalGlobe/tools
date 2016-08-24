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
#
#
# win32 configuration file
#

MAKE 		= $(MAKE_COMMAND) $(MAKEOVERRIDES)


#
# Platform specific symbols
#

Platform	= win32

#
# Standard tools
#

CC		= cl.exe /nologo
LD		= link.exe /nologo
SHLIB_LD	= link.exe /nologo
AR		= lib.exe /nologo
FILECOPY	= cp
MKDIR		= mkdir
DIRCOPY		= cp -rf
ifndef SHELL
SHELL		= /bin/sh
endif
MKINSTALLDIR	= $(TOPDIR)/config/mkinstalldirs
RSC             = rc.exe
RM		= rm
RMALL           = rm -rf
RMDIR           = rmdir


REVERSETOPDIR   = $(subst /,\,$(TOPDIR))

#
# File name extensions
#

APP_EXT         = .exe
ARCH_EXT        = lib
LIB_PREFIX	= 
LIB_SUFFIX	= .lib
OBJ_EXT		= obj
SHLIB_EXT	= dll

#
# Command switches
#

DEFINE_SW	= /D
INCL_SW		= /I
LIB_SW		= /
LINK_SW		= 


#
# Endian definition, could be little or big
#

BIG_ENDIAN          = 0

#
#Compilation and linking flags
#

# These flags are appropriate for a compiling with Visual C++ 5.0
# With these flags, you could run purify.
#

WINCPP_DEBUG	= /GX /D_DEBUG /Zi $(CPP_DEBUG) 
WINCPP_RELEASE	= /GX /Gi- /Gy /Od /DNDEBUG $(CPP_RELEASE) 
LINK_DEBUG	= /DEBUG /INCREMENTAL:no /FIXED:NO
LINK_RELEASE	= /INCREMENTAL:no /OPT:REF

#flags

# These flags are appropriate for a compiling with Visual C++ 4.0
# With these flags, you could run purify.
#

#LINK_DEBUG	= /DEBUG /INCREMENTAL:no /debugtype:cv
#WINCPP_RELEASE	= /GX /Gi- /Gy /O2 /DNDEBUG $(CPP_RELEASE) 


SMARTHEAP_LIB =

ifneq ($(CFG),release)
OPTIMIZATION	= $(WINCPP_DEBUG)
LINK_OPTIMIZATION = $(LINK_DEBUG)
else
OPTIMIZATION  = $(WINCPP_RELEASE)
LINK_OPTIMIZATION = $(LINK_RELEASE)
endif

SHLIB_CFLAGS	=
SHLIB_LDFLAGS	= /DLL
COMMON_LDFLAGS	= /DEBUG

#
# Standard location of compiled component libraries
#

LINKDIR		= $(LIBDIR)

#
# INCLUDE locations for include command
#

SYSTEM_INCLUDE	= $(patsubst %,$(INCL_SW)%,$(subst \,/,$(subst ;, ,$(INCLUDE))))
COMPAT_INCLUDE  = $(INCL_SW)$(TOPDIR)/include/win32/compat

RPC_INCLUDE     = $(INCL_SW)$(TOPDIR)/external/rpc_win32/rpc
SYS_INCLUDE     = $(INCL_SW)$(TOPDIR)/include/win32/sys

# Library locations for link command

WIN_LINKLIB           =  user32.lib gdi32.lib wsock32.lib advapi32.lib kernel32.lib
LXLIB_LINKLIB         = $(LIBDIR)/lxlib.lib
ODBC_LINKLIB          = odbc32.lib odbccp32.lib
RPC_LINKLIB           = $(LIBDIR)/static/rpc.lib


#
# rules
#

COMMON_CFLAGS = /D_WINDOWS /DWIN32 /D_MBCS \
	/Dhypot=_hypot /DNO_DIRENT_H \
	/DSIGQUIT=SIGBREAK /Dioctl=Ioctl /DSIGPIPE=SIGTERM \
	/DSIGHUP=SIGTERM /DSIGALRM=SIGTERM   /Dpopen=_popen \
	/Dpclose=_pclose \
	/DMISSING_DLFCN_H \
	$(OPTIMIZATION) \
	/W3 /YX /MD /c /Fpheaders.pch

# Disabled since it causes issue on 64bit build, and no longer needed
# FLAGS_X86DEF = /D_X86_

$(subst :,\:,$(ARCHGEN)): $(OBJECTS)
	@echo Making archive file: $@
	$(AR) /OUT:$@ $^ 
	@echo $@ made successfully ...

DEF_FILE=$(TOBEGEN_STRIPPED).def
RES_FILE=$(TOBEGEN).res
RC_FILE=$(TOBEGEN).rc

$(subst :,\:,$(DYNAGEN)): $(DEF_FILE) $(OBJECTS)
	@echo Making dynamic file: $@
	@echo 
	$(SHLIB_LD) /DLL $(LINK_OPTIMIZATION) \
	$(filter %.$(OBJ_EXT),$^) $(LINK_LIBS) /DEF:$(filter %.def,$^) \
	/OUT:$(TOBEGEN).dll \
	/IMPLIB:$(TOPDIR)/lib/$(TARGET)/$(LIB_PREFIX)$(TOBEGEN).$(ARCH_EXT) \
	/OUT:$@

ifndef WITHICON
$(subst :,\:,$(PROGGEN)): $(OBJECTS)
	@echo Making executable file:  $@
	$(LD) $^ $(LINK_LIBS) \
	$(LINK_OPTIMIZATION) $(SMARTHEAP_LINKLIB)  \
	/OUT:$@

else
$(subst :,\:,$(PROGGEN)): $(RES_FILE) $(OBJECTS)
	@echo Making executable file:  $@
	$(LD) $(filter %.$(OBJ_EXT),$^) $(LINK_LIBS) \
	$(filter %.res,$^) \
	$(LINK_OPTIMIZATION) $(SMARTHEAP_LINKLIB) \
	/OUT:$@
endif


%.obj: %.c 
	$(CC) $(CFLAGS) $(CPPFLAGS) $< 

$(subst :,\:,$(RES_FILE)): $(RC_FILE)
	$(RSC) /fo"$@" $(GENERAL_INCLUDE) $(TCLTK_INCLUDE) $^

