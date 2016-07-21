/*
*/

%module pyossim

/* This tells SWIG to treat char ** as a special case */


%typemap (in) void*
{
    $1 = PyCObject_AsVoidPtr($input);
}


%typemap(out) ossim_uint8* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}
/*
%apply char **STRING_ARRAY { char* argv[] };
%apply short[] {ossim_sint16 *};
%apply int[] {ossim_sint32 *};
%apply int[] {ossim_int32 *};
%apply int[] {int *};
%apply unsigned int[] {ossim_uint32 *};
%apply unsigned short[] {ossim_uint16 *};
%apply float[] {ossim_float32 *};
%apply double[] {ossim_float64 *};
%apply unsigned char[]  { ossim_uint8 * }
%apply char* BYTE  { ossim_int8 * }


typedef char                   ossim_int8;
typedef unsigned char          ossim_uint8;
typedef signed char            ossim_sint8;

typedef short                  ossim_int16;
typedef unsigned short         ossim_uint16;
typedef signed short           ossim_sint16;

typedef int                    ossim_int32;
typedef unsigned int           ossim_uint32;
typedef signed int             ossim_sint32;

typedef float                  ossim_float32;
typedef double                 ossim_float64;


typedef long long              ossim_int64;
typedef unsigned long long     ossim_uint64;
typedef signed long long       ossim_sint64;

*/

%typemap(out) ossim_int8* {
  $result = PyString_FromString($1);
}

%typemap(out) ossim_uint8* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
  
  //$result = PyString_FromFormat("%s",$1);
}


%typemap(out) ossim_int16* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

%typemap(out) ossim_uint16* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

%typemap(out) ossim_sint16* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}



%typemap(out) ossim_int32* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

%typemap(out) ossim_uint32* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

%typemap(out) ossim_sint32* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

/*



%typemap(out) ossim_float32* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}


%typemap(out) ossim_float64* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}


*/










%typemap(in) (int&, char **) 
{
        /* Check if is a list */
        if (PyList_Check($input)) 
        {
                $1 = PyList_Size($input);
        
                int i = 0;
                $2 = (char **) malloc(($1+1)*sizeof(char *));
                
                for (i = 0; i < $1; i++) 
                {
                        PyObject *o = PyList_GetItem($input,i);
                
                        if (PyString_Check(o))
                                $2[i] = PyString_AsString(PyList_GetItem($input,i));
                        
                        else 
                        {
                                PyErr_SetString(PyExc_TypeError,"list must contain strings");
                                free($2);
                                return NULL;
                        }
                }
                $2[i] = 0;
        } 
        
        else 
        {
                PyErr_SetString(PyExc_TypeError,"not a list");
                return NULL;
        }
}

/* This cleans up the char ** array we malloc'd before the function call */
%typemap(freearg) (int&, char **)
{
        free((char *) $2);
}

%rename(__set__) *::operator=;
%rename(__getitem__) *::operator[];
/* Importing Init Interfaces */
%include "init/ossimInit.i"



/* Importing Base Interfaces */
%include "base/ossimEnums.i"
%include "base/ossimReferenced.i"
%include "base/ossimObject.i"
%include "base/ossimObjectFactory.i"
%include "base/ossimFactoryListInterface.i"
%include "base/ossimProcessInterface.i"
%include "imaging/ossimImageHandlerRegistry.i"

%include "imaging/ossimNumPy.i"
%include "base/ossimRefPtr.i"
%include "base/ossim2dTo2dTransform.i"
%include "base/ossim2dBilinearTransform.i"
%include "base/ossim2dLinearRegression.i"
%include "base/ossim2dTo2dIdentityTransform.i"
%include "base/ossim2dTo2dShiftTransform.i"
%include "base/ossim2dTo2dTransformFactoryBase.i"
%include "base/ossim2dTo2dTransformFactory.i"
%include "base/ossim2dTo2dTransformRegistry.i"

%include "base/ossimAdjustableParameterInfo.i"
%include "base/ossimAdjustableParameterInterface.i"

%include "base/ossimBaseObjectFactory.i"



