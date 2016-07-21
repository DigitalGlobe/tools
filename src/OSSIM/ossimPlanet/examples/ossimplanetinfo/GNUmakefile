TOPDIR = ../..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	ossimplanetinfo.cpp

LIBS     += $(OSSIM_LIBS) -lossimPlanet -lossimPredator $(OSG_LIBS) $(OTHER_LIBS) 

ifeq ($(OS),Darwin)
	LIBS += -bind_at_load
endif

INSTFILES = \
	$(CXXFILES)\
	GNUmakefile.inst=GNUmakefile
LIB=
EXEC = ossimplanetinfo

include $(TOPDIR)/Make/makerules 

