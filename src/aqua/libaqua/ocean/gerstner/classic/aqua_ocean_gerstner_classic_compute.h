/*  Emacs mode: -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2005 2006 Jocelyn Fréchot

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


#ifndef AQUA_OCEAN_GERSTNER_CLASSIC_COMPUTE_H
#define AQUA_OCEAN_GERSTNER_CLASSIC_COMPUTE_H


/****************  Includes  ****************/


#include "aqua_ocean_gerstner_compute.h"

#include "aqua_array.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  Classes  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace gerstner
    {
      namespace classic
      {

/*  no const  */

class Compute : public gerstner::Compute
{
public:

  /*  Non-adaptive  */
  Compute(size_t size_x,
	  size_t size_y,
	  float length_x,
	  float length_y,
	  float displacement_factor,
	  bool normalized_normals);
  /*  Adaptive  */
  Compute(size_t size_x,
	  size_t size_y,
	  float displacement_factor,
	  bool normalized_normals);

  virtual ~Compute(void);


  /****  Set  ****/
  virtual void reset(size_t size_x, size_t size_y);
  virtual void set_displacement_factor(float displacement_factor);

  /****  Update  ****/
  virtual void update(const class aqua::ocean::Wave_Set & wave_set, float time);


protected:

  std::vector<float> base_sine;
  std::vector<float> base_cosine;
//  	float * base_sine;
//  	float * base_cosine;

  float * t_s_n;
  float * t_s_n_1;
  float * t_c_n;
  float * t_c_n_1;


  void init_surface(const class Base_Surface & base_surface);
  void init_normals(float displacement_factor);
  void init_derivatives(void);
  void init_eigenvalues(float displacement_factor);

  /**  Fills “base_sine” and “base_cosine”.  */
  void fill_bases(const class Base_Surface & base_surface,
		  size_t size,
		  float wave_vector_x,
		  float wave_vector_y,
		  float angular_velocity,
		  float phase_base);
  void fill_bases_adaptive(const class Base_Surface & base_surface,
			   size_t size,
			   float wave_vector_x,
			   float wave_vector_y,
			   float angular_velocity,
			   float phase_base);

  /****  Compute  ****/

  void compute_surface( const std::vector<float> & base_sine,
		       const std::vector<float> & base_cosine,
		       // const float * base_sine,
//  			     const float * base_cosine,
		       size_t size,
		       float wave_vector_unit_x,
		       float wave_vector_unit_y,
		       float displacement_factor);
  void compute_normals(const std::vector<float> & base_sine,
		       // const float * base_sine,
		       size_t size,
		       float wave_vector_x,
		       float wave_vector_y);
  void compute_normals(std::vector<float> & derivative_x_dx,
		       std::vector<float> & derivative_x_dy,
		       std::vector<float> & derivative_y_dy,
		       const std::vector<float> & base_cosine,
		       // const float * base_cosine,
		       size_t size,
		       float wave_vector_x_unit_x,
		       float wave_vector_y_unit_y,
		       float wave_vector_x_unit_y,
		       float displacement_factor);
  /*  “normals” must be filled with derivatives.  */
  void finalize_normals(class aqua::Array<3> & normals,
			const std::vector<float> & derivative_x_dx,
			const std::vector<float> & derivative_x_dy,
			const std::vector<float> & derivative_y_dy,
			size_t size);


private:

  /****  Not defined  ****/
  Compute(const class  Compute &);
  void operator =(const class Compute &);
};

      }
    }
  }
}


#endif  /*  AQUA_OCEAN_GERSTNER_CLASSIC_COMPUTE_H  */
