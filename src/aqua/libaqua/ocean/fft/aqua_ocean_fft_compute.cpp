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

/**
   \file
   Field computations.
*/


/****************  includes  ****************/


#include "aqua_ocean_fft_compute.h"

#include "aqua_ocean_fft_fft.h"
#include "aqua_ocean_fft_type.h"
#include "aqua_ocean_fft_wave_set.h"

/*  libaqua  */
#include "aqua_array.h"
#include "aqua_array_1.h"

/*  local includes  */
#include "constant.h"
#include "vector.h"
#include "sincos.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::ocean::fft;


/****************  public functions  ****************/


Compute::Compute(enum type fft_type,
		 size_t size_x,
		 size_t size_y,
		 float length_x,
		 float length_y,
		 float displacement_factor,
		 bool normalized_normals)
  : aqua::ocean::Compute(this->compute_size_x(fft_type, size_x),
			 size_y + 1,
			 length_x,
			 length_y,
			 this->compute_resolution(size_x, length_x),
			 this->compute_resolution(size_y, length_y),
			 displacement_factor,
			 normalized_normals),
    fft_type(fft_type),
    size_logical_x(size_x),
    size_logical_y(size_y),
    fourier_amplitudes(this->compute_fourier_size(fft_type,
						  this->size_logical_x,
						  this->size_logical_y)),
    fft_1(new class Fft(this->fft_type, size_x, size_y)),
    fft_2(new class Fft(this->fft_type, size_x, size_y)),
    fft_x_dx(new class Fft(this->fft_type, size_x, size_y)),
    fft_y_dy(new class Fft(this->fft_type, size_x, size_y)),
    fft_x_dy(new class Fft(this->fft_type, size_x, size_y))
{
  this->init_surface(displacement_factor, resolution_x, resolution_y);
  this->init_eigenvalues(displacement_factor);
}



Compute::~Compute(void)
{
  delete this->fft_x_dy;
  delete this->fft_y_dy;
  delete this->fft_x_dx;
  delete this->fft_2;
  delete this->fft_1;
}


/****  set  ****/


void
Compute::reset(size_t size_x, size_t size_y)
{
  float resolution_x, resolution_y;


  resolution_x = this->compute_resolution(size_x, this->length_x);
  resolution_y = this->compute_resolution(size_y, this->length_y);


  delete this->fft_x_dy;
  delete this->fft_y_dy;
  delete this->fft_x_dx;
  delete this->fft_2;
  delete this->fft_1;

  this->aqua::ocean::Compute::reset(this->compute_size_x(this->fft_type, size_x),
				    size_y + 1,
				    resolution_x,
				    resolution_y);
  this->size_logical_x = size_x;
  this->size_logical_y = size_y;

  this->fourier_amplitudes
    .resize(this->compute_fourier_size(this->fft_type,
				       this->size_logical_x,
				       this->size_logical_y));
  this->fft_1 = new class Fft(this->fft_type, size_x, size_y);
  this->fft_2 = new class Fft(this->fft_type, size_x, size_y);
  this->fft_x_dx = new class Fft(this->fft_type, size_x, size_y);
  this->fft_y_dy = new class Fft(this->fft_type, size_x, size_y);
  this->fft_x_dy = new class Fft(this->fft_type, size_x, size_y);
  this->init_surface(this->displacement_factor, resolution_x, resolution_y);
  this->init_eigenvalues(this->displacement_factor);
}


void
Compute::set_length(float length_x, float length_y)
{
  float resolution_x, resolution_y;


  resolution_x = this->compute_resolution(this->size_logical_x, length_x);
  resolution_y = this->compute_resolution(this->size_logical_y, length_y);


  this->aqua::ocean::Compute::set_length(length_x,
					 length_y,
					 resolution_x,
					 resolution_y);

  this->init_surface(this->displacement_factor, resolution_x, resolution_y);
}


void
Compute::set_displacement_factor(float displacement_factor)
{
  this->aqua::ocean::Compute::set_displacement_factor(displacement_factor);

  this->init_surface(displacement_factor,
		     this->resolution_x,
		     this->resolution_y);
  this->init_eigenvalues(displacement_factor);
}


/****  get  ****/

size_t
Compute::get_size_x(void) const
{
  return this->size_logical_x;
}


