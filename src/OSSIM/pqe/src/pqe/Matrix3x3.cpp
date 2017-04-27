//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
// $Id$

#include <iostream>

#include <pqe/Matrix3x3.h>
#include <pqe/newmatap.h>
#include <pqe/ColumnVector3d.h>

pqe::Matrix pqe::Matrix3x3::create()
{
   pqe::Matrix m(3, 3);
   
   m[0][0] = 1.0;
   m[0][1] = 0.0;
   m[0][2] = 0.0;
   
   m[1][0] = 0.0;
   m[1][1] = 1.0;
   m[1][2] = 0.0;
   
   m[2][0] = 0.0;
   m[2][1] = 0.0;
   m[2][2] = 1.0;

   return m;
}

pqe::Matrix pqe::Matrix3x3::create(
   pqe_float64 v00, pqe_float64 v01, pqe_float64 v02,
   pqe_float64 v10, pqe_float64 v11, pqe_float64 v12,
   pqe_float64 v20, pqe_float64 v21, pqe_float64 v22)
{
   pqe::Matrix m(3, 3);
   
   m[0][0] = v00;
   m[0][1] = v01;
   m[0][2] = v02;
   
   m[1][0] = v10;
   m[1][1] = v11;
   m[1][2] = v12;
   
   m[2][0] = v20;
   m[2][1] = v21;
   m[2][2] = v22;

   return m;
}

pqe::Matrix pqe::Matrix3x3::create(const pqe::Matrix& rhs)
{
   pqe::Matrix m(3, 3);
   
   if (rhs.Ncols() != 3 || rhs.Nrows() != 3)
   {
      std::cerr
         << "pqe::Matrix3x3::create(const pqe::Matrix& rhs) ERROR:"
         << "\nMatrix passed to function not a 3x3!"
         << "\nnumber of columns:  " << rhs.Ncols()
         << "\nnumber of rows:     " << rhs.Nrows()
         << "\nReturn blank 3x3 matrix...\n";
      return m;
   }
   
   m[0][0] = rhs[0][0];
   m[0][1] = rhs[0][1];
   m[0][2] = rhs[0][2];
   m[0][3] = rhs[0][3];
   m[1][0] = rhs[1][0];
   m[1][1] = rhs[1][1];
   m[1][2] = rhs[1][2];
   m[1][3] = rhs[1][3];
   m[2][0] = rhs[2][0];
   m[2][1] = rhs[2][1];
   m[2][2] = rhs[2][2];
   m[2][3] = rhs[2][3];

   return m;
}

pqe::ColumnVector3d pqe::Matrix3x3::getEigenValues(const pqe::Matrix& matrix)
{
   if (matrix.Ncols() != 3 || matrix.Nrows() != 3)
   {
      std::cerr
         << "FATAL: pqe::ColumnVector3d operator*(const pqe::Matrix& lhs,"
         << "\nconst pqe::ColumnVector3d& rhs), "
         << "\nMatrix passed to function not a 3x3!"
         << "\nnumber of columns:  " << matrix.Ncols()
         << "\nnumber of rows:     " << matrix.Nrows()
         << "\nReturn blank pqe::ColumnVector3d...\n";
      return pqe::ColumnVector3d();
   }

   pqe::DiagonalMatrix d;
   pqe::SymmetricMatrix s;
   
   s << matrix;
   
   pqe::EigenValues(s, d);
   
   return pqe::ColumnVector3d(d[0], d[1], d[2]);
}

pqe::Matrix pqe::Matrix3x3::createIdentity()
{
   pqe::Matrix m(3,3);

   m[0][0] = 1.0;
   m[0][1] = 0.0;
   m[0][2] = 0.0;
   
   m[1][0] = 0.0;
   m[1][1] = 1.0;
   m[1][2] = 0.0;
   
   m[2][0] = 0.0;
   m[2][1] = 0.0;
   m[2][2] = 1.0;
   
   return m;
}

pqe::Matrix pqe::Matrix3x3::createZero()
{
  pqe::Matrix m(3,3);

  m = 0.0;

  return m;
}

