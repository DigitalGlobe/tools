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


#ifndef AQUA_SPECTRUM_ENERGY_JONSWAP_H
#define AQUA_SPECTRUM_ENERGY_JONSWAP_H


/****************  includes  ****************/


#include <libaqua/aqua_spectrum_energy.h>
#include <libaqua/aqua_spectrum_energy_pierson.h>


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

    class Jonswap : public Energy
    {
    public:

      Jonswap(double wind_speed,  /*  v  */
	      double fetch);      /*  F  */

      virtual ~Jonswap(void);

      /****  Compute  ****/

      /*  S(ω)  */
      virtual double compute(double theta) const;
      virtual double compute_integral(double omega_min,
				      double omega_max) const;
      /*  Computes integral from 0.0 to +∞ using “samples”.  */
      virtual double compute_integral(const class Samples & samples) const;


    protected:

      Pierson pierson;

      double gamma;               /*  γ  */
      double sigma_lesser;        /*  σ₋  */
      double sigma_greater;       /*  σ₊  */
      double sigma_lesser_term;   /*  2 × σ₋² ωp²  */
      double sigma_greater_term;  /*  2 × σ₊² ωp²  */
      double omega_pm_term;       /*  √(−2 ln(ln(1 + ∆ω) ∕ ln(γ)))  */
      double omega_pm_min;    /* ωp × (1 − σ₋ √(−2 ln(ln(1 + ∆ω) ∕ ln(γ)))) */
      double omega_pm_max;    /* ωp × (1 + σ₊ √(−2 ln(ln(1 + ∆ω) ∕ ln(γ)))) */


      virtual void fill_integral_samples(class Samples & samples) const;


      /****  compute  ****/

      /*  α = 0.076 (v² ∕ (F g))^0.22  */
      double compute_alpha(double wind_speed, double fetch) const;
      /*  ωp = 22 (g² ∕ (v F))^(1 ∕ 3) */
      double compute_omega_peak(double wind_speed, double fetch) const;
      /*  2 σ² ωp²  */
      double compute_sigma_term(double sigma, double omega_peak) const;
      /*  √(−2 ln(ln(1 + ∆ω) ∕ ln(γ)))  */
      double compute_omega_pm_term(double gamma) const;
      /*  ωp × (1 − σ₋ √(−2 ln(ln(1 + ∆ω) ∕ ln(γ))))  */
      double compute_omega_pm_min(double omega_peak,
				  double omega_pm_term,
				  double sigma_lesser) const;
      /*  ωp × (1 + σ₊ √(−2 ln(ln(1 + ∆ω) ∕ ln(γ))))  */
      double compute_omega_pm_max(double omega_peak,
				  double omega_pm_term,
				  double sigma_greater) const;
      /*  γ^(exp(−(ω − ωp)² ∕ (2 σ² ωp²)))  */
      double compute_gamma_term(double omega) const;


    private:

      /****  not defined  ****/
      Jonswap(const class Jonswap &);
      void operator =(const class Jonswap &);
    };

  }
}


#endif  /*  AQUA_SPECTRUM_ENERGY_JONSWAP_H  */
