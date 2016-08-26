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


/****************  Includes  ****************/


#include "ogl.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
}


/****************  Public functions  ****************/


Ogl::Ogl(void)
{
}


/****************  Protected functions  ****************/


void
Ogl::move_around_origin(const float position[3],
			float angle_x,
			float angle_y) const
{
  /*  The rotation center of the camera is the origin.  */
  CHECK_GL(glTranslatef(position[0], position[1], position[2]));
  this->rotate(angle_x, angle_y);
}


void
Ogl::move_around_observer(const float position[3],
			  float angle_x,
			  float angle_y) const
{
  /*  The rotation center of the camera is the observer.  */
  this->rotate(angle_x, angle_y);
  CHECK_GL(glTranslatef(position[0], position[1], position[2]));
}


void
Ogl::rotate(float angle_x, float angle_y) const
{
  CHECK_GL(glRotatef(angle_x, 1.0, 0.0, 0.0));
  CHECK_GL(glRotatef(angle_y, 0.0, 1.0, 0.0));
}


/*  Makes the Z axis pointing up.  */
void
Ogl::rotate_to_z_up(void) const
{
  CHECK_GL(glRotatef(-90.0, 1.0, 0.0, 0.0));
}
