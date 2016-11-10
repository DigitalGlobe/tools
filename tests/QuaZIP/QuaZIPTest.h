//-----------------------------------------------------------------------------
/*
 * CQuaZIPTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CQuaZIPTest</code> class represents test fixtures for the QuaZIP
 * library.
 */
class CQuaZIPTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CQuaZIPTest);
            CPPUNIT_TEST(CQuaZIPTest::testChecksum);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testChecksum();

};
//-----------------------------------------------------------------------------
