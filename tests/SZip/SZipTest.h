//-----------------------------------------------------------------------------
/*
 * SZipTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>

//-----------------------------------------------------------------------------
/**
 * The <code>SZipTest</code> class represents test fixtures for the SZip
 * library.
 */
class CSZipTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CSZipTest);
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
            /** the maximum size of an image */
            static const long SIZE_IMAGE_MAXIMUM;
            //-----------------------------------------------------------------

        //---------------------------------------------------------------------
        // private methods

            long readImage( const char* szFileName  ,
                            char*       szReadImage );

};
//-----------------------------------------------------------------------------
