//-----------------------------------------------------------------------------
/*
* AprUtilTest.cpp
*/
//-----------------------------------------------------------------------------

#include "AprUtilTest.h"
#include <apr_errno.h>
#include <apr_md5.h>

using namespace std;

//-----------------------------------------------------------------------------
// class macros

CPPUNIT_TEST_SUITE_REGISTRATION(CAprUtilTest);

//-----------------------------------------------------------------------------
/**
* Sets up this fixture.
*/
void CAprUtilTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
* Tears down this fixture.
*/
void CAprUtilTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
* Tests password validation.
*/
void CAprUtilTest::testValidate()
{
    const char* HASH     = "1fWDc9QWYCWrQ";
    const char* PASSWORD = "pass1";

    CPPUNIT_ASSERT( ::apr_password_validate( PASSWORD ,
                                             HASH     ) != 0 );
}
//-----------------------------------------------------------------------------
