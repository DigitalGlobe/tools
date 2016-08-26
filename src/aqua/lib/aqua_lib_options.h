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


#ifndef AQUA_LIB_OPTIONS_H
#define AQUA_LIB_OPTIONS_H


/****************  includes  ****************/


/*  C++ lib  */
#include <string>

/*  C lib  */
extern "C"
{
#include <getopt.h>
}


/****************  abstract classes  ****************/


namespace aqua_lib
{
  class Options_Arguments
  {
  public:

    virtual ~Options_Arguments(void) = 0;
  };


  class Options
  {
  public:

    Options(int argc,
	    char **argv,
	    char *shortopts,
	    int non_option_arguments,
	    const char *program_version,
	    const char *library_name,
	    const char *library_version);

    virtual ~Options(void) = 0;


    void parse(class Options_Arguments *arguments) const;


  protected:

    const char * const Program_name;
    const int Argc;
    const std::string Version;
    /*  getopt  variables  */
    const char *Shortopts;
    const int Non_option_arguments;

    char * * const argv;

    struct option * options;
    std::string usage;


    /****  create  ****/
    std::string create_version(const char *program_name,
			       const char *program_version,
			       const char *library_name,
			       const char *library_version);

    /****  set  ****/
    void set_option(struct option *option,
		    const char *name,
		    int has_arg,
		    int *flag,
		    int val);
    void set_version(const char *program_name);
    virtual void set_usage(const char *program_name) = 0;

    /****  display and exit  ****/
    void display_and_exit_usage(bool is_error) const;
    void display_and_exit_version(void) const;

    /****  parse  ****/
    virtual void parse_options_args(class Options_Arguments *arguments,
				    int output_getopt) const = 0;
    virtual void parse_non_options_args(class Options_Arguments *arguments,
					char **argv,
					int optind,
					int remaining_arguments) const = 0;


    /*  copy constructor to be defined  */
    Options(const class Options &);
  };
}


#endif  /*  AQUA_LIB_OPTIONS_H  */
