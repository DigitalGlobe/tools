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


#include "aqua_ocean_wave_set.h"

/*  local includes  */
#include "constant.h"
#include "vector.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */
#include <cmath>


/****************  namespaces  ****************/


using namespace aqua::ocean;


/****************  public functions  ****************/


Wave_Set::Wave_Set(size_t size, float depth, float loop_time)
  : basic_angular_velocity(this->compute_basic_angular_velocity(loop_time)),
    wavenumber(size),
    length(size),
    angular_velocity(size),
    amplitude(size),
    wave_vector_x(size),
    wave_vector_y(size),
    wave_vector_unit_x(size),
    wave_vector_unit_y(size),
    wave_vector_x_unit_x(size),
    wave_vector_y_unit_y(size),
    wave_vector_x_unit_y(size),
    size(size),
    depth(depth)
{
}


/*  virtual  */
Wave_Set::~Wave_Set(void)
{
}


/****  Set  ****/

void
Wave_Set::set_depth(float depth)
{
//   this->depth = depth;

//   this->fill_angular_velocity(this->wavenumber,
// 			      this->depth,
// 			      this->basic_angular_velocity);
}


void
Wave_Set::set_spectrum(const class Spectrum & spectrum)
{
//   this->fill_amplitude(spectrum,
// 		       this->wavenumber,
// 		       this->wave_vector_unit_x,
// 		       this->wave_vector_unit_y,
// 		       this->length);
}


/****  Get  ****/

float
Wave_Set::get_depth(void) const
{
  return this->depth;
}


size_t
Wave_Set::get_size(void) const
{
  return this->size;
}


const std::vector<float> &
Wave_Set::get_wavenumber(void) const
{
  return this->wavenumber;
}


const std::vector<float> &
Wave_Set::get_length(void) const
{
  return this->length;
}


const std::vector<float> &
Wave_Set::get_angular_velocity(void) const
{
  return this->angular_velocity;
}


const std::vector<float> &
Wave_Set::get_amplitude(void) const
{
  return this->amplitude;
}


const std::vector<float> &
Wave_Set::get_wave_vector_x(void) const
{
  return this->wave_vector_x;
}


const std::vector<float> &
Wave_Set::get_wave_vector_y(void) const
{
  return this->wave_vector_y;
}


const std::vector<float> &
Wave_Set::get_wave_vector_unit_x(void) const
{
  return this->wave_vector_unit_x;
}


const std::vector<float> &
Wave_Set::get_wave_vector_unit_y(void) const
{
  return this->wave_vector_unit_y;
}


const std::vector<float> &
Wave_Set::get_wave_vector_x_unit_x(void) const
{
  return this->wave_vector_x_unit_x;
}


const std::vector<float> &
Wave_Set::get_wave_vector_y_unit_y(void) const
{
  return this->wave_vector_y_unit_y;
}


const std::vector<float> &
Wave_Set::get_wave_vector_x_unit_y(void) const
{
  return this->wave_vector_x_unit_y;
}


/****************  protected functions  ****************/


/****  compute  ****/

float
Wave_Set::compute_basic_angular_velocity(float loop_time) const
{
  float basic_angular_velocity;

  if (std::isnormal(loop_time) == 0)
    {
      /*  no loop  */
      basic_angular_velocity = 0.0;
    }
  else
    {
      basic_angular_velocity = Constant_2_pi / loop_time;
    }

  return basic_angular_velocity;
}


/****  fill  ****/

