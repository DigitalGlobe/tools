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

/*  "class Rng" with unit gaussian distribution  */


/****************  includes  ****************/


#include "aqua_rng_ugaussian.h"

/*  GSL lib  */
extern "C"
{
#include <gsl/gsl_randist.h>
}


/****************  namespaces  ****************/


using namespace aqua::rng;


/****************  public functions  ****************/


Ugaussian::Ugaussian(void)
{
}


/*  virtual  */
Ugaussian::~Ugaussian(void)
{
}


/****  get  ****/

/*  returns a random number of uniform gaussian distribution  */
float
Ugaussian::get_number(void) const
{
  return gsl_ran_ugaussian(this->generator);
}
