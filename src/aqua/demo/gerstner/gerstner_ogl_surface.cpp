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


#include "gerstner_ogl_surface.h"

#include "ogl_drawlist.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  libaqua  */
#include <libaqua/aqua_array.h>    /*  surface.get_{normals,surface}()  */
#include <libaqua/aqua_array_1.h>  /*  surface.get_overlaps()  */
#include <libaqua/aqua_ocean_gerstner.h>

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
#include <GL/glut.h>
}

/*  C lib  */
#include <cmath>  /*  isnormal  */


/****************  namespaces  ****************/


using namespace gerstner;


/****************  public functions  ****************/


gerstner::Ogl_Surface::Ogl_Surface(void)
{
}


void
gerstner::Ogl_Surface::draw(unsigned int & overlaps_number,
			    const class aqua::ocean::Surface & surface,
			    const class Ogl_Drawlist & drawlist,
			    float alpha,
			    bool wired,
			    bool drawn_normals,
			    bool drawn_overlaps) const
{
  const GLuint * list;
  int list_size;


  list = drawlist.get_list();
  list_size = drawlist.get_size();


  this->draw_before(alpha, wired);

  CHECK_GL(glPushMatrix());
  {
    this->rotate_to_z_up();

    this->draw_surface_before(surface, list, list_size, wired);
    this->draw_surface_after(overlaps_number,
			     surface,
			     wired,
			     drawn_normals,
			     drawn_overlaps);
  }
  CHECK_GL(glPopMatrix());

  this->draw_after();
}


/****************  protected functions  ****************/


