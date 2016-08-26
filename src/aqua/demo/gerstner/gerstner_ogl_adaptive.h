/*  Emacs mode: -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2006  Jocelyn Fréchot

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


#ifndef AQUA_DEMO_GERSTNER_OGL_ADAPTIVE_H
#define AQUA_DEMO_GERSTNER_OGL_ADAPTIVE_H


/****************  Includes  ****************/


#include "ogl.h"

/*  Libaqua  */
#include <libaqua/aqua_ocean_gerstner_base_surface.h>

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>  /*  GLuint  */
}

/*  C lib  */
#include <cstddef>  /*  size_t  */

/*  C++ lib  */
#include <vector>


/****************  Polymorphic classes  ****************/


namespace gerstner
{

  class Ogl_Adaptive : public Ogl
  {
  public:

    Ogl_Adaptive(size_t size_x,
		 size_t size_y,
		 unsigned int window_width,
		 unsigned int window_height);

    virtual ~Ogl_Adaptive(void);

    /****  Set  ****/
    void set_size(size_t size_x, size_t size_y);
    void set_window_size(unsigned int width, unsigned int height);
    /****  Update  ****/
    void
    update(class aqua::ocean::gerstner::Base_Surface & rested_surface) const;


  protected:

    class std::vector<int> window_x;
    class std::vector<int> window_y;

    unsigned int window_width;
    unsigned int window_height;
    GLuint list;

    void fill_window_any(std::vector<int> & window, unsigned int window_length);
    void make_display_list(GLuint list);


  private:

    /****  not defined  ****/
    Ogl_Adaptive(const class Ogl_Adaptive &);
    void operator =(const class Ogl_Adaptive &);
  };

}


#endif  /*  AQUA_DEMO_GERSTNER_OGL_ADAPTIVE_H  */
