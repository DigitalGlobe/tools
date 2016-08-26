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


#ifndef AQUA_DEMO_FFT_SURFACE_FACTORY_JONSWAP_UG_H
#define AQUA_DEMO_FFT_SURFACE_FACTORY_JONSWAP_UG_H


/****************  includes  ****************/


#include "surface_factory.h"

/*  libaqua  */
#include <libaqua/aqua_ocean_fft.h>


/****************  class declaration  ****************/


namespace aqua
{
  namespace ocean
  {
    class Surface;
  }
}


/****************  polymorphic classes  ****************/


namespace fft
{
  class Surface_Factory_Jonswap_Ug : public ::Surface_Factory
  {
  public:

    Surface_Factory_Jonswap_Ug(enum aqua::ocean::fft::type fft_type);

    virtual ~Surface_Factory_Jonswap_Ug(void);

    virtual class aqua::ocean::Surface *
    create(const class Scene_Context & context) const;


  protected:

    const enum aqua::ocean::fft::type fft_type;


  private:

    /****  not defined  ****/
    Surface_Factory_Jonswap_Ug(const class Surface_Factory_Jonswap_Ug &);
    void operator =(const class Surface_Factory_Jonswap_Ug &);
  };
}

#endif  /*  AQUA_DEMO_FFT_SURFACE_FACTORY_JONSWAP_UG_H  */
