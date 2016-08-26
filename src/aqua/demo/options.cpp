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


#include "options.h"

/*  config  */
#include "config/config_option.h"

/*  local include  */
#include "version.h"

/*  local lib  */ 
#include "aqua_lib_options.h"

/*  libaqua  */
#include <libaqua/aqua_get_version.h>

/*  C++ lib  */
#include <sstream>


/*****************************************************/
/****************  Options_Arguments  ****************/
/*****************************************************/


/****************  public functions  ****************/


Options_Arguments::Options_Arguments(void)
  : is_fullscreen(false),
    adaptive(false),
    spectrum(config::option::Spectrum),
    model(config::option::Model)
{
}


/****  get  ****/

bool
Options_Arguments::get_is_fullscreen(void) const
{
  return this->is_fullscreen;
}


bool
Options_Arguments::get_adaptive(void) const
{
  return this->adaptive;
}


enum Options_Arguments::spectrum
Options_Arguments::get_spectrum(void) const
{
  return this->spectrum;
}


enum Options_Arguments::model
Options_Arguments::get_model(void) const
{
  return this->model;
}


/****  set  ****/

void
Options_Arguments::set_is_fullscreen(bool is_fullscreen)
{
  this->is_fullscreen = is_fullscreen;
}


void
Options_Arguments::set_adaptive(bool adaptive)
{
  this->adaptive = adaptive;
}


void
Options_Arguments::set_spectrum(enum spectrum spectrum)
{
  this->spectrum = spectrum;
}


void
Options_Arguments::set_model(enum model model)
{
  this->model = model;
}


/*******************************************/
/****************  Options  ****************/
/*******************************************/


/****************  public functions  ****************/


Options::Options(int argc, char * * argv)
  : aqua_lib::Options(argc,
		      argv,
		      "hvfacgrjp",
		      0,
		      Version_aqua_demo,
		      "libaqua",
		      aqua::get_version())
{
  this->set_usage(this->Program_name);
  this->set_options();
}


/*  virtual  */
Options::~Options(void)
{
}


/****************  protected functions  ****************/


/****  set  ****/

void
Options::set_usage(const char * program_name)
{
  std::ostringstream temp_stream;

  temp_stream << "Usage: " << program_name << " "
	      << "[-f] [-a] [-c | -g | -r] [-j | -p]"
	      << "\n"
	      << "       " << program_name << " -h | -v"
	      << "\n"
	      << "\n"
	      << "Options:"
	      << "\n"
	      << "  -h, --help         display this help"
	      << "\n"
	      << "  -v, --version      display version"
	      << "\n"
	      << "  -f, --fullscreen   run in fullscreen mode"
	      << "\n"
	      << "  -a, --adaptive     adaptive (not for the FFT)"
	      << "\n"
	      << "\n"
	      << "Model type:"
	      << "\n"
	      << "  -c, --fft-complex  Fast Fourier transform, "
	        << "complex numbers (default)"
	      << "\n"
	      << "  -g, --gerstner     Classic"
	      << "\n"
	      << "  -r, --fft-real     Fast Fourier transform, real numbers"
	      << "\n"
	      << "\n"
	      << "Spectrum type:"
	      << "\n"
	      << "  -j, --jonswap      JONSWAP"
	      << "\n"
	      << "  -p, --pierson      Pierson-Moskowitz (default)"
	      << "\n"
    ;

  this->usage = temp_stream.str();
}


void
Options::set_options(void)
{
  const size_t size = 11;

  this->options = new struct option[size];

  this->set_option(&options[0], "help",        no_argument, NULL, 'h');
  this->set_option(&options[1], "version",     no_argument, NULL, 'v');
  this->set_option(&options[2], "fullscreen",  no_argument, NULL, 'f');
  this->set_option(&options[3], "adaptive",    no_argument, NULL, 'a');
  this->set_option(&options[4], "fft-complex", no_argument, NULL, 'c');
  this->set_option(&options[5], "gerstner",    no_argument, NULL, 'g');
  this->set_option(&options[6], "xxxx",        no_argument, NULL, 'x');
  this->set_option(&options[7], "fft-real",    no_argument, NULL, 'r');
  this->set_option(&options[8], "jonswap",     no_argument, NULL, 'j');
  this->set_option(&options[9], "pierson",     no_argument, NULL, 'p');
  /*  Index above must be “size − 2”.  */
  /*
    This must be the last element of the array (cf. getopt_long
    documentation).
  */
  this->set_option(&options[size - 1], 0, 0, 0, 0);
}


/****  parse  ****/
#include <iostream>
void
Options::parse_options_args(class aqua_lib::Options_Arguments * arguments,
			    int output_getopt) const
{
  class Options_Arguments * temp_arguments;


  temp_arguments = dynamic_cast<class Options_Arguments *>(arguments);

  switch(output_getopt)
    {
    case 'h':
      this->display_and_exit_usage(false);
      break;

    case 'v':
      this->display_and_exit_version();
      break;

    case 'f':
      temp_arguments->set_is_fullscreen(true);
      break;

    case 'a':
      temp_arguments->set_adaptive(true);
      break;

      /****  model  ****/

    case 'c':
      temp_arguments->set_model(Options_Arguments::Fft_complex);
      break;

    case 'g':
      temp_arguments->set_model(Options_Arguments::Gerstner);
      break;
      
    case 'r':
      temp_arguments->set_model(Options_Arguments::Fft_real);
      break;

      /****  spectrum  ****/      

    case 'j':
      temp_arguments->set_spectrum(Options_Arguments::Jonswap);
      break;
      
    case 'p':
      temp_arguments->set_spectrum(Options_Arguments::Pierson_moskowitz);
      break;
      
    default:
      this->display_and_exit_usage(true);
    }
}


void
Options::parse_non_options_args(class aqua_lib::Options_Arguments * arguments,
				char * * argv,
				int optind,
				int remaining_arguments) const
{
  /*  avoids compiler warnings for unused parameters  */
  static_cast<void>(arguments);
  static_cast<void>(argv);
  static_cast<void>(optind);
  static_cast<void>(remaining_arguments);
}
