#include <oms/Ephemeris.h>
#ifdef OSSIM_ENABLE_GPSTK
#ifdef TWO_PI
#undef TWO_PI
#endif

#include <gpstk/SunPosition.hpp>
#include <gpstk/MoonPosition.hpp>
#include <gpstk/Position.hpp>
#include <gpstk/WGS84Ellipsoid.hpp>
#include <gpstk/SystemTime.hpp>
#include <gpstk/ReferenceFrame.hpp>
#endif
#include <ossim/base/ossimDate.h>
#include <iostream>
#include <iomanip>
#include <ctime>
class omsEphemerisPrivateData
{
public:
#ifdef OSSIM_ENABLE_GPSTK
	omsEphemerisPrivateData()
	{
		
	}
	void setSunPositionFromIsoTime(const std::string& isoTime)
	{
		ossimLocalTm date;

		date.setIso8601(isoTime);
//		std::time_t t = date.getEpoc();
//std::cout << date << std::endl;
//    std::cout << "UTC:   " << std::put_time(std::gmtime(&t), "%c %Z") << '\n';
 //   std::cout << "local: " << std::put_time(std::localtime(&t), "%c %Z") << '\n';

 		gpstk::UnixTime now(date);
 		now.setTimeSystem(gpstk::TimeSystem::UTC);

   	gpstk::Position sunEcef;
   	sunEcef = m_sunPosition.getPosition(now);

		m_sunPositionLlh = gpstk::Position(sunEcef,
                                         gpstk::Position::Cartesian,
                                         &m_ellipsoidModel).asGeodetic(&m_ellipsoidModel);
//		std::cout << "Lat: " << m_sunPositionLlh.getGeodeticLatitude() << " Lon: " << m_sunPositionLlh.longitude() << std::endl;
   }
   double getSunPositionLatitude()const
   {
   	return m_sunPositionLlh.getGeodeticLatitude();
   }
   double getSunPositionLongitude()const
   {
   	return m_sunPositionLlh.longitude();
   }

	double getSunElevationAtLatLonHeight(double lat, double lon, double height)const
	{
   	gpstk::Position llh(lat, lon, height,
   							  gpstk::Position::Geodetic,
                          &m_ellipsoidModel,
                          gpstk::ReferenceFrame::WGS84);
   	return llh.elevation(m_sunPositionLlh);

	}
	double getSunAzimuthAtLatLonHeight(double lat, double lon, double height)const
	{
   	gpstk::Position llh(lat, lon, height,
   							  gpstk::Position::Geodetic,
                          &m_ellipsoidModel,
                          gpstk::ReferenceFrame::WGS84);
   	return llh.azimuth(m_sunPositionLlh);

	}
	mutable gpstk::WGS84Ellipsoid m_ellipsoidModel;
	gpstk::SunPosition m_sunPosition;
	gpstk::Position    m_sunPositionLlh;
#else
	omsEphemerisPrivateData()
	{
		
	}
	void setSunPositionFromIsoTime(const std::string& /*isoTime*/)
	{

	}
   double getSunPositionLatitude()const
   {
   	return 0.0;
   }
   double getSunPositionLongitude()const
   {
   	return 0.0;
   }
	double getSunElevationAtLatLonHeight(double /*lat*/, double /*lon*/, double /*height*/)const
	{
		return 0.0;
	}
	double getSunAzimuthAtLatLonHeight(double /*lat*/, double /*lon*/, double /*height*/)const
	{
		return 0.0;
	}
#endif
};

oms::Ephemeris::Ephemeris()
:m_privateData(0)
{
#ifdef OSSIM_ENABLE_GPSTK
	m_privateData = new omsEphemerisPrivateData();
#endif
}

oms::Ephemeris::~Ephemeris()
{
#ifdef OSSIM_ENABLE_GPSTK
	if(m_privateData) delete ((omsEphemerisPrivateData*)m_privateData);
#endif

	m_privateData = 0;
}

bool oms::Ephemeris::enabled()const
{
	return (m_privateData!=0);
}
double oms::Ephemeris::getSunPositionLatitude()const
{
	if(m_privateData)
	{
		return ((omsEphemerisPrivateData*)m_privateData)->getSunPositionLatitude();
	}
	return 0.0;
}
double oms::Ephemeris::getSunPositionLongitude()const
{
	if(m_privateData)
	{
		return ((omsEphemerisPrivateData*)m_privateData)->getSunPositionLongitude();
	}
	return 0.0;
}

void   oms::Ephemeris::setSunPositionFromIsoTime(const std::string& isoTime)
{
	if(m_privateData)
	{
		((omsEphemerisPrivateData*)m_privateData)->setSunPositionFromIsoTime(isoTime);
	} 
}

double oms::Ephemeris::getSunElevationAtLatLonHeight(double lat, double lon, double height)const
{
	if(m_privateData) return ((omsEphemerisPrivateData*)m_privateData)->getSunElevationAtLatLonHeight(lat, lon, height);

	return 0.0;
}

double oms::Ephemeris::getSunAzimuthAtLatLonHeight(double lat, double lon, double height)const
{
	if(m_privateData) return ((omsEphemerisPrivateData*)m_privateData)->getSunAzimuthAtLatLonHeight(lat, lon, height);

	return 0.0;
}

