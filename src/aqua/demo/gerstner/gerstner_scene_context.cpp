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


/****************  includes  ****************/


#include "gerstner_scene_context.h"

#include "config/config_scene_context.h"

#include "config_array.h"


/****************  namespaces  ****************/


using namespace config::scene_context;


/****************  public functions  ****************/


gerstner::Scene_Context::Scene_Context(size_t index_size_x,
				       size_t index_size_y,
				       size_t index_length_x,
				       size_t index_length_y,
				       size_t index_depth,
				       size_t index_displacement_factor,
				       size_t index_spectrum_factor,
				       size_t index_smallest_wavelength,
				       size_t index_largest_wavelength,
				       size_t index_wind_speed,
				       size_t index_wind_angle,
				       size_t index_surface_alpha,
				       size_t index_waves_number,
				       unsigned int waves_number_max)
  : ::Scene_Context(index_size_x,
		    index_size_y,
		    index_length_x,
		    index_length_y,
		    index_depth,
		    index_displacement_factor,
		    index_spectrum_factor,
		    index_smallest_wavelength,
		    index_largest_wavelength,
		    index_wind_speed,
		    index_wind_angle,
		    index_surface_alpha),
  waves_number_max(waves_number_max),
  waves_number(new class Config_Array<const unsigned int>(Waves_number,
							  Waves_number_s,
							  index_waves_number))
{
}


gerstner::Scene_Context::~Scene_Context(void)
{
  delete this->waves_number;
}


/****  set  ****/

/*  waves_number  */
bool
gerstner::Scene_Context::decrease_waves_number(void)
{
  return this->waves_number->decrease();
}


bool
gerstner::Scene_Context::increase_waves_number(void)
{
  return this->waves_number->increase();
}


/****  get  ****/

unsigned int
gerstner::Scene_Context::get_waves_number_max(void) const
{
  return this->waves_number_max;
}


unsigned int
gerstner::Scene_Context::get_waves_number(void) const
{
  return this->waves_number->get();
}
