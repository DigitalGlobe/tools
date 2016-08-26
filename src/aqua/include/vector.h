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

/*  templates for vector mathematics  */


#ifndef AQUA_TEMPLATE_VECTOR_H
#define AQUA_TEMPLATE_VECTOR_H


/****************  includes  ****************/


/*  C  lib  */
#include <cmath>


/****************  functions  ****************/


namespace aqua_math
{


//  std::inner_product() seems (very) little slower
/*  returns the scalar product (or dot product) of "vector1" by "vector2"  */
template <int SIZE, typename TYPE>
inline TYPE
vector_scalar_product(const TYPE vector1[SIZE], const TYPE vector2[SIZE])
{
  TYPE product;
  int i;

  product = 0;

  for (i = 0; i < SIZE; i++)
    {
      product += vector1[i] * vector2[i];
    }

  return product;
}


template <typename TYPE>
inline TYPE
vector_scalar_product(const TYPE vector1_x,
		      const TYPE vector1_y,
		      const TYPE vector2_x,
		      const TYPE vector2_y)
{
  return vector1_x * vector2_x + vector1_y * vector2_y;
}


/*  returns the length of “vector”  */
template <int SIZE>
inline float
vector_length(const float vector[SIZE])
{
  float length;

  /*  like "hypotf()" if (SIZE == 2)  */
  length = sqrtf(vector_scalar_product<SIZE>(vector, vector));

  return length;
}


inline void
vector_cartesian_to_polar(double & magnitude,
			  double & angle,
			  double vector_x,
			  double vector_y)
{
  magnitude = hypot(vector_x, vector_y);
  angle = atan2(vector_y, vector_x);      /*  atan2(Y, X)  */
}


/*  normalizes "vector" of length "length"  */
template <int SIZE>
inline void
vector_normalize(float vector[SIZE], float length)
{
  int i;

  if (length > 0)
    {
      for (i = 0; i < SIZE; i++)
	{
	  vector[i] /= length;
	}
    }
}


/*  normalizes "vector"  */
template <int SIZE>
inline void
vector_normalize(float vector[SIZE])
{
  float length;

  length = vector_length<SIZE>(vector);

  vector_normalize<SIZE>(vector, length);
}


/*  vector product (cross product)  */
inline void
vector_product(float vector1[3], float vector2[3], float result[3])
{
  result[0] = vector1[1] * vector2[2] - vector2[1] * vector1[2];
  result[1] = - (vector1[0] * vector2[2] - vector2[0] * vector1[2]);
  //result[1] = vector2[0] * vector1[2] - vector1[0] * vector2[2];
  result[2] = vector1[0] * vector2[1] - vector2[0] * vector1[1];
}


inline void
vector_product(float vector_1_x,
	       float vector_1_y,
	       float vector_1_z,
	       float vector_2_x,
	       float vector_2_y,
	       float vector_2_z, 
	       float result[3])
{
  result[0] = vector_1_y * vector_2_z - vector_2_y * vector_1_z;
  result[1] = - (vector_1_x * vector_2_z - vector_2_x * vector_1_z);
  result[2] = vector_1_x * vector_2_y - vector_2_x * vector_1_y;
}


/*  "vector_in" and "vector_out" have to be differrent  */
inline void
vector_rotate(float sine,
	      float cosine,
	      float vector_in[2],
	      float vector_out[2])
{
  vector_out[0] = vector_in[0] * cosine - vector_in[1] * sine;
  vector_out[1] = vector_in[0] * sine + vector_in[1] * cosine;
}


}


#endif  /*  AQUA_TEMPLATE_VECTOR_H  */
