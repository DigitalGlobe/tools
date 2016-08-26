/*
  This file is part of “The Aqua library”.

  Copyright © 2003 2004 2005 2006  Jocelyn Fréchot

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


#include "gerstner_scene.h"

#include "gerstner_ogl_adaptive.h"
#include "gerstner_scene_context.h"

#include "floor.h"
#include "ogl_drawlist.h"
#include "scene_context.h"

/*  Libaqua  */
#include <libaqua/aqua_ocean_gerstner.h>

/*  C++ lib  */
#include <string>
#include <sstream>  /*  ostringstream  */

/*  C lib  */
#include <cmath>


/****************  Namespaces  ****************/


using namespace gerstner;


/****************  Public functions  ****************/


gerstner::Scene::Scene(class ::Scene_Context * context,
		       class Surface_Factory * surface_factory,
		       class Help * help,
		       unsigned int window_width,
		       unsigned int window_height)
  : ::Scene(surface_factory,
	    context,
	    new class Floor(context->get_length_x(),
			    context->get_length_y(),
			    context->get_depth()),
	    help),
    ogl_adaptive(context->get_size_x(),
		 context->get_size_y(),
		 window_width,
		 window_height),
    ogl_drawlist(new
		 class Ogl_Drawlist(this->surface->get_surface().get_size_j(),
				    this->surface->get_surface().get_size_i())),
    surface_gerstner(dynamic_cast<class aqua::ocean::gerstner::Surface *>(this->surface)),
    context_gerstner(dynamic_cast<class gerstner::Scene_Context *>(this->context))
{
}


gerstner::Scene::~Scene(void)
{
  /*  “surface_gerstner” and “context_gerstner” are just pointers.  */
  delete this->ogl_drawlist;
}


/****  Set  ****/

// void
// gerstner::Scene::set_adaptive(bool adaptive) const
// {
//   this->surface_gerstner->set_adaptive(adaptive);
// }


void
gerstner::Scene::decrease_waves(void)
{
  if (this->context_gerstner->decrease_waves_number())
    {
      this->set_waves_number(*this->surface_gerstner);
    }
}


void
gerstner::Scene::increase_waves(void)
{
  if (this->context_gerstner->increase_waves_number())
    {
      this->set_waves_number(*this->surface_gerstner);
    }
}


void
gerstner::Scene::set_window_size(unsigned int width, unsigned int height)
{
  this->ogl_adaptive.set_window_size(width, height);
}


/****  Get  ****/

bool
gerstner::Scene::get_adaptive(void) const
{
  return this->surface_gerstner->get_adaptive();
}


const class aqua::ocean::gerstner::Surface &
gerstner::Scene::get_surface_gerstner(void) const
{
  return *this->surface_gerstner;
}


const class Ogl_Adaptive &
gerstner::Scene::get_ogl_adaptive(void) const
{
  return this->ogl_adaptive;
}


class Ogl_Adaptive &
gerstner::Scene::get_ogl_adaptive(void)
{
  return this->ogl_adaptive;
}


const class Ogl_Drawlist &
gerstner::Scene::get_ogl_drawlist(void) const
{
  return *this->ogl_drawlist;
}


class Ogl_Drawlist &
gerstner::Scene::get_ogl_drawlist(void)
{
  return *this->ogl_drawlist;
}


class aqua::ocean::gerstner::Base_Surface &
gerstner::Scene::get_base_surface(void) const
{
  return this->surface_gerstner->get_base_surface();
}


std::string
gerstner::Scene::make_info(unsigned int overlaps_number) const
{
  std::ostringstream temp_stream;


  temp_stream
    << "number of waves: "
      << this->context_gerstner->get_waves_number()
    << "\n"
    ;


  return this->::Scene::make_info(overlaps_number) + temp_stream.str();
}


/****************  Protected functions  ****************/


/****  Global changes  ****/

void
gerstner::Scene::set_size(class aqua::ocean::Surface & surface,
			  const class ::Scene_Context & context)
{
  this->::Scene::set_size(surface, context);

  this->ogl_adaptive.set_size(this->context->get_size_x(),
			      this->context->get_size_y());
  this->ogl_drawlist->reset(this->context->get_size_x(),
			    this->context->get_size_y());
}


void
gerstner::Scene::set_waves_number(class aqua::ocean::gerstner::Surface & surface)
{
  surface.set_number_of_waves(this->context_gerstner->get_waves_number());
}
