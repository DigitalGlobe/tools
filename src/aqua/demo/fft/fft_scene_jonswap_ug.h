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

/*  "Scene" with "Aqua_Surface_Jonswap_Ug"  */


#ifndef AQUA_DEMO_FFT_SCENE_JONSWAP_UG_H
#define AQUA_DEMO_FFT_SCENE_JONSWAP_UG_H


/****************  includes  ****************/


#include "fft_scene.h"

/*  libaqua  */
#include <libaqua/aqua_ocean_fft.h>

/*  C++ lib  */
#include <string>


/****************  class declarations  ****************/


class Scene_Context_Jonswap;

namespace aqua
{
  namespace fft
  {
    class Surface_Jonswap_Ug;
  }
}


/****************  polymorphic classes  ****************/


namespace fft
{
  class Scene_Jonswap_Ug : public Scene
  {
  public:

    Scene_Jonswap_Ug(enum aqua::ocean::fft::type fft_type);
    virtual ~Scene_Jonswap_Ug(void);


    /****  set  ****/
    void decrease_fetch(void);
    void increase_fetch(void);

    /****  get  ****/
    float get_fetch(void) const;


  protected:

    class Scene_Context_Jonswap * const context_j;
    class aqua::ocean::fft::Surface_Jonswap_Ug * const surface_j;


    class Scene_Context_Jonswap * create_context(void) const;
    virtual std::string make_info(unsigned int overlaps_number) const;
    void set_fetch(class aqua::ocean::fft::Surface_Jonswap_Ug & surface);


    /*  copy constructor to be defined  */
    Scene_Jonswap_Ug(const class Scene_Jonswap_Ug &);
  };
}


#endif  /*  AQUA_DEMO_FFT_SCENE_JONSWAP_UG_H  */
