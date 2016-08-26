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


#ifndef AQUA_DEMO_FFT_FLOOR_H
#define AQUA_DEMO_FFT_FLOOR_H


/****************  includes  ****************/


#include "floor.h"


/****************  polymorphic classes  ****************/


namespace fft
{
  class Floor : public ::Floor
  {
  public:

    Floor(float length_x_base, float length_z_base, float depth, bool tiled);
    virtual ~Floor(void);

    /****  set  ****/
    virtual void set_length(float length_x_base, float length_z_base);
    void set_tiled(bool tiled);

    /****  get  ****/
    bool get_tiled(void) const;



  protected:

    float length_x_base;
    float length_z_base;
    bool tiled;


    float compute_length(float length_base, bool tiled) const;


  private:

    /****  not defined  ****/
    Floor(const class Floor &);
    void operator =(const class Floor &);
  };
}


#endif  /*  AQUA_DEMO_FFT_FLOOR_H  */
