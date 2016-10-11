//-----------------------------------------------------------------------------
/*
 * CAPRTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CAPRTest</code> class represents test fixtures for the APR
 * library.
 */
class CAPRTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CAPRTest);
            CPPUNIT_TEST(CAPRTest::testRead);
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
