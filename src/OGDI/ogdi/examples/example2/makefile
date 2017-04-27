TOBEGEN	= example2
TARGETGEN=$(PROGGEN)

SOURCES = example2.c 

INCLUDES = $(CURRENT_INCLUDE) $(GENERAL_INCLUDE) $(OGDI_INCLUDE) $(PROJ_INCLUDE)

CFLAGS 	= $(INCLUDES) $(COMMON_CFLAGS)

LINK_LIBS= $(OGDI_LINKLIB)  $(PROJ_STATICLINKLIB)


include $(TOPDIR)/config/common.mak


all: MKOBJECTDIR
	$(MAKE) --directory $(OBJDIR) -f ../makefile PASS='depend' $(TARGETGEN)
	$(COPY_LOCAL)

clean: default-clean
