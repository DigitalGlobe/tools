//-----------------------------------------------------------------------------
/* 
 * LibGeoTiffTest.cpp
 */
//-----------------------------------------------------------------------------

#include "LibGeoTiffTest.h"
#include "../UtilityDirectory.h"
#include <stdio.h>
#include <string>

using namespace DigitalGlobeTesting;
using namespace std;

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CLibGeoTiffTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CLibGeoTiffTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CLibGeoTiffTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests writing a GeoTIFF file.  The code for this method derives from:
 *     <a href="http://svn.osgeo.org/metacrs/geotiff/trunk/libgeotiff/bin/makegeo.c">makegeo.c</a>.
 */
void CLibGeoTiffTest::testWriteFile()
{
    const char* MODE            = "w";
    GTIF*       pGeoTiff        = nullptr;
    TIFF*       pTiff           = nullptr;
    string      sOutputFileName = CUtilityDirectory::GetTemporaryFileName();
    int         iWriteResult    = 0;
    
    // open output file
    pTiff = ::XTIFFOpen( sOutputFileName.c_str() ,
                         MODE                    );
    CPPUNIT_ASSERT(nullptr != pTiff);
    
    // create new GeoTIFF
    pGeoTiff = ::GTIFNew(pTiff);
    CPPUNIT_ASSERT(nullptr != pGeoTiff);
    
    // write the GeoTIFF
    this->SetUpTiffDirectory(pTiff);
    this->SetUpGeoKeys(pGeoTiff);
    this->WriteImage(pTiff);
    iWriteResult = ::GTIFWriteKeys(pGeoTiff);
    CPPUNIT_ASSERT(0 != iWriteResult);
    
    // close the GeoTIFF
    ::GTIFFree(pGeoTiff);
    pGeoTiff = nullptr;
    ::XTIFFClose(pTiff);
    pTiff = nullptr;
}
//-----------------------------------------------------------------------------
/**
 * Sets up the geo keys.  The code for this method derives from:
 *     <a href="http://svn.osgeo.org/metacrs/geotiff/trunk/libgeotiff/bin/makegeo.c">makegeo.c</a>.
 *
 * @param   pGeoTiff  the GeoTIFF to use
 */
void CLibGeoTiffTest::SetUpGeoKeys(GTIF* pGeoTiff)
{
    ::GTIFKeySet(pGeoTiff, GTModelTypeGeoKey      , TYPE_SHORT , 1, ModelGeographic                     );
    ::GTIFKeySet(pGeoTiff, GTRasterTypeGeoKey     , TYPE_SHORT , 1, RasterPixelIsArea                   );
    ::GTIFKeySet(pGeoTiff, GTCitationGeoKey       , TYPE_ASCII , 0, "Just An Example"                   );
    ::GTIFKeySet(pGeoTiff, GeographicTypeGeoKey   , TYPE_SHORT , 1, KvUserDefined                       );
    ::GTIFKeySet(pGeoTiff, GeogCitationGeoKey     , TYPE_ASCII , 0, "Everest Ellipsoid Used."           );
    ::GTIFKeySet(pGeoTiff, GeogAngularUnitsGeoKey , TYPE_SHORT , 1, Angular_Degree                      );
    ::GTIFKeySet(pGeoTiff, GeogLinearUnitsGeoKey  , TYPE_SHORT , 1, Linear_Meter                        );
    ::GTIFKeySet(pGeoTiff, GeogGeodeticDatumGeoKey, TYPE_SHORT , 1, KvUserDefined                       );
    ::GTIFKeySet(pGeoTiff, GeogEllipsoidGeoKey    , TYPE_SHORT , 1, Ellipse_Everest_1830_1967_Definition);
    ::GTIFKeySet(pGeoTiff, GeogSemiMajorAxisGeoKey, TYPE_DOUBLE, 1, static_cast<double>(6377298.556)    );
    ::GTIFKeySet(pGeoTiff, GeogInvFlatteningGeoKey, TYPE_DOUBLE, 1, static_cast<double>(300.8017   )    );
}
//-----------------------------------------------------------------------------
/**
 * Sets up the TIFF directory.  The code for this method derives from:
 *     <a href="http://svn.osgeo.org/metacrs/geotiff/trunk/libgeotiff/bin/makegeo.c">makegeo.c</a>.
 *
 * @param   pTiff  the TIFF to use
 */
void CLibGeoTiffTest::SetUpTiffDirectory(TIFF* pTiff)
{
    double adTiePoints[6]  = {
                                 0     ,
                                 0     ,
                                 0.0   ,
                                 130.0 ,
                                 32.0  ,
                                 0.0   };
    double adPixelScale[3] = {
                                 1.0 ,
                                 1.0 ,
                                 0.0
                             };
    
    ::TIFFSetField(pTiff, TIFFTAG_IMAGEWIDTH    , 20L                   );
    ::TIFFSetField(pTiff, TIFFTAG_IMAGELENGTH   , 20L                   );
    ::TIFFSetField(pTiff, TIFFTAG_COMPRESSION   , COMPRESSION_NONE      );
    ::TIFFSetField(pTiff, TIFFTAG_PHOTOMETRIC   , PHOTOMETRIC_MINISBLACK);
    ::TIFFSetField(pTiff, TIFFTAG_PLANARCONFIG  , PLANARCONFIG_CONTIG   );
    ::TIFFSetField(pTiff, TIFFTAG_BITSPERSAMPLE , 8                     );
    ::TIFFSetField(pTiff, TIFFTAG_ROWSPERSTRIP  , 20L                   );
    ::TIFFSetField(pTiff, TIFFTAG_GEOTIEPOINTS  , 6, adTiePoints        );
    ::TIFFSetField(pTiff, TIFFTAG_GEOPIXELSCALE , 3, adPixelScale       );
}
//-----------------------------------------------------------------------------
/**
 * Writes a specified image.  This method derives from:
 *     <a href="http://svn.osgeo.org/metacrs/geotiff/trunk/libgeotiff/bin/makegeo.c">makegeo.c</a>.
 *
 * @param   pTiff  the image to write
 */
void CLibGeoTiffTest::WriteImage(TIFF* pTiff)
{
    const int HEIGHT    = 20;
    const int WIDTH     = 20;
    int       iLineIndex = 0;
    char      achBuffer[WIDTH];
    
    ::memset( achBuffer                  ,
              0                          ,
              static_cast<size_t>(WIDTH) );
    for ( iLineIndex = 0      ;
          iLineIndex < HEIGHT ;
          ++iLineIndex        )
    {
        int iWriteResult = ::TIFFWriteScanline( pTiff      ,
                                                achBuffer  ,
                                                iLineIndex ,
                                                0          );
                                                
        CPPUNIT_ASSERT(0 != iWriteResult);
    }
}
//-----------------------------------------------------------------------------
