/*
  This file is part of “The Aqua library”.

  Copyright © 2006  Jocelyn Fréchot

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


/****************  Includes  ****************/


#include "gerstner_scene_classic.h"

#include "gerstner_surface_factory.h"

/*  Libaqua  */
#include <libaqua/aqua_ocean_gerstner.h>


/****************  Namespaces  ****************/


using namespace ::gerstner;
using aqua::ocean::gerstner::classic::Surface;


/****************  Public functions  ****************/


Scene_Classic::Scene_Classic(class gerstner::Surface_Factory * factory,
			     unsigned int window_width,
			     unsigned int window_height)
  : Scene_Pierson(factory, window_width, window_height),
    surface_classic(dynamic_cast<class Surface *>(this->surface_gerstner))
{
}


/*  Virtual  */
Scene_Classic::~Scene_Classic(void)
{
}


/****************  Protected functions  ****************/
