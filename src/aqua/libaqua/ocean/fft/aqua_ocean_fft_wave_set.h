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


#ifndef AQUA_OCEAN_FFT_WAVE_SET_H
#define AQUA_OCEAN_FFT_WAVE_SET_H


/****************  includes  ****************/


#include "ocean/aqua_ocean_wave_set.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  class declarations  ****************/


namespace aqua
{
  namespace rng
  {
    class Rng;
  }

  namespace spectrum
  {
    class Directional;
  }
}


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace fft
    {

      class Wave_Set : public aqua::ocean::Wave_Set
      {
      public:

	Wave_Set(const class aqua::rng::Rng * rng,
		 const class aqua::spectrum::Directional & spectrum,
		 size_t size_x,
		 size_t size_y,
		 float length_x,
		 float length_y,
		 float depth,                         /* meters */
		 float loop_time);       /* rad.s-1 */

	virtual ~Wave_Set(void);


	/****  set  ****/
	void set_size(const class aqua::spectrum::Directional & spectrum,
		      size_t size_x,
		      size_t size_y);
	void set_length(const class aqua::spectrum::Directional & spectrum,
			float length_x,
			float length_y);

	/****  get  ****/
	double get_energy_ratio(void) const;
	const std::vector<float> & get_random_real(void) const;
	const std::vector<float> & get_random_imaginary(void) const;


      protected:

	const class aqua::rng::Rng * const rng;

	std::vector<float> random_real;
	std::vector<float> random_imaginary;

	size_t size_x;
	size_t size_y;
	float length_x;
	float length_y;
	double energy_ratio;


	/****  fill  ****/
	void
	fill_wave_vector(double & energy_ratio,
			 const class aqua::spectrum::Directional & spectrum,
			 size_t size_x,
			 size_t size_y,
			 float length_x,
			 float length_y);
	void fill_random(const class aqua::rng::Rng & rng);


      private:

	/****  not defined  ****/
	Wave_Set(const class Wave_Set &);
	void operator =(const class Wave_Set &);
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_FFT_WAVE_SET_H  */
