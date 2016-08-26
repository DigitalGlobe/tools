/*  Emacs mode: -*- C++ -*-  */
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

/*  macro function for OpenGL commands debugging  */


#ifndef CHECK_GL_H
#define CHECK_GL_H


/****************  includes  ****************/


/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
#include <GL/glu.h>
}

/*  C++ lib  */
#include <iostream>


/****************  macros  ****************/


/* 
   Don't use this macro with "glBegin()" nor between glBegin/glEnd pairs. It
   will report "invalid operation", because "glError()" (and
   "gluErrorString()"?) is not allowed inside the pair. Use it with "glEnd()".
*/
#ifdef DEBUG_GL
#define CHECK_GL(opengl_command)					\
  {									\
    GLenum error;							\
									\
    opengl_command;							\
									\
    for (;;)								\
      {									\
	error = glGetError();						\
	if (error == GL_NO_ERROR)					\
	  {								\
	    break;							\
	  }								\
	else								\
	  {								\
	    std::cerr << "[" << __FILE__ << ": " << __LINE__ << "]"	\
		      << " \"" << #opengl_command << "\""		\
		      << " failed with error \""			\
		      << gluErrorString(error) << "\""			\
		      << std::endl;					\
	  }								\
      }									\
  }
#else
#define CHECK_GL(opengl_command)		\
  {						\
    opengl_command;				\
  }
#endif  /*  DEBUG_GL  */


#endif  /*  CHECK_GL_H  */
