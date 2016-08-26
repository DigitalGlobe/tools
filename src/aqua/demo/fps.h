/*  Emacs mode:  -*- C++ -*-  */
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

/*  frames per second  */


#ifndef AQUA_DEMO_FPS_H
#define AQUA_DEMO_FPS_H


/****************  includes  ****************/


/*  C++ lib  */
#include <string>


/****************  classes  ****************/


class Fps
{
public:

  /*
    Creates and inits a FPS counter.
    Average FPS will be computed every "number_of_frames_max" frames.
  */
  Fps(void);

  // do we need times to be of type "float"?


  /****  set  ****/
  /*
    To be called each frame. "elapsed_time_lib" is the time taken by the
    library during the curent frame.
  */
  void set_elapsed_time(float time_before_display,
			float time_after_display);
  /*  re-inits Fps  */
  void reset(float current_time);

  /****  get  ****/
  std::string make_string(void) const;


protected:

  const unsigned int Frames_max;

  unsigned int current_frame;
  /*  all time variables are in seconds  */
  float current_time;
  float time_total;
  float time_total_display;
  float time_average;
  float time_average_display;
  float time_average_lib;


  /*  copy constructor to be defined  */
  Fps(const class Fps &);
};


#endif  /*  AQUA_DEMO_FPS_H  */
