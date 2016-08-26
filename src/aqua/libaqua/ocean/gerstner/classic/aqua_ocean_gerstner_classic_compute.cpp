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

/**
   \file
   Field computations.
*/


/****************  includes  ****************/


#include "aqua_ocean_gerstner_classic_compute.h"

#include "aqua_ocean_gerstner_base_surface.h"
#include "aqua_ocean_gerstner_wave_set.h"

/*  libaqua  */
#include "aqua_array.h"

/*  local includes  */
#include "vector.h"
#include "sincos.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  namespaces  ****************/


using namespace aqua::ocean::gerstner::classic;


/****************  public functions  ****************/


/*  Non-adaptive  */
Compute::Compute(size_t size_x,
		 size_t size_y,
		 float length_x,
		 float length_y,
		 float displacement_factor,
		 bool normalized_normals)
  : aqua::ocean::gerstner::Compute(size_x,
				   size_y,
				   length_x,
				   length_y,
				   displacement_factor,
				   normalized_normals),
    base_sine(this->size),
    base_cosine(this->size),
//     base_sine(new float [this->size]),
//     base_cosine(new float [this->size]),
    t_s_n(new float [size_x]),
    t_s_n_1(new float [size_x]),
    t_c_n(new float [size_x]),
    t_c_n_1(new float [size_x])
{
  this->init_eigenvalues(displacement_factor);
}


/*  Adaptive  */
Compute::Compute(size_t size_x,
		 size_t size_y,
		 float displacement_factor,
		 bool normalized_normals)
  : aqua::ocean::gerstner::Compute(size_x,
				   size_y,
				   displacement_factor,
				   normalized_normals),
    base_sine(this->size),
    base_cosine(this->size)
{
  this->init_eigenvalues(displacement_factor);
}


/*  Virtual  */
Compute::~Compute(void)
{
}


/****  Set  ****/


void
Compute::reset(size_t size_x, size_t size_y)
{ 
  this->aqua::ocean::gerstner::Compute::reset(size_x, size_y);

  this->base_sine.resize(this->size);
  this->base_cosine.resize(this->size);
//   delete [] base_sine;
//   delete [] base_cosine;
//   base_sine = new float [this->size];
//   base_cosine = new float [this->size];

  delete [] t_s_n;
  delete [] t_s_n_1;
  delete [] t_c_n;
  delete [] t_c_n_1;

  t_s_n = new float [size_x];
  t_s_n_1 = new float [size_x];
  t_c_n = new float [size_x];
  t_c_n_1 = new float [size_x];

  this->init_eigenvalues(this->displacement_factor);
}


void
Compute::set_displacement_factor(float displacement_factor)
{
  this->aqua::ocean::Compute::set_displacement_factor(displacement_factor);
  this->init_eigenvalues(displacement_factor);
}


/****  Update  ****/


