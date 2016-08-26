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

/*  "class Rng" with unit gaussian distribution  */


#ifndef AQUA_RNG_UGAUSSIAN_H
#define AQUA_RNG_UGAUSSIAN_H


/****************  includes  ****************/


#include "aqua_rng_rng.h"


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace rng
  {
    class Ugaussian : public Rng
    {
    public:

      Ugaussian(void);
      virtual ~Ugaussian(void);


      /****  get  ****/
      /*  returns a random number of uniform gaussian distribution  */
      virtual float get_number(void) const;


    protected:


    private:

      /****  not defined  ****/
      Ugaussian(const class Ugaussian &);
      void operator =(const class Ugaussian &);
    };

  }
}


#endif  /*  AQUA_RNG_UGAUSSIAN_H  */
