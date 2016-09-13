//-----------------------------------------------------------------------------
/* 
 * GSoapTest.cpp
 */
//-----------------------------------------------------------------------------

#include "GSoapTest.h"
#include <process.h>
#include <string>

using namespace std;

//-----------------------------------------------------------------------------
// global variables

    //-------------------------------------------------------------------------
    /** the name of the expected file */
    extern string g_sExpectedFileName;
    //-------------------------------------------------------------------------
    /** the name of the GSoap executable file */
    extern string g_sGSoapExecutableFileName;
    //-------------------------------------------------------------------------
    /** the name of the test input file */
    extern string g_sInputFileName;
    //-------------------------------------------------------------------------
    /** the name of the output file */
    extern string g_sOutputFileName;
    //-------------------------------------------------------------------------
    /** the name of the output path */
    extern string g_sOutputPathName;
    //-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CGSoapTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CGSoapTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CGSoapTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests executing GSoap.
 */
void CGSoapTest::testExecution()
{
    const int MAXIMUM_COMMAND_LINE_LENGTH = 2047;
    char      szCommandLine[MAXIMUM_COMMAND_LINE_LENGTH + 1];
    int       iExecutionResult            = 0;
    string    sExpectedOutput;
    string    sResultingOutput;

    // execute GSoap
    ::sprintf_s( szCommandLine                        ,
                 MAXIMUM_COMMAND_LINE_LENGTH          ,
                 "\"\"%s\" \"%s\" -d\"%s\"\""         ,
                 ::g_sGSoapExecutableFileName.c_str() ,
                 ::g_sInputFileName.c_str()           ,
                 ::g_sOutputPathName.c_str()          );
    printf("%s\n", szCommandLine);
    iExecutionResult = ::system(szCommandLine);
    CPPUNIT_ASSERT_EQUAL( 0                ,
                          iExecutionResult );

    // read the resulting output and expected output
    sResultingOutput = this->ReadFile( ::g_sOutputFileName.c_str()   );
    sExpectedOutput  = this->ReadFile( ::g_sExpectedFileName.c_str() );
//    CPPUNIT_ASSERT(sExpectedOutput == sResultingOutput);
}
//-----------------------------------------------------------------------------
/**
 * Reads the content of a specified file.
 *
 * @param   szFielName  the name of the file to read
 * @return  the content of the specified file
 */
const string CGSoapTest::ReadFile(const char* szFileName)
{
    std::ifstream inputStream(szFileName);
    
    return string( ( std::istreambuf_iterator<char>(inputStream) ) ,
                   ( std::istreambuf_iterator<char>(           ) ) );
}
//-----------------------------------------------------------------------------
