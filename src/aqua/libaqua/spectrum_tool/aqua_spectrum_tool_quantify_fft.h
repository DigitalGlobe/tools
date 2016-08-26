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


#ifndef AQUA_SPECTRUM_TOOL_QUANTIFY_FFT_H
#define AQUA_SPECTRUM_TOOL_QUANTIFY_FFT_H


/****************  includes  ****************/


/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  external class declarations  ****************/


namespace aqua
{
  namespace spectrum
  {
    class Directional;
  }

  namespace spectrum_tool
  {
    class Wave_Cartesian;
  }
}


/****************  functions  ****************/


namespace aqua
{
  namespace spectrum_tool
  {
    extern double
    quantify_fft(class std::vector<class Wave_Cartesian> & waves,
		 const size_t size_x,
		 const size_t size_y,
		 const double length_x,
		 const double length_y,
		 const class spectrum::Directional & spectrum,
		 const size_t size_min = 0);
  }
}


#endif  /*  AQUA_SPECTRUM_TOOL_QUANTIFY_FFT_H  */
