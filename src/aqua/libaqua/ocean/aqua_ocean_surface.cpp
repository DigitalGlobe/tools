/*
  This file is part of “The Aqua library”.

  Copyright © 2003 2004 2005  Jocelyn Fréchot

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


#include "aqua_ocean_surface.h"

#include "aqua_array.h"
#include "aqua_array_1.h"
#include "aqua_ocean_compute.h"
#include "aqua_ocean_wave_set.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua;
using namespace aqua::ocean;


/****************  public functions  ****************/


/*  creates a surface and sets it at time 0.0  */
Surface::Surface(class Wave_Set * wave_set, class Compute * compute)
  : wave_set(wave_set),
    compute(compute),
    time(0.0)
{
}


Surface::~Surface(void)
{
  delete this->wave_set;
  delete this->compute;
}


/****  set  ****/

void
Surface::reset(size_t size_x, size_t size_y)
{
  this->compute->reset(size_x, size_y);

  this->compute->update(*this->wave_set, this->time);
}


void
Surface::set_length(float length_x, float length_y)
{
  this->compute->set_length(length_x, length_y);

  this->compute->update(*this->wave_set, this->time);
}


void
Surface::set_time(float time)
{
  this->time = time;

  this->compute->update(*this->wave_set, time);
}


/*  if depth == 0.0, doesn't take it into account (deep water)  */
void
Surface::set_depth(float depth)
{
  this->wave_set->set_depth(depth);

  this->compute->update(*this->wave_set, this->time);
}


void
Surface::set_displacement_factor(float displacement_factor)
{
  this->compute->set_displacement_factor(displacement_factor);

  this->compute->update(*this->wave_set, this->time);
}


void
Surface::set_computed_surface(bool computed_surface)
{
  bool computed_surface_old;

  computed_surface_old = this->compute->get_computed_surface();

  this->compute->set_computed_surface(computed_surface);

  if (!computed_surface_old && computed_surface)
    {
      this->compute->update(*this->wave_set, this->time);
    }
}


void
Surface::set_computed_normals(bool computed_normals)
{
  bool computed_normals_old;

  computed_normals_old = this->compute->get_computed_normals();

  this->compute->set_computed_normals(computed_normals);

  if (!computed_normals_old && computed_normals)
    {
      this->compute->update(*this->wave_set, this->time);
    }
}


void
Surface::set_computed_eigenvalues(bool computed_eigenvalues)
{
  bool computed_eigenvalues_old;

  computed_eigenvalues_old = this->compute->get_computed_eigenvalues();

  this->compute->set_computed_eigenvalues(computed_eigenvalues);

  if (!computed_eigenvalues_old && computed_eigenvalues)
    {
      this->compute->update(*this->wave_set, this->time);
    }
}


/*  spectrum  */

void
Surface::set_smallest_wavelength(float smallest_wavelength)
{
//   this->spectrum->set_smallest_wavelength(smallest_wavelength);
//   this->set_spectrum(*this->spectrum);
}


void
Surface::set_largest_wavelength(float largest_wavelength)
{
//   this->spectrum->set_largest_wavelength(largest_wavelength);
//   this->set_spectrum(*this->spectrum);
}



void
Surface::set_spectrum_factor(float spectrum_factor)
{
//   this->spectrum->set_spectrum_factor(spectrum_factor);
//   this->set_spectrum(*this->spectrum);
}



void
Surface::set_wind_speed(float wind_speed)                   /*  m.s^-1   */
{
//   this->spectrum->set_wind_speed(wind_speed);
//   this->set_spectrum(*this->spectrum);
}



void
Surface::set_wind_angle(float wind_angle)                   /*  radians  */
{
//   this->spectrum->set_wind_angle(wind_angle);
//   this->set_spectrum(*this->spectrum);
}


/**/

void
Surface::set_spectrum(const class Spectrum & spectrum)
{
//   this->wave_set->set_spectrum(spectrum);

//   this->compute->update(*this->wave_set, this->time);
}


/****  get  *****/

const class Array<3> &
Surface::get_surface(void) const
{
  return this->compute->get_surface();
}


const class Array<3> &
Surface::get_normals(void) const
{
  return this->compute->get_normals();
}


/*  Negative eigenvalues indicate overlaps.  */
const class Array<1> &
Surface::get_overlaps(void) const
{
  return this->compute->get_eigenvalues();
}


float
Surface::get_time(void) const
{
  return this->time;
}


size_t
Surface::get_size_x(void) const
{
  return this->compute->get_size_x();
}


size_t
Surface::get_size_y(void) const
{
  return this->compute->get_size_y();
}


float
Surface::get_length_x(void) const
{
  return this->compute->get_length_x();
}


float
Surface::get_length_y(void) const
{
  return this->compute->get_length_y();
}


float
Surface::get_resolution_x(void) const
{
  return this->compute->get_resolution_x();
}


float
Surface::get_resolution_y(void) const
{
  return this->compute->get_resolution_y();
}


float
Surface::get_depth(void) const
{
  return this->wave_set->get_depth();
}


float
Surface::get_displacement_factor(void) const
{
  return this->compute->get_displacement_factor();
}


bool
Surface::get_computed_surface(void) const
{
  return this->compute->get_computed_surface();
}


bool
Surface::get_computed_normals(void) const
{
  return this->compute->get_computed_normals();
}


bool
Surface::get_computed_eigenvalues(void) const
{
  return this->compute->get_computed_eigenvalues();
}
