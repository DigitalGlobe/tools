//-----------------------------------------------------------------------------
/*
 * CBisonTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <string>

//-----------------------------------------------------------------------------
/**
 * The <code>CBisonTest</code> class represents test fixtures for the Bison
 * library.
 */
class CBisonTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CBisonTest);
            CPPUNIT_TEST(CBisonTest::testExecution);
            CPPUNIT_TEST_SUITE_END();

    public:

        //---------------------------------------------------------------------
        // initialization and deinitialization methods

            void setUp();
            void tearDown();

        //---------------------------------------------------------------------
        // test methods

            void testExecution();

        //---------------------------------------------------------------------
        // private methods

            const std::string ReadFile(const char* szFileName);
};
//-----------------------------------------------------------------------------
