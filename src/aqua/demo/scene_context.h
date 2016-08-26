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


#ifndef AQUA_DEMO_SCENE_CONTEXT_H
#define AQUA_DEMO_SCENE_CONTEXT_H


/****************  includes  ****************/


#include "config_array.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  polymorphic classes  ****************/


class Scene_Context
{
public:

  Scene_Context(size_t index_size_x,
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
		size_t index_surface_alpha);

  virtual ~Scene_Context(void);


  /****  set  ****/

  /*  size  */
  bool decrease_size(void);
  bool increase_size(void);
  bool decrease_size_x(void);
  bool increase_size_x(void);
  bool decrease_size_y(void);
  bool increase_size_y(void);
  /*  length  */
  bool decrease_length(void);
  bool increase_length(void);
  bool decrease_length_x(void);
  bool increase_length_x(void);
  bool decrease_length_y(void);
  bool increase_length_y(void);
  /*  depth  */
  bool decrease_depth(void);
  bool increase_depth(void);
  /*  displacement_factor  */
  bool decrease_displacement_factor(void);
  bool increase_displacement_factor(void);
  /*  smallest_wavelength  */
  bool decrease_smallest_wavelength(void);
  bool increase_smallest_wavelength(void);
  /*  largest_wavelength  */
  bool decrease_largest_wavelength(void);
  bool increase_largest_wavelength(void);
  /*  spectrum_factor  */
  bool decrease_spectrum_factor(void);
  bool increase_spectrum_factor(void);
  /*  wind_speed  */
  bool decrease_wind_speed(void);
  bool increase_wind_speed(void);
  /*  wind_angle  */
  bool decrease_wind_angle(void);
  bool increase_wind_angle(void);
  /*  surface_alpha  */
  bool decrease_surface_alpha(void);
  bool increase_surface_alpha(void);

  /****  get  ****/

  size_t get_size_x(void) const;
  size_t get_size_y(void) const;
  float get_length_x(void) const;
  float get_length_y(void) const;
  float get_depth(void) const;
  float get_displacement_factor(void) const;
  float get_smallest_wavelength(void) const;
  float get_largest_wavelength(void) const;
  float get_spectrum_factor(void) const;
  float get_wind_speed(void) const;
  size_t get_wind_beaufort_number(void) const;
  const char * const get_wind_speed_description(void) const;
  float get_wind_angle(void) const;
  float get_surface_alpha(void) const;


protected:

  /*  values arrays  */
  class Config_Array<const size_t> * const size_x;
  class Config_Array<const size_t> * const size_y;
  class Config_Array<> * const length_x;
  class Config_Array<> * const length_y;
  class Config_Array<> * const depth;
  class Config_Array<> * const displacement_factor;
  class Config_Array<> * const smallest_wavelength;
  class Config_Array<> * const largest_wavelength;
  class Config_Array<> * const spectrum_factor;
  // paires? <speed, description>
  class Config_Array<> * const wind_speed;
  class Config_Array<const char * const> * const wind_speed_description;
  class Config_Array<> * const wind_angle;
  class Config_Array<> * const surface_alpha;

  
  /*  copy constructor to be defined  */
  Scene_Context(const class Scene_Context &);
};


#endif  /*  AQUA_DEMO_SCENE_CONTEXT_H  */
