//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//   Contains declaration of a 3D point object in the Earth-centered, earth
//   fixed (ECEF) coordinate system.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
// $Id$

#include <iostream>

#include <pqe/LsrPoint.h>
#include <pqe/Gpt.h>
#include <pqe/LsrVector.h>
#include <pqe/Matrix3x3.h>

pqe::LsrPoint::LsrPoint(const pqe::LsrPoint& convert_this,
                        const pqe::LsrSpace& new_space)
   : theLsrSpace(new_space)
{
   initialize(pqe::EcefPoint(convert_this));
}

pqe::LsrPoint::LsrPoint(const pqe::Gpt&  gpt,
                        const pqe::LsrSpace& space)
   : theLsrSpace(space)
{
   initialize(pqe::EcefPoint(gpt));
}

pqe::LsrPoint::LsrPoint(const pqe::EcefPoint& convert_this,
                        const pqe::LsrSpace& space)
   : theLsrSpace(space)
{
   initialize(convert_this);
}

//*****************************************************************************
//  PROTECTED METHOD: pqe::LsrPoint::initialize(pqe::EcefPoint)
//  
//  Convenience method used by several constructors for initializing theData
//  given an ECEF point. Assumes theLsrSpace has been previously initialized.
//  
//*****************************************************************************
void pqe::LsrPoint::initialize(const pqe::EcefPoint& ecef_point)
{
   if(ecef_point.hasNans())
   {
      makeNan();
   }
   else
   {
      //
      // Translate to new space given the space's offset origin:
      //
      pqe::ColumnVector3d xlated ((ecef_point - theLsrSpace.origin()).data());
      
      //
      // Rotate by the inverse (transpose) of the LSR-to-ECEF rot matrix:
      //
      theData = theLsrSpace.ecefToLsrRotMatrix() * xlated;
   }
}

pqe::LsrVector pqe::LsrPoint::operator-(const pqe::LsrPoint& p) const
{
   if(hasNans()||p.hasNans()||(theLsrSpace != p.lsrSpace()))
   {
      theLsrSpace.lsrSpaceErrorMessage(std::cout);
      return pqe::LsrVector(pqe::nan(), pqe::nan(), pqe::nan(), theLsrSpace);
      
   }
   return pqe::LsrVector(theData-p.theData, theLsrSpace);
}

pqe::LsrPoint pqe::LsrPoint::operator+(const pqe::LsrVector& v) const
{
   if(hasNans()||v.hasNans()||(theLsrSpace != v.lsrSpace()))
   {
      pqe::LsrSpace::lsrSpaceErrorMessage(std::cout);
      return pqe::LsrPoint(pqe::nan(), pqe::nan(), pqe::nan(), theLsrSpace);
      
   }
   return pqe::LsrPoint(theData + v.data(), theLsrSpace);
}

std::ostream& pqe::LsrPoint::print(std::ostream& os) const
{
   os << "(pqe::LsrPoint)\n"
      << "  theData = " << theData
      << "\n  theLsrSpace = " << theLsrSpace;
   return os;
}

std::ostream& operator<<(std::ostream& os, const pqe::LsrPoint& instance)
{
   return instance.print(os);
}