%include "base/ossimProperty.i"
%include "base/ossimPropertyInterfaceFactory.i"
%include "base/ossimPropertyInterface.i"
%include "base/ossimPropertyInterfaceRegistry.i"

%include "base/ossimListener.i"
%include "base/ossimListenerManager.i"

%include "base/ossimConnectableObject.i"
%include "base/ossimConnectableContainerInterface.i"
%include "base/ossimConnectableObjectListener.i"
%include "base/ossimConnectableContainer.i"
%include "base/ossimConnectableDisplayListener.i"

%include "base/ossimEvent.i"

%include "base/ossimContainerEvent.i"
%include "base/ossimContainerProperty.i"
/*
%include "base/ossimCsvFile.i"
*/
%include "base/ossimCustomEditorWindowFactoryBase.i"
%include "base/ossimCustomEditorWindow.i"
%include "base/ossimCustomEditorWindowRegistry.i"

%include "base/ossimDateProperty.i"
%include "base/ossimDatumFactoryInterface.i"
%include "base/ossimDatumFactory.i"
%include "base/ossimDatumFactoryRegistry.i"
%include "base/ossimDatum.i"

/* Commented as per KewCorp request. Need to test. drb - 01 June 2015
%include "base/ossimDirectoryData.i"
*/

%include "base/ossimDirectory.i"
%include "base/ossimDisplayEventListener.i"
%include "base/ossimDisplayInterface.i"
%include "base/ossimDisplayListEvent.i"
%include "base/ossimDisplayRefreshEvent.i"

%include "base/ossimDpt3d.i"
%include "base/ossimDpt.i"
%include "base/ossimEbcdicToAscii.i"

%include "base/ossimElevationManagerEvent.i"
%include "base/ossimElevationManagerEventListener.i"
%include "base/ossimEllipsoidFactory.i"
%include "base/ossimEllipsoid.i"

%include "base/ossimEnvironmentUtility.i"
%include "base/ossimEpsgDatumFactory.i"
%include "base/ossimEquTokenizer.i"
%include "base/ossimErrorCodes.i"
%include "base/ossimErrorStatusInterface.i"


%include "base/ossimException.i"

%include "base/ossimEcefPoint.i"
%include "base/ossimEcefVector.i"

%include "base/ossimString.i"
%include "base/ossimStringListProperty.i"
%include "base/ossimStringProperty.i"

%include "base/ossimFilename.i"
%include "base/ossimFilenameProperty.i"
%include "base/ossimFontInformation.i"
%include "base/ossimFontProperty.i"

%include "base/ossimGeocent.i"
%include "base/ossimGeoid.i"
%include "base/ossimGeoidEgm96.i"
%include "base/ossimGeoidManager.i"
%include "base/ossimGeoidNgsHeader.i"
%include "base/ossimGeoidNgs.i"
%include "base/ossimGeoPolygon.i"
%include "base/ossimGeoref.i"
%include "base/ossimLookUpTable.i"
%include "base/ossimGeoTiffCoordTransformsLut.i"

%include "base/ossimGeoTiffDatumLut.i"

%include "base/ossimGpt.i"

%include "base/ossimIpt.i"
%include "base/ossimIrect.i"
%include "base/ossimDrect.i"
%include "base/ossimLine.i"

%include "base/ossimLsrPoint.i"
%include "base/ossimLsrRay.i"
%include "base/ossimLsrSpace.i"
%include "base/ossimLsrVector.i"

%include "base/ossimThreeParamDatum.i"

%include "base/ossimNadconGridDatum.i"
%include "base/ossimNadconGridFile.i"
%include "base/ossimNadconGridHeader.i"
%include "base/ossimNadconNarDatum.i"
%include "base/ossimNadconNasDatum.i"
%include "base/ossimObjectDestructingEvent.i"

%include "base/ossimStreamBase.i"
%include "base/ossimStreamFactoryBase.i"
%include "base/ossimStreamFactory.i"
%include "base/ossimStreamFactoryRegistry.i"

%include "base/ossimTieGpt.i"
%include "base/ossimTieGptSet.i"

%include "base/ossimPointHash.i"

%include "base/ossimTiledImageHash.i"
%include "base/ossimTileHash.i"
%include "base/ossimTimer.i"
%include "base/ossimTrace.i"
%include "base/ossimTraceManager.i"

