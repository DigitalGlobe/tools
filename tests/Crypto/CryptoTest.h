//-----------------------------------------------------------------------------
/*
 * CCryptoTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <string>

//-----------------------------------------------------------------------------
/**
 * The <code>CCryptoTest</code> class represents test fixtures for the Crypto
 * library.
 */
class CCryptoTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CCryptoTest);
            CPPUNIT_TEST(CCryptoTest::testEncryption);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testEncryption();

};
//-----------------------------------------------------------------------------
