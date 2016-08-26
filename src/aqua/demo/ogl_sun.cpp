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


#include "ogl_sun.h"

#include "sun.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
#include <GL/glut.h>
}


/****************  public functions  ****************/


Ogl_Sun::Ogl_Sun(void)
{
}


void
Ogl_Sun::draw(const class Sun & sun) const
{
  const float * position;

  position = sun.get_position();

  CHECK_GL(glMatrixMode(GL_MODELVIEW));
  CHECK_GL(glPushMatrix());
  {
    CHECK_GL(glTranslatef(position[0], position[1], position[2]));

    CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));
    {
      /**/
      CHECK_GL(glDisable(GL_LIGHTING));
      /**/

      /*  Sun color. Alpha value is for the corona.  */
      CHECK_GL(glColor4f(1.0, 0.9, 0.0, 0.3));
      /*  sun  */
      glutSolidSphere(sun.get_radius(), 20, 20);
      /*  corona  */
      CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
      CHECK_GL(glEnable(GL_BLEND));
      glutSolidSphere(sun.get_radius() * 1.5, 20, 20);
    }
    CHECK_GL(glPopAttrib());
  }
  CHECK_GL(glPopMatrix());
}
