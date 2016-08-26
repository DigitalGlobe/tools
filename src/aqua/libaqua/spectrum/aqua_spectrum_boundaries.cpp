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


#include "aqua_spectrum_boundaries.h"

#include "spectrum/aqua_spectrum_dispersion.h"
#include "spectrum/aqua_spectrum_samples.h"

/*  C++ lib  */
#include <deque>
#include <vector>

/*  C lib  */
#include <cmath>    /*  ceil()  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::spectrum;


/****************  public functions  ****************/


Boundaries::Boundaries(const double omega_peak,
		       const double step_omega,
		       const double step_theta,
		       const class spectrum::Samples & integral_samples,
		       const class spectrum::Dispersion & dispersion)
  : omega_peak(omega_peak),
    step_omega(step_omega),
    step_theta(step_theta),
    integral_samples(integral_samples),
    dispersion(dispersion),
    /*  Initialized in “this->init()”  */
    index_top(0),
    index_bottom(0)
{
}


double
Boundaries::init(double & top,
		 double & right,
		 double & bottom,
		 const double omega_min)
{
  double energy;
  double temp;


  /*  Current boundary values  */
  top    = this->omega_peak + this->step_omega;
  right  = this->step_theta;
  bottom = this->omega_peak - this->step_omega;

  /*  “integral_samples” indices  */
  this->index_top    = static_cast<size_t>(ceil((this->omega_peak - omega_min)
						/ this->step_omega));
  this->index_bottom = this->index_top - 1;

  /*  Boundary containers  */
  this->boundary_top.push_back(this->dispersion.compute(0.0, top));
  this->boundary_top.push_back(this->dispersion.compute(right, top));

  this->boundary_bottom.push_back(this->dispersion.compute(0.0, bottom));
  this->boundary_bottom.push_back(this->dispersion.compute(right, bottom));

  this->boundary_right.push_back(boundary_bottom.back());
  this->boundary_right.push_back(this->dispersion.compute(right, omega_peak));
  this->boundary_right.push_back(boundary_top.back());

  /*  Current energy  */
  temp = this->dispersion.compute(0.0, this->omega_peak);
  energy =
    this->integral_samples[this->index_top   ] * (this->boundary_top[0]
						  + this->boundary_top[1]
						  + temp
						  + this->boundary_right[1]);
  energy +=
    this->integral_samples[this->index_bottom] * (temp
						  + this->boundary_right[1]
						  + this->boundary_bottom[0]
						  + this->boundary_bottom[1]);

  return energy;
}


double
Boundaries::add_top(double & top)
{
  double energy;
  double theta;
  double temp_boundary_1, temp_boundary_2;

  energy = 0.0;

  top += this->step_omega;
  this->index_top++;

  theta = 0.0;
  temp_boundary_1 = this->boundary_top[0];
  this->boundary_top[0] = this->dispersion.compute(theta, top);
  for (size_t i = 1; i < this->boundary_top.size(); i++)
    {
      theta += this->step_theta;
      temp_boundary_2 = this->boundary_top[i];
      this->boundary_top[i] = this->dispersion.compute(theta, top);

      energy +=
	temp_boundary_1
	+ temp_boundary_2
	+ this->boundary_top[i - 1]
	+ this->boundary_top[i];

      temp_boundary_1 = temp_boundary_2;
    }

  this->boundary_right.push_back(this->boundary_top.back());

  return energy * this->integral_samples[this->index_top];
}


double
Boundaries::add_right(double & right, const double bottom)
{
  double energy;
  double omega;
  size_t temp_index;
  double temp_boundary_1, temp_boundary_2;

  energy = 0.0;

  right += this->step_theta;

  temp_index = this->index_bottom;

  omega = bottom;
  temp_boundary_1 = this->boundary_right[0];
  this->boundary_right[0] = this->dispersion.compute(right, omega);
  for (size_t i = 1; i < this->boundary_right.size(); i++)
    {
      omega += this->step_omega;
      temp_boundary_2 = this->boundary_right[i];
      this->boundary_right[i] = this->dispersion.compute(right, omega);

      energy +=
	this->integral_samples[temp_index] * (temp_boundary_1
					      + temp_boundary_2
					      + this->boundary_right[i - 1]
					      + this->boundary_right[i]);

      temp_boundary_1 = temp_boundary_2;
      temp_index++;
    }

  this->boundary_bottom.push_back(this->boundary_right.front());
  this->boundary_top.push_back(this->boundary_right.back());

  return energy;
}


double
Boundaries::add_bottom(double & bottom)
{
  double energy;
  double theta;
  double temp_boundary_1, temp_boundary_2;

  energy = 0.0;

  bottom -= this->step_omega;
  this->index_bottom--;

  theta = 0.0;
  temp_boundary_1 = this->boundary_bottom[0];
  this->boundary_bottom[0] = this->dispersion.compute(theta, bottom);
  for (size_t i = 1; i < this->boundary_bottom.size(); i++)
    {
      theta += this->step_theta;
      temp_boundary_2 = this->boundary_bottom[i];
      this->boundary_bottom[i] = this->dispersion.compute(theta, bottom);

      energy +=
	temp_boundary_1
	+ temp_boundary_2
	+ this->boundary_bottom[i - 1]
	+ this->boundary_bottom[i];

      temp_boundary_1 = temp_boundary_2;
    }

  this->boundary_right.push_front(this->boundary_bottom.back());

  return energy * this->integral_samples[index_bottom];
}
