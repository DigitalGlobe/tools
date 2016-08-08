//-----------------------------------------------------------------------------
/*
 * ZLibTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>ZLibTest</code> class represents test fixtures for the ZLib
 * library.
 */
class CZLibTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CZLibTest);
            CPPUNIT_TEST(testCompress);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testCompress();
};
//-----------------------------------------------------------------------------