size_t
Compute::get_size_y(void) const
{
  return this->size_logical_y;
}

enum type
Compute::get_fft_type(void) const
{
  return this->fft_type;
}


/****  update  ****/


void
Compute::update(const class aqua::ocean::Wave_Set & wave_set, float time)
{
  if(   this->computed_surface
	|| this->computed_normals
	|| this->computed_eigenvalues)
    {
      const class Wave_Set & wave_set_fft =
	dynamic_cast<const class Wave_Set &>(wave_set);

      this->fill_fourier_amplitudes(wave_set.get_angular_velocity(),
				    wave_set.get_amplitude(),
				    wave_set_fft.get_random_real(),
				    wave_set_fft.get_random_imaginary(),
				    wave_set.get_size(),
				    this->fft_type,
				    time);
    }

  if (this->computed_surface)
    {
      this->compute_surface(this->fourier_amplitudes,
			    wave_set,
			    *this->fft_1);
    }

  if (this->computed_normals || this->computed_eigenvalues)
    {
      if (this->computed_normals)
	{
	  /*  sets “fft_1” and “fft_2”  */
	  this->compute_derivatives(*this->fft_1,
				    this->fourier_amplitudes,
				    wave_set.get_wave_vector_x());
	  this->compute_derivatives(*this->fft_2,
				    this->fourier_amplitudes,
				    wave_set.get_wave_vector_y());
	}

      if (std::isnormal(this->displacement_factor) != 0)
	{
	  /*  sets “fft_x_dx”, “fft_y_dy” and “fft_x_dy”  */
	  this->compute_partial_deriv(*this->fft_x_dx,
				      this->fourier_amplitudes,
				      wave_set.get_wave_vector_x_unit_x());
	  this->compute_partial_deriv(*this->fft_y_dy,
				      this->fourier_amplitudes,
				      wave_set.get_wave_vector_y_unit_y());
	  this->compute_partial_deriv(*this->fft_x_dy,
				      this->fourier_amplitudes,
				      wave_set.get_wave_vector_x_unit_y());

	  if (this->computed_normals)
	    {
	      this->compute_normals(*this->fft_1,
				    *this->fft_2,
				    *this->fft_x_dx,
				    *this->fft_y_dy,
				    *this->fft_x_dy);
	    }
	  if (this->computed_eigenvalues)
	    {
	      this->compute_eigenvalues(*this->fft_x_dx,
					*this->fft_y_dy,
					*this->fft_x_dy);
	    }
	}
      else
	{
	  /*  eigenvalues had already been set to 1  */
	  if (this->computed_normals)
	    {
	      this->compute_normals(*this->fft_1, *this->fft_2);
	    }
	}
    }
}


/****************  protected functions  ****************/


size_t
Compute::compute_fourier_size(enum type fft_type,
			      size_t size_logical_x,
			      size_t size_logical_y) const
{
  size_t temp;

  if (fft_type == Real_to_real)
    {
      temp = size_logical_x * size_logical_y;
    }
  else
    {
      temp = size_logical_x * size_logical_y * 2;
    }

  return temp;
}


void
Compute::init_surface(float displacement_factor,
		      float resolution_x,
		      float resolution_y)
{
  /*
    if we don't use displacement, sets X and Z values of "surface" and doesn't
    recompute them in "get_surface()"
  */
  if (std::isnormal(displacement_factor) == 0)
    {
      size_t i, j;

      for (i = 0; i < this->size_y; i++)
	{
	  for (j = 0; j < this->size_x; j++)
	    {
	      this->surface->get(0, i, j) = j * resolution_x;
	      this->surface->get(1, i, j) = i * resolution_y;
	    }
	}
    }
}


void
Compute::init_eigenvalues(float displacement_factor)
{
  if (std::isnormal(displacement_factor) == 0)
    {
      size_t i;

      for (i = 0; i < this->size_x * this->size_logical_y; i++)
	{
	  this->eigenvalues->get(i) = 1.0;
	}

      this->fill_borders(*this->eigenvalues, this->fft_type);
    }
}


