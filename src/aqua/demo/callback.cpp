/*
  This file is part of “The Aqua library”.

  Copyright © 2003 2004 2005 2006  Jocelyn Fréchot

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

/*  OpenGL (GLUT) callbacks functions  */


/****************  includes  ****************/


#include "callback.h"

#include "config/config_help.h"
#include "config/config.h"
#include "config/config_ogl.h"

#include "ogl_scene.h"
#include "scene.h"
#include "tga.h"


/*  local includes  */
#include "check_gl.h"  /*  CHECK_GL  */

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
}


/****************  static variables  ****************/

namespace
{
  /*
    pointer to “this”, allows to reach GLUT callback member functions via C
    functions
  */
  class Callback * callback_local;
}


/****************  static functions prototypes  ****************/

namespace
{
  /*  C functions used to reach GLUT callback member functions  */
  extern "C"
  {
    void idle_c(void);
    void display_c(void);
    void reshape_c(int w, int h);
    void mouse_c(int button, int state, int x, int y);
    void motion_c(int x, int y);
    void keyboard_c(unsigned char c, int x, int y);
    void special_c(int key, int x, int y);
  }
}


/****************  public functions  ****************/


Callback::Callback(class Scene * scene, class Ogl_Scene * ogl_scene)
  : Perspective_fovy(config::ogl::Perspective_fovy),
    Perspective_znear(config::ogl::Perspective_znear),
    Perspective_zfar(config::ogl::Perspective_zfar),
    scene(scene),
    ogl_scene(ogl_scene),
    tga(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)),
    tga_on(false)
{
  this->scene->set_time(0.0);  // ?

  ::callback_local = this;

  /*  time  */
  this->time_current = 0.0;
  this->time_pause = 0.0;
  /*  pause  */
  this->pause = config::Callback_pause;
  /*  mouse  */
  this->mouse_is_moving = false;
  this->mouse_start_x = 0;
  this->mouse_start_y = 0;

  /****  GLUT callbacks  ****/

  /*  “...when window system events are not being received.”  */
  glutIdleFunc(idle_c);
  glutDisplayFunc(display_c);
  glutReshapeFunc(reshape_c);
  glutMouseFunc(mouse_c);
  glutMotionFunc(motion_c);
  glutKeyboardFunc(keyboard_c);
  glutSpecialFunc(special_c);

  //glutTimerFunc();
  //glutVisibilityFunc();
}


/*  virtual  */
Callback::~Callback(void)
{
  delete this->ogl_scene;
  delete this->scene;
}


/****************  protected functions  ****************/


/*****
      local GLUT callback functions, used to extend regular ones in inherited
      classes
*****/

void
Callback::idle_local_before(void)
{
}


void
Callback::display_local(void)
{
}


void
Callback::reshape_local(int, int)
{
}


void
Callback::motion_local(int, int)
{
}


/*  returns "true" if key event is catched  */
bool
Callback::keyboard_local(unsigned char, int, int)
{
  return false;
}


/*  returns "true" if key event is catched  */
bool
Callback::special_local(int, int, int)
{
  return false;
}


/****************  public functions  ****************/


/****  GLUT callback functions  ****/
/*
  These functions should be declared “protected” but must be reachable by
  static C functions. Users should not call them directly.
*/


void
Callback::idle(void)
{
  if (!this->pause)
    {
      this->time_current = glutGet(GLUT_ELAPSED_TIME) - this->time_pause;

      this->scene->set_time(this->time_current / 1000.0);

      glutPostRedisplay();
    }
}

// void
// Callback::idle(void)
// {
//   static float time = 0.0;

//   if (!this->pause)
//     {
//       this->time_current = glutGet(GLUT_ELAPSED_TIME) - this->time_pause;

//       this->scene->set_time(time);
//       time += 1.0 / 15.0;

//       glutPostRedisplay();
//     }
// }


void
Callback::display(void)
{
  CHECK_GL(glMatrixMode(GL_MODELVIEW)); 
  CHECK_GL(glPushMatrix());
  {
    this->ogl_scene->draw(*this->scene);
  }
  CHECK_GL(glMatrixMode(GL_MODELVIEW)); 
  CHECK_GL(glPopMatrix());

  if (this->tga_on)
    {
      this->tga.snap();
    }
}


void
Callback::reshape(int w, int h)
{
  CHECK_GL(glViewport(0, 0, w, h));
  CHECK_GL(glMatrixMode(GL_PROJECTION));
  {
    CHECK_GL(glLoadIdentity());
    gluPerspective(this->Perspective_fovy,
		   static_cast<float>(w) / static_cast<float>(h),
		   this->Perspective_znear,
		   this->Perspective_zfar);
  }
  CHECK_GL(glMatrixMode(GL_MODELVIEW));

  this->tga.resize(w, h);

  this->reshape_local(w, h);

  glutPostRedisplay();
}


