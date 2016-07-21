//*****************************************************************************
// FILE: EcefPoint.cpp
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains declaration of a 3D point object in the Earth-centered, earth
//   fixed (ECEF) coordinate system.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (http://www.oscarkramer.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id$

#include <ostream>

#include <pqe/EcefPoint.h>
#include <pqe/Gpt.h>
#include <pqe/Dpt3d.h>
#include <pqe/Datum.h>
#include <pqe/Ellipsoid.h>
#include <pqe/EcefVector.h>

pqe::EcefPoint::EcefPoint(const pqe::Gpt&  gpt)
{
   if(!gpt.isHgtNan())
   {
      gpt.datum()->ellipsoid()->latLonHeightToXYZ(gpt.latd(),
                                                  gpt.lond(),
                                                  gpt.height(),
                                                  theData[0],
                                                  theData[1],
                                                  theData[2]);
   }
   else
   {
      gpt.datum()->ellipsoid()->latLonHeightToXYZ(gpt.latd(),
                                                  gpt.lond(),
                                                  0.0,
                                                  theData[0],
                                                  theData[1],
                                                  theData[2]);
   }
}

pqe::EcefPoint::EcefPoint(const pqe::Dpt3d& pt)
   :theData(pt.x, pt.y, pt.z)
{
}

pqe::EcefVector pqe::EcefPoint::operator-(const pqe::EcefPoint& p) const
{
   return pqe::EcefVector(theData - p.theData);
}

pqe::EcefPoint pqe::EcefPoint::operator+(const pqe::EcefVector& v) const
{
   return pqe::EcefPoint(theData + v.data());
}

pqe::EcefPoint pqe::EcefPoint::operator-(const pqe::EcefVector& v) const
{
   return pqe::EcefPoint(theData - v.data());
}

std::string pqe::EcefPoint::toString(pqe_uint32 precision) const
{
   return theData.toString(precision);
}

void pqe::EcefPoint::toPoint(const std::string& s)
{
   theData.toPoint(s);
}

std::ostream& pqe::EcefPoint::print(std::ostream& os) const
{
   if(isNan())
   {
      os << "(EcefPoint) " << "nan nan nan";
   }
   
   os << "(EcefPoint) " << theData;
   
   return os;
}

std::ostream& pqe::operator<<(std::ostream& os ,
                              const pqe::EcefPoint& instance)
{
   return instance.print(os);
}
