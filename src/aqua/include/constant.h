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


#ifndef CONSTANT_H
#define CONSTANT_H


/****************  Includes  ****************/


/*  GSL lib  */
extern "C"
{
#include <gsl/gsl_const_mksa.h>
}

/*  C lib  */
#include <cmath>


/****************  Macros  ****************/


/*  Mathematical macros (prefixed with “M_”) are not part of ISO C.  */
// use GSL ones?

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif  /*  M_PI  */
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif  /*  M_PI_2  */
#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif  /*  M_SQRT2  */


/****************  Constants  ****************/


/****  Physical constants  ****/

/*  the standard gravitational acceleration on Earth, g  */
const float Constant_g                = GSL_CONST_MKSA_GRAV_ACCEL; /* m·s⁻² */
/*  obliquity of the ecliptic (rough)  */
const float Constant_ecliptic_obliquity   = 23.5;  /*  degrees  */
const float Constant_sun_angular_diameter = 0.5;   /*  degrees  */
/*  Phillips constant  */
const float Constant_phillips = 0.0081;


/****  Mathematical constants  ****/

/*  Pi  */
const double Constant_pi        = M_PI;                    /*  π  */
const double Constant_2_pi      = M_PI * 2.0;              /*  π × 2  */
const double Constant_pi_by_2   = M_PI_2;                  /*  π ∕ 2  */
const double Constant_sqrt_pi   = sqrtf(M_PI);             /*  √π  */
const double Constant_2_sqrt_pi = Constant_sqrt_pi * 2.0;  /*  √π × 2  */
/*  Srqt  */
const double Constant_sqrt2     = M_SQRT2;                 /*  √2  */

/*  Conversions  */
const double Constant_deg_to_rad = M_PI / 180.0;  /*  degrees → radians  */
const float  Constant_ms_to_kmh  = 3.6;           /*  m·s⁻¹ → km∕h  */


#endif  /*  CONSTANT_H  */
