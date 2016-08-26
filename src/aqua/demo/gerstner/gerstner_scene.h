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


#ifndef AQUA_DEMO_GERSTNER_SCENE_H
#define AQUA_DEMO_GERSTNER_SCENE_H


/****************  Includes  ****************/


#include "scene.h"

#include "gerstner_ogl_adaptive.h"

/*  C++ lib  */
#include <string>


/****************  Class declarations  ****************/


class Floor;
class Help;
class Ogl_Drawlist;
class Scene_Context;
class Surface_Factory;

namespace gerstner
{
  class Scene_Context;
}

namespace aqua
{
  namespace ocean
  {
    namespace gerstner
    {
      class Base_Surface;
      class Surface;
    }
  }
}


/****************  Polymorphic classes  ****************/


namespace gerstner
{


class Scene : public ::Scene
{
public:

  Scene(class ::Scene_Context * context,
	class Surface_Factory * surface_factory,
	class Help * help,
	unsigned int window_width,
	unsigned int window_height);

  virtual ~Scene(void);


  /****  Set  ****/
  //void set_adaptive(bool adaptive) const;
  void decrease_waves(void);
  void increase_waves(void);
  void set_window_size(unsigned int width, unsigned int height);

  /****  Get  ****/
  bool get_adaptive(void) const;

  const class aqua::ocean::gerstner::Surface &
  get_surface_gerstner(void) const;

  const class Ogl_Adaptive & get_ogl_adaptive(void) const;
  class Ogl_Adaptive & get_ogl_adaptive(void);

  const class Ogl_Drawlist & get_ogl_drawlist(void) const;
  class Ogl_Drawlist & get_ogl_drawlist(void);

  class aqua::ocean::gerstner::Base_Surface & get_base_surface(void) const;

  virtual std::string make_info(unsigned int overlaps_number = 0) const;


protected:

  class Ogl_Adaptive ogl_adaptive;

  class Ogl_Drawlist * const ogl_drawlist;
  class aqua::ocean::gerstner::Surface * const surface_gerstner;
  class gerstner::Scene_Context * const context_gerstner;


  /****  Global changes  ****/
  virtual void set_size(class aqua::ocean::Surface & surface,
			const class ::Scene_Context & context);
  void set_waves_number(class aqua::ocean::gerstner::Surface & surface);


private:

  /****  Not defined  ****/
  Scene(const class Scene &);
  void operator =(const class Scene &);
};


}  /*  namespace gerstner  */


#endif  /*  AQUA_DEMO_GERSTNER_SCENE_H  */
