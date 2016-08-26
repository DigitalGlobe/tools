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


#ifndef AQUA_OCEAN_GERSTNER_SURFACE_H
#define AQUA_OCEAN_GERSTNER_SURFACE_H


/****************  Includes  ****************/


#include <libaqua/aqua_ocean_surface.h>

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
      class Compute;
      class Wave_Set;
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

      class Surface : public aqua::ocean::Surface
      {
      public:

	/*  creates a surface and set it at time 0.0  */
	Surface(const char * wave_file,
		class Compute * compute,
		size_t waves_number_current,
		float wind_angle,
		float depth,
		float loop_time = 0.0);

	virtual ~Surface(void) = 0;


	/****  Get  ****/
	class Base_Surface & get_base_surface(void);
	bool get_adaptive(void) const;

	/****  Set  ****/
	virtual void set_number_of_waves(size_t number_of_waves) = 0;
	//void set_adaptive(bool adaptive);


      protected:

	class Wave_Set * const wave_set_gerstner;
	class Compute * const compute_gerstner;


      private:

	/****  not defined  ****/
	Surface(const class Surface &);
	void operator =(const class Surface &);
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_GERSTNER_SURFACE_H  */
