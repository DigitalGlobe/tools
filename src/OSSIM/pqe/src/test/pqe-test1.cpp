//-----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Simple test application for PositionQualityEvaluator class.
//
//-----------------------------------------------------------------------------
// $Id$

#include <iostream>
using namespace std;

#include <pqe/Constants.h>
#include <pqe/PositionQualityEvaluator.h>
#include <pqe/EcefPoint.h>
#include <pqe/Gpt.h>
#include <pqe/Matrix3x3.h>
#include <pqe/ColumnVector3d.h>


int main(int argc, char* argv[])
{
   pqe::Gpt gpt(28.598864426198883,
                -80.681869456196623,
                -30.58);

   pqe::EcefPoint ecefPt(gpt);
   
   pqe_float64 errBiasLOS = 4.78;
   pqe_float64 rrRandLOS  = 0.5;
   pqe_float64 elevAngleLOS = 63.81 * RAD_PER_DEG;
   pqe_float64 azimAngleLOS = 161.31 * RAD_PER_DEG;
   pqe::ColumnVector3d surfN(-0.00405, -0.00324, 0.99481);
   
   pqe::Matrix m3x3In = pqe::Matrix3x3::create(
      86.856146, 0.0,       0.0,
      0.0,       86.856146, 0.0,
      0.0,       0.0,       36.959067);

   pqe::PositionQualityEvaluator* pe =
      new pqe::PositionQualityEvaluator(ecefPt,
                                        errBiasLOS,
                                        rrRandLOS,
                                        elevAngleLOS,
                                        azimAngleLOS,
                                        surfN,
                                        m3x3In);

   pqe::Matrix m3x3Out = pqe::Matrix3x3::create();

   pe->getCovMatrix(m3x3Out);

   pqe_float64 ce = 0.0;
   pqe_float64 le = 0.0;

   pe->computeCE_LE(pqe::ONE_SIGMA, ce, le);

   cout << "RPC test:"
        << "\nInput gpt: " << gpt
        << "\nInput 3x3:\n" << m3x3In
        << "\nP68 CE: " << ce
        << "\nP68 LE: " << le
        << "\nOutput matrix:\n" << m3x3Out
        << endl;
   
   
   pqe::Matrix m3x3In2 = pqe::Matrix3x3::create(
      24.671839,   -4.503825,   0.0,
      -4.503825,   35.985047,   0.0,
      0.0,         0.0,       36.897057);
   
   pqe::PositionQualityEvaluator* pe2 =
      new pqe::PositionQualityEvaluator(ecefPt, m3x3In2);
   
   pqe::Matrix m3x3Out2 = pqe::Matrix3x3::create();
   
   pe2->getCovMatrix(m3x3Out2);
   
   pqe::pqeErrorEllipse ellipse;
   
   pe2->extractErrorEllipse(pqe::ONE_SIGMA, ellipse);

   pe2->computeCE_LE(pqe::ONE_SIGMA, ce, le);

   cout << "Covariance matrix test:"
        << "\nInput gpt: " << gpt
        << "\nInput 3x3:\n" << m3x3In2
        << "\nOutput matrix:\n" << m3x3Out2
        << "\nP68 CE: " << ce
        << "\nP68 LE: " << le
        << "\nellipse: " << ellipse.theSemiMajorAxis
        << " " << ellipse.theSemiMinorAxis
        <<"\n [m, 1 sigma] at "
        << ellipse.theAzimAngle*DEG_PER_RAD
        << endl;
   
   delete pe;
   delete pe2;
   
   return 0;
}
