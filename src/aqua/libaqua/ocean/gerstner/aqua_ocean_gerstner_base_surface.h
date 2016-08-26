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


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_OCEAN_GERSTNER_BASE_SURFACE_H
#define AQUA_OCEAN_GERSTNER_BASE_SURFACE_H


/****************  Includes  ****************/


#include <libaqua/aqua_array.h>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  Structure  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace gerstner
    {


      struct Base_Surface
      {
	Base_Surface(size_t size_y, size_t size_x)
	  : surface(size_y, size_x), start(0), end(0) {};


	Array<2> surface;
	size_t start, end;
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_GERSTNER_BASE_SURFACE_H  */
