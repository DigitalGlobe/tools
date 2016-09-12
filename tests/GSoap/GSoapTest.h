//-----------------------------------------------------------------------------
/*
 * CGSoapTest.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <string>

//-----------------------------------------------------------------------------
/**
 * The <code>CGSoapTest</code> class represents test fixtures for the GSoap
 * library.
 */
class CGSoapTest : public CppUnit::TestFixture
{
        //---------------------------------------------------------------------
        // class macros

            CPPUNIT_TEST_SUITE(CGSoapTest);
            CPPUNIT_TEST(CGSoapTest::testExecution);
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
