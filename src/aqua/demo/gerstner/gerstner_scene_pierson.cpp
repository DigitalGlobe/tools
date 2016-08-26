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

/*  "Scene" with "Aqua_Surface_Pierson"  */


/****************  includes  ****************/


#include "gerstner_scene_pierson.h"

#include "config/config_scene_context.h"

#include "gerstner_help.h"
#include "gerstner_scene_context.h"
#include "gerstner_surface_factory.h"


/****************  namespaces  ****************/


using namespace config::scene_context;
using namespace gerstner;


/****************  public functions  ****************/


Scene_Pierson::Scene_Pierson(class gerstner::Surface_Factory * factory,
			     unsigned int window_width,
			     unsigned int window_height)
  : Scene(this->create_context(),
	  factory,
	  new class gerstner::Help,
	  window_width,
	  window_height)
{
}


/*  virtual  */
Scene_Pierson::~Scene_Pierson(void)
{
}


/****************  protected functions  ****************/

//  context factory?
class ::Scene_Context *
Scene_Pierson::create_context(void) const
{
  return new class gerstner::Scene_Context(Index_size_x,
					   Index_size_y,
					   Index_length_x,
					   Index_length_y,
					   Index_depth,
					   Index_displacement_factor,
					   Index_spectrum_factor_phillips,
					   Index_smallest_wavelength,
					   Index_largest_wavelength,
					   Index_wind_speed,
					   Index_wind_angle,
					   Index_surface_alpha,
					   Index_waves_number,
					   Waves_number_max);
}
