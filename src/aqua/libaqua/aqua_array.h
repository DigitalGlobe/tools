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
   Class template for interleaved arrays.
*/


/*  LIBAQUA HEADER FILE  */


#ifndef AQUA_ARRAY_H
#define AQUA_ARRAY_H


/****************  Includes  ****************/


/*  C lib  */
#include <cstddef>  /*  size_t  */


/****************  Class declarations  ****************/


namespace aqua
{
  /**  Interleaved array class template.  */
  template<unsigned int STRIDE, typename TYPE = float>
  class Array
  {
  public:

    /****  Constructors and destructor  ****/
    Array(size_t size_i, size_t size_j);
    ~Array(void);

    /****  Get  ****/

    size_t get_size(void) const;
    size_t get_size_i(void) const;
    size_t get_size_j(void) const;

    TYPE & get(unsigned int dimension, size_t i, size_t j);
    TYPE   get(unsigned int dimension, size_t i, size_t j) const;

    TYPE & get(unsigned int dimension, size_t i);
    TYPE   get(unsigned int dimension, size_t i) const;

          TYPE * get_element(size_t i);
    const TYPE * get_element(size_t i) const;

          TYPE * get_element(size_t i, size_t j);
    const TYPE * get_element(size_t i, size_t j) const;

          TYPE * get_array(void);
    const TYPE * get_array(void) const;



  protected:

    /****  Constants  ****/
    /*  Logical sizes of the array  */
    const size_t size;    /*  size_i × size_j  */
    const size_t size_i;
    const size_t size_j;
    const size_t width;

    TYPE * const array;         /*  STRIDE × size_i × size_j  */


  private:

    /****  Not defined  ****/
    Array(const class Array<STRIDE, TYPE> &);
    void operator =(const class Array<STRIDE, TYPE> &);
  };
}


/****************  Public function definitions  ****************/


namespace aqua
{
  template<unsigned int STRIDE, typename TYPE>
  Array<STRIDE, TYPE>::Array(size_t size_i, size_t size_j)
    : size(size_i * size_j),
      size_i(size_i),
      size_j(size_j),
      width(size_j * STRIDE),
      array(new TYPE[size_i * this->width])
  {
  }


  template<unsigned int STRIDE, typename TYPE>
  Array<STRIDE, TYPE>::~Array(void)
  {
    delete[] this->array;
  }


  /****  Get  ****/

  template<unsigned int STRIDE, typename TYPE>
  size_t
  Array<STRIDE, TYPE>::get_size(void) const
  {
    return this->size;
  }


  template<unsigned int STRIDE, typename TYPE>
  size_t
  Array<STRIDE, TYPE>::get_size_i(void) const
  {
    return this->size_i;
  }


  template<unsigned int STRIDE, typename TYPE>
  size_t
  Array<STRIDE, TYPE>::get_size_j(void) const
  {
    return this->size_j;
  }


  template<unsigned int STRIDE, typename TYPE>
  inline TYPE &
  Array<STRIDE, TYPE>::get(unsigned int dimension, size_t i, size_t j)
  {
    return this->array[i * this->width + j * STRIDE + dimension];
  }


  template<unsigned int STRIDE, typename TYPE>
  inline TYPE
  Array<STRIDE, TYPE>::get(unsigned int dimension, size_t i, size_t j)
    const
  {
    return this->array[i * this->width + j * STRIDE + dimension];
  }


  template<unsigned int STRIDE, typename TYPE>
  inline TYPE &
  Array<STRIDE, TYPE>::get(unsigned int dimension, size_t i)
  {
    return this->array[i * STRIDE + dimension];
  }


  template<unsigned int STRIDE, typename TYPE>
  inline TYPE
  Array<STRIDE, TYPE>::get(unsigned int dimension, size_t i) const
  {
    return this->array[i * STRIDE + dimension];
  }


  template<unsigned int STRIDE, typename TYPE>
  inline TYPE *
  Array<STRIDE, TYPE>::get_element(size_t i)
  {
    return this->array + i * STRIDE;
  }


  template<unsigned int STRIDE, typename TYPE>
  inline const TYPE *
  Array<STRIDE, TYPE>::get_element(size_t i) const
  {
    return this->array + i * STRIDE;
  }


  template<unsigned int STRIDE, typename TYPE>
  inline TYPE *
  Array<STRIDE, TYPE>::get_element(size_t i, size_t j)
  {
    return this->array + i * this->width + j * STRIDE;
  }


  template<unsigned int STRIDE, typename TYPE>
  inline const TYPE *
  Array<STRIDE, TYPE>::get_element(size_t i, size_t j) const
  {
    return this->array + i * this->width + j * STRIDE;
  }


  template<unsigned int STRIDE, typename TYPE>
  TYPE *
  Array<STRIDE, TYPE>::get_array(void)
  {
    return this->array;
  }


  template<unsigned int STRIDE, typename TYPE>
  const TYPE *
  Array<STRIDE, TYPE>::get_array(void) const
  {
    return this->array;
  }
}


#endif  /*  AQUA_ARRAY_H  */
