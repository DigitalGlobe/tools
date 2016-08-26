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


#ifndef AQUA_DEMO_GERSTNER_SCENE_CONTEXT_H
#define AQUA_DEMO_GERSTNER_SCENE_CONTEXT_H


/****************  includes  ****************/


#include "scene_context.h"

#include "config_array.h"


/****************  polymorphic classes  ****************/


namespace gerstner
{
  class Scene_Context : public ::Scene_Context
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
		  size_t index_surface_alpha,
		  size_t index_waves_number,
		  unsigned int waves_number_max);

    virtual ~Scene_Context(void);


    /****  set  ****/

    /*  fetch  */
    bool decrease_waves_number(void);
    bool increase_waves_number(void);

    /****  get  ****/

    unsigned int get_waves_number_max(void) const;
    unsigned int get_waves_number(void) const;


  protected:

    const unsigned int waves_number_max;

    /*  values arrays  */
    class Config_Array<const unsigned int> * const waves_number;

  
    /*  copy constructor to be defined  */
    Scene_Context(const class Scene_Context &);
  };
}


#endif  /*  AQUA_DEMO_GERSTNER_SCENE_CONTEXT_H  */
