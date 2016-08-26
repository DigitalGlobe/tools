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

/*  "class Rng" with uniform distribution in the range [ 0.0, 2π [  */


/****************  includes  ****************/


#include "aqua_rng_uniform_two_pi.h"

/*  local includes  */
#include "constant.h"

/*  GSL lib  */
extern "C"
{
#include <gsl/gsl_rng.h>
}


/****************  namespaces  ****************/


using namespace aqua::rng;


/****************  public functions  ****************/


Uniform_Two_Pi::Uniform_Two_Pi(void)
{
}


/*  virtual  */
Uniform_Two_Pi::~Uniform_Two_Pi(void)
{
}


/****  get  ****/

/*
  returns a random number of uniform distribution in the range [ 0.0, 2π [
*/
float
Uniform_Two_Pi::get_number(void) const
{
  return gsl_rng_uniform(this->generator) * Constant_2_pi;
}
