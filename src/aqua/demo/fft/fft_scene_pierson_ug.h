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

/*  "Scene" with "Aqua_Surface_PIERSON_Ug"  */


#ifndef AQUA_DEMO_FFT_SCENE_PIERSON_UG_H
#define AQUA_DEMO_FFT_SCENE_PIERSON_UG_H


/****************  includes  ****************/


#include "fft_scene.h"

/*  libaqua  */
#include <libaqua/aqua_ocean_fft.h>


/****************  class declarations  ****************/


class Scene_Context;


/****************  polymorphic classes  ****************/


namespace fft
{
  class Scene_Pierson_Ug : public Scene
  {
  public:

    Scene_Pierson_Ug(enum aqua::ocean::fft::type fft_type);

    virtual ~Scene_Pierson_Ug(void);


  protected:

    class Scene_Context * create_context(void) const;


    /*  copy constructor to be defined  */
    Scene_Pierson_Ug(const class Scene_Pierson_Ug &);
  };
}


#endif  /*  AQUA_DEMO_FFT_SCENE_PIERSON_UG_H  */
