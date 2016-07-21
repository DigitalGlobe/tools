###
# $Id: Makefile 10406 2007-01-30 21:55:38Z dburken $
###

###
# Must edit OSSIM_HOME and OP_HOME:
###
#OSSIM_HOME=/work/drb/ossim

OP_HOME=$(OSSIM_DEV_HOME)/ossimPredator
FFMPEG_INSTALL=/usr/local

include $(OSSIM_DEV_HOME)/ossim/Makefile.common

OP_INCLUDE_PATH=$(OP_HOME)/include

###
# INSTALL DIRECTORY:
# NOTE you can override at make time like this:
# make prefix=/usr/local
###
# prefix=$(OMS_HOME)
#prefix=/usr/local
prefix=$(OSSIM_RELEASE_HOME)

libdir=$(prefix)/lib
includedir=$(prefix)/include/ossimPredator

###
# Pick up platform specific overrides.
###
# include ../make_platform.opt

BASE_INCLUDES := -I$(OP_INCLUDE_PATH) $(BASE_INCLUDES) -I$(FFMPEG_INSTALL)/include -I$(FFMPEG_INSTALL)/include/ffmpeg -I/usr/local/include
OSSIM_APP_LINK := $(OSSIM_APP_LINK) -L$(FFMPEG_INSTALL)/lib -lavcodec -lavformat -lavdevice -lavutil -lswscale

###
# For pattern rule to make objects.
###
CPPSRCS=$(wildcard *.cpp)
CPPOBJS=$(patsubst %.cpp,%.o,$(CPPSRCS))
OBJS=$(CPPOBJS)

###
# For pattern rule to make .d files.
###
DEPENDS_FILES=$(wildcard *.d)


PLUGIN_LIB=libossimPredator
SHARED_LIB_VERSIONLESS=$(PLUGIN_LIB).$(SO_VERSIONLESS_EXT)
SHARED_LIB_DIR=../lib
SHARED_LIB_TARGET=$(SHARED_LIB_DIR)/$(SHARED_LIB_VERSIONLESS)
SHARED_OBJ_DIR=`pwd`/$(SHARED_OBJ_DIR)
SHARED_OBJECT_FILES=*.$(OBJEXT)
DOT_D_FILES=*.d
INSTALL_LIB_TARGET=$(OSSIM_PLUGINS_INSTAL_LIB_DIR)/$(SHARED_LIB_VERSIONLESS)

# linux stuff #
ifeq ($(HOST_RESULT),linux)
# Override for linux to just do a straight link. 
LD_SHARED=$(CXX) -shared 
endif

# ---
# all target:
# 1) Make all objects.
# 2) link objects to library.
# ---
all:    depends $(OBJS)
	$(LD_SHARED) -o $(SHARED_LIB_TARGET) $(SHARED_OBJECT_FILES) $(OSSIM_APP_LINK)

# ---
# install target:
# ---
install: all
	if test "$(prefix)" != "$(OP_HOME)" ; then \
		$(INSTALL) -d $(libdir); \
		$(INSTALL) -d $(includedir); \
		$(INSTALL) $(SHARED_LIB_TARGET) $(libdir)/$(SHARED_LIB) ; \
		( cd $(OP_INCLUDE_PATH)/ossimPredator; \
			for h in `find . -name "*.h"` ; do \
				$(INSTALL) -m 664 $$h $(includedir)/$$h; \
			done ; \
		) \
	fi

# ---
# clean target:
# ---
clean:
	$(RM_NOPROMPT) $(DOT_D_FILES) $(SHARED_OBJECT_FILES) $(SHARED_LIB_TARGET) *~

# ---
# uninstall target:
# ---
uninstall:
	if test -f "$(INSTALL_LIB_TARGET)" ; then $(RM_NOPROMPT) $(INSTALL_LIB_TARGET) ; fi

# ---
# depends target (for .d files):
# ---
depends: $(CPPSRCS:.cpp=.d)

# ---
# Pattern rule for objects from .cpp files.
# ---
%.$(OBJEXT): %.cpp
	$(CXX) $(CXXFLAGS) $(PIC_FLAG) $(CDEBUGFLAGS) $(BASE_DEFINES) $(BASE_INCLUDES) -c $< -o $@

###
# Pattern rule for .d dependency file from .cpp files.
###
%.d: %.cpp
	$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) $(CDEBUGFLAGS) $(BASE_DEFINES) $(BASE_INCLUDES) $< >> $@;'

ifneq ( $(DEPENDS_FILES),)
-include $(DEPENDS_FILES) 
endif
