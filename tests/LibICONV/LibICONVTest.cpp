//-----------------------------------------------------------------------------
/* 
 * LibICONVTest.cpp
 */
//-----------------------------------------------------------------------------

#include "LibICONVTest.h"
#include <iconv.h>

using namespace std;

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CLibICONVTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CLibICONVTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CLibICONVTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests conversion.  This method derives from:
 * <a href="http://zarb.org/~gc/html/libiconv.html" />.
 */
void CLibICONVTest::testConversion()
 {
    const int    LENGTH_MAXIMUM     = 255;
    const char   TEST_STRING[]      = "abcde";
    const char*  CODE_FROM          = "CP1250";
    const char*  CODE_TO            = "UTF-8";
    const size_t LENGTH_SOURCE      = 6;
    const size_t LENGTH_DESTINATION = 12;
    char         szDestination[LENGTH_MAXIMUM];
    const char*  pszInputString     = TEST_STRING;
    char*        pszOutputString    = szDestination;
    size_t       iSourceLength      = LENGTH_SOURCE;
    size_t       iDestinationLength = LENGTH_DESTINATION;
 
    ::memset( szDestination  ,
              '\0'           ,
              LENGTH_MAXIMUM );
    iconv_t pDescriptor = ::iconv_open( CODE_TO   ,
                                        CODE_FROM );
    CPPUNIT_ASSERT(nullptr != pDescriptor);
    ::iconv(  pDescriptor        ,
             &pszInputString     ,
             &iSourceLength      ,
             &pszOutputString    ,
             &iDestinationLength );
    ::iconv_close(pDescriptor);
    pDescriptor = nullptr;
    CPPUNIT_ASSERT_EQUAL( 0                         ,
                          ::strcmp( TEST_STRING   ,
                                    szDestination ) );
}
//-----------------------------------------------------------------------------
