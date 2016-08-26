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


/****************  Includes  ****************/


#include "gerstner_ogl_adaptive.h"

/*  Libaqua  */
#include <libaqua/aqua_array.h>

/*  Local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  Graphic lib  */
extern "C"
{
#include <GL/glu.h>
#include <GL/gl.h>
}

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cmath>    /*  round()  */
#include <cstddef>  /*  size_t  */


/****************  Namespaces  ****************/


using namespace gerstner;


/****************  Public functions  ****************/


Ogl_Adaptive::Ogl_Adaptive(size_t size_x,
			   size_t size_y,
			   unsigned int window_width,
			   unsigned int window_height)
  : window_x(size_x),
    window_y(size_y),
    window_width(window_width),
    window_height(window_height),
    list(glGenLists(1))
{
  this->fill_window_any(this->window_x, window_width);
  this->fill_window_any(this->window_y, window_height);
  this->make_display_list(this->list);
}


/*  Virtual destructor  */
Ogl_Adaptive::~Ogl_Adaptive(void)
{
  CHECK_GL(glDeleteLists(this->list, 1));
}


/****  Set  ****/

void
Ogl_Adaptive::set_size(size_t size_x, size_t size_y)
{
  this->window_x.resize(size_x);
  this->window_y.resize(size_y);

  this->fill_window_any(window_x, this->window_width);
  this->fill_window_any(window_y, this->window_height);
}


void
Ogl_Adaptive::set_window_size(unsigned int width, unsigned int height)
{
  this->window_width = width;
  this->window_height = height;

  this->fill_window_any(this->window_x, window_width);
  this->fill_window_any(this->window_y, window_height);
}


/****  Update  ****/

void
Ogl_Adaptive::update(class aqua::ocean::gerstner::Base_Surface & base_surface) const
{
  CHECK_GL(glCallList(this->list));

  GLfloat window_z;
  GLdouble temp_x, temp_y, temp_z;
  GLdouble modelview_matrix[16], projection_matrix[16];
  GLint viewport[4];
  bool is_start_set;
  size_t i, j;


  CHECK_GL(glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix));
  CHECK_GL(glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix));
  CHECK_GL(glGetIntegerv(GL_VIEWPORT, viewport));
  is_start_set = false;

  /*  By default, sets “start” and “end” to the last line.  */
  base_surface.start = this->window_y.size() - 1;
  base_surface.end = base_surface.start;

  for (i = 0; i < this->window_y.size(); i++)
    {
      j = 0;

      CHECK_GL(glReadPixels(this->window_x[j],
			    this->window_y[i],
			    1,
			    1,
			    GL_DEPTH_COMPONENT,
			    GL_FLOAT,
			    &window_z));

      if (window_z < 1.0)
	{
	  if (!is_start_set)
	    {
	      base_surface.start = i;
	      is_start_set = true;
	    }

	  // == GL_TRUE
	  gluUnProject(this->window_x[j],
		       this->window_y[i],
		       window_z,
		       modelview_matrix,
		       projection_matrix,
		       viewport,
		       &temp_x,
		       &temp_y,
		       &temp_z);
	  base_surface.surface.get(0, i, j) = temp_x;
	  base_surface.surface.get(1, i, j) = temp_y;

 	  for (j = 1; j < this->window_x.size(); j++)
 	    {
	      CHECK_GL(glReadPixels(this->window_x[j],
				    this->window_y[i],
				    1,
				    1,
				    GL_DEPTH_COMPONENT,
				    GL_FLOAT,
				    &window_z));

	      // == GL_TRUE
	      gluUnProject(this->window_x[j],
			   this->window_y[i],
			   window_z,
			   modelview_matrix,
			   projection_matrix,
			   viewport,
			   &temp_x,
			   &temp_y,
			   &temp_z);
	      base_surface.surface.get(0, i, j) = temp_x;
	      base_surface.surface.get(1, i, j) = temp_y;
 	    }
	}
      else if (is_start_set)
	{
	  base_surface.end = i - 1;
	  break;
	}
    }
}


/****************  Protected functions  ****************/


void
Ogl_Adaptive::fill_window_any(std::vector<int> & window,
			      unsigned int window_length)
{
  const double increment =
    static_cast<double>(window_length - 1)  //  [0, 799] × [0, 599]
    / static_cast<double>(window.size() - 1);

  for (size_t i = 0; i < window.size(); i++)
    {
      window[i] = static_cast<int>(round(i * increment));
    }
}


void
Ogl_Adaptive::make_display_list(GLuint list)
{
  if (list == 0)
    {
      throw "ogl_adaptive: no more display lists.";
    }


  CHECK_GL(glNewList(list, GL_COMPILE));
  {
    CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));
    {
      CHECK_GL(glDisable(GL_LIGHTING));

      CHECK_GL(glRectf(-10000.0, -10000.0, 10000.0, 10000.0));
    }
    CHECK_GL(glPopAttrib());
  }
  CHECK_GL(glEndList());
}
