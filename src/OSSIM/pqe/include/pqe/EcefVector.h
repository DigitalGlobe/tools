//-----------------------------------------------------------------------------
// FILE: EcefVector.h
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains declaration of a 3D vector object in the Earth-centered, earth
//   fixed (ECEF) coordinate system.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer
//              Initial coding.
//<
//-----------------------------------------------------------------------------
//  $Id: ossimEcefVector.h 11398 2007-07-26 13:29:58Z dburken $

#ifndef pqe_EcefVector_HEADER
#define pqe_EcefVector_HEADER 1

#include <iosfwd>
#include <cmath>

#include <pqe/Common.h>
#include <pqe/ColumnVector3d.h>
#include <pqe/EcefPoint.h>

namespace pqe
{
   class Gpt;

   class PQEDLL EcefVector
   {
   public:
      
      /**
       * CONSTRUCTORS: 
       */
      EcefVector()
         : theData(0,0,0) {}
         
      EcefVector(const pqe::EcefVector& copy_this)
         : theData (copy_this.theData) {}
         
      EcefVector(const pqe::EcefPoint& from,
                 const pqe::EcefPoint& to)
         : theData (to.data() - from.data()) {}
         
      EcefVector(const pqe::Gpt& from,
                 const pqe::Gpt& to)
         : theData ((pqe::EcefPoint(to) - pqe::EcefPoint(from)).data()) {}
         
      EcefVector(const pqe_float64& x,
                 const pqe_float64& y,
                 const pqe_float64& z)
         : theData(x, y, z) {}
         
      EcefVector(const pqe::ColumnVector3d& assign_this)
         : theData(assign_this) {}
         
      void makeNan()
      {
         theData[0] = pqe::nan();
         theData[1] = pqe::nan();
         theData[2] = pqe::nan();
      }
   
      bool hasNans()const
      {
         return ( pqe::isnan(theData[0]) ||
                  pqe::isnan(theData[1]) ||
                  pqe::isnan(theData[2]) );
      }
      
      bool isNan()const
      {
         return ( pqe::isnan(theData[0]) &&
                  pqe::isnan(theData[1]) &&
                  pqe::isnan(theData[2]) );
      }
      
      /**
       * OPERATORS: (all inlined below)
       */
      inline pqe::EcefVector        operator- () const; 
      inline pqe::EcefVector        operator+ (const pqe::EcefVector&) const;
      inline pqe::EcefVector        operator- (const pqe::EcefVector&) const;
      inline pqe::EcefPoint         operator+ (const pqe::EcefPoint&)  const;
      inline pqe::EcefVector        operator* (const pqe_float64&)     const;
      inline pqe::EcefVector        operator/ (const pqe_float64&)     const;
      inline bool                   operator==(const pqe::EcefVector&) const;
      inline bool                   operator!=(const pqe::EcefVector&) const;
      inline const pqe::EcefVector& operator= (const pqe::EcefVector&);
      
      /**
       * Vector-related functions:  (all inlined below)
       */
      inline pqe_float64 dot    (const pqe::EcefVector&) const;
      inline pqe_float64 angleTo(const pqe::EcefVector&) const; // degrees
      
      inline pqe::EcefVector cross  (const pqe::EcefVector&) const;
      inline pqe::EcefVector unitVector()                    const;
      
      inline pqe_float64 magnitude() const; // meters
      inline pqe_float64 norm2()     const; // squared meters
      inline pqe_float64 length()    const;
      inline pqe_float64 normalize();
      
      /**
       * COMPONENT ACCESS METHODS: 
       */
      pqe_float64    x() const { return theData[0]; }
      pqe_float64&   x()       { return theData[0]; }
      pqe_float64    y() const { return theData[1]; }
      pqe_float64&   y()       { return theData[1]; }
      pqe_float64    z() const { return theData[2]; }
      pqe_float64&   z()       { return theData[2]; }
      pqe_float64& operator [](int idx){return theData[idx];}
      const pqe_float64& operator [](int idx)const{return theData[idx];}
      
      const pqe::ColumnVector3d& data() const { return theData; }
      pqe::ColumnVector3d&       data()       { return theData; }
      
      /**
       * @brief To string method.
       * 
       * @param precision Output floating point precision.
       * 
       * @return std::string representing point.
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
      
      /** Debug Dump:  */
      std::ostream& print(std::ostream& os) const;
      
      friend std::ostream& operator<<(std::ostream& os ,
                                      const pqe::EcefVector& instance);
      
   protected:
      pqe::ColumnVector3d theData;

   };  // class EcefVector

