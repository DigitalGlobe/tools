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


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_SPECTRUM_TOOL_QUANTIFY_FILE_H
#define AQUA_SPECTRUM_TOOL_QUANTIFY_FILE_H


/****************  includes  ****************/


/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  external functions  ****************/


namespace aqua
{
  namespace spectrum
  {
    class Directional;
  }

  namespace spectrum_tool
  {
    class Wave_Polar;
  }
}


/****************  function declarations  ****************/


namespace aqua
{
  namespace spectrum_tool
  {
    extern void
    quantify_file_write(const char * file_name,
			const class spectrum::Directional & spectrum,
			const size_t number_of_waves);

    extern void
    quantify_file_read(class std::vector<size_t> & indices,
		       class std::vector<class Wave_Polar> & waves,
		       const char * file_name);
    extern void
    quantify_file_read(class std::vector<class Wave_Polar> & waves,
		       const size_t size_max,
		       const std::vector<size_t> & indices,
		       const std::vector<class Wave_Polar> & read_waves,
		       const double theta_peak);
//     extern void
//     quantify_file_read(std::vector<std::vector<class Wave_Polar> > &waves,
// 		       const char * file_name);
  }
}


#endif  /*  AQUA_SPECTRUM_TOOL_QUANTIFY_FILE_H  */
