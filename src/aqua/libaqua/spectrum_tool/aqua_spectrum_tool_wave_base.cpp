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


#include "aqua_spectrum_tool_wave_base.h"

/*  C++ lib  */
#include <ostream>
#include <istream>


/**************** namespaces  ****************/


using namespace aqua::spectrum_tool;


/****************  public functions  ****************/


Wave_Base::Wave_Base(double amplitude)
  : amplitude(amplitude)
{
}


/****  operators  ****/

bool
Wave_Base::operator < (const class Wave_Base & wave) const
{
  return (this->amplitude < wave.amplitude);
}


/****************  non-member functions  ****************/


std::ostream &
aqua::spectrum_tool::operator << (std::ostream & o,
				  const class Wave_Base & wave)
{
  return o << wave.amplitude;
}


std::istream &
aqua::spectrum_tool::operator >> (std::istream & i, class Wave_Base & wave)
{
  return i >> wave.amplitude;
}
