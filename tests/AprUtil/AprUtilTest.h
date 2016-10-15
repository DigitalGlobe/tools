//-----------------------------------------------------------------------------
/*
 * CAprUtilTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CAprUtilTest</code> class represents test fixtures for the AprUtil
 * library.
 */
class CAprUtilTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CAprUtilTest);
            CPPUNIT_TEST(CAprUtilTest::testValidate);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testValidate();

};
//-----------------------------------------------------------------------------
