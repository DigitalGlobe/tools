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


#ifndef AQUA_DEMO_FFT_OGL_SCENE_H
#define AQUA_DEMO_FFT_OGL_SCENE_H


/****************  includes  ****************/


#include "ogl_scene.h"

/*  graphic lib  */
extern "C"
{
#include <GL/gl.h>  /*  GLenum  */
}


/****************  classe declarations  ****************/


class Scene;


/****************  polymorphic classes  ****************/


namespace fft
{
  class Ogl_Scene : public ::Ogl_Scene
  {
  public:

    Ogl_Scene(void);

    ~Ogl_Scene(void);


    /****  draw  ****/
    /*  “scene”  must be of type “class fft::Scene”  */
    virtual void draw(const class ::Scene & scene) const;


  protected:

    class Ogl_Surface * const ogl_surface;


  private:

    /****  not defined  ****/
    Ogl_Scene(const class Ogl_Scene &);
    void operator =(const class Ogl_Scene &);
  };

}


#endif  /*  AQUA_DEMO_FFT_OGL_SCENE_H  */
