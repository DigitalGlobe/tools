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


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_SPECTRUM_DISPERSION_H
#define AQUA_SPECTRUM_DISPERSION_H


/****************  abstract classes  ****************/


namespace aqua
{
  namespace spectrum
  {

    class Dispersion
    {
    public:

      Dispersion(void);
      virtual ~Dispersion(void) = 0;


      /****  compute  ****/

      /*  D(ω, θ)  */
      virtual double compute(double theta,             /*  θ  */
			     double omega) const = 0;  /*  ω  */


    protected:

      /*  previous  */
      mutable double omega_previous;                 /*  ω  */
      mutable double theta_previous;                 /*  θ  */


    private:

      /****  not defined  ****/
      Dispersion(const class Dispersion &);
      void operator =(const class Dispersion &);
    };

  }
}


#endif  /*  AQUA_SPECTRUM_DISPERSION_H  */
