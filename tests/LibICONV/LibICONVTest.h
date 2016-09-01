//-----------------------------------------------------------------------------
/*
 * LibICONVTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>LibICONVTest</code> class represents test fixtures for the LibICONV
 * library.
 */
class CLibICONVTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CLibICONVTest);
            CPPUNIT_TEST(testConversion);
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
            /** the name of the input file */
            static const char* FILE_NAME_INPUT;
            //-----------------------------------------------------------------
};
//-----------------------------------------------------------------------------