void
Compute::update(const class aqua::ocean::Wave_Set & wave_set, float time)
{
  float temp_amplitude, temp_phase;


  if (   this->computed_surface
      || this->computed_normals
      || this->computed_eigenvalues)
    {
      const class Wave_Set & wave_set_gerstner =
	dynamic_cast<const class Wave_Set &>(wave_set);

      size_t i;


      if (this->computed_surface)
	{
	  this->init_surface(*this->base_surface);
	}

      if (this->computed_normals)
	{
	  this->init_normals(this->displacement_factor);

	  if (std::isnormal(this->displacement_factor) != 0)
	    {
	      this->init_derivatives();
	    }
	}

      for (i = 0; i < wave_set_gerstner.get_size_current(); i++)
	{
	  //  amplitude and random_amplitude are always used together?
	  temp_amplitude =
	    wave_set.get_amplitude()[i]
	    * wave_set_gerstner.get_random_amplitude()[i];
	  temp_phase =
	    - wave_set.get_angular_velocity()[i] * time
	    + wave_set_gerstner.get_random_phase()[i];

	  if (this->adaptive)
	    {
	      this->fill_bases_adaptive
		(*this->base_surface,
		 this->size,
		 wave_set.get_wave_vector_x()[i],
		 wave_set.get_wave_vector_y()[i],
		 temp_amplitude,
		 temp_phase);
	    }
	  else
	    {
	      this->fill_bases(*this->base_surface,
			       this->size,
			       wave_set.get_wave_vector_x()[i],
			       wave_set.get_wave_vector_y()[i],
			       temp_amplitude,
			       temp_phase);
	    }

	  if (this->computed_surface)
	    {
	      this->compute_surface(this->base_sine,
				    this->base_cosine,
				    this->size,
				    wave_set.get_wave_vector_unit_x()[i],
				    wave_set.get_wave_vector_unit_y()[i],
				    this->displacement_factor);
	    }

	  if (this->computed_normals || this->computed_eigenvalues)
	    {
	      this->compute_normals(this->base_sine,
				    this->size,
				    wave_set.get_wave_vector_x()[i],
				    wave_set.get_wave_vector_y()[i]);

	      if (std::isnormal(this->displacement_factor) != 0)
		{
		  this->
		    compute_normals(this->derivative_x_dx,
				    this->derivative_x_dy,
				    this->derivative_y_dy,
				    this->base_cosine,
				    this->size,
				    wave_set.get_wave_vector_x_unit_x()[i],
				    wave_set.get_wave_vector_y_unit_y()[i],
				    wave_set.get_wave_vector_x_unit_y()[i],
				    this->displacement_factor);
		}
	    }
	}

      if (this->computed_normals)
	{
	  if (std::isnormal(this->displacement_factor) != 0)
	    {
	      this->finalize_normals(*this->normals,
				     this->derivative_x_dx,
				     this->derivative_x_dy,
				     this->derivative_y_dy,
				     this->size);
	    }

	  this->normalize_normals(this->size_x, this->size_y);
	}
    }
}


/****************  protected functions  ****************/


void
Compute::init_surface(const class Base_Surface & base_surface)
{
  size_t i;

  for (i = 0; i < this->size; i++)
    {
      this->surface->get(0, i) = base_surface.surface.get(0, i);
      this->surface->get(1, i) = base_surface.surface.get(1, i);
      this->surface->get(2, i) = 0.0;
    }
}


void
Compute::init_normals(float displacement_factor)
{
  size_t i;

  for (i = 0; i < this->size; i++)
    {
      this->normals->get(0, i) = 0.0;
      this->normals->get(1, i) = 0.0;

      if (std::isnormal(displacement_factor) == 0)
	{
	  /*  used for derivatives  */
	  this->normals->get(2, i) = 1.0;
	}
    }
}


void
Compute::init_derivatives(void)
{
  /*  fills vectors  */
  for (size_t i = 0; i < this->size; i++)
    {
      this->derivative_x_dx[i] = 1.0;
      this->derivative_y_dy[i] = 1.0;
      this->derivative_x_dy[i] = 0.0;
    }
}


void
Compute::init_eigenvalues(float displacement_factor)
{
  if (std::isnormal(displacement_factor) == 0)
    {
      size_t i;

      // eigenvalues->fill(1.0);
      for (i = 0; i < this->size; i++)
	{
	  this->eigenvalues->get(i) = 1.0;
	}
    }
}


/**  Fills “base_sine” and “base_cosine”.  */
void
Compute::fill_bases(const class Base_Surface & base_surface,
		    size_t size,
		    float wave_vector_x,
		    float wave_vector_y,
		    float amplitude,
		    float phase_base)
{
  const float delta_x = wave_vector_x * this->resolution_x;
  const float delta_y = wave_vector_y * this->resolution_y;

  float temp_phase_spatial_x, temp_phase_spatial_y;
  float temp_sin, temp_cos;
  size_t index;


  /*  Starts at 0.0.  */
  temp_phase_spatial_x = phase_base;
  temp_phase_spatial_y = phase_base;
  index = 0;

  for (size_t i = 0; i < size_y; i++)
    {
      for (size_t j = 0; j < size_x; j++)
	{
	  aqua_math::sincosf(temp_phase_spatial_x,
			     &temp_sin,
			     &temp_cos);

	  this->base_sine[index]   = amplitude * temp_sin;
	  this->base_cosine[index] = amplitude * temp_cos;

	  temp_phase_spatial_x += delta_x;

	  index++;
	}
      temp_phase_spatial_y += delta_y;
      temp_phase_spatial_x = temp_phase_spatial_y;
    }
}

