TOP_DIR = ..

## includes

include $(TOP_DIR)/config.mk


## objects

ifndef HAVE_GETOPT
OBJ += getopt.o
endif  # HAVE_GETOPT


## objects which need "getopt.h"

ifndef HAVE_GETOPT
OBJ_NEED_GETOPT_H = options.o
endif  # HAVE_GETOPT


## CPPFLAGS

# if system doesn't provide "getopt.h", use local files
ifndef HAVE_GETOPT
CPPFLAGS += -I.
endif  # HAVE_GETOPT

# for "callback.cpp"
ifdef DEBUG_GL
CPPFLAGS += -DDEBUG_GL
endif  # DEBUG_GL


## main rules

all: $(OBJ)


## getopt.o

# compile getopt.o and check for getopt.h only if we use them
ifndef HAVE_GETOPT
getopt.o: getopt.c getopt.h
$(OBJ_NEED_GETOPT_H): getopt.h
else
getopt.o: ; # ?
endif  # HAVE_GETOPT


## (dirs/dep/objects/clean/phony) rules

include $(TOP_DIR)/config_rules.mk
