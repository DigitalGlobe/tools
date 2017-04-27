// $Id$

#ifndef pqe_Quaternion_HEADER
#define pqe_Quaternion_HEADER 1

#include <pqe/Constants.h>
#include <pqe/Common.h>
#include <pqe/ColumnVector3d.h>
#include <pqe/Dpt3d.h>

namespace pqe
{
   class Matrix4x4;

   /**
    * A quaternion class.
    * It can be used to represent an orientation in 3D space.
    */
   class PQEDLL Quaternion
   {
   public:
      
      pqe_float64 theVector[4];    // a four-vector
      
      Quaternion()
      {
         theVector[0]=0.0;
         theVector[1]=0.0;
         theVector[2]=0.0;
         theVector[3]=1.0;
      }

      Quaternion( pqe_float64 x, pqe_float64 y, pqe_float64 z, pqe_float64 w )
      {
         theVector[0]=x;
         theVector[1]=y;
         theVector[2]=z;
         theVector[3]=w;
      }

      Quaternion(pqe_float64 angle, const pqe::Dpt3d& axis)
      {
         makeRotate(angle,axis);
      }

      Quaternion(pqe_float64 angle, const pqe::ColumnVector3d& axis)
      {
         makeRotate(angle,axis);
      }

      Quaternion& operator = (const Quaternion& v)
      {
         theVector[0]=v.theVector[0];
         theVector[1]=v.theVector[1];
         theVector[2]=v.theVector[2];
         theVector[3]=v.theVector[3];
         return *this;
      }
      
      bool operator == (const Quaternion& v) const
      {
         return
            theVector[0]==v.theVector[0] &&
            theVector[1]==v.theVector[1] &&
            theVector[2]==v.theVector[2] && theVector[3]==v.theVector[3];
      }
      
      bool operator != (const Quaternion& v) const
      {
         return
            theVector[0]!=v.theVector[0] ||
            theVector[1]!=v.theVector[1] ||
            theVector[2]!=v.theVector[2] ||
            theVector[3]!=v.theVector[3];
      }

      bool operator <  (const Quaternion& v) const
      {
         if (theVector[0]<v.theVector[0]) return true;
         else if (theVector[0]>v.theVector[0]) return false;
         else if (theVector[1]<v.theVector[1]) return true;
         else if (theVector[1]>v.theVector[1]) return false;
         else if (theVector[2]<v.theVector[2]) return true;
         else if (theVector[2]>v.theVector[2]) return false;
         else return (theVector[3]<v.theVector[3]);
      }

      void set(pqe_float64 x, pqe_float64 y, pqe_float64 z, pqe_float64 w)
      {
         theVector[0]=x;
         theVector[1]=y;
         theVector[2]=z;
         theVector[3]=w;
      }
      
      void set(const pqe::Matrix4x4& matrix);
      
      void get(pqe::Matrix4x4& matrix) const;
      
      
      pqe_float64 & operator [] (int i) { return theVector[i]; }
      pqe_float64   operator [] (int i) const { return theVector[i]; }
      
      pqe_float64 & x() { return theVector[0]; }
      pqe_float64 & y() { return theVector[1]; }
      pqe_float64 & z() { return theVector[2]; }
      pqe_float64 & w() { return theVector[3]; }
      
      pqe_float64 x() const { return theVector[0]; }
      pqe_float64 y() const { return theVector[1]; }
      pqe_float64 z() const { return theVector[2]; }
      pqe_float64 w() const { return theVector[3]; }

      /**
       * return true if the Quaternion represents a zero rotation,
       * and therefore can be ignored in computations.
       */
      bool zeroRotation() const
      {
         return
            theVector[0]==0.0 &&
            theVector[1]==0.0 &&
            theVector[2]==0.0 &&
            theVector[3]==1.0;
      } 
      

      /* ------------------------------------------------------------- 
         BASIC ARITHMETIC METHODS            
         Implemented in terms of Vec4s.  Some Vec4 operators, e.g.
         operator* are not appropriate for quaternions (as
         mathematical objects) so they are implemented differently.
         Also define methods for conjugate and the multiplicative inverse.            
         ------------------------------------------------------------- */
      // Multiply by scalar 
      const Quaternion operator * (pqe_float64 rhs) const
      {
         return Quaternion(theVector[0]*rhs,
                           theVector[1]*rhs,
                           theVector[2]*rhs,
                           theVector[3]*rhs);
      }

      /// Unary multiply by scalar 
      Quaternion& operator *= (pqe_float64 rhs)
      {
         theVector[0]*=rhs;
         theVector[1]*=rhs;
         theVector[2]*=rhs;
         theVector[3]*=rhs;
         return *this;        // enable nesting
      }

