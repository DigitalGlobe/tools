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


#ifndef AQUA_SPECTRUM_TOOL_SUM_FOUR_NEIGHBOURS_H
#define AQUA_SPECTRUM_TOOL_SUM_FOUR_NEIGHBOURS_H


/****************  includes  ****************/


/*  C++ lib  */
#include <vector>


/****************  external functions  ****************/


namespace aqua
{
  namespace spectrum_tool
  {
    /*
      array[y][x] + array[y + 1][x] + array[y][x + 1] + array[y + 1][x + 1]
    */
    extern double sum_four_neighbours(const class std::vector<double> & array,
				      unsigned int array_width,
				      unsigned int index_x,
				      unsigned int index_y);
  }
}

#endif  /*  AQUA_SPECTRUM_TOOL_SUM_FOUR_NEIGHBOURS_H  */
