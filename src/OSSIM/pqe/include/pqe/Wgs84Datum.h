//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// ossimWgs84Datum.  Special hardcoded datum.  It will create a static
// instance of a Wgs84Ellipsoid and set the initial defaults for
// that are specific to a ossimWgs84Datum
//*******************************************************************
//  $Id$

#ifndef pqe_Wgs84Datum_HEADER
#define pqe_Wgs84Datum_HEADER 1

#include <pqe/Constants.h>
#include <pqe/ThreeParamDatum.h>
#include <pqe/Gpt.h>

namespace pqe
{
   class PQEDLL Wgs84Datum : public pqe::ThreeParamDatum
   {
   public:
      Wgs84Datum();
      virtual ~Wgs84Datum(){}
      
      virtual pqe::Gpt shift(const pqe::Gpt& aPt)const;
      virtual pqe::Gpt shiftToWgs84(const pqe::Gpt& aPt)const;
      virtual pqe::Gpt shiftFromWgs84(const pqe::Gpt& aPt)const;
      
   }; // End of class Wgs84Datum

} // End of namespace pqe

#endif /* End of #ifndef pqe_Wgs84Datum_HEADER */
