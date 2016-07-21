//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// ossimThreeParamDatum. This is a base class for all
// three param datums.
//*******************************************************************
// $Id$

#ifndef pqe_ThreeParamDatum_HEADER
#define pqe_ThreeParamDatum_HEADER 1

#include <pqe/Constants.h>
#include <pqe/Datum.h>

namespace pqe
{
   class PQEDLL ThreeParamDatum : public pqe::Datum
   {
   public:
      ThreeParamDatum(const std::string &code, const std::string &name,
                      const pqe::Ellipsoid* anEllipsoid,
                      pqe_float64 sigmaX,
                      pqe_float64 sigmaY,
                      pqe_float64 sigmaZ,
                      pqe_float64 westLongitude,
                      pqe_float64 eastLongitude,
                      pqe_float64 southLatitude,
                      pqe_float64 northLatitude,
                      pqe_float64 param1,
                      pqe_float64 param2,
                      pqe_float64 param3)
         : pqe::Datum(code, name, anEllipsoid,
                      sigmaX, sigmaY, sigmaZ,
                      westLongitude, eastLongitude,
                      southLatitude, northLatitude),
         theParam1(param1),
         theParam2(param2),
         theParam3(param3)
         {}

      virtual ~ThreeParamDatum(){}

      /**
       * Will shift a ground point to this datum.  If they are the same
       * datum then nothing happens.
       */
      virtual pqe::Gpt    shift(const pqe::Gpt    &aPt)const;
   
      virtual pqe::Gpt    shiftToWgs84(const pqe::Gpt &aPt)const;
      virtual pqe::Gpt    shiftFromWgs84(const pqe::Gpt &aPt)const;
      virtual pqe_float64 param1()const{return theParam1;}
      virtual pqe_float64 param2()const{return theParam2;}
      virtual pqe_float64 param3()const{return theParam3;}
      virtual pqe_float64 param4()const{return 0.0;}
      virtual pqe_float64 param5()const{return 0.0;}
      virtual pqe_float64 param6()const{return 0.0;}
      virtual pqe_float64 param7()const{return 1.0;}
      
   protected:
      
      pqe_float64 theParam1;
      pqe_float64 theParam2;
      pqe_float64 theParam3;
      
   }; // end of class ThreeParamDatum

} // end of namespace pqe

#endif /* end of #ifndef pqe_ThreeParamDatum_HEADER */
