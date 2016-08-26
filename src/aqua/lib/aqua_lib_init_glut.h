/*  Emacs mode: -*- C++ -*-  */
/*
  This file is part of “The Aqua library”.

  Copyright © 2004 2005  Jocelyn Fréchot

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


#ifndef AQUA_LIB_INIT_GLUT_H
#define AQUA_LIB_INIT_GLUT_H


/****************  external functions  ****************/


namespace aqua_lib
{
  /*  "init_glut()" must be call before command line arguments parsing  */
  extern void init_glut(int window_size_x,
			int window_size_y,
			int window_position_x,
			int window_position_y,
			unsigned int display_mode,
			int &argc,
			char **argv);
  extern void init_glut_create_window(bool is_fullscreen, char *program_name);
}


#endif  /*  AQUA_LIB_INIT_GLUT_H  */
