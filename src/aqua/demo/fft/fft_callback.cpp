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

/*  OpenGL (GLUT) callbacks functions  */


/****************  includes  ****************/


#include "fft_callback.h"

#include "config/config_help.h"

#include "fft_scene.h"
#include "fft_ogl_scene.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  graphic lib  */
extern "C"
{
#include <GL/glut.h>
}


/****************  namespaces  ****************/


using namespace fft;


/****************  public functions  ****************/


fft::Callback::Callback(class Scene * scene)
  : ::Callback(scene, new class fft::Ogl_Scene),
  scene_fft(scene)
{
}


fft::Callback::~Callback(void)
{
  /*  “scene_fft” is just a pointer  */
}


/****************  protected functions  ****************/


/****
     local GLUT callback functions, used to extend regular ones in inherited
     classes
****/

void
fft::Callback::idle_local_before(void)
{
}

void
fft::Callback::display_local(void)
{
}


void
fft::Callback::motion_local(int, int)
{
}


bool
fft::Callback::keyboard_local(unsigned char c, int, int)
{
  bool is_key_catched;

  is_key_catched = true;


  /****  view mode  ****/

  /*  reset current view mode  */
  if (c == config::help::View_reset)
    {
      this->scene_fft->view_mode_reset();
      glutPostRedisplay();
    }

  /****  surface look  ****/

  /*  tiled surface  */
  else if (c == config::help::Tiled)
    {
      this->scene_fft->toggle_tiled();
      glutPostRedisplay();
    }

  /****  if nothing matches  ****/

  else
    {
      is_key_catched = false;
    }

  return is_key_catched;
}


bool
fft::Callback::special_local(int, int, int)
{
  return false;
}