   //=============== BEGIN DEFINITIONS FOR INLINE METHODS =====================

   //--------------------------------------------------------------------------
   //  INLINE METHOD: EcefVector::operator-()
   //  Reverses direction of vector.
   //--------------------------------------------------------------------------
   inline pqe::EcefVector pqe::EcefVector::operator-() const
   {
      return pqe::EcefVector(-theData);
   }

   inline pqe::EcefVector pqe::EcefVector::operator+(
      const pqe::EcefVector& v) const
   {
      return pqe::EcefVector(theData + v.theData);
   }

   inline pqe::EcefVector
      pqe::EcefVector::operator-(const pqe::EcefVector& v) const
   {
      return pqe::EcefVector(theData - v.theData);
   }

   inline  pqe::EcefPoint pqe::EcefVector::operator+(
      const pqe::EcefPoint& p) const
   {
      return pqe::EcefPoint(theData + p.data());
   }
   
   inline pqe::EcefVector pqe::EcefVector::operator*(
      const pqe_float64& scalar) const
   {
      return pqe::EcefVector(theData*scalar);
   }

   inline pqe::EcefVector pqe::EcefVector::operator/(
      const pqe_float64& scalar) const
   {
      return pqe::EcefVector(theData/scalar);
   }

   inline bool pqe::EcefVector::operator==(const pqe::EcefVector& v) const
   {
      return (theData == v.theData);
   }

   inline bool pqe::EcefVector::operator!=(const pqe::EcefVector& v) const
   {
      return (theData != v.theData);
   }

   inline const pqe::EcefVector& pqe::EcefVector::operator=(
      const pqe::EcefVector& v)
   {
      theData = v.theData;
      return *this;
   }
   

   //-------------------------------------------------------------------------
   //  INLINE METHOD: pqe::EcefVector::dot()
   //  Computes the scalar product.
   //--------------------------------------------------------------------------
        inline pqe_float64 pqe::EcefVector::dot(const pqe::EcefVector& v) const
   {
      return theData.dot(v.theData);
   }

   //--------------------------------------------------------------------------
   //  INLINE METHOD: pqe::EcefVector::angleTo()
   //  Returns the angle subtended (in DEGREES) between this and arg vector
   //--------------------------------------------------------------------------
   inline pqe_float64 pqe::EcefVector::angleTo(const pqe::EcefVector& v) const
   {
      pqe_float64 mag_product = theData.magnitude() * v.theData.magnitude();
      return pqe::acosd(theData.dot(v.theData)/mag_product);
   }

   //--------------------------------------------------------------------------
   //  INLINE METHOD: pqe::EcefVector::cross()
   //  Computes the cross product.
   //--------------------------------------------------------------------------
   inline pqe::EcefVector pqe::EcefVector::cross(
      const pqe::EcefVector& v) const
   {
      return pqe::EcefVector(theData.cross(v.theData));
   }
   
   //--------------------------------------------------------------------------
   //  INLINE METHOD: pqe::EcefVector::unitVector()
   //  Returns a unit vector parallel to this.
   //--------------------------------------------------------------------------
   inline pqe::EcefVector pqe::EcefVector::unitVector() const
   {
      return pqe::EcefVector(theData/theData.magnitude());
   }
   
   //--------------------------------------------------------------------------
   //  INLINE METHOD: pqe::EcefVector::magnitude()
   //--------------------------------------------------------------------------
   inline pqe_float64 pqe::EcefVector::magnitude() const
   {
      return theData.magnitude();
   }
   
   //--------------------------------------------------------------------------
   //  INLINE METHOD: pqe::EcefVector::norm2()
   //--------------------------------------------------------------------------
   inline pqe_float64 pqe::EcefVector::norm2() const
   {
      return theData.norm2();
   }
   
   //--------------------------------------------------------------------------
   //  INLINE METHOD: pqe::EcefVector::length()
   //--------------------------------------------------------------------------
   inline pqe_float64 pqe::EcefVector::length() const
   {
      return theData.magnitude();
   }
   
   //--------------------------------------------------------------------------
   //  INLINE METHOD: pqe::EcefVector::normalize()
   //  Normalizes this vector.
   //--------------------------------------------------------------------------
   inline pqe_float64 pqe::EcefVector::normalize()
   {
      pqe_float64 result = theData.magnitude();
      if(result > 1e-15)
      {
         theData /= result;
      }
      
      return result;
   }

} // namespace pqe

#endif /* #ifndef pqe_EcefVector_HEADER */
