//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Generic PQE Exception that is a std::exception with a
// what() method implemented.
//----------------------------------------------------------------------------
// $Id$

#include <pqe/Exception.h>

pqe::Exception::Exception() throw()
   : theErrorMessage()
{
}

pqe::Exception::Exception(const std::string& errorMessage) throw()
   : theErrorMessage(errorMessage)
{
}

pqe::Exception::~Exception() throw()
{
}

const char* pqe::Exception::what() const throw()
{
   return theErrorMessage.c_str();
}
