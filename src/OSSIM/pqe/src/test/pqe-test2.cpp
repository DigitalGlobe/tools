//-----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  A place to put pqe library test.
//
//-----------------------------------------------------------------------------
// $Id$

#include <iostream>
#include <string>
using namespace std;

#include <pqe/Constants.h>
#include <pqe/Gpt.h>

int main(int argc, char* argv[])
{
   std::string s = "( 28.598864426198883, -80.681869456196623, -30.580, WGE )";
   pqe::Gpt gpt;
   gpt.toPoint(s);
   std::string s2 = gpt.toString();
   std::string s3 = gpt.toString(8);

   cout << "\ns:                     " << s
        << "\ngpt.toPoint(s):        " << gpt
        << "\ns2 = gpt.toString():   " << s2
        << "\ns3 = gpt.toString(8):  " << s3
      
        << endl;

   return 0;
}
