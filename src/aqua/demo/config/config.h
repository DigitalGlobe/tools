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


#ifndef AQUA_DEMO_CONFIG_H
#define AQUA_DEMO_CONFIG_H


/****************  includes  ****************/


#include "config_ogl.h"  /*  Perspective_zfar  */


/****************  constants  ****************/


namespace config
{

  /****  window  ****/

  const int Window_size_x     = 800;
  const int Window_size_y     = 600;
  const int Window_position_x = 20;
  const int Window_position_y = 20;

  /****  callback  ****/

  const bool Callback_pause = false;

  /****  scene  ****/

  const bool Scene_wired          = false;
  const bool Scene_tiled          = false;
  const bool Scene_drawn_normals  = false;
  const bool Scene_drawn_overlaps = false;
  const bool Scene_drawn_stone    = true;
  const bool Scene_displayed_fps  = false;
  const bool Scene_displayed_help = true;
  const bool Scene_displayed_info = false;


  /****  fps  ****/

  const unsigned int Fps_frames_max = 3;

  /****  surface  ****/

  const char Surface_wind_speed_file[] = "../data/pm_15";
  const bool Surface_normalized_normals = false;

  /****  stone  ****/

  const float Stone_depth  = 10.0;  /*  meters  */
  const float Stone_height = 10.0;  /*  meters  */
  const float Stone_width  =  1.0;  /*  meters  */

  /****  sun  ****/

  /*  farest but not out of the box  */
  const float Sun_distance    = ogl::Perspective_zfar * 0.8;
  const float Sun_day_length  = 30.0;  /*  seconds  */
  const float Sun_latitude    = 45.0;  /*  degrees, 45 is Lacanau, France  */
  const float Sun_day_of_year =  0.0;  /* [ 0, 2pi [, 0 is march equinox */
}


#endif  /*  AQUA_DEMO_CONFIG_H  */
