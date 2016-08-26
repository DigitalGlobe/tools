/*  Emacs mode: -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2005  Jocelyn Fréchot

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


#ifndef AQUA_DEMO_GERSTNER_OGL_SURFACE_H
#define AQUA_DEMO_GERSTNER_OGL_SURFACE_H


/****************  includes  ****************/


#include "ogl_surface.h"


/****************  class declarations  ****************/


class Ogl_Drawlist;

namespace aqua
{
  namespace ocean
  {
    class Surface;
  }
}


/****************  classes  ****************/


namespace gerstner
{
  class Ogl_Surface : public ::Ogl_Surface
  {
  public:

    Ogl_Surface(void);


    /****  draw  ****/
    void draw(unsigned int & overlaps_number,
	      const class aqua::ocean::Surface & surface,
	      const class Ogl_Drawlist & drawlist,
	      float alpha,
	      bool wired,
	      bool drawn_normals,
	      bool drawn_overlaps) const;


  protected:


  private:

    /****  not defined  ****/
    Ogl_Surface(const class Ogl_Surface &);
    void operator =(const class Ogl_Surface &);
  };

}


#endif  /*  AQUA_DEMO_GERSTNER_OGL_SURFACE_H  */
