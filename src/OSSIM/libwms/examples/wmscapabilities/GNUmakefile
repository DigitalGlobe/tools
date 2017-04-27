TOPDIR = ../..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	wmscapabilities.cpp\

LIBS     += -lwms $(OTHER_LIBS) 

INSTFILES = \
	$(CXXFILES)\
	GNUmakefile.inst=GNUmakefile

EXEC = wmscapabilities

INC +=  $(X_INC) 
LIB=
include $(TOPDIR)/Make/makerules 