void
Callback::mouse(int button, int state, int x, int y)
{
  /*  rotate the scene with the left mouse button  */
  if (button == GLUT_LEFT_BUTTON)
    {
      if (state == GLUT_DOWN)
	{
	  this->mouse_is_moving = true;
	  this->mouse_start_x = x;
	  this->mouse_start_y = y;
	}

      if (state == GLUT_UP)
	{
	  this->mouse_is_moving = false;
	}
    }
}


void
Callback::motion(int x, int y)
{
  // motion est appellé si la souris bouge pendant qu'un bouton est pressé
  // (!= motion_passive)
  // « mouse_is_moving » à renomer en « is_left_button_pressed » ?
  if (this->mouse_is_moving)
    {
      this->scene->rotate_view(y - this->mouse_start_y,
			       x - this->mouse_start_x);
      this->mouse_start_x = x;
      this->mouse_start_y = y;

      glutPostRedisplay();
    }
}


void
Callback::keyboard(unsigned char c, int x, int y)
{
  /****  general  ****/

  /*  quit  */
  if (c == config::help::Quit || c == config::help::Quit_2)
    {
      exit(EXIT_SUCCESS);
    }

  /*  pause  */
  else if (c == config::help::Pause)
    {
      this->pause = !this->pause;
      if (!this->pause)
	{
	  this->time_pause = glutGet(GLUT_ELAPSED_TIME) - this->time_current;
	}
    }

  /*  help (keys list)  */
  if (c == config::help::Help)
    {
      this->scene->toggle_displayed_help();
      glutPostRedisplay();
    }

  else if (c == config::help::Fps)
    {
      this->scene->toggle_displayed_fps();
//       if (this->is_fps_displayed)
// 	{
// 	  this->fps->reset(this->time_current);
// 	}
      glutPostRedisplay();
    }

  /*  display info  */
  else if (c == config::help::Info)
    {
      this->scene->toggle_displayed_info();
      glutPostRedisplay();
    }

  /*  draw graduated stone  */
  else if (c == config::help::Stone)
    {
      this->scene->toggle_drawn_stone();
      glutPostRedisplay();
    }

  /****  view mode  ****/

  /*  switch view mode  */
  else if (c == config::help::View_switch)
    {
      this->scene->view_mode_switch();
      glutPostRedisplay();
    }

  /****  surface look  ****/

  /*  switch surface display color/wire  */
  else if (c == config::help::Wire)
    {
      this->scene->toggle_wired();
      glutPostRedisplay();
    }

  /*  show surface normals (wire mode)  */
  else if (c == config::help::Normals)
    {
      this->scene->toggle_drawn_normals();
      glutPostRedisplay();
    }

  /*  surface alpha (opacity)  */
  else if (c == config::help::Surface_alpha_decrease)
    {
      this->scene->decrease_surface_alpha();
      glutPostRedisplay();
    }
  else if (c == config::help::Surface_alpha_increase)
    {
      this->scene->increase_surface_alpha();
      glutPostRedisplay();
    }

  /*  overlaps  */
  else if (c == config::help::Overlaps)
    {
      this->scene->toggle_drawn_overlaps();
      glutPostRedisplay();
    }

  /****  surface physics  ****/

  /*  number of points  */
  else if (c == config::help::Size_decrease)
    {
      this->scene->decrease_size();
      glutPostRedisplay();
    }
  else if (c == config::help::Size_increase)
    {
      this->scene->increase_size();
      glutPostRedisplay();
    }
  /*  only X direction  */
  else if (c == config::help::Size_x_decrease)
    {
      this->scene->decrease_size_x();
      glutPostRedisplay();
    }
  else if (c == config::help::Size_x_increase)
    {
      this->scene->increase_size_x();
      glutPostRedisplay();
    }
  /*  only Z direction  */
  else if (c == config::help::Size_y_decrease)
    {
      this->scene->decrease_size_y();
      glutPostRedisplay();
    }
  else if (c == config::help::Size_y_increase)
    {
      this->scene->increase_size_y();
      glutPostRedisplay();
    }

  /*  surface length  */
  else if (c == config::help::Length_decrease)
    {
      this->scene->decrease_length();
      glutPostRedisplay();
    }
  else if (c == config::help::Length_increase)
    {
      this->scene->increase_length();
      glutPostRedisplay();
    }
  /*  only X direction  */
  else if (c == config::help::Length_x_decrease)
    {
      this->scene->decrease_length_x();
      glutPostRedisplay();
    }
  else if (c == config::help::Length_x_increase)
    {
      this->scene->increase_length_x();
      glutPostRedisplay();
    }
  /*  only Z direction  */
  else if (c == config::help::Length_y_decrease)
    {
      this->scene->decrease_length_y();
      glutPostRedisplay();
    }
  else if (c == config::help::Length_y_increase)
    {
      this->scene->increase_length_y();
      glutPostRedisplay();
    }

  /*  displacement factor  */
  else if (c == config::help::Displacement_factor_decrease)
    {
      this->scene->decrease_displacement_factor();
      glutPostRedisplay();
    }
  else if (c == config::help::Displacement_factor_increase)
    {
      this->scene->increase_displacement_factor();
      glutPostRedisplay();
    }

  /*  floor depth  */
  else if (c == config::help::Depth_decrease)
    {
      this->scene->decrease_depth();
      glutPostRedisplay();
    }
  else if (c == config::help::Depth_increase)
    {
      this->scene->increase_depth();
      glutPostRedisplay();
    }

  /****  spectrum  ****/

  /*  smallest wave length  */
  else if (c == config::help::Smallest_wavelength_decrease)
    {
      this->scene->decrease_smallest_wavelength();
      glutPostRedisplay();
    }
  else if (c == config::help::Smallest_wavelength_increase)
    {
      this->scene->increase_smallest_wavelength();
      glutPostRedisplay();
    }

  /*  largest wave length  */
  else if (c == config::help::Largest_wavelength_decrease)
    {
      this->scene->decrease_largest_wavelength();
      glutPostRedisplay();
    }
  else if (c == config::help::Largest_wavelength_increase)
    {
      this->scene->increase_largest_wavelength();
      glutPostRedisplay();
    }

  /*  spectrum factor  */
  if (c == config::help::Spectrum_factor_decrease)
    {
      this->scene->decrease_spectrum_factor();
      glutPostRedisplay();
    }
  else if (c == config::help::Spectrum_factor_increase)
    {
      this->scene->increase_spectrum_factor();
      glutPostRedisplay();
    }

  /*  wind speed  */
  if (c == config::help::Wind_speed_decrease)
    {
      this->scene->decrease_wind_speed();
      glutPostRedisplay();
    }
  else if (c == config::help::Wind_speed_increase)
    {
      this->scene->increase_wind_speed();
      glutPostRedisplay();
    }

  /*  wind angle  */
  else if (c == config::help::Wind_angle_decrease)
    {
      this->scene->decrease_wind_angle();
      glutPostRedisplay();
    }
  else if (c == config::help::Wind_angle_increase)
    {
      this->scene->increase_wind_angle();
      glutPostRedisplay();
    }

  else if (c == config::help::Snap)
    {
      this->tga_on = !this->tga_on;
      if (this->tga_on)
	{
	  std::cout << glutGet(GLUT_WINDOW_WIDTH) << "×"
		    << glutGet(GLUT_WINDOW_HEIGHT)
		    << std::endl;
	}
      //glutPostRedisplay();
    }

  /****  if nothing matches  ****/

  else
    {
      static_cast<void>(this->keyboard_local(c, x, y));
    }
}


