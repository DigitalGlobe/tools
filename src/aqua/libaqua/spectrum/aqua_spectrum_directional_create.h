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


#ifndef AQUA_SPECTRUM_DIRECTIONAL_CREATE_H
#define AQUA_SPECTRUM_DIRECTIONAL_CREATE_H


/****************  function declarations  ****************/


namespace aqua
{
  namespace spectrum
  {

    extern class Directional *
    directional_create_pierson_longuet(double wind_speed);
    extern class Directional *
    directional_create_jonswap_longuet(double wind_speed, double fetch);

  }
}


#endif  /*  AQUA_SPECTRUM_DIRECTIONAL_CREATE_H  */
