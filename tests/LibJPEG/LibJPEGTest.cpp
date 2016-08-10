//-----------------------------------------------------------------------------
/* 
 * LibJPEGTest.cpp
 */
//-----------------------------------------------------------------------------

#include "LibJPEGTest.h"
#include "..\\UtilityDirectory.h"
#include <stdio.h>
#include <string>

extern "C"
{
    #include <jpeglib.h>
}

using namespace DigitalGlobeTesting;
using namespace std;

//-----------------------------------------------------------------------------
// class constants

    const char* CLibJPEGTest::FILE_NAME_INPUT = "data\\DigitalGlobe.jpg";

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CLibJPEGTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CLibJPEGTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CLibJPEGTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests reading a JPEG file.  The code for this method derives from:
 * <a href="https://github.com/LuaDist/libjpeg/blob/master/example.c" />.
 */
void CLibJPEGTest::testReadFile()
{
    const char*                   MODE        = "rb";
    FILE*                         pInputFile  = nullptr;
    int                           iReadResult = 0;
    string                        sInputFileName;
    struct jpeg_decompress_struct decompressionInstance;
    struct jpeg_error_mgr         errorHandler;

    // open the input file
    sInputFileName += CUtilityDirectory::GetCurrentDirectory();
    sInputFileName += "\\LibJPEG\\";
    sInputFileName += CLibJPEGTest::FILE_NAME_INPUT;
    ::fopen_s( &pInputFile             ,
                sInputFileName.c_str() ,
                MODE                   );
    CPPUNIT_ASSERT(nullptr != pInputFile);

    // read the input file
    decompressionInstance.err = ::jpeg_std_error(&errorHandler);
    jpeg_create_decompress(&decompressionInstance);
    ::jpeg_stdio_src( &decompressionInstance ,
                       pInputFile            );
    iReadResult = ::jpeg_read_header( &decompressionInstance ,
                                       TRUE                  );
    CPPUNIT_ASSERT(0 != iReadResult);
    CPPUNIT_ASSERT_EQUAL( 339                                                  ,
                          static_cast<int>(decompressionInstance.image_width ) );
    CPPUNIT_ASSERT_EQUAL( 104                                                  ,
                          static_cast<int>(decompressionInstance.image_height) );

    // close the input file
    ::fclose(pInputFile);
    pInputFile = nullptr;
}
//-----------------------------------------------------------------------------
