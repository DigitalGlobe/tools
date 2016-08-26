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

/*  "class Options_Libaqua" for local program  */


#ifndef AQUA_DEMO_OPTIONS_H
#define AQUA_DEMO_OPTIONS_H


/****************  includes  ****************/


/*  local lib  */
#include "aqua_lib_options.h"


/****************  classes  ****************/

//  polymorphic?

class Options_Arguments : public aqua_lib::Options_Arguments
{
 public:

  enum spectrum { Jonswap, Pierson_moskowitz };
  enum model { Fft_complex, Gerstner, Fft_real };


  Options_Arguments(void);


  /****  get  ****/
  bool get_is_fullscreen(void) const;
  bool get_adaptive(void) const;
  enum spectrum get_spectrum(void) const;
  enum model get_model(void) const;

  /****  set  ****/
  void set_is_fullscreen(bool is_fullscreen);
  void set_adaptive(bool adaptive);
  void set_spectrum(enum spectrum spectrum);
  void set_model(enum model model);


 protected:

  bool is_fullscreen;
  bool adaptive;
  enum spectrum spectrum;
  enum model model;


  /*  copy constructor to be defined  */
  Options_Arguments(const class Options_Arguments &);
};


class Options : public aqua_lib::Options
{
 public:

  Options(int argc, char * * argv);

  virtual ~Options(void);


 protected:

  /*  set  */
  virtual void set_usage(const char * program_name);
  virtual void set_options(void);

  /*  parse  */
  virtual void parse_options_args(class aqua_lib::Options_Arguments * arguments,
				  int output_getopt) const;
  virtual void
  parse_non_options_args(class aqua_lib::Options_Arguments * arguments,
			 char * * argv,
			 int optind,
			 int remaining_arguments) const;


  /*  copy constructor to be defined  */
  Options(const class Options &);
};


#endif  /*  AQUA_DEMO_OPTIONS_H  */
