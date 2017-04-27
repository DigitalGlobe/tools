TOPDIR = ../..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	wmsurl.cpp\

LIBS     += -lwms $(OTHER_LIBS) 

INSTFILES = \
	$(CXXFILES)\
	GNUmakefile.inst=GNUmakefile

EXEC = wmsurl
LIB=
include $(TOPDIR)/Make/makerules 

