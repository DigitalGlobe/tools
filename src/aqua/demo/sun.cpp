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


#include "sun.h"

/*  local includes  */
#include "constant.h"
#include "sincos.h"

/*  C lib  */
#include <cmath>


/****************  public functions  ****************/


Sun::Sun(float distance,     /*  distance from the origin, in meters       */
	 float day_length,   /*  in seconds                                */
	 float latitude,     /*  latitude of the observer, in degrees      */
	 float day_of_year)  /*  [0, 2pi[, in radians, 0 is march equinox  */ 
  : distance(distance),
    radius(this->compute_radius(distance)),
    angular_speed(Constant_pi / day_length)
{
  this->position_base_set(latitude, day_of_year);
  /*  sets this->position  */
  this->set_time(0.0);  /*  set at time 0  */
}


/****  set  ****/

/*  "time" in seconds  */
void
Sun::set_time(float time)
{
  /*  simulates sun race  */

  float phi;
  float cos_phi, sin_phi;

  phi = time * this->angular_speed;
  /*  skips the night  */
  if (cosf(phi) < 0.0)
    {
      phi -= Constant_pi;
    }

  aqua_math::sincosf(phi, &sin_phi, &cos_phi);

  /*  orientation is normalized  */
  /*  west  */
  this->orientation[0] = this->position_base_x_mult * sin_phi;
  /*  south  */
  this->orientation[1] =
    this->position_base_y_mult * cos_phi + this->position_base_y_add;
  /*  up  */
  this->orientation[2] =
    this->position_base_z_mult * cos_phi + this->position_base_z_add;

  this->position[0] = this->orientation[0] * this->distance;
  this->position[1] = this->orientation[1] * this->distance;
  this->position[2] = this->orientation[2] * this->distance;
}


/****  get  ****/

/*  fills three elements vector "position"  */
void
Sun::get_orientation(float orientation[3]) const
{
  int i;

  for (i = 0; i < 3; i++)
    {
      orientation[i] = this->orientation[i];
    }
}


const float *
Sun::get_position(void) const
{
  return this->position;
}


float
Sun::get_radius(void) const
{
  return this->radius;
}


/****************  protected functions  ****************/


float
Sun::compute_radius(float distance) const
{
  return
    tanf(Constant_sun_angular_diameter / 2.0 * Constant_deg_to_rad)
    * distance;
}


void
Sun::position_base_set(float latitude, float day_of_year)
{
  float alpha, psi, theta;
  float cos_alpha, sin_alpha;
  float cos_psi, sin_psi, tan_psi;
  float cos_theta, sin_theta;
  float sec_phi;

  alpha = Constant_ecliptic_obliquity * Constant_deg_to_rad;
  psi = day_of_year;
  theta = latitude * Constant_deg_to_rad;

  aqua_math::sincosf(alpha, &sin_alpha, &cos_alpha);
  aqua_math::sincosf(psi, &sin_psi, &cos_psi);
  tan_psi = tanf(psi);
  aqua_math::sincosf(theta, &sin_theta, &cos_theta);
  sec_phi = sqrtf(1 + (tan_psi * cos_alpha) * (tan_psi * cos_alpha));

  /*  north  */
  this->position_base_z_mult =   sin_theta * cos_psi * sec_phi;
  this->position_base_z_add  = - sin_alpha * cos_theta * sin_psi;
  /*  east  */
  this->position_base_x_mult = - cos_psi * sec_phi;
  /*  up  */
  this->position_base_y_mult = cos_theta * cos_psi * sec_phi;
  this->position_base_y_add  = sin_alpha * sin_theta * sin_psi;
}