%include "base/ossimViewController.i"
%include "base/ossimViewEvent.i"

%include "base/ossimViewListener.i"

%include "base/ossimTDpt.i"
%include "base/ossimTempFilename.i"
%include "base/ossimTextProperty.i"
%include "base/ossimThinPlateSpline.i"


/* Importing Imaging Interfaces */

%include "imaging/ossimAnnotationObject.i"

%include "imaging/ossimGeoAnnotationObject.i"

%include "imaging/ossimAnnotationEllipseObject.i"
%include "imaging/ossimAnnotationFontObject.i"
%include "imaging/ossimAnnotationLineObject.i"
%include "imaging/ossimAnnotationMultiEllipseObject.i"
%include "imaging/ossimAnnotationMultiLineObject.i"
%include "imaging/ossimAnnotationMultiPolyLineObject.i"
%include "imaging/ossimAnnotationMultiPolyObject.i"
%include "imaging/ossimAnnotationObjectFactory.i"

%include "imaging/ossimAnnotationPolyObject.i"
%include "imaging/ossimAnnotationSource.i"
%include "imaging/ossimAdrgHeader.i"
%include "base/ossimSource.i"
%include "base/ossimOutputSource.i"
%include "base/ossimViewInterface.i"
%include "imaging/ossimImageSource.i"
%include "imaging/ossimImageCombiner.i"
%include "imaging/ossimBandMergeSource.i"
%include "imaging/ossimMemoryImageSource.i"
%include "elevation/ossimElevSource.i"

%include "elevation/ossimElevationDatabaseFactoryBase.i"
%include "elevation/ossimElevationDatabaseFactory.i"
%include "elevation/ossimElevationDatabase.i"
%include "elevation/ossimElevationDatabaseRegistry.i"
%include "elevation/ossimElevCellHandlerFactory.i"
%include "elevation/ossimElevCellHandler.i"
%include "elevation/ossimElevManager.i"
%include "elevation/ossimElevSourceFactory.i"

%include "elevation/ossimDtedElevationDatabase.i"
%include "elevation/ossimDtedFactory.i"
%include "elevation/ossimDtedHandler.i"

%include "elevation/ossimGeneralRasterElevationDatabase.i"
%include "elevation/ossimGeneralRasterElevFactory.i"
%include "elevation/ossimGeneralRasterElevHandler.i"
%include "elevation/ossimImageElevationDatabase.i"
%include "elevation/ossimImageElevationHandler.i"
%include "elevation/ossimSrtmElevationDatabase.i"
%include "elevation/ossimSrtmFactory.i"
%include "elevation/ossimSrtmHandler.i"

%include "imaging/ossimBandAverageFilter.i"
%include "imaging/ossimBandClipFilter.i"


%include "imaging/ossimBandSelector.i"

%include "imaging/ossimBitMaskTileSource.i"
%include "imaging/ossimBitMaskWriter.i"
%include "imaging/ossimBlendMosaic.i"
%include "imaging/ossimConvolutionFilter1D.i"
%include "imaging/ossimConvolutionSource.i"
%include "imaging/ossimEdgeFilter.i"
%include "imaging/ossimFilter.i"
%include "imaging/ossimFilterResampler.i"
%include "imaging/ossimFilterTable.i"
%include "base/ossimKeyword.i"
%include "base/ossimKeywordlist.i"
%include "imaging/ossimGeneralRasterInfo.i"
%include "imaging/ossimGeoAnnotationBitmap.i"
%include "imaging/ossimGeoAnnotationEllipseObject.i"
%include "imaging/ossimGeoAnnotationFontObject.i"
%include "imaging/ossimGeoAnnotationGdBitmapFont.i"
%include "imaging/ossimGeoAnnotationLineObject.i"
%include "imaging/ossimGeoAnnotationMultiEllipseObject.i"
%include "imaging/ossimGeoAnnotationMultiPolyLineObject.i"
%include "imaging/ossimGeoAnnotationMultiPolyObject.i"

%include "imaging/ossimGeoAnnotationPolyLineObject.i"
%include "imaging/ossimGeoAnnotationPolyObject.i"
%include "imaging/ossimGeoAnnotationSource.i"

