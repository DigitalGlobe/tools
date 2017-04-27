# Copyright (C) 2001 Her Majesty the Queen in Right of Canada.
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

include $(TOPDIR)/config/common.mak

#
# Sub-directories that need to be built
#
subdirs	= ogdi_import ogdi_info

#
# Default target to build everything in all sub-directories
#
all: $(subdirs)

#
# Target to allow individual sub-directories to be built 
# (e.g.  make cgmmod)
#
.PHONY: $(subdirs)
$(subdirs): 
	cd $@; $(MAKE)

#
# Pass specialized targets into the sub-directories
#
.PHONY: $(STANDARD_TARGETS)
$(STANDARD_TARGETS):
	@for i in $(subdirs); do \
	  $(MAKE) --directory $$i $@; \
	done

