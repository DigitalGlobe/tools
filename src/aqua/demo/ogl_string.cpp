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


#include "ogl_string.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
#include <GL/glut.h>
}

/*  C++ lib  */
#include <string>


/****************  public functions  ****************/


Ogl_String::Ogl_String()
{
}


/****************  protected functions  ****************/


void
Ogl_String::draw(const std::string & string,
		 bool origin_at_top,
		 int position_x,
		 int position_y) const
{
  const char * message;
  int line;
  int i;


  message = string.data();
  line = 0;
  i = 0;


  if (origin_at_top)
    {
      CHECK_GL(glMatrixMode(GL_MODELVIEW));
      CHECK_GL(glPushMatrix());

      CHECK_GL(glScalef(1, -1, 1));
      CHECK_GL(glTranslatef(0, - glutGet(GLUT_WINDOW_HEIGHT), 0));
    }

  CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));
  {
    CHECK_GL(glDisable(GL_LIGHTING));
    CHECK_GL(glDisable(GL_DEPTH_TEST));
    CHECK_GL(glShadeModel(GL_FLAT));
    CHECK_GL(glColor3f(1.0, 1.0, 0.0));
    /*  must be called after "glColor"  */
    CHECK_GL(glRasterPos2i(position_x, position_y));
      
    while (message[i] != '\0')
      {
	if (message[i] == '\n')
	  {
	    line++;
	    CHECK_GL(glRasterPos2i(position_x, position_y + 12 * line));
	    /*  doesn't print this character  */
	    i++;
	    continue;
	  }
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
	i++;
      }
  }
  CHECK_GL(glPopAttrib());
    
  if (origin_at_top)
    {
      CHECK_GL(glMatrixMode(GL_MODELVIEW));
      CHECK_GL(glPopMatrix());
    }
}
