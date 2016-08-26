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


#ifndef AQUA_DEMO_OGL_H
#define AQUA_DEMO_OGL_H


/****************  classes  ****************/


class Ogl
{
public:

  Ogl(void);


protected:

  void move_around_origin(const float position[3],
			  float angle_x,
			  float angle_y) const;
  void move_around_observer(const float position[3],
			    float angle_x,
			    float angle_y) const;
  void rotate(float angle_x, float angle_y) const;
  /*  Makes the Z axis pointing up.  */
  void rotate_to_z_up(void) const;


private:

  /****  Not defined  ****/
  Ogl(const class Ogl &);
  void operator =(const class Ogl &);
};


#endif  /*  AQUA_DEMO_OGL_H  */