%include "imaging/ossimGridRemapEngineFactory.i"
%include "imaging/ossimGridRemapEngine.i"
%include "imaging/ossimGridRemapSource.i"
%include "imaging/ossimHistogramEqualization.i"
%include "imaging/ossimHistogramMatchFilter.i"


%include "imaging/ossimImageSourceFilter.i"
%include "imaging/ossimImageSourceHistogramFilter.i"

%include "imaging/ossimHistogramRemapper.i"
%include "imaging/ossimHistogramThreshholdFilter.i"
%include "imaging/ossimHistogramWriter.i"
%include "imaging/ossimHistoMatchRemapper.i"
%include "imaging/ossimHsiRemapper.i"
%include "imaging/ossimHsiToRgbSource.i"
%include "imaging/ossimHsvGridRemapEngine.i"
%include "imaging/ossimHsvToRgbSource.i"
%feature("notabstract") ossimSingleImageChain;
%feature("notabstract") ossimImageChain;
%include "imaging/ossimIgenGenerator.i"
%include "imaging/ossimImageCacheBase.i"
%include "imaging/ossimImageCacheTileSource.i"
%include "imaging/ossimImageChain.i"
%include "imaging/ossimSingleImageChain.i"
%include "imaging/ossimImageRenderer.i"
%include "imaging/ossimImageDataHelper.i"


%include "base/ossimDataObject.i"
%include "base/ossimRectilinearDataObject.i"
%include "imaging/ossimImageData.i"

%include "imaging/ossimImageGaussianFilter.i"
%include "imaging/ossimImageGeometryFactoryBase.i"
%include "imaging/ossimImageGeometryFactory.i"
%include "imaging/ossimImageGeometry.i"
%include "imaging/ossimImageGeometryRegistry.i"

%include "imaging/ossimImageHandlerFactoryBase.i"
%include "imaging/ossimImageHandlerFactory.i"
%include "imaging/ossimImageHandler.i"

%include "imaging/ossimImageMetaData.i"
%include "imaging/ossimImageMetaDataWriterFactoryBase.i"
%include "imaging/ossimImageMetaDataWriterFactory.i"
%include "imaging/ossimImageMetaDataWriterRegistry.i"
%include "imaging/ossimImageModel.i"
%include "imaging/ossimImageMosaic.i"
%include "imaging/ossimImageSharpenFilter.i"
%include "imaging/ossimImageSourceFactoryBase.i"
%include "imaging/ossimImageSourceFactory.i"
%include "imaging/ossimImageSourceFactoryRegistry.i"
%include "imaging/ossimImageSourceSequencer.i"
%include "imaging/ossimImageStatisticsSource.i"
%include "imaging/ossimImageToPlaneNormalFilter.i"
%include "imaging/ossimImageWriter.i"
%include "imaging/ossimImageWriterFactoryBase.i"
%include "imaging/ossimImageWriterFactory.i"
%include "imaging/ossimImageWriterFactoryRegistry.i"
%include "imaging/ossimImageFileWriter.i"
%include "imaging/ossimJpegTileSource.i"
%include "imaging/ossimJpegWriter.i"
%include "imaging/ossimJpegYCbCrToRgbSource.i"

%include "imaging/ossimTiffTileSource.i"
%include "imaging/ossimTiffWriter.i"
/*
%include "imaging/ossimMaskedImageHandler.i"
%include "imaging/ossimMaskFilter.i"
%include "imaging/ossimMaskTileSource.i"
*/
%include "imaging/ossimMaxMosaic.i"
%include "imaging/ossimMetadataFileWriter.i"

