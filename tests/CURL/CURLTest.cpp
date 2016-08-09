//-----------------------------------------------------------------------------
/* 
 * CURLTest.cpp
 */
//-----------------------------------------------------------------------------

#include "CURLTest.h"
#include <curl/curl.h>

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CCURLTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CCURLTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CCURLTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests HTTP GET.  The code for this method derives from:
 *     <a href="https://curl.haxx.se/libcurl/c/simple.html">simple.c</a>.
 */
void CCURLTest::testGet()
{
    const char* URL_TEST   = "http://www.google.com";
    CURL*       pCurl      = nullptr;
    CURLcode    iGetResult = CURLcode::CURLE_OK;

    // initialize CURL
    pCurl = ::curl_easy_init();
    CPPUNIT_ASSERT(nullptr != pCurl);
    ::curl_easy_setopt( pCurl                              ,
                        CURLoption::CURLOPT_URL            ,
                        URL_TEST                           );
    ::curl_easy_setopt( pCurl                              ,
                        CURLoption::CURLOPT_FOLLOWLOCATION ,
                        1L                                 );

    // do the HTTP GET
    iGetResult = ::curl_easy_perform(pCurl);
    CPPUNIT_ASSERT_EQUAL( CURLcode::CURLE_OK ,
                          iGetResult         );

    // clean up
    ::curl_easy_cleanup(pCurl);
    pCurl = nullptr;
}
//-----------------------------------------------------------------------------
