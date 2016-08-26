/*
  This file is part of “The Aqua library”.

  Copyright © 2005  Jocelyn Fréchot

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


#include "aqua_spectrum_energy.h"

#include "aqua_spectrum_samples.h"

/*  C lib  */
#include <cmath>  /*  INIFINITY, isnan()  */


/****************  namespaces  ****************/


using namespace aqua::spectrum;


/****************  public functions  ****************/


Energy::Energy(double omega_peak)  /*  ωp  */
  : omega_peak(omega_peak)
{
  // must test omega_peak is not equal to zero!
}


/*  virtual  */
Energy::~Energy(void)
{
}


/****  get  ****/

double
Energy::get_omega_peak(void) const
{
  return this->omega_peak;
}


/****  Make  ****/

class Samples *
Energy::make_integral_samples(const double omega_min,
			      const double omega_max,
			      const double precision) const
{
  class Samples * samples;

  samples = new class Samples(omega_min, omega_max, precision);

  this->fill_integral_samples(*samples);

  return samples;
}


/****  Compute  ****/


/**  Compute integral  **/

/*  Computes integral from 0.0 to +∞.  */
double
Energy::compute_integral(void) const
{
  return this->compute_integral(0.0, INFINITY);
}


/****************  protected functions  ****************/


void
Energy::check_omega_range(double omega_from,
			  double omega_to) const throw (const char *)
{
  if (   (std::isnan(omega_from) != 0)
      || (std::isnan(omega_to)   != 0)
      || (omega_from < 0.0)
      || (omega_to   < 0.0)
      || (omega_from > omega_to))
    {
      throw "aqua::spectrum::Energy::check_omega_range error\n";
    }
}
