/*  Emacs mode: -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2003 2004 2005 2006  Jocelyn Fréchot

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


#ifndef AQUA_DEMO_GERSTNER_SCENE_PIERSON_H
#define AQUA_DEMO_GERSTNER_SCENE_PIERSON_H


/****************  includes  ****************/


#include "gerstner_scene.h"


/****************  class declarations  ****************/


class Scene_Context;

namespace gerstner
{
  class Surface_Factory;
}


/****************  polymorphic classes  ****************/


namespace gerstner
{
  class Scene_Pierson : public Scene
  {
  public:

    Scene_Pierson(class gerstner::Surface_Factory * factory,
		  unsigned int window_width,
		  unsigned int window_height);
    virtual ~Scene_Pierson(void);


  protected:

    class ::Scene_Context * create_context(void) const;


    /*  not defined  */
    Scene_Pierson(const class Scene_Pierson &);
    void operator=(const class Scene_Pierson &);
  };
}


#endif  /*  AQUA_DEMO_GERSTNER_SCENE_PIERSON_H  */
