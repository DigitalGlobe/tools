//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//  Class for representing vectors in some local space rectangular (LSR)
//  coordinate system. This coordinate system is related to the ECEF system
//  by the ossimLsrSpace member object. This class simplifies coordinate
//  conversions between LSR and ECEF, and other LSR vectors.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
// $Id$

#ifndef pqe_LsrVector_HEADER
#define pqe_LsrVector_HEADER 1

#include <iostream>

#include <pqe/Constants.h>
#include <pqe/LsrPoint.h>
#include <pqe/LsrSpace.h>
#include <pqe/EcefPoint.h>
#include <pqe/EcefVector.h>
#include <pqe/ColumnVector3d.h>

namespace pqe
{

   class Gpt;

   class PQEDLL LsrVector
   {
   public:
      /*!
       * CONSTRUCTORS: 
       */
      LsrVector()
         : theData (0,0,0) {}
         
      LsrVector(const pqe::LsrVector& copy_this)
         : theData(copy_this.theData), theLsrSpace(copy_this.theLsrSpace) {}
         
      LsrVector(const pqe::ColumnVector3d& assign_this,
                const pqe::LsrSpace& space)
         : theData(assign_this), theLsrSpace(space) {}
         
      LsrVector(const double& x,
                const double& y,
                const double& z,
                const pqe::LsrSpace& space)
         : theData(x,y,z), theLsrSpace(space) {}
         
      LsrVector(const pqe::EcefVector& convert_this,
                const pqe::LsrSpace&);
      LsrVector(const pqe::LsrVector& convert_this,
                const pqe::LsrSpace&);

      /*!
       * OPERATORS: (all methods inlined below)
       */
      const pqe::LsrVector&  operator= (const pqe::LsrVector&);
      pqe::LsrVector         operator- ()                      const;
      pqe::LsrVector         operator+ (const pqe::LsrVector&) const;
      pqe::LsrVector         operator- (const pqe::LsrVector&) const;
      pqe::LsrPoint          operator+ (const pqe::LsrPoint&)  const;
      pqe::LsrVector         operator* (const double& scalar)  const;
      pqe::LsrVector         operator/ (const double& scalar)  const;
      bool                   operator==(const pqe::LsrVector&) const;
      bool                   operator!=(const pqe::LsrVector&) const;
      
      /*!
       * CASTING OPERATOR:
       * Used as: myEcefVector = pqe::EcefVector(this) -- looks like a
       * constructor but is an operation on this object.
       * ECEF knows nothing about LSR, so cannot provide an
       * pqe::EcefVector(pqe::LsrVector) constructor.
       */
      operator pqe::EcefVector() const; // inline below
      
      /*!
       * Vector-related functions: 
       */
      double                 dot(const pqe::LsrVector&)       const;
      double                 angleTo(const pqe::LsrVector&)   const;
      pqe::LsrVector         cross(const pqe::LsrVector&)     const;
      pqe::LsrVector         unitVector() const;//inline below
      double                 magnitude()  const;//inline below
      void                   normalize(); // inline below
      
      /*!
       * DATA ACCESS METHODS: 
       */
      double    x() const { return theData[0]; }
      double&   x()       { return theData[0]; }
      double    y() const { return theData[1]; }
      double&   y()       { return theData[1]; }
      double    z() const { return theData[2]; }
      double&   z()       { return theData[2]; }
      
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
      pqe::ColumnVector3d&       data()           { return theData; }
      const pqe::ColumnVector3d& data()     const { return theData; }
      
      pqe::LsrSpace&             lsrSpace()       { return theLsrSpace; }
      const pqe::LsrSpace&       lsrSpace() const { return theLsrSpace; }
      
      /*!
       * Debug Dump: 
       */
      std::ostream&print(std::ostream& stream) const;
      
      friend std::ostream& operator<<(std::ostream& os,
                                      const pqe::LsrVector& instance);
      
   protected:
      /*!
       * METHOD: initialize(pqe::EcefVector)
       * Convenience method used by several constructors for initializing
       * theData given an ECEF vector.
       * Assumes theLsrSpace has been previously initialized
       */
      void initialize(const pqe::EcefVector& ecef_point);
      
      pqe::ColumnVector3d theData;
      pqe::LsrSpace       theLsrSpace;
      
   }; // End of: class LsrVector

