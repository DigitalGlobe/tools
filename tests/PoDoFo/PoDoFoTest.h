//-----------------------------------------------------------------------------
/*
 * CPoDoFoTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CPoDoFoTest</code> class represents test fixtures for the PoDoFo
 * library.
 */
class CPoDoFoTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CPoDoFoTest);
            CPPUNIT_TEST(CPoDoFoTest::testCreate);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testCreate();
};
//-----------------------------------------------------------------------------
