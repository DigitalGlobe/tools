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

/*  OpenGL (Glut) callbacks functions  */


#ifndef AQUA_DEMO_GERSTNER_CALLBACK_CLASSIC_H
#define AQUA_DEMO_GERSTNER_CALLBACK_CLASSIC_H


/****************  Includes  ****************/


#include "gerstner_callback.h"


/****************  Class declarations  ****************/


namespace gerstner
{
  class Scene_Classic;
  class Surface_Factory_Classic;
}


/****************  Polymorphic classes  ****************/


namespace gerstner
{
  class Callback_Classic : public Callback
  {
  public:

    Callback_Classic(class Surface_Factory_Classic * factory);

    virtual ~Callback_Classic(void);


  protected:

    class Scene_Classic * const scene_classic;

    /*
      Local GLUT callback functions, used to extend regular ones in inherited
      classes.
    */
    /*  returns "true" if key event is catched  */
    virtual bool keyboard_local(unsigned char c, int, int);


    /*  Not defined  */
    Callback_Classic(const class Callback_Classic &);
    void operator=(const class Callback_Classic &);
  };
}


#endif  /*  AQUA_DEMO_GERSTNER_CALLBACK_CLASSIC_H  */
