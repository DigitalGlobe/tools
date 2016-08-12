//-----------------------------------------------------------------------------
/* 
 * LibTIFFTest.cpp
 */
//-----------------------------------------------------------------------------

#include "LibTIFFTest.h"

#include <libtiff/tiffio.h>

using namespace std;

//-----------------------------------------------------------------------------
// class constants

    const char* CLibTIFFTest::FILE_NAME_INPUT = "LibTIFF\\data\\DigitalGlobe.tif";

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CLibTIFFTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CLibTIFFTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CLibTIFFTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests reading a TIFF file.  The code for this method derives from:
 *     
 */
void CLibTIFFTest::testReadFile()
{
    const char* MODE     = "rb";
    TIFF*       pImage   = nullptr;
    uint32      uiHeight = 0;
    uint32      uiWidth  = 0;

    // open input file
    pImage = ::TIFFOpen( CLibTIFFTest::FILE_NAME_INPUT ,
                         MODE                          );
    CPPUNIT_ASSERT(nullptr != pImage);

    // get the image dimensions
    ::TIFFGetField(  pImage              ,
                     TIFFTAG_IMAGEWIDTH  ,
                    &uiWidth             );
    ::TIFFGetField(  pImage              ,
                     TIFFTAG_IMAGELENGTH ,
                    &uiHeight            );
    CPPUNIT_ASSERT_EQUAL( 339U     ,
                          uiWidth  );
    CPPUNIT_ASSERT_EQUAL( 104U     ,
                          uiHeight );

    // close input file
    ::TIFFClose(pImage);
    pImage = nullptr;
}
//-----------------------------------------------------------------------------
