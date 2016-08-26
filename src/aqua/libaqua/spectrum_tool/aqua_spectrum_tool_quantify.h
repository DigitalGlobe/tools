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


#ifndef AQUA_SPECTRUM_TOOL_QUANTIFY_H
#define AQUA_SPECTRUM_TOOL_QUANTIFY_H


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
}


/****************  external functions  ****************/


namespace aqua
{
  namespace spectrum_tool
  {
    /*  waves are sorted by amplitude  */
    extern void
    quantify(class std::vector<class std::vector<class Wave_Polar> > & waves,
	     const class spectrum::Directional & spectrum,
	     size_t number_of_waves);

    /*  waves are sorted by amplitude  */
    extern void quantify(class std::vector<class Wave_Polar> & waves,
			 const class spectrum::Directional & spectrum,
			 size_t number_of_waves);

    /*  waves are unsorted  */
    extern void quantify(class std::vector<class Wave_Polar> & waves,
			 class std::vector<size_t> & wave_indices,
			 const class spectrum::Directional & spectrum,
			 size_t number_of_waves);
  }
}

#endif  /*  AQUA_SPECTRUM_TOOL_QUANTIFY_H  */
