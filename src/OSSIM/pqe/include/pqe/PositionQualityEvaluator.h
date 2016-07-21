//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Class for position quality evaluation operations.
//----------------------------------------------------------------------------
// $Id

#ifndef pqe_PositionQualityEvaluator_HEADER
#define pqe_PositionQualityEvaluator_HEADER 1

#include <vector>

#include <pqe/Constants.h>
#include <pqe/Dpt.h>
#include <pqe/EcefPoint.h>
#include <pqe/Gpt.h>
#include <pqe/LsrVector.h>
#include <pqe/ColumnVector3d.h>
#include <pqe/Matrix3x3.h>
#include <pqe/newmat.h>
#include <pqe/newmatap.h>
#include <pqe/newmatio.h>


namespace pqe
{

   // Probability level specifiers
   enum pqeProbLev_t
   {
      ONE_SIGMA = 0,
      P50,
      P90,
      P95,
      NUM_PROB_LEVELS
   };

   // Ground space ellipse
   struct pqeErrorEllipse
   {
      pqe_float64   theSemiMinorAxis;
      pqe_float64   theSemiMajorAxis;
      pqe_float64   theAzimAngle;
      pqe::Gpt theCenter;
   };

   // RPC model parameters
   struct pqeRPCModel
   {
      pqe_float64 theLineScale;
      pqe_float64 theSampScale;
      pqe_float64 theLatScale;
      pqe_float64 theLonScale;
      pqe_float64 theHgtScale;
      pqe_float64 theLineOffset;
      pqe_float64 theSampOffset;
      pqe_float64 theLatOffset;
      pqe_float64 theLonOffset;
      pqe_float64 theHgtOffset;
      pqe_float64 theLineNumCoef[20];
      pqe_float64 theLineDenCoef[20];
      pqe_float64 theSampNumCoef[20];
      pqe_float64 theSampDenCoef[20];
      char   theType; // A or B
   };
   
   // Image space ellipse point set
   typedef std::vector<pqe::Dpt> pqeImageErrorEllipse_t;

   /**
    * Class for position quality evaluation operations.
    */
   class PQEDLL PositionQualityEvaluator
   {
   public:
      
      /** @brief default constructor */
      PositionQualityEvaluator();
   
      /** @brief covariance matrix-based constructor
       *
       * @param pt     Current ECF ground estimate.
       * @param covMat 3X3 ECF covariance matrix.
       */
      PositionQualityEvaluator(const pqe::EcefPoint& pt,
                               const pqe::Matrix& covMat);
   
      /** @brief LOS error/geometry-based constructor
       *
       * @param pt           Current ECF ground estimate.
       * @param errBiasLOS   0.68p LOS bias component.
       * @param errRandLOS   0.68p LOS random component.
       * @param elevAngleLOS target elevation angle. Units = radians.
       * @param azimAngleLOS target azimuth angle. Units = radians.
       * @param surfN        surface normal unit vector (defaults to unit Z).
       * @param surfCovMat   surface ENU 3X3 covariance matrix
       * (defaults to zero).
       */
      PositionQualityEvaluator(
         const pqe::EcefPoint&      pt,
         const pqe_float64&       errBiasLOS,
         const pqe_float64&       errRandLOS,
         const pqe_float64&       elevAngleLOS,
         const pqe_float64&       azimAngleLOS,
         const pqe::ColumnVector3d& surfN = pqe::ColumnVector3d(0,0,1),
         const pqe::Matrix&         surfCovMat = pqe::Matrix3x3::createZero());
   
      /** @brief LOS error/coefficient-based constructor
       *
       * @param pt         Current ECF ground estimate.
       * @param errBiasLOS 0.68p LOS bias component.
       * @param errRandLOS 0.68p LOS random component.
       * @param rpc        RPC coefficients.
       * @param surfN      surface normal unit vector (defaults to unit Z).
       * @param surfCovMat surface ENU 3X3 covariance matrix
       * (defaults to zero).
       */
      PositionQualityEvaluator(
         const pqe::EcefPoint&    pt,
         const pqe_float64&       errBiasLOS,
         const pqe_float64&       errRandLOS,
         const pqe::pqeRPCModel&         rpc,
         const pqe::ColumnVector3d& surfN = pqe::ColumnVector3d(0,0,1),
         const pqe::Matrix&      surfCovMat = pqe::Matrix3x3::createZero());

      /** @brief destructor */
      ~PositionQualityEvaluator();
   
      /**
       * @brief Add contributing covariance matrix
       *
       * @param covMat 3X3 covariance matrix.
       *
       * @return true on success, false on error.
       */
      bool addContributingCovariance(pqe::Matrix& covMat);
   
      /**
       * @brief Add contributing CE/LE
       *
       * @param cCE contributing 90% circular error (m).
       * @param cLE contributing 90% linear error (m).
       *
       * @return true on success, false on error.
       */
      bool addContributingCE_LE(const pqe_float64& cCE,
                                const pqe_float64& cLE);
   
      /**
       * @brief Subtract contributing covariance matrix
       *
       * @param covMat 3X3 covariance matrix.
       *
       * @return true on success, false on error.
       */
      bool subtractContributingCovariance(pqe::Matrix& covMat);
   
