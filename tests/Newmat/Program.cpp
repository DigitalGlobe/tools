//-----------------------------------------------------------------------------
//
// Program.cpp
//
//-----------------------------------------------------------------------------

#include "NewmatTest.h"
#include <stdio.h>
#include <stdlib.h>

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
    int         iResult = 0;
    CNewmatTest newmatTest;

    try
    {
        newmatTest.setUp();
        newmatTest.tearDown();
        newmatTest.testRegression();
        printf("OK\n");
    }
    catch (const CTestException& ex)
    {
        printf("Failed\n");

        iResult = -1;
    }

    return iResult;
}
//-----------------------------------------------------------------------------
