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


#include "fft_ogl_surface.h"

#include "fft_drawlist_set.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  libaqua  */
#include <libaqua/aqua_array.h>    /*  surface.get_{normals,surface}()  */
#include <libaqua/aqua_array_1.h>  /*  surface.get_overlaps()  */
#include <libaqua/aqua_ocean_fft.h>

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
#include <GL/glut.h>
}

/*  C lib  */
#include <cmath>  /*  isnormal  */


/****************  namespaces  ****************/


using namespace fft;


/****************  public functions  ****************/


fft::Ogl_Surface::Ogl_Surface(void)
{
}


void
fft::Ogl_Surface::draw(unsigned int & overlaps_number,
		       const class aqua::ocean::fft::Surface & surface,
		       const class Drawlist_Set & drawlist_set,
		       float alpha,
		       bool wired,
		       bool drawn_normals,
		       bool drawn_overlaps,
		       bool tiled) const
{
  this->draw_before(alpha, wired);

  if (!tiled)
    {
      /*  surface + normals + overlaps  */
      draw_surface_unit(overlaps_number,
			surface,
			drawlist_set,
			wired,
			drawn_normals,
			drawn_overlaps,
			Drawlist_Set::Normal);
    }
  else
    {
      /*  normal resolution  */
      draw_surface_unit(overlaps_number,
			surface,
			drawlist_set,
			wired,
			false,
			false,
			Drawlist_Set::Normal);

      CHECK_GL(glPushMatrix());
      {
	CHECK_GL(glTranslatef(surface.get_length_x(), 0.0, 0.0));

	/*  surface + normals + overlaps  */
	draw_surface_unit(overlaps_number,
			  surface,
			  drawlist_set,
			  wired,
			  drawn_normals,
			  drawn_overlaps,
			  Drawlist_Set::Normal);

	CHECK_GL(glTranslatef(surface.get_length_x(), 0.0, 0.0));

	draw_surface_unit(overlaps_number,
			  surface,
			  drawlist_set,
			  wired,
			  false,
			  false,
			  Drawlist_Set::Normal);
      }
      CHECK_GL(glPopMatrix());

      /*  fourth of resolution  */
      CHECK_GL(glPushMatrix());
      {
	CHECK_GL(glTranslatef(0.0, 0.0, - surface.get_length_y()));

	draw_surface_unit(overlaps_number,
			  surface,
			  drawlist_set,
			  wired,
			  false,
			  false,
			  Drawlist_Set::Fourth);

	CHECK_GL(glPushMatrix());
	{
	  CHECK_GL(glTranslatef(surface.get_length_x(), 0.0, 0.0));

	  draw_surface_unit(overlaps_number,
			    surface,
			    drawlist_set,
			    wired,
			    false,
			    false,
			    Drawlist_Set::Fourth);

	  CHECK_GL(glTranslatef(surface.get_length_x(), 0.0, 0.0));

	  draw_surface_unit(overlaps_number,
			    surface,
			    drawlist_set,
			    wired,
			    false,
			    false,
			    Drawlist_Set::Fourth);
	}
	CHECK_GL(glPopMatrix());

	/*  sixteenth of resolution  */
	CHECK_GL(glTranslatef(0.0, 0.0, - surface.get_length_y()));

	draw_surface_unit(overlaps_number,
			  surface,
			  drawlist_set,
			  wired,
			  false,
			  false,
			  Drawlist_Set::Sixteenth);

	CHECK_GL(glPushMatrix());
	{
	  CHECK_GL(glTranslatef(surface.get_length_x(), 0.0, 0.0));

	  draw_surface_unit(overlaps_number,
			    surface,
			    drawlist_set,
			    wired,
			    false,
			    false,
			    Drawlist_Set::Sixteenth);

	  CHECK_GL(glTranslatef(surface.get_length_x(), 0.0, 0.0));

	  draw_surface_unit(overlaps_number,
			    surface,
			    drawlist_set,
			    wired,
			    false,
			    false,
			    Drawlist_Set::Sixteenth);
	}
	CHECK_GL(glPopMatrix());
      }
      CHECK_GL(glPopMatrix());
    }
  /*  else  */

  this->draw_after();
}


/****************  protected functions  ****************/


void
fft::Ogl_Surface::
draw_surface_unit(unsigned int & overlaps_number,
		  const class aqua::ocean::fft::Surface & surface,
		  const class Drawlist_Set & drawlist_set,
		  bool wired,
		  bool drawn_normals,
		  bool drawn_overlaps,
		  enum Drawlist_Set::resolution resolution) const
{
  overlaps_number = 0;

  CHECK_GL(glPushMatrix());
  {
    this->rotate_to_z_up();

    /****  draw first half  ****/
    draw_surface_unit_half(overlaps_number,
			   surface,
			   drawlist_set,
			   wired,
			   drawn_normals,
			   drawn_overlaps,
			   resolution,
			   false);
  }
  CHECK_GL(glPopMatrix());

  if (surface.get_fft_type() == aqua::ocean::fft::Real_to_real)
    {
      /****  reset origin  ****/
      float position[] = { surface.get_length_x(),
			   0.0,
			   - surface.get_length_y() };

      CHECK_GL(glPushMatrix());
      {
	this->move_around_origin(position, 0.0, 180.0);

	this->rotate_to_z_up();

	/****  draw second half  ****/
	draw_surface_unit_half(overlaps_number,
			       surface,
			       drawlist_set,
			       wired,
			       drawn_normals,
			       drawn_overlaps,
			       resolution,
			       true);
      }
      CHECK_GL(glPopMatrix());
    }
  /*  if  */
}


void
fft::Ogl_Surface::
draw_surface_unit_half(unsigned int & overlaps_number,
		       const class aqua::ocean::fft::Surface & surface,
		       const class Drawlist_Set & drawlist_set,
		       bool wired,
		       bool drawn_normals,
		       bool drawn_overlaps,
		       enum Drawlist_Set::resolution resolution,
		       bool junction) const
{
  const GLuint * list;
  const GLuint * list_junction;
  int list_size, list_junction_size;


  list_size = drawlist_set.get_list_size(resolution);
  list = drawlist_set.get_list(resolution);
  if (junction)
    {
//       list_junction_size = drawlist_set.get_list_junction_size(resolution);
//       list_junction = drawlist_set.get_list_junction(resolution);

//       list_size -= list_junction_size;
//       list += list_junction_size;
    }
  else
    {
      /*  avoid compiler warnings for might be uninitialized variable  */
      list_junction_size = 0;
      list_junction = NULL;
    }


  this->draw_surface_before(surface, list, list_size, wired);

  if (junction)
    {
      CHECK_GL(glDrawElements(GL_TRIANGLE_STRIP,
			      list_junction_size,
			      GL_UNSIGNED_INT,
			      list_junction));
    }

  this->draw_surface_after(overlaps_number,
			   surface,
			   wired,
			   drawn_normals,
			   drawn_overlaps);
}
