//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Contains class declaration for locpt.  This uses geotrans
// local cartesian implementation.
//*******************************************************************
// $Id$

#include <pqe/Matrix4x4.h>
#include <pqe/newmatap.h>

pqe::Matrix4x4::Matrix4x4(const pqe::Matrix& m)
   :theData(4,4)
{
   if((m.Nrows() == 4) &&
      (m.Ncols() == 4))
   {
      theData = m;
   }
   else if((m.Nrows()==3)&&
           (m.Ncols()==3))
   {
      theData[0][0] = m[0][0];
      theData[0][1] = m[0][1];
      theData[0][2] = m[0][2];
      theData[0][3] = 0.0;
      theData[1][0] = m[1][0];
      theData[1][1] = m[1][1];
      theData[1][2] = m[1][2];
      theData[1][3] = 0.0;
      theData[2][0] = m[2][0];
      theData[2][1] = m[2][1];
      theData[2][2] = m[2][2];
      theData[2][3] = 0.0;
      theData[3][0] = 0.0;
      theData[3][1] = 0.0;
      theData[3][2] = 0.0;
      theData[3][3] = 1.0;
   }
   else
   {
      theData[0][0] = 1.0;
      theData[0][1] = 0.0;
      theData[0][2] = 0.0;
      theData[0][3] = 0.0;
      
      theData[1][0] = 0.0;
      theData[1][1] = 1.0;
      theData[1][2] = 0.0;
      theData[1][3] = 0.0;
      
      theData[2][0] = 0.0;
      theData[2][1] = 0.0;
      theData[2][2] = 1.0;
      theData[2][3] = 0.0;
      
      theData[3][0] = 0.0;
      theData[3][1] = 0.0;
      theData[3][2] = 0.0;
      theData[3][3] = 1.0;
   }
}

pqe::Matrix4x4::Matrix4x4()
  :theData(4, 4)
{
  theData[0][0] = 1.0;
  theData[0][1] = 0.0;
  theData[0][2] = 0.0;
  theData[0][3] = 0.0;

  theData[1][0] = 0.0;
  theData[1][1] = 1.0;
  theData[1][2] = 0.0;
  theData[1][3] = 0.0;

  theData[2][0] = 0.0;
  theData[2][1] = 0.0;
  theData[2][2] = 1.0;
  theData[2][3] = 0.0;

  theData[3][0] = 0.0;
  theData[3][1] = 0.0;
  theData[3][2] = 0.0;
  theData[3][3] = 1.0;
}

pqe::Matrix4x4::Matrix4x4(
   pqe_float64 v00, pqe_float64 v01, pqe_float64 v02, pqe_float64 v03,
   pqe_float64 v10, pqe_float64 v11, pqe_float64 v12, pqe_float64 v13,
   pqe_float64 v20, pqe_float64 v21, pqe_float64 v22, pqe_float64 v23,
   pqe_float64 v30, pqe_float64 v31, pqe_float64 v32, pqe_float64 v33)
  :theData(4, 4)
{
  theData[0][0] = v00;
  theData[0][1] = v01;
  theData[0][2] = v02;
  theData[0][3] = v03;

  theData[1][0] = v10;
  theData[1][1] = v11;
  theData[1][2] = v12;
  theData[1][3] = v13;

  theData[2][0] = v20;
  theData[2][1] = v21;
  theData[2][2] = v22;
  theData[2][3] = v23;

  theData[3][0] = v30;
  theData[3][1] = v31;
  theData[3][2] = v32;
  theData[3][3] = v33;
}

#define QX  q.theVector[0]
#define QY  q.theVector[1]
#define QZ  q.theVector[2]
#define QW  q.theVector[3]

void pqe::Matrix4x4::makeRotate( const pqe::Quaternion& quat)
{
   setIdentity();
   setRotate(quat);
}

