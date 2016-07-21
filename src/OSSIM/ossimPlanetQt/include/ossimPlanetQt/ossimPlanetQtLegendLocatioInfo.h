#ifndef ossimPlanetQtLegendLocationInfo_HEADER
#define ossimPlanetQtLegendLocationInfo_HEADER
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimString.h>

class ossimPlanetQtLegendLocationInfo : public ossimReferenced
{
public:
   ossimPlanetQtLegendLocationInfo()
      :theCategory(""),
      theDescription(""),
      theTimeOutInSeconds(9999999999.0),
      theLat(0.0),
      theLon(0.0),
      theAltitude(0.0),
      theMetadata("")
   {
   }
   ossimPlanetQtLegendLocationInfo(const ossimPlanetQtLegendLocationInfo& src)
      :theCategory(src.theCategory),
      theDescription(src.theDescription),
      theTimeOutInSeconds(src.theTimeOutInSeconds),
      theLat(src.theLat),
      theLon(src.theLon),
      theAltitude(src.theAltitude),
      theMetadata(src.theMetadata)
      {
      }
   ossimPlanetQtLegendLocationInfo* clone()const
   {
      return new ossimPlanetQtLegendLocationInfo(*this);
   }
   ossimString theCategory;
   ossimString theDescription;
   double      theTimeOutInSeconds;
   double      theLat;
   double      theLon;
   double      theAltitude;
   ossimString theMetadata;
};
#endif
