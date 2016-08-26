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

/*  "class Surface_Fft" with Pierson and Moskowitz spectrum  */


/****************  includes  ****************/


#include "aqua_ocean_fft_surface_pierson.h"

/*  libaqua  */
#include "spectrum/aqua_spectrum_directional_create.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::ocean::fft;


/****************  public functions  ****************/


/*  creates a surface and sets it at time 0.0  */
Surface_Pierson::Surface_Pierson(enum type fft_type,
				 size_t size_x,/* numbers of discrete sample... */
				 size_t size_y,/*...points, must be power of two*/
				 float length_x,              /*  meters  */
				 float length_y,              /*  meters  */
				 float depth, /* of water, meters, 0 = infinite */
				 float displacement_factor,   /*  horizontal  */
				 float smallest_wavelength,  /*  meters  */
				 float largest_wavelength,   /*  meters  */
				 float spectrum_factor,
				 float wind_speed,
				 float wind_angle,
				 class aqua::rng::Rng *rng,
				 bool normalized_normals,
				 float loop_time)   /* seconds, 0 means no loop */
  : Surface(spectrum::directional_create_pierson_longuet(wind_speed),
	    rng,
	    fft_type,
	    size_x,
	    size_y,
	    length_x,
	    length_y,
	    depth,
	    displacement_factor,
	    normalized_normals,
	    loop_time)
{
}


/*  virtual  */
Surface_Pierson::~Surface_Pierson(void)
{
}
