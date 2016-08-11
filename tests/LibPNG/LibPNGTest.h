//-----------------------------------------------------------------------------
/*
 * LibPNGTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>LibPNGTest</code> class represents test fixtures for the LibPNG
 * library.
 */
class CLibPNGTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CLibPNGTest);
            CPPUNIT_TEST(testReadFile);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testReadFile();

    private:

        //---------------------------------------------------------------------
        // private constants

            //-----------------------------------------------------------------
            /** the name of the input file */
            static const char* FILE_NAME_INPUT;
            //-----------------------------------------------------------------
};
//-----------------------------------------------------------------------------
