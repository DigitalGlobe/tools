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


/****************  includes  ****************/


#include "aqua_spectrum_dispersion.h"


/****************  namespaces  ****************/


using namespace aqua::spectrum;


/****************  public functions  ****************/


Dispersion::Dispersion(void)
  : omega_previous(0.0), theta_previous(0.0)
{
}


//  if we have “set” functions, we will need to change “previous” values as
//  well


/*  virtual  */
Dispersion::~Dispersion(void)
{
}
