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

/*  OpenGL (Glut) callbacks functions for "class aqua_surface_phill_ugauss"  */


/****************  includes  ****************/


#include "gerstner_callback_pierson.h"

#include "gerstner_scene_pierson.h"

/*  Graphic lib  */
extern "C"
{
#include <GL/glut.h>
}


/****************  namespaces  ****************/


using namespace gerstner;


/****************  public functions  ****************/


Callback_Pierson::Callback_Pierson(class Surface_Factory * factory)
  : Callback(new class Scene_Pierson(factory,
				     glutGet(GLUT_WINDOW_WIDTH),
				     glutGet(GLUT_WINDOW_HEIGHT)))
{
}


/*  virtual  */
Callback_Pierson::~Callback_Pierson(void)
{
}
