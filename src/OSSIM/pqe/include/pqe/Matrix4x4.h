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

#ifndef pqe_Matrix4x4_HEADER
#define pqe_Matrix4x4_HEADER

#include <iosfwd>

#include <pqe/Constants.h>
#include <pqe/newmat.h>
#include <pqe/newmatio.h>
#include <pqe/ColumnVector4d.h>
#include <pqe/ColumnVector3d.h>
#include <pqe/Quaternion.h>

namespace pqe
{
   class PQEDLL Matrix4x4
   {
   public:
      friend std::ostream& operator<<(std::ostream& out,
                                      const pqe::Matrix4x4& data)
      {
         return out << data.getData();
      }
      Matrix4x4();
      Matrix4x4(const pqe::Matrix& m);
      Matrix4x4(pqe_float64 v00, pqe_float64 v01, pqe_float64 v02, pqe_float64 v03,
                pqe_float64 v10, pqe_float64 v11, pqe_float64 v12, pqe_float64 v13,
                pqe_float64 v20, pqe_float64 v21, pqe_float64 v22, pqe_float64 v23,
                pqe_float64 v30, pqe_float64 v31, pqe_float64 v32, pqe_float64 v33);
      inline explicit Matrix4x4(const pqe::Quaternion& quat){ makeRotate(quat);}
      Matrix4x4(const pqe::Matrix4x4& rhs)
         :theData(4,4)
      {
         theData[0][0] = rhs.theData[0][0];
         theData[0][1] = rhs.theData[0][1];
         theData[0][2] = rhs.theData[0][2];
         theData[0][3] = rhs.theData[0][3];
         theData[1][0] = rhs.theData[1][0];
         theData[1][1] = rhs.theData[1][1];
         theData[1][2] = rhs.theData[1][2];
         theData[1][3] = rhs.theData[1][3];
         theData[2][0] = rhs.theData[2][0];
         theData[2][1] = rhs.theData[2][1];
         theData[2][2] = rhs.theData[2][2];
         theData[2][3] = rhs.theData[2][3];
         theData[3][0] = rhs.theData[3][0];
         theData[3][1] = rhs.theData[3][1];
         theData[3][2] = rhs.theData[3][2];
         theData[3][3] = rhs.theData[3][3];
      }

      void makeRotate( const pqe::Quaternion& quat);
      void setRotate( const pqe::Quaternion& quat);
      pqe::Quaternion getRotate()const;
      void getRotate(pqe::Quaternion& quat)const;
         
      pqe::Matrix4x4 operator+ (const pqe::Matrix4x4& rhs)const
      {
         return pqe::Matrix4x4(theData[0][0] + rhs.theData[0][0], theData[0][1] + rhs.theData[0][1], theData[0][2] + rhs.theData[0][2], theData[0][3] + rhs.theData[0][3],
                               theData[1][0] + rhs.theData[1][0], theData[1][1] + rhs.theData[1][1], theData[1][2] + rhs.theData[1][2], theData[1][3] + rhs.theData[1][3],
                               theData[2][0] + rhs.theData[2][0], theData[2][1] + rhs.theData[2][1], theData[2][2] + rhs.theData[2][2], theData[2][3] + rhs.theData[2][3],
                               theData[3][0] + rhs.theData[3][0], theData[3][1] + rhs.theData[3][1], theData[3][2] + rhs.theData[3][2], theData[3][3] + rhs.theData[3][3]);
      }

      pqe::Matrix4x4 operator- (const pqe::Matrix4x4& rhs)const
      {
         return pqe::Matrix4x4(theData[0][0] - rhs.theData[0][0], theData[0][1] - rhs.theData[0][1], theData[0][2] - rhs.theData[0][2], theData[0][3] - rhs.theData[0][3],
                               theData[1][0] - rhs.theData[1][0], theData[1][1] - rhs.theData[1][1], theData[1][2] - rhs.theData[1][2], theData[1][3] - rhs.theData[1][3],
                               theData[2][0] - rhs.theData[2][0], theData[2][1] - rhs.theData[2][1], theData[2][2] - rhs.theData[2][2], theData[2][3] - rhs.theData[2][3],
                               theData[3][0] - rhs.theData[3][0], theData[3][1] - rhs.theData[3][1], theData[3][2] - rhs.theData[3][2], theData[3][3] - rhs.theData[3][3]);
      }
  
      friend pqe::ColumnVector4d operator * (const pqe::Matrix4x4& lhs, const pqe::ColumnVector4d& rhs)
      {
         return pqe::ColumnVector4d( (lhs.theData[0][0]*rhs[0] + lhs.theData[0][1]*rhs[1] + lhs.theData[0][2]*rhs[2] + lhs.theData[0][3]*rhs[3]),
                                     (lhs.theData[1][0]*rhs[0] + lhs.theData[1][1]*rhs[1] + lhs.theData[1][2]*rhs[2] + lhs.theData[1][3]*rhs[3]),
                                     (lhs.theData[2][0]*rhs[0] + lhs.theData[2][1]*rhs[1] + lhs.theData[2][2]*rhs[2] + lhs.theData[2][3]*rhs[3]),
                                     (lhs.theData[3][0]*rhs[0] + lhs.theData[3][1]*rhs[1] + lhs.theData[3][2]*rhs[2] + lhs.theData[3][3]*rhs[3]));
         
      }
      
