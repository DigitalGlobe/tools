/*  Emacs mode: -*-  C++  -*-  */
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


#ifndef AQUA_DEMO_CONFIG_OPTION_H
#define AQUA_DEMO_CONFIG_OPTION_H


/****************  includes  ****************/


#include "options.h"


/****************  constants  ****************/


namespace config
{
  namespace option
  {

    const enum Options_Arguments::spectrum Spectrum =
      Options_Arguments::Pierson_moskowitz;
    const enum Options_Arguments::model Model =
      Options_Arguments::Fft_complex;

  }
}


#endif  /*  AQUA_DEMO_CONFIG_OPTION_H  */
