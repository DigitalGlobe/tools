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


#include "ogl_floor.h"

#include "floor.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
}


/****************  public functions  ****************/


Ogl_Floor::Ogl_Floor(void)
{
}


/****  draw  ****/

void
Ogl_Floor::draw(const class Floor & floor) const
{
  GLfloat mat_diffuse[] = { 0.5, 0.8, 0.8, 1.0 };
  GLfloat mat_ambient[] = { 0.0, 0.7, 0.5, 1.0 };


  CHECK_GL(glPushMatrix());
  {
    CHECK_GL(glTranslatef(0.0, - floor.get_depth(), 0.0));

    CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));
    {
      CHECK_GL(glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse));
      CHECK_GL(glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient));

      glBegin(GL_QUADS);
      {
	glNormal3f(0.0, 1.0, 0.0);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(floor.get_length_x(), 0.0, 0.0);
	glVertex3f(floor.get_length_x(), 0.0, - floor.get_length_z());
	glVertex3f(0.0, 0.0, - floor.get_length_z());
      }
      CHECK_GL(glEnd());
    }
    CHECK_GL(glPopAttrib());
  }
  CHECK_GL(glPopMatrix());
}
