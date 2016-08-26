/*
  This file is part of “The Aqua library”.

  Copyright © 2005 2006  Jocelyn Fréchot

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


#include "gerstner_ogl_scene.h"

#include "gerstner_ogl_adaptive.h"
#include "gerstner_ogl_surface.h"

#include "gerstner_scene.h"

#include "floor.h"
#include "ogl_drawlist.h"
#include "ogl_floor.h"

/*  Local includes  */
#include "check_gl.h"

/*  Graphic lib  */
extern "C"
{
#include <GL/gl.h>
}


/****************  namespaces  ****************/


using namespace gerstner;


/****************  public functions  ****************/


gerstner::Ogl_Scene::Ogl_Scene(void)
  : ::Ogl_Scene(),
    ogl_surface(new class gerstner::Ogl_Surface)
{
}


/*  virtual destructor  */
gerstner::Ogl_Scene::~Ogl_Scene(void)
{
  delete this->ogl_surface;
}


/****  draw  ****/

/*  “scene”  must be of type “class gerstner::Scene”  */
void
gerstner::Ogl_Scene::draw(const class ::Scene & scene) const
{
  const class gerstner::Scene & scene_gerstner_c
    = dynamic_cast<const class gerstner::Scene &>(scene);
  class gerstner::Scene & scene_gerstner
    = const_cast<class gerstner::Scene &>(scene_gerstner_c);

  float time_before_display;
  unsigned int overlaps_number;

  if (scene_gerstner.get_adaptive())
    {
      this->set_base_surface(scene_gerstner_c);
      scene_gerstner.get_ogl_drawlist().set_boundaries(scene_gerstner.get_base_surface().start,
						       scene_gerstner.get_base_surface().end);
    }

  this->draw_before_surface(time_before_display, scene);


  if (!scene_gerstner.get_adaptive())
    {
      CHECK_GL(glTranslatef(scene.get_floor().get_origin_x(),
			    0.0,
			    scene.get_floor().get_origin_z()));

      if (scene.get_drawn_floor())
	{
	  this->ogl_floor->draw(scene.get_floor());
	}
    }


  this->ogl_surface->draw(overlaps_number,
			  scene.get_surface(),
			  scene_gerstner.get_ogl_drawlist(),
			  scene.get_surface_alpha(),
			  scene.get_wired(),
			  scene.get_drawn_normals(),
			  scene.get_drawn_overlaps());

  this->draw_after_surface(scene, time_before_display, overlaps_number);
}


void
gerstner::Ogl_Scene::set_base_surface(const class Scene & scene) const
{
  float position[3];
  float angle_x, angle_y;


  scene.get_camera().get_opposite_position(position);
  scene.get_camera().get_angle(angle_x, angle_y);


  CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  CHECK_GL(glPushMatrix());
  {
    this->move(scene.get_camera().get_view_mode(),
	       position,
	       angle_x,
	       angle_y);
    this->rotate_to_z_up();
    scene.get_ogl_adaptive().update(scene.get_base_surface());
  }
  CHECK_GL(glPopMatrix());
}