   //=============== BEGIN DEFINITIONS FOR INLINE METHODS =====================
   
   inline const pqe::LsrVector& pqe::LsrVector::operator=(
      const pqe::LsrVector& v)
   {
      theData = v.theData;
      theLsrSpace = v.theLsrSpace;
      
      return *this;
   }

   inline pqe::LsrVector pqe::LsrVector::operator-() const
   {
      return pqe::LsrVector(-theData, theLsrSpace);
   }

   inline pqe::LsrVector pqe::LsrVector::operator+(
      const pqe::LsrVector& v) const
   {
      if ((theLsrSpace != v.theLsrSpace)||hasNans()||v.hasNans())
      {
         theLsrSpace.lsrSpaceErrorMessage(std::cout);
         return pqe::LsrVector(pqe::nan(), pqe::nan(), pqe::nan(),
                               theLsrSpace);
      }
      return pqe::LsrVector(theData + v.theData, theLsrSpace);
   }

   inline pqe::LsrVector pqe::LsrVector::operator-(
      const pqe::LsrVector& v) const
   {
      if ((theLsrSpace != v.theLsrSpace)||hasNans()||v.hasNans())
      {
         theLsrSpace.lsrSpaceErrorMessage(std::cout);
         return pqe::LsrVector(pqe::nan(), pqe::nan(), pqe::nan(),
                               theLsrSpace);
      }
      return pqe::LsrVector(theData - v.data(), theLsrSpace);
   }

   inline pqe::LsrPoint pqe::LsrVector::operator+(const pqe::LsrPoint& p) const
   {
      if ((theLsrSpace != p.lsrSpace())||hasNans()||p.hasNans())
      {
         theLsrSpace.lsrSpaceErrorMessage(std::cout);
         return pqe::LsrPoint(pqe::nan(), pqe::nan(), pqe::nan(), theLsrSpace);
      }
      return pqe::LsrPoint(theData + p.data(), theLsrSpace);
   }

   inline pqe::LsrVector pqe::LsrVector::operator*(const double& scalar) const
   {
      return pqe::LsrVector(theData*scalar, theLsrSpace);
   }
   
   inline pqe::LsrVector pqe::LsrVector::operator/(const double& scalar) const
   {
      return pqe::LsrVector(theData/scalar, theLsrSpace);
   }

   inline bool pqe::LsrVector::operator==(const pqe::LsrVector& v) const
   {
      return ((theData == v.theData) && (theLsrSpace == v.theLsrSpace));
   }
   
   inline bool pqe::LsrVector::operator!=(const pqe::LsrVector& v) const
   {
      return (!(*this == v));
   }

   inline pqe::LsrVector::operator pqe::EcefVector() const
   {   
      return pqe::EcefVector(theLsrSpace.lsrToEcefRotMatrix()*theData);
   }   

   //**************************************************************************
   //  INLINE METHOD:  pqe::LsrVector::unitVector()
   //  Returns a unit vector parallel to this.
   //**************************************************************************
   inline pqe::LsrVector pqe::LsrVector::unitVector() const
   {
      if(hasNans())
      {
         return pqe::LsrVector(pqe::nan(), pqe::nan(), pqe::nan(),
                               theLsrSpace);
      }
      return pqe::LsrVector(theData/theData.magnitude(), theLsrSpace);
   }

   inline double pqe::LsrVector::magnitude() const
   {
      if(hasNans()) return pqe::nan();
      return theData.magnitude();
   }

   //**************************************************************************
   //  INLINE METHOD: pqe::LsrVector::normalize()
   //  Normalizes this vector.
   //**************************************************************************
   inline void pqe::LsrVector::normalize()
   {
      theData /= theData.magnitude();
   }

   //**************************************************************************
   //  PROTECTED INLINE METHOD: pqe::LsrPoint::initialize(pqe::EcefPoint)
   //  
   //  Convenience method used by several constructors for initializing theData
   //  given an ECEF point. Assumes theLsrSpace has been previously
   //  initialized.
   //**************************************************************************
   inline void pqe::LsrVector::initialize(const pqe::EcefVector& ecef_vector)
   {
      theData = theLsrSpace.ecefToLsrRotMatrix() * ecef_vector.data();
   }

} // End of: namespace pqe

#endif /*End of: #ifndef pqe_LsrVector_HEADER */
