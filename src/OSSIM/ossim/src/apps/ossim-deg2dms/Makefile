PROGRAM=ossim-deg2dms

include ../../../Makefile.common

CPPSRCS=$(wildcard *.cpp)
CPPOBJS=$(patsubst %.cpp,%.o,$(CPPSRCS))

OBJS=$(CPPOBJS)

DEPEND_FILES = $(wildcard *.d)

# The binary program extension, if any, including dots (e.g. '.exe')
PROGRAM_EXT = $(EXEEXT)
BIN_PROGRAM = $(PROGRAM_PREFIX)$(PROGRAM)$(PROGRAM_EXT)

SRCS      = $(CPPSRCS)

# Pattern rule for objects from .cpp files.
%.$(OBJEXT): %.cpp
	$(CXX) $(CXXFLAGS) $(CDEBUGFLAGS) -c $< -o $@

%.d: %.cpp
	$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) $(CDEBUGFLAGS) $< >> $@;'

all:	$(BIN_PROGRAM)

$(BIN_PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CDEBUGFLAGS) $(OBJS) -o $@
	$(INSTALL_PROGRAM) $(BIN_PROGRAM) $(OSSIM_TOP)/bin

depends: 	$(CPPSRCS:.cpp=.d)

clean_depends:
	$(RM_NOPROMPT) *.d

clean:
	$(RM_CMD) $(BIN_PROGRAM)

# Only add the depends if they exist
#
ifneq ($(DEPENDS_FILES),)
-include $(DEPENDS_FILES)
endif
