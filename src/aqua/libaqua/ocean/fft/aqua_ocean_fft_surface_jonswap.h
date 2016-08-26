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

/*  "class Surface_Fft" with JONSWAP spectrum  */


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_OCEAN_FFT_SURFACE_JONSWAP_H
#define AQUA_OCEAN_FFT_SURFACE_JONSWAP_H


/****************  includes  ****************/


#include <libaqua/aqua_ocean_fft_surface.h>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  class declarations  ****************/


namespace aqua
{
  namespace rng
  {
    class Rng;
  }
}


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace fft
    {

      class Surface_Jonswap : public Surface
      {
      public:

	/*  creates a surface and sets it at time 0.0  */
	Surface_Jonswap(enum type fft_type,
			size_t size_x,     /* numbers of discrete sample... */
			size_t size_y,     /* ...points, must be power of two */
			float length_x,              /*  meters  */
			float length_y,              /*  meters  */
			float depth,  /* of water, meters, 0.0 means infinite */
			float displacement_factor,   /*  horizontal  */
			float smallest_wavelength,  /*  meters  */
			float largest_wavelength,   /*  meters  */
			float spectrum_factor,
			float wind_speed,            /*  m.s-1  */
			float wind_angle,            /*  radians  */
			float fetch,                 /*  meters  */
			class aqua::rng::Rng * rng,
			bool normalized_normals,
			float loop_time = 0.0);   /* seconds, 0 means no loop */

	virtual ~Surface_Jonswap(void);


	/****  set  ****/
	void set_fetch(float fetch);  /*  meters  */

	/****  get  ****/
	float get_fetch(void) const;  /*  meters  */


      protected:


      private:

	/****  not defined  ****/
	Surface_Jonswap(const class Surface_Jonswap &);
	void operator =(const class Surface_Jonswap &);
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_FFT_SURFACE_JONSWAP_H  */
