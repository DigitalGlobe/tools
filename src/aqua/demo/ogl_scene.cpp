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


#include "ogl_scene.h"

#include "config/config.h"
#include "config/config_ogl.h"

#include "ogl_floor.h"
#include "ogl_stone.h"
#include "ogl_string.h"
#include "ogl_sun.h"

#include "camera.h"
#include "floor.h"
#include "fps.h"
#include "scene.h"
#include "sun.h"

/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  graphic lib  */
extern "C"
{
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
}

#include <sys/resource.h>
#include <ctime>


/****************  public functions  ****************/


Ogl_Scene::Ogl_Scene(void)
  : ogl_floor(new class Ogl_Floor),
    ogl_stone(new class Ogl_Stone),
    ogl_string(new class Ogl_String),
    ogl_sun(new class Ogl_Sun),
    fps(new class Fps),
    light(GL_LIGHT0),
    surface_normalized_normals(config::Surface_normalized_normals)
{
  /*  light parameters  */
  GLfloat light_diffuse[]  = { 1.0, 0.9, 0.8, 1.0 };
  GLfloat light_specular[] = { 1.0, 0.9, 0.8, 1.0 };
  GLfloat light_ambient[]  = { 0.3, 0.3, 0.3, 1.0 };

  /*  sets light parameters  */
  CHECK_GL(glLightfv(this->light, GL_DIFFUSE, light_diffuse));
  CHECK_GL(glLightfv(this->light, GL_SPECULAR, light_specular));
  CHECK_GL(glLightfv(this->light, GL_AMBIENT, light_ambient));

  CHECK_GL(glEnable(this->light));

  /****  GL  ****/

  /*  clear values  */
  CHECK_GL(glClearColor(0.4, 0.7, 1.0, 0.0));
  CHECK_GL(glClearDepth(1.0));

  /*  hints  */
  CHECK_GL(glHint(GL_PERSPECTIVE_CORRECTION_HINT,
		  config::ogl::Hint_perspective));
  CHECK_GL(glHint(GL_POINT_SMOOTH_HINT, config::ogl::Hint_point));
  CHECK_GL(glHint(GL_LINE_SMOOTH_HINT, config::ogl::Hint_line));
  CHECK_GL(glHint(GL_POLYGON_SMOOTH_HINT, config::ogl::Hint_polygon));

  /*  light  */
  /*  for sun reflection on water surface  */
  CHECK_GL(glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE));
  CHECK_GL(glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE));
  CHECK_GL(glShadeModel(GL_SMOOTH));
  CHECK_GL(glEnable(GL_LIGHTING));

  /*  cull face  */
  CHECK_GL(glEnable(GL_CULL_FACE));

  /*  depth  */
  CHECK_GL(glEnable(GL_DEPTH_TEST));
}


/*  virtual destructor  */
Ogl_Scene::~Ogl_Scene(void)
{
  delete this->fps;
  delete this->ogl_sun;
  delete this->ogl_string;
  delete this->ogl_stone;
  delete this->ogl_floor;
}


/****  Draw  ****/

void
Ogl_Scene::draw_before_surface(float & time_before_display,
			       const class Scene & scene) const
{
//   getrusage(RUSAGE_SELF, &(this->rusage));
//   this->time_end =
//     this->rusage.ru_utime.tv_sec + this->rusage.ru_utime.tv_usec / 1000000.0;
//   this->time_whole = this->time_end - this->time_begin;
//   this->time_begin = this->time_end;

//   this->clock_end = clock();
//   this->clock_whole =
//     static_cast<double>(this->clock_end - this->clock_begin) / CLOCKS_PER_SEC;
//   this->clock_begin = this->clock_end;


  float orientation[4];
  float position[3];
  float angle_x, angle_y;

  /*  time_before_display  */
  if (scene.get_displayed_fps())
    {
      time_before_display = glutGet(GLUT_ELAPSED_TIME);
    }
  else
    {
      /*  avoids compiler warning  */
      time_before_display = 0.0;
    }


  scene.get_sun().get_orientation(orientation);
  orientation[3] = 0.0;  /*  directionnal light  */

  scene.get_camera().get_opposite_position(position);
  scene.get_camera().get_angle(angle_x, angle_y);

  CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


  CHECK_GL(glMatrixMode(GL_MODELVIEW));
  glLoadIdentity();

  /*  “glPopMatrix()” happens in “draw_after_surface()”  */
  CHECK_GL(glPushMatrix());


  CHECK_GL(glPushMatrix());
  {
    this->rotate(angle_x, angle_y);
    //  looking at south
    //  Scene property?
    this->rotate(0.0, 180.0);
    this->ogl_sun->draw(scene.get_sun());

    CHECK_GL(glLightfv(this->light, GL_POSITION, orientation));
  }
  CHECK_GL(glMatrixMode(GL_MODELVIEW));
  CHECK_GL(glPopMatrix());


  this->move(scene.get_camera().get_view_mode(), position, angle_x, angle_y);

  if (scene.get_drawn_stone())
    {
      // 	if (!is_tiled)
      // 	  {
      // 	    stone_y = 0.0;
      // 	  }
      // 	else
      // 	  {
      // 	    stone_y = -length_y;
      // 	  }

      // 	CHECK_GL(glPushMatrix());
      // 	{
      // 	  CHECK_GL(glTranslatef(0.0, stone_y, 0.0));

      // 	  drawn_stone(stone_depth, stone_height, stone_width);
      // 	}
      // 	CHECK_GL(glPopMatrix());

      this->ogl_stone->draw(scene.get_stone());
    }


  /*  surface  */
  if (!this->surface_normalized_normals)
    {
      CHECK_GL(glEnable(GL_NORMALIZE));
    }
}


