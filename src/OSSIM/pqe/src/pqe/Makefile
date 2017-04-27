####
# $Id: Makefile 11658 2007-08-24 18:46:29Z dburken $
####

###
# Dependancy files:
###
DEPENDS_FILES=$(wildcard *.d)

###
# Files to compile:
###
CPPSRCS=$(wildcard *.cpp)
CPPOBJS=$(patsubst %.cpp,%.o,$(CPPSRCS))
OBJS=$(CPPOBJS) $(COBJS)

###
# Compiler:
###
CXX=g++

###
# Compiler flags:
###
CXXFLAGS=-Wall -pipe -fPIC

###
# Compiler debug flags.  Note kept separate so you can override like:
# make CXXDEBUGFLAGS-g
###
CXXDEBUGFLAGS=-O2

#CXXDEFINES=-DPQEMAKINGDLL
CXXDEFINES=

CXXINCLUDES=-I../../include

###
# For make install:
###
prefix=/usr/local
INSTALL=/usr/bin/install

RM=/bin/rm

###
# The library name:
###
SHARED_LIBRARY_BASENAME=libpqe.so
SHARED_LIBRARY=../../lib/$(SHARED_LIBRARY_BASENAME)

###
# Flags for ld:
###
LDFLAGS=-shared


all: depends $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(SHARED_LIBRARY)

clean:
	$(RM) -f $(SHARED_LIBRARY) *.o *.d *~

depends: 	$(CPPSRCS:.cpp=.d)

install: all
	$(INSTALL) -d $(prefix)
	$(INSTALL) -d $(prefix)/include/pqe
	$(INSTALL) -d $(prefix)/lib
	$(INSTALL) -m 755 $(SHARED_LIBRARY) $(prefix)/lib
	$(INSTALL) -m 644 ../../include/pqe/*.h $(prefix)/include/pqe

uninstall:
	$(RM) -rf $(prefix)/include/pqe
	$(RM) -f $(prefix)/lib/$(SHARED_LIBRARY_BASENAME)

###
# Pattern rule for objects from .cpp files.
###
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) $(CXXDEFINES) $(CXXINCLUDES) -c $< -o $@

###
# Pattern rule for .d dependency file from .cpp files.
###
%.d: %.cpp
	$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) $(CXXDEBUGFLAGS) $(CXXDEFINES) $(CXXINCLUDES) $< >> $@;'

ifneq ( $(DEPENDS_FILES),)
-include $(DEPENDS_FILES) 
endif
