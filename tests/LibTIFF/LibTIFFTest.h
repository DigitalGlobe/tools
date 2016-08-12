//-----------------------------------------------------------------------------
/*
 * LibTIFFTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>LibTIFFTest</code> class represents test fixtures for the LibTIFF
 * library.
 */
class CLibTIFFTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CLibTIFFTest);
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
