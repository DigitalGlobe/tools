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


/****************  includes  ****************/


#include "aqua_spectrum_dispersion_longuet.h"

/*  local includes  */
#include "constant.h"

/*  C lib  */
#include <cmath>


/****************  namespaces  ****************/


using namespace aqua::spectrum;


/****************  public functions  ****************/


Longuet::Longuet(double omega_peak,  /*  ωp  */
		 double wind_speed)  /*  v  */
  : omega_peak(omega_peak),
    spread_factor_term(this->compute_spread_factor_term(omega_peak,
							wind_speed)),
    spread_factor_2_previous(0.0),
    //  Γ(1.0) ∕ (2 * √π * Γ(0.5)) = 1 ∕ (2 * √π * √π) = 1 ∕ (2 * π)
    normalization_factor_previous(1.0 / Constant_2_pi),
    cos_theta_previous(cos(0.0 / 2.0))
{
  // must test omega_peak and wind_speed are not equal to zero!
  // like in “class Spectrum”
}


//  if we have “set” functions, we will need to change “previous” values as
//  well


/*  virtual  */
Longuet::~Longuet(void)
{
}


/****  compute  ****/

/*  D(ω, θ)  */
double
Longuet::compute(double theta,        /*  θ  */
		 double omega) const  /*  ω  */
{
  double spread_factor_2;       /*  s(ω) * 2  */
  double normalization_factor;  /*  N(s(ω))  */
  double cos_theta;             /*  cos((θw - θ) / 2)  */

  /*  spread_factor_2, normalization_factor  */
  if (omega == omega_previous)
    {
      spread_factor_2 = this->spread_factor_2_previous;
      normalization_factor = this->normalization_factor_previous;
    }
  else
    {
      double spread_factor;

      spread_factor = this->compute_spread_factor(omega);

      spread_factor_2 = spread_factor * 2;
      normalization_factor = this->compute_normalization(spread_factor);

      /*  previous  */
      this->omega_previous = omega;
      this->spread_factor_2_previous = spread_factor_2;
      this->normalization_factor_previous = normalization_factor;
    }

  /*  cos_theta  */
  if (theta == theta_previous)
    {
      cos_theta = this->cos_theta_previous;
    }
  else
    {
      cos_theta = cos(theta / 2.0);

      /*  previous  */
      this->theta_previous = theta;
      this->cos_theta_previous = cos_theta;
    }

  //  cos_theta must be positive!
  return normalization_factor * pow(fabsf(cos_theta), spread_factor_2);
}


/****************  protected functions  ****************/


/****  compute  ****/

/*  11.5 * (g / (ωp * v))^2.5  */
double
Longuet::compute_spread_factor_term(double omega_peak,        /*  ωp  */
				    double wind_speed) const  /*  v  */
{
  return 11.5 * pow(Constant_g / (omega_peak * wind_speed), 2.5);
}


/*  s(ω)  */
double
Longuet::compute_spread_factor(double omega) const  /*  ω  */
{
  double mu;

  if (omega <= this->omega_peak)
    {
      mu = 5.0;
    }
  else
    {
      mu = -2.5;
    }

  return this->spread_factor_term * pow(omega / this->omega_peak, mu);
}


/*  N(s(ω))  */
double
Longuet::compute_normalization(double spread_factor) const  /*  s(ω) */
{
  /*  gamma(x) > 0 for x > 0  */

  /*  “tgammaf” returns “infinite” if “spread_factor” is too big  */

  return
    tgamma(spread_factor + 1.0)
    / (Constant_2_sqrt_pi * tgamma(spread_factor + 0.5));
}