      /// Binary multiply 
      const Quaternion operator*(const Quaternion& rhs) const
      {
         return Quaternion( rhs.theVector[3]*theVector[0] + rhs.theVector[0]*theVector[3] + rhs.theVector[1]*theVector[2] - rhs.theVector[2]*theVector[1],
                            rhs.theVector[3]*theVector[1] - rhs.theVector[0]*theVector[2] + rhs.theVector[1]*theVector[3] + rhs.theVector[2]*theVector[0],
                            rhs.theVector[3]*theVector[2] + rhs.theVector[0]*theVector[1] - rhs.theVector[1]*theVector[0] + rhs.theVector[2]*theVector[3],
                            rhs.theVector[3]*theVector[3] - rhs.theVector[0]*theVector[0] - rhs.theVector[1]*theVector[1] - rhs.theVector[2]*theVector[2] );
      }

      /// Unary multiply 
      Quaternion& operator*=(const Quaternion& rhs)
      {
         pqe_float64 x = rhs.theVector[3]*theVector[0] + rhs.theVector[0]*theVector[3] + rhs.theVector[1]*theVector[2] - rhs.theVector[2]*theVector[1];
         pqe_float64 y = rhs.theVector[3]*theVector[1] - rhs.theVector[0]*theVector[2] + rhs.theVector[1]*theVector[3] + rhs.theVector[2]*theVector[0];
         pqe_float64 z = rhs.theVector[3]*theVector[2] + rhs.theVector[0]*theVector[1] - rhs.theVector[1]*theVector[0] + rhs.theVector[2]*theVector[3];
         theVector[3]   = rhs.theVector[3]*theVector[3] - rhs.theVector[0]*theVector[0] - rhs.theVector[1]*theVector[1] - rhs.theVector[2]*theVector[2];
         
         theVector[2] = z;
         theVector[1] = y;
         theVector[0] = x;
         
         return (*this);            // enable nesting
      }
      
      /// Divide by scalar 
      Quaternion operator / (pqe_float64 rhs) const
      {
         pqe_float64 div = 1.0/rhs;
         return Quaternion(theVector[0]*div, theVector[1]*div, theVector[2]*div, theVector[3]*div);
      }
      
      /// Unary divide by scalar 
      Quaternion& operator /= (pqe_float64 rhs)
      {
         pqe_float64 div = 1.0/rhs;
         theVector[0]*=div;
         theVector[1]*=div;
         theVector[2]*=div;
         theVector[3]*=div;
         return *this;
      }
      
      /// Binary divide 
      const Quaternion operator/(const Quaternion& denom) const
      {
         return ( (*this) * denom.inverse() );
      }
      
      /// Unary divide 
      Quaternion& operator/=(const Quaternion& denom)
      {
         (*this) = (*this) * denom.inverse();
         return (*this);            // enable nesting
      }
      
      /// Binary addition 
      const Quaternion operator + (const Quaternion& rhs) const
      {
         return Quaternion(theVector[0]+rhs.theVector[0], theVector[1]+rhs.theVector[1],
                           theVector[2]+rhs.theVector[2], theVector[3]+rhs.theVector[3]);
      }
      
      /// Unary addition
      Quaternion& operator += (const Quaternion& rhs)
      {
         theVector[0] += rhs.theVector[0];
         theVector[1] += rhs.theVector[1];
         theVector[2] += rhs.theVector[2];
         theVector[3] += rhs.theVector[3];
         return *this;            // enable nesting
      }
      
      /// Binary subtraction 
      const Quaternion operator - (const Quaternion& rhs) const
      {
         return Quaternion(theVector[0]-rhs.theVector[0], theVector[1]-rhs.theVector[1],
                           theVector[2]-rhs.theVector[2], theVector[3]-rhs.theVector[3] );
      }
      
      /// Unary subtraction 
      Quaternion& operator -= (const Quaternion& rhs)
      {
         theVector[0]-=rhs.theVector[0];
         theVector[1]-=rhs.theVector[1];
         theVector[2]-=rhs.theVector[2];
         theVector[3]-=rhs.theVector[3];
         return *this;            // enable nesting
      }

      /** Negation operator - returns the negative of the quaternion.
          Basically just calls operator - () on the Vec4 */
      const Quaternion operator - () const
      {
         return Quaternion (-theVector[0], -theVector[1], -theVector[2], -theVector[3]);
      }
      
      /// Length of the quaternion = sqrt( vec . vec )
      pqe_float64 length() const
      {
         return std::sqrt( theVector[0]*theVector[0] + theVector[1]*theVector[1] + theVector[2]*theVector[2] + theVector[3]*theVector[3]);
      }
      
