TOPDIR= ../..
include $(TOPDIR)/Make/makedefs 

CXXFILES = \
iochannel.cpp \
mkUtils.cpp\
netBuffer.cpp\
netChannel.cpp\
netChat.cpp\
netMessage.cpp\
netMonitor.cpp\
netSocket.cpp\
ossimPlanet.cpp\
ossimPlanetAction.cpp\
ossimPlanetActionReceiver.cpp\
ossimPlanetActionRouter.cpp\
ossimPlanetAnimationPath.cpp \
ossimPlanetAnimatedPointModel.cpp \
ossimPlanetAnnotationLayer.cpp\
ossimPlanetAnnotationLayerNode.cpp\
ossimPlanetApi.cpp\
ossimPlanetArchive.cpp\
ossimPlanetArchiveMapping.cpp\
ossimPlanetBillboardIcon.cpp\
ossimPlanetBoundingBox.cpp \
ossimPlanetCache.cpp \
ossimPlanetCacheTextureLayer.cpp \
ossimPlanetClientThread.cpp \
ossimPlanetCloudLayer.cpp \
ossimPlanetCompass.cpp \
ossimPlanetCubeGrid.cpp \
ossimPlanetDatabasePager.cpp \
ossimPlanetDestinationCommandAction.cpp \
ossimPlanetDtedElevationDatabase.cpp \
ossimPlanetElevationDatabase.cpp \
ossimPlanetElevationDatabaseGroup.cpp \
ossimPlanetElevationGrid.cpp \
ossimPlanetElevationFactory.cpp \
ossimPlanetElevationRegistry.cpp \
ossimPlanetEphemeris.cpp \
ossimPlanetFadeText.cpp \
ossimPlanetGeneralRasterElevationDatabase.cpp \
ossimPlanetGeocoder.cpp \
ossimPlanetGridUtility.cpp \
ossimPlanetGrid.cpp \
ossimPlanetIconGeom.cpp \
ossimPlanetId.cpp \
ossimPlanetIdManager.cpp \
ossimPlanetImage.cpp \
ossimPlanetInteractionController.cpp \
ossimPlanetIoRoutableMessageHandler.cpp \
ossimPlanetIoSocket.cpp \
ossimPlanetIoSocketServerChannel.cpp \
ossimPlanetIoThread.cpp \
ossimPlanetJpegImage.cpp \
ossimPlanetKml.cpp \
ossimPlanetKmlLayer.cpp \
ossimPlanetKmlLayerNode.cpp \
ossimPlanetKmlNetworkLinkNode.cpp \
ossimPlanetKmlPlacemarkNode.cpp \
ossimPlanetKmlScreenOverlayNode.cpp \
ossimPlanetLabelGeom.cpp \
ossimPlanetLand.cpp \
ossimPlanetLandCache.cpp \
ossimPlanetLandCullCallback.cpp \
ossimPlanetLandReaderWriter.cpp \
ossimPlanetLatLonHud.cpp \
ossimPlanetLayer.cpp \
ossimPlanetLayerFactory.cpp \
ossimPlanetLayerRegistry.cpp \
ossimPlanetLookAt.cpp \
ossimPlanetLsrSpaceTransform.cpp \
ossimPlanetManipulator.cpp \
ossimPlanetNavigator.cpp \
ossimPlanetNode.cpp \
ossimPlanetNodeFactory.cpp \
ossimPlanetNodeRegistry.cpp \
ossimPlanetOperation.cpp \
ossimPlanetOssimElevationDatabase.cpp \
ossimPlanetOssimImage.cpp \
ossimPlanetOssimImageLayer.cpp \
ossimPlanetPagedLandLod.cpp \
ossimPlanetPlaneGrid.cpp \
ossimPlanetPointModel.cpp \
ossimPlanetViewer.cpp\
ossimPlanetSceneView.cpp \
ossimPlanetServerThread.cpp \
ossimPlanetShaderProgramSetup.cpp \
ossimPlanetSocketNetworkConnection.cpp \
ossimPlanetSousaLayer.cpp \
ossimPlanetSrtmElevationDatabase.cpp \
ossimPlanetStandardTextureLayerFactory.cpp \
ossimPlanetTerrain.cpp \
ossimPlanetTerrainGeometryTechnique.cpp \
ossimPlanetTerrainLayer.cpp \
ossimPlanetTerrainTechnique.cpp \
ossimPlanetTerrainTile.cpp \
ossimPlanetTexture2D.cpp \
ossimPlanetTextureLayer.cpp \
ossimPlanetTextureLayerGroup.cpp \
ossimPlanetTextureLayerRegistry.cpp \
ossimPlanetThread.cpp \
ossimPlanetThreadImp.cpp \
ossimPlanetThreadPool.cpp \
ossimPlanetTileRequest.cpp \
ossimPlanetUtility.cpp \
ossimPlanetVideoLayer.cpp \
ossimPlanetViewMatrixBuilder.cpp \
ossimPlanetVisitors.cpp \
ossimPlanetWmsClient.cpp \
ossimPlanetWmsImageLayer.cpp \
ossimPlanetXmlAction.cpp \
ossimPlanetYahooGeocoder.cpp \
sg_file.cpp \
sg_socket.cpp \
sg_socket_udp.cpp \
ul.cpp \
ulClock.cpp \
ulError.cpp \
ulLinkedList.cpp \
ulList.cpp \
ulRTTI.cpp
 
CFILES = \
ioapi.c \
unzip.c \
zip.c

ifneq ("$(OSSIMPLANET_ENABLE_PREDATOR)", "no")
CXXFILES+=ossimPlanetPredatorVideoLayerNode.cpp
endif

ifeq ($(OS),MINGW)
CFILES += iowin32.c
endif

DEF     += -DOSSIMPLANET_LIBRARY

LIBS    +=  $(OSG_LIBS) $(OSSIM_LIBS) $(OTHER_LIBS)

TARGET_BASENAME = ossimPlanet
LIB = $(LIB_PREFIX)$(TARGET_BASENAME).$(LIB_EXT)

include $(TOPDIR)/Make/makerules 
