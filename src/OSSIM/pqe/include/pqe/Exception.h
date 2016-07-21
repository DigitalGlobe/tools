//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Generic PQE Exception that is a std::exception with a
// what() method implemented.
//----------------------------------------------------------------------------
// $Id: pqeException.h 10029 2006-12-04 23:23:11Z dburken $
#ifndef pqeException_HEADER
#define pqeException_HEADER

#include <exception>
#include <string>
#include <pqe/Constants.h>

namespace pqe
{
   class PQEDLL Exception : public std::exception
   {
   public:
      
      /** @brief default construction */
      Exception() throw();
      
      /**
       * @brief construction that takes an error string.
       * @param errorMessage The error message.
       */
      Exception(const std::string& errorMessage) throw();
      
      /** @brief virtual destructor. */
      virtual ~Exception() throw();
      
      /**
       * @brief Returns the error message.
       * @return The error message as a C-style character string.
       */
      virtual const char* what() const throw();
      
   private:
      
      /** This is the error message returned by what(). */
      std::string theErrorMessage;
      
   }; // end class Exception
   
} // end namespace pqe

#endif /* End of #ifndef pqeException_HEADER */
