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


/****************  includes  ****************/


#include "aqua_spectrum_tool_wave_cartesian.h"

/*  C++ lib  */
#include <ostream>
#include <istream>


/**************** namespaces  ****************/


using namespace aqua::spectrum_tool;


/****************  public functions  ****************/


Wave_Cartesian::Wave_Cartesian(double amplitude,
			       double vector_x,
			       double vector_y)
  : Wave_Base(amplitude),
    vector_x(vector_x),
    vector_y(vector_y)
{
}


/****************  non-member functions  ****************/


std::ostream &
aqua::spectrum_tool::operator << (std::ostream & o,
				  const class Wave_Cartesian & wave)
{
  return o << wave.vector_x << " "
	   << wave.vector_y << " "
	   << static_cast<const class Wave_Base &>(wave);
}


std::istream &
aqua::spectrum_tool::operator >> (std::istream & i,
				  class Wave_Cartesian & wave)
{
  return i >> wave.vector_x
	   >> wave.vector_y
	   >> static_cast<class Wave_Base &>(wave);
}
