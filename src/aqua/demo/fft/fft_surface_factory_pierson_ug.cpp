/*
  This file is part of “The Aqua library”.

  Copyright © 2005  Jocelyn Fréchot

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


/****************  includes  ****************/


#include "fft_surface_factory_pierson_ug.h"

#include "config/config.h"

#include "scene_context.h"

/****  libaqua  ****/
#include <libaqua/aqua_ocean_fft.h>


/****************  namespaces  ****************/


using namespace fft;
using aqua::ocean::fft::Surface_Pierson_Ug;


/****************  public functions  ****************/


Surface_Factory_Pierson_Ug::
Surface_Factory_Pierson_Ug(enum aqua::ocean::fft::type fft_type)
  : fft_type(fft_type)
{
}


/*  virtual  */
Surface_Factory_Pierson_Ug::~Surface_Factory_Pierson_Ug(void)
{
}


class aqua::ocean::Surface *
Surface_Factory_Pierson_Ug::create(const class Scene_Context & context) const
{
  return new class Surface_Pierson_Ug(this->fft_type,
				 context.get_size_x(),
				 context.get_size_y(),
				 context.get_length_x(),
				 context.get_length_y(),
				 context.get_depth(),
				 context.get_displacement_factor(),
				 context.get_smallest_wavelength(),
				 context.get_largest_wavelength(),
				 context.get_spectrum_factor(),
				 context.get_wind_speed(),
				 context.get_wind_angle(),
				 config::Surface_normalized_normals);
}
