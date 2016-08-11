//-----------------------------------------------------------------------------
/*
 * CHDF5Test.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CHDF5Test</code> class represents test fixtures for the HDF5
 * library.
 */
class CHDF5Test : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CHDF5Test);
            CPPUNIT_TEST(CHDF5Test::testRead);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testRead();

    private:

        //---------------------------------------------------------------------
        // private constants

            //-----------------------------------------------------------------
            /** the name of the data set */
            static const char* DATA_SET_NAME;
            //-----------------------------------------------------------------
            /** the name of the input file */
            static const char* FILE_NAME_INPUT;
            //-----------------------------------------------------------------
};
//-----------------------------------------------------------------------------
