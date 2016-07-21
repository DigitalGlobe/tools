//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:  
//
// Contains class declaration for dpt3d
// Used to represent a 3d double point containing an x, y and z data member.
//*******************************************************************
//  $Id$

#ifndef pqe_Dpt3d_HEADER
#define pqe_Dpt3d_HEADER 1

#include <cmath>

#include <iosfwd>

#include <pqe/Constants.h>
#include <pqe/Dpt.h>
#include <pqe/ColumnVector3d.h>

namespace pqe
{
   class PQEDLL Dpt3d
   {
   public:
      
      friend inline std::ostream & operator<<(std::ostream& out,
                                              const pqe::Dpt3d& rhs);
      Dpt3d(const pqe_float64 &aX=0.0,
            const pqe_float64 &aY=0.0,
            const pqe_float64 &aZ=0.0)
         :x(aX), y(aY), z(aZ) {}

      Dpt3d(const pqe::Dpt& aPt);
   
      Dpt3d(const pqe::ColumnVector3d &pt)
         : x(pt[0]), y(pt[1]), z(pt[2]) {}
   
      bool operator ==(const pqe::Dpt3d &rhs) const
      {
         return ( (x == rhs.x) &&
                  (y == rhs.y) &&
                  (z == rhs.z));
      }
      
      bool operator !=(const pqe::Dpt3d &rhs) const
      {
         return ( (x != rhs.x) ||
                  (y != rhs.y) ||
                  (z != rhs.z) );
      }
      
      void makeNan(){x = pqe::nan(); y=pqe::nan(); z=pqe::nan();}

      bool hasNans()const
      {
         return (pqe::isnan(x) || pqe::isnan(y) || pqe::isnan(z));
      }
      
      /**
       * METHOD: length()
       * Returns the RSS of the components.
       */
      pqe_float64 length() const  { return std::sqrt(x*x + y*y + z*z); }
      pqe_float64 length2() const { return x*x + y*y + z*z; }
      
      //---
      // OPERATORS: +, -, +=, -=
      // Point add/subtract with other point:
      //---
      pqe::Dpt3d operator+(const pqe::Dpt3d& p) const
      { return pqe::Dpt3d(x+p.x, y+p.y, z+p.z); }

      pqe::Dpt3d operator-(const pqe::Dpt3d& p) const
      { return pqe::Dpt3d(x-p.x, y-p.y, z-p.z); }
      
      const pqe::Dpt3d& operator+=(const pqe::Dpt3d& p)
      { x += p.x; y += p.y; z += p.z; return *this; }

      const pqe::Dpt3d& operator-=(const pqe::Dpt3d& p)
      { x -= p.x; y -= p.y; z -= p.z; return *this; }

      //---
      // OPERATORS: *, /
      // Scale point components by scalar:
      //---
      pqe::Dpt3d operator*(const pqe_float64& d) const
      { return pqe::Dpt3d(d*x, d*y, d*z); }

      pqe::Dpt3d operator/(const pqe_float64& d) const
      { return pqe::Dpt3d(x/d, y/d, z/d); }

      void operator /=(pqe_float64 value)
      {
         x /= value;
         y /= value;
         z /= value;
      }
      
      void operator *=(pqe_float64 value) 
      {
         x *= value;
         y *= value;
         z *= value;
      }
      
      pqe_float64 operator *(const pqe::Dpt3d& src)const
      {
         return (x*src.x + y*src.y + z*src.z);
      }
      inline const pqe::Dpt3d operator ^ (const pqe::Dpt3d& rhs) const
      {
         return pqe::Dpt3d(y*rhs.z-z*rhs.y,
                           z*rhs.x-x*rhs.z ,
                           x*rhs.y-y*rhs.x);
      }
      pqe_float64 x;
      pqe_float64 y;
      pqe_float64 z;
      
   }; // class Dpt3d

} // namespace pqe

#endif /* #ifndef pqe_Dpt3d_HEADER */
