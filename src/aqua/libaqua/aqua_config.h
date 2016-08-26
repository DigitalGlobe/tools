/*  Emacs mode: -*- c++ -*-  */
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

/**
   \file
   Configuration values.
*/

#ifndef AQUA_CONFIG_H
#define AQUA_CONFIG_H


/****************  includes  ****************/


/*  GSL lib  */
extern "C"
{
#include <gsl/gsl_rng.h>
}

/*  FFTW lib  */
extern "C"
{
#include <fftw3.h>
}


/****************  constants  ****************/


namespace aqua
{
  namespace config
  {

    const double Precision = 1e-4;

    /*  Start-up values  */
    const bool Computed_surface     = true;
    const bool Computed_normals     = true;
    const bool Computed_eigenvalues = false;

    /*  GSL  */
    const gsl_rng_type * const Rng_type = gsl_rng_taus2;  /*  fastest?  */

    /*  FFTW  */
    const unsigned int Fftw_plan_type = FFTW_ESTIMATE; //FFTW_MEASURE;

  }
}


#endif  /*  AQUA_CONFIG_H  */
