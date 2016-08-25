//-----------------------------------------------------------------------------
/* 
 * BisonTest.cpp
 */
//-----------------------------------------------------------------------------

#include "BisonTest.h"
#include <process.h>
#include <string>

using namespace std;

//-----------------------------------------------------------------------------
// global variables

    //-------------------------------------------------------------------------
    /** the name of the Bison executable file */
    extern string g_sBisonExecutableFileName;
    //-------------------------------------------------------------------------
    /** the name of the test input file */
    extern string g_sInputFileName;
    //-------------------------------------------------------------------------
    /** the name of the resulting output file */
    extern string g_sOutputFileName;
    //-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CBisonTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CBisonTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CBisonTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests executing Bison.
 */
void CBisonTest::testExecution()
{
    const int MAXIMUM_COMMAND_LINE_LENGTH = 2047;
    char      szCommandLine[MAXIMUM_COMMAND_LINE_LENGTH + 1];
    int       iExecutionResult            = 0;
    string    sExpectedOutput;
    string    sResultingOutput;

    // execute Bison
    ::sprintf_s( szCommandLine                        ,
                 MAXIMUM_COMMAND_LINE_LENGTH          ,
                 "\"\"%s\" \"%s\"\""                  ,
                 ::g_sBisonExecutableFileName.c_str() ,
                 ::g_sInputFileName.c_str()           );
    iExecutionResult = ::system(szCommandLine);
    CPPUNIT_ASSERT_EQUAL( 0                ,
                          iExecutionResult );

    // read the resulting output file
    sResultingOutput = this->ReadFile( ::g_sOutputFileName.c_str() );

    // delete the output file
    remove( g_sOutputFileName.c_str() );
}
//-----------------------------------------------------------------------------
/**
 * Reads the content of a specified file.
 *
 * @param   szFielName  the name of the file to read
 * @return  the content of the specified file
 */
const string CBisonTest::ReadFile(const char* szFileName)
{
    std::ifstream inputStream(szFileName);
    
    return string( ( std::istreambuf_iterator<char>(inputStream) ) ,
                   ( std::istreambuf_iterator<char>(           ) ) );
}
//-----------------------------------------------------------------------------
