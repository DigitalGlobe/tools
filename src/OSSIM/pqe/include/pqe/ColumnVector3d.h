//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description: This is a 3-D vector without the homogeneous
//              coordinate.
//
//*******************************************************************
// $Id$

#ifndef pqe_ColumnVector3d_HEADER
#define pqe_ColumnVector3d_HEADER 1


#include <iostream>
#include <cfloat> // for FLT_EPSILON
#include <cmath>
#include <string>

#include <pqe/Constants.h>
#include <pqe/newmat.h>

namespace pqe
{
   class PQEDLL ColumnVector3d
   {
   public:
      ColumnVector3d()
      {
         data[0]=0;
         data[1]=0;
         data[2]=0;
      }

      ColumnVector3d(pqe_float64 x, pqe_float64 y, pqe_float64 z=0)
      {
         data[0]=x;
         data[1]=y;
         data[2]=z;
      }
      
      ColumnVector3d(const pqe::ColumnVector3d &rhs)
      {
         data[0] = rhs.data[0];
         data[1] = rhs.data[1];
         data[2] = rhs.data[2];
      }
      
      const pqe::ColumnVector3d& operator=(const pqe::ColumnVector& rhs)
      {
         if ( (rhs.Ncols() == 1) && (rhs.Nrows() == 3) )
         {
            data[0] = rhs[0];
            data[1] = rhs[1];
            data[2] = rhs[2];
         }
         return *this;
      }
      
      friend std::ostream& operator<<(std::ostream& out,
                                      const pqe::ColumnVector3d& v);
      
      friend pqe::ColumnVector3d operator*(pqe_float64 scalar,
                                           const pqe::ColumnVector3d &v)
      {
         return pqe::ColumnVector3d(v.data[0]*scalar,
                                    v.data[1]*scalar,
                                    v.data[2]*scalar);
      }
      
      /*!
       *  Multiplies column vector times matrix.
       *  Note:
       *  If "lhs" is not a 3x3 that's an error and a blank column vector
       *  is returned.
       */
      friend pqe::ColumnVector3d operator*(const pqe::Matrix& lhs,
                                           const pqe::ColumnVector3d& rhs)
      {
         if ((lhs.Ncols() == 3) && (lhs.Nrows() == 3))
         {
            return pqe::ColumnVector3d(
               (lhs[0][0]*rhs[0] + lhs[0][1]*rhs[1] + lhs[0][2]*rhs[2]),
               (lhs[1][0]*rhs[0] + lhs[1][1]*rhs[1] + lhs[1][2]*rhs[2]),
               (lhs[2][0]*rhs[0] + lhs[2][1]*rhs[1] + lhs[2][2]*rhs[2]));
         }
         else if((lhs.Ncols() == 4) && (lhs.Nrows() == 4))
         {
            return pqe::ColumnVector3d(
               (lhs[0][0]*rhs[0] + lhs[0][1]*rhs[1] +
                lhs[0][2]*rhs[2] + lhs[0][3]) ,
               (lhs[1][0]*rhs[0] + lhs[1][1]*rhs[1] +
                lhs[1][2]*rhs[2] + lhs[1][3]) ,
               (lhs[2][0]*rhs[0] + lhs[2][1]*rhs[1] +
                lhs[2][2]*rhs[2] + lhs[2][3]));
         }
         std::cerr << "Multiplying a 3 row column vector by an invalid matrix"
                   << std::endl;
         return rhs;
      }

      pqe_float64& operator [](int index)
      {
         return data[index];
      }
      
      const pqe_float64& operator [](int index)const
      {
         return data[index];
      }
      
      pqe::ColumnVector3d operator*(pqe_float64 scalar)const
      {
         return pqe::ColumnVector3d(data[0]*scalar,
                                    data[1]*scalar,
                                    data[2]*scalar);
      }
      
      pqe::ColumnVector3d operator +(const pqe::ColumnVector3d &rhs)const
      {
         return pqe::ColumnVector3d( data[0]+rhs[0],
                                     data[1]+rhs[1],
                                     data[2]+rhs[2]);
      }
      
