/*  Emacs mode: -*-  C++  -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2003 2004 2005 2006  Jocelyn Fréchot

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


#ifndef AQUA_DEMO_CONFIG_HELP_H
#define AQUA_DEMO_CONFIG_HELP_H


/****************  Constants  ****************/


namespace config
{
  namespace help
  {
    /*  General  */
    const char Help   = 'h';
    const char Quit   = 'q';
    const char Quit_2 = 27;  /*  Escape key  */
    const char Pause  = 'p';
    const char Info   = 'i';
    const char Fps    = 'f';

    const char Stone                        = 'e';
    /*  Camera    */
    const char View_reset                   = 'v';
    const char View_switch                  = 'V';
    /*  Surface look  */
    const char Wire                         = 'w';
    const char Normals                      = 'n';
    const char Tiled                        = 't';
    const char Surface_alpha_decrease       = 'a';
    const char Surface_alpha_increase       = 'A';
    const char Overlaps                     = 'o';
    /*  Surface physics  */
    const char Size_decrease                = 's';
    const char Size_increase                = 'S';
    const char Size_x_decrease              = 'x';
    const char Size_x_increase              = 'X';
    const char Size_y_decrease              = 'z';
    const char Size_y_increase              = 'Z';
    const char Length_decrease              = 'l';
    const char Length_increase              = 'L';
    const char Length_x_decrease            = 'k';
    const char Length_x_increase            = 'K';
    const char Length_y_decrease            = 'm';
    const char Length_y_increase            = 'M';
    const char Depth_decrease               = 'd';
    const char Depth_increase               = 'D';
    const char Displacement_factor_decrease = 'c';
    const char Displacement_factor_increase = 'C';
    /*  Spectrum  */
    const char Spectrum_factor_decrease     = 'r';
    const char Spectrum_factor_increase     = 'R';
    const char Smallest_wavelength_decrease = 'b';
    const char Smallest_wavelength_increase = 'B';
    const char Largest_wavelength_decrease  = 'g';
    const char Largest_wavelength_increase  = 'G';
    const char Wind_speed_decrease          = '-';
    const char Wind_speed_increase          = '+';
    const char Wind_angle_decrease          = '<';
    const char Wind_angle_increase          = '>';
    /*  JONSWAP spectrum  */
    const char Fetch_decrease               = 'j';
    const char Fetch_increase               = 'J';

    /*  Parametric  */
    const char Waves_decrease               = 'u';
    const char Waves_increase               = 'U';

    /*  Snap  */
    const char Snap                         = '*';
  }
}


#endif  /*  AQUA_DEMO_CONFIG_HELP_H  */
