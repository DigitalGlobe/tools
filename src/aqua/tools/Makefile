TOP_DIR = ..

## includes

include $(TOP_DIR)/config.mk


## target

TARGET = aqua_integrate_spectrum aqua_compute aqua_fft cartesian2polar \
         find_boundaries quantify


## CPPFLAGS

CPPFLAGS += -I$(SRC_DIR) -I$(LIB_DIR)


## LDFLAGS

LDFLAGS += -lm

####
LOCAL_LIB =  $(LIB_DIR)/aqua_lib_exceptions.o $(LIB_DIR)/aqua_lib_utilities.o
####


## main rules

all: $(TARGET)

aqua_fft: aqua_fft.o $(LOCAL_LIB)
	$(CXX) $(LDFLAGS) $< $(LOCAL_LIB) -o $@

aqua_compute: aqua_compute.o $(LIBAQUA) $(LOCAL_LIB)
	$(CXX) $(LDFLAGS) $< $(LIBAQUA) $(LOCAL_LIB) -o $@

aqua_integrate_spectrum: aqua_integrate_spectrum.o $(LIBAQUA) $(LOCAL_LIB)
	$(CXX) $(LDFLAGS) $< $(LIBAQUA) $(LOCAL_LIB) -o $@

cartesian2polar: cartesian2polar.o
	$(CXX) $(LDFLAGS) $< -o $@

find_boundaries: find_boundaries.o $(LIBAQUA) $(LOCAL_LIB)
	$(CXX) $(LDFLAGS) $< $(LIBAQUA) $(LOCAL_LIB) -o $@

quantify: quantify.o $(LIBAQUA) $(LOCAL_LIB)
	$(CXX) $(LDFLAGS) $< $(LIBAQUA) $(LOCAL_LIB) -o $@


## (dirs/dep/objects/clean/phony) rules

include $(TOP_DIR)/config_rules.mk
