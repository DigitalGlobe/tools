/*  Emacs mode: -*-  C++  -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2005 2006  Jocelyn Fréchot

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


#ifndef AQUA_DEMO_CONFIG_SCENE_CONTEXT_H
#define AQUA_DEMO_CONFIG_SCENE_CONTEXT_H


/****************  includes  ****************/


/*  local includes  */
#include "constant.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  constants  ****************/


namespace config
{
  namespace scene_context
  {

    /****  indices  ****/

    /*  "index" values are taken from "values arrays" bellow  */

    /*  physical values  */
    const size_t Index_size_x                   = 2;   /*  128  */
    const size_t Index_size_y                   = 2;   /*  128  */
    const size_t Index_length_x                 = 2;   /*  200.0 m  */
    const size_t Index_length_y                 = 2;   /*  200.0 m  */
    const size_t Index_depth                    = 3;   /*  0.0 m  */
    const size_t Index_displacement_factor      = 2;   /*  1.0  */
    const size_t Index_spectrum_factor          = 14;  /*  1.0  */
    const size_t Index_spectrum_factor_phillips = 4;   /*  1e-6  */
    const size_t Index_smallest_wavelength      = 2;   /*  0.1 m  */
    const size_t Index_largest_wavelength       = 28;  /*  800.0 m  */
    const size_t Index_wind_speed               = 7; /* Beaufort, 15.8 m.s-1*/
    const size_t Index_wind_angle               = 1;   /*  pi / 2 rad  */
    const size_t Index_fetch                    = 2;   /*  37,000 meters  */
    const size_t Index_waves_number             = 2;   /*  52  */
    /*  drawing values  */
    const size_t Index_surface_alpha            = 10;  /*  1.0  */

    /****  scene values arrays  ****/

    /*  size (number of points), must be powers of two  */
    const size_t Size_s = 6;
    const size_t Size[Size_s] = { 32, 64, 128, 256, 512, 1024 };
    // Adaptive, for 800 × 600
    //const size_t Size[Size_s] = { 100, 200, 400, 76, 150, 300 };

    /*  length, meters  */
    const size_t Length_s = 5;
    const float Length[Length_s] = { 50.0, 100.0, 200.0, 400.0, 800.0 };
    /*  depth, meters, 0.0 means infinite  */
    const size_t Depth_s = 4;
    const float Depth[Depth_s] = { 1.0, 5.0, 10.0, 0.0 };
    /*  displacement_factor, horizontal, meters  */
    const size_t Displacement_factor_s = 11;
    const float Displacement_factor[Displacement_factor_s] =
      { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0 };
    /*  wave length (smallest or largest), meters  */
    const size_t Wavelength_s = 29;
    const float Wavelength[Wavelength_s] =
      { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
	1.0, 2.0, 3.0, 4.0, 5.0,
	10.0, 15.0,
	20.0, 30.0, 40.0, 50.0,
	100.0, 150.0, 200.0, 250.0, 300.0, 350.0,
	400.0, 800.0 };
    /*  spectrum_factor  */
    const size_t Spectrum_factor_s = 17;
    const float Spectrum_factor[Spectrum_factor_s] =
      { 1e-7, 2.5e-7, 5e-7, 7.5e-7, 1e-6, 2.5e-6, 5e-6, 7.5e-6, 1e-5, 1e-4,
	1e-3, 1e-2, 0.1, 0.5, 1.0, 1.5, 2.0 };
    /*  wind_angle, radians  */
    const size_t Wind_angle_s = 2;
    const float Wind_angle[Wind_angle_s] =
      { 0.0, Constant_pi_by_2 };
    /*  fetch, meters  */
    const size_t Fetch_s = 5;
    const float Fetch[Fetch_s] =
      { 9500.0, 20000.0, 43430.0, 52000.0, 80000.0 };
    /*  number of waves  */
    const size_t Waves_number_s = 7;
    const unsigned int Waves_number[Waves_number_s] =
      { 10, 22, 52, 112, 202, 414, 800 };

    const unsigned int Waves_number_max = Waves_number[Waves_number_s - 1];


    /*  surface_alpha, OpenGL value (opacity)  */
    const size_t Surface_alpha_s = 11;
    const float Surface_alpha[Surface_alpha_s] =
      { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 };

  }
}


#endif  /*  AQUA_DEMO_CONFIG_SCENE_CONTEXT_H  */
