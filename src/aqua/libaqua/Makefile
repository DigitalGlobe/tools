TOP_DIR = ..

## Includes

include $(TOP_DIR)/config.mk


## Target

TARGET = $(LIBAQUA)


## Subdirectories

DIRS = ocean rng spectrum spectrum_tool

## Subdirectories objects
DIR_CPP = $(shell find -name "*.cpp")
DIR_OBJ = $(DIR_CPP:.cpp=.o)


## CXXFLAGS

CXXFLAGS += -fpic # -fPIC


## LDFLAGS

LDFLAGS += $(GSL_LDFLAGS) $(FFTW_LDFLAGS) -lm -shared # -Wl,-soname,libaqua.so.1


## Main rules

all: $(TARGET)


$(TARGET): $(OBJ) $(DIRS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(DIR_OBJ)

$(DIRS): $(HEADER_DIR)

$(HEADER_DIR):
	$(MAKE) -C $@


## (dirs/dep/objects/clean/phony) rules

include $(TOP_DIR)/config_rules.mk

clean::
	$(MAKE) -C $(HEADER_DIR) clean

.PHONY: $(SRC_DIR)/libaqua
