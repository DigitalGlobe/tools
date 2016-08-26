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


#ifndef AQUA_DEMO_GERSTNER_SURFACE_FACTORY_H
#define AQUA_DEMO_GERSTNER_SURFACE_FACTORY_H


/****************  Includes  ****************/


#include "surface_factory.h"


/****************  Class declaration  ****************/


class Scene_Context;

namespace aqua
{
  namespace ocean
  {
    class Surface;
  }
}


/****************  Abstract classes  ****************/


namespace gerstner
{

  class Surface_Factory : public ::Surface_Factory
  {
  public:

    Surface_Factory(bool adaptive);

    virtual ~Surface_Factory(void) = 0;

    virtual class aqua::ocean::Surface *
    create(const class ::Scene_Context & context) const = 0;


  protected:

    const bool adaptive;


  private:

    /****  Not defined  ****/
    Surface_Factory(const class Surface_Factory &);
    void operator =(const class Surface_Factory &);
  };

}  /*  namespace gerstner  */


/****************  Polymorphic classes  ****************/


namespace gerstner
{

  /****************  Surface_Factory_Classic  ****************/

  class Surface_Factory_Classic : public Surface_Factory
  {
  public:

    Surface_Factory_Classic(bool adaptive);

    virtual ~Surface_Factory_Classic(void);

    virtual class aqua::ocean::Surface *
    create(const class ::Scene_Context & context) const;


  protected:


  private:

    /****  Not defined  ****/
    Surface_Factory_Classic(const class Surface_Factory_Classic &);
    void operator =(const class Surface_Factory_Classic &);
  };


}  /*  namespace gerstner  */


#endif  /*  AQUA_DEMO_GERSTNER_SURFACE_FACTORY_H  */
