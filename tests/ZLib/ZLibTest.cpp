//-----------------------------------------------------------------------------
/* 
 * ZLibTest.cpp
 */
//-----------------------------------------------------------------------------

#include "ZLibTest.h"
#include <zlib.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CZLibTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CZLibTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CZLibTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests compress and uncompress.
 */
void CZLibTest::testCompress()
{
    const uInt    LENGTH_COMPRESSION    = ( 10000       *
                                            sizeof(int) );
    const uInt    LENGTH_UNCOMPRESSION  = ( 10000       *
                                            sizeof(int) );
    z_const char* TEST_STRING           = "This is a test string";
    Byte*         abCompressedString    = nullptr;
    Byte*         abUncompressedString  = nullptr;
    int           iCompressResult       = 0;
    int           iUncompressResult     = 0;
    uLong         ulCompressionLength   = LENGTH_COMPRESSION;
    uLong         ulUncompressionLength = LENGTH_COMPRESSION;

    // allocate storage
    abCompressedString   = static_cast<Byte*>( ::calloc( LENGTH_COMPRESSION ,
                                                         1                  ) );
    CPPUNIT_ASSERT(abCompressedString   != Z_NULL);
    abUncompressedString = static_cast<Byte*>( ::calloc( LENGTH_COMPRESSION ,
                                                         1                  ) );
    CPPUNIT_ASSERT(abUncompressedString != Z_NULL);

    // compress
    iCompressResult = ::compress(  abCompressedString                              ,
                                  &ulCompressionLength                             ,
                                   reinterpret_cast<const Bytef*>(TEST_STRING)     ,
                                   static_cast<uLong>( ::strlen(TEST_STRING) + 1 ) );
    CPPUNIT_ASSERT_EQUAL( Z_OK            ,
                          iCompressResult );

    // decompress
    iUncompressResult = ::uncompress(  abUncompressedString  ,
                                      &ulUncompressionLength ,
                                       abCompressedString    ,
                                       ulCompressionLength   );
    CPPUNIT_ASSERT_EQUAL( Z_OK              ,
                          iUncompressResult );
    CPPUNIT_ASSERT_EQUAL( 0                                                               ,
                          ::strcmp( reinterpret_cast<const char*>(abUncompressedString) ,
                                    TEST_STRING                                         ) );
}
//-----------------------------------------------------------------------------
