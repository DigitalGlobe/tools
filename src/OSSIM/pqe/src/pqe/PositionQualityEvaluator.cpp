//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Base class for position quality evaluation operations.
//
//----------------------------------------------------------------------------
// $Id$

#include <iostream>
#include <iomanip>

#include <pqe/PositionQualityEvaluator.h>
#include <pqe/Datum.h>
#include <pqe/Ellipsoid.h>
#include <pqe/LsrSpace.h>

static const bool PQE_TRACE = false;


// 2D 90% function coefficients
const pqe_uint32 nTableEntries = 21;
const pqe_uint32 nMultiplier = nTableEntries-1;
const pqe_float64 table90[nTableEntries]=
  {1.644854,1.645623,1.647912,1.651786,1.657313,
   1.664580,1.673829,1.685227,1.699183,1.716257,
   1.737080,1.762122,1.791522,1.825112,1.862530,
   1.903349,1.947158,1.993595,2.042360,2.093214,2.145966};
   
// 1D conversion factors relative to ONE_SIGMA
const pqe_float64 Fac1D[pqe::NUM_PROB_LEVELS] =
   {1.0, 0.6745, 1.6449, 1.96};
   
// 2D conversion factors relative to ONE_SIGMA
const pqe_float64 Fac2D[pqe::NUM_PROB_LEVELS] =
   {1.0, 1.1774, 2.1460, 2.4477};
   
