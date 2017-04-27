//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
//
// DESCRIPTION:
//   Contains implementation of class 
//
// SOFTWARE HISTORY:
//>
//   ddmmm2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
// $Id$

#include <pqe/LsrVector.h>
#include <pqe/EcefVector.h>
#include <pqe/Common.h>

//*****************************************************************************
//  CONSTRUCTOR: pqe::LsrVector(pqe::EcefVector, pqe::LsrSpace)
//  
//  Establishes an LSR vector given the ECEF equivalent and the LSR space.
//  
//*****************************************************************************
pqe::LsrVector::LsrVector(const pqe::EcefVector& convert_this,
                          const pqe::LsrSpace&   new_space)
   : theLsrSpace (new_space)
{
   initialize(convert_this);
}

//*****************************************************************************
//  CONSTRUCTOR: pqe::LsrVector(pqe::LsrVector, pqe::LsrSpace)
//  
//  Establishes an LSR vector in a new space given another LSR vector in a
//  different space.
//  
//*****************************************************************************
pqe::LsrVector::LsrVector(const pqe::LsrVector& convert_this,
                          const pqe::LsrSpace&  new_space)
   : theLsrSpace (new_space)
{
   initialize(pqe::EcefVector(convert_this));
}

//*****************************************************************************
//  METHOD: pqe::LsrVector::dot(pqe::LsrVector)
//  
//  Computes the scalar product.
//  
//*****************************************************************************
double pqe::LsrVector::dot(const pqe::LsrVector& v) const
{
   if(hasNans()||v.hasNans()||(theLsrSpace != v.theLsrSpace))
   {
      pqe::LsrSpace::lsrSpaceErrorMessage(std::cout);
      return pqe::nan();
   }
   return theData.dot(v.data());
}

//*****************************************************************************
//  METHOD: pqe::LsrVector::angleTo(pqe::LsrVector)
//  
//  Returns the angle subtended (in DEGREES) between this and arg vector
//
//*****************************************************************************
double pqe::LsrVector::angleTo(const pqe::LsrVector& v) const
{
   if(hasNans()||v.hasNans()||(theLsrSpace != v.theLsrSpace))
   {
      pqe::LsrSpace::lsrSpaceErrorMessage(std::cout);
      return pqe::nan();
   }
   double mag_product = theData.magnitude() * v.theData.magnitude();
   return pqe::acosd(theData.dot(v.theData)/mag_product);
}

//*****************************************************************************
//  METHOD: pqe::LsrVector::cross(pqe::LsrVector)
//  
//  Computes the cross product.
//  
//*****************************************************************************
pqe::LsrVector pqe::LsrVector::cross(const pqe::LsrVector& v) const
{
   if(hasNans()||v.hasNans()||(theLsrSpace != v.theLsrSpace))
   {
      pqe::LsrSpace::lsrSpaceErrorMessage(std::cout);
      return pqe::LsrVector(pqe::nan(), pqe::nan(), pqe::nan(),
                            theLsrSpace);
   }
   return pqe::LsrVector(theData.cross(v.data()), theLsrSpace);
}

std::ostream& pqe::LsrVector::print(std::ostream& os) const
{
   os << "(pqe::LsrVector)\n"
      << "  theData = " << theData
      << "\n  theLsrSpace = " << theLsrSpace;
   return os;
}

std::ostream& operator<<(std::ostream& os ,
                         const pqe::LsrVector& instance)
{
   return instance.print(os);
}

