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

EXTRA_INSTALL_TARGETS	=	mk_nonlocal_install_dirs

ifeq ($(TARGET),win32)
IGNORE := $(shell sed "s/@OGDI_MAJOR@/3/" < $(TOPDIR)/config/common.mak.in | sed "s/@OGDI_MINOR@/2/" > $(TOPDIR)/config/common.mak)
endif
include $(TOPDIR)/config/common.mak
 
#
# Sub-directories that need to be built
#
subdirs	= external vpflib ogdi contrib

ifneq ($(PROJ_SETTING),external)
subdirs := proj $(subdirs) 
endif

#
# Default target to build everything in all sub-directories
#
all: $(subdirs) 


#
# Target to allow individual sub-directories to be built 
# (e.g.  make cmpts)
#
.PHONY: $(subdirs)
$(subdirs): mkinstalldirs
	cd $@; $(MAKE)

#
# Make the (local) installation directories
#
.PHONY: mkinstalldirs
mkinstalldirs:
	@echo making install dirs using $(MKINSTALLDIR)
	$(MKINSTALLDIR) $(TOPDIR)/lib/$(TARGET)/static
	$(MKINSTALLDIR) $(TOPDIR)/bin/$(TARGET)

#
# Pass specialized targets into the sub-directories
#
.PHONY: $(STANDARD_TARGETS)
$(STANDARD_TARGETS):
	@for i in $(subdirs); do \
	  $(MAKE) --directory $$i $@; \
	done


mk_nonlocal_install_dirs:
	$(MKINSTALLDIR) $(prefix) $(exec_prefix) $(INST_LIB) $(INST_BIN) $(INST_INCLUDE)

