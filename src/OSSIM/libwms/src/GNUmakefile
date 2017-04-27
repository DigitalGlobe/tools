TOPDIR= ..
include $(TOPDIR)/Make/makedefs 

CXXFILES =\
	iso8601.cpp \
	wms.cpp \
	wmsCapabilitiesParser.cpp \
	wmsCapabilitiesRoot.cpp \
	wmsCapabilitiesState.cpp \
	wmsClient.cpp \
	wmsCurlMemoryStream.cpp \
	wmsMemoryStream.cpp \
	wmsNotify.cpp \
	wmsReferenced.cpp \
	wmsStyle.cpp \
	wmsUrl.cpp 

DEF     += -DWMS_LIBRARY

LIBS    += $(OTHER_LIBS)

TARGET_BASENAME = wms
LIB = $(LIB_PREFIX)$(TARGET_BASENAME).$(LIB_EXT)

include $(TOPDIR)/Make/makerules 
