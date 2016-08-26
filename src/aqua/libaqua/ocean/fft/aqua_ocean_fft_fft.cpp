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
   Fast Fourier transforms.
*/


/****************  includes  ****************/


#include "aqua_ocean_fft_fft.h"

#include "aqua_ocean_fft_type.h"

#include "aqua_config.h"  /*  Fftw_plan_type  */

/*  FFTW lib  */
extern "C"
{
#include <fftw3.h>
}

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::ocean::fft;


/****************  public functions  ****************/


/**
   Sets the FFT size to “size_x” × “size_y” and input range in
   [ -size_x / 2, size_x / 2 [ × [ -size_y / 2, size_y / 2 [.
   “size_x” and “size_y” should be powers of two.
*/
Fft::Fft(enum type type, size_t size_x, size_t size_y)
  : fft_type(type),
    size_x_real(this->compute_size_x_real(type, size_x)),
    size_x_complex(this->size_x_real / 2),
    size_y(size_y),
    size(this->size_x_complex * size_y),
    array(this->allocate_array(this->size)),
    plan(this->create_plan(type,
			   size_x,
			   size_y,
			   this->array,
			   aqua::config::Fftw_plan_type)),
    /*
      we should not need this: “get_output()” should not be called before
      “compute()”
    */
    part(Real)
{
}


Fft::~Fft(void)
{
  fftwf_destroy_plan(this->plan);
  fftwf_free(this->array);
}


/****  get  ****/

/*
  Defined in header.

  inline float &
  get_input(size_t index_y, size_t index_x)

  inline float
  get_output(size_t index_y, size_t index_x) const
*/


/**  Gets logical width of output array.  */
size_t
Fft::get_size_x_output(void) const
{
  return this->size_x_complex;
}


/****  compute  ****/

/**  Computes the FFT, from “part” to real numbers.  */
void
Fft::compute(enum part part)
{
  size_t i;


  /*  used by “get_output()”  */
  this->part = part;

  /*  computes FFT on “this->array”  */
  fftwf_execute(this->plan);

  /*  post process data  */
  if (this->fft_type == Real_to_real)
    {
      for (i = 1; i < this->size; i += 2)
	{
	  this->array[i][part] *= -1;
	}
    }
  else
    {
      size_t j;

      if (part == Real)
	{
	  for (i = 0; i < this->size_y; i += 2)
	    {
	      for (j = 1; j < this->size_x_complex; j += 2)
		{
		  this->array[i * this->size_x_complex + j][part] *= -1;
		}
	      for (j = 0; j < this->size_x_complex; j +=2)
		{
		  this->array[(i + 1) * this->size_x_complex + j][part] *= -1;
		}
	    }
	}
      else
	{
	  for (i = 0; i < this->size_y; i += 2)
	    {
	      for (j = 0; j < this->size_x_complex; j += 2)
		{
		  this->array[i * this->size_x_complex + j][part] *= -1;
		}
	      for (j = 1; j < this->size_x_complex; j +=2)
		{
		  this->array[(i + 1) * this->size_x_complex + j][part] *= -1;
		}
	    }
	}
    }
}


/****  operators  ****/

void
Fft::operator =(const std::vector<float> & vector)
{
  size_t vector_width;
  size_t i, j;

  if (this->fft_type == Real_to_real)
    {
      vector_width = this->size_x_real - 2;
    }
  else
    {
      vector_width = this->size_x_real;
    }

  for (i = 0; i < this->size_y; i++)
    {
      for (j = 0; j < vector_width; j++)
	{
	  reinterpret_cast<float *>(this->array)[i * this->size_x_real + j] =
	    vector[i * vector_width + j];
	}
    }
}


/****************  protected functions  ****************/


size_t
Fft::compute_size_x_real(enum type type, size_t size_x) const
{
  size_t temp;

  if (type == Real_to_real)
    {
      temp = size_x + 2;
    }
  else
    {
      temp = size_x * 2;
    }

  return temp;
}


fftwf_complex *
Fft::allocate_array(size_t size) const
{
  fftwf_complex * array;

  array =
    static_cast<fftwf_complex *>(fftwf_malloc(size * sizeof(fftwf_complex)));

  return array;
}


fftwf_plan
Fft::create_plan(enum type type,
		 size_t size_x,
		 size_t size_y,
		 fftwf_complex * array,
		 unsigned int plan_type) const
{
  /*
    Sets a forward real to complex FFT with input in
    [ 0, size_x [ × [ 0, size_y [. We take care of arranging data in
    “compute()”.
  */

  fftwf_plan plan;

  if (type == Real_to_real)
    {
      plan = fftwf_plan_dft_r2c_2d(size_y,
				   size_x,
				   reinterpret_cast<float *>(array),
				   array,
				   plan_type);
    }
  else
    {
      plan = fftwf_plan_dft_2d(size_y,
			       size_x,
			       array,
			       array,
			       FFTW_BACKWARD,
			       plan_type);
    }

  return plan;
}
