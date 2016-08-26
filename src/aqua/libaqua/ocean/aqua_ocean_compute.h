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


#ifndef AQUA_OCEAN_COMPUTE_H
#define AQUA_OCEAN_COMPUTE_H


/****************  includes  ****************/


#include "aqua_array.h"
#include "aqua_array_1.h"

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  class declarations  ****************/


namespace aqua
{
  namespace ocean
  {
    class Wave_Set;
  }
}


/****************  abstract classes  ****************/


namespace aqua
{
  namespace ocean
  {

    class Compute
    {
    public:

      Compute(size_t size_x,
	      size_t size_y,
	      float length_x,
	      float length_y,
	      float resolution_x,
	      float resolution_y,
	      float displacement_factor,
	      bool normalized_normals);

      virtual ~Compute(void) = 0;


      /****  set  ****/
      virtual void reset(size_t size_x, size_t size_y) = 0;
      virtual void set_length(float length_x, float length_y) = 0;
      virtual void set_displacement_factor(float displacement_factor);
      void set_computed_surface(bool computed_surface);
      void set_computed_normals(bool computed_normals);
      void set_computed_eigenvalues(bool computed_eigenvalues);

      /****  get  ****/
      const class Array<3> & get_surface(void) const;
      const class Array<3> & get_normals(void) const;
      const class Array<1> & get_eigenvalues(void) const;
      virtual size_t get_size_x(void) const;
      virtual size_t get_size_y(void) const;
      float get_length_x(void) const;
      float get_length_y(void) const;
      float get_resolution_x(void) const;
      float get_resolution_y(void) const;
      float get_displacement_factor(void) const;
      bool get_computed_surface(void) const;
      bool get_computed_normals(void) const;
      bool get_computed_eigenvalues(void) const;
      bool get_normalized_normals(void) const;

      /****  update  ****/
      virtual void update(const class Wave_Set & wave_set, float time) = 0;


    protected:

      const bool normalized_normals;

      class Array<3> * surface;
      class Array<3> * normals;
      class Array<1> * eigenvalues;

      size_t size_x;
      size_t size_y;
      float length_x;
      float length_y;
      float resolution_x;            /* resolution of the surface, meters */
      float resolution_y;
      float displacement_factor;  /*  horizontal displacement factor  */
      bool computed_surface;
      bool computed_normals;
      bool computed_eigenvalues;

      /****  compute  ****/
      float compute_resolution(size_t size, float length) const;

      /****  set  ****/
      void reset(size_t size_x,
		 size_t size_y,
		 float resolution_x,
		 float resolution_y);
      void set_length(float length_x,
		      float length_y,
		      float resolution_x,
		      float resolution_y);
      void normalize_normals(size_t size_x, size_t size_y);


    private:

      /****  not defined  ****/
      Compute(const class  Compute &);
      void operator =(const class Compute &);
    };

  }
}


#endif  /*  AQUA_OCEAN_COMPUTE_H  */
