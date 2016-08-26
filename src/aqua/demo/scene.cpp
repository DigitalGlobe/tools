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


#include "scene.h"

#include "config/config.h"

#include "camera.h"
#include "floor.h"
#include "help.h"
#include "scene_context.h"
#include "stone.h"
#include "sun.h"
#include "surface_factory.h"

/*  local includes  */
#include "constant.h"  /*  Constant_ms_to_kmh  */

/*  libaqua  */
#include <libaqua/aqua_ocean_surface.h>

/*  C++ lib  */
#include <string>
#include <sstream>  /*  ostringstream  */


/****************  public functions  ****************/


Scene::Scene(class Surface_Factory * surface_factory,
	     class Scene_Context * context,
	     class Floor * floor,
	     class Help * help)
  : help(help->make_string()),
    context(context),
    camera(new class Camera),
    surface(surface_factory->create(*context)),
    floor(floor),
    stone(new struct Stone(config::Stone_depth,
			   config::Stone_height,
			   config::Stone_width)),
    sun(new class Sun(config::Sun_distance,
		      config::Sun_day_length,
		      config::Sun_latitude,
		      config::Sun_day_of_year)),
    wired(config::Scene_wired),
    drawn_normals(config::Scene_drawn_normals),
    drawn_overlaps(config::Scene_drawn_overlaps),
    drawn_stone(config::Scene_drawn_stone),
    displayed_fps(config::Scene_displayed_fps),
    displayed_help(config::Scene_displayed_help),
    displayed_info(config::Scene_displayed_info)
{
  delete help;
  delete surface_factory;

  if (this->wired && !this->drawn_normals)
    {
      this->surface->set_computed_normals(false);
    }

  if (this->drawn_overlaps)
    {
      this->surface->set_computed_eigenvalues(true);
    }
}


Scene::~Scene(void)
{
  delete this->sun;
  delete this->stone;
  delete this->floor;
  delete this->surface;
  delete this->camera;
  delete this->context;
}


/****  set  ****/

/*  time  */

void
Scene::set_time(float time)
{
  this->sun->set_time(time);
  this->surface->set_time(time);

  if(this->camera->get_view_mode() == Camera::Swimming)
    {
      int index_x, index_y;
      float height_camera;

      index_x = this->context->get_size_x() / 2;
      index_y = 0;
      /*  sets camera one meter above the surface  */
      height_camera =
	1.0 + this->surface->get_surface().get(2, index_x, index_y);
      this->camera->set_position_y(height_camera);
    }
}


/*  size  */

void
Scene::decrease_size(void)
{
  if (this->context->decrease_size())
    {
      this->set_size(*this->surface, *this->context);
    }
}


void
Scene::increase_size(void)
{
  if (this->context->increase_size())
    {
      this->set_size(*this->surface, *this->context);
    }
}


void
Scene::decrease_size_x(void)
{
  if (this->context->decrease_size_x())
    {
      this->set_size(*this->surface, *this->context);
    }
}


void
Scene::increase_size_x(void)
{
  if (this->context->increase_size_x())
    {
      this->set_size(*this->surface, *this->context);
    }
}


void
Scene::decrease_size_y(void)
{
  if (this->context->decrease_size_y())
    {
      this->set_size(*this->surface, *this->context);
    }
}


void
Scene::increase_size_y(void)
{
  if (this->context->increase_size_y())
    {
      this->set_size(*this->surface, *this->context);
    }
}


/*  length  */

void
Scene::decrease_length(void)
{
  if (this->context->decrease_length())
    {
      this->set_length(*this->surface, *this->floor, *this->context);
    }
}


void
Scene::increase_length(void)
{
  if (this->context->increase_length())
    {
      this->set_length(*this->surface, *this->floor, *this->context);
    }
}


void
Scene::decrease_length_x(void)
{
  if (this->context->decrease_length_x())
    {
      this->set_length(*this->surface, *this->floor, *this->context);
    }
}


void
Scene::increase_length_x(void)
{
  if (this->context->increase_length_x())
    {
      this->set_length(*this->surface, *this->floor, *this->context);
    }
}


void
Scene::decrease_length_y(void)
{
  if (this->context->decrease_length_y())
    {
      this->set_length(*this->surface, *this->floor, *this->context);
    }
}


void
Scene::increase_length_y(void)
{
  if (this->context->increase_length_y())
    {
      this->set_length(*this->surface, *this->floor, *this->context);
    }
}


/*  depth  */

void
Scene::decrease_depth(void)
{
  if (this->context->decrease_depth())
    {
      this->set_depth(*this->surface, *this->floor, *this->context);
    }
}


void
Scene::increase_depth(void)
{
  if (this->context->increase_depth())
    {
      this->set_depth(*this->surface, *this->floor, *this->context);
    }
}


/*  displacement_factor  */

