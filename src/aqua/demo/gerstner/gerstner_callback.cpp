/*
  This file is part of “The Aqua library”.

  Copyright © 2005  Jocelyn Fréchot

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

/*  OpenGL (GLUT) callbacks functions  */


/****************  includes  ****************/


#include "gerstner_callback.h"

#include "config/config_help.h"

#include "gerstner_scene.h"
#include "gerstner_ogl_scene.h"

/*  graphic lib  */
extern "C"
{
#include <GL/glut.h>
}


/****************  namespaces  ****************/


using namespace gerstner;


/****************  public functions  ****************/


gerstner::Callback::Callback(class Scene * scene)
  : ::Callback(scene, new class gerstner::Ogl_Scene),
    scene_gerstner(scene)
{
}


gerstner::Callback::~Callback(void)
{
  /*  “scene_gerstner” is just a pointer  */
}


/****************  protected functions  ****************/


/****
     local GLUT callback functions, used to extend regular ones in inherited
     classes
****/

void
gerstner::Callback::idle_local_before(void)
{
}

void
gerstner::Callback::display_local(void)
{
}


void
gerstner::Callback::reshape_local(int w, int h)
{
  this->scene_gerstner->set_window_size(w, h);
}


void
gerstner::Callback::motion_local(int, int)
{
}


bool
gerstner::Callback::keyboard_local(unsigned char c, int, int)
{
  bool is_key_catched;

  is_key_catched = true;


  /****  View mode  ****/

  /*  Resets current view mode.  */
  if (c == config::help::Waves_decrease)
    {
      this->scene_gerstner->decrease_waves();
      glutPostRedisplay();
    }
  else if (c == config::help::Waves_increase)
    {
      this->scene_gerstner->increase_waves();
      glutPostRedisplay();
    }

  /****  If nothing matches.  ****/

  else
    {
      is_key_catched = false;
    }

  return is_key_catched;
}


bool
gerstner::Callback::special_local(int, int, int)
{
  return false;
}
