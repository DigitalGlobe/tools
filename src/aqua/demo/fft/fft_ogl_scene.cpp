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


#include "fft_ogl_scene.h"

#include "fft_ogl_surface.h"

#include "fft_scene.h"

#include "floor.h"
#include "ogl_floor.h"

/*  Local includes  */
#include "check_gl.h"

/*  Graphic lib  */
extern "C"
{
#include <GL/gl.h>
}


/****************  namespaces  ****************/


using namespace fft;


/****************  public functions  ****************/


fft::Ogl_Scene::Ogl_Scene(void)
  : ::Ogl_Scene(),
  ogl_surface(new class fft::Ogl_Surface)
{
}


/*  virtual destructor  */
fft::Ogl_Scene::~Ogl_Scene(void)
{
  delete this->ogl_surface;
}


/****  draw  ****/

/*  “scene”  must be of type “class fft::Scene”  */
void
fft::Ogl_Scene::draw(const class ::Scene & scene) const
{
  const class fft::Scene & scene_fft
    = dynamic_cast<const class fft::Scene &>(scene);;

  float time_before_display;
  unsigned int overlaps_number;


  this->draw_before_surface(time_before_display, scene);

  CHECK_GL(glTranslatef(scene.get_floor().get_origin_x(),
			0.0,
			scene.get_floor().get_origin_z()));

  if (scene.get_drawn_floor())
    {
      this->ogl_floor->draw(scene.get_floor());
    }

  this->ogl_surface->draw(overlaps_number,
			  scene_fft.get_surface_fft(),
			  scene_fft.get_drawlist_set(),
			  scene.get_surface_alpha(),
			  scene.get_wired(),
			  scene.get_drawn_normals(),
			  scene.get_drawn_overlaps(),
			  scene_fft.get_tiled());

  this->draw_after_surface(scene, time_before_display, overlaps_number);
}
