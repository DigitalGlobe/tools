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

/**
   \file
   Field computations.
*/


#ifndef AQUA_OCEAN_GERSTNER_COMPUTE_H
#define AQUA_OCEAN_GERSTNER_COMPUTE_H


/****************  Includes  ****************/


#include "ocean/aqua_ocean_compute.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  Class declarations  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace gerstner
    {
      class Base_Surface;
    }
  }
}


/****************  Abstract classes  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace gerstner
    {

      /*  No const  */

      class Compute : public aqua::ocean::Compute
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

	virtual ~Compute(void) = 0;


	/****  Get ****/
	class Base_Surface & get_base_surface(void);
	bool get_adaptive(void) const;

	/****  Set  ****/
	virtual void reset(size_t size_x, size_t size_y);
	virtual void set_length(float length_x, float length_y);
	virtual void set_displacement_factor(float displacement_factor) = 0;
	//void set_adaptive(bool adaptive);

	/****  Update  ****/
	virtual void update(const class aqua::ocean::Wave_Set & wave_set,
			    float time) = 0;


      protected:

	size_t size;

	class Base_Surface * base_surface;

	std::vector<float> derivative_x_dx;
	std::vector<float> derivative_y_dy;
	std::vector<float> derivative_x_dy;

	const bool adaptive;


	void fill_base_surface(size_t size_x,
			       size_t size_y,
			       float resolution_x,
			       float resolution_y);


      private:

	/****  Not defined  ****/
	Compute(const class  Compute &);
	void operator =(const class Compute &);
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_GERSTNER_COMPUTE_H  */