void
Scene::decrease_displacement_factor(void)
{
  if (this->context->decrease_displacement_factor())
    {
      this->set_displacement_factor(*this->surface, *this->context);
    }
}


void
Scene::increase_displacement_factor(void)
{
  if (this->context->increase_displacement_factor())
    {
      this->set_displacement_factor(*this->surface, *this->context);
    }
}


/*  smallest_wavelength  */

void
Scene::decrease_smallest_wavelength(void)
{
  if (this->context->decrease_smallest_wavelength())
    {
      this->set_smallest_wavelength(*this->surface, *this->context);
    }
}


void
Scene::increase_smallest_wavelength(void)
{
  if (this->context->increase_smallest_wavelength())
    {
      this->set_smallest_wavelength(*this->surface, *this->context);
    }
}


/*  largest_wavelength  */

void
Scene::decrease_largest_wavelength(void)
{
  if (this->context->decrease_largest_wavelength())
    {
      this->set_largest_wavelength(*this->surface, *this->context);
    }
}


void
Scene::increase_largest_wavelength(void)
{
  if (this->context->increase_largest_wavelength())
    {
      this->set_largest_wavelength(*this->surface, *this->context);
    }
}


/*  spectrum_factor  */

void
Scene::decrease_spectrum_factor(void)
{
  if (this->context->decrease_spectrum_factor())
    {
      this->set_spectrum_factor(*this->surface,	*this->context);
    }
}


void
Scene::increase_spectrum_factor(void)
{
  if (this->context->increase_spectrum_factor())
    {
      this->set_spectrum_factor(*this->surface,	*this->context);
    }
}


/*  wind_speed  */

void
Scene::decrease_wind_speed(void)
{
  if (this->context->decrease_wind_speed())
    {
      this->set_wind_speed(*this->surface, *this->context);
    }
}


void
Scene::increase_wind_speed(void)
{
  if (this->context->increase_wind_speed())
    {
      this->set_wind_speed(*this->surface, *this->context);
    }
}


/*  wind_angle  */

void
Scene::decrease_wind_angle(void)
{
  if (this->context->decrease_wind_angle())
    {
      this->set_wind_angle(*this->surface, *this->context);
    }
}


void
Scene::increase_wind_angle(void)
{
  if (this->context->increase_wind_angle())
    {
      this->set_wind_angle(*this->surface, *this->context);
    }
}


/*  alpha  */

void
Scene::decrease_surface_alpha(void)
{
  static_cast<void>(this->context->decrease_surface_alpha());
  //  if alpha == 0 → don't compute surface (nor normals if they are not
  //  drawn)
}


void
Scene::increase_surface_alpha(void)
{
  static_cast<void>(this->context->increase_surface_alpha());
}


/*  booleans  */

void
Scene::toggle_wired(void)
{
  this->wired = !this->wired;

  if (!this->drawn_normals)
    {
      this->surface->set_computed_normals(!this->wired);
    }
}


void
Scene::toggle_drawn_normals(void)
{
  this->drawn_normals = !this->drawn_normals;

  if (this->wired)
    {
      this->surface->set_computed_normals(this->drawn_normals);
    }
}


void
Scene::toggle_drawn_stone(void)
{
  this->drawn_stone = !this->drawn_stone;
}


void
Scene::toggle_drawn_overlaps(void)
{
  this->drawn_overlaps = !this->drawn_overlaps;

  this->surface->
    set_computed_eigenvalues(!this->surface->get_computed_eigenvalues());
}


void
Scene::toggle_displayed_fps(void)
{
  this->displayed_fps = !this->displayed_fps;
}


void
Scene::toggle_displayed_help(void)
{
  this->displayed_help = !this->displayed_help;
}


void
Scene::toggle_displayed_info(void)
{
  this->displayed_info = !this->displayed_info;
}


/****  get  ****/

const class Camera &
Scene::get_camera(void) const
{
  return *this->camera;
}


const class aqua::ocean::Surface &
Scene::get_surface(void) const
{
  return *this->surface;
}


const class Floor &
Scene::get_floor(void) const
{
  return *this->floor;
}


const class Stone &
Scene::get_stone(void) const
{
  return *this->stone;
}


const class Sun &
Scene::get_sun(void) const
{
  return *this->sun;
}


float
Scene::get_surface_alpha(void) const
{
  return this->context->get_surface_alpha();
}


bool
Scene::get_wired(void) const
{
  return this->wired;
}


bool
Scene::get_drawn_normals(void) const
{
  return this->drawn_normals;
}


bool
Scene::get_drawn_overlaps(void) const
{
  return this->drawn_overlaps;
}


bool
Scene::get_drawn_floor(void) const
{
  return (std::isnormal(this->context->get_depth()) != 0);
}


bool
Scene::get_drawn_stone(void) const
{
  return this->drawn_stone;
}


