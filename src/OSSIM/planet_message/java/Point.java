package org.ossim.planet.message;
import org.w3c.dom.Element;
import org.w3c.dom.Document;

public class Point extends Geometry
{
	boolean extrudeFlag;
	String altitudeMode;
	GeoCoordinate coordinate;
	
	public Point()
	{
		extrudeFlag = false;
		altitudeMode = "clampToGround";
		coordinate = new GeoCoordinate();
	}
	
	public Point(double lon,
					 double lat,
					 double alt)
	{
		altitudeMode = "clampToGround";
		extrudeFlag = false;
		coordinate = new GeoCoordinate(lon, lat, alt);
	}
	public Point(double lon,
					 double lat,
					 double alt,
					 String altMode)
	{
		extrudeFlag = false;
		coordinate = new GeoCoordinate(lon, lat, alt);
		altitudeMode = altMode;
	}
	public String getTagName()
	{
		return new String("Point");
	}
	
	public boolean getExtrudeFlag()
	{
		return extrudeFlag;
	}
	public void setExtrudeFlag(boolean extrudeFlag)
	{
		this.extrudeFlag = extrudeFlag;
	}
	public String getAltitudeMode()
	{
		return altitudeMode;
	}
	
	public GeoCoordinate getCoordinate()
	{
		return coordinate;
	}
	public void setCoordinate(GeoCoordinate coordinate)
	{
		this.coordinate = coordinate;
	}
	
   public void toXml(Document doc, 
                     Element thisObjectsElement)
   {
      super.toXml(doc, thisObjectsElement);
      CommonUtil.appendTextElement(doc, thisObjectsElement, "extrude", String.valueOf(extrudeFlag));
		if(altitudeMode.length()!=0)
		{
			CommonUtil.appendTextElement(doc, thisObjectsElement, "altitudeMode", String.valueOf(altitudeMode));
		}
		if(coordinate != null)
		{
			CommonUtil.appendTextElement(doc, thisObjectsElement, "coordinates", coordinate.toString(altitudeMode!="clampToGround"));
		}
   }
	public void fromXml(Element el)
	{
		super.fromXml(el);
		String coordinates = CommonUtil.getTextValue(el, "coordinates");
		coordinate.fromString(coordinates);
		String value = CommonUtil.getTextValue(el, "extrude");
		if(value.length() > 0)
		{
			extrudeFlag = Boolean.valueOf(value);
		}
		altitudeMode = CommonUtil.getTextValue(el, "altitudeMode");
	}
};