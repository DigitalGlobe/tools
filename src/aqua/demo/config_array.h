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

/*  class templates for arrays  */


#ifndef CONFIG_ARRAY_H
#define CONFIG_ARRAY_H


/****************  includes  ****************/


/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  classes declarations  ****************/


template<typename TYPE = const float>
class Config_Array
{
public:

  Config_Array(TYPE * const array, size_t size, size_t index);


  /****  increase/decrease  ****/
  bool increase(void);
  bool decrease(void);

  /****  get  ****/
  TYPE get(void) const;
  size_t get_index(void) const;


protected:

  TYPE * const array;  /*  size  */

  const size_t size;

  size_t index;


private:

  /****  not defined  ****/
  Config_Array(const class Config_Array<TYPE> &);
  void operator =(const class Config_Array<TYPE> &);
};


/****************  public functions  ****************/


template<typename TYPE>
Config_Array<TYPE>::Config_Array(TYPE * const array,
				 size_t size,
				 size_t index)
  : array(array),
    size(size),
    index(index)
{
}


/****  increase/decrease  ****/

template<typename TYPE>
bool
Config_Array<TYPE>::increase(void)
{
  bool change_index;;

  if (this->index < this->size - 1)
    {
      this->index++;
      change_index = true;
    }
  else
    {
      change_index = false;
    }

  return change_index;
}


template<typename TYPE>
bool
Config_Array<TYPE>::decrease(void)
{
  bool change_index;

  if (this->index > 0)
    {
      this->index--;
      change_index = true;
    }
  else
    {
      change_index = false;
    }

  return change_index;
}


/****  get  ****/

template<typename TYPE>
TYPE
Config_Array<TYPE>::get(void) const
{
  return this->array[this->index];
}


template<typename TYPE>
size_t
Config_Array<TYPE>::get_index(void) const
{
  return this->index;
}


#endif  /*  CONFIG_ARRAY_H  */
