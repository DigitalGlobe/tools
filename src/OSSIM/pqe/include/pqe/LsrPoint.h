//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//  Class for representing points in some local space rectangular (LSR)
//  coordinate system. This coordinate system is related to the ECEF system
//  by the ossimLsrSpace member object. This class simplifies coordinate
//  conversions between LSR and ECEF, and other LSR points.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
// $Id$

#ifndef pqe_LsrPoint_HEADER
#define pqe_LsrPoint_HEADER 1

#include <iosfwd>

#include <pqe/Constants.h>
#include <pqe/Common.h>
#include <pqe/LsrSpace.h>
#include <pqe/ColumnVector3d.h>
#include <pqe/EcefVector.h>
#include <pqe/EcefVector.h>

namespace pqe
{
   class Gpt;
   class LsrVector;

   class PQEDLL LsrPoint
   {
   public:
      /*!
       * CONSTRUCTORS: 
       */
      LsrPoint()
         : theData(0,0,0) {}
   
      LsrPoint(const pqe::LsrPoint& copy_this)
         : theData(copy_this.theData), theLsrSpace(copy_this.theLsrSpace) {}
   
      LsrPoint(const pqe::ColumnVector3d& assign_this,
               const pqe::LsrSpace& space)
         : theData(assign_this), theLsrSpace(space) {}
         
      LsrPoint(const double& x,
               const double& y,
               const double& z,
               const pqe::LsrSpace& space)
         : theData(x, y, z), theLsrSpace(space) {}
         
      LsrPoint(const pqe::LsrPoint& convert_this,
               const pqe::LsrSpace&);

      LsrPoint(const pqe::Gpt& convert_this,
               const pqe::LsrSpace&);
      
      LsrPoint(const pqe::EcefPoint& convert_this,
               const pqe::LsrSpace&);
      
      /*!
       * OPERATORS:
       */
      const pqe::LsrPoint& operator= (const pqe::LsrPoint&);
      pqe::LsrVector       operator- (const pqe::LsrPoint&)  const;
      pqe::LsrPoint        operator+ (const pqe::LsrVector&) const;
      bool                 operator==(const pqe::LsrPoint&)  const;
      bool                 operator!=(const pqe::LsrPoint&)  const;

      /*!
       * CASTING OPERATOR:
       * Used as: myEcefVector = pqe::EcefPoint(this) -- looks like a
       * constructor but is an operation on this object.
       * ECEF knows nothing about LSR, so
       * cannot provide an pqe::EcefVector(pqe::LsrPoint) constructor.
       */
      operator pqe::EcefPoint() const; // inline below
      
      /*!
       * DATA ACCESS METHODS: 
       */
      double    x() const { return theData[0]; }
      double&   x()       { return theData[0]; }
      double    y() const { return theData[1]; }
      double&   y()       { return theData[1]; }
      double    z() const { return theData[2]; }
      double&   z()       { return theData[2]; }
      
      
      pqe::ColumnVector3d&       data()           { return theData; }
      const pqe::ColumnVector3d& data()     const { return theData; }
      
      pqe::LsrSpace&             lsrSpace()       { return theLsrSpace; }
      const pqe::LsrSpace&       lsrSpace() const { return theLsrSpace; }
      
      bool hasNans()const
      {
         return (pqe::isnan(theData[0])||
                 pqe::isnan(theData[1])||
                 pqe::isnan(theData[2]));
      }
      
      void makeNan()
      {
         theData[0] = pqe::nan();
         theData[1] = pqe::nan();
         theData[2] = pqe::nan();
      }
      
      /*!
       * Debug Dump: 
       */
      std::ostream& print(std::ostream& stream) const;

      friend std::ostream& operator<<(std::ostream& os , const pqe::LsrPoint& instance);
      
   protected:
      /*!
       * METHOD: initialize(pqe::EcefPoint)
       * Convenience method used by several constructors for initializing
       * theData given an ECEF point. Assumes theLsrSpace has been
       * previously initialized.
       */
      void initialize(const pqe::EcefPoint& ecef_point);
      
      pqe::ColumnVector3d theData;
      pqe::LsrSpace       theLsrSpace;

   }; // End of: class LsrPoint

   //=============== BEGIN DEFINITIONS FOR INLINE METHODS =====================

   inline const pqe::LsrPoint& pqe::LsrPoint::operator=(const pqe::LsrPoint& p)
   {
      theData = p.theData;
      theLsrSpace = p.theLsrSpace;
      
      return *this;
   }

   inline bool pqe::LsrPoint::operator==(const pqe::LsrPoint& p) const
   {
      return ((theData == p.theData) && (theLsrSpace == p.theLsrSpace));
   }
   
   inline bool pqe::LsrPoint::operator!=(const pqe::LsrPoint& p) const
   {
      return (!(*this == p));
   }

   //**************************************************************************
   //  INLINE OPERATOR: pqe::LsrPoint::operator pqe::EcefPoint()
   //
   // Looks like a constructor for an pqe::EcefPoint but is an operation on
   // this object. Returns the pqe::EcefPoint equivalent.
   //**************************************************************************
   inline pqe::LsrPoint::operator pqe::EcefPoint() const
   {
      return pqe::EcefPoint(theLsrSpace.origin().data() +
                            theLsrSpace.lsrToEcefRotMatrix()*theData);
   }
   
} // End of: namespace pqe

#endif /* End of: #ifndef pqe_LsrPoint_HEADER */


