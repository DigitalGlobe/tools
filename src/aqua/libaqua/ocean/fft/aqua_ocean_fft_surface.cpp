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


#include "aqua_ocean_fft_surface.h"

#include "aqua_ocean_fft_compute.h"
#include "aqua_ocean_fft_type.h"
#include "aqua_ocean_fft_wave_set.h"

/*  libaqua  */
#include "rng/aqua_rng_rng.h"
#include "spectrum/aqua_spectrum_directional.h"

/*  C lib  */
#include <cmath>
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::ocean::fft;


/****************  public functions  ****************/


/*  creates a surface and sets it at time 0.0  */
Surface::Surface(class aqua::spectrum::Directional * spectrum,
		 class aqua::rng::Rng * rng,
		 enum type fft_type,
		 size_t size_x,
		 size_t size_y,
		 float length_x,
		 float length_y,
		 float depth,
		 float displacement_factor,
		 bool normalized_normals,
		 float loop_time)
  : aqua::ocean::Surface(new class Wave_Set(rng,
					    *spectrum,
					    size_x,
					    size_y,
					    length_x,
					    length_y,
					    depth,
					    loop_time),
			 new class Compute(fft_type,
					   size_x,
					   size_y,
					   length_x,
					   length_y,
					   displacement_factor,
					   normalized_normals)),
    spectrum(spectrum),
    wave_set_fft(dynamic_cast<class Wave_Set * const>(this->wave_set)),
    smallest_wavelength(this->compute_smallest_wavelength(size_x,
							  size_y,
							  length_x,
							  length_y)),
    largest_wavelength(this->compute_largest_wavelength(length_x, length_y))
{
}


/*  virtual  */
Surface::~Surface(void)
{
  delete this->spectrum;
}


/****  set  ****/

void
Surface::reset(size_t size_x, size_t size_y)
{
  this->wave_set_fft->set_size(*this->spectrum, size_x, size_y);
  this->smallest_wavelength =
    this->compute_smallest_wavelength(size_x,
				      size_y,
				      this->compute->get_length_x(),
				      this->compute->get_length_y());

  this->ocean::Surface::reset(size_x, size_y);
}


void
Surface::set_length(float length_x, float length_y)
{
  this->wave_set_fft->set_length(*this->spectrum, length_x, length_y);
  this->smallest_wavelength =
    this->compute_smallest_wavelength(this->compute->get_size_x(),
				      this->compute->get_size_y(),
				      length_x,
				      length_y);
  this->largest_wavelength =
    this->compute_largest_wavelength(length_x, length_y);

  this->ocean::Surface::set_length(length_x, length_y);
}


/****  get  ****/

enum type
Surface::get_fft_type(void) const
{
  return
    dynamic_cast<class aqua::ocean::fft::Compute *>(this->compute)->get_fft_type();
}


float
Surface::get_smallest_possible_wave(void) const
{
  return this->smallest_wavelength;
}


float
Surface::get_largest_possible_wave(void) const
{
  return this->largest_wavelength;
}


double
Surface::get_energy_ratio(void) const
{
  return this->wave_set_fft->get_energy_ratio();
}


/****************  protected functions  ****************/


/****  compute  ****/

float
Surface::compute_smallest_wavelength(float size_x,
				     float size_y,
				     float length_x,
				     float length_y) const
{
  float vector_max_x, vector_max_y;

  vector_max_x = (size_x / 2) / length_x;
  vector_max_y = (size_y / 2) / length_y;

  return 1.0 / hypotf(vector_max_x, vector_max_y);
}


float
Surface::compute_largest_wavelength(float length_x, float length_y) const
{
  return fmaxf(length_x, length_y);
}
