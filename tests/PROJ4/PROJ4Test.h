//-----------------------------------------------------------------------------
/*
 * CPROJ4Test.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CPROJ4Test</code> class represents test fixtures for the PROJ4
 * library.
 */
class CPROJ4Test : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CPROJ4Test);
            CPPUNIT_TEST(CPROJ4Test::testConversion);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testConversion();

    private:

        //---------------------------------------------------------------------
        // private constants

            //-----------------------------------------------------------------
            /** the maximum difference between which two double values are
             *  approximately equal */
            static const double DELTA_DOUBLE;
            //-----------------------------------------------------------------
};
//-----------------------------------------------------------------------------
