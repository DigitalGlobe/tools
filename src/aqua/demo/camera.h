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


#ifndef AQUA_DEMO_CAMERA_H
#define AQUA_DEMO_CAMERA_H


/****************  classes  ****************/


class Camera
{
 public:

  /*
    Observing: turn the camera means rotate the scene around the origin.
    Flying: roaming the land.
    Swimming: as Flying but “move” functions do nothing.
  */
  enum view_mode { Observing = 0, Flying = 1, Swimming = 2 };


  /*  creates a camera at the origin, looking down the Z axis  */
  Camera(void);


  /****  set  ****/
  /*  Observing → Flying → Swimming  */
  void view_mode_switch(void);
  /*  sets camera position (in meters), looking down the Z axis  */
  void reset(float position_x, float position_y, float position_z);
  void set_position_y(float position_y);

  /****  move  ****/
  void move_forward(bool run = true);
  void move_backward(bool run = true);
  void move_left(bool run = true);
  void move_right(bool run = true);
  void move_up(bool run = true);
  void move_down(bool run = true);

  /*  angles in degrees  */
  void rotate(float angle_x, float angle_y);

  /****  get  ****/
  enum view_mode get_view_mode(void) const;
  /*
    fills three elements “position” vector with “opposite” camera position
    (i.e. suitable for OpenGL transforms)
  */
  void get_opposite_position(float position[3]) const;
  /*  gets angles in degrees  */
  void get_angle(float & angle_x, float & angle_y) const;


 protected:

  float position_x;   /*  meters   */
  float position_y;   /*  meters   */
  float position_z;   /*  meters   */
  float angle_x;      /*  degrees  */
  float angle_y;      /*  degrees  */
  float angle_y_rad;  /*  radians  */
  enum view_mode view_mode;

  float get_step_length(bool run) const;


  /*  copy constructor to be defined  */
  Camera(const class Camera &);
};


#endif  /*  AQUA_DEMO_CAMERA_H  */