void
Ogl_Scene::draw_after_surface(const class Scene & scene,
			      float time_before_display,
			      unsigned int overlaps_number) const
{
  /****************/
//   CHECK_GL(glPushAttrib(GL_ALL_ATTRIB_BITS));
//   {
//     CHECK_GL(glDisable(GL_LIGHTING));
//     CHECK_GL(glShadeModel(GL_FLAT));
//     glColor3f(1.0, 0.0, 0.0);
//     CHECK_GL(glDisable(GL_CULL_FACE));
//     glBegin(GL_QUADS);
//     {
//       glVertex3f(424.5, 10, -24.5);
//       glVertex3f(425.5, 10, -24.5);
//       glVertex3f(425.5, 10, -25.5);
//       glVertex3f(424.5, 10, -25.5);
//     }
//     CHECK_GL(glEnd());
//   }
//   glPopAttrib();
  /****************/

  if (!this->surface_normalized_normals)
    {
      CHECK_GL(glDisable(GL_NORMALIZE));
    }

  /*  “glPushMatrix()” was called in “draw_before_surface()”  */
  CHECK_GL(glMatrixMode(GL_MODELVIEW));
  CHECK_GL(glPopMatrix());


  if (scene.get_displayed_fps()
      || scene.get_displayed_help()
      || scene.get_displayed_info())
    {
      CHECK_GL(glMatrixMode(GL_PROJECTION));
      CHECK_GL(glPushMatrix());
      {
	CHECK_GL(glLoadIdentity());
	gluOrtho2D(0,
		   glutGet(GLUT_WINDOW_WIDTH),
		   0,
		   glutGet(GLUT_WINDOW_HEIGHT));
	CHECK_GL(glMatrixMode(GL_MODELVIEW));

	if (scene.get_displayed_fps())
	  {
	    this->ogl_string->draw(this->fps->make_string(), true, 10, 20);
	  }
	if (scene.get_displayed_help())
	  {
	    this->ogl_string->draw(scene.get_help(), true, 10, 40);
	  }
	if (scene.get_displayed_info())
	  {
	    this->ogl_string->draw(scene.make_info(overlaps_number),
				   false,
				   10,
				   20);
	  }
      }
      CHECK_GL(glMatrixMode(GL_PROJECTION));
      CHECK_GL(glPopMatrix());
    }


  glutSwapBuffers();


  if (scene.get_displayed_fps())
    {
      this->fps->set_elapsed_time(time_before_display,
				  glutGet(GLUT_ELAPSED_TIME));
    }


//   getrusage(RUSAGE_SELF, &(this->rusage));
//   this->time_end =
//     this->rusage.ru_utime.tv_sec + this->rusage.ru_utime.tv_usec / 1000000.0;
//   this->time_cg = this->time_end - this->time_begin;
//   this->time_lib = this->time_whole - this->time_cg;

//   this->clock_end = clock();
//   this->clock_cg =
//     static_cast<double>(this->clock_end - this->clock_begin) / CLOCKS_PER_SEC;
//   this->clock_lib = this->clock_whole - this->clock_cg;
//   std::cout << "total: " << this->time_whole
// 	    << "/" << this->clock_whole << " - "
// 	    << "lib: " << this->time_lib
// 	    << "/" << this->clock_lib << " - "
// 	    << "cg: " << this->time_cg
// 	    << "/" << this->clock_cg 
// 	    << std::endl;
}


/****************  Protected functions  ****************/


void
Ogl_Scene::move(enum Camera::view_mode view_mode,
		const float position[3],
		float angle_x,
		float angle_y) const

{
  switch(view_mode)
    {
    case Camera::Observing:
      this->move_around_origin(position, angle_x, angle_y);
      break;

    case Camera::Flying:
    case Camera::Swimming:
      this->move_around_observer(position, angle_x, angle_y);
      break;
    }
}
