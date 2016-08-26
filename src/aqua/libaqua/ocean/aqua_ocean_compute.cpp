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

/**
   \file
   Field computations.
*/


/****************  includes  ****************/


#include "aqua_ocean_compute.h"

#include "aqua_config.h"

#include "aqua_array.h"
#include "aqua_array_1.h"

/*  local includes  */
#include "vector.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua;
using namespace aqua::ocean;


/****************  public functions  ****************/

Compute::Compute(size_t size_x,
		 size_t size_y,
		 float length_x,
		 float length_y,
		 float resolution_x,
		 float resolution_y,
		 float displacement_factor,
		 bool normalized_normals)
  : normalized_normals(normalized_normals),
    surface(new class Array<3>(size_y, size_x)),
    normals(new class Array<3>(size_y, size_x)),
    eigenvalues(new class Array<1>(size_y, size_x)),
    size_x(size_x),
    size_y(size_y),
    length_x(length_x),
    length_y(length_y),
    resolution_x(resolution_x),
    resolution_y(resolution_y),
    displacement_factor(displacement_factor),
    computed_surface(config::Computed_surface),
    computed_normals(config::Computed_normals),
    computed_eigenvalues(config::Computed_eigenvalues)
{
}



Compute::~Compute(void)
{
  delete this->eigenvalues;
  delete this->normals;
  delete this->surface;
}


/****  set  ****/


void
Compute::set_displacement_factor(float displacement_factor)
{
  this->displacement_factor = displacement_factor;
}


void
Compute::set_computed_surface(bool computed_surface)
{
  this->computed_surface = computed_surface;
}


void
Compute::set_computed_normals(bool computed_normals)
{
  this->computed_normals = computed_normals;
}


void
Compute::set_computed_eigenvalues(bool computed_eigenvalues)
{
  this->computed_eigenvalues = computed_eigenvalues;
}


/****  get  ****/

const class Array<3> &
Compute::get_surface(void) const
{
  return *this->surface;
}


const class Array<3> &
Compute::get_normals(void) const
{
  return *this->normals;
}


const class Array<1> &
Compute::get_eigenvalues(void) const
{
  return *this->eigenvalues;
}


size_t
Compute::get_size_x(void) const
{
  return this->size_x;
}


size_t
Compute::get_size_y(void) const
{
  return this->size_y;
}


float
Compute::get_length_x(void) const
{
  return this->length_x;
}


float
Compute::get_length_y(void) const
{
  return this->length_y;
}


float
Compute::get_resolution_x(void) const
{
  return this->resolution_x;
}


float
Compute::get_resolution_y(void) const
{
  return this->resolution_y;
}


float
Compute::get_displacement_factor(void) const
{
  return this->displacement_factor;
}


bool
Compute::get_computed_surface(void) const
{
  return this->computed_surface;
}


bool
Compute::get_computed_normals(void) const
{
  return this->computed_normals;
}


bool
Compute::get_computed_eigenvalues(void) const
{
  return this->computed_eigenvalues;
}


bool
Compute::get_normalized_normals(void) const
{
  return this->normalized_normals;
}


/****************  protected functions  ****************/

/****  compute  ****/

float
Compute::compute_resolution(size_t size, float length) const
{
  return length / size;
}


/****  set  ****/

void
Compute::reset(size_t size_x,
	       size_t size_y,
	       float resolution_x,
	       float resolution_y)
{
  delete this->eigenvalues;
  delete this->normals;
  delete this->surface;

  this->size_x = size_x;
  this->size_y = size_y;
  this->resolution_x = resolution_x;
  this->resolution_y = resolution_y;

  this->surface = new class Array<3>(size_y, size_x);
  this->normals = new class Array<3>(size_y, size_x);
  this->eigenvalues = new class Array<1>(size_y, size_x);
}


void
Compute::set_length(float length_x,
		    float length_y,
		    float resolution_x,
		    float resolution_y)
{
  this->length_x = length_x;
  this->length_y = length_y;
  this->resolution_x = resolution_x;
  this->resolution_y = resolution_y;
}


void
Compute::normalize_normals(size_t size_x, size_t size_y)
{
  if (this->normalized_normals)
    {
      size_t i, j;

      for (i = 0; i < size_y; i++)
	{
	  for (j = 0; j < size_x; j++)
	    {
	      aqua_math::vector_normalize<3>(this->normals->get_element(i,
									j));
	    }
	}
    }
}
