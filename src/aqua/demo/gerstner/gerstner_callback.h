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

/*  OpenGL (Glut) callbacks functions  */


#ifndef AQUA_DEMO_GERSTNER_CALLBACK_H
#define AQUA_DEMO_GERSTNER_CALLBACK_H


/****************  includes  ****************/


#include "callback.h"


/****************  class declarations  ****************/


namespace gerstner
{
  class Scene;
}


/****************  polymorphic classes  ****************/


namespace gerstner
{
  class Callback : public ::Callback
  {
  public:

    Callback(class Scene * scene);

    virtual ~Callback(void);


  protected:

    class Scene * const scene_gerstner;

    /****  local GLUT callback functions                    ****/
    /****  used to extend regular ones in inherited classes ****/
    virtual void idle_local_before(void);
    virtual void display_local(void);
    virtual void reshape_local(int w, int h);
    virtual void motion_local(int x, int y);
    /*  returns "true" if key event is catched  */
    virtual bool keyboard_local(unsigned char c, int, int);
    /*  returns "true" if key event is catched  */
    virtual bool special_local(int, int, int);


    /*  not defined  */
    Callback(const class Callback &);
    void operator=(const class Callback &);
  };
}


#endif  /*  AQUA_DEMO_GERSTNER_CALLBACK_H  */
