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


#ifndef AQUA_SPECTRUM_SAMPLES_H
#define AQUA_SPECTRUM_SAMPLES_H


/****************  includes  ****************/


/*  C++ lib  */
#include <valarray>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  classes  ****************/


namespace aqua
{
  namespace spectrum
  {

    class Samples
    {
    public:

      Samples(const double value_min,
	      const double value_max,
	      const double precision);


      /****  constants  ****/
      const double value_min;
      const double value_max;
      const double precision;


      /****  operators  ****/
      double   operator [](size_t i) const;
      double & operator [](size_t i);

      /****  get  ****/
      const class std::valarray<double> & get_valarray(void) const;
            class std::valarray<double> & get_valarray(void);


    protected:

      class std::valarray<double> samples;

      size_t compute_samples_size(const double value_min,
				  const double value_max,
				  const double precision) const;


    private:
      
      /****  not defined  ****/
      Samples(const class Samples &);
      void operator =(const class Samples &);
    };

  }
}


#endif  /*  AQUA_SPECTRUM_SAMPLES_H  */
