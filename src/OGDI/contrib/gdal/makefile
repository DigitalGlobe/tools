
TOBEGEN	= gdal
TARGETGEN=$(DYNAGEN)

SOURCES =  \
	gdal_serv.c \
	\
	gdalbridge.c \
	gbgetsymbol.c


INCLUDES = $(CURRENT_INCLUDE) $(GENERAL_INCLUDE) $(OGDI_INCLUDE) \
	$(PROJ_INCLUDE)
CFLAGS 	= $(INCLUDES) $(COMMON_CFLAGS)

LINK_LIBS= $(OGDI_LINKLIB) $(GUTIL_LINKLIB)

include $(TOPDIR)/config/common.mak

all:  MKOBJECTDIR
	$(MAKE) --directory $(OBJDIR) -f ../makefile PASS='depend' $(TARGETGEN)

clean: default-clean
