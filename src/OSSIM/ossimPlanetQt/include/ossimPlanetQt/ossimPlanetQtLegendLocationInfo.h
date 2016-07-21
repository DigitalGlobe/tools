#ifndef ossimPlanetQtLegendLocationInfo_HEADER
#define ossimPlanetQtLegendLocationInfo_HEADER
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimString.h>
#include <osg/ref_ptr>
#include <ossimPlanet/ossimPlanetLookAt.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class OSSIMPLANETQT_DLL ossimPlanetQtLegendLocationInfo : public ossimReferenced
{
public:
   ossimPlanetQtLegendLocationInfo()
      :theValidFlag(false),
      theCategory(""),
      theDescription(""),
      theTimeOutInSeconds(9999999999.0),
      theLat(0.0),
      theLon(0.0),
      theAltitude(0.0),
      theMetadata("")
   {
      theLookAt = new ossimPlanetLookAt();
   }
   ossimPlanetQtLegendLocationInfo(const ossimPlanetQtLegendLocationInfo& src)
      :theValidFlag(src.theValidFlag),
      theCategory(src.theCategory),
      theDescription(src.theDescription),
      theTimeOutInSeconds(src.theTimeOutInSeconds),
      theLat(src.theLat),
      theLon(src.theLon),
      theAltitude(src.theAltitude),
      theMetadata(src.theMetadata),
      theLookAt(src.theLookAt->clone())
      {
      }
   virtual ossimPlanetQtLegendLocationInfo* clone()const
   {
      return new ossimPlanetQtLegendLocationInfo(*this);
   }
   
   bool        theValidFlag;
   ossimString theCategory;
   ossimString theDescription;
   double      theTimeOutInSeconds;
   double      theLat;
   double      theLon;
   double      theAltitude;
   ossimString theMetadata;
   osg::ref_ptr<ossimPlanetLookAt> theLookAt;
};
#endif