void
Compute::fill_fourier_amplitudes(const std::vector<float> &angular_velocity,
				 const std::vector<float> & amplitude,
				 const std::vector<float> & random_real,
				 const std::vector<float> &random_imaginary,
				 size_t size,
				 enum type fft_type,
				 float time)
{
  size_t i;

  float temp_phase, temp_sin, temp_cos;

  for (i = 0; i < size; i++)
    {
      temp_phase = -angular_velocity[i] * time + random_real[i];

      aqua_math::sincosf(temp_phase, &temp_sin, &temp_cos);

      if (fft_type == Real_to_real)
	{
// 	  this->fourier_amplitudes[i] =
// 	    amplitude[i]
// 	    / Constant_sqrt2
// 	    * (random_real[i] * temp_cos - random_imaginary[i] * temp_sin);
	  this->fourier_amplitudes[i] =
	    amplitude[i] * random_imaginary[i] * temp_cos;
	}
      else
	{
// 	  this->fourier_amplitudes[i * 2] =
// 	    amplitude[i] / Constant_sqrt2 * random_real[i] * temp_cos;

// 	  this->fourier_amplitudes[i * 2 + 1] =
// 	    amplitude[i] / Constant_sqrt2 * random_real[i] * temp_sin;


 	  this->fourier_amplitudes[i * 2] =
	    amplitude[i] * random_imaginary[i] * temp_cos;

 	  this->fourier_amplitudes[i * 2 + 1] =
	    amplitude[i] * random_imaginary[i]  * temp_sin;
	}
    }
}


void
Compute::compute_surface(const std::vector<float> & fourier_amplitudes,
			 const class aqua::ocean::Wave_Set & wave_set,
			 class Fft & fft)
{
  this->compute_heights(*this->surface, fourier_amplitudes, fft);

  /*  doesn't compute horizontal coordinates if there is no displacement  */
  if (std::isnormal(this->displacement_factor) == 0)
    {
      this->fill_borders(*this->surface, 2, this->fft_type);  /*  heights  */
    }
  else
    {
      size_t i, j;

      this->compute_displacements(fft,
				  fourier_amplitudes,
				  wave_set.get_wave_vector_unit_x());
      for (i = 0; i < this->size_logical_y; i++)
	{ 
	  for (j = 0; j < this->size_x; j++)
	    {
	      this->surface->get(0, i, j) = fft.get_output(i, j);
	    }
	}

      this->compute_displacements(fft,
				  fourier_amplitudes,
				  wave_set.get_wave_vector_unit_y());
      for (i = 0; i < this->size_logical_y; i++)
	{ 
	  for (j = 0; j < this->size_x; j++)
	    {
	      this->surface->get(1, i, j) = fft.get_output(i, j);
	    }
	}


      this->fill_borders(*this->surface, this->fft_type);
      // size_x + 1
      for (i = 0; i < this->size_y; i++)
	{
	  for (j = 0; j < this->size_x; j++)
	    {
	      this->surface->get(0, i, j) =
		this->surface->get(0, i, j) * this->displacement_factor
		+ j * this->resolution_x;
	      this->surface->get(1, i, j) =
		this->surface->get(1, i, j) * this->displacement_factor
		+ i * this->resolution_y;
	    }
	}
    }
}


void
Compute::compute_normals(const class Fft & fft_1, const class Fft & fft_2)
{
  size_t i, j;

  for (i = 0; i < this->size_logical_y; i++)
    { 
      for (j = 0; j < this->size_x; j++)
	//for (j = 0; j < this->size_x - 1; j++)
	{
	  this->normals->get(0, i, j) = fft_1.get_output(i, j);
	  this->normals->get(1, i, j) = fft_2.get_output(i, j);
	  this->normals->get(2, i, j) = 1.0;
	}
    }

  this->normalize_normals(this->size_x, this->size_logical_y);

  this->fill_borders(*this->normals, this->fft_type);
}


