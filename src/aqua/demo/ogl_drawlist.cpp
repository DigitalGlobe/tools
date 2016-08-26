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


#include "ogl_drawlist.h"

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>  /*  GLuint  */
}

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  public functions  ****************/


Ogl_Drawlist::Ogl_Drawlist(size_t array_size_x,
			   size_t array_size_y,
			   unsigned int stride)
  : stride(stride),
    row_size(this->compute_row_size(array_size_x, stride)),
    column_size(this->compute_column_size(array_size_y, stride)),
    size(this->row_size * this->column_size),
    start(0),
    end(this->column_size),
    list(new GLuint[this->size])
{
  this->fill_list(this->row_size,
		  this->column_size,
		  array_size_x,
		  array_size_y,
		  stride);
}


Ogl_Drawlist::~Ogl_Drawlist(void)
{
  delete [] this->list;
}


/****  set  ****/


void
Ogl_Drawlist::reset(size_t array_size_x, size_t array_size_y)
{
  this->row_size = this->compute_row_size(array_size_x, this->stride);
  this->column_size = this->compute_column_size(array_size_y, this->stride);
  this->size = this->row_size * this->column_size;

  this->start = 0;
  this->end = this->column_size;

  delete [] this->list;
  this->list = new GLuint[this->size];

  this->fill_list(this->row_size,
		  this->column_size,
		  array_size_x,
		  array_size_y,
		  this->stride);
}


void
Ogl_Drawlist::set_boundaries(size_t start, size_t end)
{
  this->start = start;
  this->end = end;
}


/****  get  ****/


size_t
Ogl_Drawlist::get_size(void) const
{
  //return this->size;
  return this->row_size * (this->end - this->start + 1 - 1);
}


const GLuint *
Ogl_Drawlist::get_list(void) const
{
  //return this->list;
  return this->list + this->start * this->row_size;
}


/****************  protected functions  ****************/


/****  compute  ****/

//
//  if (stride != 1), “array_size_[xy]” must be odd
//
size_t
Ogl_Drawlist::compute_row_size(size_t array_size_x, unsigned int stride) const
{
  return ((array_size_x - 1) / stride + 1) * 2 + 2;
}


size_t
Ogl_Drawlist::compute_column_size(size_t array_size_y,
				  unsigned int stride) const
{
  return (array_size_y - 1) / stride;
}


/****  fill  ****/

/**/

/*
 *  Pour une grille 8 × 8 en FFT, les six premières valeurs sont fausses:
 *  44 48 24 28  4  8  (11 10 16 15…)
 *  au lieu de
 *   0  1  1  0  6  5  (11 10 16 15…)

   1  6 11 16
    __________
   |  |  |  |
   |__|__|__|_
   0  5 10 15
 */

/**/

void
Ogl_Drawlist::fill_list(size_t row_size,
			size_t column_size,
			size_t array_size_x,
			size_t array_size_y,
			unsigned int stride)
{
  size_t temp;
  size_t i_list, i_array, j_list, j_array;


  for (i_list = 0, i_array = 0;
       i_list < column_size;  //  i_array < size_y - 1
       i_list += 2, i_array += stride * 2)
    {
      temp = i_list * row_size;
      this->list[temp]     = i_array * array_size_x;
      this->list[temp + 1] = (i_array + stride) * array_size_x;

      for (j_list = 2, j_array = 0;
	   j_list < row_size; //j_array < array_size_x
	   j_list += 2, j_array += stride)
	{
	  this->list[i_list * row_size + j_list] =
	    (i_array + stride) * array_size_x + j_array;
	  this->list[i_list * row_size + j_list + 1] =
	    i_array * array_size_x + j_array;
	}

      // pas la dernière fois si pair!
      if ((i_list < column_size - 2) || (array_size_y & 1))
	{
	  temp = (i_list + 1) * row_size;
	  this->list[temp]     = this->list[temp - 1];
	  this->list[temp + 1] = this->list[temp - 2];

	  for (j_list = 2, j_array = array_size_x - 1;
	       //for (j_list = 2, j_array = array_size_x;
	       j_list < row_size; //j_array > 0
	       j_list += 2, j_array -= stride)
	    {
	      this->list[(i_list + 1) * row_size + j_list] =
		(i_array + stride) * array_size_x + j_array;
	      this->list[(i_list + 1) * row_size + j_list + 1] =
		(i_array + stride * 2) * array_size_x + j_array;
	    }
	}
    }
}
