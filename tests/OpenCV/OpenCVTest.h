//-----------------------------------------------------------------------------
/*
 * COpenCVTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>COpenCVTest</code> class represents test fixtures for the OpenCV
 * library.
 */
class COpenCVTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(COpenCVTest);
            CPPUNIT_TEST(COpenCVTest::testConvexHull);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testConvexHull();

};
//-----------------------------------------------------------------------------
