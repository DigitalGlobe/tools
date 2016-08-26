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


#include "aqua_spectrum_energy_jonswap.h"

#include "aqua_config.h"  /*  Precision  */

#include "aqua_spectrum_samples.h"

/*  local includes  */
#include "constant.h"

/*  C++ lib  */
#include <valarray>

/*  C lib  */
#include <cmath>
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::spectrum;


/****************  public functions  ****************/


Jonswap::Jonswap(double wind_speed,  /*  v  */
		 double fetch)       /*  F  */
  : Energy(this->compute_omega_peak(wind_speed, fetch)),
    pierson(this->compute_alpha(wind_speed, fetch), this->omega_peak),
    gamma(3.3),
    sigma_lesser(0.07),
    sigma_greater(0.09),
    sigma_lesser_term(this->compute_sigma_term(this->sigma_lesser,
					       this->omega_peak)),
    sigma_greater_term(this->compute_sigma_term(this->sigma_greater,
						this->omega_peak)),
    omega_pm_term(this->compute_omega_pm_term(this->gamma)),
    omega_pm_min(this->compute_omega_pm_min(this->omega_peak,
					    this->omega_pm_term,
					    this->sigma_lesser)),
    omega_pm_max(this->compute_omega_pm_max(this->omega_peak,
					    this->omega_pm_term,
					    this->sigma_greater))
{
  // must test omega_peak and wind_speed are not equal to zero!
  // like in “class Spectrum” but must test fetch too
}


/*  virtual  */
Jonswap::~Jonswap(void)
{
}


/****  compute  ****/

/*  S(ω)  */
double
Jonswap::compute(double omega) const
{
  return this->pierson.compute(omega) * this->compute_gamma_term(omega);
}


double
Jonswap::compute_integral(double omega_min,
			  double omega_max) const
{
  this->check_omega_range(omega_min, omega_max);


  double integral;
  double temp_min, temp_max;

  integral = 0.0;

  /*  [0.0, omega_pm_min]  */
  if (omega_min < this->omega_pm_min)
    {
      temp_min = omega_min;
      temp_max = fmin(omega_max, this->omega_pm_min);

      integral += this->pierson.compute_integral(temp_min, temp_max);
    }

  /*  [omega_pm_min, omega_pm_max]  */
  if ((omega_min < this->omega_pm_max) && (omega_max > this->omega_pm_min))
    {
      temp_min = fmax(omega_min, this->omega_pm_min);
      temp_max = fmin(omega_max, this->omega_pm_max);

      class Samples samples(temp_min, temp_max, config::Precision);
      this->fill_integral_samples(samples);
      integral += samples.get_valarray().sum();
    }

  /*  [omega_pm_max, +∞]  */
  if (omega_max > this->omega_pm_max)
    {
      temp_min = fmax(omega_min, this->omega_pm_max);
      temp_max = omega_max;

      integral += this->pierson.compute_integral(temp_min, temp_max);
    }

  return integral;
}


/*  Computes integral from 0.0 to +∞ using “samples”.  */
double
Jonswap::compute_integral(const class Samples & samples) const
{
  return
    this->compute_integral(0.0, samples.value_min)
    + samples.get_valarray().sum()
    + this->compute_integral(samples.value_max, INFINITY);
}


/****************  protected functions  ****************/


void
Jonswap::fill_integral_samples(class Samples & samples) const
{
  double omega;
  double pierson_exp_a, pierson_exp_b;
  double gamma_a, gamma_b;

  size_t i;


  omega = samples.value_min;
  pierson_exp_a = this->pierson.compute_exponential(omega);
  gamma_a = this->compute_gamma_term(omega);

  for (i = 0; i < samples.get_valarray().size() - 1; i++)
    {
      omega += samples.precision;

      pierson_exp_b = this->pierson.compute_exponential(omega);
      gamma_b = this->compute_gamma_term(omega);
      samples[i] =
	(pierson_exp_b - pierson_exp_a) * (gamma_a + gamma_b) / 2.0;

      pierson_exp_a = pierson_exp_b;
      gamma_a = gamma_b;
    }

  pierson_exp_b = this->pierson.compute_exponential(samples.value_max);
  gamma_b = this->compute_gamma_term(samples.value_max);
  samples[i] = (pierson_exp_b - pierson_exp_a) * (gamma_a + gamma_b) / 2.0;

  samples.get_valarray() *= this->pierson.get_integral_term();

  //  Faster way to compute integral: 
  //  return samples.get_valarray().sum() * this->pierson.get_integral_term();
}


/****  compute  ****/

/*  α = 0.076 (v² ∕ (F g))^0.22  */
double
Jonswap::compute_alpha(double wind_speed, double fetch) const
{
  return 0.076 * pow(wind_speed * wind_speed / (fetch * Constant_g), 0.22);
}


/*  ωp = 22 (g² ∕ (v F))^(1 ∕ 3) */
double
Jonswap::compute_omega_peak(double wind_speed, double fetch) const
{
  return 22 * pow(Constant_g * Constant_g / (wind_speed * fetch), 1.0 / 3.0);
}


/*  2 σ² ωp²  */
double
Jonswap::compute_sigma_term(double sigma, double omega_peak) const
{
  return 2 * sigma * sigma * omega_peak * omega_peak;
}


/*  √(−2 ln(ln(1 + ∆ω) ∕ ln(γ)))  */
double
Jonswap::compute_omega_pm_term(double gamma) const
{
  return sqrt(-2 * log(log(1 + config::Precision) / log(gamma)));
}


/*  ωp × (1 − σ₋ √(−2 ln(ln(1 + ∆ω) ∕ ln(γ))))  */
double
Jonswap::compute_omega_pm_min(double omega_peak,
			      double omega_pm_term,
			      double sigma_lesser) const
{
  return omega_peak * (1 - sigma_lesser * omega_pm_term);
}


/*  ωp × (1 + σ₊ √(−2 ln(ln(1 + ∆ω) ∕ ln(γ))))  */
double
Jonswap::compute_omega_pm_max(double omega_peak,
			      double omega_pm_term,
			      double sigma_greater) const
{
  return omega_peak * (1 + sigma_greater * omega_pm_term);
}


/*  γ^(exp(−(ω − ωp)² ∕ (2 σ² ωp²)))  */
double
Jonswap::compute_gamma_term(double omega) const
{
  double omega_minus_omega_peak;  /*  ω − ωp  */
  double sigma_term;              /*  2 σ² ωp²  */

  omega_minus_omega_peak = omega - this->omega_peak;

  /*  sigma  */
  if (omega <= this->omega_peak)
    {
      sigma_term = this->sigma_lesser_term;
    }
  else
    {
      sigma_term = this->sigma_greater_term;
    }

  return pow(this->gamma,
	     exp(- (omega_minus_omega_peak * omega_minus_omega_peak)
		 / sigma_term));
}
