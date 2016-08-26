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


/****************  includes  ****************/


#include "aqua_lib_utilities.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */
#include <cstring>  /*  basename(), strlen()  */
#include <filesystem>
using namespace std::tr2::sys;
/****************  functions  ****************/


namespace aqua_lib
{
  /****  compute_indexes_from_coordinates  ****/

  void
  compute_indexes_from_coordinates(float coord_x,
				   float coord_y,
				   float origin_x,
				   float origin_y,
				   float resolution_x,
				   float resolution_y,
				   float &index_x,
				   float &index_y)
  {
    index_x = (coord_x - origin_x) / resolution_x;
    index_y = (coord_y - origin_y) / resolution_y;
  }


  /****  index  ****/

  /*  functions return "true" if "index" has changed  */

  bool
  index_decrease(int &index)
  {
    bool is_index_changed;;

    if (index > 0)
      {
	index--;
	is_index_changed = true;
      }
    else
      {
	is_index_changed = false;
      }

    return is_index_changed;
  }


  bool
  index_increase(int &index, size_t array_size)
  {
    bool is_index_changed;;

    if (index < (static_cast<int>(array_size) - 1))
      {
	index++;
	is_index_changed = true;
      }
    else
      {
	is_index_changed = false;
      }

    return is_index_changed;
  }


  /****  get_file_name  ****/

  /*
    returns the last component of the path in "file_name" if "_GNU_SOURCE" is
    defined, "file_name" otherwise
  */
  char *
  get_file_name(const char *file_name)
  {
    bool is_gnu;
    char *temp_file_name;


#ifdef _GNU_SOURCE
    is_gnu = true;
#else
    is_gnu = false;
#endif  /*  _GNU_SOURCE  */


    if (is_gnu)
      {
		#ifdef _GNU_SOURCE
		temp_file_name = basename(file_name);
		#endif
      }
    else
      {
	//temp_file_name = new char[strlen(file_name) + 1];
	temp_file_name = const_cast<char *>(file_name);
      }


    return temp_file_name;
  }
}
