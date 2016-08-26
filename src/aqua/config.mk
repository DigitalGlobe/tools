# Variables definitions for all makefiles.


################  User configuration  ################


# This configuration should suits for most GNU based system.


## Compilers

CC  = gcc
CXX = g++


## C and CXX FLAGS

FLAGS      =  -Wextra -Wall -pedantic -pipe -g -O3 #-march=pentium4 -mfpmath=sse  -msse2 -ffast-math -malign-double -mmmx -msse
CFLAGS     = $(FLAGS) -std=ansi
CXXFLAGS   = $(FLAGS) -std=c++98 -Winline -ansi #-Weffc++


## GPROF

# CC       =  gcc
# CXX      =  g++
# LDFLAGS += -pg #-fprofile-arcs -lgcov
# FLAGS   += -pg #-O0 #-fprofile-arcs -ftest-coverage


# GSL
GSL_LDFLAGS = -lgsl -lgslcblas -lm

# FFTW
FFTW_LDFLAGS = -lfftw3f -lm

# Graphic libraries
X_LDFLAGS = -L/usr/X11R6/lib -lglut -lGLU -lGL
# -lXi -lX11 -lm # -lXmu


## System libraries

# If you do not have glut, comment the next line (the demo will not be
# compiled).
HAVE_GLUT = 1

# If your system does not provide getopt(), comment the next line.
HAVE_GETOPT = 1


## DEBUG (usefull for developers only)

# If you want the demo to report OpenGL errors, uncomment the next line.
DEBUG_GL = 1


################  End of user configuration  ################


# “TOP_DIR” must be defined in all Makefiles (before the inclusion of this
# file).


CPP_FILES = $(wildcard *.cpp)
DEP_FILES = $(CPP_FILES:.cpp=.d)
OBJ       = $(CPP_FILES:.cpp=.o)


## Directories

LIB_DIR      = $(TOP_DIR)/lib
SRC_DIR      = $(TOP_DIR)/libaqua
HEADER_DIR   = $(SRC_DIR)/libaqua
AQUA-GEN_DIR = $(TOP_DIR)/aqua-gen
DEMO_DIR     = $(TOP_DIR)/demo
INCLUDE_DIR  = $(TOP_DIR)/include
TOOLS_DIR    = $(TOP_DIR)/tools


## CPPFLAGS

CPPFLAGS += -I$(INCLUDE_DIR)


## getopt.h

GETOPT_H = $(LIB_DIR)/getopt.h


## Libraries
LIBAQUA = $(SRC_DIR)/libaqua.so