bool
Scene::get_displayed_fps(void) const
{
  return this->displayed_fps;
}


bool
Scene::get_displayed_help(void) const
{
  return this->displayed_help;
}


bool
Scene::get_displayed_info(void) const
{
  return this->displayed_info;
}


const std::string &
Scene::get_help(void) const
{
  return this->help;
}


std::string
Scene::make_info(unsigned int overlaps_number) const
{
  std::ostringstream temp_stream;

  temp_stream
    << "displacement factor: " << this->context->get_displacement_factor()
    << "\n"
    << "depth: "
    ;
  if (std::isnormal(this->context->get_depth()) == 0)
    {
      temp_stream << "infinite";
    }
  else
    {
      temp_stream << this->context->get_depth() << " m";
    }
  temp_stream
    << "\n"
    ;
  if (this->drawn_overlaps)
    {
      temp_stream
	<< "overlaps number: " << overlaps_number
	<< "\n";
    }
  temp_stream
    << "\n"
    << "X * Y resolution: "
    << this->surface->get_resolution_x() << " m"
    << " * " << this->surface->get_resolution_y() << " m"
    << "\n"
    << "grid length: "
    << this->context->get_length_x()
    << " m * " << this->context->get_length_y() << " m"
    << "\n"
    << "number of points: "
    << this->context->get_size_x() << " * " << this->context->get_size_y()
    << "\n"
    << "\n"
    /*  spectrum  */
    << "smallest / largest wave length: "
    << this->context->get_smallest_wavelength() << " m"
    << " / " << this->context->get_largest_wavelength() << " m"
    << "\n"
    << "wind angle: " << this->context->get_wind_angle() << " rad"
    << "\n"
    << "wind speed: "
    << this->context->get_wind_speed() << " m.s-1"
    << " ("
    << this->context->get_wind_speed() * Constant_ms_to_kmh << " km/h), "
    << "Beaufort " << this->context->get_wind_beaufort_number() << ", "
    << this->context->get_wind_speed_description()
    << "\n"
    << "spectrum factor: " << this->context->get_spectrum_factor()
    << "\n"
    ;

  return temp_stream.str();
}


/****  move  ****/

void
Scene::view_mode_reset(void)
{
}


void
Scene::view_mode_switch(void)
{
  this->camera->view_mode_switch();
}


void
Scene::move_forward(bool run)
{
  this->camera->move_forward(run);
}


void
Scene::move_backward(bool run)
{
  this->camera->move_backward(run);
}


void
Scene::move_left(bool run)
{
  this->camera->move_left(run);
}


void
Scene::move_right(bool run)
{
  this->camera->move_right(run);
}


void
Scene::move_up(bool run)
{
  this->camera->move_up(run);
}


void
Scene::move_down(bool run)
{
  this->camera->move_down(run);
}


void
Scene::rotate_view(float angle_x, float angle_y)
{
  this->camera->rotate(angle_x, angle_y);
}


/****************  protected functions  ****************/


/****  global changes  ****/

void
Scene::set_size(class aqua::ocean::Surface & surface,
		const class Scene_Context & context)
{
  surface.reset(context.get_size_x(), context.get_size_y());
}


void
Scene::set_length(class aqua::ocean::Surface & surface,
		  class Floor & floor,
		  const class Scene_Context & context)
{
  surface.set_length(context.get_length_x(), context.get_length_y());
  floor.set_length(context.get_length_x(), context.get_length_y());
}


void
Scene::set_depth(class aqua::ocean::Surface & surface,
		 class Floor & floor,
		 const class Scene_Context & context)
{
  surface.set_depth(context.get_depth());
  floor.get_depth() = context.get_depth();
}


void
Scene::set_displacement_factor(class aqua::ocean::Surface & surface,
			       const class Scene_Context & context)
{
  surface.set_displacement_factor(context.get_displacement_factor());
}


void
Scene::set_smallest_wavelength(class aqua::ocean::Surface & surface,
			       const class Scene_Context & context)
{
  surface.set_smallest_wavelength(context.get_smallest_wavelength());
}


void
Scene::set_largest_wavelength(class aqua::ocean::Surface & surface,
			      const class Scene_Context & context)
{
  surface.set_largest_wavelength(context.get_largest_wavelength());
}


void
Scene::set_spectrum_factor(class aqua::ocean::Surface & surface,
			   const class Scene_Context & context)
{
  surface.set_spectrum_factor(context.get_spectrum_factor());
}


void
Scene::set_wind_speed(class aqua::ocean::Surface & surface,
		      const class Scene_Context & context)
{
  surface.set_wind_speed(context.get_wind_speed());
}


void
Scene::set_wind_angle(class aqua::ocean::Surface & surface,
		      const class Scene_Context & context)
{
  surface.set_wind_angle(context.get_wind_angle());
}