      friend pqe::ColumnVector3d operator * (const pqe::Matrix4x4& lhs, const pqe::ColumnVector3d& rhs)
      {
         // assume the 4 coordinate (homogeneous coord) of the 3d vector is 1
         return pqe::ColumnVector3d( (lhs.theData[0][0]*rhs[0] + lhs.theData[0][1]*rhs[1] + lhs.theData[0][2]*rhs[2] + lhs.theData[0][3]),
                                     (lhs.theData[1][0]*rhs[0] + lhs.theData[1][1]*rhs[1] + lhs.theData[1][2]*rhs[2] + lhs.theData[1][3]),
                                     (lhs.theData[2][0]*rhs[0] + lhs.theData[2][1]*rhs[1] + lhs.theData[2][2]*rhs[2] + lhs.theData[2][3]));         
      }
      
      pqe::ColumnVector4d rotateOnly(const pqe::ColumnVector4d& rhs)const
      {
         // multiply only by the 3x3 submatrix.
         return pqe::ColumnVector4d( (theData[0][0]*rhs[0] + theData[0][1]*rhs[1] + theData[0][2]*rhs[2]),
                                     (theData[1][0]*rhs[0] + theData[1][1]*rhs[1] + theData[1][2]*rhs[2]),
                                     (theData[2][0]*rhs[0] + theData[2][1]*rhs[1] + theData[2][2]*rhs[2]));         
      }
      
      pqe::ColumnVector3d rotateOnly(const pqe::ColumnVector3d& rhs)const
      {
         // assume the 4 coordinate (homogeneous coord) of the 3d vector is 1
         return pqe::ColumnVector3d( (theData[0][0]*rhs[0] + theData[0][1]*rhs[1] + theData[0][2]*rhs[2]),
                                     (theData[1][0]*rhs[0] + theData[1][1]*rhs[1] + theData[1][2]*rhs[2]),
                                     (theData[2][0]*rhs[0] + theData[2][1]*rhs[1] + theData[2][2]*rhs[2]));
      }
      
      pqe::Matrix4x4 operator*(pqe_float64 scalar)const
      {
         return pqe::Matrix4x4(theData[0][0]*scalar, theData[0][1]*scalar, theData[0][2]*scalar, theData[0][3]*scalar,
                               theData[1][0]*scalar, theData[1][1]*scalar, theData[1][2]*scalar, theData[1][3]*scalar,
                               theData[2][0]*scalar, theData[2][1]*scalar, theData[2][2]*scalar, theData[2][3]*scalar,
                               theData[0][0]*scalar, theData[0][1]*scalar, theData[0][2]*scalar, theData[0][3]*scalar);
         
      }
      
