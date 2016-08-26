/*
  This file is part of “The Aqua library”.

  Copyright © 2005 2006  Jocelyn Fréchot

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


#include "aqua_spectrum_directional.h"

#include "aqua_spectrum_boundaries.h"
#include "aqua_spectrum_dispersion.h"
#include "aqua_spectrum_energy.h"
#include "aqua_spectrum_samples.h"

/*  local includes  */
#include "constant.h"
#include "vector.h"

/*  C++ lib  */
#include <algorithm>  /*  sort()  */
#include <utility>    /*  class pair  */
#include <vector>

/*  C lib  */
#include <cmath>
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::spectrum;


/****************  public functions  ****************/


Directional::
Directional(class Energy * energy,          /*  S(ω)  */
	    class Dispersion * dispersion)  /*  D(ω, θ)  */
  : energy(energy),
    dispersion(dispersion),
    omega_previous(0.0),
    energy_previous(0.0)
{
}


/*  Virtual  */
Directional::~Directional(void)
{
  delete this->dispersion;
  delete this->energy;
}


/****  Get  ****/

double
Directional::get_omega_peak(void) const
{
  return this->energy->get_omega_peak();
}


const class Energy &
Directional::get_energy(void) const
{
  return *this->energy;
}


const class Dispersion &
Directional::get_dispersion(void) const
{
  return *this->dispersion;
}


/****  Compute  ****/

/*  E(ω, θ)  */
double
Directional::compute(double theta,        /*  θ  */
		     double omega) const  /*  ω  */
{
  double energy;

  if (omega == this->omega_previous)
    {
      energy = this->energy_previous;
    }
  else
    {
      energy = this->energy->compute(omega);

      this->omega_previous = omega;
      this->energy_previous = energy;
    }

  return energy * this->dispersion->compute(theta, omega);
}


/*  E(ω⃗)  */
double
Directional::compute_cartesian(double omega_x, double omega_y) const
{
  double omega, theta;
  double temp;


  aqua_math::vector_cartesian_to_polar(omega, theta, omega_x, omega_y);

  if (std::isnormal(omega) != 0)
    {
      /*  ∬ E(ω⃗) = ∬ E(ω, θ)∕ω  */
      temp = this->compute(theta, omega) / omega;
    }
  else
    {
      temp = 0.0;
    }
  return temp;
}


/*  E(κ⃗)  */
double
Directional::compute_cartesian_wavenumber(double kappa_x,
					  double kappa_y) const
{
  double kappa, theta, omega;
  double temp;


  aqua_math::vector_cartesian_to_polar(kappa, theta, kappa_x, kappa_y);

  if (std::isnormal(kappa) != 0)
    {
      omega = sqrt(Constant_g * kappa);
      /*  ∬ E(κ⃗) = ∬ E(κ, θ)∕κ = ∬ E(ω, θ)∕κ dω∕dκ = ∬ E(ω, θ)∕κ ½√(g∕κ)  */
      temp =
	this->compute(theta, omega) / kappa * .5 * sqrt(Constant_g / kappa);
    }
  else
    {
      temp = 0.0;
    }
  return temp;
}


/****  Boundaries  ****/


void
Directional::find_boundaries(double & top,
			     double & right,
			     double & bottom) const
{
  /****  Constants  ****/

  const size_t samples_omega = 1000;
  const size_t samples_theta = samples_omega / 2;

  const double omega_max = this->get_omega_peak() * 2.5;
  const double omega_min = 0.0;
  const double theta_max = Constant_pi;

  const double step_omega = (omega_max - omega_min) / samples_omega;
  const double step_theta = theta_max / samples_theta;
  const class Samples & integral_samples =
    *(this->energy->make_integral_samples(omega_min, omega_max, step_omega));

  const double integral =
    this->energy->compute_integral(integral_samples)
    * 4
    / step_theta
    / 2.0;

  /****  Variables  ****/

  class Boundaries boundaries(this->get_omega_peak(),
			      step_omega,
			      step_theta,
			      integral_samples,
			      *this->dispersion);

  /*  Whole energy  */
  double energy;
  /*  Current boundary energies, to be sorted.  */
  class std::vector<class std::pair<double, enum boundary_direction> >
    boundary_energies;
  /*  Temp  */
  double temp;


  /****  Variable initializations  ****/

  energy = boundaries.init(top, right, bottom, omega_min);

  /**  Boundary energy vector  **/

  /*  Top  */
  temp = boundaries.add_top(top);
  boundary_energies.push_back(std::make_pair(temp, Top));
  energy += temp;
  /*  Right  */
  temp = boundaries.add_right(right, bottom);
  boundary_energies.push_back(std::make_pair(temp, Right));
  energy += temp;
  /*  Bottom  */
  temp = boundaries.add_bottom(bottom);
  boundary_energies.push_back(std::make_pair(temp, Bottom));
  energy += temp;


  /****  Computation  ****/

  while (energy < integral * 0.95)
    {
      std::sort(boundary_energies.begin(), boundary_energies.end());

      switch (boundary_energies.back().second)
	{
	case Top:
	  temp = boundaries.add_top(top);
	  boundary_energies.back().first = temp;
	  if (top > omega_max - step_omega)
	    {
	      throw "Find boundaries: omega range is too small.";
	    }
	  break;

	case Right:
	  temp = boundaries.add_right(right, bottom);
	  boundary_energies.back().first = temp;
	  if (right > theta_max - step_theta)
	    {
	      boundary_energies.pop_back();
	    }
	  break;

	case Bottom:
	  temp = boundaries.add_bottom(bottom);
	  boundary_energies.back().first = temp;
	  if (bottom < omega_min + step_omega)
	    {
	      boundary_energies.pop_back();
	    }
	  break;
	}
      energy += temp;
    }


  /****  Frees memory.  ****/
  delete &integral_samples;
}
