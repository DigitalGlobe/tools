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


#ifndef AQUA_SPECTRUM_DISPERSION_LONGUET_H
#define AQUA_SPECTRUM_DISPERSION_LONGUET_H


/****************  includes  ****************/


#include <libaqua/aqua_spectrum_dispersion.h>


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace spectrum
  {

    class Longuet : public Dispersion
    {
    public:

      Longuet(double omega_peak,   /*  ωp  */
	      double wind_speed);  /*  v  */
      virtual ~Longuet(void);


      /****  compute  ****/

      /*  D(ω, θ)  */
      virtual double compute(double theta,         /*  θ  */
			     double omega) const;  /*  ω  */


    protected:

      const double omega_peak;          /*  ωp  */
      const double spread_factor_term;  /*  (g / (ωp * v))^2.5  */

      /*  previous  */
      mutable double spread_factor_2_previous;       /*  s(ω)  */
      mutable double normalization_factor_previous;  /*  N(s(ω))  */
      mutable double cos_theta_previous;             /*  cos((θw - θ) / 2)  */


      /****  compute  ****/

      /*  11.5 * (g / (ωp * v))^2.5  */
      double compute_spread_factor_term(double omega_peak,         /*  ωp  */
					double wind_speed) const;  /*  v  */
      /*  s(ω)  */
      double compute_spread_factor(double omega) const;  /*  ω  */
      /*  N(s(ω))  */
      double compute_normalization(double spread_factor) const;  /*  s(ω) */


    private:

      /****  not defined  ****/
      Longuet(const class Longuet &);
      void operator =(const class Longuet &);
    };

  }
}


#endif  /*  AQUA_SPECTRUM_DISPERSION_LONGUET_H  */
