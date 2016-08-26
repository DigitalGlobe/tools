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


/****************  includes  ****************/


#include "aqua_ocean_fft_surface_ug.h"

/*  libaqua  */
#include "rng/aqua_rng_ugaussian.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua;
using namespace aqua::ocean::fft;


/****************  public functions  ****************/


Surface_Ug::Surface_Ug(class spectrum::Directional * spectrum,
		       enum type fft_type,
		       size_t size_x,/* numbers of discrete sample... */
		       size_t size_y,/*...points, must be power of two*/
		       float length_x,              /*  meters  */
		       float length_y,              /*  meters  */
		       float depth, /* of water, meters, 0 = infinite */
		       float displacement_factor,   /*  horizontal  */
		       bool normalized_normals,
		       float loop_time)   /* seconds, 0 means no loop */
  : Surface(spectrum,
	    new class rng::Ugaussian,
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
Surface_Ug::~Surface_Ug(void)
{
}
