//-----------------------------------------------------------------------------
//
// Program.cpp
//
//-----------------------------------------------------------------------------

#include "..\\CppUnitFacade.h"
#include <stdio.h>
#include <string>

using namespace DigitalGlobeTesting;
using namespace std;

//-----------------------------------------------------------------------------
// global variables

    //-------------------------------------------------------------------------
    /** the name of the test input file */
    string g_sInputFileName;
    //-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// function prototypes


    static void DisplayUsage();
    static int  ProcessCommandLineArguments( int   argc   ,
                                             char* argv[] );

//-----------------------------------------------------------------------------
/**
 * Displays usage information.
 */
static void DisplayUsage()
{
    ::printf("Usage: APR.exe <input_file_name>");
    ::printf("\n");
}
//-----------------------------------------------------------------------------
/**
 * Processes specified command-line arguments.
 *
 * @param   argc  the number of command-line arguments
 * @param   argv  the command-line arguments
 * @return  zero if this method successfully processed the specified
 *          command-line arguments, non-zero if the specified command-
 *          lines are invaild
 */
static int ProcessCommandLineArguments( int   argc   ,
                                        char* argv[] )
{
    int iResult = 0;

    if (argc == 2)
    {
        ::g_sInputFileName = argv[1];
        iResult            = 0;
    }
    else
    {
        iResult = -1;
    }

    return iResult;
}
//-----------------------------------------------------------------------------
/**
 * The main method of the application.
 *
 * @param   argc  the number of command-line arguments
 * @param   argv  the command-line arguments
 */
int main( int   argc   ,
          char* argv[] )
{
    int            iResult = 0;
    CCppUnitFacade cppUnitFacade;

    // process command-line arguments
    if ( ::ProcessCommandLineArguments( argc ,
                                        argv ) != 0 )
    {
        ::DisplayUsage();

        return -1;
    }

    // run the tests
    iResult = cppUnitFacade.RunCppUnitTests();

    return iResult;
}
//-----------------------------------------------------------------------------