void
Callback::special(int key, int x, int y)
{
  bool run;


  if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
    {
      run = false;
    }
  else
    {
      run = true;
    }


  switch (key)
    {
      /*  forward  */
    case GLUT_KEY_UP:
      this->scene->move_forward(run);
      glutPostRedisplay();
      break;

      /*  backward  */
    case GLUT_KEY_DOWN:
      this->scene->move_backward(run);
      glutPostRedisplay();
      break;

      /*  left  */
    case GLUT_KEY_LEFT:
      this->scene->move_left(run);
      glutPostRedisplay();
      break;

      /*  right  */
    case GLUT_KEY_RIGHT:
      this->scene->move_right(run);
      glutPostRedisplay();
      break;

      /*  up  */
    case GLUT_KEY_PAGE_UP:
      this->scene->move_up(run);
      glutPostRedisplay();
      break;

      /*  down  */
    case GLUT_KEY_PAGE_DOWN:
      this->scene->move_down(run);
      glutPostRedisplay();
      break;

    default:
      static_cast<void>(this->special_local(key, x, y));
      break;
    }
}


/****************  static functions  ****************/


namespace
{
  /*  C functions used to reach GLUT callback member functions  */
  extern "C"
  {
    void
    idle_c(void)
    {
      callback_local->idle();
    }
    
    void
    display_c(void)
    {
      callback_local->display();
    }

    void
    reshape_c(int w, int h)
    {
      callback_local->reshape(w, h);
    }

    void
    mouse_c(int button, int state, int x, int y)
    {
      callback_local->mouse(button, state, x, y);
    }

    void
    motion_c(int x, int y)
    {
      callback_local->motion(x, y);
    }

    void
    keyboard_c(unsigned char c, int x, int y)
    {
      callback_local->keyboard(c, x, y);
    }

    void
    special_c(int key, int x, int y)
    {
      callback_local->special(key, x, y);
    }
  }
}
