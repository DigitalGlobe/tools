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
   Field computations.
*/


#ifndef AQUA_OCEAN_FFT_COMPUTE_H
#define AQUA_OCEAN_FFT_COMPUTE_H


/****************  includes  ****************/


#include "ocean/aqua_ocean_compute.h"

#include "aqua_ocean_fft_type.h"  /*  enum type  */

#include "aqua_array.h"
#include "aqua_array_1.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace fft
    {

      class Compute : public aqua::ocean::Compute
      {
      public:

	Compute(enum type fft_type,
		size_t size_x,
		size_t size_y,
		float length_x,
		float length_y,
		float displacement_factor,
		bool normalized_normals);

	virtual ~Compute(void);


	/****  set  ****/
	virtual void reset(size_t size_x, size_t size_y);
	virtual void set_length(float length_x, float length_y);
	virtual void set_displacement_factor(float displacement_factor);

	/****  get  ****/
	virtual size_t get_size_x(void) const;
	virtual size_t get_size_y(void) const;
	enum type get_fft_type(void) const;

	/****  update  ****/
	void update(const class aqua::ocean::Wave_Set & wave_set, float time);


      protected:

	const enum type fft_type;


	size_t size_logical_x;
	size_t size_logical_y;

	std::vector<float> fourier_amplitudes;

	class Fft * fft_1;
	class Fft * fft_2;
	/*  FFT used for partial derivatives */
	class Fft * fft_x_dx;
	class Fft * fft_y_dy;
	class Fft * fft_x_dy;


	size_t compute_fourier_size(enum type fft_type,
				    size_t size_logical_x,
				    size_t size_logical_y) const;
	void init_surface(float displacement_factor,
			  float resolution_x,
			  float resolution_y);
	void init_eigenvalues(float displacement_factor);

	void
	fill_fourier_amplitudes(const std::vector<float> & angular_velocity,
				const std::vector<float> & amplitude,
				const std::vector<float> & random_real,
				const std::vector<float> & random_imaginary,
				size_t size,
				enum type fft_type,
				float time);
		     
	/****  compute  ****/
	size_t compute_size_x(enum type type, size_t size_x);
	void compute_surface(const std::vector<float> & fourier_amplitudes,
			     const class aqua::ocean::Wave_Set & wave_set,
			     class Fft & fft);
	void compute_normals(const class Fft & fft_1,
			     const class Fft & fft_2);
	void compute_normals(const class Fft & fft_1,
			     const class Fft & fft_2,
			     const class Fft & fft_x_dx,
			     const class Fft & fft_y_dy,
			     const class Fft & fft_x_dy);
	/*  fills "eigenvalues"  */
	void compute_eigenvalues(const class Fft & fft_x_dx,
				 const class Fft & fft_y_dy,
				 const class Fft & fft_x_dy);

	/*  fills "heights"  */
	void compute_heights(class Array<3> & surface,
			     const std::vector<float> & fourier_amplitudes,
			     class Fft & fft);
	/*  fills "displacement"  */
	void
	compute_displacements(class Fft & fft,
			      const std::vector<float> & fourier_amplitudes,
			      const std::vector<float> & unit_vector);
	void compute_derivatives(class Fft & fft,
				 const std::vector<float> &fourier_amplitudes,
				 const std::vector<float> & wave_vector);
	void
	compute_partial_deriv(class Fft & fft,
			      const std::vector<float> & fourier_amplitudes,
			      const std::vector<float> & w_vector_times_unit);

	/****  fill_borders  ****/
	/*  copy the first line/column of "field" to the last one  */
	void fill_borders(class Array<1> & field, enum type fft_type);
	void fill_borders(class Array<3> & field, enum type fft_type);
	void fill_borders(class Array<3> & field,
			  int dimension,
			  enum type fft_type);


      private:

	/****  not defined  ****/
	Compute(const class  Compute &);
	void operator =(const class Compute &);
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_FFT_COMPUTE_H  */
