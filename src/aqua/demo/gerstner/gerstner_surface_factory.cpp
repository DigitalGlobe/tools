/*
  This file is part of “The Aqua library”.

  Copyright © 2005 2006  Jocelyn Fréchot

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


/****************  Includes  ****************/


#include "gerstner_surface_factory.h"

#include "config/config.h"

#include "gerstner_scene_context.h"

#include "scene_context.h"

/****  libaqua  ****/
#include <libaqua/aqua_ocean_gerstner.h>


/****************  Namespaces  ****************/


using namespace aqua::ocean::gerstner;


/****************  Public functions  ****************/


namespace gerstner
{


/****************  Surface_Factory  ****************/


Surface_Factory::Surface_Factory(bool adaptive)
  : adaptive(adaptive)
{
}


/*  Virtual  */
Surface_Factory::~Surface_Factory(void)
{
}


/****************  Surface_Factory_Classic  ****************/


Surface_Factory_Classic::Surface_Factory_Classic(bool adaptive)
  : Surface_Factory(adaptive)
{
}


/*  Virtual  */
Surface_Factory_Classic::~Surface_Factory_Classic(void)
{
}


class aqua::ocean::Surface *
Surface_Factory_Classic::create(const class ::Scene_Context & context) const
{
  const class gerstner::Scene_Context & context_gerstner =
    dynamic_cast<const class ::gerstner::Scene_Context &>(context);

  class classic::Surface * surface;


  if (this->adaptive)
    {
      surface =
	new class classic::Surface(config::Surface_wind_speed_file,
				   context_gerstner.get_waves_number(),
				   context.get_size_x(),
				   context.get_size_y(),
				   context.get_wind_angle(),
				   context.get_depth(),
				   context.get_displacement_factor(),
				   // context.get_smallest_wavelength(),
				   // context.get_largest_wavelength(),
				   // context.get_spectrum_factor(),
				   // context.get_wind_speed(),
				   // context.get_wind_angle(),
				   config::Surface_normalized_normals);
    }
  else
    {
      surface =
	new class classic::Surface(config::Surface_wind_speed_file,
				   context_gerstner.get_waves_number(),
				   context.get_size_x(),
				   context.get_size_y(),
				   context.get_length_x(),
				   context.get_length_y(),
				   context.get_wind_angle(),
				   context.get_depth(),
				   context.get_displacement_factor(),
				   config::Surface_normalized_normals);
    }

  return surface;
}


}  /*  namespace gerstner  */
