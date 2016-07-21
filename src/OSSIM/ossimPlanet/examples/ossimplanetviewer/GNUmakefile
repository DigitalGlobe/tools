TOPDIR = ../..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	ossimplanetviewer.cpp \
	opvLoadModel.cpp

LIBS     += $(OSSIM_LIBS) -lossimPlanet $(OSG_LIBS) $(OTHER_LIBS) 

ifeq ($(OS),Darwin)
	LIBS += -bind_at_load
endif

INSTFILES = \
	$(CXXFILES)\
	GNUmakefile.inst=GNUmakefile
LIB=
EXEC = ossimplanetviewer

include $(TOPDIR)/Make/makerules 