      const pqe::ColumnVector3d& operator +=(const pqe::ColumnVector3d &rhs)
      {
         data[0] += rhs[0];
         data[1] += rhs[1];
         data[2] += rhs[2];
         
         return *this;
      }
      
      pqe::ColumnVector3d operator - ()const
      {
         return pqe::ColumnVector3d(-data[0],
                                    -data[1],
                                    -data[2]);
      }
      
      pqe::ColumnVector3d operator /(pqe_float64 scalar)const
      {
         return pqe::ColumnVector3d( data[0]/scalar,
                                     data[1]/scalar,
                                     data[2]/scalar );
      }
      
      const pqe::ColumnVector3d& operator /=(pqe_float64 scalar)
      {
         data[0]/=scalar;
         data[1]/=scalar;
         data[2]/=scalar;
         
         return *this;
      }
      
      pqe::ColumnVector3d operator -(const pqe::ColumnVector3d &rhs)const
      {
         return pqe::ColumnVector3d( data[0]-rhs[0],
                                     data[1]-rhs[1],
                                     data[2]-rhs[2] );
      }
      
      const pqe::ColumnVector3d& operator -=(const pqe::ColumnVector3d &rhs)
      {
         data[0] -= rhs[0];
         data[1] -= rhs[1];
         data[2] -= rhs[2];
         
         return *this;
      }
      
      bool operator ==(const pqe::ColumnVector3d &rhs) const
      {
         return ((fabs(data[0] - rhs[0]) <= FLT_EPSILON) &&
                 (fabs(data[1] - rhs[1]) <= FLT_EPSILON) &&
                 (fabs(data[2] - rhs[2]) <= FLT_EPSILON));
      }
      
      bool operator !=(const pqe::ColumnVector3d &rhs) const
      {
         return !(*this == rhs);
      }
      
      pqe_float64 magnitude()const
      {
         return std::sqrt(data[0]*data[0] +
                          data[1]*data[1] +
                          data[2]*data[2]);
      }
      
      pqe_float64 norm2() const //!speedup
      {
         return data[0]*data[0] + data[1]*data[1] + data[2]*data[2];
      }
      
      pqe_float64 dot(const pqe::ColumnVector3d &rhs) const
      {
         return (data[0]*rhs[0]+
                 data[1]*rhs[1]+
                 data[2]*rhs[2]);
      }
      
      pqe::ColumnVector3d unit() const
      {
         pqe_float64 mag = magnitude();
         
         if(fabs(mag) > FLT_EPSILON)
         {
            mag = 1.0/mag;
            return (*this *mag);
         }
         
         return *this;
      }

      pqe::ColumnVector3d cross(const pqe::ColumnVector3d &rhs) const
      {
         return pqe::ColumnVector3d( data[1]*rhs[2] - data[2]*rhs[1],
                                     data[2]*rhs[0] - data[0]*rhs[2],
                                     data[0]*rhs[1] - data[1]*rhs[0] );
      }
      
      const pqe::ColumnVector3d& xAligned()
      {
         data[0] = 1;
         data[1] = 0;
         data[2] = 0;
         
         return *this;
      }

      const pqe::ColumnVector3d& yAligned()//
      {
         data[0] = 0;
         data[1] = 1;
         data[2] = 0;
         
         return *this;
      }

      const pqe::ColumnVector3d& zAligned() //
      {
         data[0] = 0;
         data[1] = 0;
         data[2] = 1;
         
         return *this;
      }
      
      /**
       * @brief To string method.
       * 
       * @param precision Output floating point precision.
       * 
       * @return ossimString representing point.
       *
       * Output format:
       * ( 0.0000000,  0.0000000,  0.00000000 )
       *   -----x----  -----y----  ------z----
       */
      std::string toString(pqe_uint32 precision=15) const;
      
      /**
       * @brief Initializes this point from string.
       *
       * Expected format:
       * 
       * ( 0.0000000,  0.0000000,  0.00000000 )
       *   -----x----  -----y----  ------z----
       *
       * @param s String to initialize from.
       */
      void toPoint(const std::string& s);
      
   private:
      pqe_float64 data[3];
      
   }; // class pqe::ColumnVector3d

} // namespace pqe


#endif /* #ifndef pqe_ColumnVector3d_HEADER */
