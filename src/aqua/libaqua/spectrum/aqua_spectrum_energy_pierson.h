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


#ifndef AQUA_SPECTRUM_ENERGY_PIERSON_H
#define AQUA_SPECTRUM_ENERGY_PIERSON_H


/****************  includes  ****************/


#include <libaqua/aqua_spectrum_energy.h>


/****************  class declarations  ****************/


namespace aqua
{
  namespace spectrum
  {
    class Samples;
  }
}


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace spectrum
  {
    class Pierson : public Energy
    {
    public:

      Pierson(double wind_speed);  /*  v  */

      Pierson(double alpha,        /*  α  */
	      double omega_peak);  /*  ωp  */


      virtual ~Pierson(void);


      /****  Get  ****/

      /*  α g² ∕ (5 ωp⁴)  */
      double get_integral_term(void) const;

      /****  Compute  ****/

      /*  S(ω) = α g² ∕ ω⁵ × exp(−5 ∕ 4 × (ωp ∕ ω)⁴)  */
      virtual double compute(double omega) const;
      /*  ∫S(ω) = α g² ∕ (5 ωp⁴) × exp(−5 ∕ 4 × (ωp ∕ ω)⁴)  */
      virtual double compute_integral(double omega_min,
				      double omega_max) const;
      /*  Computes integral from 0.0 to +∞ (not) using “samples”.  */
      virtual double compute_integral(const class Samples &) const;
      /*  exp(−5 ∕ 4 × (ωp ∕ ω)⁴)  */
      double compute_exponential(double omega) const;


    protected:

      double alpha_g2;          /*  α g²  */
      double omega_peak_4;      /*  ωp⁴  */
      double exponential_term;  /*  −5 ∕ 4 × ωp⁴  */
      double integral_term;     /*  α g² ∕ (5 ωp⁴)  */


      virtual void fill_integral_samples(class Samples & samples) const;


      /****  Compute  ****/

      /*  ωp  */
      double compute_omega_peak(double wind_speed) const;
      /*  α g²  */
      double compute_alpha_g2(double alpha) const;
      /*  ωp⁴  */
      double compute_omega_peak_4(double omega_peak) const;
      /*  −5 ∕ 4 × ωp⁴  */
      double compute_exponential_term(double omega_peak) const;
      /*  α g² ∕ (5 ωp⁴)  */
      double compute_integral_term(double alpha_g2, double omega_peak) const;
      /*  exp(−5 ∕ 4 × (ωp ∕ ω)⁴)  */
      double compute_exponential(double omega,           /*  ω  */
				 double omega_4) const;  /*  ω⁴  */


    private:

      /****  Not defined  ****/
      Pierson(const class Pierson &);
      void operator =(const class Pierson &);
    };
  }
}


#endif  /*  AQUA_SPECTRUM_ENERGY_PIERSON_H  */
