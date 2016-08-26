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


#include "fft_ogl_drawlist_junction.h"

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


Ogl_Drawlist_Junction::Ogl_Drawlist_Junction(size_t array_size_x,
					     size_t array_size_y,
					     unsigned int stride)
  : stride(stride),
    array_size_y(array_size_y),
    size(this->compute_size(array_size_x, stride)),
    list(new GLuint[this->size])
{
  this->fill_list(this->size, this->array_size_y, stride);
}


Ogl_Drawlist_Junction::~Ogl_Drawlist_Junction(void)
{
  delete [] this->list;
}


/*  resets "list_size", "list_junction_size", "list" and "list_junction"  */
void
Ogl_Drawlist_Junction::reset(size_t array_size_x, size_t array_size_y)
{
  this->array_size_y = array_size_y;
  this->size = this->compute_size(array_size_x, this->stride);

  delete [] this->list;
  this->list = new GLuint[this->size];

  this->fill_list(this->size, this->array_size_y, this->stride);
}


/****  get  ****/


size_t
Ogl_Drawlist_Junction::get_size(void) const
{
  return this->size;
}


const GLuint *
Ogl_Drawlist_Junction::get_list(void) const
{
  return this->list;
}


/****************  protected functions  ****************/


/****  compute  ****/

size_t
Ogl_Drawlist_Junction::compute_size(size_t array_size_x,
				    unsigned int stride) const
{
  //  row(?) size (voir info FFTW)
  return ((array_size_x - 1) / stride + 1) * 2 + 2;
}


/****  fill  ****/

void
Ogl_Drawlist_Junction::fill_list(size_t size,
				 size_t array_size_y,
				 unsigned int stride)
{
  size_t i_list, i_array;

  for (i_list = 0,        i_array = 0;
       i_list < size - 4;
       i_list += 4,       i_array += 2)
    {
      this->list[i_list]     =       i_array * stride * array_size_y + stride;
      this->list[i_list + 1] =	     i_array * stride * array_size_y;
      this->list[i_list + 2] = (i_array + 1) * stride * array_size_y + stride;
      this->list[i_list + 3] = (i_array + 2) * stride * array_size_y;
    }

  this->list[i_list]     = i_array * stride * array_size_y + stride;
  this->list[i_list + 1] = i_array * stride * array_size_y;
  this->list[i_list + 2] = i_array * stride * array_size_y;
  this->list[i_list + 3] = i_array * stride * array_size_y;
}
