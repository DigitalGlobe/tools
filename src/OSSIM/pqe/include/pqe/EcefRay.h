//------------------------------------------------------------------*
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//  Class for representing a ray in the earth-centered, earth-fixed (ECEF)
//  coordinate system. A ray is defined as having an origin point and a
//  unit direction vector radiating from the origin.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//-----------------------------------------------------------------------------
//  $Id$

#ifndef pqe_EcefRay_HEADER
#define pqe_EcefRay_HEADER 1

#include <iosfwd>

#include <pqe/EcefPoint.h>
#include <pqe/EcefVector.h>
#include <pqe/DatumFactory.h>

namespace pqe
{
   class Gpt;
   class LsrRay;
   
   class PQEDLL EcefRay
   {
   public:
      /**
       * CONSTRUCTORS: 
       */
      EcefRay() {};
      
      EcefRay(const pqe::EcefRay& copy_this)
         : theOrigin(copy_this.theOrigin),
         theDirection(copy_this.theDirection) {}
         
      EcefRay(const pqe::EcefPoint&  origin,
              const pqe::EcefVector& direction)
         : theOrigin(origin), theDirection(direction.unitVector()) {}
         
      EcefRay(const pqe::EcefPoint& from, const pqe::EcefPoint& to);
      
      EcefRay(const pqe::Gpt& from, const pqe::Gpt& to);
      
      bool isNan()const
      {
         return theOrigin.isNan()&&theDirection.isNan();
      }
      
      bool hasNans()const
      {
         return theOrigin.isNan()||theDirection.isNan();
      }
      
      void makeNan()
      {
         theOrigin.makeNan();
         theDirection.makeNan();
      }
      
      /**
       * OPERATORS:
       */
      const pqe::EcefRay& operator= (const pqe::EcefRay& r); // inline below
      bool operator==(const pqe::EcefRay& r) const; // inline below
      bool operator!=(const pqe::EcefRay& r) const; // inline below
      
      /**
    * DATA ACCESS METHODS:
    */
      const pqe::EcefPoint&  origin()    const     { return theOrigin; }
      const pqe::EcefVector& direction() const     { return theDirection; }
      void  setOrigin(const pqe::EcefPoint& orig)  { theOrigin = orig; }
      void  setDirection(const pqe::EcefVector& d)
      {
         theDirection=d.unitVector();
      }

      /**
       * Extends the ray by distance t (meters) from the origin to the ECEF
       * point returned.
       */
      pqe::EcefPoint extend(const double& t) const; // inline below
      
      /**
       * This method computes a ray with the same origin but a new direction
       * corresponding to a reflection from some surface defined by its normal
       * vector (assumed to be a unit vector):
       */
      pqe::EcefRay reflectRay(
         const pqe::EcefVector& normal) const;// inline below
      
      /**
       * Intersects the ray with the given elevation above the earth ellipsoid.
       */
      pqe::EcefPoint intersectAboveEarthEllipsoid(
         const double& heightAboveEllipsoid,
         const pqe::Datum* aDatum =
            pqe::DatumFactory::instance()->wgs84()) const;
      
      /**
       * Debug Dump
       * inline below
       */
      std::ostream& print(std::ostream& os) const; 
      
      friend std::ostream& operator<<(std::ostream& os ,
                                      const pqe::EcefRay& instance);
      
   private:
      pqe::EcefPoint  theOrigin;
      pqe::EcefVector theDirection;
      
   }; // class EcefRay

   //=============== BEGIN DEFINITIONS FOR INLINE METHODS =====================

   inline const pqe::EcefRay& pqe::EcefRay::operator=(const pqe::EcefRay& r)
   {
      theOrigin = r.theOrigin;
      theDirection = r.theDirection;
      return *this;
   }

   inline bool pqe::EcefRay::operator==(const pqe::EcefRay& r) const
   {
      return ((theOrigin == r.theOrigin) && (theDirection == r.theDirection));
   }

   inline bool pqe::EcefRay::operator!=(const pqe::EcefRay& r) const 
   {
      return !(*this == r);
   }

   //--------------------------------------------------------------------------
   //  INLINE METHOD: pqe::EcefRay::extend(double t)
   //  
   //  Extends the ray by distance t (meters) from the origin to the ECEF
   //  point returned.
   //--------------------------------------------------------------------------
   inline pqe::EcefPoint pqe::EcefRay::extend(const double& t) const
   {
      return (theOrigin + theDirection*t);
   }
   
   //--------------------------------------------------------------------------
   //  INLINE METHOD: EcefRay::reflectRay(normal)
   //  
   //   This method computes a ray with the same origin but a new direction
   //   corresponding to a reflection from some surface defined by its normal
   //   vector:
   //--------------------------------------------------------------------------
   inline pqe::EcefRay pqe::EcefRay::reflectRay(
      const pqe::EcefVector& normal) const
   {
      pqe::EcefVector new_dir(theDirection - normal*2.0 *
                              normal.dot(theDirection));
      return pqe::EcefRay(theOrigin, new_dir);
   }

} // namespace pqe
   
#endif /* #ifndef pqe_EcefRay_HEADER */
