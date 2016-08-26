/*
  This file is part of “The Aqua library”.

  Copyright © 2006  Jocelyn Fréchot

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


/****************  Includes  ****************/


#include "gerstner_callback_classic.h"

#include "config/config_help.h"

#include "gerstner_scene_classic.h"
#include "gerstner_surface_factory.h"

/*  Graphic lib  */
extern "C"
{
#include <GL/glut.h>
}


/****************  Namespaces  ****************/


using namespace gerstner;


/****************  Public functions  ****************/


Callback_Classic::Callback_Classic(class Surface_Factory_Classic * factory)
  : Callback(new class Scene_Classic(factory,
				     glutGet(GLUT_WINDOW_WIDTH),
				     glutGet(GLUT_WINDOW_HEIGHT))),
    scene_classic(dynamic_cast<class Scene_Classic *>(this->scene_gerstner))
{
}


Callback_Classic::~Callback_Classic(void)
{
  /*  “scene_classic” is just a pointer.  */
}


/****************  Protected functions  ****************/


/*
  Local GLUT callback functions, used to extend regular ones in inherited
  classes.
*/


bool
Callback_Classic::keyboard_local(unsigned char c, int, int)
{
  bool is_key_catched;

  is_key_catched = true;

  if (!this->gerstner::Callback::keyboard_local(c, 0, 0))
    {
      is_key_catched = false;
    }

  return is_key_catched;
}
