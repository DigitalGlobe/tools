//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description: This is your 3-D vector where the 4-th dimension is
//              for the homogenious coordinate.
//
//*******************************************************************
//  $Id$

#ifndef pqe_ColumnVector4d_HEADER
#define pqe_ColumnVector4d_HEADER 1

#include <cfloat> // for FLT_EPSILON
#include <cmath>

#include <pqe/Constants.h>
#include <pqe/Common.h>

namespace pqe
{
   class PQEDLL ColumnVector4d
   {
   public:
      ColumnVector4d()
      {
         data[0]=0;
         data[1]=0;
         data[2]=0;
         data[3]=1;
      }

      ColumnVector4d(pqe_float64 x,
                     pqe_float64 y,
                     pqe_float64 z,
                     pqe_float64 w=1.0)
      {
         data[0]=x;
         data[1]=y;
         data[2]=z;
         data[3]=w;
      }

      ColumnVector4d(const pqe::ColumnVector4d &rhs)
      {
         data[0] = rhs.data[0];
         data[1] = rhs.data[1];
         data[2] = rhs.data[2];
         data[3] = rhs.data[3];
      }
      
      friend std::ostream& operator<<(std::ostream& out,
                                      const pqe::ColumnVector4d& v);
      
      friend pqe::ColumnVector4d operator*(pqe_float64 scalar, const pqe::ColumnVector4d &v)
      {
         return pqe::ColumnVector4d(v.data[0]*scalar,
                                    v.data[1]*scalar,
                                    v.data[2]*scalar);
      }
      
      pqe_float64& operator [](int index)
      {
         return data[index];
      }
      
      const pqe_float64& operator [](int index)const
      {
         return data[index];
      }
      
      pqe::ColumnVector4d operator*(pqe_float64 scalar)const
      {
         return pqe::ColumnVector4d(data[0]*scalar,
                                    data[1]*scalar,
                                    data[2]*scalar);
      }
      
      pqe::ColumnVector4d operator +(const pqe::ColumnVector4d &rhs)const
      {
         return pqe::ColumnVector4d( data[0]+rhs[0],
                                     data[1]+rhs[1],
                                     data[2]+rhs[2]);
      }
      
      pqe::ColumnVector4d operator +=(const pqe::ColumnVector4d &rhs)
      {
         data[0] += rhs[0];
         data[1] += rhs[1];
         data[2] += rhs[2];
         
         return *this;
      }
      
      pqe::ColumnVector4d operator - ()const
      {
         return pqe::ColumnVector4d(-data[0],
                                    -data[1],
                                    -data[2]);
      }
      
      pqe::ColumnVector4d operator /(pqe_float64 scalar)const
      {
         return pqe::ColumnVector4d( data[0]/scalar,
                                     data[1]/scalar,
                                     data[2]/scalar);
      }
      
      pqe::ColumnVector4d& operator /=(pqe_float64 scalar)
      {
         data[0]/=scalar;
         data[1]/=scalar;
         data[2]/=scalar;
         
         return *this;
      }
      
      pqe::ColumnVector4d operator -(const pqe::ColumnVector4d &rhs)const
      {
         return pqe::ColumnVector4d( data[0]-rhs[0],
                                     data[1]-rhs[1],
                                     data[2]-rhs[2]);
      }
      
      pqe::ColumnVector4d operator -=(const pqe::ColumnVector4d &rhs)
      {
         data[0] -= rhs[0];
         data[1] -= rhs[1];
         data[2] -= rhs[2];
         
         return *this;
      }
      
      pqe_float64 magnitude()const
      {
         return std::sqrt(data[0]*data[0] +
                          data[1]*data[1] +
                          data[2]*data[2]);
      }
      pqe_float64 dot(const pqe::ColumnVector4d &rhs)const
      {
         return (data[0]*rhs[0]+
                 data[1]*rhs[1]+
                 data[2]*rhs[2]);
      }
      
      pqe::ColumnVector4d unit()const
      {
         pqe_float64 mag = magnitude();
         
         if(std::abs(mag) > DBL_EPSILON)
         {
            mag = 1.0/mag;
            return (*this *mag);
         }
         
         return *this;
      }
      
      pqe::ColumnVector4d cross(const pqe::ColumnVector4d &rhs)
      {
         return pqe::ColumnVector4d( data[1]*rhs[2] - data[2]*rhs[1],
                                     data[2]*rhs[0] - data[0]*rhs[2],
                                     data[0]*rhs[1] - data[1]*rhs[0]);
      }
      
      pqe::ColumnVector4d& xAligned()// creates 4-D homogeneous vectors
      {
         data[0] = 1;
         data[1] = 0;
         data[2] = 0;
         data[3] = 1;
         
         return *this;
      }
      
      pqe::ColumnVector4d& yAligned()//
      {
         data[0] = 0;
         data[1] = 1;
         data[2] = 0;
         data[3] = 1;
         
         return *this;
      }
      pqe::ColumnVector4d& zAligned() //
      {
         data[0] = 0;
         data[1] = 0;
         data[2] = 1;
         data[3] = 1;
         
         return *this;
      }
      
   private:
      pqe_float64 data[4];
      
   }; // class ColumnVector4d

} // namespace pqe

#endif
