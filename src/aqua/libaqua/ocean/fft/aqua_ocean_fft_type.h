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

/**
   \file
   Fourier transform types.
*/


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_OCEAN_FFT_TYPE_H
#define AQUA_OCEAN_FFT_TYPE_H


/****************  enumerations  ****************/


namespace aqua
{
  namespace ocean
  {
    namespace fft
    {
      enum type { Real_to_real, Complex_to_real };
    }
  }
}


#endif  /*  AQUA_OCEAN_FFT_TYPE_H  */
