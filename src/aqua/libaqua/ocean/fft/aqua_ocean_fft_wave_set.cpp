/*
  This file is part of “The Aqua library”.

  Copyright © 2003 2004 2005  Jocelyn Fréchot

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


#include "aqua_ocean_fft_wave_set.h"

/*  libaqua  */
#include "rng/aqua_rng_rng.h"
#include "spectrum/aqua_spectrum_directional.h"
#include "spectrum/aqua_spectrum_energy.h"
#include "spectrum_tool/aqua_spectrum_tool_quantify_fft.h"
#include "spectrum_tool/aqua_spectrum_tool_wave_cartesian.h"

/*  local includes  */
#include "constant.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::ocean::fft;


/****************  public functions  ****************/


/**/
// use surface symmetry to reduce wave_set access?
/**/


Wave_Set::Wave_Set(const class aqua::rng::Rng * rng,
		   const class aqua::spectrum::Directional & spectrum,
		   size_t size_x,
		   size_t size_y,
		   float length_x,
		   float length_y,
		   float depth,                         /* meters */
		   float loop_time)
  : aqua::ocean::Wave_Set(size_x * size_y, depth, loop_time),  /*  sets size  */
    rng(rng),
    random_real(this->size),
    random_imaginary(this->size),
    size_x(size_x),
    size_y(size_y),
    length_x(length_x),
    length_y(length_y),
    /*  Initialized bellow.  */
    energy_ratio(0.0)
{
  /*  “energy_ratio”.  */
  this->fill_wave_vector(this->energy_ratio,
			 spectrum,
			 size_x,
			 size_y,
			 length_x,
			 length_y);

  this->fill_all(this->wave_vector_x, this->wave_vector_y);
  this->fill_random(*rng);
}


Wave_Set::~Wave_Set(void)
{
  delete this->rng;
}


/****  set  ****/

void
Wave_Set::set_size(const class aqua::spectrum::Directional & spectrum,
		   size_t size_x,
		   size_t size_y)
{
  this->size_x = size_x;
  this->size_y = size_y;

  /*  resizes  */
  this->ocean::Wave_Set::set_size(size_x * size_y);
  this->random_real.resize(this->size);
  this->random_imaginary.resize(this->size);

  /*  fills  */
  this->fill_wave_vector(this->energy_ratio,
			 spectrum,
			 size_x,
			 size_y,
			 this->length_x,
			 this->length_y);
  this->fill_all(this->wave_vector_x, this->wave_vector_y);
  this->fill_random(*this->rng);
}


void
Wave_Set::set_length(const class aqua::spectrum::Directional & spectrum,
		     float length_x,
		     float length_y)
{
  this->length_x = length_x;
  this->length_y = length_y;

  this->fill_wave_vector(this->energy_ratio,
			 spectrum,
			 this->size_x,
			 this->size_y,
			 length_x,
			 length_y);
  this->fill_all(this->wave_vector_x, this->wave_vector_y);
}


/****  get  ****/

double
Wave_Set::get_energy_ratio(void) const
{
  return this->energy_ratio;
}


const std::vector<float> &
Wave_Set::get_random_real(void) const
{
  return this->random_real;
}


const std::vector<float> &
Wave_Set::get_random_imaginary(void) const
{
  return this->random_imaginary;
}


/****************  protected functions  ****************/


/****  fill  ****/

void
Wave_Set::fill_wave_vector(double & energy_ratio,
			   const class aqua::spectrum::Directional & spectrum,
			   size_t size_x,
			   size_t size_y,
			   float length_x,
			   float length_y)
{
  class std::vector<class aqua::spectrum_tool::Wave_Cartesian> waves;

  double vector_x, vector_y;


  energy_ratio = aqua::spectrum_tool::quantify_fft(waves,
						   size_x,
						   size_y,
						   length_x,
						   length_y,
						   spectrum
						   );
						   //, 1024);
  energy_ratio /= spectrum.get_energy().compute_integral();

  for (unsigned i = 0; i < this->size; i++)
    {
      vector_x = waves[i].vector_x;
      vector_y = waves[i].vector_y;

      this->amplitude[i] = waves[i].amplitude;
      this->angular_velocity[i] =
	sqrt(Constant_g * hypot(vector_x, vector_y));
      this->wave_vector_x[i] = vector_x;
      this->wave_vector_y[i] = vector_y;
    }


//   size_t index;
//   size_t i, j;

//   for (i = 0; i < size_y; i++)
//     {
//       for (j = 0; j < size_x; j++)
// 	{
// 	  index = i * size_x + j;

// 	  this->wave_vector_x[index] =
// 	    (j - size_x / 2.0) * Constant_2_pi / length_x;
// 	  this->wave_vector_y[index] =
// 	    (i - size_y / 2.0) * Constant_2_pi / length_y;
// 	}
//     }
}


/*  determinist  */
#include "rng/aqua_rng_uniform_two_pi.h"
void
Wave_Set::fill_random(const class aqua::rng::Rng & rng)
{
  size_t i;
  const class aqua::rng::Uniform_Two_Pi rng2;

  for (i = 0; i < this->size; i++)
    {
      this->random_real[i] = rng2.get_number();
      this->random_imaginary[i] = 1.0;
    }
}


/*  non-determinist  */
// #include "vector.h"
// void
// Wave_Set::fill_random(const class aqua::rng::Rng & rng)
// {
//   size_t i;

//   double magnitude, angle;

//   for (i = 0; i < this->size; i++)
//     {
//       aqua_math::vector_cartesian_to_polar(magnitude,
// 					   angle,
// 					   rng.get_number(),
// 					   rng.get_number());
//       this->random_real[i] = angle;
//       this->random_imaginary[i] = magnitude;
//     }
// }


// void
// Wave_Set::fill_random(const class aqua::Rng & rng)
// {
//   size_t i;

//   for (i = 0; i < this->size; i++)
//     {
//       this->random_real[i] = rng.get_number();
//       this->random_imaginary[i] = rng.get_number();
//     }
// }
