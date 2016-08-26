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


#include "fft_floor.h"


/****************  namespaces  ****************/


using namespace fft;



/****************  public functions  ****************/


fft::Floor::Floor(float length_x_base,
		  float length_z_base,
		  float depth,
		  bool tiled)
  : ::Floor(this->compute_length(length_x_base, tiled),
	    this->compute_length(length_z_base, tiled),
	    depth),
  length_x_base(length_x_base),
  length_z_base(length_z_base),
  tiled(tiled)
{
}


/*  virtual destructor  */
fft::Floor::~Floor(void)
{
}


/****  set  ****/

void
fft::Floor::set_length(float length_x_base, float length_z_base)
{
  this->length_x_base = length_x_base;
  this->length_z_base = length_z_base;

  this->::Floor::set_length(this->compute_length(length_x_base, this->tiled),
			    this->compute_length(length_z_base, this->tiled));
}


void
fft::Floor::set_tiled(bool tiled)
{
  this->tiled = tiled;

  this->::Floor::set_length(this->compute_length(this->length_x_base, tiled),
			    this->compute_length(this->length_z_base, tiled));
}


/****  get  ****/

bool
fft::Floor::get_tiled(void) const
{
  return this->tiled;
}


/****************  public functions  ****************/


float
fft::Floor::compute_length(float length_base, bool tiled) const
{
  float temp;

  if (!tiled)
    {
      temp = length_base;
    }
  else
    {
      temp = length_base * 3.0;
    }

  return temp;
}