void
Compute::fill_bases_adaptive(const class Base_Surface & base_surface,
			     size_t size,
			     float wave_vector_x,
			     float wave_vector_y,
			     float amplitude,
			     float phase_base)
{
  float temp_phase_spatial_x;
  float delta_x;
  float temp_sin, temp_cos;
  size_t index;


  index = 0;

  for (size_t i = 0; i < size_y; i++)
    {
      temp_phase_spatial_x =
	phase_base
	+ aqua_math::vector_scalar_product(wave_vector_x,
					   wave_vector_y,
					   base_surface.surface.get(0, index),
					   base_surface.surface.get(1, index));

      delta_x =
	wave_vector_x
	* (base_surface.surface.get(0, i, 1)
	   - base_surface.surface.get(0, i, 0))
	+ wave_vector_y
	* (base_surface.surface.get(1, i, 1)
	   - base_surface.surface.get(1, i, 0));

      for (size_t j = 0; j < size_x; j++)
	{
	  aqua_math::sincosf(temp_phase_spatial_x,
			     &temp_sin,
			     &temp_cos);

	  this->base_sine[index]   = amplitude * temp_sin;
	  this->base_cosine[index] = amplitude * temp_cos;

	  temp_phase_spatial_x += delta_x;

	  index++;
	}
    }
}


void
Compute::compute_surface(const std::vector<float> & base_sine,
			 const std::vector<float> & base_cosine,
			 // const float * base_sine,
			 // const float * base_cosine,
			 size_t size,
			 float wave_vector_unit_x,
			 float wave_vector_unit_y,
			 float displacement_factor)
{
  size_t i;

  for (i = 0; i < size; i++)
    {
      this->surface->get(2, i) -= base_cosine[i];
    }

  /*  doesn't compute horizontal coordinates if there is no displacement  */
  if (std::isnormal(this->displacement_factor) != 0)
    {
      for (i = 0; i < size; i++)
	{
	  this->surface->get(0, i) +=
	    displacement_factor * wave_vector_unit_x * base_sine[i];

	  this->surface->get(1, i) +=
	    displacement_factor * wave_vector_unit_y * base_sine[i];
	}
    }
}


void
Compute::compute_normals(const std::vector<float> & base_sine,
			 //const float * base_sine,
			 size_t size,
			 float wave_vector_x,
			 float wave_vector_y)
{
  size_t i;

  for (i = 0; i < size; i++)
    {
      this->normals->get(0, i) -= wave_vector_x * base_sine[i];
      this->normals->get(1, i) -= wave_vector_y * base_sine[i];
    }
}


void
Compute::compute_normals(std::vector<float> & derivative_x_dx,
			 std::vector<float> & derivative_x_dy,
			 std::vector<float> & derivative_y_dy,
			 const std::vector<float> & base_cosine,
			 //const float * base_cosine,
			 size_t size,
			 float wave_vector_x_unit_x,
			 float wave_vector_y_unit_y,
			 float wave_vector_x_unit_y,
			 float displacement_factor)
{
  size_t i;

  for (i = 0; i < size; i++)
    {
      derivative_x_dx[i] -=
	displacement_factor * (- wave_vector_x_unit_x * base_cosine[i]);
      derivative_x_dy[i] -=
	displacement_factor * (- wave_vector_x_unit_y * base_cosine[i]);
      derivative_y_dy[i] -=
	displacement_factor * (- wave_vector_y_unit_y * base_cosine[i]);
    }
}


/*  “normals” must be filled with derivatives  */
void
Compute::finalize_normals(class aqua::Array<3> & normals,
			  const std::vector<float> & derivative_x_dx,
			  const std::vector<float> & derivative_x_dy,
			  const std::vector<float> & derivative_y_dy,
			  size_t size)
{
  size_t i;

  for (i = 0; i < size; i++)
    {
      aqua_math::vector_product(derivative_x_dx[i],
				derivative_x_dy[i], /* ∂y ∕ ∂x₀ == ∂x ∕ ∂y₀ */
				- normals.get(0, i),      /*  ∂z ∕ ∂x₀  */
				derivative_x_dy[i],
				derivative_y_dy[i],
				- normals.get(1, i),      /*  ∂z ∕ ∂y₀  */
				normals.get_element(i));
    }
}
