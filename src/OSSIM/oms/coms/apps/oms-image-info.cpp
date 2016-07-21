//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OMS image info application.
// 
//----------------------------------------------------------------------------
// $Id$

#include <iostream>
#include <string>
#include <oms/Init.h>
#include <oms/DataInfo.h>

int main(int argc, char *argv[])
{
   oms::Init::instance()->initialize(argc, argv);

   if (argc != 2)
   {
      std::cout << argv[0] << " <file>\nWill dump info for file." << std::endl;
      return 1;
   }

   oms::DataInfo info;
   if ( info.open(std::string(argv[1])) )
   {
      std::string s = info.getInfo();
      std::cout << s.c_str() << std::endl;
   }
   else
   {
      std::cerr << "Could not open: " << argv[1] << std::endl;
      return 1;
   }
   
   return 0;
}

