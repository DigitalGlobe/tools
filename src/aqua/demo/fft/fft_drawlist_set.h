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


#ifndef AQUA_DEMO_FFT_DRAWLIST_SET_H
#define AQUA_DEMO_FFT_DRAWLIST_SET_H


/****************  includes  ****************/

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>  /*  GLuint  */
}

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  class declarations  ****************/


class Ogl_Drawlist;


/****************  classes  ****************/


namespace fft
{
  class Drawlist_Set
  {
  public:

    /****  type declarations  ****/
    enum resolution { Normal, Fourth, Sixteenth };


    Drawlist_Set(size_t size_x, size_t size_y);

    ~Drawlist_Set(void);


    void reset(size_t size_x, size_t size_y);

    /****  get  ****/
    size_t get_list_size(enum resolution resolution) const;
    size_t get_list_junction_size(enum resolution resolution) const;

    const GLuint * get_list(enum resolution resolution) const;
    const GLuint * get_list_junction(enum resolution resolution) const;


  protected:

    class Ogl_Drawlist * const drawlist;        /*  normal resolution  */
    class Ogl_Drawlist * const drawlist_by_4;   /*  resolution by four  */
    class Ogl_Drawlist * const drawlist_by_16;  /*  resolution by sixteen  */

    class Ogl_Drawlist_Junction * const drawlist_junction;       /*  normal resolution  */
    class Ogl_Drawlist_Junction * const drawlist_junction_by_4;  /*  resolution by four  */
    class Ogl_Drawlist_Junction * const drawlist_junction_by_16; /* resolution by sixteen*/


    /****  get  ****/
    const class Ogl_Drawlist & get_drawlist(enum resolution resolution) const;
    const class Ogl_Drawlist_Junction &
    get_drawlist_junction(enum resolution resolution) const;


  private:

    /****  not defined  ****/
    Drawlist_Set(const class Drawlist_Set &);
    void operator =(const class Drawlist_Set &);
  };
}


#endif  /*  AQUA_DEMO_FFT_DRAWLIST_SET_H  */
