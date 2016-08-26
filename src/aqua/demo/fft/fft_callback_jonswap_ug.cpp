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

/*
  OpenGL (Glut) callbacks functions for "class Aqua_Surface_Jonswap_Ug"
*/


/****************  includes  ****************/


#include "fft_callback_jonswap_ug.h"

#include "fft_scene_jonswap_ug.h"

#include "config/config_help.h"

/*  libaqua  */
#include <libaqua/aqua_ocean_fft.h>

/*  graphic lib  */
extern "C"
{
#include <GL/glut.h>
}


/****************  namespaces  ****************/


using namespace fft;


/****************  public functions  ****************/


Callback_Jonswap_Ug::Callback_Jonswap_Ug(enum aqua::ocean::fft::type fft_type)
  : Callback(new class Scene_Jonswap_Ug(fft_type)),
    scene_jonswap(dynamic_cast<class Scene_Jonswap_Ug * const>(this->scene))
{
}


/*  virtual destructor  */
Callback_Jonswap_Ug::~Callback_Jonswap_Ug(void)
{
}


bool
Callback_Jonswap_Ug::keyboard_local(unsigned char c, int x, int y)
{
  bool is_key_catched;

  is_key_catched = true;
  /*  avoids compiler warnings for unused parameters  */
  static_cast<void>(x);
  static_cast<void>(y);

  if (!this->Callback::keyboard_local(c, x, y))
    {
      /*  fetch  */
      if (c == config::help::Fetch_decrease)
	{
	  this->scene_jonswap->decrease_fetch();
	  glutPostRedisplay();
	}
      else if (c == config::help::Fetch_increase)
	{
	  this->scene_jonswap->increase_fetch();
	  glutPostRedisplay();
	}
      else
	{
	  is_key_catched = false;
	}
    }

  return is_key_catched;
}
