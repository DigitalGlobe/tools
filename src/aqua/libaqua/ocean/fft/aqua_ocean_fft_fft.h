/*  Emacs mode: -*- C++ -*-  */
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


#ifndef AQUA_OCEAN_FFT_FFT_H
#define AQUA_OCEAN_FFT_FFT_H


/****************  includes  ****************/


#include "aqua_ocean_fft_type.h"

/*  FFTW lib  */
extern "C"
{
#include <fftw3.h>
}

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  classe  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace fft
    {

      /**
	 Sets a two dimensional backward FFT. Computations can be made from
	 real to real numbers or from imaginary to real numbers.
      */
      class Fft
      {
      public:

	/****  types  ****/

	/**  Part of the numbers to get.  */
	enum part { Real = 0, Imaginary = 1 };


	/****  constructors and destructor  ****/

	/**
	   Sets the FFT size to “size_x” × “size_y” and input range in
	   [ -size_x / 2, size_x / 2 [ × [ -size_y / 2, size_y / 2 [.
	   “size_x” and “size_y” should be powers of two.
	*/
	Fft(enum type type, size_t size_x, size_t size_y);

	~Fft(void);


	/****  get  ****/

	/**  Sets input data. Doing so will alter output data.  */
	float & get_input(size_t index_y, size_t index_x);      /*  inline  */
	/**  Gets output data.  */
	float get_output(size_t index_y, size_t index_x) const; /*  inline  */

	/**  Gets logical width of output array.  */
	size_t get_size_x_output(void) const;


	/****  compute  ****/

	/**  Computes the FFT, from “part” to real numbers.  */
	void compute(enum part part);

	/****  operators  ****/

	void operator =(const std::vector<float> & vector);


      protected:

	const enum type fft_type;
	const size_t size_x_real;     /*  logical width of the input array  */
	const size_t size_x_complex;  /*  logical width of the output array */
	const size_t size_y;
	const size_t size;            /*  physical size of “array”  */
	fftwf_complex * const array;  /*  FFTW array used for the FFT  */
	const fftwf_plan plan;        /*  FFTW plan used for the FFT   */
	/* type of input data used with the last computed FFT */
	mutable enum part part;


	size_t compute_size_x_real(enum type type, size_t size_x) const;
	fftwf_complex * allocate_array(size_t size) const;
	fftwf_plan create_plan(enum type type,
			       size_t size_x,
			       size_t size_y,
			       fftwf_complex *array,
			       unsigned int plan_type) const;


      private:

	/****  not defined  ****/
	Fft(const class Fft &);
	void operator =(const class Fft &);
      };
    }
  }
}


/****************  inline functions  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace fft
    {

      /**  Sets input data. Doing so will alter output data.  */
      inline float &
      Fft::get_input(size_t index_y, size_t index_x)
      {
	return
	  reinterpret_cast<float *>(this->array)[index_y * this->size_x_real
						 + index_x];
      }


      /**  Gets output data.  */
      inline float
      Fft::get_output(size_t index_y, size_t index_x) const
      {
	return
	  this->array[index_y * this->size_x_complex + index_x][this->part];
      }

    }
  }
}


#endif  /*  AQUA_OCEAN_FFT_FFT_H  */
