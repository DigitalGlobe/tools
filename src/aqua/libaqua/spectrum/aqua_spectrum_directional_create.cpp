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


#include "aqua_spectrum_directional_create.h"

#include "aqua_spectrum_directional.h"
#include "aqua_spectrum_dispersion_longuet.h"
#include "aqua_spectrum_energy_jonswap.h"
#include "aqua_spectrum_energy_pierson.h"


/****************  namespaces  ****************/


using namespace aqua::spectrum;


/****************  static function declarations  ****************/


namespace
{
  class Directional *
  create_any_longuet(class Energy * ensergy, double wind_speed);
}


/****************  functions  ****************/


namespace aqua
{
  namespace spectrum
  {

class Directional *
directional_create_pierson_longuet(double wind_speed)
{
  class Pierson * pierson;

  pierson = new class Pierson(wind_speed);

  return create_any_longuet(pierson, wind_speed);
}


class Directional *
directional_create_jonswap_longuet(double wind_speed, double fetch)
{
  class Jonswap * jonswap;

  jonswap = new class Jonswap(wind_speed, fetch);

  return create_any_longuet(jonswap, wind_speed);
}

  }
}


/****************  static function definitions  ****************/


namespace
{

class Directional *
create_any_longuet(class Energy * energy, double wind_speed)
{
  class Longuet * longuet;
  class Directional * directional;

  longuet = new class Longuet(energy->get_omega_peak(), wind_speed);
  directional = new class Directional(energy, longuet);

  return directional;
}

}
