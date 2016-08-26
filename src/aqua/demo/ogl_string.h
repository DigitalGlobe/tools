/*  Emacs mode: -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2005  Jocelyn Fréchot

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


#ifndef AQUA_DEMO_OGL_STRING_H
#define AQUA_DEMO_OGL_STRING_H


/****************  includes  ****************/


#include "ogl.h"

/*  C++ lib  */
#include <string>


/****************  classes  ****************/


class Ogl_String : public Ogl
{
public:

  Ogl_String(void);


  /****  draw  ****/
  void draw(const std::string & string,
	    bool origin_at_top,
	    int position_x,
	    int position_y) const;


protected:


private:

  /****  not defined  ****/
  Ogl_String(const class Ogl_String &);
  void operator =(const class Ogl_String &);
};


#endif  /*  AQUA_DEMO_OGL_STRING_H  */
