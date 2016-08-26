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


#include "camera.h"

#include "config/config_camera.h"

/*  local includes  */
#include "constant.h"
#include "sincos.h"

/*  C lib  */
#include <cmath>


/****************  public functions  ****************/


/*  creates a camera at the origin, looking up the Y axis  */
Camera::Camera(void)
  : position_x(0.0),
    position_y(0.0),
    position_z(0.0),
    angle_x(0.0),
    angle_y(0.0),
    angle_y_rad(0.0),
    view_mode(config::camera::View_mode)
{
}


/****  set  ****/

/*  Observing -> Flying -> Swimming  */
void
Camera::view_mode_switch(void)
{
  switch (this->view_mode)
    {
    case Observing:
      this->view_mode = Flying;
      break;
    case Flying:
      this->view_mode = Swimming;
      break;
    case Swimming:
      this->view_mode = Observing;
      break;
    }
}


/*  sets camera position (in meters), looking up the Y axis  */
void
Camera::reset(float position_x, float position_y, float position_z)
{
  this->position_x = position_x;
  this->position_z = position_z;
  this->position_y = position_y;
  this->angle_x = 0;
  this->angle_y = 0.0;
  this->angle_y_rad = 0.0;
}


void
Camera::set_position_y(float position_y)
{
  this->position_y = position_y;
}


/****  move  ****/

void
Camera::move_forward(bool run)
{
  float step_length;

  step_length = this->get_step_length(run);

  switch (this->view_mode)
    {
    case Observing:
      this->position_z -= step_length;
      break;

    case Flying:
      float temp_sin, temp_cos;
      // ?
      aqua_math::sincosf(this->angle_y_rad, &temp_sin, &temp_cos);

      this->position_x += temp_sin * step_length;
      this->position_z -= temp_cos * step_length;
      break;

    case Swimming:
      break;
    }
}


void
Camera::move_backward(bool run)
{
  float step_length;

  step_length = this->get_step_length(run);

  switch (this->view_mode)
    {
    case Observing:
      this->position_z += step_length;
      break;

    case Flying:
      this->position_x -= sinf(this->angle_y_rad) * step_length;
      this->position_z += cosf(this->angle_y_rad) * step_length;
      break;

    case Swimming:
      break;
    }
}


void
Camera::move_left(bool run)
{
  float step_length;

  step_length = this->get_step_length(run);

  switch (this->view_mode)
    {
    case Observing:
      this->position_x -= step_length;
      break;

    case Flying:
      this->position_x -= cosf(this->angle_y_rad) * step_length;
      this->position_z -= sinf(this->angle_y_rad) * step_length;
      break;

    case Swimming:
      break;
    }
}


void
Camera::move_right(bool run)
{
  float step_length;

  step_length = this->get_step_length(run);

  switch (this->view_mode)
    {
    case Observing:
      this->position_x += step_length;
      break;

    case Flying:
      this->position_x += cosf(this->angle_y_rad) * step_length;
      this->position_z += sinf(this->angle_y_rad) * step_length;
      break;

    case Swimming:
      break;
    }
}


void
Camera::move_up(bool run)
{
  float step_length;

  step_length = this->get_step_length(run);

  switch (this->view_mode)
    {
    case Observing:
    case Flying:
      this->position_y += step_length;
      break;

    case Swimming:
      break;
    }
}


void
Camera::move_down(bool run)
{
  float step_length;

  step_length = this->get_step_length(run);

  switch (this->view_mode)
    {
    case Observing:
    case Flying:
      this->position_y -= step_length;
      break;

    case Swimming:
      break;
    }
}


/*  angles in degrees  */
void
Camera::rotate(float angle_x, float angle_y)
{
  this->angle_x += angle_x;
  this->angle_y += angle_y;
  this->angle_y_rad += angle_y * Constant_deg_to_rad;
}


/****  get  ****/

enum Camera::view_mode
Camera::get_view_mode(void) const
{
  return this->view_mode;
}


/*
  fills three elements "position" vector with "opposite" camera position
  (i.e. suitable for OpenGL transforms)
*/
void
Camera::get_opposite_position(float position[3]) const
{
  position[0] = - this->position_x;
  position[1] = - this->position_y;
  position[2] = - this->position_z;
}


/*  gets angles in degrees  */
void
Camera::get_angle(float & angle_x, float & angle_y) const
{
  angle_x = this->angle_x;
  angle_y = this->angle_y;
}


/****************  protected functions  ****************/


float
Camera::get_step_length(bool run) const
{
  float step_length;

  if (run)
    {
      step_length = config::camera::Step_length_run;
    }
  else
    {
      step_length = config::camera::Step_length_walk;
    }

  return step_length;
}