/*  fills all vectors using wavenumber vectors  */
void
Wave_Set::fill_all(const std::vector<float> & wave_vector_x,
		   const std::vector<float> & wave_vector_y)
{
  float temp_vector[2];
  float temp_length;
  size_t i;


  for (i = 0; i < this->size; i++)
    {
      /*  wavenumbers  */
      temp_vector[0] = wave_vector_x[i];
      temp_vector[1] = wave_vector_y[i];
      this->wavenumber[i] = aqua_math::vector_length<2>(temp_vector);

      /*  lentgh  */
      if (std::isnormal(wavenumber[i]) == 0)
	{
	  temp_length = 0.0;
	}
      else
	{
	  temp_length = Constant_2_pi / this->wavenumber[i];
	}
      this->length[i] = temp_length;

      /*  unit vectors  */
      aqua_math::vector_normalize<2>(temp_vector, this->wavenumber[i]);
      this->wave_vector_unit_x[i] = temp_vector[0];
      this->wave_vector_unit_y[i] = temp_vector[1];

      /*  wavenumber × unit  */
      this->wave_vector_x_unit_x[i] =
	wave_vector_x[i] * this->wave_vector_unit_x[i];
      this->wave_vector_y_unit_y[i] =
	wave_vector_y[i] * this->wave_vector_unit_y[i];
      this->wave_vector_x_unit_y[i] =
	wave_vector_x[i] * this->wave_vector_unit_y[i];
    }

  /*  angular velocities  */
//   this->fill_angular_velocity(this->wavenumber,
// 			      this->depth,
// 			      this->basic_angular_velocity);

  /*  amplitudes  */
//   this->fill_amplitude(spectrum,
// 		       this->wavenumber,
// 		       this->wave_vector_unit_x,
// 		       this->wave_vector_unit_y,
// 		       this->length);
}


// void
// Wave_Set::fill_angular_velocity(const std::vector<float> & wavenumber,
// 				float depth,
// 				float basic_angular_velocity)
// {
//   size_t i;


//   for (i = 0; i < this->size; i++)
//     {
//       float temp_angular_velocity;

//       temp_angular_velocity = Constant_g * wavenumber[i];

//       if (std::isnormal(depth) != 0)
// 	{
// 	  temp_angular_velocity *= tanhf(wavenumber[i] * depth);
// 	}

//       temp_angular_velocity = sqrtf(temp_angular_velocity);

//       if (std::isnormal(basic_angular_velocity) != 0)
// 	{
// 	  temp_angular_velocity =
// 	    floorf(temp_angular_velocity / basic_angular_velocity)
// 	    * basic_angular_velocity;
// 	}

//       this->angular_velocity[i] = temp_angular_velocity;
//     }
// }


// void
// Wave_Set::fill_amplitude(const class Spectrum & spectrum,
// 			 const std::vector<float> & wavenumber,
// 			 const std::vector<float> & wave_vector_unit_x,
// 			 const std::vector<float> & wave_vector_unit_y,
// 			 const std::vector<float> & length)
// {
//   float temp_unit_vector[2];
//   size_t i;


//   for (i = 0; i < this->size; i++)
//     {
//       float temp_amplitude;

//       if (length[i] < spectrum.get_smallest_wavelength()
// 	  || length[i] > spectrum.get_largest_wavelength())
// 	{
// 	  temp_amplitude = 0.0;
// 	}
//       else
// 	{
// 	  temp_unit_vector[0] = wave_vector_unit_x[i];
// 	  temp_unit_vector[1] = wave_vector_unit_y[i];
//   	  temp_amplitude = spectrum.compute_amplitude(wavenumber[i],
//  						      temp_unit_vector);
// 	  //temp_amplitude = 0.0;
// 	}

//       this->amplitude[i] = temp_amplitude;
//     }
//   //amplitude[16 * 32 + 15] = 5.0;
// }


/****  Set  ****/

/*  Sets size and resizes all vectors.  */
void
Wave_Set::set_size(size_t size)
{
  this->wavenumber.resize(size);
  this->length.resize(size);
  this->angular_velocity.resize(size);
  this->amplitude.resize(size);
  this->wave_vector_x.resize(size);
  this->wave_vector_y.resize(size);
  this->wave_vector_unit_x.resize(size);
  this->wave_vector_unit_y.resize(size);
  this->wave_vector_x_unit_x.resize(size);
  this->wave_vector_y_unit_y.resize(size);
  this->wave_vector_x_unit_y.resize(size);

  this->size = size;
}
