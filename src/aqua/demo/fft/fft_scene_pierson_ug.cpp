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

/*  "Scene" with "Surface_PIERSON_Ug"  */


/****************  includes  ****************/


#include "fft_scene_pierson_ug.h"

#include "config/config_scene_context.h"

#include "fft_surface_factory_pierson_ug.h"

#include "help.h"
#include "scene_context.h"

/*  libaqua  */
#include <libaqua/aqua_ocean_fft.h>


/****************  namespaces  ****************/


using namespace config::scene_context;
using namespace fft;


/****************  public functions  ****************/


Scene_Pierson_Ug::Scene_Pierson_Ug(enum aqua::ocean::fft::type fft_type)
  : Scene(this->create_context(),
	  new class Surface_Factory_Pierson_Ug(fft_type),
	  new class Help)
{
}


/*  virtual  */
Scene_Pierson_Ug::~Scene_Pierson_Ug(void)
{
}


/****************  protected functions  ****************/


class Scene_Context *
Scene_Pierson_Ug::create_context(void) const
{
  return new class Scene_Context(Index_size_x,
				 Index_size_y,
				 Index_length_x,
				 Index_length_y,
				 Index_depth,
				 Index_displacement_factor,
				 Index_spectrum_factor,
				 Index_smallest_wavelength,
				 Index_largest_wavelength,
				 Index_wind_speed,
				 Index_wind_angle,
				 Index_surface_alpha);
}
