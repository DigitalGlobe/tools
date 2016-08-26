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

/**
   \file
   Specialization of “class Array” template with a “STRIDE” value of 1.
*/


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_ARRAY_1_H
#define AQUA_ARRAY_1_H


/****************  includes  ****************/


#include <libaqua/aqua_array.h>

/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  Classes declarations  ****************/


namespace aqua
{
  /**  Interleaved array class template.  */
  template<typename TYPE>
  class Array<1, TYPE>
  {
  public:

    /****  Constructors and destructor  ****/
    Array(size_t size_i, size_t size_j);
    ~Array(void);


    /****  Get  ****/
    size_t get_size(void) const;
    size_t get_size_i(void) const;
    size_t get_size_j(void) const;
    /*  Returns this->array[i][j].  */
    TYPE & get(size_t i, size_t j);      
    TYPE   get(size_t i, size_t j) const;
    /*  Returns this->array[i].  */
    TYPE & get(size_t i);             
    TYPE   get(size_t i) const;
    TYPE & operator[](size_t i);
    TYPE   operator[](size_t i) const;

          TYPE * get_array(void);
    const TYPE * get_array(void) const;


  protected:

    const size_t size;    /*  size_i × size_y  */
    const size_t size_i;
    const size_t size_j;

    TYPE * const array;  /*  size_i × size_y  */


  private:

    /****  Not defined  ****/
    Array(const class Array<1, TYPE> &);
    void operator=(const class Array<1, TYPE> &);
  };
}


/****************  Public function definitions  ****************/


namespace aqua
{

  template<typename TYPE>
  Array<1, TYPE>::Array(size_t size_i, size_t size_j)
    : size(size_i * size_j),
      size_i(size_i),
      size_j(size_j),
      array(new TYPE[this->size])
  {
  }


  template<typename TYPE>
  Array<1, TYPE>::~Array(void)
  {
    delete[] this->array;
  }


  /****  Get  ****/

  template<typename TYPE>
  size_t
  Array<1, TYPE>::get_size_i(void) const
  {
    return this->size_i;
  }


  template<typename TYPE>
  size_t
  Array<1, TYPE>::get_size_j(void) const
  {
    return this->size_j;
  }


  template<typename TYPE>
  size_t
  Array<1, TYPE>::get_size(void) const
  {
    return this->size;
  }


  template<typename TYPE>
  inline TYPE &
  Array<1, TYPE>::get(size_t i, size_t j)
  {
    return this->array[i * this->size_j + j];
  }


  template<typename TYPE>
  inline TYPE
  Array<1, TYPE>::get(size_t i, size_t j) const
  {
    return this->array[i * this->size_j + j];
  }


  template<typename TYPE>
  inline TYPE &
  Array<1, TYPE>::get(size_t i)
  {
    return this->array[i];
  }


  template<typename TYPE>
  inline TYPE
  Array<1, TYPE>::get(size_t i) const
  {
    return this->array[i];
  }


  template<typename TYPE>
  inline TYPE &
  Array<1, TYPE>::operator[](size_t i)
  {
    return this->array[i];
  }


  template<typename TYPE>
  inline TYPE
  Array<1, TYPE>::operator[](size_t i) const
  {
    return this->array[i];
  }


  template<typename TYPE>
  TYPE *
  Array<1, TYPE>::get_array(void)
  {
    return this->array;
  }
}


#endif  /*  AQUA_ARRAY_1_H  */
