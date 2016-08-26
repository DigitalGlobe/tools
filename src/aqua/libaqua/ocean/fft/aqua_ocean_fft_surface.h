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


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_OCEAN_FFT_SURFACE_H
#define AQUA_OCEAN_FFT_SURFACE_H


/****************  includes  ****************/


#include <libaqua/aqua_ocean_fft_type.h>
#include <libaqua/aqua_ocean_surface.h>

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

  namespace ocean
  {
    namespace fft
    {
      class Wave_Set;
    }
  }
}


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace fft
    {

      class Surface : public aqua::ocean::Surface
      {
      public:

	/*  creates a surface and set it at time 0.0  */
	Surface(class aqua::spectrum::Directional * spectrum,
		class aqua::rng::Rng * rng, /*  random number generator  */
		enum type fft_type,
		size_t size_x,             /* numbers of discrete sample... */
		size_t size_y,             /* ...points, must be power of two */
		float length_x,             /*  meters  */
		float length_y,             /*  meters  */
		float depth,   /* depth of water, meters (0.0 means infinite) */
		float displacement_factor, /*  horizontal displacement factor */
		bool normalized_normals,
		float loop_time = 0.0);     /*  seconds (0.0 means no loop)  */

	virtual ~Surface(void);


	/****  set  ****/
	virtual void reset(size_t size_x, size_t size_y);
	virtual void set_length(float length_x, float length_y);

	/****  get  ****/
	enum type get_fft_type(void) const;
	float get_smallest_possible_wave(void) const;
	float get_largest_possible_wave(void) const;
	double get_energy_ratio(void) const;


      protected:

	class spectrum::Directional * const spectrum;
	class Wave_Set * const wave_set_fft;

	float smallest_wavelength;  /*  meters  */
	float largest_wavelength;   /*  meters  */


	/****  compute  ****/
	float compute_smallest_wavelength(float size_x,
					  float size_y,
					  float length_x,
					  float length_y) const;
	float compute_largest_wavelength(float length_x, float length_y) const;


      private:

	/****  not defined  ****/
	Surface(const class Surface &);
	void operator =(const class Surface &);
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_FFT_SURFACE_H  */
