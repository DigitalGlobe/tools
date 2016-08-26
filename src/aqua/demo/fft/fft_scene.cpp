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


#include "fft_scene.h"

#include "config/config.h"

#include "fft_drawlist_set.h"
#include "fft_floor.h"

#include "camera.h"
#include "scene_context.h"

/*  libaqua  */
#include <libaqua/aqua_ocean_fft.h>

/*  C++ lib  */
#include <string>
#include <sstream>  /*  ostringstream  */

/*  C lib  */
#include <cmath>


/****************  namespaces  ****************/


using namespace fft;


/****************  public functions  ****************/


fft::Scene::Scene(class Scene_Context * context,
		  class Surface_Factory * surface_factory,
		  class Help * help)
  : ::Scene(surface_factory,
	    context,
	    new class fft::Floor(context->get_length_x(),
				 context->get_length_y(),
				 context->get_depth(),
				 config::Scene_tiled),
	    help),
  drawlist_set(new
	       class Drawlist_Set(this->surface->get_surface().get_size_j(),
				  this->surface->get_surface().get_size_i())),
  surface_fft(dynamic_cast<class aqua::ocean::fft::Surface *>(this->surface)),
  floor_fft(dynamic_cast<class fft::Floor *>(this->floor)),
  tiled(config::Scene_tiled)
{
  this->camera_reset(context->get_length_x(), context->get_length_y(), tiled);
}


fft::Scene::~Scene(void)
{
  /*  “surface_fft” is just a pointer  */
  delete this->drawlist_set;
}


/****  set  ****/

/*  booleans  */

void
fft::Scene::toggle_tiled(void)
{
  this->tiled = !this->tiled;
  this->floor_fft->set_tiled(this->tiled);
  if (this->camera->get_view_mode() == Camera::Swimming)
    {
      this->camera_reset(this->context->get_length_x(),
			 this->context->get_length_y(),
			 this->tiled);
    }
}


/****  get  ****/

const class aqua::ocean::fft::Surface &
fft::Scene::get_surface_fft(void) const
{
  return *this->surface_fft;
}


const class Drawlist_Set &
fft::Scene::get_drawlist_set(void) const
{
  return *this->drawlist_set;
}


bool
fft::Scene::get_tiled(void) const
{
  return this->tiled;
}


std::string
fft::Scene::make_info(unsigned int overlaps_number) const
{
  std::ostringstream temp_stream;


  temp_stream
    << "smallest_possible_wave: "
      << this->surface_fft->get_smallest_possible_wave()
    << "\n"
    << "largest_possible_wave: "
    << this->surface_fft->get_largest_possible_wave()
    << "\n"
    << "energy ratio: " << this->surface_fft->get_energy_ratio()
    << "\n"
    ;


  return this->::Scene::make_info(overlaps_number) + temp_stream.str();
}


/****  move  ****/

void
fft::Scene::view_mode_reset(void)
{
  this->camera_reset(this->context->get_length_x(),
		     this->context->get_length_y(),
		     this->tiled);
}


void
fft::Scene::view_mode_switch(void)
{
  this->::Scene::view_mode_switch();
  this->camera_reset(this->context->get_length_x(),
		     this->context->get_length_y(),
		     this->tiled);
}


/****************  protected functions  ****************/


/****  global changes  ****/

void
fft::Scene::set_size(class aqua::ocean::Surface & surface,
		     const class Scene_Context & context)
{
  this->::Scene::set_size(surface, context);

  this->drawlist_set->reset(this->surface->get_surface().get_size_j(),
			    this->surface->get_surface().get_size_i());
}


void
fft::Scene::set_length(class aqua::ocean::Surface & surface,
		       class ::Floor & floor,
		       const class Scene_Context & context)
{
  this->::Scene::set_length(surface, floor, context);

  if (this->camera->get_view_mode() == Camera::Swimming)
    {
      this->camera_reset(context.get_length_x(),
			 context.get_length_y(),
			 this->tiled);
    }
}


void
fft::Scene::camera_reset(float length_x, float length_y, bool tiled)
{
  float position_z;

  position_z = fmaxf(length_x, length_y);

  if (!tiled)
    {
      position_z *= 0.5;
    }
  else
    {
      position_z *= 1.5;
    }

  this->camera->reset(0.0, 5.0, position_z);
}
