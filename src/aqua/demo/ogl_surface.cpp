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


/****************  includes  ****************/


#include "ogl_surface.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  libaqua  */
#include <libaqua/aqua_array.h>    /*  surface.get_{normals,surface}()  */
#include <libaqua/aqua_array_1.h>  /*  surface.get_overlaps()  */
#include <libaqua/aqua_ocean_surface.h>

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
#include <GL/glut.h>
}

/*  C lib  */
#include <cmath>  /*  isnormal  */


/****************  public functions  ****************/


Ogl_Surface::Ogl_Surface(void)
{
}


/****************  protected functions  ****************/


/****  draw  ****/

void
Ogl_Surface::draw_before(float alpha, bool wired) const
{
  CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));

  /****  colors and lighting  ****/
  GLfloat mat_shininess[] = { 120.0 };
  GLfloat mat_specular[] = { 0.8, 0.8, 0.8, 1.0 };
  GLfloat mat_diffuse[] = { 0.3, 0.5, 0.75, alpha };
  GLfloat mat_ambient[] = { 0.15, 0.5, 0.35, 1.0 };

  if (!wired)
    {
      CHECK_GL(glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess));
      CHECK_GL(glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular));
      CHECK_GL(glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse));
      CHECK_GL(glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient));
      /*  transparency  */
      if (std::isnormal(1.0 - alpha) != 0)
	{
	  CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	  CHECK_GL(glEnable(GL_BLEND));
	}
    }
  else
    {
      /**/
      /*
	must occur before “glPolygonMode()” to avoid a crash with Xorg 6.8.2
      */
      glBegin(GL_LINES);
      CHECK_GL(glEnd());
      /**/

      CHECK_GL(glPolygonMode(GL_FRONT, GL_LINE));
      CHECK_GL(glDisable(GL_LIGHTING));
      CHECK_GL(glColor3f(0.2, 0.5, 0.5));
    }
}


void
Ogl_Surface::draw_after(void) const
{
  CHECK_GL(glPopAttrib());
}


/****  draw surface  ****/

void
Ogl_Surface::draw_surface_before(const class aqua::ocean::Surface & surface,
				 const GLuint * list,
				 int list_size,
				 bool wired) const
{
  /*  vertex arrays  */
  CHECK_GL(glVertexPointer(3,
			   GL_FLOAT,
			   0,
			   surface.get_surface().get_array()));
  CHECK_GL(glEnableClientState(GL_VERTEX_ARRAY));
  if (!wired)
    {
      CHECK_GL(glNormalPointer(GL_FLOAT,
			       0,
			       surface.get_normals().get_array()));
      CHECK_GL(glEnableClientState(GL_NORMAL_ARRAY));
    }

  CHECK_GL(glDrawElements(GL_QUAD_STRIP,  //GL_TRIANGLE_STRIP,
			  list_size,
			  GL_UNSIGNED_INT,
			  list));
}


void
Ogl_Surface::draw_surface_after(unsigned int & overlaps_number,
				const class aqua::ocean::Surface & surface,
				bool wired,
				bool drawn_normals,
				bool drawn_overlaps) const
{
  if (!wired)
    {
      CHECK_GL(glDisableClientState(GL_NORMAL_ARRAY));
    }
  CHECK_GL(glDisableClientState(GL_VERTEX_ARRAY));


  if (drawn_normals || drawn_overlaps)
    {
      float scale;

      // Ne fonctionne pas si en adaptatif :
      //scale = (surface.get_resolution_x() + surface.get_resolution_y()) / 2.0;
      scale = 1.0;
      if (drawn_normals)
	{
	  this->draw_normals_half(surface, scale);
	}

      if (drawn_overlaps)
	{
	  this->draw_overlaps_half(overlaps_number, surface, scale);
	}
    }
}


/****  others  ****/

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//  pb avec les normales (et overlaps) en adaptatif : il faut se baser sur la
//  taille de la grille utilisée, pas la taille totale
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


void
Ogl_Surface::draw_normals_half(const class aqua::ocean::Surface & surface,
			       float scale) const
{
  float color[3] = { 1.0, 0.0, 0.0 };
  float base_x, base_y, base_z;
  size_t i, j;


  CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));
  {
    CHECK_GL(glDisable(GL_LIGHTING));
    CHECK_GL(glColor3fv(color));

    for (i = 0; i < surface.get_surface().get_size_i(); i++)
      {
	for (j = 0; j < surface.get_surface().get_size_j(); j++)
	  {
	    glBegin(GL_LINE_STRIP);
	    {
	      base_x = surface.get_surface().get(0, i, j);
	      base_y = surface.get_surface().get(1, i, j);
	      base_z = surface.get_surface().get(2, i, j);

	      glVertex3f(base_x, base_y, base_z);
	      glVertex3f(base_x + surface.get_normals().get(0, i, j) * scale,
			 base_y + surface.get_normals().get(1, i, j) * scale,
			 base_z + surface.get_normals().get(2, i, j) * scale);
	    }
	    CHECK_GL(glEnd());
	  }
      }
  }
  CHECK_GL(glPopAttrib());
}


/*  draws overlaps and return overlaps number  */
void
Ogl_Surface::draw_overlaps_half(unsigned int & overlaps_number,
				const class aqua::ocean::Surface & surface,
				float scale) const
{
  float color[3] = { 1.0, 0.0, 0.0 };
  size_t i;


  CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));
  {
    CHECK_GL(glDisable(GL_LIGHTING));
    CHECK_GL(glColor3fv(color));

    for (i = 0; i < surface.get_overlaps().get_size(); i++)
      {
	if (surface.get_overlaps().get(i) < 0.0)
	  {
	    overlaps_number += 1;

	    CHECK_GL(glPushMatrix());
	    {
	      CHECK_GL(glTranslatef(surface.get_surface().get(0, i),
				    surface.get_surface().get(1, i),
				    surface.get_surface().get(2, i)));
	      // displaylist?
	      glutSolidSphere(.2 * scale, 10, 10);
	    }
	    CHECK_GL(glPopMatrix());
	  }
      }
  }
  CHECK_GL(glPopAttrib());
}
