//-----------------------------------------------------------------------------
/*
 * CCURLTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CCURLTest</code> class represents test fixtures for the CURL
 * library.
 */
class CCURLTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CCURLTest);
            CPPUNIT_TEST(CCURLTest::testGet);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testGet();
};
//-----------------------------------------------------------------------------
