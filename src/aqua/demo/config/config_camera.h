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


#ifndef AQUA_DEMO_CONFIG_CAMERA_H
#define AQUA_DEMO_CONFIG_CAMERA_H


/****************  includes  ****************/


#include "camera.h"  /*  enum Camera::view_mode  */


/****************  constants  ****************/


namespace config
{
  namespace camera
  {

    const enum Camera::view_mode View_mode = Camera::Observing;
    const float Step_length_run            = 5.0;
    const float Step_length_walk           = 1.0;

  }
}


#endif  /*  AQUA_DEMO_CONFIG_CAMERA_H  */
