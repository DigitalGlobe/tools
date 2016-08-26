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

/*  OpenGL (Glut) callbacks functions for "class Aqua_Surface_Pierson_Ugauss"  */


/****************  includes  ****************/


#include "fft_callback_pierson_ug.h"

#include "fft_scene_pierson_ug.h"

/*  libaqua  */
#include <libaqua/aqua_ocean_fft.h>


/****************  namespaces  ****************/


using namespace fft;


/****************  public functions  ****************/


Callback_Pierson_Ug::Callback_Pierson_Ug(enum aqua::ocean::fft::type fft_type)
  : Callback(new class Scene_Pierson_Ug(fft_type))
{
}


/*  virtual  */
Callback_Pierson_Ug::~Callback_Pierson_Ug(void)
{
}
