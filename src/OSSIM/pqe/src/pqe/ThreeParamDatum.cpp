//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//*******************************************************************
//  $Id$

#include <cmath>

#include <pqe/ThreeParamDatum.h>
#include <pqe/Constants.h>

#include <pqe/Ellipsoid.h>
#include <pqe/Gpt.h>
#include <pqe/EcefPoint.h>
#include <pqe/DatumFactory.h>
#include <pqe/Datum.h>

pqe::Gpt pqe::ThreeParamDatum::shiftToWgs84(const pqe::Gpt &aPt)const
{
   pqe::EcefPoint p1 = aPt;
   pqe::EcefPoint p2;
 
   if(withinMolodenskyRange(aPt.latd()))
   {
      const pqe::Datum* wgs84 = DatumFactory::instance()->wgs84();
      pqe_float64 latin, lonin, hgtin;
      pqe_float64 latout, lonout, hgtout;
      
      pqe_float64 da = wgs84->ellipsoid()->getA() - ellipsoid()->getA();
      pqe_float64 df = wgs84->ellipsoid()->getFlattening() - ellipsoid()->getFlattening();

      latin = aPt.latr();
      lonin = aPt.lonr();
      hgtin = aPt.height();

      if(aPt.isHgtNan())
      {
         hgtin = 0.0;
      }
      molodenskyShift(ellipsoid()->getA(), da, ellipsoid()->getFlattening(), df, param1(), param2(), param3(),
                      latin, lonin, hgtin,
                      latout, lonout, hgtout);
      pqe::Gpt g;

      g.latr(latout);
      g.lonr(lonout);
      g.height(hgtout);
      g.datum(this);
      return g;
   }
   else
     {
       p2 = pqe::EcefPoint(p1.x() + theParam1,
			   p1.y() + theParam2,
			   p1.z() + theParam3);
     }
   
   return pqe::Gpt(p2); // defaults to WGS84
}

pqe::Gpt pqe::ThreeParamDatum::shiftFromWgs84(const pqe::Gpt &aPt)const
{
   pqe::EcefPoint p1=aPt;
   pqe::EcefPoint p2=aPt;
   
   if(withinMolodenskyRange(aPt.latd()))
   {
      const pqe::Datum* wgs84 = DatumFactory::instance()->wgs84();
      pqe_float64 latin, lonin, hgtin;
      pqe_float64 latout, lonout, hgtout;
      
      pqe_float64 da = ellipsoid()->getA() -  wgs84->ellipsoid()->getA();
      pqe_float64 df = ellipsoid()->getFlattening() - wgs84->ellipsoid()->getFlattening();

      latin = aPt.latr();
      lonin = aPt.lonr();
      hgtin = aPt.height();

      if(aPt.isHgtNan())
      {
         hgtin = 0.0;
      }
      molodenskyShift(wgs84->ellipsoid()->getA(), da, wgs84->ellipsoid()->getFlattening(), df, -param1(), -param2(), -param3(),
                      latin, lonin, hgtin,
                      latout, lonout, hgtout);
      pqe::Gpt g;

      g.latr(latout);
      g.lonr(lonout);
      g.height(hgtout);
      g.datum(this);
      return g;
   }
   else
   {
      p2 = pqe::EcefPoint(p1.x() - theParam1,
                          p1.y() - theParam2,
                          p1.z() - theParam3);
   }
   
   
   return pqe::Gpt(p2, this);
}


pqe::Gpt pqe::ThreeParamDatum::shift(const pqe::Gpt &aPt)const
{
   const pqe::Datum *aDatum = aPt.datum();

   if( code() == aDatum->code())
   {
      return pqe::Gpt(aPt.latd(), aPt.lond(), aPt.height(), this);
   }
   
   if(aDatum)
   {
      return shiftFromWgs84(aDatum->shiftToWgs84(aPt));
   }

   return aPt;
}
