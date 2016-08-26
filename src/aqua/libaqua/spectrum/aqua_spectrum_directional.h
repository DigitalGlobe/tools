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


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_SPECTRUM_DIRECTIONAL_H
#define AQUA_SPECTRUM_DIRECTIONAL_H


/****************  class declarations  ****************/


namespace aqua
{
  namespace spectrum
  {
    class Dispersion;
    class Energy;
    class Samples;
  }
}


/****************  base classes  ****************/


namespace aqua
{
  namespace spectrum
  {

    class Directional
    {
    public:

      Directional(class Energy * energy,           /*  S(ω)  */
		  class Dispersion * dispersion);  /*  D(ω, θ)  */

      virtual ~Directional(void);


      /****  get  ****/

      double get_omega_peak(void) const;
      const class Energy & get_energy(void) const;
      const class Dispersion & get_dispersion(void) const;

      /****  compute  ****/

      /*  E(ω, θ)  */
      double compute(double theta,         /*  θ  */
		     double omega) const;  /*  ω  */
      /*  E(ω⃗)  */
      double compute_cartesian(double omega_x, double omega_y) const;
      /*  E(κ⃗)  */
      double compute_cartesian_wavenumber(double kappa_x,
					  double kappa_y) const;
      void find_boundaries(double & top,
			   double & right,
			   double & bottom) const;


    protected:

      enum boundary_direction { Top, Right, Bottom };

      const class Energy * const energy;
      const class Dispersion * const dispersion;
      /*  previous  */
      mutable double omega_previous;
      mutable double energy_previous;


    private:

      /****  not defined  ****/
      Directional(const class Directional &);
      void operator =(const class Directional &);
    };

  }
}


#endif  /*  AQUA_SPECTRUM_DIRECTIONAL_H  */
