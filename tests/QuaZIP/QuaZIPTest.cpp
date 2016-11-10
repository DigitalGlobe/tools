//-----------------------------------------------------------------------------
/* 
 * QuaZIPTest.cpp
 */
//-----------------------------------------------------------------------------

#include "QuaZIPTest.h"

#include <iostream>

#include <quacrc32.h>

using namespace std;

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CQuaZIPTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CQuaZIPTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CQuaZIPTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests checksums.
 */
void CQuaZIPTest::testChecksum()
{
    const char* TEST_DATA = "Wikipedia";
    QuaCrc32    crc32;

    CPPUNIT_ASSERT_EQUAL( 0xADAAC02Eu                ,
                          crc32.calculate(TEST_DATA) );
}
//-----------------------------------------------------------------------------