      /// Length of the quaternion = vec . vec
      pqe_float64 length2() const
      {
         return theVector[0]*theVector[0] + theVector[1]*theVector[1] + theVector[2]*theVector[2] + theVector[3]*theVector[3];
      }
      
      /// Conjugate 
      Quaternion conj () const
      { 
         return Quaternion( -theVector[0], -theVector[1], -theVector[2], theVector[3] );
      }
      
      /// Multiplicative inverse method: q^(-1) = q^*/(q.q^*)
      const Quaternion inverse () const
      {
         return conj() / length2();
      }
      
      /* -------------------------------------------------------- 
         METHODS RELATED TO ROTATIONS
         Set a quaternion which will perform a rotation of an
         angle around the axis given by the vector (x,y,z).
         Should be written to also accept an angle and a Vec3?
         
         Define Spherical Linear interpolation method also
         
         Not inlined - see the Quat.cpp file for implementation
         -------------------------------------------------------- */
      void makeRotate( pqe_float64  angle, 
                       pqe_float64  x, pqe_float64  y, pqe_float64  z );
      void makeRotate ( pqe_float64  angle, const pqe::ColumnVector3d& vec )
      { makeRotate(angle, vec[0], vec[1], vec[2]);}
      void makeRotate ( pqe_float64  angle, const pqe::Dpt3d& vec )
      { makeRotate(angle, vec.x, vec.y, vec.z);}
      
      void makeRotate ( pqe_float64  angle1, const pqe::ColumnVector3d& axis1, 
                        pqe_float64  angle2, const pqe::ColumnVector3d& axis2,
                        pqe_float64  angle3, const pqe::ColumnVector3d& axis3);
      void makeRotate ( pqe_float64  angle1, const pqe::Dpt3d& axis1, 
                        pqe_float64  angle2, const pqe::Dpt3d& axis2,
                        pqe_float64  angle3, const pqe::Dpt3d& axis3);
      
      /** Make a rotation Quaternion which will rotate vec1 to vec2.
          Generally take a dot product to get the angle between these
          and then use a cross product to get the rotation axis
          Watch out for the two special cases when the vectors
          are co-incident or opposite in direction.*/
      void makeRotate( const pqe::ColumnVector3d& vec1, const pqe::ColumnVector3d& vec2 );
      /** Make a rotation Quaternion which will rotate vec1 to vec2.
          Generally take a dot product to get the angle between these
          and then use a cross product to get the rotation axis
          Watch out for the two special cases of when the vectors
          are co-incident or opposite in direction.*/
      void makeRotate( const pqe::Dpt3d& vec1, const pqe::Dpt3d& vec2 );
      
      /**
       * void makeRotate_original( const Vec3d& vec1, const Vec3d& vec2 ); */
      
      /**
       * Return the angle and vector components represented by the quaternion.
       * Angle returned is in degrees.
       */
      void getRotate ( pqe_float64 & angle, pqe_float64 & x, pqe_float64 & y, pqe_float64 & z ) const;
      
      /**
       * Return the angle and vector components represented by the quaternion.
       * Angle returned is in degrees.
       */
      void getRotate ( pqe_float64& angle, pqe::Dpt3d& vec ) const
      { getRotate(angle, vec.x, vec.y, vec.z);}
      
      
      /** Spherical Linear Interpolation.
          As it goes from 0 to 1, the Quaternion object goes from "from" to "to". */
      void slerp   ( pqe_float64  t, const Quaternion& from, const Quaternion& to);
      /** Rotate a vector by this quaternion.*/
      pqe::Dpt3d operator* (const pqe::Dpt3d& v) const
      {
         // nVidia SDK implementation
         pqe::Dpt3d uv, uuv; 
         pqe::Dpt3d qvec(theVector[0], theVector[1], theVector[2]);
         uv = qvec ^ v;
         uuv = qvec ^ uv; 
         uv *= ( 2.0f * theVector[3] ); 
         uuv *= 2.0f; 
         return v + uv + uuv;
      }

      friend pqe::Dpt3d operator *(const pqe::Dpt3d& lhs,
                                   const pqe::Quaternion& rhs)
      {
         pqe::Dpt3d uv, uuv; 
         pqe::Dpt3d qvec(rhs.theVector[0], rhs.theVector[1], rhs.theVector[2]);
         uv = qvec ^ lhs;
         uuv = qvec ^ uv; 
         uv *= ( 2.0f * rhs.theVector[3] ); 
         uuv *= 2.0f;
         
         return lhs + uv + uuv;
      }
      
   };    // end of class prototype
   
}    // end of namespace

#endif /* #ifndef pqe_Quaternion_HEADER */
