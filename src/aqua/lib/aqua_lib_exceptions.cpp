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

/*  exceptions management  */


/****************  includes  ****************/


#include "aqua_lib_exceptions.h"

#include "aqua_lib_utilities.h"  /*  get_file_name()  */

/*  C lib  */
#include <cstdlib>  /*  EXIT_FAILURE  */

/*  C++ lib  */
#include <iostream>   /*  cerr  */
#include <exception>  /*  class exception  */
#include <new>        /*  class bad_alloc  */


/****************  namespaces  ****************/


using std::cerr;
using std::endl;


/****************  functions  ****************/


namespace aqua_lib
{
  void
  exceptions(char *program_name, const char *exception)
  {
    program_name = get_file_name(program_name);

    cerr << "\n"
	 << program_name << " error: " << exception
	 << endl
      ;

    exit(EXIT_FAILURE);
  }


  void
  exceptions(char *program_name, class std::bad_alloc &exception)
  {
    program_name = get_file_name(program_name);

    cerr << "\n"
	 << program_name << " error: unable to allocate memory "
	 << "(" << exception.what() << ")"
	 << endl
      ;

    exit(EXIT_FAILURE);
  }


  void
  exceptions(char *program_name, class std::exception &exception)
  {
    program_name = get_file_name(program_name);

    cerr << "\n"
	 << program_name << " error: unknown exception " << exception.what()
	 << endl
      ;

    exit(EXIT_FAILURE);
  }
}
