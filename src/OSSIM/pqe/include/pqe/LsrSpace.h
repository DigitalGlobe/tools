//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//  This class maintains the relationship between a local space rectangular
//  (LSR) coordinate system and the earth-centered, earth-fixed (ECEF) system.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
// $Id$

#ifndef pqe_LsrSpace_HEADER
#define pqe_LsrSpace_HEADER 1

#include <iosfwd>

#include <pqe/Constants.h>
#include <pqe/EcefPoint.h>
#include <pqe/Matrix3x3.h>

namespace pqe
{
   class EcefVector;
   class Gpt;

   //**************************************************************************
   //  CLASS: LsrSpace
   //
   //  This class maintains the relationship between a local space rectangular
   //  (LSR) coordinate system and the earth-centered, earth-fixed (ECEF)
   //  system.
   //
   //**************************************************************************
   class PQEDLL LsrSpace
   {
   public:
      /*!
       * DEFAULT & COPY CONSTRUCTOR: 
       */
      LsrSpace() {}
      
      LsrSpace(const pqe::LsrSpace& copy_this)
         :
	 theLsrToEcefRotMatrix(copy_this.theLsrToEcefRotMatrix),
	 theOrigin(copy_this.theOrigin) {}
         
      /*!
       * CONSTRUCTORS: The following three constructors accept an ECEF origin
       * and two of the three axes direction vectors. Note the use of the dummy
       * "int" argument (not used) to hold the place of the unspecified axes. 
       */
      LsrSpace(const pqe::EcefPoint&  origin,
               const pqe::EcefVector& x_dir_ecf_vec,
               const pqe::EcefVector& y_dir_ecf_vec,
               const int z_not_provided_space_holder);
      
      LsrSpace(const pqe::EcefPoint&  origin,
               const pqe::EcefVector& x_dir_ecf_vec,
               const int y_not_provided_space_holder,
               const pqe::EcefVector& z_dir_ecf_vec);
      
      LsrSpace(const pqe::EcefPoint&  origin,
               const int x_not_provided_space_holder,
               const pqe::EcefVector& y_dir_ecf_vec,
               const pqe::EcefVector& z_dir_ecf_vec);
      
      /*!
       * CONSTRUCTOR: LsrSpace(pqe::Gpt, y_azimuth)
       * This constructor sets up a local coordinate system centered at the
       * specified groundpoint, with the Z-axis normal to the ellipsoid and the
       * Y-axis rotated clockwise from north by the y_azimuth.
       * This angle defaults
       * to 0, producing an East-North-Up (ENU) system.
       */
      LsrSpace(const pqe::Gpt& origin,
               const double&   y_azimuth=0);
      
      /*!
       * CONSTRUCTOR: LsrSpace(pqe::EcefPt, pqe::Matrix)
       * This constructor sets up a local coordinate system centered at the
       * specified ECF, with given LSR-to-ECF rotation.
       */
      LsrSpace(const pqe::EcefPoint& origin,
                    const pqe::Matrix& lsrToEcfRot)
         : theLsrToEcefRotMatrix (lsrToEcfRot), theOrigin (origin) {}
         
      ~LsrSpace() {}
      
      /*!
       * OPERATORS:
       */
      bool operator == (const pqe::LsrSpace&) const;
      
      bool operator != (const pqe::LsrSpace& that) const
      { return !(*this == that); }
      
      const pqe::LsrSpace& operator = (const pqe::LsrSpace& space);
      
      /*!
       * DATA ACCESS METHODS:
       */
      const pqe::EcefPoint&  origin()          const { return theOrigin; }
      const pqe::Matrix&  lsrToEcefRotMatrix() const
      { return theLsrToEcefRotMatrix; }

      pqe::Matrix ecefToLsrRotMatrix() const
      { return theLsrToEcefRotMatrix.t(); }
      
      /*!
       * METHOD: lsrSpaceErrorMessage()
       * Convenience method accessible to all owners of an pqe::LsrSpace for
       * displaying an error message when LSR spaces do not match between
       * objects. All operations between LSR objects must be in a common space.
       */
      static std::ostream& lsrSpaceErrorMessage(std::ostream& os);
      
      /*!
       * Debug Dump: 
       */
      std::ostream& print(std::ostream& stream) const;
      
      friend std::ostream& operator<<(std::ostream& os,
                                      const pqe::LsrSpace& instance);
      
   private:
      pqe::Matrix      theLsrToEcefRotMatrix;
      pqe::EcefPoint   theOrigin;
      
   }; // End of: class LsrSpace

} // End of: namespace pqe

#endif /* #ifndef pqe_LsrSpace_HEADER */
