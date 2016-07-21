//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Wgs84Datum.  Special hardcoded datum.  It will create a static
// instance of a Wgs84Ellipsoid and set the initial defaults for
// that are specific to a Wgs84Datum
//*******************************************************************
// $Id$

#include <pqe/Wgs84Datum.h>
#include <pqe/EllipsoidFactory.h>
#include <pqe/Ellipsoid.h>

pqe::Wgs84Datum::Wgs84Datum()
   : pqe::ThreeParamDatum("WGE",
                          "World Geodetic System 1984",
                          pqe::EllipsoidFactory::instance()->wgs84(),
                          0.0, 0.0, 0.0,
                          -M_PI/2.0, M_PI/2.0, -M_PI, M_PI,
                          0.0, 0.0, 0.0)
{
}

pqe::Gpt pqe::Wgs84Datum::shift(const pqe::Gpt &aPt)const
{
   const pqe::Datum *aDatum = aPt.datum();
   
    if( aPt.datum()->code() == code())
    {
       return pqe::Gpt(aPt.latd(), aPt.lond(), aPt.height(), this);
    }
   
    if(aDatum)
    {
       return aDatum->shiftToWgs84(aPt);
    }

   return aPt;      
}

pqe::Gpt pqe::Wgs84Datum::shiftToWgs84(const pqe::Gpt &aPt)const
{
   return pqe::Gpt(aPt.latd(),
                   aPt.lond(),
                   aPt.height(),
                   DatumFactory::instance()->wgs84());
}

pqe::Gpt pqe::Wgs84Datum::shiftFromWgs84(const pqe::Gpt &aPt)const
{
   return aPt;
}
