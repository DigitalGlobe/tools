/*  Emacs mode: -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2005 2006  Jocelyn Fréchot

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

/*  OpenGL (Glut) callbacks functions for "class aqua_surface_phill_ugauss"  */


#ifndef AQUA_DEMO_GERSTNER_CALLBACK_PIERSON_H
#define AQUA_DEMO_GERSTNER_CALLBACK_PIERSON_H


/****************  includes  ****************/


#include "gerstner_callback.h"


/****************  polymorphic classes  ****************/


namespace gerstner
{
  class Callback_Pierson : public Callback
  {
  public:

    Callback_Pierson(class Surface_Factory * factory);

    virtual ~Callback_Pierson(void);


  protected:


    /*  not defined  */
    Callback_Pierson(const class Callback_Pierson &);
    void operator=(const class Callback_Pierson &);
  };
}


#endif  /*  AQUA_DEMO_GERSTNER_CALLBACK_PIERSON_H  */
