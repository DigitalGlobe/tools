//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// This is the class declaration for ossimEllipsoid. Though valid for the
// general class of geometric shape, this implementation contains additional
// methods suited to the Earth ellipsoid.
//
//*******************************************************************
// $Id$

#ifndef pqe_Ellipsoid_HEADER
#define pqe_Ellipsoid_HEADER 1

#include <string>

#include <pqe/Constants.h>
#include <pqe/newmat.h>

namespace pqe
{
   class EcefRay;
   class EcefPoint;
   class EcefVector;
   class Matrix4x4;
   
   class PQEDLL Ellipsoid
   {
   public:
      
      /**
       * CONSTRUCTORS...
       */
      Ellipsoid(const pqe::Ellipsoid &ellipsoid);
      Ellipsoid(const std::string &name,
                const std::string &code,
                const pqe_float64 &major_axis,
                const pqe_float64 &minor_axis);
      Ellipsoid(const pqe_float64 &major_axis,
                const pqe_float64 &minor_axis);
      Ellipsoid();
      
      ~Ellipsoid(){};
      
      /**
       * ACCESS METHOD...
       */
      const std::string& name() const;
      const std::string& code() const;
      
      const pqe_float64&    a() const; // major axis
      const pqe_float64&    b() const; // minor axis
      
      const pqe_float64& getA() const; 
      const pqe_float64& getB() const;
      const pqe_float64& getFlattening() const;
      
      void setA(pqe_float64 a);
      void setB(pqe_float64 b);
      void setAB(pqe_float64 a, pqe_float64 b);
      
      pqe_float64 eccentricitySquared() const;
      
      pqe_float64 flattening() const;
      
      pqe_float64 eccentricity() const;
   
      /**
       * METHOD: nearestIntersection()
       * Returns the point of nearest intersection of the ray with the ellipsoid.
       * The first version performs the intersection at the ellipsoid surface.
       * The second version computes the ray's intersection with a surface at
       * some offset outside (for positive offset) of the ellipsoid (think
       * elevation).
       */
      bool nearestIntersection(const pqe::EcefRay&  ray,
                               pqe::EcefPoint& rtnPt) const;
      bool nearestIntersection(const pqe::EcefRay&  ray,
                               const pqe_float64&        offset,
                               pqe::EcefPoint& rtnPt) const;
   
      /**
       * METHOD: evaluate()
       * evaluate will evalate the function at location x, y, z (ECEF).
       */
      pqe_float64   evaluate(const pqe::EcefPoint&)const;
      
      /**
       * METHOD: gradient()
       * Compute the partials along location x, y, and z and place
       * the result in the result vector.
       */
      void gradient(const pqe::EcefPoint& location,
                    pqe::EcefVector& result)const;
      pqe::EcefVector gradient(const pqe::EcefPoint& loc)const;
      
      /**
       * METHOD: prinRadiiOfCurv()
       * Computes the meridional radius and prime vertical at given point.
       */
      void prinRadiiOfCurv(const pqe::EcefPoint& location,
                           pqe_float64& merRadius,
                           pqe_float64& primeVert)const;
      
      /**
       * METHOD: jacobianWrtEcef()
       * Forms Jacobian of partials of geodetic WRT ECF.
       */
      void jacobianWrtEcef(const pqe::EcefPoint& location,
                           pqe::Matrix& jMat)const;
      
      /**
       * METHOD: jacobianWrtGeo()
       * Forms Jacobian of partials of ECF WRT geodetic.
       */
      void jacobianWrtGeo(const pqe::EcefPoint& location,
                          pqe::Matrix& jMat)const;
      
      /**
       * Computes the "geodetic" radius for a given latitude in DEGREES:
       */
      pqe_float64 geodeticRadius(const pqe_float64& latitude) const;
      
      void latLonHeightToXYZ(pqe_float64 lat,
                             pqe_float64 lon,
                             pqe_float64 height,
                             pqe_float64 &x,
                             pqe_float64 &y,
                             pqe_float64 &z)const;
      
      void XYZToLatLonHeight(pqe_float64 x,
                             pqe_float64 y,
                             pqe_float64 z,
                             pqe_float64& lat,
                             pqe_float64& lon,
                             pqe_float64& height)const;
      
      //---
      // this is a utility from open scene graph that allows you to create a
      // local space rotational
      // and translation matrix
      //---
      void computeLocalToWorldTransformFromXYZ(
         pqe_float64 x,
         pqe_float64 y,
         pqe_float64 z,
         pqe::Matrix4x4& localToWorld)const;
      
      bool operator==(const pqe::Ellipsoid& rhs)const;
      
   protected:
      
      void computeFlattening();
      
      std::string theName;
      std::string theCode;
      pqe_float64 theA;           //semi-major axis in meters
      pqe_float64 theB;           //semi-minor axis in meters
      pqe_float64 theFlattening;
      pqe_float64 theA_squared;
      pqe_float64 theB_squared;
      pqe_float64 theEccentricitySquared;
      
   }; // End of class Ellipsoid.

} // End of namespace pqe.

#endif /* End of #ifndef pqe_Ellipsoid_HEADER. */
