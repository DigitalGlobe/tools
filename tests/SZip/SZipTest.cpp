//-----------------------------------------------------------------------------
/* 
 * SZipTest.cpp
 */
//-----------------------------------------------------------------------------

#include "SZipTest.h"
#include <assert.h>
#include <stdio.h>

extern "C"
{
    #include <szlib.h>
}

//-----------------------------------------------------------------------------
// class constants

    const char* CSZipTest::FILE_NAME_INPUT    = "SZip\\data\\h5repack_szip.h5";
    const long  CSZipTest::SIZE_IMAGE_MAXIMUM = (1024L * 1024L);

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CSZipTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CSZipTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CSZipTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests reading a file.
 */
void CSZipTest::testReadFile()
{
    const int  BITS_PER_PIXEL        = 8;
    const int  PIXELS_PER_BLOCK      = 8;
    const int  PIXELS_PER_SCANLINE   = 16;
    const long BYTES_PER_PIXEL       = 2;
    char*      pImage                = nullptr;
    char*      pOutput               = nullptr;
    int        iDecompressResult     = 0;
    int        iInitializationResult = 0;
    long       lImageSize            = 0L;
    sz_stream  imageStream           = { 0 };

    // allocate image
    pImage = new char[CSZipTest::SIZE_IMAGE_MAXIMUM];
    assert(pImage  != nullptr);
    pOutput = new char[CSZipTest::SIZE_IMAGE_MAXIMUM];
    assert(pOutput != nullptr);

    // read image
    lImageSize = this->readImage( CSZipTest::FILE_NAME_INPUT ,
                                  pImage                     );
    CPPUNIT_ASSERT_EQUAL( 5588L      ,
                          lImageSize );

    // decompress image
    imageStream.next_in             = pImage;
    imageStream.next_out            = pOutput;
    imageStream.options_mask        = ( SZ_RAW_OPTION_MASK |
                                        SZ_NN_OPTION_MASK  |
                                        SZ_MSB_OPTION_MASK );
	imageStream.bits_per_pixel      = BITS_PER_PIXEL;
	imageStream.pixels_per_block    = PIXELS_PER_BLOCK;
	imageStream.pixels_per_scanline = PIXELS_PER_SCANLINE;
    imageStream.image_pixels        = ( lImageSize      /
                                        BYTES_PER_PIXEL );
    iInitializationResult           = ::SZ_DecompressInit(&imageStream);
    CPPUNIT_ASSERT_EQUAL( SZ_OK                 ,
                          iInitializationResult );
    iDecompressResult               = ::SZ_Decompress( &imageStream ,
                                                        SZ_NO_FLUSH );
    CPPUNIT_ASSERT_EQUAL( SZ_OK             ,
                          iDecompressResult );

    // deallocate image
    delete [] pOutput;
    pOutput = nullptr;
    delete [] pImage;
    pImage  = nullptr;
}
//-----------------------------------------------------------------------------
/**
 * Reads a specified image.
 *
 * @param   szFileName   the name of the image file to read
 * @param   szReadImage  the read image (output)
 * @return  the size of the read image
 */
long CSZipTest::readImage( const char* szFileName  ,
                           char*       szReadImage )
{
    const char*  MODE         = "rb";
    const size_t ELEMENT_SIZE = 1;
    const size_t MAX_COUNT    = (16 * 1024);
    FILE*        pFile        = nullptr;
    int          iReadCount   = 0;
    int          iOpenResult  = 0;
    long         lSize        = 0L;
    
    // open file
    iOpenResult = ::fopen_s( &pFile      ,
                              szFileName ,
                              MODE       );
    CPPUNIT_ASSERT(nullptr != pFile);
    
    // read file
    do
    {
        iReadCount  = ::fread_s( (szReadImage + lSize)         ,
                                 CSZipTest::SIZE_IMAGE_MAXIMUM ,
                                 ELEMENT_SIZE                  ,
                                 MAX_COUNT                     ,
                                 pFile                         );
        lSize      += iReadCount;
    }
    while (iReadCount > 0);
    
    // close file
    fclose(pFile);
    pFile = nullptr;

    return lSize;
}
//-----------------------------------------------------------------------------
