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


#include "scene_context.h"

#include "config/config_scene_context.h"

#include "config_array.h"

/*  local includes  */
#include "wind_speed.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace config::scene_context;


/****************  public functions  ****************/


Scene_Context::Scene_Context(size_t index_size_x,
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
			     size_t index_surface_alpha)
  : size_x(new class Config_Array<const size_t>(Size, Size_s, index_size_x)),
    size_y(new class Config_Array<const size_t>(Size, Size_s, index_size_y)),
    length_x(new class Config_Array<>(Length, Length_s, index_length_x)),
    length_y(new class Config_Array<>(Length, Length_s, index_length_y)),
    depth(new class Config_Array<>(Depth, Depth_s, index_depth)),
    displacement_factor(new class Config_Array<>(Displacement_factor,
						 Displacement_factor_s,
						 index_displacement_factor)),
    smallest_wavelength(new class Config_Array<>(Wavelength,
						 Wavelength_s,
						 index_smallest_wavelength)),
    largest_wavelength(new class Config_Array<>(Wavelength,
						Wavelength_s,
						index_largest_wavelength)),
    spectrum_factor(new class Config_Array<>(Spectrum_factor,
					     Spectrum_factor_s,
					     index_spectrum_factor)),
    /*  wind_speed.h  */
    wind_speed(new class Config_Array<>(Wind_speed,
					Wind_speed_s,
					index_wind_speed)),
    wind_speed_description(new class Config_Array<const char * const>
			   (Wind_speed_description,
			    Wind_speed_s,
			    index_wind_speed)),
    wind_angle(new class Config_Array<>(Wind_angle,
					Wind_angle_s,
					index_wind_angle)),
    surface_alpha(new class Config_Array<>(Surface_alpha,
					   Surface_alpha_s,
					   index_surface_alpha))
{
}


Scene_Context::~Scene_Context(void)
{
  delete this->surface_alpha;
  delete this->wind_angle;
  delete this->wind_speed_description;
  delete this->wind_speed;
  delete this->spectrum_factor;
  delete this->largest_wavelength;
  delete this->smallest_wavelength;
  delete this->displacement_factor;
  delete this->depth;
  delete this->length_y;
  delete this->length_x;
  delete this->size_y;
  delete this->size_x;
}


/****  set  ****/

/*  size  */

bool
Scene_Context::decrease_size(void)
{
  bool temp_x, temp_y;

  temp_x = this->size_x->decrease();
  temp_y = this->size_y->decrease();

  return  (temp_x || temp_y);
}


bool
Scene_Context::increase_size(void)
{
  bool temp_x, temp_y;

  temp_x = this->size_x->increase();
  temp_y = this->size_y->increase();

  return  (temp_x || temp_y);
}


bool
Scene_Context::decrease_size_x(void)
{
  return this->size_x->decrease();
}


bool
Scene_Context::increase_size_x(void)
{
  return this->size_x->increase();
}


bool
Scene_Context::decrease_size_y(void)
{
  return this->size_y->decrease();
}


bool
Scene_Context::increase_size_y(void)
{
  return this->size_y->increase();
}


/*  length  */

bool
Scene_Context::decrease_length(void)
{
  bool temp_x, temp_y;

  temp_x = this->length_x->decrease();
  temp_y = this->length_y->decrease();

  return  (temp_x || temp_y);
}


bool
Scene_Context::increase_length(void)
{
  bool temp_x, temp_y;

  temp_x = this->length_x->increase();
  temp_y = this->length_y->increase();

  return  (temp_x || temp_y);
}


bool
Scene_Context::decrease_length_x(void)
{
  return this->length_x->decrease();
}


bool
Scene_Context::increase_length_x(void)
{
  return this->length_x->increase();
}


bool
Scene_Context::decrease_length_y(void)
{
  return this->length_y->decrease();
}


bool
Scene_Context::increase_length_y(void)
{
  return this->length_y->increase();
}


/*  depth  */

bool
Scene_Context::decrease_depth(void)
{
  return this->depth->decrease();
}


bool
Scene_Context::increase_depth(void)
{
  return this->depth->increase();
}


/*  displacement_factor  */

bool
Scene_Context::decrease_displacement_factor(void)
{
  return this->displacement_factor->decrease();
}


bool
Scene_Context::increase_displacement_factor(void)
{
  return this->displacement_factor->increase();
}


/*  smallest_wavelength  */

bool
Scene_Context::decrease_smallest_wavelength(void)
{
  return this->smallest_wavelength->decrease();
}


bool
Scene_Context::increase_smallest_wavelength(void)
{
  return this->smallest_wavelength->increase();
}


/*  largest_wavelength  */

bool
Scene_Context::decrease_largest_wavelength(void)
{
  return this->largest_wavelength->decrease();
}


bool
Scene_Context::increase_largest_wavelength(void)
{
  return this->largest_wavelength->increase();
}


/*  spectrum_factor  */

bool
Scene_Context::decrease_spectrum_factor(void)
{
  return this->spectrum_factor->decrease();
}


bool
Scene_Context::increase_spectrum_factor(void)
{
  return this->spectrum_factor->increase();
}


/*  wind_speed  */

bool
Scene_Context::decrease_wind_speed(void)
{
  this->wind_speed->decrease();
  return this->wind_speed_description->decrease();
}


bool
Scene_Context::increase_wind_speed(void)
{
  this->wind_speed->increase();
  return this->wind_speed_description->increase();
}


/*  wind_angle  */

bool
Scene_Context::decrease_wind_angle(void)
{
  return this->wind_angle->decrease();
}


bool
Scene_Context::increase_wind_angle(void)
{
  return this->wind_angle->increase();
}


/*  surface_alpha  */
bool
Scene_Context::decrease_surface_alpha(void)
{
  return this->surface_alpha->decrease();
}


bool
Scene_Context::increase_surface_alpha(void)
{
  return this->surface_alpha->increase();
}


/****  get  ****/

size_t
Scene_Context::get_size_x(void) const
{
  return this->size_x->get();
}


size_t
Scene_Context::get_size_y(void) const
{
  return this->size_y->get();
}


float
Scene_Context::get_length_x(void) const
{
  return this->length_x->get();
}


float
Scene_Context::get_length_y(void) const
{
  return this->length_y->get();
}


float
Scene_Context::get_depth(void) const
{
  return this->depth->get();
}


float
Scene_Context::get_displacement_factor(void) const
{
  return this->displacement_factor->get();
}


float
Scene_Context::get_smallest_wavelength(void) const
{
  return this->smallest_wavelength->get();
}


float
Scene_Context::get_largest_wavelength(void) const
{
  return this->largest_wavelength->get();
}


float
Scene_Context::get_spectrum_factor(void) const
{
  return this->spectrum_factor->get();
}


float
Scene_Context::get_wind_speed(void) const
{
  return this->wind_speed->get();
}


size_t
Scene_Context::get_wind_beaufort_number(void) const
{
  return this->wind_speed_description->get_index();
}


const char * const
Scene_Context::get_wind_speed_description(void) const
{
  return this->wind_speed_description->get();
}


float
Scene_Context::get_wind_angle(void) const
{
  return this->wind_angle->get();
}


float
Scene_Context::get_surface_alpha(void) const
{
  return this->surface_alpha->get();
}
