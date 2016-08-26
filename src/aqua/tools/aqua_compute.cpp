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

#include "constant.h"

/*  local lib  */
#include "aqua_lib_exceptions.h"

/*  C++ lib  */
#include <iostream>

/*  C lib  */
#include <cstdlib>  /*  strtod()  */
#include <cmath>    /*  sqrt()  */


/****************  namespaces  ****************/


using namespace std;


/****************  main  ****************/


int
main(int argc, char ** argv)
{
  try
    {
      if ((argc < 3) || (argc > 4))
	{
	  throw "step wind (fetch).";
	}

      class aqua::spectrum::Directional * spectrum;

      const double step = strtod(argv[1], NULL);
      const double w = strtod(argv[2], NULL);

      if (argc == 3)
	{
	  spectrum = aqua::spectrum::directional_create_pierson_longuet(w);
	}
      else if (argc == 4)
	{
	  const double f = strtod(argv[3], NULL);
	  spectrum = aqua::spectrum::directional_create_jonswap_longuet(w,f);
	}

      double omega_from = 0.01;
      double omega_to = 0.011;

      double theta_from = 0.01;
      double theta_to = 0.011;

      size_t n_omega = (omega_to - omega_from) / step;
      size_t n_theta = (theta_to - theta_from) / step;

      double e = 0;
      for (size_t i = 0; i < n_omega; i++)
	{
	  for (size_t j = 0; j < n_theta; j++)
	    {
	      double t = theta_from + j * step;
	      double o = omega_from + i * step;
	      e +=
		(spectrum->compute_cartesian_wavenumber(t, o)
		 + spectrum->compute_cartesian_wavenumber(t + step, o)
		 + spectrum->compute_cartesian_wavenumber(t, o + step)
		 + spectrum->compute_cartesian_wavenumber(t + step, o + step)
		 );
	    }
	}
      cout << e / 4.0 * step * step <<endl;
      cout << sqrt(2 * e / 4.0 * step * step) <<endl;
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
