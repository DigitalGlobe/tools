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


#include "aqua_spectrum_energy_pierson.h"

#include "aqua_spectrum_samples.h"

/*  local includes  */
#include "constant.h"

/*  C++ lib  */
#include <valarray>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::spectrum;


/****************  public functions  ****************/


Pierson::Pierson(double wind_speed)  /*  v  */
  : Energy(this->compute_omega_peak(wind_speed)),
    alpha_g2(this->compute_alpha_g2(Constant_phillips)),
    omega_peak_4(this->compute_omega_peak_4(this->omega_peak)),
    exponential_term(this->compute_exponential_term(this->omega_peak_4)),
    integral_term(this->compute_integral_term(this->alpha_g2,
					      this->omega_peak_4))
{
}


Pierson::Pierson(double alpha,       /*  α  */
		 double omega_peak)  /*  ωp  */
  : Energy(omega_peak),
    alpha_g2(this->compute_alpha_g2(alpha)),
    omega_peak_4(this->compute_omega_peak_4(this->omega_peak)),
    exponential_term(this->compute_exponential_term(this->omega_peak_4)),
    integral_term(this->compute_integral_term(this->alpha_g2,
					      this->omega_peak_4))
{
}


/*  virtual  */
Pierson::~Pierson(void)
{
}


/****  Get  ****/

/*  α g² ∕ (5 ωp⁴)  */
double
Pierson::get_integral_term(void) const
{
  return this->integral_term;
}


/****  Compute  ****/

/*  S(ω) = α g² ∕ ω⁵ × exp(−5 ∕ 4 × (ωp ∕ ω)⁴)  */
double
Pierson::compute(double omega) const
{
  double omega_4;  /*  ω⁴  */
  double omega_5;  /*  ω⁵  */
  double base;

  if (std::isnormal(omega) != 0)
    {
      omega_4 = omega * omega * omega * omega;
      omega_5 = omega_4 * omega;
      base =
	this->alpha_g2 / omega_5 * this->compute_exponential(omega, omega_4);
    }
  else
    {
      base = 0.0;
    }

  return base;
}


/*  ∫S(ω) = α g² ∕ (5 ωp⁴) × exp(−5 ∕ 4 × (ωp ∕ ω)⁴)  */
double
Pierson::compute_integral(double omega_min,
			  double omega_max) const
{
  this->check_omega_range(omega_min, omega_max);

  return
    this->integral_term
    * (this->compute_exponential(omega_max)
       - this->compute_exponential(omega_min));
}


/*  Computes integral from 0.0 to +∞ (not) using “samples”.  */
double
Pierson::compute_integral(const class Samples &) const
{
  return this->compute_integral(0.0, INFINITY);;
}


/*  exp(−5 ∕ 4 × (ωp ∕ ω)⁴)  */
double
Pierson::compute_exponential(double omega) const
{
  double omega_4;  /*  ω⁴  */

  omega_4 = omega * omega * omega * omega;

  return this->compute_exponential(omega, omega_4);
}


/****************  protected functions  ****************/


void
Pierson::fill_integral_samples(class Samples & samples) const
{
  double exponential_a, exponential_b;
  double omega;

  size_t i;


  samples.get_valarray() = this->integral_term;

  omega = samples.value_min;
  exponential_a = this->compute_exponential(omega);

  for (i = 0; i < samples.get_valarray().size() - 1; i++)
    {
      omega += samples.precision;

      exponential_b = this->compute_exponential(omega);
      samples[i] *= exponential_b - exponential_a;

      exponential_a = exponential_b;
    }
  exponential_b = this->compute_exponential(samples.value_max);
  samples[i] *= exponential_b - exponential_a;
}


/****  compute  ****/

/*  ωp  */
double
Pierson::compute_omega_peak(double wind_speed) const
{
  return 0.855 * Constant_g / wind_speed;
}


/*  α g²  */
double
Pierson::compute_alpha_g2(double alpha) const
{
  return alpha * Constant_g * Constant_g;
}


/*  ωp⁴  */
double
Pierson::compute_omega_peak_4(double omega_peak) const
{
  return omega_peak * omega_peak * omega_peak * omega_peak;
}


/*  −5 ∕ 4 × ωp⁴  */
double
Pierson::compute_exponential_term(double omega_peak_4) const
{
  return -1.25 * omega_peak_4;
}


/*  α g² ∕ (5 ωp⁴)  */
double
Pierson::compute_integral_term(double alpha_g2, double omega_peak_4) const
{
  return alpha_g2 / (5 * omega_peak_4);
}


/*  exp(−5 ∕ 4 × (ωp ∕ ω)⁴)  */
double
Pierson::compute_exponential(double omega,          /*  ω  */
			     double omega_4) const  /*  ω⁴  */
{
  // (omega != NaN) && (omega >= 0.0)

  double exponential;

  if (std::isnormal(omega) != 0)
    {
      exponential = exp(this->exponential_term / omega_4);
    }
  else if (std::isfinite(omega) != 0)  // omega is FP_ZERO or FP_SUBNORMAL
    {
      exponential = 0.0;
    }
  else  //  omega is +∞
    {
      exponential = 1;
    }

  return exponential;
}
