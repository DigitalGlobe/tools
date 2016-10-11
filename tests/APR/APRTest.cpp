//-----------------------------------------------------------------------------
/*
* APRTest.cpp
*/
//-----------------------------------------------------------------------------

#include "APRTest.h"
#include <apr.h>
#include <apr_file_io.h>
#include <stdlib.h>
#include <string>

using namespace std;

//-----------------------------------------------------------------------------
// global variables

    //-------------------------------------------------------------------------
    /** the name of the test input file */
    extern string g_sInputFileName;
    //-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// class macros

CPPUNIT_TEST_SUITE_REGISTRATION(CAPRTest);

//-----------------------------------------------------------------------------
/**
* Sets up this fixture.
*/
void CAPRTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
* Tears down this fixture.
*/
void CAPRTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
* Tests reading a file.
*/
void CAPRTest::testRead()
{
    const char*  TEXT_EXPECTED = "This is only a test";
    apr_file_t*  pFile         = nullptr;
    apr_pool_t*  pPool         = nullptr;
    apr_size_t   iBytesCount   = 1;
    apr_status_t iStatus       = 0;
    char         ch            = '\0';
    string       sText;

    // initialize APR
    ::apr_initialize();
    ::atexit(::apr_terminate);
    apr_pool_create( &pPool ,
                     NULL   );

    // open the file
    iStatus = ::apr_file_open( &pFile                      ,
                                ::g_sInputFileName.c_str() ,
                                APR_FOPEN_READ             ,
                                APR_OS_DEFAULT             ,
                                pPool                      );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(APR_SUCCESS) ,
                          static_cast<int>(iStatus)     );
                    
    // read the file
    do
    {
        iStatus = ::apr_file_read( pFile        ,
                                   &ch          ,
                                   &iBytesCount );
        CPPUNIT_ASSERT( (iStatus == APR_SUCCESS)   ||
                        APR_STATUS_IS_EOF(iStatus)  );
        if (iStatus == APR_SUCCESS)
        {
            sText += ch;
        }
    }
    while (iStatus == APR_SUCCESS);
    CPPUNIT_ASSERT( sText.compare(TEXT_EXPECTED) == 0 );

    // close the file
    ::apr_file_close(pFile);
    pFile = nullptr;
}
//-----------------------------------------------------------------------------
