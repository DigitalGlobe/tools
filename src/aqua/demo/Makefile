TOP_DIR = ..

## includes

include $(TOP_DIR)/config.mk


## target

TARGET = aqua-demo


## subdirectories

DIRS = fft gerstner
DIR_CPP = $(shell for n in $(DIRS); do echo $$n/*.cpp; done)
DIR_OBJ = $(DIR_CPP:.cpp=.o)

LIB_CPP = $(wildcard $(LIB_DIR)/*.cpp)
LIB_OBJ = $(LIB_CPP:.cpp=.o)

LIBAQUA_CPP = $(wildcard $(SRC_DIR)/*.cpp)
LIBAQUA_CPP += $(wildcard $(SRC_DIR)/ocean/*.cpp)
LIBAQUA_CPP += $(wildcard $(SRC_DIR)/ocean/fft/*.cpp)
LIBAQUA_CPP += $(wildcard $(SRC_DIR)/ocean/gerstner/*.cpp)
LIBAQUA_CPP += $(wildcard $(SRC_DIR)/ocean/gerstner/classic/*.cpp)
LIBAQUA_CPP += $(wildcard $(SRC_DIR)/rng/*.cpp)
LIBAQUA_CPP += $(wildcard $(SRC_DIR)/spectrum/*.cpp)
LIBAQUA_CPP += $(wildcard $(SRC_DIR)/spectrum_tool/*.cpp)
LIBAQUA_OBJ = $(LIBAQUA_CPP:.cpp=.o)


## CPPFLAGS
# need DEMO_DIR for config
CPPFLAGS += -I$(DEMO_DIR) -I$(LIB_DIR) -I$(SRC_DIR)

ifdef DEBUG_GL
CPPFLAGS += -DDEBUG_GL
endif  # DEBUG_GL


## LDFLAGS

LDFLAGS += $(X_LDFLAGS) -lm


## main rules

all: $(TARGET)

$(TARGET): $(OBJ) $(LIB_OBJ) $(DIRS) $(LIBAQUA)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJ) $(LIB_OBJ) $(DIR_OBJ) $(LIBAQUA_OBJ) $(GSL_LDFLAGS) $(FFTW_LDFLAGS)
#	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJ) $(LIB_OBJ) $(DIR_OBJ) $(LIBAQUA)


## (dirs/dep/objects/clean/phony) rules

include $(TOP_DIR)/config_rules.mk
