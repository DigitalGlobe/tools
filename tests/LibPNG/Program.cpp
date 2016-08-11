//-----------------------------------------------------------------------------
//
// Program.cpp
//
//-----------------------------------------------------------------------------

#include "..\\CppUnitFacade.h"

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
    CCppUnitFacade cppUnitFacade;

    cppUnitFacade.RunCppUnitTests();
system("pause");

    return 0;

}
//-----------------------------------------------------------------------------
