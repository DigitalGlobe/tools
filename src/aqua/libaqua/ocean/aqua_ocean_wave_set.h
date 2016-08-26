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


#ifndef AQUA_OCEAN_WAVE_SET_H
#define AQUA_OCEAN_WAVE_SET_H


/****************  includes  ****************/


/*  C++ lib  */
#include <vector>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace ocean
  {

    class Wave_Set
    {
    public:

      Wave_Set(size_t size, float depth, float loop_time);

      virtual ~Wave_Set(void);


      /****  Set  ****/
      void set_depth(float depth);
      void set_spectrum(const class Spectrum & spectrum);

      /****  Get  ****/
      float get_depth(void) const;
      size_t get_size(void) const;
      const std::vector<float> & get_wavenumber(void) const;
      const std::vector<float> & get_length(void) const;
      const std::vector<float> & get_angular_velocity(void) const;
      const std::vector<float> & get_amplitude(void) const;
      const std::vector<float> & get_wave_vector_x(void) const;
      const std::vector<float> & get_wave_vector_y(void) const;
      const std::vector<float> & get_wave_vector_unit_x(void) const;
      const std::vector<float> & get_wave_vector_unit_y(void) const;
      const std::vector<float> & get_wave_vector_x_unit_x(void) const;
      const std::vector<float> & get_wave_vector_y_unit_y(void) const;
      const std::vector<float> & get_wave_vector_x_unit_y(void) const;


    protected:

      const float basic_angular_velocity; /* used if loop_time != 0, rad.s-1*/

      std::vector<float> wavenumber;
      std::vector<float> length;
      std::vector<float> angular_velocity;
      std::vector<float> amplitude;
      // wavevector au lieu de wavenumber_vector
      std::vector<float> wave_vector_x;
      std::vector<float> wave_vector_y;
      std::vector<float> wave_vector_unit_x;
      std::vector<float> wave_vector_unit_y;
      std::vector<float> wave_vector_x_unit_x;
      std::vector<float> wave_vector_y_unit_y;
      std::vector<float> wave_vector_x_unit_y;

      size_t size;
      float depth;  /*  depth of water, meters (0.0 means infinite)  */


      /****  compute  ****/
      float compute_basic_angular_velocity(float loop_time) const;

      /****  fill  ****/
      /*  fills all vectors using wavenumber vectors  */
      void fill_all(const std::vector<float> & wave_vector_x,
		    const std::vector<float> & wave_vector_y);
      /*  fills only angular velocities vector  */
      //     void fill_angular_velocity(const std::vector<float> & wavenumber,
      // 			       float depth,
      // 			       float basic_angular_velocity);
      /*  fills only amplitudes vector  */
      //     void fill_amplitude(const class Spectrum & spectrum,
      // 			const std::vector<float> & wavenumber,
      // 			const std::vector<float> & wave_vector_unit_x,
      // 			const std::vector<float> & wave_vector_unit_y,
      // 			const std::vector<float> & length);

      /****  Set  ****/
      /*  Sets size and resizes all vectors.  */
      void set_size(size_t size);


    private:

      /****  not defined  ****/
      Wave_Set(const class Wave_Set &);
      void operator =(const class Wave_Set &);
    };

  }
}


#endif  /*  AQUA_OCEAN_WAVE_SET_H  */
