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

/*  help message for Pierson and Moskowitz spectrum  */


/****************  includes  ****************/


#include "help_jonswap.h"

#include "config/config_help.h"

/*  C++ lib  */
#include <sstream>  /*  ostringstream  */
#include <string>


/****************  namespaces  ****************/


using namespace config::help;


/****************  public functions  ****************/


Help_Jonswap::Help_Jonswap(void)
{
}


/*  virtual  */
Help_Jonswap::~Help_Jonswap(void)
{
}


/****************  protected functions  ****************/


std::string
Help_Jonswap::make_string(void) const
{
  std::ostringstream temp_stream;

  temp_stream
    << "  increase / decrease fetch: "
    << Fetch_increase << " / " << Fetch_decrease
    << "\n"
    ;

  return this->Help::make_string() + temp_stream.str();
}
