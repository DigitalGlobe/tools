#ifndef omsWktUtility_HEADER
#define omsWktUtility_HEADER
#include <oms/Constants.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimConstants.h>
namespace oms 
{
   class OMSDLL WktUtility
   {
   public:
      WktUtility();
      
      /**
       * this will convert a center radius to a wkt polygon.
       *
       * @param center Center point of the circle produced.  The point
       *               is in WGS84 lat lon.
       * @param radius The radial distance for the new points.
       * @param radialUnit The radial units of the radius argument.
       * @param numberOfSamples The number of samples used to generate the circle/Polygon.
       * @param directinalSign  The direction you ant the polygon points to go.  -1 is counter
       *                        clockwise and 1 is clockwise.
       */
      std::string toWktGeometryGivenCenterRadius(const ossimGpt& center, 
                                                double radius,
                                                ossimUnitType radialUnit,
                                                unsigned int numberOfSamples,
                                                int          directionalSign)const;
   };
}

#endif
