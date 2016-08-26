/*  Emacs mode: -*- C++ -*-  */
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


#ifndef AQUA_DEMO_OGL_SCENE_H
#define AQUA_DEMO_OGL_SCENE_H


/****************  Includes  ****************/


#include "ogl.h"

#include "camera.h"  /*  enum Camera::view_mode  */

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>  /*  GLenum  */
}

#include <sys/resource.h>
#include <ctime>


/****************  Polymorphic classes  ****************/


class Ogl_Scene : public Ogl
{
public:

  Ogl_Scene(void);

  virtual ~Ogl_Scene(void);

  /****  draw  ****/
  virtual void draw(const class Scene & scene) const = 0;

  void draw_before_surface(float & time_before_display,
			   const class Scene & scene) const;
  void draw_after_surface(const class Scene & scene,
			  float time_before_display,
			  unsigned int overlaps_number) const;


protected:

  class Ogl_Floor * const ogl_floor;
  class Ogl_Stone * const ogl_stone;
  class Ogl_String * const ogl_string;
  class Ogl_Sun * const ogl_sun;

  class Fps * const fps;

  const GLenum light;
  const bool surface_normalized_normals;

  mutable struct rusage rusage;
  mutable double time_begin, time_end, time_whole, time_lib, time_cg;
  mutable clock_t  clock_begin, clock_end;
  mutable double clock_whole, clock_lib, clock_cg;


  void move(enum Camera::view_mode view_mode,
	    const float position[3],
	    float angle_x,
	    float angle_y) const;


private:

  /****  Not defined  ****/
  Ogl_Scene(const class Ogl_Scene &);
  void operator =(const class Ogl_Scene &);
};


#endif  /*  AQUA_DEMO_OGL_SCENE_H  */
