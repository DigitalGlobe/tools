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

/*  random number generator  */


/****************  includes  ****************/


#include "aqua_rng_rng.h"

#include "aqua_config.h"  /*  Rng_type  */

/*  GSL lib  */
extern "C"
{
#include <gsl/gsl_rng.h>
}

/*  C lib  */
#include <ctime>  /*  time_t, time()  */


/****************  namespaces  ****************/


using namespace aqua::rng;


/****************  public functions  ****************/


Rng::Rng(void)
  : generator(gsl_rng_alloc(aqua::config::Rng_type))
{
  time_t seed;

  seed = time(NULL);

  gsl_rng_set(this->generator, static_cast<unsigned long>(seed));
}


Rng::~Rng(void)
{
  gsl_rng_free(this->generator);
}
