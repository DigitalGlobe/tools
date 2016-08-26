/*  Emacs mode: -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2006  Jocelyn Fréchot

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

/*  "Scene" with "Aqua_Surface_Phill"  */


#ifndef AQUA_DEMO_GERSTNER_SCENE_CLASSIC_H
#define AQUA_DEMO_GERSTNER_SCENE_CLASSIC_H


/****************  Includes  ****************/


#include "gerstner_scene_pierson.h"


/****************  Class declarations  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace gerstner
    {
      namespace classic
      {
	class Surface;
      }
    }
  }
}

namespace gerstner
{
  class Surface_Factory;
}


/****************  Polymorphic classes  ****************/


namespace gerstner
{
  class Scene_Classic : public Scene_Pierson
  {
  public:

    Scene_Classic(class gerstner::Surface_Factory * factory,
		  unsigned int window_width,
		  unsigned int window_height);
    virtual ~Scene_Classic(void);


  protected:

    class aqua::ocean::gerstner::classic::Surface * surface_classic;


  private:

    /*  Not defined  */
    Scene_Classic(const class Scene_Classic &);
    void operator=(const class Scene_Classic &);
  };
}


#endif  /*  AQUA_DEMO_GERSTNER_SCENE_CLASSIC_H  */
