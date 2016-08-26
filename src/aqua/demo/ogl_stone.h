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


#ifndef AQUA_DEMO_OGL_STONE_H
#define AQUA_DEMO_OGL_STONE_H


/****************  includes  ****************/


#include "ogl.h"


/****************  classes  ****************/


class Ogl_Stone : public Ogl
{
public:

  Ogl_Stone(void);


  /****  draw  ****/
  void draw(const struct Stone & stone) const;


protected:

  void draw_square_centered(float width) const;


private:

  /****  not defined  ****/
  Ogl_Stone(const class Ogl_Stone &);
  void operator =(const class Ogl_Stone &);
};


#endif  /*  AQUA_DEMO_OGL_STONE_H  */
