#ifndef omsEphemeris_HEADER
#define omsEphemeris_HEADER
#include <oms/Constants.h>
#include <string>

namespace oms{
	class OMSDLL Ephemeris{
	public:
		Ephemeris();
		virtual ~Ephemeris();
		bool enabled()const;

		void   setSunPositionFromIsoTime(const std::string& isoTime);
		double getSunPositionLatitude()const;
		double getSunPositionLongitude()const;
		double getSunElevationAtLatLonHeight(double lat, double lon, double height)const;
		double getSunAzimuthAtLatLonHeight(double lat, double lon, double height)const;


	protected:
	   void *m_privateData;
	};
}

#endif
