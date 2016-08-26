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


#ifndef AQUA_SPECTRUM_BOUNDARIES_H
#define AQUA_SPECTRUM_BOUNDARIES_H


/****************  includes  ****************/


/*  C++ lib  */
#include <deque>
#include <vector>


/****************  class declarations  ****************/


namespace aqua
{
  namespace spectrum
  {
    class Samples;
    class Dispersion;
  }
}


/****************  external functions  ****************/


namespace aqua
{
  namespace spectrum
  {
    class Boundaries
    {
    public:

      Boundaries(const double omega_peak,
		 const double step_omega,
		 const double step_theta,
		 const class spectrum::Samples & integral_samples,
		 const class spectrum::Dispersion & dispersion);

      double init(double & top,
		  double & right,
		  double & bottom,
		  const double omega_min);

      double add_top(double & top);
      double add_right(double & right, const double bottom);
      double add_bottom(double & bottom);


    protected:

      const double omega_peak;
      const double step_omega, step_theta;

      const class spectrum::Samples & integral_samples;
      const class spectrum::Dispersion & dispersion;

      class std::vector<double> boundary_top;
      class std::deque <double> boundary_right;
      class std::vector<double> boundary_bottom;

      /*  “integral_samples” indices  */
      size_t index_top, index_bottom;


    private:

      /****  not defined  ****/
      Boundaries(const class Boundaries &);
      void operator =(const class Boundaries &);
    };
  }
}


#endif  /*  AQUA_SPECTRUM_BOUNDARIES_H  */