pqe::Matrix pqe::Matrix3x3::createTranslationMatrix(pqe_float64 dx,pqe_float64 dy)
{
  pqe::Matrix m(3,3);

   m[0][0] = 1.0;
   m[0][1] = 0.0;
   m[0][2] = dx;
   
   m[1][0] = 0.0;
   m[1][1] = 1.0;
   m[1][2] = dy;
   
   m[2][0] = 0.0;
   m[2][1] = 0.0;
   m[2][2] = 1.0;

   return m;
}

pqe::Matrix pqe::Matrix3x3::createRotationMatrix(pqe_float64 angleX,
                                                 pqe_float64 angleY,
                                                 pqe_float64 angleZ,
                                                 pqeCoordSysOrientMode orientationMode)
{
   return (createRotationZMatrix(angleZ, orientationMode)*
           createRotationYMatrix(angleY, orientationMode)*
           createRotationXMatrix(angleX, orientationMode));
}

pqe::Matrix pqe::Matrix3x3::createRotationXMatrix(pqe_float64 angle,
                                                     pqeCoordSysOrientMode orientationMode)
{
  pqe::Matrix m(3,3);
  pqe_float64 Cosine = cos(angle*RAD_PER_DEG);
  pqe_float64 Sine   = sin(angle*RAD_PER_DEG);

  if(orientationMode == PQE_RIGHT_HANDED)
  {
     m << 1 << 0      << 0
       << 0 << Cosine << Sine
       << 0 << -Sine  << Cosine;
  }
  else
  {
     m << 1 << 0      << 0
       << 0 << Cosine << -Sine
       << 0 << Sine  << Cosine;
  }
  return m;
}

pqe::Matrix pqe::Matrix3x3::createRotationYMatrix(pqe_float64 angle,
                                                     pqeCoordSysOrientMode orientationMode)
{
   pqe::Matrix m(3,3);
   pqe_float64 Cosine = cos(angle*RAD_PER_DEG);
   pqe_float64 Sine   = sin(angle*RAD_PER_DEG);
   
  if(orientationMode == PQE_RIGHT_HANDED)
  {
     m[0][0] = Cosine;
     m[0][1] = 0.0;
     m[0][2] = -Sine;
     
     m[1][0] = 0.0;
     m[1][1] = 1.0;
     m[1][2] = 0.0;
     
     m[2][0] = Sine;
     m[2][1] = 0.0;
     m[2][2] = Cosine;
  }
  else
  {
     m[0][0] = Cosine;
     m[0][1] = 0.0;
     m[0][2] = Sine;
     
     m[1][0] = 0.0;
     m[1][1] = 1.0;
     m[1][2] = 0.0;
     
     m[2][0] = -Sine;
     m[2][1] = 0.0;
     m[2][2] = Cosine;
  }
  return m;
}


pqe::Matrix pqe::Matrix3x3::createRotationZMatrix(pqe_float64 angle,
                                                  pqeCoordSysOrientMode orientationMode)
{
   pqe::Matrix m(3,3);
   pqe_float64 Cosine = cos(angle*RAD_PER_DEG);
   pqe_float64 Sine   = sin(angle*RAD_PER_DEG);
   
  if(orientationMode == PQE_RIGHT_HANDED)
  {
     m[0][0] = Cosine;
     m[0][1] = Sine;
     m[0][2] = 0.0;
     
     m[1][0] = -Sine;
     m[1][1] = Cosine;
     m[1][2] = 0.0;
     
     m[2][0] = 0.0;
     m[2][1] = 0.0;
     m[2][2] = 1.0;
  }
  else
  {
     m[0][0] = Cosine;
     m[0][1] = -Sine;
     m[0][2] = 0.0;
     
     m[1][0] = Sine;
     m[1][1] = Cosine;
     m[1][2] = 0.0;
     
     m[2][0] = 0.0;
     m[2][1] = 0.0;
     m[2][2] = 1.0;
  }
  return m;
}

pqe::Matrix pqe::Matrix3x3::createScaleMatrix(pqe_float64 x, pqe_float64 y, pqe_float64 z)
{
    pqe::Matrix m = createIdentity();

    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;

    return m;
}

pqe::Matrix3x3::Matrix3x3(const pqe::Matrix3x3& /* rhs */)
{
}

const pqe::Matrix3x3& pqe::Matrix3x3::operator=(const pqe::Matrix3x3& rhs)
{
   return rhs;
}


