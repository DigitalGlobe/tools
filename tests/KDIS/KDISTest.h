//-----------------------------------------------------------------------------
/*
 * CKDISTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CKDISTest</code> class represents test fixtures for the KDIS
 * library.
 */
class CKDISTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CKDISTest);
            CPPUNIT_TEST(CKDISTest::testRead);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testRead();

};
//-----------------------------------------------------------------------------
