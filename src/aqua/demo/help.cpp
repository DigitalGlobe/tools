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


/****************  includes  ****************/


#include "help.h"

#include "config/config_help.h"

/*  C++ lib  */
#include <sstream>  /*  ostringstream  */
#include <string>


/****************  namespaces  ****************/


using namespace config::help;


/****************  public functions  ****************/


Help::Help(void)
{
}


/*  virtual  */
Help::~Help(void)
{
}


std::string
Help::make_string(void) const
{
  std::ostringstream temp_stream;

  temp_stream
    << "This help: " << config::help::Help
    << "\n"
    << "Quit: " << Quit
    << "\n"
    << "Pause: " << Pause
    << "\n"
    << "Show info: " << Info
    << "\n"
    << "Frames per seconde: " << Fps
    << "\n"
    << "Graduated stone: " << Stone
    << "\n"
    << "\n"
    << "Camera"
    << "\n"
    << "  reset current view mode: " << View_reset << "\n"
    << "  switch view mode "
    << "(observing / flying / swimming): " << View_switch << "\n"
    << "  forward / backward / right / left: arrow keys\n"
    << "  up / down: page-up / page-down\n"
    << "\n"
    << "Surface look"
    << "\n"
    << "  wire mode: " << Wire
    << "\n"
    << "  normals: " << Normals
    << "\n"
    << "  tiled surface: " << Tiled
    << "\n"
    << "  increase / decrease surface alpha (opacity): "
    << Surface_alpha_increase << " / " << Surface_alpha_decrease
    << "\n"
    << "  overlaps detection: " << Overlaps
    << "\n"
    << "\n"
    << "Surface physics"
    << "\n"
    << "  increase / decrease number of points: "
    << Size_increase << " / " << Size_decrease
    << " (X and Y direction: " << Size_x_increase << " / " << Size_x_decrease
    << " and " << Size_y_increase << " / " << Size_y_decrease << ")"
    << "\n"
    << "  increase / decrease surface length: "
    << Length_increase << " / " << Length_decrease
    << " (X and Y direction: "
    << Length_x_increase << " / " << Length_x_decrease
    << " and " << Length_y_increase << " / " << Length_y_decrease << ")"
    << "\n"
    << "  increase / decrease displacement factor: "
    << Displacement_factor_increase << " / " << Displacement_factor_decrease
    << "\n"
    << "  increase / decrease depth: "
    << Depth_increase << " / " << Depth_decrease
    << "\n"
    << "\n"
    << "Spectrum"
    << "\n"
    << "  increase / decrease smallest and largest wave length: "
    << Smallest_wavelength_increase << " / " << Smallest_wavelength_decrease
    << " and "
    << Largest_wavelength_increase << " / " << Largest_wavelength_decrease
    << "\n"
    << "  increase / decrease spectrum factor: "
    << Spectrum_factor_increase << " / " << Spectrum_factor_decrease
    << "\n"
    << "  increase / decrease wind speed: "
    << Wind_speed_increase << " / " << Wind_speed_decrease
    << "\n"
    << "  increase / decrease wind angle: "
    << Wind_angle_increase << " / " << Wind_angle_decrease
    << "\n"
    ;

  return temp_stream.str();
}
