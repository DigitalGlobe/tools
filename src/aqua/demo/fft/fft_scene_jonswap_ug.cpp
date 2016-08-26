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

/*  "Scene" with "Aqua_Surface_Jonswap_Ug"  */


/****************  includes  ****************/


#include "fft_scene_jonswap_ug.h"

#include "config/config_scene_context.h"

#include "fft_surface_factory_jonswap_ug.h"

#include "help_jonswap.h"
#include "scene_context_jonswap.h"

/*  libaqua  */
#include <libaqua/aqua_ocean_fft.h>


/*  C++ lib  */
#include <string>
#include <sstream>


/****************  namespaces  ****************/


using namespace config::scene_context;
using namespace fft;

using aqua::ocean::fft::Surface_Jonswap_Ug;


/****************  public functions  ****************/


Scene_Jonswap_Ug::Scene_Jonswap_Ug(enum aqua::ocean::fft::type fft_type)
  : Scene(this->create_context(),
	  new class Surface_Factory_Jonswap_Ug(fft_type),
	  new class Help_Jonswap),
    context_j(dynamic_cast<class Scene_Context_Jonswap *>(this->context)),
    surface_j(dynamic_cast<class Surface_Jonswap_Ug *>(this->surface))
{
}


/*  virtual  */
Scene_Jonswap_Ug::~Scene_Jonswap_Ug(void)
{
}


/****  set  ****/

/*  fetch  */

void
Scene_Jonswap_Ug::decrease_fetch(void)
{
  if (this->context_j->decrease_fetch())
    {
      this->set_fetch(*this->surface_j);
    }
}


void
Scene_Jonswap_Ug::increase_fetch(void)
{
  if (this->context_j->increase_fetch())
    {
      this->set_fetch(*this->surface_j);
    }
}


/****  get  ****/

float
Scene_Jonswap_Ug::get_fetch(void) const
{
  return this->context_j->get_fetch();
}


/****************  protected functions  ****************/


class Scene_Context_Jonswap *
Scene_Jonswap_Ug::create_context(void) const
{
  return new class Scene_Context_Jonswap(Index_size_x,
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
					 Index_surface_alpha,
					 Index_fetch);
}


std::string
Scene_Jonswap_Ug::make_info(unsigned int overlaps_number) const
{
  std::ostringstream temp_stream;


  temp_stream
    << "fetch: " << this->get_fetch() << " m"
    << "\n"
    ;


  return this->Scene::make_info(overlaps_number) + temp_stream.str();
}


/****  global changes  ****/

void
Scene_Jonswap_Ug::set_fetch(class Surface_Jonswap_Ug & surface)
{
  surface.set_fetch(this->context_j->get_fetch());
}
