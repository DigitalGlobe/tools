package org.ossim.planet.message;

public class GeoCoordinate
{
	private double longitude;
	private double latitude;
	private double altitude;
	
	public GeoCoordinate()
	{
		longitude = 0.0;
		latitude  = 0.0;
		altitude  = 0.0;
	}
	public GeoCoordinate(double lon, 
								double lat,
								double alt)
	{
		set(lon, lat, alt);
	}
	public void set(double lon, 
						 double lat,
						 double alt)
	{
		longitude = lon;
		latitude  = lat;
		altitude  = alt;
	}
	public void setLongitude(double lon)
	{
		longitude = lon;
	}
	public double getLongitude()
	{
		return longitude;
	}
	public void setLatitude(double lat)
	{
		latitude = lat;
	}
	public double getLatitude()
	{
		return latitude;
	}
	public void setAltitude(double alt)
	{
		altitude = alt;
	}
	public double getAltitude()
	{
		return altitude;
	}
	public String toString(boolean includeAltitudeFlag)
	{
		String result = String.valueOf(longitude);
		result += ("," + String.valueOf(latitude));
		
		if(includeAltitudeFlag)
		{
			result += ","+String.valueOf(altitude);
		}
		
		return result;
	}
	public void fromString(String value)
	{
		String[] sArray = value.split(",");
		if(sArray.length > 1)
		{
			longitude = Double.valueOf(sArray[0]);
			latitude  = Double.valueOf(sArray[1]);
			altitude = 0.0;
			if(sArray.length >2)
			{
				altitude = Double.valueOf(sArray[2]);
			}
		}
	}
}