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


/****************  includes  ****************/
#include <Windows.h>

#include "config/config.h"

#include "options.h"
#include "callback.h"

/*  fft  */
#include "fft/fft_callback_jonswap_ug.h"
#include "fft/fft_callback_pierson_ug.h"

/*  gerstner  */
#include "gerstner/gerstner_callback_classic.h"
#include "gerstner/gerstner_callback_pierson.h"
#include "gerstner/gerstner_surface_factory.h"

/*  local lib  */
#include "aqua_lib_exceptions.h"
#include "aqua_lib_init_glut.h"
#include "aqua_lib_utilities.h"   /*  get_file_name()  */



/*  graphic lib  */
extern "C"
{
#include <GL/glut.h>
}

/*  C++ lib  */
#include <exception>  /*  class exception  */
#include <new>        /*  class bad_alloc  */

/*  C lib  */
#include <cstdlib>  /*  EXIT_SUCCESS  */


/****************  functions  ****************/


int
main(int argc, char * * argv)
{
  class Options_Arguments * arguments;
  class Options * options;
  class Callback * callback;

  try
    {
      arguments = new class Options_Arguments;
      options = new class Options(argc, argv);


      aqua_lib::init_glut(config::Window_size_x,
			  config::Window_size_y,
			  config::Window_position_x,
			  config::Window_position_y,
			  //GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH,
			  GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH,
			  argc,
			  argv);
      options->parse(arguments);
      aqua_lib::init_glut_create_window(arguments->get_is_fullscreen(),
					aqua_lib::get_file_name(argv[0]));

      /*  OpenGL callbacks function initialization  */
      if ((arguments->get_model() == Options_Arguments::Fft_real)
	  || (arguments->get_model() == Options_Arguments::Fft_complex))
	{
	  enum aqua::ocean::fft::type type;

	  if (arguments->get_model() == Options_Arguments::Fft_real)
	    {
	      type = aqua::ocean::fft::Real_to_real;
	    }
	  else
	    {
	      type = aqua::ocean::fft::Complex_to_real;
	    }

	  if (arguments->get_spectrum() == Options_Arguments::Jonswap)
	    {
	      callback = new class fft::Callback_Jonswap_Ug(type);
	    }
	  else if (arguments->get_spectrum() ==
		   Options_Arguments::Pierson_moskowitz)
	    {
	      callback = new class fft::Callback_Pierson_Ug(type);
	    }
	  else
	    {
	      throw "unknown spectrum";
	    }
	}
      else if (arguments->get_model() == Options_Arguments::Gerstner)
	{
	  const bool adaptive = arguments->get_adaptive();

	  class gerstner::Surface_Factory_Classic * const factory =
	    new class gerstner::Surface_Factory_Classic(adaptive);

	  callback = new class gerstner::Callback_Classic(factory);
	}
      else
	{
	  throw "unknown model";
	}

      /*  frees no more needed objects  */
      delete options;
      delete arguments;
    }
  catch(class std::bad_alloc & exception)
    {
      aqua_lib::exceptions(argv[0], exception);
    }
  catch(class std::exception & exception)
    {
      aqua_lib::exceptions(argv[0], exception);
    }
  catch(const char * exception)
    {
      aqua_lib::exceptions(argv[0], exception);
    }
  catch(...)
    {
      aqua_lib::exceptions(argv[0], "unknown error");
    }

  //  exceptions?
  //  In “Callback” for consistency?
  glutMainLoop();

  /*  never reached  */
  return EXIT_SUCCESS;
}
