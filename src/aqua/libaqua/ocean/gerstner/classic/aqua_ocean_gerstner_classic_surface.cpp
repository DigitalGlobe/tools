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


/****************  Includes  ****************/


#include "aqua_ocean_gerstner_classic_surface.h"

#include "aqua_ocean_gerstner_classic_compute.h"

#include "aqua_ocean_gerstner_wave_set.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  Namespaces  ****************/


using namespace aqua::ocean::gerstner::classic;


/****************  Public functions  ****************/


/*  creates a surface and sets it at time 0.0  */
/*  Non-adaptive  */
Surface::Surface(const char * wave_file,
		 size_t waves_number_current,
		 size_t size_x,
		 size_t size_y,
		 float length_x,
		 float length_y,
		 float wind_angle,
		 float depth,
		 float displacement_factor,
		 bool normalized_normals,
		 float loop_time)
  : aqua::ocean::gerstner::Surface(wave_file,
				   new class classic::Compute(size_x,
							      size_y,
							      length_x,
							      length_y,
							      displacement_factor,
							      normalized_normals),
				   waves_number_current,
				   wind_angle,
				   depth,
				   loop_time),
    compute_classic(dynamic_cast<class classic::Compute *>(this->compute_gerstner))
{
}


/*  Adaptive  */
Surface::Surface(const char * wave_file,
		 size_t waves_number_current,
		 size_t size_x,
		 size_t size_y,
		 float wind_angle,
		 float depth,
		 float displacement_factor,
		 bool normalized_normals,
		 float loop_time)
  : aqua::ocean::gerstner::Surface(wave_file,
				   new class classic::Compute(size_x,
							      size_y,
							      displacement_factor,
							      normalized_normals),
				   waves_number_current,
				   wind_angle,
				   depth,
				   loop_time),
    compute_classic(dynamic_cast<class classic::Compute *>(this->compute_gerstner))
{
}


/*  virtual  */
Surface::~Surface(void)
{
}


/****  Set  ****/

void
Surface::set_number_of_waves(size_t number_of_waves)
{
  this->wave_set_gerstner->set_size_current(number_of_waves);

  this->compute->update(*this->wave_set, this->time);
}