void pqe::Matrix4x4::setRotate(const pqe::Quaternion& quat)
{
    pqe::Quaternion q(quat);
    pqe_float64 length2 = q.length2();
    if (length2!=1.0 && length2!=0)
    {
        // normalize quat if required.
        q /= sqrt(length2);
    }

    // Source: Gamasutra, Rotating Objects Using Quaternions
    //
    //http://www.gamasutra.com/features/19980703/quaternions_01.htm

    pqe_float64 wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

    // calculate coefficients
    x2 = QX + QX;
    y2 = QY + QY;
    z2 = QZ + QZ;

    xx = QX * x2;
    xy = QX * y2;
    xz = QX * z2;

    yy = QY * y2;
    yz = QY * z2;
    zz = QZ * z2;

    wx = QW * x2;
    wy = QW * y2;
    wz = QW * z2;

    theData[0][0] = 1.0 - (yy + zz);
    theData[0][1] = xy - wz;
    theData[0][2] = xz + wy;
    
    theData[1][0] = xy + wz;
    theData[1][1] = 1.0 - (xx + zz);
    theData[1][2] = yz - wx;

    theData[2][0] = xz - wy;
    theData[2][1] = yz + wx;
    theData[2][2] = 1.0 - (xx + yy);
}

pqe::Quaternion pqe::Matrix4x4::getRotate()const
{
   pqe::Quaternion quat;

   getRotate(quat);
   
   return quat;
}

void pqe::Matrix4x4::getRotate(pqe::Quaternion& q)const
{
   pqe_float64 tr, s;
   pqe_float64 tq[4];
   int    i, j, k;
   
   int nxt[3] = {1, 2, 0};
   
   tr = theData[0][0] + theData[1][1] + theData[2][2]+1.0;
   
   // check the diagonal
   if (tr > 0.0)
   {
      s = (pqe_float64)std::sqrt (tr);
      QW = s / 2.0;
      s = 0.5 / s;
      QX = (theData[2][1] - theData[1][2]) * s;
      QY = (theData[0][2] - theData[2][0]) * s;
      QZ = (theData[1][0] - theData[0][1]) * s;
   }
   else
   {
      // diagonal is negative
      i = 0;
      if (theData[1][1] > theData[0][0])
         i = 1;
      if (theData[2][2] > theData[i][i])
         i = 2;
      j = nxt[i];
      k = nxt[j];
      
      s = (pqe_float64)std::sqrt((theData[i][i] -
                                  (theData[j][j] + theData[k][k])) + 1.0);
      
      tq[i] = s * 0.5;
      
      if (s != 0.0)
         s = 0.5 / s;
      
      tq[3] = (theData[k][j] - theData[j][k]) * s;
      tq[j] = (theData[j][i] + theData[i][j]) * s;
      tq[k] = (theData[k][i] + theData[i][k]) * s;
      
      QX = tq[0];
      QY = tq[1];
      QZ = tq[2];
      QW = tq[3];
   }
}

pqe::ColumnVector3d pqe::Matrix4x4::getEigenValues()const
{
  pqe::DiagonalMatrix d;
  pqe::SymmetricMatrix s;

  s << theData;

  pqe::EigenValues(s, d);

  return pqe::ColumnVector3d(d[0], d[1], d[2]);
}

void pqe::Matrix4x4::setZero()
{
  theData[0][0] = 0.0;
  theData[0][1] = 0.0;
  theData[0][2] = 0.0;
  theData[0][3] = 0.0;

  theData[1][0] = 0.0;
  theData[1][1] = 0.0;
  theData[1][2] = 0.0;
  theData[1][3] = 0.0;

  theData[2][0] = 0.0;
  theData[2][1] = 0.0;
  theData[2][2] = 0.0;
  theData[2][3] = 0.0;

  theData[3][0] = 0.0;
  theData[3][1] = 0.0;
  theData[3][2] = 0.0;
  theData[3][3] = 0.0;
}

void pqe::Matrix4x4::setIdentity()
{
   setZero();
   theData[0][0] = 1.0;
   theData[1][1] = 1.0;
   theData[2][2] = 1.0;
   theData[3][3] = 1.0;
}

