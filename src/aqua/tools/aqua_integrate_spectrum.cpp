/*
  This file is part of “The Aqua library”.

  Copyright © 2005 2006  Jocelyn Fréchot

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


/****************  includes  ****************/


#include <libaqua/aqua_spectrum.h>

/*  local lib  */
#include "aqua_lib_exceptions.h"

/*  C++ lib  */
#include <iostream>

/*  C lib  */
#include <cstdlib>  /*  strtod()  */
#include <cmath>    /*  INFINITY  */


/****************  namespaces  ****************/


using namespace std;


/****************  main  ****************/


int
main(int argc, char ** argv)
{
  if (argc != 5)
    {
      cerr << argv[0] << " w f from to" << endl;
    }

  double w = strtod(argv[1], NULL);
  double f = strtod(argv[2], NULL);
  double from = strtod(argv[3], NULL);
  double to = strtod(argv[4], NULL);


  class aqua::spectrum::Energy * spectrum;


  try
    {
//       if (argc == 4)
// 	{
// 	  double f = strtod(argv[3], NULL);
// 	  spectrum = new class Jonswap(w, f);
// 	}
//       else
// 	{
      spectrum = new class aqua::spectrum::Jonswap(w, f);
// 	}

      cout << spectrum->compute_integral(from, to) << endl;


      delete spectrum;
    }
  catch(class std::bad_alloc & exception)
    {
      aqua_lib::exceptions(argv[0], exception);
    }
  catch(class std::exception & exception)
    {
      aqua_lib::exceptions(argv[0], exception);
    }
  catch(const char * exception)
    {
      aqua_lib::exceptions(argv[0], exception);
    }
  catch(...)
    {
      aqua_lib::exceptions(argv[0], "unknown error");
    }
}
