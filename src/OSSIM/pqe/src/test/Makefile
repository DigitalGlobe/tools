###
# Test GNU Makefile.
# $Id$
###

# List of c++ files.  Each of these will be treated as a "main" app.
CPPSRCS=$(wildcard *.cpp)

# Strip off the .cpp
PROGRAMS=$(patsubst %.cpp,%,$(CPPSRCS))

# replace .cpp with .o
OBJS=$(patsubst %.cpp,%.o,$(CPPSRCS))

RM=/bin/rm

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

# CXXDEFINES=-DPQEUSINGDLL
CXXDEFINES=

CXXINCLUDES=-I../../include

all: $(OBJS) $(PROGRAMS)

# $(PROGRAMS): $(OBJS)

clean:
	$(RM) -f $(PROGRAMS) *.o *~ *.d

depends: $(CPPSRCS:.cpp=.d)

# Pattern rule for objects from .cpp files.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) $(CXXINCLUDES) $(CXXDEFINES) -c $< -o $@

%: %.o
	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) $(CXXINCLUDES) $(CXXDEFINES) $@.o ../../lib/libpqe.so -o $@

# Pattern rule for depends ".d" from ".cpp" file.
%.d: %.cpp
	$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) $(CXXDEBUGFLAGS) $(CXXINCLUDES) $(CXXDEFINES) $< > $@;'
