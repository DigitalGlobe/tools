/*  Emacs mode:  -*- C++ -*-  */
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

/*  help message for JONSWAP spectrum  */


#ifndef AQUA_DEMO_HELP_JONSWAP_H
#define AQUA_DEMO_HELP_JONSWAP_H


/****************  includes  ****************/


#include "help.h"

/*  C++ lib  */
#include <string>


/****************  polymorphic classes  ****************/


class Help_Jonswap : public Help
{
public:

  Help_Jonswap(void);
  virtual ~Help_Jonswap(void);

  virtual std::string make_string(void) const;


protected:

  /*  copy constructor to be defined  */
  Help_Jonswap(const class Help_Jonswap &);
};


#endif  /*  AQUA_DEMO_HELP_JONSWAP_H  */
