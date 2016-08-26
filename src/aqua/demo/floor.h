/*  Emacs mode: -*- C++ -*-  */
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


#ifndef AQUA_DEMO_FLOOR_H
#define AQUA_DEMO_FLOOR_H


/****************  polymorphic classes  ****************/


class Floor
{
public:

  Floor(float length_x, float length_z, float depth);
  virtual ~Floor(void);


  /****  set  ****/
  virtual void set_length(float length_x, float length_z);

  /****  get  ****/
  float get_origin_x(void) const;
  float get_origin_z(void) const;
  float get_length_x(void) const;
  float get_length_z(void) const;

  float & get_depth(void);
  float   get_depth(void) const;


protected:

  float length_x;
  float length_z;
  float origin_x;
  float origin_z;
  float depth;


  float compute_origin(float length) const;


private:

  /****  not defined  ****/
  Floor(const class Floor &);
  void operator =(const class Floor &);
};


#endif  /*  AQUA_DEMO_FLOOR_H  */
