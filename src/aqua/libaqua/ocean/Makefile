TOP_DIR = ../..

## includes

include $(TOP_DIR)/config.mk


## Subdirectories

DIRS = fft gerstner


## CPPFLAGS

CPPFLAGS += -I$(SRC_DIR)


## CXXFLAGS

CXXFLAGS += -fpic


## main rules

all: $(OBJ) $(DIRS)


## (dirs/dep/objects/clean/phony) rules

include $(TOP_DIR)/config_rules.mk
