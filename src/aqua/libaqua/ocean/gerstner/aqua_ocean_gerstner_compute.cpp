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

/**
   \file
   Field computations.
*/


/****************  Includes  ****************/


#include "aqua_ocean_gerstner_compute.h"

#include "aqua_ocean_gerstner_base_surface.h"
#include "aqua_ocean_gerstner_wave_set.h"

/*  Local includes  */
#include "vector.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  Namespaces  ****************/


using namespace aqua::ocean::gerstner;


/****************  Public functions  ****************/


/*  Non-adaptive  */
Compute::Compute(size_t size_x,
		 size_t size_y,
		 float length_x,
		 float length_y,
		 float displacement_factor,
		 bool normalized_normals)
  : aqua::ocean::Compute(size_x,
			 size_y,
			 length_x,
			 length_y,
			 this->compute_resolution(size_x, length_x),
			 this->compute_resolution(size_y, length_y),
			 displacement_factor,
			 normalized_normals),
    size(size_x * size_y),
    base_surface(new class Base_Surface(size_y, size_x)),
    derivative_x_dx(this->size),
    derivative_y_dy(this->size),
    derivative_x_dy(this->size),
    adaptive(false)
{
  this->fill_base_surface(size_x,
			  size_y,
			  this->resolution_x,
			  this->resolution_y);
}


/*  Adaptive  */
Compute::Compute(size_t size_x,
		 size_t size_y,
		 float displacement_factor,
		 bool normalized_normals)
  : aqua::ocean::Compute(size_x,
			 size_y,
			 0.0,
			 0.0,
			 0.0,
			 0.0,
			 displacement_factor,
			 normalized_normals),
    size(size_x * size_y),
    base_surface(new class Base_Surface(size_y, size_x)),
    derivative_x_dx(this->size),
    derivative_y_dy(this->size),
    derivative_x_dy(this->size),
    adaptive(true)
{
}


/*  Virtual  */
Compute::~Compute(void)
{
  delete this->base_surface;
}


/****  Get ****/

class Base_Surface &
Compute::get_base_surface(void)
{
  return *this->base_surface;
}


bool
Compute::get_adaptive(void) const
{
  return this->adaptive;
}


/****  Set  ****/

void
Compute::reset(size_t size_x, size_t size_y)
{
  float resolution_x, resolution_y;

  if (!this->adaptive)
    {
      resolution_x = this->compute_resolution(size_x, this->length_x);
      resolution_y = this->compute_resolution(size_y, this->length_y);
    }
  else
    {
      resolution_x = 0.0;
      resolution_y = 0.0;
    }

  this->aqua::ocean::Compute::reset(size_x,
				    size_y,
				    resolution_x,
				    resolution_y);

  this->size = size_x * size_y;

  delete this->base_surface;
  this->base_surface = new class Base_Surface(size_y, size_x);

  this->derivative_x_dx.resize(this->size);
  this->derivative_y_dy.resize(this->size);
  this->derivative_x_dy.resize(this->size);

  if (!this->adaptive)
    {
      this->fill_base_surface(size_x, size_y, resolution_x, resolution_y);
    }
}


void
Compute::set_length(float length_x, float length_y)
{
  if (!this->adaptive)
    {
      float resolution_x, resolution_y;

      resolution_x = this->compute_resolution(this->size_x, length_x);
      resolution_y = this->compute_resolution(this->size_y, length_y);

      this->aqua::ocean::Compute::set_length(length_x,
					     length_y,
					     resolution_x,
					     resolution_y);
      this->fill_base_surface(this->size_x,
			      this->size_y,
			      resolution_x,
			      resolution_y);
    }
}


// void
// Compute::set_adaptive(bool adaptive)
// {
//   if (this->adaptive && !adaptive)
//     {
//       this->fill_base_surface(this->size_x,
// 			      this->size_y,
// 			      this->resolution_x,
// 			      this->resolution_y);
//     }

//   this->adaptive = adaptive;
// }


/****************  Protected functions  ****************/


void
Compute::fill_base_surface(size_t size_x,
			   size_t size_y,
			   float resolution_x,
			   float resolution_y)
{
  size_t i, j;

  for (i = 0; i < size_y; i++)
    {
      for (j = 0; j < size_x; j++)
	{
	  this->base_surface->surface.get(0, i, j) = j * resolution_x;
	  this->base_surface->surface.get(1, i, j) = i * resolution_y;
	}
    }
}
