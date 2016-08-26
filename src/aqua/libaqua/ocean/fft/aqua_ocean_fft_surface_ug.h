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

/*  "class Surface" with unit gaussian distribution  */


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_OCEAN_FFT_SURFACE_UG_H
#define AQUA_OCEAN_FFT_SURFACE_UG_H


/****************  includes  ****************/


#include "aqua_ocean_fft_surface.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  class declarations  ****************/


namespace aqua
{
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

      class Surface_Ug : public Surface
      {
      public:

	/*  creates a surface and set it at time 0.0  */
	Surface_Ug(class aqua::spectrum::Directional * spectrum,
		   enum type fft_type,
		   size_t size_x,/* numbers of discrete sample... */
		   size_t size_y,/*...points, must be power of two*/
		   float length_x,              /*  meters  */
		   float length_y,              /*  meters  */
		   float depth, /* of water, meters, 0 = infinite */
		   float displacement_factor,   /*  horizontal  */
		   bool normalized_normals,
		   float loop_time = 0.0);/*seconds, 0=no loop*/

	virtual ~Surface_Ug(void);


      protected:


      private:

	/****  not defined  ****/
	Surface_Ug(const class Surface_Ug &);
	void operator =(const class Surface_Ug &);
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_FFT_SURFACE_UG_H  */
