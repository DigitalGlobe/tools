#include <oms/WktUtility.h>
#include <ossim/base/ossimUnitConversionTool.h>
oms::WktUtility::WktUtility()
{
   
}

std::string oms::WktUtility::toWktGeometryGivenCenterRadius(const ossimGpt& center, 
                                                            double radius,
                                                            ossimUnitType radialUnit,
                                                            unsigned int numberOfSamples,
                                                            int          directionalSign)const
{
   std::string result = "";
   if(ossim::almostEqual(radius, 0.0))
   {
      result = "POINT(" + ossimString::toString(center.lond()).string() + " " + ossimString::toString(center.latd()).string() + ")";
   }
   else 
   {
      double headingStepSizeInDegrees = (360.0/static_cast<double>(numberOfSamples))*
      directionalSign;
      ossim_uint32 step = 0;
      double degreesDistance = ossimUnitConversionTool(radius, radialUnit).getValue(OSSIM_DEGREES);
      ossimDpt centerGpt(center);
      ossimDpt newPoint;
      ossimDpt firstPoint;
      if(numberOfSamples > 0)
      {
         result = "POLYGON((";
      }
      for(step = 0; step < numberOfSamples; ++step)
      {
         double x = ossim::degreesToRadians(headingStepSizeInDegrees*step);// - 90.0));
         double dy = sin(x);
         double dx = cos(x);
         newPoint.lat = dy*degreesDistance;
         newPoint.lon = dx*degreesDistance;
         newPoint = centerGpt + newPoint;
         if(step == 0)
         {
            firstPoint = newPoint;
         }
         result += (ossimString::toString(ossim::clamp(newPoint.lon, -180.0, 180.0)).string()+" "
                    +ossimString::toString(ossim::clamp(newPoint.lat, -90.0, 90.0)).string()+",");
      }
      if(numberOfSamples > 0)
      {
         result += (ossimString::toString(firstPoint.lon ).string() + " " +
                    ossimString::toString(firstPoint.lat ).string() );
         result += "))";
      }
   }

   return result;
}
