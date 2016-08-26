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


#include "floor.h"


/****************  public functions  ****************/


Floor::Floor(float length_x, float length_z, float depth)
  : length_x(length_x),
    length_z(length_z),
    origin_x(this->compute_origin(length_x)),
    origin_z(- this->compute_origin(length_z)),
    depth(depth)
{
}


/*  virtual destructor  */
Floor::~Floor(void)
{
}


/****  set  ****/

void
Floor::set_length(float length_x, float length_z)
{
  this->origin_x = this->compute_origin(length_x);
  this->origin_z = - this->compute_origin(length_z);
  this->length_x = length_x;
  this->length_z = length_z;
}


/****  get  ****/

float
Floor::get_origin_x(void) const
{
  return this->origin_x;
}


float
Floor::get_origin_z(void) const
{
  return this->origin_z;
}


float
Floor::get_length_x(void) const
{
  return this->length_x;
}


float
Floor::get_length_z(void) const
{
  return this->length_z;
}


float &
Floor::get_depth(void)
{
  return this->depth;
}


float
Floor::get_depth(void) const
{
  return this->depth;
}


/****************  public functions  ****************/


float
Floor::compute_origin(float length) const
{
  return - length / 2.0;
}
