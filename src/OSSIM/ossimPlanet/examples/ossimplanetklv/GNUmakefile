TOPDIR = ../..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	ossimplanetpredator.cpp

LIBS     += $(OSSIM_LIBS) -lossimPlanet -lossimPredator $(OSG_LIBS) $(OTHER_LIBS) 

ifeq ($(OS),Darwin)
	LIBS += -bind_at_load
endif

INSTFILES = \
	$(CXXFILES)\
	GNUmakefile.inst=GNUmakefile
LIB=
EXEC = ossimplanetpredator

include $(TOPDIR)/Make/makerules 

