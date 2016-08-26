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


#ifndef AQUA_DEMO_SCENE_H
#define AQUA_DEMO_SCENE_H


/****************  includes  ****************/


/*  C++ lib  */
#include <string>


/****************  class declarations  ****************/


namespace aqua
{
  namespace ocean
  {
    class Surface;
  }
}


/****************  polymorphic classes  ****************/


class Scene
{
public:

  Scene(class Surface_Factory * surface_factory,
	class Scene_Context * context,
	class Floor * floor,
	class Help * help);

  virtual ~Scene(void);


  /****  set  ****/
  /*  time  */
  void set_time(float time);
  /*  size  */
  void decrease_size(void);
  void increase_size(void);
  void decrease_size_x(void);
  void increase_size_x(void);
  void decrease_size_y(void);
  void increase_size_y(void);
  /*  length  */
  void decrease_length(void);
  void increase_length(void);
  void decrease_length_x(void);
  void increase_length_x(void);
  void decrease_length_y(void);
  void increase_length_y(void);
  /*  depth  */
  void decrease_depth(void);
  void increase_depth(void);
  /*  displacement_factor  */
  void decrease_displacement_factor(void);
  void increase_displacement_factor(void);
  /*  smallest_wavelength  */
  void decrease_smallest_wavelength(void);
  void increase_smallest_wavelength(void);
  /*  largest_wavelength  */
  void decrease_largest_wavelength(void);
  void increase_largest_wavelength(void);
  /*  spectrum_factor  */
  void decrease_spectrum_factor(void);
  void increase_spectrum_factor(void);
  /*  wind_speed  */
  void decrease_wind_speed(void);
  void increase_wind_speed(void);
  /*  wind_angle  */
  void decrease_wind_angle(void);
  void increase_wind_angle(void);
  /*  surface_alpha  */
  void decrease_surface_alpha(void);
  void increase_surface_alpha(void);
  /*  booleans  */
  void toggle_wired(void);
  void toggle_drawn_normals(void);
  void toggle_drawn_stone(void);
  void toggle_drawn_overlaps(void);
  void toggle_displayed_fps(void);
  void toggle_displayed_help(void);
  void toggle_displayed_info(void);

  /****  get  ****/
  const class Camera & get_camera(void) const;
  const class aqua::ocean::Surface & get_surface(void) const;
  const class Floor & get_floor(void) const;
  const struct Stone & get_stone(void) const;
  const class Sun & get_sun(void) const;

  float get_surface_alpha(void) const;
  bool get_wired(void) const;
  bool get_drawn_normals(void) const;
  bool get_drawn_overlaps(void) const;
  bool get_drawn_floor(void) const;
  bool get_drawn_stone(void) const;
  bool get_displayed_fps(void) const;
  bool get_displayed_help(void) const;
  bool get_displayed_info(void) const;

  const std::string & get_help(void) const;
  virtual std::string make_info(unsigned int overlaps_number = 0) const;

  /****  move  ****/
  void view_mode_reset(void);
  void view_mode_switch(void);
  void move_forward(bool run);
  void move_backward(bool run);
  void move_left(bool run);
  void move_right(bool run);
  void move_up(bool run);
  void move_down(bool run);
  void rotate_view(float angle_x, float angle_y);


protected:

  const std::string help;

  class Scene_Context * const context;
  class Camera * const camera;
  class aqua::ocean::Surface * const surface;
  class Floor * const floor;
  struct Stone * const stone;
  class Sun * const sun;

  bool wired;
  bool drawn_normals;
  bool drawn_overlaps;  /*  are eigenvalues computed and drawn?  */
  bool drawn_stone;
  bool displayed_fps;
  bool displayed_help;
  bool displayed_info;


  /****  global changes  ****/
  virtual void set_size(class aqua::ocean::Surface & surface,
			const class Scene_Context & context);
  virtual void set_length(class aqua::ocean::Surface & surface,
			  class Floor & floor,
			  const class Scene_Context & context);
  virtual void set_depth(class aqua::ocean::Surface & surface,
			 class Floor & floor,
			 const class Scene_Context & context);
  virtual void set_displacement_factor(class aqua::ocean::Surface & surface,
				       const class Scene_Context & context);
  virtual void set_smallest_wavelength(class aqua::ocean::Surface & surface,
				       const class Scene_Context & context);
  virtual void set_largest_wavelength(class aqua::ocean::Surface & surface,
				      const class Scene_Context & context);
  virtual void set_spectrum_factor(class aqua::ocean::Surface & surface,
				   const class Scene_Context & context);
  virtual void set_wind_speed(class aqua::ocean::Surface & surface,
			      const class Scene_Context & context);
  virtual void set_wind_angle(class aqua::ocean::Surface & surface,
			      const class Scene_Context & context);


private:

  /****  Not defined  ****/
  Scene(const class Scene &);
  void operator =(const class Scene &);
};


#endif  /*  AQUA_DEMO_SCENE_H  */
