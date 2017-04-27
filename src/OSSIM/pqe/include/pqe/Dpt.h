//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Simple container class for an image point.
//
//----------------------------------------------------------------------------
// $Id: Dpt.h 11216 2007-06-14 21:20:55Z dburken $

#ifndef pqe_Dpt_HEADER
#define pqe_Dpt_HEADER 1

#include <iosfwd>
#include <string>

#include <pqe/Constants.h> /* for dll export */
#include <pqe/Common.h>    /* for nan stuff  */

namespace pqe
{
   /** Container class for a double point (x, y). */
   class PQEDLL Dpt
   {
   public:
      
      /** default constructor */
      Dpt() : x(0.0), y(0.0) {}

      /** constructor that takes an x and y */
      Dpt(pqe_float64 x, pqe_float64 y) : x(x), y(y) {}

      /** assignment opertator */
      const pqe::Dpt& operator=(const pqe::Dpt& pt)
      {
         if (this != &pt)
         {
            x = pt.x;
            y = pt.y;
         }
         return *this;
      }

      /** destructor */
      ~Dpt() {}

      /** Makes x and y nan. */
      void makeNan() { x = pqe::nan(); y = pqe::nan(); }

      /** @return true if x or y are nan. */
      bool hasNans() const { return pqe::isnan(x) || pqe::isnan(y); }

      /** @return true if x and y are nan. */
      bool isNan() const { return pqe::isnan(x) && pqe::isnan(y); }
       
      /**
       * @return std::string representing point.
       *
       * Format:  ( 30.00, -90.00 )
       *             -x-     -y-
       */
      std::string toString() const;

      /**
       * @return std::string representing point.
       *
       * Format:  ( 30.00, -90.00 )
       *             -x-     -y-
       */
      std::string toString(pqe_uint32 precision) const;

      /**
       * Initializes this point from string.  This method opens an istream to
       * s and then calls operator>>.
       *
       * Expected format:  ( 30.00, -90.00 )
       *                      -x-     -y-
       *
       * @param s String to initialize from.
       *
       * @see operator>>
       */
      void toPoint(const std::string& s);
      
      /**
       * @brief Print method.
       * @return std::ostream&
       *
       * Format:  ( 30.00, -90.00 )
       *             -x-     -y-
       */
      std::ostream& print(std::ostream& out, pqe_uint32 precision) const;

      /** Friend operator << function */
      friend PQEDLL std::ostream& operator<<(std::ostream& out,
                                             const pqe::Dpt& obj);

      /**
       * Method to input the formatted string of the "operator<<".
       *
       * Expected format:  ( 30.00, -90.00 )
       *                      -x-     -y-
       * 
       * This method starts by doing a "makeNan" on pt.  So if anything goes
       * wrong with the stream or parsing pt could be all or partially nan.
       *
       * @param is Input stream istream to formatted text.
       * @param pt osimIpt to be initialized from stream.
       * @return istream pass in.
       */
      friend PQEDLL std::istream& operator>>(std::istream& is,
                                             pqe::Dpt& pt);

      //---
      // Public data members:
      //---
      union{pqe_float64 x; pqe_float64 samp; pqe_float64 u; pqe_float64 lon;};
      union{pqe_float64 y; pqe_float64 line; pqe_float64 v; pqe_float64 lat;};

   }; // End of class Dpt.

} // End of namespace pqe.

#endif /* End of #ifndef pqe_Dpt_HEADER */