%include "imaging/ossimNormalizedRemapTable.i"
%include "imaging/ossimNormalizedS16RemapTable.i"
%include "imaging/ossimNormalizedU11RemapTable.i"
%include "imaging/ossimNormalizedU16RemapTable.i"
%include "imaging/ossimNormalizedU8RemapTable.i"
%include "imaging/ossimOverviewBuilderBase.i"
%include "imaging/ossimOverviewBuilderFactoryBase.i"
%include "imaging/ossimOverviewBuilderFactory.i"
%include "imaging/ossimOverviewBuilderFactoryRegistry.i"
%include "imaging/ossimOverviewSequencer.i"
%include "imaging/ossimPixelFlipper.i"
%include "imaging/ossimPolyCutter.i"
%include "imaging/ossimQuickbirdNitfTileSource.i"
%include "imaging/ossimQuickbirdTiffTileSource.i"
%include "imaging/ossimRectangleCutFilter.i"
%include "imaging/ossimRgbGridRemapEngine.i"
/*%include "imaging/ossimRgbImage.i"*/
%include "imaging/ossimTiffTileSource.i"
%include "imaging/ossimRgbToGreyFilter.i"
%include "imaging/ossimRgbToHsiSource.i"
%include "imaging/ossimRgbToHsvSource.i"
%include "imaging/ossimRgbToIndexFilter.i"
%include "imaging/ossimRgbToJpegYCbCrSource.i"
%include "imaging/ossimWatermarkFilter.i"
%include "imaging/ossimWorldFileWriter.i"
%include "support_data/ossimApplanixEOFile.i"
/* Importing Projection Interfaces */
%include "projection/ossimProjectionFactoryBase.i"

%include "projection/ossimAffineProjection.i"
%include "projection/ossimAlbersProjection.i"
%include "projection/ossimBonneProjection.i"
%include "projection/ossimBuckeyeSensor.i"
%include "projection/ossimCassiniProjection.i"
%include "projection/ossimEpsgProjectionDatabase.i"
%include "projection/ossimEpsgProjectionFactory.i"
%include "projection/ossimGnomonicProjection.i"
%include "projection/ossimIkonosRpcModel.i"
%include "projection/ossimImageProjectionModel.i"
%include "projection/ossimImageViewAffineTransform.i"
%include "projection/ossimImageViewTransform.i"
%include "projection/ossimImageViewProjectionTransform.i"
%include "projection/ossimImageViewTransformFactory.i"

%include "projection/ossimLandSatModel.i"
%include "projection/ossimMapProjectionFactory.i"
%include "projection/ossimMapProjection.i"
%include "projection/ossimMapProjectionInfo.i"
%include "projection/ossimMercatorProjection.i"
%include "projection/ossimNitfMapModel.i"
%include "projection/ossimNitfProjectionFactory.i"
%include "projection/ossimNitfRpcModel.i"
%include "projection/ossimObliqueMercatorProjection.i"
%include "projection/ossimOrthoGraphicProjection.i"
%include "projection/ossimPolyconicProjection.i"
%include "projection/ossimPolynomProjection.i"
%include "projection/ossimPositionQualityEvaluator.i"

%include "projection/ossimProjectionFactoryRegistry.i"
%include "projection/ossimProjection.i"
%include "projection/ossimOptimizableProjection.i"
%include "projection/ossimProjectionViewControllerFactory.i"
%include "projection/ossimQuadProjection.i"
%include "projection/ossimSensorModelFactory.i"
%include "projection/ossimSensorModel.i"
%include "projection/ossimSensorModelTuple.i"
%include "projection/ossimSpaceObliqueMercatorProjection.i"
%include "projection/ossimStatePlaneProjectionInfo.i"
%include "projection/ossimTiffProjectionFactory.i"
%include "projection/ossimTransCylEquAreaProjection.i"
%include "projection/ossimTransMercatorProjection.i"
%include "projection/ossimUpsProjection.i"
%include "projection/ossimUpspt.i"
%include "projection/ossimUtmProjection.i"
%include "projection/ossimUtmpt.i"
%include "projection/ossimApplanixUtmModel.i"
%include "projection/ossimApplanixEcefModel.i"

/* Importing Util Interfaces */
/*%include "util/ossimElevUtil.i" */

/*this block can be removed later*/
%module pyossim
%{
#include <map>
#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimSingleImageChain.h>
#include <ossim/projection/ossimMapProjection.h>
%}
%include "util/ossimFileWalker.i"
%include "util/ossimInfo.i"
%include "util/ossimRpfUtil.i"

/* Importing Support_data Interfaces */
%include "support_data/ossimInfoBase.i"
