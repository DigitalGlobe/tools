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


#include "aqua_spectrum_samples.h"

/*  C++ lib  */
#include <valarray>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::spectrum;


/****************  public functions  ****************/


Samples::Samples(const double value_min,
		 const double value_max,
		 const double precision)
  : value_min(value_min),
    value_max(value_max),
    precision(precision),
    samples(this->compute_samples_size(this->value_min,
				       this->value_max,
				       this->precision))
{
}


/****  operators  ****/

double
Samples::operator [](size_t i) const
{
  return this->samples[i];
}


double &
Samples::operator [](size_t i)
{
  return this->samples[i];
}


/****  get  ****/

const class std::valarray<double> &
Samples::get_valarray(void) const
{
  return this->samples;
}


class std::valarray<double> &
Samples::get_valarray(void)
{
  return this->samples;
}


/****************  protected functions  ****************/


size_t
Samples::compute_samples_size(const double value_min,
			      const double value_max,
			      const double precision) const
{
  return static_cast<size_t>((value_max - value_min) / precision) + 1;
}