pqe::Matrix pqe::Matrix4x4::createIdentity()
{
  pqe::Matrix4x4 m(1.0, 0.0, 0.0, 0.0, 
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
  return m.getData();
}

pqe::Matrix pqe::Matrix4x4::createRotateOnly(const pqe::Matrix4x4 &aMatrix)
{
  pqe::Matrix4x4 m = aMatrix;

  m.theData[0][3] = 0;
  m.theData[1][3] = 0;
  m.theData[2][3] = 0;
  m.theData[3][3] = 1.0;

  m.theData[3][0] = 0.0;
  m.theData[3][1] = 0.0;
  m.theData[3][2] = 0.0;
  m.theData[3][3] = 1.0;

  return m.getData();
}

pqe::Matrix pqe::Matrix4x4::createZero()
{
  pqe::Matrix4x4 m;

  m.getData() = 0.0;

  return m.getData();
}

pqe::Matrix pqe::Matrix4x4::createTranslationMatrix(pqe_float64 x,
                                                    pqe_float64 y,
                                                    pqe_float64 z) 
{
   pqe::Matrix4x4 m(1.0, 0.0, 0.0, x,
                    0.0, 1.0, 0.0, y,
                    0.0, 0.0, 1.0, z,
                    0.0, 0.0, 0.0, 1.0);
   
   return m.getData();
}

pqe::Matrix pqe::Matrix4x4::createRotationMatrix(
   pqe_float64 angleX,
   pqe_float64 angleY,
   pqe_float64 angleZ,
   pqeCoordSysOrientMode orientationMode)
{
  return (createRotationZMatrix(angleZ,
                                orientationMode)*
	  createRotationYMatrix(angleY,
                                orientationMode)*
	  createRotationXMatrix(angleX,
                                orientationMode));
}

pqe::Matrix pqe::Matrix4x4::createRotationXMatrix(
   pqe_float64 angle, pqeCoordSysOrientMode orientationMode)
{
   pqe::Matrix m(4,4);
   
   pqe_float64 Cosine = cos(angle*RAD_PER_DEG);
   pqe_float64 Sine   = sin(angle*RAD_PER_DEG);
   
   if(orientationMode == PQE_RIGHT_HANDED)
   {
      m << 1.0 << 0.0 << 0.0 << 0.0
        << 0.0 << Cosine << Sine << 0.0
        << 0.0 << -Sine  << Cosine << 0.0
        << 0.0 << 0.0 << 0.0 << 1.0;
   }
   else
   {
      m << 1.0 << 0.0 << 0.0 << 0.0
        << 0.0 << Cosine << -Sine << 0.0
        << 0.0 << Sine  << Cosine << 0.0
        << 0.0 << 0.0 << 0.0 << 1.0;
   }
  
   return m;
}

pqe::Matrix pqe::Matrix4x4::createRotationYMatrix(
   pqe_float64 angle, pqeCoordSysOrientMode orientationMode)
{
   pqe::Matrix m(4,4);
   
   pqe_float64 Cosine = cos(angle*RAD_PER_DEG);
   pqe_float64 Sine   = sin(angle*RAD_PER_DEG);
   
   if(orientationMode == PQE_RIGHT_HANDED)
   {
      m << Cosine << 0.0 << -Sine  << 0.0
        << 0.0    << 1.0 << 0.0    << 0.0
        << Sine   << 0.0 << Cosine << 0.0
        << 0.0    << 0.0 << 0.0    << 1.0;
   }
   else
   {
      m << Cosine << 0.0 << Sine   << 0.0
        << 0.0    << 1.0 << 0.0    << 0.0
        << -Sine  << 0.0 << Cosine << 0.0
        << 0.0    << 0.0 << 0.0    << 1.0;
   }
   
  return m;
}


pqe::Matrix pqe::Matrix4x4::createRotationZMatrix(
   pqe_float64 angle, pqeCoordSysOrientMode orientationMode)
{
   pqe::Matrix m(4,4);
   
   pqe_float64 Cosine = cos(angle*RAD_PER_DEG);
   pqe_float64 Sine   = sin(angle*RAD_PER_DEG);
   
   if(orientationMode == PQE_RIGHT_HANDED)
   {
      m << Cosine << Sine   << 0.0 << 0.0
        << -Sine  << Cosine << 0.0 << 0.0
        << 0.0    << 0.0    << 1.0 << 0.0
        << 0.0    << 0.0    << 0.0 << 1.0;
   }
   else
   {
      m << Cosine << -Sine   << 0.0 << 0.0
        << Sine   << Cosine << 0.0 << 0.0
        << 0.0    << 0.0    << 1.0 << 0.0
        << 0.0    << 0.0    << 0.0 << 1.0;
   }
   
   return m;
}


pqe::Matrix pqe::Matrix4x4::createScaleMatrix(
   pqe_float64 x, pqe_float64 y, pqe_float64 z)
{
   pqe::Matrix m(4, 4);
   
      m << x   << 0.0 << 0.0 << 0.0
        << 0.0 << y   << 0.0 << 0.0
        << 0.0 << 0.0 << z   << 0.0
        << 0.0 << 0.0 << 0.0 << 1.0;

    return m;
}
