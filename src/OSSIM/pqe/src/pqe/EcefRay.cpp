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
//   10Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//-----------------------------------------------------------------------------
//  $Id$

#include <ostream>

#include <pqe/EcefRay.h>
#include <pqe/Gpt.h>
#include <pqe/Datum.h>
#include <pqe/Ellipsoid.h>

//-----------------------------------------------------------------------------
//  CONSTRUCTOR: EcefRay(EcefPoint, EcefPoint)
//  
//  Constructs a ray originating at "from" and pointing in direction of "to"
//  
//-----------------------------------------------------------------------------
pqe::EcefRay::EcefRay(const EcefPoint& from,
                           const EcefPoint& to)
   : theOrigin(from)
{
   EcefVector ecef (to - from);
   ecef.normalize();
   theDirection = ecef;
}
   

//-----------------------------------------------------------------------------
//  CONSTRUCTOR: EcefRay(Gpt, Gpt)
//  
//  Constructs a ray originating at "from" and pointing in direction of "to"
//  
//-----------------------------------------------------------------------------
pqe::EcefRay::EcefRay(const pqe::Gpt& from, const pqe::Gpt& to)
   : theOrigin(from)
{
   EcefVector ecef (EcefPoint(to) - EcefPoint(from));
   ecef.normalize();
   theDirection = ecef;
}
   
//-----------------------------------------------------------------------------
//  METHOD: pqe::EcefRay::intersectAboveEarthEllipsoid
//  
//  Intersects the ray with the given elevation above the earth ellipsoid.
//  
//-----------------------------------------------------------------------------
pqe::EcefPoint pqe::EcefRay::intersectAboveEarthEllipsoid(
   const double&     argHeight,
   const pqe::Datum* argDatum) const
{
   const pqe::Datum* datum = argDatum;
   if (!datum)
   {
      datum = pqe::DatumFactory::instance()->wgs84();
   }

   //---
   // Pass the call on to ellipsoid where intersection math is implemented:
   //---
   EcefPoint solution;
   bool intersected = datum->ellipsoid()->nearestIntersection
                      (*this, argHeight, solution);

   if (!intersected)
      solution.makeNan();

   return solution;
}

std::ostream& pqe::EcefRay::print(std::ostream& os) const
{
   os << "(pqe::EcefRay)\n"
      << "theOrigin    = " << theOrigin
      << "\ntheDirection = " << theDirection << std::endl;
   return os;
}

std::ostream& operator<< (std::ostream& os , const pqe::EcefRay& instance)
{
   return instance.print(os);
}
