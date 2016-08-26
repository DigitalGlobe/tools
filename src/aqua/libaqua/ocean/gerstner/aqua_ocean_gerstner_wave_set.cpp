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


/****************  Includes  ****************/


#include "aqua_ocean_gerstner_wave_set.h"

#include "spectrum_tool/aqua_spectrum_tool_quantify_file.h"
#include "spectrum_tool/aqua_spectrum_tool_wave_polar.h"
#include "spectrum_tool/aqua_spectrum_tool_wave_cartesian.h"

/*  Libaqua  */
#include "rng/aqua_rng_uniform_two_pi.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */

/*  C++ lib  */
#include <vector>


/****************  Namespaces  ****************/


using namespace aqua::ocean::gerstner;
using namespace aqua::spectrum_tool;


/****************  Public functions  ****************/


Wave_Set::Wave_Set(const char * wave_file,
		   size_t size_current,
		   float wind_angle,
		   float depth,                         /* meters */
		   float loop_time)
  : aqua::ocean::Wave_Set(size_current, depth, loop_time),
    size_current(size_current),
    wind_angle(wind_angle)
{
  /*  Fills “waves_indices” and “waves_all”.  */
  quantify_file_read(this->waves_indices, this->waves_all, wave_file);

  this->fill_wave_vector(this->waves_indices,
			 this->waves_all,
			 size_current,
			 wind_angle);
  this->fill_all(this->wave_vector_x, this->wave_vector_y);
  this->resize_and_fill_random_phase(size_current);
}


/*  Virtual  */
Wave_Set::~Wave_Set(void)
{
}


/****  Set  ****/

void
Wave_Set::set_size_current(size_t size)
{
  this->set_size(size);
  this->size_current = size;
  this->fill_wave_vector(this->waves_indices,
			 this->waves_all,
			 size,
			 this->wind_angle);
  this->fill_all(this->wave_vector_x, this->wave_vector_y);
  this->resize_and_fill_random_phase(size);
}


/****  Get  ****/

size_t
Wave_Set::get_size_current(void) const
{
  return this->size_current;
}


const std::vector<float> &
Wave_Set::get_random_phase(void) const
{
  return this->random_phase;
}


/****************  Protected functions  ****************/


/*
  Sets “size” and fills “amplitude”, “angular_velocity”,
  “wave_vector_x” and “wave_vector_y”
*/
void
Wave_Set::
fill_wave_vector(const std::vector<size_t> & waves_indices,
		 const std::vector<spectrum_tool::Wave_Polar> & waves_all,
		 const size_t size,
		 const float wind_angle)
{
  class std::vector<class spectrum_tool::Wave_Polar> waves;
  class Wave_Cartesian wave_cartesian(0.0, 0.0, 0.0);

  quantify_file_read(waves, size, waves_indices, waves_all, wind_angle);

  for (size_t i = 0; i < size; i++)
    {
      wave_cartesian = waves[i].get_cartesian_wavenumber();

      this->amplitude[i] = waves[i].amplitude;
      this->angular_velocity[i] = waves[i].omega;
      this->wave_vector_x[i] = wave_cartesian.vector_x;
      this->wave_vector_y[i] = wave_cartesian.vector_y;
    }
}


/*  Determinist  */
void
Wave_Set::resize_and_fill_random_phase(const size_t size)
{
  const size_t size_previous = this->random_phase.size();

  this->random_phase.resize(size);
  this->random_amplitude.resize(size);

  if (size > size_previous)
    {
      const class aqua::rng::Uniform_Two_Pi rng;

      for (size_t i = size_previous; i < size; i++)
	{
	  this->random_phase[i] = rng.get_number();
	  this->random_amplitude[i] = 1.0;
	}
    }
}


/*  Non-determinist  */
// #include "vector.h"
// #include "rng/aqua_rng_ugaussian.h"
// void
// Wave_Set::resize_and_fill_random_phase(const size_t size)
// {
//   const size_t size_previous = this->random_phase.size();


//   this->random_phase.resize(size);
//   this->random_amplitude.resize(size);

//   if (size > size_previous)
//     {
//       const class aqua::rng::Ugaussian rng;
//       double magnitude, angle;

//       for (size_t i = size_previous; i < size; i++)
// 	{
// 	  aqua_math::vector_cartesian_to_polar(magnitude,
// 					       angle,
// 					       rng.get_number(),
// 					       rng.get_number());
// 	  this->random_amplitude[i] = magnitude;
// 	  this->random_phase[i] = angle;
					       
// 	}
//     }
// }


const std::vector<float> & 
Wave_Set::get_random_amplitude(void) const
{
  return this->random_amplitude;
}
