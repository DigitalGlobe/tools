//*****************************************************************************
// FILE: EcefVector.cpp
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
//*****************************************************************************
// $Id$

#include <sstream>
#include <ostream>

#include <pqe/EcefVector.h>

std::string pqe::EcefVector::toString(pqe_uint32 precision) const
{
   return theData.toString(precision);
}

void pqe::EcefVector::toPoint(const std::string& s)
{
   theData.toPoint(s);
}

std::ostream& pqe::EcefVector::print(std::ostream& os) const
{
   os << "(pqe::EcefVector) " << theData;
   return os;
}

std::ostream& pqe::operator<<(std::ostream& os ,
                              const pqe::EcefVector& instance)
{
   return instance.print(os);
}
