/*  Emacs mode:  -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2004 2005  Jocelyn Fréchot

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

/*  lists of vertices indices to be drawn  */


#ifndef AQUA_DEMO_FFT_OGL_DRAWLIST_JUNCTION_H
#define AQUA_DEMO_FFT_OGL_DRAWLIST_JUNCTION_H


/****************  includes  ****************/


/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>  /*  GLuint  */
}

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  classes  ****************/


namespace fft
{
  class Ogl_Drawlist_Junction
  {
  public:

    Ogl_Drawlist_Junction(size_t array_size_x,
			  size_t array_size_y,
			  unsigned int stride);

    ~Ogl_Drawlist_Junction(void);


    void reset(size_t array_size_x, size_t array_size_y);

    /****  get  ****/
    size_t get_size(void) const;
    const GLuint * get_list(void) const;


  protected:

    const unsigned int stride;

    size_t array_size_y;
    size_t size;

    GLuint * list;


    /****  compute  ****/
    size_t compute_size(size_t array_size_x, unsigned int stride) const;

    /****  fill  ****/
    void fill_list(size_t list_junction_size,
		   size_t array_size_y,
		   unsigned int stride);


    private:

      /****  not defined  ****/
      Ogl_Drawlist_Junction(const class Ogl_Drawlist_Junction &);
      void operator =(const class Ogl_Drawlist_Junction &);
  };
}


#endif  /*  AQUA_DEMO_FFT_OGL_DRAWLIST_JUNCTION_H  */
