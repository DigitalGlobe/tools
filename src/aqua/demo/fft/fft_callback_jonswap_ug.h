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

/*
  OpenGL (Glut) callbacks functions for "class Aqua_Surface_Jonswap_Ug"
*/


#ifndef AQUA_DEMO_FFT_CALLBACK_JONSWAP_UG_H
#define AQUA_DEMO_FFT_CALLBACK_JONSWAP_UG_H


/****************  includes  ****************/


#include "fft_callback.h"

/*  libaqua  */
#include "libaqua/aqua_ocean_fft.h"


/****************  polymorphic classes  ****************/


namespace fft
{
  class Callback_Jonswap_Ug : public Callback
  {
  public:

    Callback_Jonswap_Ug(enum aqua::ocean::fft::type fft_type);

    virtual ~Callback_Jonswap_Ug(void);


  protected:

    class Scene_Jonswap_Ug * const scene_jonswap;


    /*  used to extend "keyboard" in inherited classes  */
    virtual bool keyboard_local(unsigned char c, int x, int y);


    /*  copy constructor to be defined  */
    Callback_Jonswap_Ug(const class Callback_Jonswap_Ug &);
  };
}


#endif  /*  AQUA_DEMO_FFT_CALLBACK_JONSWAP_UG_H  */
