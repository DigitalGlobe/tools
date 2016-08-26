/*  Emacs mode: -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2003 2004 2005  Jocelyn Fréchot

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

/*  wind speed, description and Beaufort number  */


#ifndef WIND_SPEED_H
#define WIND_SPEED_H


/****************  includes  ****************/


/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  constants  ****************/


/*  index in the arrays is the corresponding Beaufort number (from 0 to 12)  */

const size_t Wind_speed_s = 13;

const float Wind_speed[Wind_speed_s] =
  { 0, 1.2, 2.8, 4.9, 7.7, 10.5, 13.1, 15.8, 18.8, 22.1, 25.9, 30.2, 35.2 };

const char * const Wind_speed_description[Wind_speed_s] =
  {
    "calm",
    "light air",
    "light breeze",
    "gentle breeze",
    "moderate breeze",
    "fresh breeze",
    "strong breeze",
    "near gale",
    "gale",
    "strong gale",
    "storm",
    "violent storm",
    "hurricane"
  };


#endif  /*  WIND_SPEED_H  */
