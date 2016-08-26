TOP_DIR = ../..

## includes

include $(TOP_DIR)/config.mk


HEADER_STRING = "/\*  LIBAQUA HEADER FILE  \*/"

HOST = $(shell hostname)
ifeq ($(HOST),random)
FIND_OPT  = -name
else
FIND_OPT  = -wholename
endif

HEADERS = $(shell find $(SRC_DIR) $(FIND_OPT) "$(HEADER_DIR)" -prune \
                                  -o -name "*.h" | \
                  xargs grep -l $(HEADER_STRING))


## main rules

all:
	for header in $(HEADERS); do ln -sf $$header; done

clean:
	for header in $(shell find -type l); do $(RM) $$header; done


.PHONY: all clean clean_header
