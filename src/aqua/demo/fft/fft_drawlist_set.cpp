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


/****************  includes  ****************/


#include "fft_drawlist_set.h"

#include "fft_ogl_drawlist_junction.h"

#include "ogl_drawlist.h"

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>  /*  GLuint  */
}

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace fft;


/****************  public functions  ****************/


Drawlist_Set::Drawlist_Set(size_t size_x, size_t size_y)
  : drawlist(new class Ogl_Drawlist(size_x, size_y, 1)),
    drawlist_by_4(new class Ogl_Drawlist(size_x, size_y, 2)),
    drawlist_by_16(new class Ogl_Drawlist(size_x, size_y, 4)),
    drawlist_junction(new class Ogl_Drawlist_Junction(size_x, size_y, 1)),
    drawlist_junction_by_4(new class Ogl_Drawlist_Junction(size_x,
							   size_y,
							   2)),
    drawlist_junction_by_16(new class Ogl_Drawlist_Junction(size_x,
							    size_y,
							    4))
{
}


Drawlist_Set::~Drawlist_Set(void)
{
  delete this->drawlist_junction_by_16;
  delete this->drawlist_junction_by_4;
  delete this->drawlist_junction;
  delete this->drawlist_by_16;
  delete this->drawlist_by_4;
  delete this->drawlist;
}


void
Drawlist_Set::reset(size_t size_x, size_t size_y)
{
  this->drawlist->reset(size_x, size_y);
  this->drawlist_by_4->reset(size_x, size_y);
  this->drawlist_by_16->reset(size_x, size_y);

  this->drawlist_junction->reset(size_x, size_y);
  this->drawlist_junction_by_4->reset(size_x, size_y);
  this->drawlist_junction_by_16->reset(size_x, size_y);
}


/****  get  ****/


size_t
Drawlist_Set::get_list_size(enum resolution resolution) const
{
  return this->get_drawlist(resolution).get_size();
}


size_t
Drawlist_Set::get_list_junction_size(enum resolution resolution) const
{
  return this->get_drawlist_junction(resolution).get_size();
}


const GLuint *
Drawlist_Set::get_list(enum resolution resolution) const
{
  return this->get_drawlist(resolution).get_list();
}


const GLuint *
Drawlist_Set::get_list_junction(enum resolution resolution) const
{
  return this->get_drawlist_junction(resolution).get_list();
}


/****************  protected functions  ****************/


/****  get  ****/

const class Ogl_Drawlist &
Drawlist_Set::get_drawlist(enum resolution resolution) const
{
  class Ogl_Drawlist * temp;

  /*  avoid compiler warnings for might be uninitialized variable  */
  temp = NULL;

  switch (resolution)
    {
    case Normal:
      temp = this->drawlist;
      break;

    case Fourth:
      temp = this->drawlist_by_4;
      break;

    case Sixteenth:
      temp = this->drawlist_by_16;
      break;
    }

  return *temp;
}


const class Ogl_Drawlist_Junction &
Drawlist_Set::get_drawlist_junction(enum resolution resolution) const
{
  class Ogl_Drawlist_Junction * temp;

  /*  avoid compiler warnings for might be uninitialized variable  */
  temp = NULL;

  switch (resolution)
    {
    case Normal:
      temp = this->drawlist_junction;
      break;

    case Fourth:
      temp = this->drawlist_junction_by_4;
      break;

    case Sixteenth:
      temp = this->drawlist_junction_by_16;
      break;
    }

  return *temp;
}