void
Compute::compute_normals(const class Fft & fft_1,
			 const class Fft & fft_2,
			 const class Fft & fft_x_dx,
			 const class Fft & fft_y_dy,
			 const class Fft & fft_x_dy)
{
  float temp_x_dx, temp_x_dy, temp_x_dz, temp_y_dy, temp_y_dz;
  size_t i, j;
      
  for (i = 0; i < this->size_logical_y; i++)
    {
      for (j = 0; j < this->size_x; j++)
	{
	  temp_x_dx =
	    1
	    + this->displacement_factor
	    * fft_x_dx.get_output(i, j);
	  temp_x_dy =
	    this->displacement_factor * fft_x_dy.get_output(i, j);
	  temp_x_dz = - fft_1.get_output(i, j);
	  /*  y_dx == x_dy  */
	  temp_y_dy =
	    1
	    + this->displacement_factor
	    * fft_y_dy.get_output(i, j);
	  temp_y_dz = - fft_2.get_output(i, j);
	  
	  aqua_math::vector_product(temp_x_dx,
				    temp_x_dy,
				    temp_x_dz,
				    temp_x_dy,
				    temp_y_dy,
				    temp_y_dz,
				    this->normals->get_element(i, j));
	}
    }

  this->normalize_normals(this->size_x, this->size_logical_x);

  this->fill_borders(*this->normals, this->fft_type);
}


/*  fills "eigenvalues"  */
void
Compute::compute_eigenvalues(const class Fft &fft_x_dx,
			     const class Fft &fft_y_dy,
			     const class Fft &fft_x_dy)
{
  size_t i, j;

  for (i = 0; i < this->size_logical_y; i++)
    {
      for (j = 0; j < this->size_x; j++)
	{
	  this->eigenvalues->get(i, j) =
	    1.0
	    + (this->displacement_factor
	       * (fft_x_dx.get_output(i, j)
		  + fft_y_dy.get_output(i, j)
		  + (this->displacement_factor
		     * ((fft_x_dx.get_output(i, j)
			 * fft_y_dy.get_output(i, j))
			- (fft_x_dy.get_output(i, j)
			   * fft_x_dy.get_output(i, j))))));
	}
    }

  this->fill_borders(*this->eigenvalues, this->fft_type);
}


/****  compute  ****/

size_t
Compute::compute_size_x(enum type type, size_t size_x)
{
  size_t temp;

  if (type == Real_to_real)
    {
      temp = size_x / 2 + 1;
    }
  else
    {
      temp = size_x + 1;
    }

  return temp;
}


void
Compute::compute_heights(class Array<3> & surface,
			 const std::vector<float> & fourier_amplitudes,
			 class Fft & fft)
{
  size_t i, j;

  //   for (i = 0; i < this->size_logical_x; i++)
  //     {
  //       for (j = 0; j < this->size_logical_y; j++)
  // 	{
  // 	  fft.get_input(i, j) = fourier_amplitudes[i * this->size_logical_y +j];
  // 	}
  //     }

  size_t temp_size_x;

  if (this->fft_type == Real_to_real)
    {
      temp_size_x = this->size_x;
    }
  else
    {
      temp_size_x = this->size_x - 1;
    }


  fft = fourier_amplitudes;

  fft.compute(Fft::Real);

  for (i = 0; i < this->size_logical_y; i++)
    {
      for (j = 0; j < temp_size_x; j++)
	{
	  surface.get(2, i, j) = fft.get_output(i, j);
	}
    }
}


/*  fills "displacement"  */
void
Compute::compute_displacements(class Fft & fft,
			       const std::vector<float> &fourier_amplitudes,
			       const std::vector<float> & wave_vector_unit)
{
  //  imbriquer les boucles

  size_t i, j;

  /*  fills displacements  */

  if (this->fft_type == Real_to_real)
    {
      for (i = 0; i < this->size_logical_y; i++)
	{
	  for (j = 0; j < this->size_logical_x; j++)
	    {
	      fft.get_input(i, j) =
		wave_vector_unit[i * this->size_logical_x + j]
		* fourier_amplitudes[i * this->size_logical_x + j];
	    }
	}
    }
  //fft = wave_vector_unit * fourier_amplitudes;
  else
    {
      for (i = 0; i < this->size_logical_y; i++)
	{
	  for (j = 0; j < this->size_logical_x; j++)
	    {
	      fft.get_input(i, j * 2) =
		wave_vector_unit[i * this->size_logical_x + j]
		* fourier_amplitudes[(i * this->size_logical_x + j) * 2];
	      fft.get_input(i, j * 2 + 1) =
		wave_vector_unit[i * this->size_logical_x + j]
		* fourier_amplitudes[(i * this->size_logical_x + j) * 2 + 1];
	    }
	}
    }

  fft.compute(Fft::Imaginary);
}


