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
#include <libaqua/aqua_spectrum_tool.h>

/*  local lib  */
#include "aqua_lib_exceptions.h"

/*  C++ lib  */
#include <iostream>

/*  C lib  */
#include <cstdlib>  /*  strtod()  */


/****************  namespaces  ****************/


using namespace std;


/****************  main  ****************/


int
main(int argc, char ** argv)
{
  const double w = strtod(argv[1], NULL);

  class aqua::spectrum::Directional * s;

  double top, right, bottom;


  if (argc == 3)
    {
      const double f = strtod(argv[2], NULL);
      s = aqua::spectrum::directional_create_jonswap_longuet(w, f);
    }
  else
    {
      s = aqua::spectrum::directional_create_pierson_longuet(w);
    }


  try
    {
      s->find_boundaries(top, right, bottom);
      delete s;
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

  cout << "top: " << top
       << " - right: " << right
       << " - bottom: " << bottom
       << endl;
}
