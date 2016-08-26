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


#ifndef AQUA_DEMO_SUN_H
#define AQUA_DEMO_SUN_H


/****************  classes  ****************/


class Sun
{
 public:

  Sun(float distance,      /*  distance from the origin, in meters  */
      float day_length,    /*  seconds  */
      float latitude,      /*  latitude of the observer, in degrees  */
      float day_of_year);  /*  [0, 2pi[, in radians, 0 is march equinox  */ 


  /****  set  ****/
  /*  "time" in seconds  */
  void set_time(float time);

  /****  get  ****/
  /*  fills three elements vector "position"  */
  void get_orientation(float orientation[3]) const;
  const float * get_position(void) const;
  float get_radius(void) const;


 protected:

  const float distance;            /*  distance from the origin, in meters  */
  const float radius;                /*  radius of the sun, in meters  */
  const float angular_speed;         /*  rad.s-1  */

  /*  should be constants  */
  float position_base_x_mult;  /*  bases for "position" computation  */
  float position_base_y_mult;
  float position_base_y_add;
  float position_base_z_mult;
  float position_base_z_add;

  float orientation[3];
  float position[3];


  float compute_radius(float distance) const;
  void position_base_set(float latitude, float day_of_year);


 private:

  /****  not defined  ****/
  Sun(const class Sun &);
  void operator =(const class Sun &);
};


#endif  /*  AQUA_DEMO_SUN_H  */
