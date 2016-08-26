/*  Emacs mode: -*- C++ -*-  */
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


#ifndef AQUA_OCEAN_GERSTNER_WAVE_SET_H
#define AQUA_OCEAN_GERSTNER_WAVE_SET_H


/****************  Includes  ****************/


#include "ocean/aqua_ocean_wave_set.h"
//#include "spectrum_tool/aqua_spectrum_tool_wave_polar.h"

/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  Class declarations  ****************/


namespace aqua
{
  namespace spectrum_tool
  {
    class Wave_Polar;
  }
}


/****************  Polymorphic classes  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace gerstner
    {

      class Wave_Set : public aqua::ocean::Wave_Set
      {
      public:

	Wave_Set(const char * wave_file,
		 size_t size_current,
		 float wind_angle,
		 float depth,                         /* meters */
		 float loop_time);       /* rad.s-1 */

	virtual ~Wave_Set(void);


	/****  set  ****/
	void set_size_current(size_t size);

	/****  get  ****/
	size_t get_size_current(void) const;
	const std::vector<float> & get_random_phase(void) const;
	/**/
	const std::vector<float> & get_random_amplitude(void) const;


      protected:

	mutable size_t size_current;

	float wind_angle;

	class std::vector<size_t> waves_indices;
	class std::vector<class spectrum_tool::Wave_Polar> waves_all;
	class std::vector<float> random_phase;
	/**/
	class std::vector<float> random_amplitude;

	/*
	  Sets “size” and fills “amplitude”, “angular_velocity”,
	  “wave_vector_x” and “wave_vector_y”
	*/
	void
	fill_wave_vector(const std::vector<size_t> & waves_indices,
			 const std::vector<spectrum_tool::Wave_Polar> & waves_all,
			 const size_t size,
			 const float wind_angle);
	void resize_and_fill_random_phase(const size_t size);


      private:

	/****  not defined  ****/
	Wave_Set(const class Wave_Set &);
	void operator =(const class Wave_Set &);
      };

    }
  }
}


#endif  /*  AQUA_OCEAN_GERSTNER_WAVE_SET_H  */
