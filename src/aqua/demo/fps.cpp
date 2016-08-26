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


/****************  includes  ****************/


#include "fps.h"

#include "config/config.h"

/*  C++ lib  */
#include <sstream>  /*  ostringstream  */
#include <string>

/*  C lib  */
#include <cmath>  /*  rintf()  */


/****************  public functions  ****************/


/*
  Creates and inits a FPS counter.
  Average FPS will be computed every "number_of_frames_max" frames.
*/
Fps::Fps(void)
  : Frames_max(config::Fps_frames_max)
{
  this->reset(0.0);
  this->time_average = 0.0;
  this->time_average_lib = 0.0;
}


/****  set  ****/

/*
  To be called each frame. "elapsed_time_lib" is the time taken by the
  library during the curent frame.
*/
void
Fps::set_elapsed_time(float time_before_display, float time_after_display)
{
  this->current_frame++;

  /*  store elapsed time between several calls  */
  this->time_total += time_after_display - this->current_time;
  this->time_total_display += time_after_display - time_before_display;

  this->current_time = time_after_display;

  if (this->current_frame >= this->Frames_max)
    {
      this->time_average = this->time_total / this->Frames_max;
      this->time_average_display =
	this->time_total_display / this->Frames_max;
      this->time_average_lib =
	this->time_average - this->time_average_display;

      this->reset(this->current_time);
    }
}


/*  re-inits Fps  */
void
Fps::reset(float current_time)
{
  this->current_frame = 0;
  this->current_time = current_time;
  this->time_total = 0.0;
  this->time_total_display = 0.0;
}


/****  get  ****/

std::string
Fps::make_string(void) const
{
  std::ostringstream temp_stream;

  temp_stream << rintf(1000.0 / this->time_average)  << " FPS "
	      << "(total: " << rintf(this->time_average) << " ms, "
	      << "GL: " << rintf(this->time_average_display) << " ms, "
	      << "lib: " << rintf(this->time_average_lib) << " ms)"
	      << "\n"
	      << std::flush
    ;

  return temp_stream.str();
}
