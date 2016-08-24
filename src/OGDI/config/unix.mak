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



# Unix configuration file.  Symbols and macros common
# all Unix platforms.
#

UNIX		= 1
UNIX_DEFINE	= -Dunix

#
# Standard tools
#
CC		= gcc
LD		= gcc
SHLIB_LD	= gcc
AR		= ar cr
RM		= rm
RMALL		= rm -rf
FILECOPY	= cp
MKDIR		= mkdir
DIRCOPY		= cp -r
SHELL		= /bin/sh
MKINSTALLDIR	= $(TOPDIR)/config/mkinstalldirs

#
# File name extensions
#
APP_EXT		=
ARCH_EXT	= a
LIB_PREFIX	= lib
LIB_SUFFIX	=
OBJ_EXT		= o
SHLIB_EXT	= so

#
# Command switches
#
DEFINE_SW	= -D
INCL_SW		= -I
LIB_SW		= $(subst :, -l,:)
LINK_SW		= -L

ifeq ($(CFG),debug)
OPTIMIZATION	= -g
else
OPTIMIZATION	= -O
endif

#
# Standard location of compiled component libraries
#
LINKDIR		= $(BINDIR)

#
# Includes
#
SYSTEM_INCLUDE	= $(patsubst %,$(INCL_SW)%,$(subst \,/,$(subst :, ,$(INCLUDE))))

#
# Library locations for link command
#

RPC_LINKLIB	=
MATH_LINKLIB	= $(LIB_SW)m
DL_LINKLIB	= 

LIBC_LINKLIB	= -lc

#
# make rules
#
$(ARCHGEN): $(OBJECTS)
	@echo Making archive file: $@
	$(AR) $@ $^ 
	@echo $@ made successfully ...

$(PROGGEN): $(OBJECTS)
	@echo Making executable: $@
	$(LD) $(COMMON_LDFLAGS) $(COMMON_CFLAGS) -o $@ $^ $(LINK_LIBS)
	@echo $@ made successfully ...

$(SHRDGEN): $(OBJECTS)
	@echo Making shared library: $@
	$(SHLIB_LD) $(SHLIB_LDFLAGS) $(COMMON_LDFLAGS) $(COMMON_CFLAGS) -Wl,-soname,$(LIB_PREFIX)$(TOBEGEN).$(SHLIB_EXT).$(OGDI_MAJOR) -o $@ $^ $(LINK_LIBS) 
	cd $(TOPDIR)/bin/$(TARGET); ln -s $(LIB_PREFIX)$(TOBEGEN).$(SHLIB_EXT).$(OGDI_MAJOR).$(OGDI_MINOR) $(LIB_PREFIX)$(TOBEGEN).$(SHLIB_EXT); \
	ln -s $(LIB_PREFIX)$(TOBEGEN).$(SHLIB_EXT).$(OGDI_MAJOR).$(OGDI_MINOR) $(LIB_PREFIX)$(TOBEGEN).$(SHLIB_EXT).$(OGDI_MAJOR); cd $(CURDIR)
	@echo $@ made successfully ...

$(DYNAGEN): $(OBJECTS)
	@echo Making dynamic library: $@
	$(SHLIB_LD) $(SHLIB_LDFLAGS) $(COMMON_LDFLAGS) $(COMMON_CFLAGS) -o $@ $^ $(LINK_LIBS) 
	@echo $@ made successfully ...




