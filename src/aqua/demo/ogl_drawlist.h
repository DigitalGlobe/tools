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


#ifndef AQUA_DEMO_OGL_DRAWLIST_H
#define AQUA_DEMO_OGL_DRAWLIST_H


/****************  includes  ****************/


/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>  /*  GLuint  */
}

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  classes  ****************/


class Ogl_Drawlist
{
public:

  Ogl_Drawlist(size_t array_size_x,
	       size_t array_size_y,
	       unsigned int stride = 1);

  ~Ogl_Drawlist(void);


  /****  set  ****/
  void reset(size_t array_size_x, size_t array_size_y);
  void set_boundaries(size_t start, size_t end);

  /****  get  ****/
  size_t get_size(void) const;
  const GLuint * get_list(void) const;


protected:

  const unsigned int stride;

  // ? voir info FFTW
  size_t row_size;
  size_t column_size;

  size_t size;

  size_t start, end;

  GLuint * list;


  /****  compute  ****/
  size_t compute_row_size(size_t array_size_x, unsigned int stride) const;
  size_t compute_column_size(size_t array_size_y, unsigned int stride) const;

  /****  fill  ****/
  void fill_list(size_t row_size,
		 size_t column_size,
		 size_t array_size_x,
		 size_t array_size_y,
		 unsigned int stride);


private:

  /****  not defined  ****/
  Ogl_Drawlist(const class Ogl_Drawlist &);
  void operator =(const class Ogl_Drawlist &);
};


#endif  /*  AQUA_DEMO_OGL_DRAWLIST_H  */
