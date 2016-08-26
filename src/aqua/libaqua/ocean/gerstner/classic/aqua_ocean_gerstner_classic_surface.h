/*  Emacs mode: -*- C++ -*-  */
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


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_OCEAN_GERSTNER_CLASSIC_SURFACE_H
#define AQUA_OCEAN_GERSTNER_CLASSIC_SURFACE_H


/****************  Includes  ****************/


#include <libaqua/aqua_ocean_gerstner_surface.h>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  Class declarations  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace gerstner
    {
      namespace classic
      {
	class Compute;
      }
    }
  }
}


/****************  Polymorphic classes  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace gerstner
    {
      namespace classic
      {

class Surface : public gerstner::Surface
{
public:

  /*  creates a surface and set it at time 0.0  */
  /*  Non-adaptive  */
  Surface(const char * wave_file,
	  size_t waves_number_current,
	  size_t size_x,
	  size_t size_y,
	  float length_x,
	  float length_y,
	  float wind_angle,
	  float depth,
	  float displacement_factor,
	  bool normalized_normals,
	  float loop_time = 0.0);
  /*  Adaptive  */
  Surface(const char * wave_file,
	  size_t waves_number_current,
	  size_t size_x,
	  size_t size_y,
	  float wind_angle,
	  float depth,
	  float displacement_factor,
	  bool normalized_normals,
	  float loop_time = 0.0);

  virtual ~Surface(void);

  /****  Set  ****/
  virtual void set_number_of_waves(size_t number_of_waves);


protected:

  class Compute * compute_classic;


private:

  /****  Not defined  ****/
  Surface(const class Surface &);
  void operator =(const class Surface &);
};

      }
    }
  }
}


#endif  /*  AQUA_OCEAN_GERSTNER_CLASSIC_SURFACE_H  */
