//-----------------------------------------------------------------------------
/*
 * COpenThreadsTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>COpenThreadsTest</code> class represents test fixtures for the OpenThreads
 * library.
 */
class COpenThreadsTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(COpenThreadsTest);
            CPPUNIT_TEST(COpenThreadsTest::testThreading);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testThreading();

};
//-----------------------------------------------------------------------------
