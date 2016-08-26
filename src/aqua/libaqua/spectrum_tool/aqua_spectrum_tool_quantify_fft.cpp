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


#include "aqua_spectrum_tool_quantify_fft.h"

#include "aqua_spectrum_tool_sum_four_neighbours.h"
#include "aqua_spectrum_tool_wave_cartesian.h"

#include "spectrum/aqua_spectrum_directional.h"

/*  local includes  */
#include "constant.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cmath>
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::spectrum_tool;


/****************  static function prototypes  ****************/


namespace
{
  double compute_energy(const class std::vector<double> & integral,
			unsigned integral_width,
			unsigned n,
			unsigned m,
			unsigned samples_by_unit_square_x,
			unsigned samples_by_unit_square_y,
			double coefficient);
}


/****************  functions  ****************/


double
aqua::spectrum_tool::
quantify_fft(class std::vector<class Wave_Cartesian> & waves,
	     const size_t size_x,
	     const size_t size_y,
	     const double length_x,
	     const double length_y,
	     const class spectrum::Directional & spectrum,
	     const size_t size_min)
{
  /*  “samples” is the maximum amongst “size_x”, “size_y” and “size_min”.  */
  const unsigned int samples =
    static_cast<size_t>(fmaxf(fmaxf(size_x, size_y), size_min));

  const double two_pi_by_l_x = Constant_2_pi / length_x;
  const double two_pi_by_l_y = Constant_2_pi / length_y;

  const unsigned int samples_by_unit_square_x = samples / size_x;
  const unsigned int samples_by_unit_square_y = samples / size_y;

  const double coefficient
    = size_x * two_pi_by_l_x / samples
    * size_y * two_pi_by_l_y / samples
    / 4.0;

  const unsigned int origin_n = samples / 2 + samples_by_unit_square_y / 2;
  const unsigned int origin_m = samples / 2 + samples_by_unit_square_x / 2;

  const size_t integral_width = samples + 1;

  class std::vector<double> integral(integral_width * integral_width);

  double amplitude, vector_x, vector_y;
  double energy;
  double temp_energy;


  /****  Fills “integral”.  ****/

  /*  κx = 2π n ∕ Lx,  κy = 2π m ∕ Ly  */
  for (size_t n = 0; n < integral_width; n++)
    {
      vector_y =
	two_pi_by_l_y
	* (static_cast<int>(n) - static_cast<int>(origin_n))
	/ samples_by_unit_square_y;

      // on recalcule vector_x pour chaque valeur de vector_y...
      for (size_t m = 0; m < integral_width; m++)
	{
	  vector_x =
	    two_pi_by_l_x
	    * (static_cast<int>(m) - static_cast<int>(origin_m))
	    / samples_by_unit_square_x;

	  integral[n * integral_width + m] =
	    spectrum.compute_cartesian_wavenumber(vector_x, vector_y);
	}
    }

  /****  Fills “waves”.  ****/

  waves.clear();

  energy = 0.0;

  for (size_t n = 0; n < size_y; n++)
    {
      //  no cast needed here
      vector_y = two_pi_by_l_y * (n - size_y / 2.0);

      for (size_t m = 0; m < size_x; m++)
	{
	  vector_x = two_pi_by_l_x * (m - size_x / 2.0);

	  temp_energy = compute_energy(integral,
				       integral_width,
				       n,
				       m,
				       samples_by_unit_square_x,
				       samples_by_unit_square_y,
				       coefficient);
	  energy += temp_energy;
	  amplitude = sqrt(2.0 * temp_energy);

	  waves.push_back(Wave_Cartesian(amplitude, vector_x, vector_y));
	}
    }

  return energy;
}


/****************  static functions  ****************/


namespace
{


double
compute_energy(const class std::vector<double> & integral,
	       unsigned integral_width,
	       unsigned n,
	       unsigned m,
	       unsigned samples_by_unit_square_x,
	       unsigned samples_by_unit_square_y,
	       double coefficient)
{
  double energy;
  unsigned index_x, index_y;

  energy = 0;

  for (unsigned i = 0; i < samples_by_unit_square_y; i++)
    {
      index_y =	n * samples_by_unit_square_y + i;

      for (unsigned j = 0; j < samples_by_unit_square_x; j++)
	{
	  index_x = m * samples_by_unit_square_x + j;

	  energy +=
 	    sum_four_neighbours(integral, integral_width, index_x, index_y);
	}
    }

  return energy * coefficient;
}


}
