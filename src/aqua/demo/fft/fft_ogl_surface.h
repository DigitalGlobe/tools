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


#ifndef AQUA_DEMO_FFT_OGL_SURFACE_H
#define AQUA_DEMO_FFT_OGL_SURFACE_H


/****************  includes  ****************/


#include "ogl_surface.h"

#include "fft_drawlist_set.h"  /*  enum resolution  */


/****************  class declarations  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace fft
    {
      class Surface;
    }
  }
}


/****************  classes  ****************/


namespace fft
{
  class Ogl_Surface : public ::Ogl_Surface
  {
  public:

    Ogl_Surface(void);


    /****  draw  ****/
    void draw(unsigned int & overlaps_number,
	      const class aqua::ocean::fft::Surface & surface,
	      const class Drawlist_Set & drawlist_set,
	      float alpha,
	      bool wired,
	      bool drawn_normals,
	      bool drawn_overlaps,
	      bool tiled) const;


  protected:

    /****  draw  ****/
    void draw_surface_unit(unsigned int & overlaps_number,
			   const class aqua::ocean::fft::Surface & surface,
			   const class Drawlist_Set & drawlist_set,
			   bool wired,
			   bool drawn_normals,
			   bool drawn_overlaps,
			   enum Drawlist_Set::resolution resolution) const;
    void draw_surface_unit_half(unsigned int & overlaps_number,
				const class aqua::ocean::fft::Surface & surface,
				const class Drawlist_Set & drawlist_set,
				bool wired,
				bool drawn_normals,
				bool drawn_overlaps,
				enum Drawlist_Set::resolution resolution,
				bool junction) const;


  private:

    /****  not defined  ****/
    Ogl_Surface(const class Ogl_Surface &);
    void operator =(const class Ogl_Surface &);
  };

}


#endif  /*  AQUA_DEMO_FFT_OGL_SURFACE_H  */