      /**
       * @brief Subtract contributing CE/LE
       *
       * @param cCE contributing 90% circular error (m).
       * @param cLE contributing 90% linear error (m).
       *
       * @return true on success, false on error.
       */
      bool subtractContributingCE_LE(const pqe_float64& cCE,
                                     const pqe_float64& cLE);
   
      /**
       * @brief Covariance matrix access
       *
       * @param covMat 3X3 covariance matrix.
       *
       * @return true on success, false on error.
       */
      bool getCovMatrix(pqe::Matrix& covMat) const;
   
      /**
       * @brief Compute circular/linear error (CE/LE).
       *
       * @param pLev Probability level.
       * @param CE   pLev% circular error (m).
       * @param LE   pLev% linear error (m).
       *
       * @return true on success, false on error.
       */
      bool computeCE_LE(const pqe::pqeProbLev_t pLev,
                        pqe_float64& CE,
                        pqe_float64& LE) const;

   
      /**
       * @brief Extract error ellipse parameters
       *
       * @param pLev    Probability level.
       * @param ellipse pLev% error ellipse.
       *
       * @return true on success, false on error.
       */
      bool extractErrorEllipse(const pqe::pqeProbLev_t pLev,
                               pqe::pqeErrorEllipse& ellipse);
      
      
      /**
       * @brief Extract error ellipse parameters;
       * valid only with RPC parameters
       *
       * @param pLev             Probability level.
       *
       * @param angularIncrement Angular increment for ellipse point
       * spacing (deg)
       *
       * @param ellipse          pLev% error ellipse.
       *
       * @param ellImage         pLev% image space error ellipse.
       *
       * @return true on success, false on error.
       */
      bool extractErrorEllipse(const pqe::pqeProbLev_t pLev,
                               const pqe_float64& angularIncrement,
                               pqe::pqeErrorEllipse& ellipse,
                               pqeImageErrorEllipse_t& ellImage);
                                  
      
      /**
       * @brief State accessor.
       */
      inline bool isValid() const { return theEvaluatorValid; }
   
      /**
       * @brief Print method.
       */
      std::ostream& print(std::ostream& out) const;

   private:
      //---
      // Members
      //---
      bool                      theEvaluatorValid;
      pqe::Gpt                  thePtG;
      pqe::Matrix               theCovMat;  //local ENU frame
      pqe::LsrSpace             theLocalFrame;
      pqe::pqeErrorEllipse theEllipse;
      pqe::Matrix               theEigenvectors;
      pqe::pqeRPCModel          theRpcModel;
      
      //---
      // Method for eigenvector decomposition of covariance matrix
      //---
      bool decomposeMatrix();
   
      //---
      // Method for covariance matrix formation from 1-sigma LOS
      // error componenents and imaging geometry
      //---
      bool constructMatrix(const pqe_float64&       errBiasLOS,
                           const pqe_float64&       errRandLOS,
                           const pqe_float64&       elevAngleLOS,
                           const pqe_float64&       azimAngleLOS,
                           const pqe::ColumnVector3d& surfN,
                           const pqe::Matrix&      surfCovMat);

      //---
      // Method for forming 3X3 ENU covariance matrix from CE/LE
      //---
      bool formCovMatrixFromCE_LE(const pqe_float64&  CE,
                                  const pqe_float64&  LE,
                                  pqe::Matrix& covMat) const;
      
      //---
      // Method for computing 90% CE using interpolating function
      //---
      pqe_float64 compute90PCE() const;
      
      //---
      // Method for computing acquisition angles from RPC coefficients
      //---
      bool computeElevAzim(const pqe::pqeRPCModel rpc,
                           pqe_float64&  elevAngleLOS,
                           pqe_float64&  azimAngleLOS) const;
      
      //---
      // Methods for computing RPC polynomial and its derivatives
      //---
      pqe_float64 polynomial(const pqe_float64& nlat,
                             const pqe_float64& nlon,
                             const pqe_float64& nhgt,
                             const pqe_float64* coeffs) const;
      pqe_float64 dPoly_dLat(const pqe_float64& nlat,
                             const pqe_float64& nlon,
                             const pqe_float64& nhgt,
                             const pqe_float64* coeffs) const;
      pqe_float64 dPoly_dLon(const pqe_float64& nlat,
                             const pqe_float64& nlon,
                             const pqe_float64& nhgt,
                             const pqe_float64* coeffs) const;
      pqe_float64 dPoly_dHgt(const pqe_float64& nlat,
                             const pqe_float64& nlon,
                             const pqe_float64& nhgt,
                             const pqe_float64* coeffs) const;
      
      
      
      //---
      // Utility methods
      //---
      pqe::ColumnVector3d vperp(const pqe::ColumnVector3d& v1,
                                const pqe::ColumnVector3d& v2) const;
      
      pqe_float64 atan3(const pqe_float64 y, const pqe_float64 x) const;
      
   }; // End of: class PositionQualityEvaluator

} // End of: namespace pqe

#endif /* End of: #ifndef pqe_PositionQualityEvaluator_HEADER */
