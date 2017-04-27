//*****************************************************************************
// FILE: EcefPoint.h
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
//  $Id: EcefPoint.h 11860 2007-10-15 19:59:10Z dburken $

#ifndef pqe_EcefPoint_HEADER
#define pqe_EcefPoint_HEADER 1

#include <iosfwd>

#include <pqe/Constants.h>
#include <pqe/Common.h>
#include <pqe/ColumnVector3d.h>

namespace pqe
{
   class Gpt;
   class EcefVector;
   class Dpt3d;
   
   class PQEDLL EcefPoint
   {
   public:
      /**
       * CONSTRUCTORS: 
       */
      EcefPoint() : theData(0,0,0) {}

      EcefPoint(const EcefPoint& copy_this) : theData (copy_this.theData) {}
         
      EcefPoint(const Gpt& convert_this);

      EcefPoint(const pqe_float64& x,
                const pqe_float64& y,
                const pqe_float64& z)
         : theData(x, y, z) {}

      EcefPoint(const ColumnVector3d& assign_this)
         : theData(assign_this) {}

      EcefPoint(const Dpt3d& pt);
   
      void makeNan()
      {
         theData[0] = pqe::nan();
         theData[1] = pqe::nan();
         theData[2] = pqe::nan();
      }
   
      bool hasNans()const
      {
         return ( pqe::isnan(theData[0]) ||
                  pqe::isnan(theData[1]) ||
                  pqe::isnan(theData[2]) );
         
      }
      
      bool isNan()const
      {
         return ( pqe::isnan(theData[0]) &&
                  pqe::isnan(theData[1]) &&
                  pqe::isnan(theData[2]) );
         
      }
      
      /**
       * OPERATORS:
       */
      EcefVector       operator- (const EcefPoint&)  const;
      EcefPoint        operator+ (const EcefVector&) const;
      EcefPoint        operator- (const EcefVector&) const;
      const EcefPoint& operator= (const EcefPoint&);        // inline
      bool             operator==(const EcefPoint&)  const; // inline
      bool             operator!=(const EcefPoint&)  const; // inline
   
      /*!
       * COMPONENT ACCESS METHODS: 
       */
      pqe_float64    x() const { return theData[0]; }
      pqe_float64&   x()       { return theData[0]; }
      pqe_float64    y() const { return theData[1]; }
      pqe_float64&   y()       { return theData[1]; }
      pqe_float64    z() const { return theData[2]; }
      pqe_float64&   z()       { return theData[2]; }
      pqe_float64&   operator[](int idx){return theData[idx];}
      const pqe_float64&   operator[](int idx)const{return theData[idx];}
      const ColumnVector3d& data() const { return theData; }
      ColumnVector3d&       data()       { return theData; }
      
      pqe_float64 getMagnitude() const
      {
         return theData.magnitude();
      }
      pqe_float64 magnitude()const
      {
         return theData.magnitude();
      }
      pqe_float64 length()const
      {
         return theData.magnitude();
      }
      pqe_float64 normalize()
      {
         pqe_float64 result = magnitude();
         
         if(result > 1e-15)
         {
            theData[0]/=result;
            theData[1]/=result;
            theData[2]/=result;
         }
         
         return result;
      }
      
      /**
       * @brief To string method.
       * 
       * @param precision Output floating point precision.
       * 
       * @return std::string representing point.
       *
       * Output format:
       * ( 0.0000000,  0.0000000,  0.00000000 )
       *   -----x----  -----y----  ------z----
       */
      std::string toString(pqe_uint32 precision=15) const;
      
      /**
       * @brief Initializes this point from string.
       *
       * Expected format:
       * 
       * ( 0.0000000,  0.0000000,  0.00000000 )
       *   -----x----  -----y----  ------z----
       *
       * @param s String to initialize from.
       */
      void toPoint(const std::string& s);
      
      /*!
       * Debug Dump: 
       */
      std::ostream& print(std::ostream& os) const;
      
      friend PQEDLL std::ostream& operator<<(std::ostream& os ,
                                             const EcefPoint& instance);
      
   protected:
      ColumnVector3d theData;
      
   }; // class PQEDLL EcefPoint

   //=============== BEGIN DEFINITIONS FOR INLINE METHODS =====================

   inline const EcefPoint& EcefPoint::operator=(const EcefPoint& p)
   {
      theData = p.theData;
      return *this;
   }

   inline bool EcefPoint::operator==(const EcefPoint& p) const
   {
      return (theData == p.theData);
   }

   inline bool EcefPoint::operator!=(const EcefPoint& p) const
   {
      return (theData != p.theData);
   }

} // namespace pqe

#endif /* #ifndef pqe_EcefPoint_HEADER */
