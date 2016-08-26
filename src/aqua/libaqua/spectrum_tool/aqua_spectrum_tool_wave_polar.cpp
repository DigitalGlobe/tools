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


#include "aqua_spectrum_tool_wave_polar.h"

#include "aqua_spectrum_tool_wave_cartesian.h"

/*  local includes  */
#include "constant.h"
#include "sincos.h"

/*  C++ lib  */
#include <ostream>
#include <istream>


/**************** namespaces  ****************/


using namespace aqua::spectrum_tool;


/****************  public functions  ****************/


Wave_Polar::Wave_Polar(double amplitude, double theta, double omega)
  : Wave_Base(amplitude),
    theta(theta),
    omega(omega)
{
}


class Wave_Cartesian
Wave_Polar::get_cartesian_frequency(void) const
{
  double sine, cosine;
  double vector_x, vector_y;


  aqua_math::sincos(this->theta, &sine, &cosine);

  vector_x = this->omega * cosine;
  vector_y = this->omega * sine;


  return Wave_Cartesian(this->amplitude, vector_x, vector_y);
}


class Wave_Cartesian
Wave_Polar::get_cartesian_wavenumber(void) const
{
  double sine, cosine;
  double vector_x, vector_y;


  aqua_math::sincos(this->theta, &sine, &cosine);

  vector_x = this->omega * this->omega * cosine / Constant_g;
  vector_y = this->omega * this->omega * sine  / Constant_g;


  return Wave_Cartesian(this->amplitude, vector_x, vector_y);
}


/****************  non-member functions  ****************/


std::ostream &
aqua::spectrum_tool::operator << (std::ostream & o,
				  const class Wave_Polar & wave)
{
  return o << wave.theta << " "
	   << wave.omega << " "
	   << static_cast<const class Wave_Base &>(wave);
}


std::istream &
aqua::spectrum_tool::operator >> (std::istream & i, class Wave_Polar & wave)
{
  return i >> wave.theta
	   >> wave.omega
	   >> static_cast<class Wave_Base &>(wave);
}