// 2D conversion factors relative to P90
const pqe_float64 Fac2D90[pqe::NUM_PROB_LEVELS] =
   {0.46598, 0.54865, 1.0, 1.14059};


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::PositionQualityEvaluator()
//  
//  Constructor.
//  
//*****************************************************************************
pqe::PositionQualityEvaluator::PositionQualityEvaluator()
{
   if (PQE_TRACE)
   {
      std::cout
         << "\nPositionQualityEvaluator::PositionQualityEvaluator DEBUG:"
         << std::endl;
   }
   
   theEvaluatorValid = false;
   theRpcModel.theType = 'N';
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::PositionQualityEvaluator()
//  
//  Covariance matrix-based constructor.
//  
//*****************************************************************************
pqe::PositionQualityEvaluator::
PositionQualityEvaluator(const pqe::EcefPoint& pt,const pqe::Matrix& covMat)
{
   if (PQE_TRACE)
   {
      std::cout
         << "\nPositionQualityEvaluator::PositionQualityEvaluator DEBUG:"
         << std::endl;
   }
   theRpcModel.theType = 'N';

   // Set the point
   pqe::Gpt ptG(pt);
   thePtG = ptG;
   
   // Define the local frame centered on the point
   pqe::LsrSpace enu(ptG);
   theLocalFrame = enu;
   
   // Propagate input ECF cov matrix to local
   theCovMat = theLocalFrame.ecefToLsrRotMatrix()*covMat*
               theLocalFrame.lsrToEcefRotMatrix();

   // Compute evaluation parameters
   theEvaluatorValid = decomposeMatrix();
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::PositionQualityEvaluator()
//  
//  LOS error/geometry-based constructor.
//  
//*****************************************************************************
pqe::PositionQualityEvaluator::PositionQualityEvaluator(
   const pqe::EcefPoint&      pt,
   const pqe_float64&       errBiasLOS,
   const pqe_float64&       errRandLOS,
   const pqe_float64&       elevAngleLOS,
   const pqe_float64&       azimAngleLOS,
   const pqe::ColumnVector3d& surfN,
   const pqe::Matrix&      surfCovMat)
{
   if (PQE_TRACE)
   {
      std::cout
         << "\nPositionQualityEvaluator::PositionQualityEvaluator DEBUG:"
         << std::endl;
   }

   theRpcModel.theType = 'N';

   // Set the point
   pqe::Gpt ptG(pt);
   thePtG = ptG;
    
   // Define the local frame centered on the point
   pqe::LsrSpace enu(ptG);
   theLocalFrame = enu;

   // Form the covariance matrix
   if (constructMatrix
      (errBiasLOS, errRandLOS, elevAngleLOS, azimAngleLOS, surfN, surfCovMat))
   {
      // Compute evaluation parameters
      theEvaluatorValid = decomposeMatrix();
   }
   else
   {
      theEvaluatorValid = false;
   }
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::PositionQualityEvaluator()
//  
//  LOS error/coefficient-based constructor.
//  
//*****************************************************************************
pqe::PositionQualityEvaluator::PositionQualityEvaluator(
   const pqe::EcefPoint&      pt,
   const pqe_float64&       errBiasLOS,
   const pqe_float64&       errRandLOS,
   const pqe::pqeRPCModel&         rpc,
   const pqe::ColumnVector3d& surfN,
   const pqe::Matrix&      surfCovMat)
{
   if (PQE_TRACE)
   {
      std::cout
         << "\nPositionQualityEvaluator::PositionQualityEvaluator DEBUG:"
         << std::endl;
   }

   // Set the point
   pqe::Gpt ptG(pt);
   thePtG = ptG;
    
   // Define the local frame centered on the point
   pqe::LsrSpace enu(ptG);
   theLocalFrame = enu;
   
   // Set the model parameters
   theRpcModel = rpc;
   
   // Compute the target elevation & azimuth angles
   pqe_float64 elevAngleLOS;
   pqe_float64 azimAngleLOS;
   computeElevAzim(rpc, elevAngleLOS, azimAngleLOS);
   
   // Form the covariance matrix
   if (constructMatrix
      (errBiasLOS, errRandLOS, elevAngleLOS, azimAngleLOS, surfN, surfCovMat))
   {
      // Compute evaluation parameters
      theEvaluatorValid = decomposeMatrix();
   }
   else
   {
      theEvaluatorValid = false;
   }
}


//*****************************************************************************
//  DESTRUCTOR: ~PositionQualityEvaluator()
//  
//*****************************************************************************
pqe::PositionQualityEvaluator::~PositionQualityEvaluator()
{
   if (PQE_TRACE)  std::cout
      << "DEBUG: ~PositionQualityEvaluator(): returning..." << std::endl;
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::print()
//  
//  Print info.
//  
//*****************************************************************************
std::ostream& pqe::PositionQualityEvaluator::print(std::ostream& out) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   out << "\nPositionQualityEvaluator Summary...\n"
       << " theEvaluatorValid: ";
   if (theEvaluatorValid)
   {
      out<<"True\n";
   }
   else
   {
      out<<"False\n";
   }
   out << " thePtG: "<<thePtG
       << "\n theCovMat [m]:\n"<<theCovMat
       << std::setiosflags(std::ios_base::fixed) << std::setprecision(1)
       << "\ntheEllipse: "<<theEllipse.theSemiMajorAxis<<"  "
       <<theEllipse.theSemiMinorAxis
       <<"\n [m, 1 sigma] at "
       << theEllipse.theAzimAngle*DEG_PER_RAD<<" [deg] azimuth" << std::endl;

   // Reset flags.
   out.setf(f);

   return out;
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::getCovMatrix()
//  
//  Access the covariance matrix.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::getCovMatrix(pqe::Matrix& covMat) const
{
   if (theEvaluatorValid)
   {
      covMat = theCovMat;
   }

   return theEvaluatorValid;
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::addContributingCovariance()
//  
//  Sum in a contributing covariance matrix.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::addContributingCovariance(
   pqe::Matrix& covMat)
{
   bool matrixOK = (covMat.Nrows()==3) && (covMat.Nrows()==3);
   if (theEvaluatorValid && matrixOK)
   {
      // Add contribution
      theCovMat += covMat;
   
      // Update the ellipse parameters
      theEvaluatorValid = decomposeMatrix();
   }

   return (theEvaluatorValid && matrixOK);
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::addContributingCE_LE()
//  
//  Sum in a contributing CE/LE.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::addContributingCE_LE(
   const pqe_float64& cCE, const pqe_float64& cLE)
{
   pqe::Matrix covMat(3,3);
   
   formCovMatrixFromCE_LE(cCE, cLE, covMat);

   return addContributingCovariance(covMat);
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::subtractContributingCovariance()
//  
//  Subtract out a contributing covariance matrix.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::subtractContributingCovariance(
   pqe::Matrix& covMat)
{
   bool matrixOK = (covMat.Nrows()==3) && (covMat.Nrows()==3);
   if (theEvaluatorValid && matrixOK)
   {
      // Subtract contribution
      theCovMat -= covMat;
   
      // Update the ellipse parameters
      theEvaluatorValid = decomposeMatrix();
   }

   return (theEvaluatorValid && matrixOK);
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::subtractContributingCE_LE()
//  
//  Subtract out a contributing CE/LE.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::subtractContributingCE_LE(
   const pqe_float64& cCE, const pqe_float64& cLE)
{
   pqe::Matrix covMat(3,3);
   
   formCovMatrixFromCE_LE(cCE, cLE, covMat);

   return subtractContributingCovariance(covMat);
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::computeCE_LE()
//  
//  Compute CE/LE (ft) @ pLev probability level.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::computeCE_LE(
   const pqe::pqeProbLev_t pLev, pqe_float64& CE, pqe_float64& LE) const
{
   if (theEvaluatorValid)
   {
      // Compute 1D LE
      LE = sqrt(theCovMat(3,3)) * (pqe_float64)Fac1D[pLev];

      // Compute 2D CE
      CE = (pqe_float64)Fac2D90[pLev] * compute90PCE();
   }
   
   return theEvaluatorValid;
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::extractErrorEllipse()
//  
//  Extract error ellipse parameters @ pLev probability level.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::extractErrorEllipse(
   const pqe::pqeProbLev_t pLev, pqe::pqeErrorEllipse& ellipse)
{
   if (theEvaluatorValid)
   {
      // Scale the axes
      ellipse.theSemiMinorAxis =
         (pqe_float64)Fac2D[pLev] * theEllipse.theSemiMinorAxis;
      ellipse.theSemiMajorAxis =
         (pqe_float64)Fac2D[pLev] * theEllipse.theSemiMajorAxis;
      
      // Orientation angle
      ellipse.theAzimAngle = theEllipse.theAzimAngle;
      
      // Center position
      ellipse.theCenter = thePtG;
   }

   return theEvaluatorValid;
}


//*****************************************************************************
//  METHOD: pqe::PositionQualityEvaluator::extractErrorEllipse()
//  
//  Extract error ellipse parameters @ pLev probability level.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::extractErrorEllipse(
   const pqe::pqeProbLev_t     pLev,
   const pqe_float64&   angularIncrement,
   pqe::pqeErrorEllipse& ellipse,
   pqeImageErrorEllipse_t& ellImage)
{
   bool computeImageEllipse = true;
   
   if (theRpcModel.theType == 'N')
      computeImageEllipse = false;
      
   if (theEvaluatorValid && computeImageEllipse)
   {
      // Get object space ellipse parameters
      extractErrorEllipse(pLev, ellipse);
      
      //***
      // Generate the image space ellipse point at 'angularIncrement' spacing
      //***
      int numSteps = 360/(int)angularIncrement;

      // Semi-axes components
      pqe_float64 dxMaj = ellipse.theSemiMajorAxis*sin(ellipse.theAzimAngle);
      pqe_float64 dyMaj = ellipse.theSemiMajorAxis*cos(ellipse.theAzimAngle);
      pqe_float64 dxMin = ellipse.theSemiMinorAxis*sin(ellipse.theAzimAngle+M_PI/2.0);
      pqe_float64 dyMin = ellipse.theSemiMinorAxis*cos(ellipse.theAzimAngle+M_PI/2.0);
      
      for (int j = 0; j<=numSteps; ++j)
      {
      
         // Compute current ENU ellipse point
         pqe_float64 ang = TWO_PI*j/numSteps;
         pqe_float64 sang = sin(ang);
         pqe_float64 cang = cos(ang);
         pqe_float64 x = dxMaj*cang + dxMin*sang;
         pqe_float64 y = dyMaj*cang + dyMin*sang;
         pqe_float64 z = 0.0;
         
         pqe::LsrPoint cpLSR(x, y, z, theLocalFrame);
         pqe::EcefPoint cp = pqe::EcefPoint(cpLSR);
         pqe::Gpt cpG(cp);
         pqe_float64 lat = cpG.latd();
         pqe_float64 lon = cpG.lond();
         pqe_float64 hgt = cpG.height();
      
         // Normalize the lat, lon, hgt:
         pqe_float64 nlat = (lat - theRpcModel.theLatOffset) /
                        theRpcModel.theLatScale;
         pqe_float64 nlon = (lon - theRpcModel.theLonOffset) /
                        theRpcModel.theLonScale;
         pqe_float64 nhgt;

         if( pqe::isnan(hgt) )
         {
            nhgt = (theRpcModel.theHgtScale - theRpcModel.theHgtOffset) /
                    theRpcModel.theHgtScale;
         }
         else
         {
            nhgt = (hgt - theRpcModel.theHgtOffset) / theRpcModel.theHgtScale;
         }

         //---
         // Compute the normalized line (Un) and sample (Vn)
         //---
         pqe_float64 Pu = polynomial(nlat, nlon, nhgt, theRpcModel.theLineNumCoef);
         pqe_float64 Qu = polynomial(nlat, nlon, nhgt, theRpcModel.theLineDenCoef);
         pqe_float64 Pv = polynomial(nlat, nlon, nhgt, theRpcModel.theSampNumCoef);
         pqe_float64 Qv = polynomial(nlat, nlon, nhgt, theRpcModel.theSampDenCoef);
         pqe_float64 Un  = Pu / Qu;
         pqe_float64 Vn  = Pv / Qv;

         //---
         // Compute the actual line (U) and sample (V):
         //---
         pqe_float64 U  = Un*theRpcModel.theLineScale + theRpcModel.theLineOffset;
         pqe_float64 V  = Vn*theRpcModel.theSampScale + theRpcModel.theSampOffset;

         pqe::Dpt img(V,U);
         ellImage.push_back(img);
      
      }
   }

   return (theEvaluatorValid && computeImageEllipse);
}

//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::decomposeMatrix()
//  
//  Perform eigenvector decomposition and extract ellipse parameters.
//  Compute eigenvalues (D) of horizontal 2X2 sub-matrix
//  Note: eigenvectors (columns of V) contain unit vectors
//        defining orientation of pMin/pMax error ellipse axes
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::decomposeMatrix()
{
      // Decompose upper left 2X2 partition
      pqe::SymmetricMatrix S(2);
      S<<theCovMat(1,1)<<theCovMat(1,2)<<theCovMat(2,2);
      pqe::DiagonalMatrix D;
      pqe::Matrix V;
      pqe::Jacobi(S,D,V);
      theEllipse.theSemiMinorAxis = sqrt(D(1,1));
      theEllipse.theSemiMajorAxis = sqrt(D(2,2));
      theEigenvectors = V;

      // Compute error ellipse orientation
      //    (ccw rotation of major axis from x-axis)
      pqe_float64 sin2theta = 2.0*theCovMat(1,2);
      pqe_float64 cos2theta = theCovMat(1,1)-theCovMat(2,2);
      if (cos2theta == 0.0)
      {
         return false;
      }
      else
      {
         // Convert "ccw from x-axis" to "cw from y-axis(N)"
         pqe_float64 rotAngle = atan3(sin2theta, cos2theta)/2.0;
         theEllipse.theAzimAngle = M_PI/2.0 - rotAngle;
         if (theEllipse.theAzimAngle < 0.0)
            theEllipse.theAzimAngle += TWO_PI;
      }

      return true;
}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::constructMatrix()
//  
//  Construct covariance matrix from LOS-centered error components
//  and target acxquistion geometry.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::constructMatrix(
   const pqe_float64&       errBiasLOS,
   const pqe_float64&       errRandLOS,
   const pqe_float64&       elevAngleLOS,
   const pqe_float64&       azimAngleLOS,
   const pqe::ColumnVector3d& surfN,
   const pqe::Matrix&      surfCovMat)
{
   bool constructOK = true;
   pqe::ColumnVector3d lsrNorm(0.0,0.0,1.0);
   
   // Set the total error
   pqe_float64 eTot = sqrt(errBiasLOS*errBiasLOS + errRandLOS*errRandLOS);
   if (eTot == 0.0)
      eTot = 0.001;
   
   // Set the LOS unit vector
   pqe_float64 elC = elevAngleLOS;
   pqe_float64 azC = azimAngleLOS;   
   pqe::ColumnVector3d  LOS(sin(azC)*cos(elC), cos(azC)*cos(elC), sin(elC));

   if (PQE_TRACE)
   {
      std::cout
         <<"DEBUG: constructMatrix..."
         << "\n  tEl,tAz: " << elC*DEG_PER_RAD<<"  "<< azC*DEG_PER_RAD
         << "\n  LOS:     " << LOS
         << "\n  eTot:    " << eTot << std::endl;
   }
   
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Set ENU-referenced terrain slope normal
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   pqe::ColumnVector3d tSlopeN = surfN.unit();
   if (PQE_TRACE)
   {
      std::cout <<"  tSlopeN: " << tSlopeN << std::endl;
   }
   
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Compute normal to plane containing LOS and terrain normal
   //   this is direction of minor axis unless geometry causes swap
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   pqe::ColumnVector3d pMinU(0,1,0);
   pqe::ColumnVector3d pMinAxis = LOS.cross(tSlopeN);
   if (pMinAxis.magnitude() > DBL_EPSILON)
   {
      pMinU = pMinAxis.unit();
   }
   
   if (PQE_TRACE)
   {
      std::cout << "  pMinU: " << pMinU << std::endl;
   }
   
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Compute nominal major axis direction from cross 
   // product of minor axis vector and terrain slope normal
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   pqe::ColumnVector3d pMaxU = (tSlopeN.cross(pMinU)).unit();
   if (PQE_TRACE)
   {
      std::cout << "  pMaxU: "<< pMaxU << std::endl;
   }
   
   // Compute elevation angle relative to terrain plane and check for positive
   pqe_float64 elevAngTerr = acos(pMaxU.dot(LOS));
   if (PQE_TRACE)
   {
      std::cout << "  elev angle rel to surface: "
                << elevAngTerr*DEG_PER_RAD << std::endl;
   }
   
   if (elevAngTerr > 0.0)
   {
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Compute the LOS error the surface plane  
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      pqe_float64 planeErrorL = eTot/sin(elevAngTerr);
      
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Project axes to horizontal  
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
      pqe::ColumnVector3d pL = vperp(pMaxU, lsrNorm);
      pqe::ColumnVector3d pN = vperp(pMinU, lsrNorm);
      
      pqe::ColumnVector3d eL = pL * planeErrorL;
      pqe::ColumnVector3d eN = pN * eTot;
      pqe_float64 magL = eL.magnitude();
      pqe_float64 magN = eN.magnitude();

      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Compute vertical component due to intersection geometry 
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      pqe::ColumnVector3d pV = pMaxU - pL;
      pqe::ColumnVector3d eV = pV * planeErrorL;
      pqe_float64 magV = eV.magnitude();
      
      if (PQE_TRACE)
      {
         std::cout
         << "  Projected horizontal/vertical components...."
         << "\n   pL: " << pL << "  magL: " << magL
         << "\n   pN: " << pN << "  magN: " << magN
         << "\n   pV: " << pV << "  magV: " << magV << std::endl;
      }

      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Now compute the contributions of the surface uncertainty
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

      // Terrain slope angle
      pqe_float64 tSlope = acos(tSlopeN.dot(lsrNorm));

      // Vertical & horizontal components from surface covariance matrix
      pqe_float64 surfSigV = sqrt(surfCovMat(3,3));
      pqe_float64 surfSigH = sqrt((surfCovMat(1,1)+surfCovMat(2,2))/2.0);
      pqe_float64 surfSigV_h = surfSigH * tan(tSlope);

      // Effective total vertical error component includes
      // horizontal uncertainty induced by local slope
      pqe_float64 vSigTot = sqrt(surfSigV*surfSigV + surfSigV_h*surfSigV_h);
      
      // Project to surface normal direction to bring it into the L-surfN plane
      pqe::ColumnVector3d s_surfN = (lsrNorm.dot(tSlopeN))*tSlopeN;
      if (PQE_TRACE)
      {
         std::cout << " Surface uncertainty...."
                   << "\n  tSlope angle: "<< tSlope*DEG_PER_RAD
                   << "\n  s_surfN(unit): "<< s_surfN << std::endl;
      }
      
      s_surfN = s_surfN * vSigTot;
      pqe_float64 sigTn = s_surfN.magnitude();
      
      // Compute corresponding error in LOS direction
      pqe_float64 sigVl = sigTn/sin(elevAngTerr);

      // Resolve total vertical error to components based on intersection
      // geometry
      pqe::ColumnVector3d vSigHvec = sigVl * vperp(LOS, lsrNorm);
      pqe::ColumnVector3d vSigVvec = sigVl * (LOS.dot(lsrNorm))*lsrNorm;
      pqe_float64 vSigH = vSigHvec.magnitude();
      pqe_float64 vSigV = vSigVvec.magnitude();

      if (PQE_TRACE)
      {
         std::cout
            << " s_surfN: " << s_surfN
            << "\n vSigHvec: " << vSigHvec
            << "\n vSigVvec: " << vSigVvec
            << "\n----------------------------"
            << "\n surfSigH: "<<surfSigH
            << "\n surfSigV: "<<surfSigV
            << "\n  vSigTot: "<<vSigTot
            << "\n    vSigH: "<<vSigH
            << "\n    vSigV: "<<vSigV
            << "\n----------------------------" << std::endl;
      }

      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Add vSigH contribution (vSigH in L-surfN plane)
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      magL = sqrt(magL*magL + vSigH*vSigH);

      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Compute the axes magnitudes & rotation angle
      //    These parameters represent the horizontal error
      //    due to acquisition geometry & terrain slope
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      pqe_float64 theta;
      pqe_float64 pMax, pMin;
      if (magL > magN)
      {
         pMax = magL;
         pMin = magN;
         theta = atan3(pL[1],pL[0]);
      }
      else
      {
         pMax = magN;
         pMin = magL;
         theta = atan3(pN[1],pN[0]);
      }


      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Form final covariance matrix from axes & rotation angle
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      pqe::Matrix Cov(2,2);
      pqe::Matrix Vcomp(2,2);
      pqe::DiagonalMatrix Dcomp(2);
      
      Dcomp(1,1) = pMax*pMax;
      Dcomp(2,2) = pMin*pMin;
      Vcomp(1,1) = cos(theta);
      Vcomp(2,1) = sin(theta);
      Vcomp(1,2) =-Vcomp(2,1);
      Vcomp(2,2) = Vcomp(1,1);
      Cov = Vcomp*Dcomp*Vcomp.t();

      // Load full 3X3 matrix in local frame
      pqe::Matrix covMat(3,3);
      covMat(1,1) = Cov(1,1);   
      covMat(1,2) = Cov(1,2);   
      covMat(1,3) = 0.0;   
      covMat(2,1) = Cov(2,1);
      covMat(2,2) = Cov(2,2);   
      covMat(2,3) = 0.0;
      covMat(3,1) = covMat(1,3);
      covMat(3,2) = covMat(2,3);
      covMat(3,3) = magV*magV + vSigV*vSigV;

      // Save the matrix in local frame
      theCovMat = covMat;
      
   }  //end if (elevAngTerr > 0.0)
   else
   {
      constructOK = false;
      std::cout
         << "WARNING: pqe::PositionQualityEvaluator::constructMatrix(): "
         << "\n   terrain-referenced elev ang: "<<elevAngTerr*DEG_PER_RAD
         << std::endl;
   }

   return constructOK;
}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::formCovMatrixFromCE_LE()
//  
//  Form 3X3 ENU covariance matrix from CE/LE.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::formCovMatrixFromCE_LE(
   const pqe_float64&  CE,
   const pqe_float64&  LE,
   pqe::Matrix& covMat) const
{         
   covMat = 0.0;
   covMat(1,1) = CE/2.146;
   covMat(2,2) = CE/2.146;
   covMat(3,3) = LE/1.6449;
   covMat(1,1) *= covMat(1,1);
   covMat(2,2) *= covMat(2,2);
   covMat(3,3) *= covMat(3,3);

   return true;
}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::compute90PCE()
//  
//  Compute CE @ 90% probability level.
//  
//*****************************************************************************
pqe_float64 pqe::PositionQualityEvaluator::compute90PCE() const
{         
   // Evaluate CE function via linear interpolation
   pqe_float64 pRatio = theEllipse.theSemiMinorAxis/
                          theEllipse.theSemiMajorAxis;
   pqe_uint32 ndx = int(floor(pRatio*nMultiplier));
   pqe_float64 alpha;

   if (ndx == nMultiplier)
   {
      alpha = table90[ndx];
   }
   else
   {
      pqe_float64 fac =
         (pRatio-(pqe_float64)ndx/(pqe_float64)nMultiplier) / 0.05;
      alpha = fac * (table90[ndx+1]-table90[ndx]) + table90[ndx];
   }
              
   pqe_float64 CE90 = alpha * theEllipse.theSemiMajorAxis;
   
   return CE90;
}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::computeElevAzim()
//  
//  Compute elevation and azimuth angles from RPC coefficients.
//  
//*****************************************************************************
bool pqe::PositionQualityEvaluator::computeElevAzim(
   const pqe::pqeRPCModel rpc,
   pqe_float64&  elevAngleLOS,
   pqe_float64&  azimAngleLOS) const
{
   //---
   // Normalize the lat, lon, hgt:
   //---
   pqe_float64 nlat = (thePtG.lat - rpc.theLatOffset) / rpc.theLatScale;
   pqe_float64 nlon = (thePtG.lon - rpc.theLonOffset) / rpc.theLonScale;
   pqe_float64 nhgt;

   if( pqe::isnan(thePtG.hgt) )
   {
      nhgt = (rpc.theHgtScale - rpc.theHgtOffset) / rpc.theHgtScale;
   }
   else
   {
      nhgt = (thePtG.hgt - rpc.theHgtOffset) / rpc.theHgtScale;
   }
   
   //---
   // Compute the numerators & denominators
   //---
   pqe_float64 Pu = polynomial(nlat, nlon, nhgt, rpc.theLineNumCoef);
   pqe_float64 Qu = polynomial(nlat, nlon, nhgt, rpc.theLineDenCoef);
   pqe_float64 Pv = polynomial(nlat, nlon, nhgt, rpc.theSampNumCoef);
   pqe_float64 Qv = polynomial(nlat, nlon, nhgt, rpc.theSampDenCoef);

   //---
   // Compute the partials of each polynomial wrt lat, lon, hgt
   //---
   pqe_float64 dPu_dLat = dPoly_dLat(nlat, nlon, nhgt, rpc.theLineNumCoef);
   pqe_float64 dQu_dLat = dPoly_dLat(nlat, nlon, nhgt, rpc.theLineDenCoef);
   pqe_float64 dPv_dLat = dPoly_dLat(nlat, nlon, nhgt, rpc.theSampNumCoef);
   pqe_float64 dQv_dLat = dPoly_dLat(nlat, nlon, nhgt, rpc.theSampDenCoef);
   pqe_float64 dPu_dLon = dPoly_dLon(nlat, nlon, nhgt, rpc.theLineNumCoef);
   pqe_float64 dQu_dLon = dPoly_dLon(nlat, nlon, nhgt, rpc.theLineDenCoef);
   pqe_float64 dPv_dLon = dPoly_dLon(nlat, nlon, nhgt, rpc.theSampNumCoef);
   pqe_float64 dQv_dLon = dPoly_dLon(nlat, nlon, nhgt, rpc.theSampDenCoef);
   pqe_float64 dPu_dHgt = dPoly_dHgt(nlat, nlon, nhgt, rpc.theLineNumCoef);
   pqe_float64 dQu_dHgt = dPoly_dHgt(nlat, nlon, nhgt, rpc.theLineDenCoef);
   pqe_float64 dPv_dHgt = dPoly_dHgt(nlat, nlon, nhgt, rpc.theSampNumCoef);
   pqe_float64 dQv_dHgt = dPoly_dHgt(nlat, nlon, nhgt, rpc.theSampDenCoef);

   //---
   // Compute partials of quotients U and V wrt lat, lon, hgt 
   //---
   pqe_float64 dU_dLat = (Qu*dPu_dLat - Pu*dQu_dLat)/(Qu*Qu);
   pqe_float64 dU_dLon = (Qu*dPu_dLon - Pu*dQu_dLon)/(Qu*Qu);
   pqe_float64 dU_dHgt = (Qu*dPu_dHgt - Pu*dQu_dHgt)/(Qu*Qu);
   pqe_float64 dV_dLat = (Qv*dPv_dLat - Pv*dQv_dLat)/(Qv*Qv);
   pqe_float64 dV_dLon = (Qv*dPv_dLon - Pv*dQv_dLon)/(Qv*Qv);
   pqe_float64 dV_dHgt = (Qv*dPv_dHgt - Pv*dQv_dHgt)/(Qv*Qv);
   
    //---
    // Apply necessary scale factors 
    //---
   dU_dLat *= rpc.theLineScale/rpc.theLatScale;
   dU_dLon *= rpc.theLineScale/rpc.theLonScale;
   dU_dHgt *= rpc.theLineScale/rpc.theHgtScale;
   dV_dLat *= rpc.theSampScale/rpc.theLatScale;
   dV_dLon *= rpc.theSampScale/rpc.theLonScale;
   dV_dHgt *= rpc.theSampScale/rpc.theHgtScale;

   dU_dLat *= DEG_PER_RAD;
   dU_dLon *= DEG_PER_RAD;
   dV_dLat *= DEG_PER_RAD;
   dV_dLon *= DEG_PER_RAD;
   
   // Save the partials referenced to ECF
   pqe::EcefPoint location(thePtG);
   pqe::Matrix jMat(3,3);
   thePtG.datum()->ellipsoid()->jacobianWrtEcef(location, jMat);
   pqe::Dpt pWRTx;
   pqe::Dpt pWRTy;
   pqe::Dpt pWRTz;
   //  Line
   pWRTx.u = dU_dLat*jMat(1,1)+dU_dLon*jMat(2,1)+dU_dHgt*jMat(3,1);
   pWRTy.u = dU_dLat*jMat(1,2)+dU_dLon*jMat(2,2)+dU_dHgt*jMat(3,2);
   pWRTz.u = dU_dLat*jMat(1,3)+dU_dLon*jMat(2,3)+dU_dHgt*jMat(3,3);
   //  Samp
   pWRTx.v = dV_dLat*jMat(1,1)+dV_dLon*jMat(2,1)+dV_dHgt*jMat(3,1);
   pWRTy.v = dV_dLat*jMat(1,2)+dV_dLon*jMat(2,2)+dV_dHgt*jMat(3,2);
   pWRTz.v = dV_dLat*jMat(1,3)+dV_dLon*jMat(2,3)+dV_dHgt*jMat(3,3);

   // Form required partials in local frame
   pqe::Matrix jECF(3,2);
   jECF(1,1) = pWRTx.u;
   jECF(1,2) = pWRTx.v;
   jECF(2,1) = pWRTy.u;
   jECF(2,2) = pWRTy.v;
   jECF(3,1) = pWRTz.u;
   jECF(3,2) = pWRTz.v;
   pqe::Matrix jLSR(3,2);
   jLSR = theLocalFrame.ecefToLsrRotMatrix()*jECF;
   pqe_float64 dU_dx = jLSR(1,1);
   pqe_float64 dU_dy = jLSR(2,1);
   pqe_float64 dU_dz = jLSR(3,1);
   pqe_float64 dV_dx = jLSR(1,2);
   pqe_float64 dV_dy = jLSR(2,2);
   pqe_float64 dV_dz = jLSR(3,2);
   
   // Compute azimuth & elevation angles
   pqe_float64 den = dU_dy*dV_dx - dV_dy*dU_dx;
   pqe_float64 dY  = dU_dx*dV_dz - dV_dx*dU_dz;
   pqe_float64 dX  = dV_dy*dU_dz - dU_dy*dV_dz;
   pqe_float64 dy_dH = dY / den;
   pqe_float64 dx_dH = dX / den;
   
   azimAngleLOS = atan2(dx_dH, dy_dH);
   elevAngleLOS = atan2(1.0, sqrt(dy_dH*dy_dH+dx_dH*dx_dH));

   if (PQE_TRACE)
   {
      std::cout << "DEBUG: computeElevAzim..."
                << " el,az = " << elevAngleLOS*DEG_PER_RAD << " "
                << azimAngleLOS*DEG_PER_RAD << std::endl;
   }

   return true;
}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::vperp()
//  
//  Perpendicular vector component.
//  
//*****************************************************************************
pqe::ColumnVector3d pqe::PositionQualityEvaluator::vperp(
   const pqe::ColumnVector3d& v1, const pqe::ColumnVector3d& v2) const
{
   
   pqe_float64 scale = v1.dot(v2)/v2.dot(v2);
   pqe::ColumnVector3d v = v2*scale;
   
   pqe::ColumnVector3d p = v1 - v;
   
   return p;
}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::atan3()
//
//  arctan 0-360 counter-clockwise from x-axis
//  
//*****************************************************************************
pqe_float64 pqe::PositionQualityEvaluator::atan3(const pqe_float64  y,
                                            const pqe_float64  x) const
{
   pqe_float64 u,v,pih=0.5*M_PI,result;

   if (x == 0.0)
      result = M_PI - pih * pqe::sgn(y);
   else
   {
      if (y == 0.0)
      {
         if (x > 0.0)
            result = 0.0;
         else
            result = M_PI;
      }
      else
      {
         u = y/x;
         v = fabs(u);
         result = atan(v);
         result *= v/u;
         if (x < 0.0)
            result += M_PI;
         else
            if (result < 0.0)
               result += TWO_PI;
      }
   }
   
   return result;

}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::polynomial
//  
//  Evaluates polynomial function.
//  
//*****************************************************************************
pqe_float64 pqe::PositionQualityEvaluator::polynomial(
   const pqe_float64& P,
   const pqe_float64& L,
   const pqe_float64& H,
   const pqe_float64* c) const
{
   pqe_float64 r;

   if (theRpcModel.theType == 'A')
   {
      r = c[ 0]       + c[ 1]*L     + c[ 2]*P     + c[ 3]*H     +
          c[ 4]*L*P   + c[ 5]*L*H   + c[ 6]*P*H   + c[ 7]*L*P*H +
          c[ 8]*L*L   + c[ 9]*P*P   + c[10]*H*H   + c[11]*L*L*L +
          c[12]*L*L*P + c[13]*L*L*H + c[14]*L*P*P + c[15]*P*P*P +
          c[16]*P*P*H + c[17]*L*H*H + c[18]*P*H*H + c[19]*H*H*H;
   }
   else
   {
      r = c[ 0]       + c[ 1]*L     + c[ 2]*P     + c[ 3]*H     +
          c[ 4]*L*P   + c[ 5]*L*H   + c[ 6]*P*H   + c[ 7]*L*L   +
          c[ 8]*P*P   + c[ 9]*H*H   + c[10]*L*P*H + c[11]*L*L*L +
          c[12]*L*P*P + c[13]*L*H*H + c[14]*L*L*P + c[15]*P*P*P +
          c[16]*P*H*H + c[17]*L*L*H + c[18]*P*P*H + c[19]*H*H*H;
   }
   
   return r;
}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::dPoly_dLat
//  
//  Computes derivative of polynomial WRT normalized latitude P.
//  
//*****************************************************************************
pqe_float64 pqe::PositionQualityEvaluator::dPoly_dLat(
   const pqe_float64& P,
   const pqe_float64& L,
   const pqe_float64& H,
   const pqe_float64* c) const
{
   pqe_float64 dr;

   if (theRpcModel.theType == 'A')
   {
      dr = c[2] + c[4]*L + c[6]*H + c[7]*L*H + 2*c[9]*P + c[12]*L*L +
           2*c[14]*L*P + 3*c[15]*P*P +2*c[16]*P*H + c[18]*H*H;
   }
   else
   {
      dr = c[2] + c[4]*L + c[6]*H + 2*c[8]*P + c[10]*L*H + 2*c[12]*L*P +
           c[14]*L*L + 3*c[15]*P*P + c[16]*H*H + 2*c[18]*P*H;
   }
   
   return dr;
}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::dPoly_dLon
//  
//  Computes derivative of polynomial WRT normalized longitude L.
//  
//*****************************************************************************
pqe_float64 pqe::PositionQualityEvaluator::dPoly_dLon(
   const pqe_float64& P,
   const pqe_float64& L,
   const pqe_float64& H,
   const pqe_float64* c) const
{
   pqe_float64 dr;

   if (theRpcModel.theType == 'A')
   {
      dr = c[1] + c[4]*P + c[5]*H + c[7]*P*H + 2*c[8]*L + 3*c[11]*L*L +
           2*c[12]*L*P + 2*c[13]*L*H + c[14]*P*P + c[17]*H*H;
   }
   else
   {
      dr = c[1] + c[4]*P + c[5]*H + 2*c[7]*L + c[10]*P*H + 3*c[11]*L*L +
           c[12]*P*P + c[13]*H*H + 2*c[14]*P*L + 2*c[17]*L*H;
   }
   return dr;
}


//*****************************************************************************
//  PRIVATE METHOD: pqe::PositionQualityEvaluator::dPoly_dHgt
//  
//  Computes derivative of polynomial WRT normalized height H.
//  
//*****************************************************************************
pqe_float64 pqe::PositionQualityEvaluator::dPoly_dHgt(
   const pqe_float64& P,
   const pqe_float64& L,
   const pqe_float64& H,
   const pqe_float64* c) const
{
   pqe_float64 dr;

   if (theRpcModel.theType == 'A')
   {
      dr = c[3] + c[5]*L + c[6]*P + c[7]*L*P + 2*c[10]*H + c[13]*L*L +
           c[16]*P*P + 2*c[17]*L*H + 2*c[18]*P*H + 3*c[19]*H*H;
   }
   else
   {
      dr = c[3] + c[5]*L + c[6]*P + 2*c[9]*H + c[10]*L*P + 2*c[13]*L*H +
           2*c[16]*P*H + c[17]*L*L + c[18]*P*P + 3*c[19]*H*H;
   }
   return dr;
}
