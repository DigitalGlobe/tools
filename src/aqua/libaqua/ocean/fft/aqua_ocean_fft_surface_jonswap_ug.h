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

/*  "class Surface_Fft_Jonswap" with unit gaussian distribution  */


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_OCEAN_FFT_SURFACE_JONSWAP_UG_H
#define AQUA_OCEAN_FFT_SURFACE_JONSWAP_UG_H


/****************  includes  ****************/


#include <libaqua/aqua_ocean_fft_surface_jonswap.h>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace fft
    {

      class Surface_Jonswap_Ug : public Surface_Jonswap
      {
      public:

	/*  creates a surface and set it at time 0.0  */
	Surface_Jonswap_Ug(enum type fft_type,
			   size_t size_x,/*numbers of discrete sample..*/
			   size_t size_y,/*points, must be power of two*/
			   float length_x,              /*  meters  */
			   float length_y,              /*  meters  */
			   float depth,/*of water, meters, 0 = infinite*/
			   float displacement_factor,   /*  horizontal */
			   float smallest_wavelength,  /*  meters  */
			   float largest_wavelength,   /*  meters  */
			   float spectrum_factor,
			   float wind_speed,            /*  m.s-1  */
			   float wind_angle,            /*  radians  */
			   float fetch,                 /*  meters  */
			   bool normalized_normals,
			   float loop_time = 0.0);/*seconds, 0= no loop*/

	virtual ~Surface_Jonswap_Ug(void);


      protected:


	/****  not defined  ****/
	Surface_Jonswap_Ug(const class Surface_Jonswap_Ug &);
	void operator =(const class Surface_Jonswap_Ug &);
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_FFT_SURFACE_JONSWAP_UG_H  */
