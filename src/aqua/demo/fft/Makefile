TOP_DIR = ../..

## includes

include $(TOP_DIR)/config.mk


## CPPFLAGS

CPPFLAGS += -I$(DEMO_DIR) -I$(LIB_DIR) -I$(SRC_DIR)

ifdef DEBUG_GL
CPPFLAGS += -DDEBUG_GL
endif  # DEBUG_GL


## main rules

all: $(OBJ)


## (dirs/dep/objects/clean/phony) rules

include $(TOP_DIR)/config_rules.mk
