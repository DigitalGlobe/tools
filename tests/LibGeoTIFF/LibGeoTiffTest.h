//-----------------------------------------------------------------------------
/*
 * LibGeoTiffTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <geotiffio.h>
#include <libxtiff/xtiffio.h>

//-----------------------------------------------------------------------------
/**
 * The <code>LibGeoTiffTest</code> class represents test fixtures for the LibGeoTiff
 * library.
 */
class CLibGeoTiffTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CLibGeoTiffTest);
            CPPUNIT_TEST(testWriteFile);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testWriteFile();

    private:

        //---------------------------------------------------------------------
        // private methods

            void SetUpGeoKeys(GTIF* pGeoTiff);
            void SetUpTiffDirectory(TIFF* pTiff);
            void WriteImage(TIFF* pTiff);

};
//-----------------------------------------------------------------------------
