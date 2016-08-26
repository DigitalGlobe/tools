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


#ifndef AQUA_SINCOS_H
#define AQUA_SINCOS_H


/****************  includes  ****************/


#include <cmath>


/****************  functions  ****************/


namespace aqua_math
{


inline void
sincosf(const float angle, float * sine, float * cosine)
{
#ifdef _GNU_SOURCE

  ::sincosf(angle, sine, cosine);

#else

  *sine   = sinf(angle);
  *cosine = cosf(angle);

#endif  /*  _GNU_SOURCE  */
}


inline void
sincos(const double angle, double * sine, double * cosine)
{
#ifdef _GNU_SOURCE

  ::sincos(angle, sine, cosine);

#else

  *sine   = sin(angle);
  *cosine = cos(angle);

#endif  /*  _GNU_SOURCE  */
}


}


#endif  /*  AQUA_SINCOS_H  */
