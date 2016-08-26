/*  Emacs mode: -*- C++ -*-  */
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


#ifndef AQUA_DEMO_FFT_SCENE_H
#define AQUA_DEMO_FFT_SCENE_H


/****************  includes  ****************/


#include "scene.h"

/*  C++ lib  */
#include <string>


/****************  class declarations  ****************/


namespace fft
{
  class Floor;
}

class Help;
class Scene_Context;
class Surface_Factory;

namespace aqua
{
  namespace ocean
  {
    namespace fft
    {
      class Surface;
    }
  }
}


/****************  polymorphic classes  ****************/


namespace fft
{
  class Scene : public ::Scene
  {
  public:

    Scene(class Scene_Context * context,
	  class Surface_Factory * surface_factory,
	  class Help * help);

    virtual ~Scene(void);


    /****  set  ****/
    /*  booleans  */
    void toggle_tiled(void);

    /****  get  ****/
    const class aqua::ocean::fft::Surface & get_surface_fft(void) const;
    const class Drawlist_Set & get_drawlist_set(void) const;

    bool get_tiled(void) const;

    virtual std::string make_info(unsigned int overlaps_number = 0) const;

    /****  move  ****/
    virtual void view_mode_reset(void);
    virtual void view_mode_switch(void);


  protected:

    class Drawlist_Set * const drawlist_set;
    class aqua::ocean::fft::Surface * const surface_fft;
    class Floor * const floor_fft;

    bool tiled;              /*  is surface tiled?  */


    /****  global changes  ****/
    virtual void set_size(class aqua::ocean::Surface & surface,
			  const class Scene_Context & context);
    virtual void set_length(class aqua::ocean::Surface & surface,
			    class ::Floor & floor,
			    const class Scene_Context & context);
    
    void camera_reset(float length_x, float length_y, bool tiled);


    /*  copy constructor to be defined  */
    Scene(const class Scene &);
  };
}


#endif  /*  AQUA_DEMO_FFT_SCENE_H  */
