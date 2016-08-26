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


#include "ogl_stone.h"

#include "stone.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
#include <GL/glut.h>
}


/****************  public functions  ****************/


Ogl_Stone::Ogl_Stone(void)
{
}


/****  draw  ****/

void
Ogl_Stone::draw(const struct Stone & stone) const
{
  GLfloat mat_diffuse[] = { 0.4, 0.4, 0.4, 1.0 };
  GLfloat mat_ambient[] = { 0.0, 0.7, 0.5, 1.0 };

  int i;


  CHECK_GL(glPushMatrix());
  {
    CHECK_GL(glTranslatef(0.0, (-stone.depth + stone.height) / 2.0, 0.0));
    CHECK_GL(glScalef(1.0, (stone.depth + stone.height) / stone.width, 1.0));

    CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));
    {
      CHECK_GL(glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse));
      CHECK_GL(glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient));

      glutSolidCube(stone.width);
    }
    CHECK_GL(glPopAttrib());
  }
  CHECK_GL(glPopMatrix());

  /*  graduations  */
  CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));
  {
    CHECK_GL(glDisable(GL_LIGHTING));
    CHECK_GL(glShadeModel(GL_FLAT));

    /*  zero  */
    CHECK_GL(glColor3f(0.6, 0.0, 0.0));
    this->draw_square_centered(stone.width);

    CHECK_GL(glColor3f(0.4, 0.0, 0.0));
    /*  above  */
    CHECK_GL(glPushMatrix());
    {
      for (i = 1; i < stone.height; i++)
	{
	  CHECK_GL(glTranslatef(0.0, 1.0, 0.0));
	  this->draw_square_centered(stone.width);
	}
    }
    CHECK_GL(glPopMatrix());
    /*  below  */
    CHECK_GL(glPushMatrix());
    {
      for (i = -1; i > -stone.depth; i--)
	{
	  CHECK_GL(glTranslatef(0.0, -1.0, 0.0));
	  this->draw_square_centered(stone.width);
	}
    }
    CHECK_GL(glPopMatrix());
  }
  CHECK_GL(glPopAttrib());
}


/****************  protected functions  ****************/


void
Ogl_Stone::draw_square_centered(float width) const
{
  float half_width;

  half_width = width / 2.0 + 0.01;

  glBegin(GL_LINE_LOOP);
  {
    glVertex3f(-half_width, 0.0, half_width);
    glVertex3f(half_width, 0.0, half_width);
    glVertex3f(half_width, 0.0, -half_width);
    glVertex3f(-half_width, 0.0, -half_width);
  }
  CHECK_GL(glEnd());
}