      pqe::Matrix4x4 operator*(const pqe::Matrix4x4& rhs)const
      {
         pqe::Matrix4x4 m;
         
         // element 0,0 is first row time first column
         m.theData[0][0] = (theData[0][0]*rhs.theData[0][0] +
                            theData[0][1]*rhs.theData[1][0] +
                         theData[0][2]*rhs.theData[2][0] +
                         theData[0][3]*rhs.theData[3][0]);
         
         // element 0, 1 is first row second column
         m.theData[0][1] = (theData[0][0]*rhs.theData[0][1] +
                         theData[0][1]*rhs.theData[1][1] +
                         theData[0][2]*rhs.theData[2][1] +
                         theData[0][3]*rhs.theData[3][1]);
         
         // element 0, 2 is first row third column
         m.theData[0][2] = (theData[0][0]*rhs.theData[0][2] +
                         theData[0][1]*rhs.theData[1][2] +
                         theData[0][2]*rhs.theData[2][2] +
                         theData[0][3]*rhs.theData[3][2]);
         
         // element 0, 3 is first row fourth column
         m.theData[0][3] = (theData[0][0]*rhs.theData[0][3] +
                         theData[0][1]*rhs.theData[1][3] +
                         theData[0][2]*rhs.theData[2][3] +
                         theData[0][3]*rhs.theData[3][3]);
         
         // element 1, 0 is second row first column
         m.theData[1][0] = (theData[1][0]*rhs.theData[0][0] +
                         theData[1][1]*rhs.theData[1][0] +
                         theData[1][2]*rhs.theData[2][0] +
                         theData[1][3]*rhs.theData[3][0]);
         
         // element 1, 1 is second row second column
         m.theData[1][1] = (theData[1][0]*rhs.theData[0][1] +
                         theData[1][1]*rhs.theData[1][1] +
                         theData[1][2]*rhs.theData[2][1] +
                         theData[1][3]*rhs.theData[3][1]);
         
         // element 1, 2 is second row third column
         m.theData[1][2] = (theData[1][0]*rhs.theData[0][2] +
                         theData[1][1]*rhs.theData[1][2] +
                         theData[1][2]*rhs.theData[2][2] +
                         theData[1][3]*rhs.theData[3][2]);
         
         // element 1, 3 is second row fourth column
         m.theData[1][3] = (theData[1][0]*rhs.theData[0][3] +
                         theData[1][1]*rhs.theData[1][3] +
                         theData[1][2]*rhs.theData[2][3] +
                         theData[1][3]*rhs.theData[3][3]);
         
         // element 2, 0 is third row first column
         m.theData[2][0] = (theData[2][0]*rhs.theData[0][0] +
                         theData[2][1]*rhs.theData[1][0] +
                         theData[2][2]*rhs.theData[2][0] +
                         theData[2][3]*rhs.theData[3][0]);
         
         // element 2, 1 is third row second column
         m.theData[2][1] = (theData[2][0]*rhs.theData[0][1] +
                         theData[2][1]*rhs.theData[1][1] +
                         theData[2][2]*rhs.theData[2][1] +
                         theData[2][3]*rhs.theData[3][1]);
         
         // element 2, 2 is third row third column
         m.theData[2][2] = (theData[2][0]*rhs.theData[0][2] +
                         theData[2][1]*rhs.theData[1][2] +
                         theData[2][2]*rhs.theData[2][2] +
                         theData[2][3]*rhs.theData[3][2]);
         
         // element 2, 3 is third row fourth column
         m.theData[2][3] = (theData[2][0]*rhs.theData[0][3] +
                         theData[2][1]*rhs.theData[1][3] +
                         theData[2][2]*rhs.theData[2][3] +
                         theData[2][3]*rhs.theData[3][3]);
         
         // element 3, 0 is fourth row first column
         m.theData[3][0] = (theData[3][0]*rhs.theData[0][0] +
                         theData[3][1]*rhs.theData[1][0] +
                         theData[3][2]*rhs.theData[2][0] +
                         theData[3][3]*rhs.theData[3][0]);
         
         // element 3, 1 is fourth row second column
         m.theData[3][1] = (theData[3][0]*rhs.theData[0][1] +
                         theData[3][1]*rhs.theData[1][1] +
                         theData[3][2]*rhs.theData[2][1] +
                         theData[3][3]*rhs.theData[3][1]);
         
         // element 3, 2 is fourth row third column
         m.theData[3][2] = (theData[3][0]*rhs.theData[0][2] +
                         theData[3][1]*rhs.theData[1][2] +
                         theData[3][2]*rhs.theData[2][2] +
                         theData[3][3]*rhs.theData[3][2]);
         
         // element 3, 3 is fourth row fourth column
         m.theData[3][3] = (theData[3][0]*rhs.theData[0][3] +
                         theData[3][1]*rhs.theData[1][3] +
                         theData[3][2]*rhs.theData[2][3] +
                         theData[3][3]*rhs.theData[3][3]);
         
         return m;
      }

      /*!
       * uses the matrix package to compute the inverse
       */
      pqe::Matrix4x4& i()
      {
         theData = theData.i();
         return *this;
      }
      
      /*!
       * Uses the matrix package to compute the transpose.
       */
      pqe::Matrix4x4& t()
      {        
         theData = theData.t();
         return *this;
      }
      
      /*!
       * uses the matrix package to compute the determinant
       */
      pqe_float64 getDeterminant()const
      {
         return theData.Determinant();
      }
   
      /*!
       * Uses the matrix package to compute the eigenvalues for this
       * matrix
       */ 
      pqe::ColumnVector3d getEigenValues()const;
      
      void setZero();
      void setIdentity();
      static pqe::Matrix createIdentity();
      static pqe::Matrix createRotateOnly(const pqe::Matrix4x4& aMatrix);
      
      static pqe::Matrix createZero();
      static pqe::Matrix createTranslationMatrix(pqe_float64 x,
                                                 pqe_float64 y,
                                                 pqe_float64 z);
      static pqe::Matrix createRotationMatrix(pqe_float64 angleX,
                                              pqe_float64 angleY,
                                              pqe_float64 angleZ,
                                              pqeCoordSysOrientMode orientationMode=PQE_RIGHT_HANDED);
      static pqe::Matrix createRotationXMatrix(pqe_float64 angle,
                                               pqeCoordSysOrientMode orientationMode=PQE_RIGHT_HANDED);
      static pqe::Matrix createRotationYMatrix(pqe_float64 angle,
                                               pqeCoordSysOrientMode orientationMode=PQE_RIGHT_HANDED);
      static pqe::Matrix createRotationZMatrix(pqe_float64 angle,
                                               pqeCoordSysOrientMode orientationMode=PQE_RIGHT_HANDED);
      
      static pqe::Matrix createScaleMatrix(pqe_float64 X, pqe_float64 Y, pqe_float64 Z);
      
      const pqe::Matrix& getData()const{return theData;}
      pqe::Matrix& getData(){return theData;}
      
   private:
      pqe::Matrix theData;
      
   }; // End of class Matrix4x4

} // End of namespace pqe

#endif /* End of #ifndef pqe_Matrix4x4_HEADER. */
