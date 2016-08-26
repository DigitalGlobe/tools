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


#include "aqua_lib_options.h"

#include "aqua_lib_utilities.h"  /*  get_file_name()  */

/*  local includes  */
#include "version.h"  /*  Version_gpl  */

/*  C++ lib  */
#include <iostream>
#include <sstream>   /*  ostringstream  */
#include <string>

/*  C lib  */
#include <cstdlib>    /*  EXIT_{SUCCESS,FAILURE}  */
extern "C"
{
#include <getopt.h>  //  "getopt.h" ?
}


/****************  namespaces  ****************/


using namespace aqua_lib;


/*****************************************************/
/****************  Options_Arguments  ****************/
/*****************************************************/


/****************  public functions  ****************/


/*  virtual destructor  */
Options_Arguments::~Options_Arguments(void)
{
}


/*******************************************/
/****************  Options  ****************/
/*******************************************/


/****************  public functions  ****************/


Options::Options(int argc,
		 char **argv,
		 char *shortopts,
		 int non_option_arguments,
		 const char *program_version,
		 const char *library_name,
		 const char *library_version)
  : Program_name(get_file_name(argv[0])),
    Argc(argc),
    Version(create_version(this->Program_name,
			   program_version,
			   library_name,
			   library_version)),
    Shortopts(shortopts),
    Non_option_arguments(non_option_arguments),
    argv(argv)
{
}


/*  virtual destructor  */
Options::~Options(void)
{
}


void
Options::parse(class Options_Arguments *arguments) const
{
  int output_getopt;
  int remaining_arguments;


  /*  parses options arguments  */
  while(true)
    {
      output_getopt = getopt_long(this->Argc,
				  this->argv,
				  this->Shortopts,
				  this->options,
				  NULL);

      /*  no more option  */
      if (output_getopt == -1)
	{
	  break;
	}

      this->parse_options_args(arguments, output_getopt);
    }

  /*  parses remaining arguments  */
  remaining_arguments = this->Argc - optind;
  if (remaining_arguments > this->Non_option_arguments)
    {
      std::cerr << this->Program_name << ": too many arguments"
		<< "\n"
		<< std::endl
	;
      this->display_and_exit_usage(true);
    }
  else
    {
      this->parse_non_options_args(arguments,
				   this->argv,
				   optind,
				   remaining_arguments);
    }
}


/****************  protected functions  ****************/


/****  create  ****/

std::string
Options::create_version(const char *program_name,
			const char *program_version,
			const char *library_name,
			const char *library_version)
{
  std::ostringstream temp_stream;

  temp_stream << program_name << " " << program_version
	      << " (" << library_name << " " << library_version << ")"
	      << "\n"
	      << "\n"
	      << Version_gpl
	      << "\n"
    ;

  return temp_stream.str();
}


/****  set  ****/

void
Options::set_option(struct option *option,
		    const char *name,
		    int has_arg,
		    int *flag,
		    int val)
{
  option->name = name;
  option->has_arg = has_arg;
  option->flag = flag;
  option->val = val;
}


/****  display and exit  ****/

void
Options::display_and_exit_usage(bool is_error) const
{
  int exit_status;

  if (is_error)
    {
      std::cerr << this->usage;
      exit_status = EXIT_FAILURE;
    }
  else
    {
      std::cout << this->usage;
      exit_status = EXIT_SUCCESS;
    }

  exit(exit_status);
}


void
Options::display_and_exit_version(void) const
{
  std::cout << this->Version;

  exit(EXIT_SUCCESS);
}
