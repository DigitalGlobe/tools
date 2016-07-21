//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Helper class for using NEWMAT::Matrix's.
//
//*******************************************************************
// $Id$

#ifndef pqe_Matrix3x3_HEADER
#define pqe_Matrix3x3_HEADER 1

#include <pqe/Constants.h>
#include <pqe/Common.h>
#include <pqe/newmat.h>
#include <pqe/newmatio.h>


namespace pqe
{
   class ColumnVector3d;
   
   class PQEDLL Matrix3x3
   {
   public:
      
      /*!
       *  Uses the matrix package to compute the eigenvalues for this
       *  matrix
       */ 
      pqe::ColumnVector3d getEigenValues(const pqe::Matrix& rhs);

      /*!
       *  Returns a blank 3x3 matrix.
       */
      static pqe::Matrix create();
      
      /*!
       *  Returns a copy of "m".
       *  Note:
       *  If "m" is not a 3x3 that's an error and a blank matrix is returned.
       */
      static pqe::Matrix create(const pqe::Matrix& m);
      
      /*!
       *  Create a 3x3 with assigned values.
       */
      static pqe::Matrix create(pqe_float64 v00, pqe_float64 v01, pqe_float64 v02,
                                pqe_float64 v10, pqe_float64 v11, pqe_float64 v12,
                                pqe_float64 v20, pqe_float64 v21, pqe_float64 v22);
      
      static pqe::Matrix createIdentity();
      
      static pqe::Matrix createZero();
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
      
      static pqe::Matrix createTranslationMatrix(pqe_float64 dx,pqe_float64 dy);
      
   protected:
      Matrix3x3(){}  // To squash un-warranted compiler warnings...
      
   private:
      Matrix3x3(const pqe::Matrix3x3& rhs);
      const pqe::Matrix3x3& operator=(const pqe::Matrix3x3& rhs);
      
   }; // End of: class Matrix3x3
   
} // End of: namespace pqe

#endif /* End of: #ifndef pqe_Matrix3x3_HEADER */
