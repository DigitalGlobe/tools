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


#ifndef AQUA_LIB_UTILITIES_H
#define AQUA_LIB_UTILITIES_H


/****************  includes  ****************/


/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  external functions  ****************/


namespace aqua_lib
{
  /****  compute_indexes_from_coordinates  ****/
  extern void compute_indexes_from_coordinates(float coord_x,
					       float coord_y,
					       float origin_x,
					       float origin_y,
					       float resolution_x,
					       float resolution_y,
					       float &index_x,
					       float &index_y);

  /****  index  ****/
  /*  functions return "true" if "index" has changed  */
  extern bool index_decrease(int &index);
  extern bool index_increase(int &index, size_t array_size);

  /****  get_file_name  ****/
  /*
    returns the last component of the path in "file_name" if "_GNU_SOURCE" is
    defined, "file_name" otherwise
  */
  extern char * get_file_name(const char *file_name);
}


#endif  /*  AQUA_LIB_UTILITIES_H  */
