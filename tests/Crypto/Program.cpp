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

    iResult = cppUnitFacade.RunCppUnitTests();

    return iResult;
}
//-----------------------------------------------------------------------------