void
Compute::compute_derivatives(class Fft & fft,
			     const std::vector<float> & fourier_amplitudes,
			     const std::vector<float> & wave_vector)
{
  size_t i, j;

  /*  fills slopes  */
  if (this->fft_type == Real_to_real)
    {
      for (i = 0; i < this->size_logical_y; i++)
	{
	  for (j = 0; j < this->size_logical_x; j++)
	    {
	      fft.get_input(i, j) =
		- wave_vector[i * this->size_logical_x + j]
		* fourier_amplitudes[i * this->size_logical_x + j];
	    }
	}
    }
  //  fft = (- wave_vector * fourier_amplitudes);
  else
    {
      for (i = 0; i < this->size_logical_y; i++)
	{
	  for (j = 0; j < this->size_logical_x; j++)
	    {
	      fft.get_input(i, j * 2) =
		- wave_vector[i * this->size_logical_x + j]
		* fourier_amplitudes[(i * this->size_logical_x + j) * 2];
	      fft.get_input(i, j * 2 + 1) =
		- wave_vector[i * this->size_logical_x + j]
		* fourier_amplitudes[(i * this->size_logical_x + j) * 2 + 1];
	    }
	}
    }
  fft.compute(Fft::Imaginary);
}


void
Compute::
compute_partial_deriv(class Fft & fft,
		      const std::vector<float> & fourier_amplitudes,
		      const std::vector<float> & w_vector_times_unit)
{
  size_t i, j;

  if (this->fft_type == Real_to_real)
    {
      for (i = 0; i < this->size_logical_y; i++)
	{
	  for (j = 0; j < this->size_logical_x; j++)
	    {
	      fft.get_input(i, j) =
		- w_vector_times_unit[i * this->size_logical_x + j]
		* fourier_amplitudes[i * this->size_logical_x + j];
	    }
	}
    }
  //  fft = (- w_vector_times_unit * fourier_amplitudes);
  else
    {
      for (i = 0; i < this->size_logical_y; i++)
	{
	  for (j = 0; j < this->size_logical_x; j++)
	    {
	      fft.get_input(i, j * 2) =
		- w_vector_times_unit[i * this->size_logical_x + j]
		* fourier_amplitudes[(i * this->size_logical_x + j) * 2];
	      fft.get_input(i, j * 2 + 1) =
		- w_vector_times_unit[i * this->size_logical_x + j]
		* fourier_amplitudes[(i * this->size_logical_x + j) * 2 + 1];
	    }
	}
    }

  fft.compute(Fft::Real);
}


/****  fill_borders  ****/

/*  “memcpy()” does not seem faster  */


/*  copy the first line/column of "field" to the last one  */
void
Compute::fill_borders(class Array<1> & field, enum type fft_type)
{
  for (size_t j = 0; j < this->size_x; j++)
    {
      field.get(this->size_logical_y, j) = field.get(0, j);
    }

  if (fft_type == Complex_to_real)
    {
      for (size_t i = 0; i < this->size_y; i++)
	{
	  field.get(i, this->size_logical_x) = field.get(i, 0);
	}
    }
}


void
Compute::fill_borders(class Array<3> & field, enum type fft_type)
{
  for (size_t j = 0; j < this->size_x; j++)
    {
      for (size_t k = 0; k < 3; k++)
	{
	  field.get(k, this->size_logical_y, j) = field.get(k, 0, j);
	}
    }
  /**/
  if (fft_type == Complex_to_real)
    {
      for (size_t i = 0; i < this->size_y; i++)
	{
	  for (size_t k = 0; k < 3; k++)
	    {
	      field.get(k, i, this->size_logical_x) = field.get(k, i, 0);
	    }
	}
    }
}


void
Compute::fill_borders(class Array<3> & field,
		      int dimension,
		      enum type fft_type)
{
  for (size_t j = 0; j < this->size_x; j++)
    {
      field.get(dimension, this->size_logical_y, j) =
	field.get(dimension, 0, j);
    }

  if (fft_type == Complex_to_real)
    {
      for (size_t i = 0; i < this->size_y; i++)
	{
	  field.get(dimension, i, this->size_logical_x) =
	    field.get(dimension, i, 0);
	}
    }
}
