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


#include "aqua_ocean_gerstner_surface.h"

#include "aqua_ocean_gerstner_compute.h"
#include "aqua_ocean_gerstner_wave_set.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::ocean::gerstner;


/****************  public functions  ****************/


/*  creates a surface and sets it at time 0.0  */
Surface::Surface(const char * wave_file,
		 class Compute * compute,
		 size_t waves_number_current,
		 float wind_angle,
		 float depth,
		 float loop_time)
  : aqua::ocean::Surface(new class Wave_Set(wave_file,
					    waves_number_current,
					    wind_angle,
					    depth,
					    loop_time),
			 compute),
    wave_set_gerstner(dynamic_cast<class Wave_Set * const>(this->wave_set)),
    compute_gerstner(dynamic_cast<class Compute * const>(this->compute))
{
}


/*  virtual  */
Surface::~Surface(void)
{
}


/****  Get  ****/

class Base_Surface &
Surface::get_base_surface(void)
{
  return this->compute_gerstner->get_base_surface();
}


bool
Surface::get_adaptive(void) const
{
  return this->compute_gerstner->get_adaptive();
}


/****  Set  ****/

// void
// Surface::set_adaptive(bool adaptive)
// {
//   this->compute_gerstner->set_adaptive(adaptive);
// }
