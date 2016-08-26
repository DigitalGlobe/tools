/*  Emacs mode: -*-  C++  -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2003 2004 2005 2006  Jocelyn Fréchot

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


#ifndef AQUA_DEMO_CONFIG_OGL_H
#define AQUA_DEMO_CONFIG_OGL_H


/****************  includes  ****************/


/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
}


/****************  constants  ****************/


namespace config
{
  namespace ogl
  {
    /****  gluPerspective  ****/

    /* 
       Assumes that screen height is 25 cm and viewpoint is 50 cm from the
       screen (znear), so fovy is
       atan(screen_height ∕ 2.0 ∕ znear) × 2.0 ≈ 28.0 degrees.
       → If znear = 2 × screen height, then we have
         atan(1 ∕ 2 ∕ 2) × 2 = atan(1 ∕ 4) × 2 ≈ 28 degrees.
    */
    const float Perspective_fovy  = 28.0;    /*  degrees  */
    const float Perspective_znear = 0.5;     /*  meters  */
    const float Perspective_zfar  = 4000.0;  /*  meters  */

    /****  GL hints  ****/

    const GLenum Hint_perspective = GL_DONT_CARE;
    const GLenum Hint_point       = GL_DONT_CARE;
    const GLenum Hint_line        = GL_DONT_CARE;
    const GLenum Hint_polygon     = GL_DONT_CARE;
  }
}


#endif  /*  AQUA_DEMO_CONFIG_OGL_H  */
