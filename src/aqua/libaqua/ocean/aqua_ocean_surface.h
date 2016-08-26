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


#ifndef AQUA_OCEAN_SURFACE_H
#define AQUA_OCEAN_SURFACE_H


/****************  includes  ****************/


#include <libaqua/aqua_array.h>
#include <libaqua/aqua_array_1.h>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  polymorphic classes  ****************/


namespace aqua
{
  namespace ocean
  {
    class Surface
    {
    public:

      /*  creates a surface and set it at time 0.0  */
      Surface(class Wave_Set * wave_set, class Compute * compute);

      virtual ~Surface(void);


      /****  set  ****/
      virtual void reset(size_t size_x, size_t size_y);
      virtual void set_length(float length_x, float length_y);

      void set_time(float time);                               /*  seconds  */
      /*  if depth == 0.0, doesn't take it into account (infinite depth)  */
      void set_depth(float depth);                             /*  meters   */
      void set_displacement_factor(float displacement_factor);
      void set_computed_surface(bool computed_surface);
      void set_computed_normals(bool computed_normals);
      void set_computed_eigenvalues(bool computed_eigenvalues);

      /*  spectrum  */
      void set_smallest_wavelength(float smallest_wavelength);
      void set_largest_wavelength(float largest_wavelength);
      void set_spectrum_factor(float spectrum_factor);
      void set_wind_speed(float wind_speed);                   /*  m.s^-1   */
      void set_wind_angle(float wind_angle);                   /*  radians  */
      /**/
      void set_spectrum(const class Spectrum & spectrum);


      /****  get  ****/
      const class Array<3> & get_surface(void) const;
      const class Array<3> & get_normals(void) const;
      /*  Negative eigenvalues indicate overlaps.  */
      const class Array<1> & get_overlaps(void) const;

      float get_time(void) const;
      size_t get_size_x(void) const;
      size_t get_size_y(void) const;
      float get_length_x(void) const;
      float get_length_y(void) const;
      float get_resolution_x(void) const;
      float get_resolution_y(void) const;
      float get_depth(void) const;
      float get_displacement_factor(void) const;
      bool get_computed_surface(void) const;
      bool get_computed_normals(void) const;
      bool get_computed_eigenvalues(void) const;


    protected:

      class Wave_Set * const wave_set;
      class Compute * const compute;
 
      float time;  /*  time at which the surface is computed, seconds  */


    private:

      /****  not defined  ****/
      Surface(const class Surface &);
      void operator =(const class Surface &);
    };

  }
}


#endif  /*  AQUA_OCEAN_SURFACE_H  */
