//-----------------------------------------------------------------------------
/*
 * COpenDISTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>COpenDISTest</code> class represents test fixtures for the OpenDIS
 * library.
 */
class COpenDISTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(COpenDISTest);
            CPPUNIT_TEST(COpenDISTest::testMarshal);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testMarshal();

};
//-----------------------------------------------------------------------------
