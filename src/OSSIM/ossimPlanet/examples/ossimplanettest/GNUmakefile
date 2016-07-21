TOPDIR = ../..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	ossimplanettest.cpp 

LIBS     += $(OSSIM_LIBS) -lossimPlanet $(OSG_LIBS) $(OTHER_LIBS) 

ifeq ($(OS),Darwin)
	LIBS += -bind_at_load
endif

INSTFILES = \
	$(CXXFILES)\
	GNUmakefile.inst=GNUmakefile
LIB=
EXEC = ossimplanettest

include $(TOPDIR)/Make/makerules 

