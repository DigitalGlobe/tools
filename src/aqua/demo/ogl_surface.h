/*  Emacs mode: -*- C++ -*-  */
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


#ifndef AQUA_DEMO_OGL_SURFACE_H
#define AQUA_DEMO_OGL_SURFACE_H


/****************  includes  ****************/


#include "ogl.h"

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>  /*  GLuint  */
}


/****************  classe declarations  ****************/


namespace aqua
{
  namespace ocean
  {
    class Surface;
  }
}


/****************  classes  ****************/


class Ogl_Surface : public Ogl
{
public:

  Ogl_Surface(void);


protected:

  /****  draw  ****/
  void draw_before(float alpha, bool wired) const;
  void draw_after(void) const;

  /****  draw surface  ****/
  void draw_surface_before(const class aqua::ocean::Surface & surface,
			   const GLuint * list,
			   int list_size,
			   bool wired) const;
  void draw_surface_after(unsigned int & overlaps_number,
			  const class aqua::ocean::Surface & surface,
			  bool wired,
			  bool drawn_normals,
			  bool drawn_overlaps) const;

  /****  others  ****/
  void draw_normals_half(const class aqua::ocean::Surface & surface,
			 float scale) const;
  void draw_overlaps_half(unsigned int & overlaps_number,
			  const class aqua::ocean::Surface & surface,
			  float scale) const;


private:

  /****  not defined  ****/
  Ogl_Surface(const class Ogl_Surface &);
  void operator =(const class Ogl_Surface &);
};


#endif  /*  AQUA_DEMO_OGL_SURFACE_H  */
