//-----------------------------------------------------------------------------
/* 
 * LibPNGTest.cpp
 */
//-----------------------------------------------------------------------------

#include "LibPNGTest.h"
#include "..\\UtilityDirectory.h"
#include <png.h>
#include <stdio.h>
#include <string>

using namespace DigitalGlobeTesting;
using namespace std;

//-----------------------------------------------------------------------------
// class constants

    const char* CLibPNGTest::FILE_NAME_INPUT = "LibPNG\\data\\DigitalGlobe.png";

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CLibPNGTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CLibPNGTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CLibPNGTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests reading a PNG file.  The code for this method derives from:
 * <a href="http://zarb.org/~gc/html/libpng.html" />.
 */
void CLibPNGTest::testReadFile()
 {
    const char*  MODE         = "rb";
    FILE*        pInputFile   = nullptr;
    int          iHeight      = 0;
    int          iReadResult  = 0;
    int          iWidth       = 0;
    png_infop    pInformation = nullptr;
    png_structp  pImage       = nullptr;

    // open the input file
    ::fopen_s( &pInputFile                   ,
                CLibPNGTest::FILE_NAME_INPUT ,
                MODE                         );
    CPPUNIT_ASSERT(nullptr != pInputFile);

    // read the input file
    pImage = ::png_create_read_struct( PNG_LIBPNG_VER_STRING ,
                                       NULL                  ,
                                       NULL                  ,
                                       NULL                  );
    CPPUNIT_ASSERT(nullptr != pImage);
    pInformation = png_create_info_struct(pImage);
    CPPUNIT_ASSERT(nullptr != pInformation);
    ::png_init_io( pImage     ,
                   pInputFile );
    ::png_read_info( pImage       ,
                     pInformation );
    iWidth  = ::png_get_image_width( pImage       ,
                                     pInformation );
    iHeight = ::png_get_image_height( pImage       ,
                                      pInformation );
    CPPUNIT_ASSERT_EQUAL( 339     ,
                          iWidth  );
    CPPUNIT_ASSERT_EQUAL( 104     ,
                          iHeight );

    // close the input file
    ::fclose(pInputFile);
    pInputFile = nullptr;
}
//-----------------------------------------------------------------------------
