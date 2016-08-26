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


#ifndef AQUA_SPECTRUM_TOOL_WAVE_POLAR_H
#define AQUA_SPECTRUM_TOOL_WAVE_POLAR_H


/****************  includes  ****************/


#include "aqua_spectrum_tool_wave_base.h"

/*  C++ lib  */
#include <ostream>
#include <istream>


/****************  classes  ****************/


namespace aqua
{
  namespace spectrum_tool
  {
    class Wave_Polar : public Wave_Base
    {
    public:

      Wave_Polar(double amplitude, double theta, double omega);

      double theta;
      double omega;

      class Wave_Cartesian get_cartesian_frequency(void) const;
      class Wave_Cartesian get_cartesian_wavenumber(void) const;


    protected:


    private:
      /*  we allow default copy constructor and assignement operator  */
    };
  }
}

/****************  non-member functions  ****************/


namespace aqua
{
  namespace spectrum_tool
  {
    extern std::ostream &
    operator << (std::ostream & o, const class Wave_Polar & wave);
    extern std::istream &
    operator >> (std::istream & i, class Wave_Polar & wave);
  }
}


#endif  /*  AQUA_SPECTRUM_TOOL_WAVE_POLAR_H  */
