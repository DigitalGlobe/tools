/*  Emacs mode: -*- C++ -*-  */
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

/*  OpenGL (Glut) callbacks functions  */


#ifndef AQUA_DEMO_CALLBACK_H
#define AQUA_DEMO_CALLBACK_H



#include "tga.h"


/****************  abstract classes  ****************/


class Callback
{
public:

  Callback(class Scene * scene, class Ogl_Scene * ogl_scene);

  virtual ~Callback(void) = 0;


protected:

  /*  gluPerspective  */
  const float Perspective_fovy;
  const float Perspective_znear;
  const float Perspective_zfar;

  class Scene * const scene;
  class Ogl_Scene * const ogl_scene;

  class TGASnapShot tga;
  bool tga_on;

  /*  used to synchronize everything  */
  float time_current;  /*  miliseconds  */
  /*  time elapsed during pause  */
  float time_pause;    /*  miliseconds  */
  /*  pause  */
  bool pause;
  /*  mouse  */
  bool mouse_is_moving;
  int mouse_start_x, mouse_start_y;


  /*
    local GLUT callback functions, used to extend regular ones in inherited
    classes
  */
  virtual void idle_local_before(void);
  virtual void display_local(void);
  virtual void reshape_local(int, int);
  virtual void motion_local(int, int);
  /*  returns "true" if key event is catched  */
  virtual bool keyboard_local(unsigned char, int, int);
  /*  returns "true" if key event is catched  */
  virtual bool special_local(int, int, int);


  /*  copy constructor to be defined  */
  Callback(const class Callback &);


public:

  /*  GLUT callback functions  */
  /*
    These functions should be declared “protected” but must be reachable by
    static C functions. Users should not call them directly.
  */
  void idle(void);
  void display(void);
  void reshape(int w, int h);
  void mouse(int button, int state, int x, int y);
  void motion(int x, int y);
  void keyboard(unsigned char c, int x, int y);
  void special(int key, int x, int y);
};


#endif  /*  AQUA_DEMO_CALLBACK_H  */
