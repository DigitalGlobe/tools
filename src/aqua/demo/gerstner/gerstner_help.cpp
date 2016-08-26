/*
  This file is part of “The Aqua library”.

  Copyright © 2005  Jocelyn Fréchot

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


#include "gerstner_help.h"

#include "config/config_help.h"

/*  C++ lib  */
#include <sstream>  /*  ostringstream  */
#include <string>


/****************  namespaces  ****************/


using namespace gerstner;
using namespace config::help;


/****************  public functions  ****************/


gerstner::Help::Help(void)
{
}


/*  virtual  */
gerstner::Help::~Help(void)
{
}


/****************  protected functions  ****************/


std::string
gerstner::Help::make_string(void) const
{
  std::ostringstream temp_stream;

  temp_stream
    << "\n"
    << "  increase / decrease number of waves: "
    << Waves_increase << " / " << Waves_decrease
    << "\n"
    ;

  return this->::Help::make_string() + temp_stream.str();
}
