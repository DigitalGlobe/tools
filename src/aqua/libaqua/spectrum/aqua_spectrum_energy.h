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


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_SPECTRUM_ENERGY_H
#define AQUA_SPECTRUM_ENERGY_H


/****************  class declarations  ****************/


namespace aqua
{
  namespace spectrum
  {
    class Samples;
  }
}


/****************  abstract classes  ****************/


namespace aqua
{
  namespace spectrum
  {

    class Energy
    {
    public:

      Energy(double omega_peak);  /*  ωp  */

      virtual ~Energy(void) = 0;


      /****  Get  ****/

      double get_omega_peak(void) const;

      /****  Make  ****/

      class Samples * make_integral_samples(const double omega_min,
					    const double omega_max,
					    const double precision) const;
      /****  Compute  ****/

      /*  S(ω)  */
      virtual double compute(double omega) const = 0;  /*  ω  */

      /**  Compute integral  **/

      /*  Computes integral from 0.0 to +∞.  */
      double compute_integral(void) const;

      /*  Computes integral from “omega_from” to “omega_to”.  */
      virtual double compute_integral(double omega_from,
				      double omega_to) const = 0;

      /*  Computes integral from 0.0 to +∞ using “samples”.  */
      virtual double compute_integral(const class Samples & samples) const =0;


    protected:

      const double omega_peak;

      void check_omega_range(double omega_from,
			     double omega_to) const throw (const char *);

      virtual void fill_integral_samples(class Samples & samples) const = 0;


    private:
      
      /****  not defined  ****/
      Energy(const class Energy &);
      void operator =(const class Energy &);
    };

  }
}


#endif  /*  AQUA_SPECTRUM_ENERGY_H  */
