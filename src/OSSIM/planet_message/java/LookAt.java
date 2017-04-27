//
//  Camera.java
//  
//
//  Created by Garrett Potts on 4/8/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//
package org.ossim.planet.message;
import org.w3c.dom.Element;
import org.w3c.dom.Document;

public class LookAt extends AbstractObject
{
   private String verticalReferenceAttribute = "";
   private String longitude = "";
   private String latitude = "";
   private String altitude ="";
   private String heading ="";
   private String pitch = "";
   private String roll = "";
   private String altitudeMode = "";
	private String range = "";
   
   public LookAt()
   {
   }
   public LookAt(double lon,
                 double lat,
                 double alt,
                 double h,
                 double p,
                 double r,
                 String altMode,
					  double ran)
   {
      set(lon, lat, alt, h, p, r, altMode, ran);
   }
   public void set(double lon,
                   double lat,
                   double alt,
                   double h,
                   double p,
                   double r,
                   String altMode,
						 double ran)
   {
      longitude = String.valueOf(lon);
      latitude  = String.valueOf(lat);
      altitude  = String.valueOf(alt);
      heading   = String.valueOf(h);
      pitch     = String.valueOf(p);
      roll      = String.valueOf(r);
      altitudeMode = altMode;      
      range     = String.valueOf(ran);
   }
   public void setVerticalReferenceAttribute(String value)
   {
      verticalReferenceAttribute = value;
   }
   public String getVerticalReferenceAttribute()
   {
      return verticalReferenceAttribute;
   }
   public double getLongitude()
   {
      return CommonUtil.toDouble(longitude);
   }
   public void setLongitude(double value)
   {
      longitude = String.valueOf(value);
   }
   public double getLatitude()
   {
      return CommonUtil.toDouble(latitude);
   }
   public void setLatitude(double value)
   {
      latitude = String.valueOf(value);
   }
   public double getAltitude()
   {
      return CommonUtil.toDouble(altitude);
   }
   public void setAltitude(double value)
   {
      altitude = String.valueOf(value);
   }
   public double getHeading()
   {
      return CommonUtil.toDouble(heading);
   }
   public void setHeading(double value)
   {
      heading = String.valueOf(value);;
   }
   public double getPitch()
   {
      return CommonUtil.toDouble(pitch);
   }
   public void setPitch(double value)
   {
      pitch = String.valueOf(value);;
   }
   public double getRoll()
   {
      return CommonUtil.toDouble(roll);
   }
   public void setRoll(double value)
   {
      roll = String.valueOf(value);
   }
   public void setAltitudeMode(String value)
   {
      altitudeMode = value;
   }
   public String getAltitudeMode()
   {
      return altitudeMode;
   }
   public void setOrientation(double h, double p, double r)
   {
      setHeading(h);
      setPitch(p);
      setRoll(r);
   }
   public void setPosition(double lon, double lat, double alt)
   {
      setLongitude(lon);
      setLatitude(lat);
      setAltitude(alt);
   }
	public void setRange(double value)
	{
		range = String.valueOf(value);;
	}
	public String getRange()
	{
		return range;
	}
	public String getTagName()
	{
		return new String("LookAt");
	}
   public void toXml(Document doc, 
                     Element thisObjectsElement)
   {
      super.toXml(doc, thisObjectsElement);
      super.toXml(doc, thisObjectsElement);
      if(verticalReferenceAttribute.length() != 0)
      {
         thisObjectsElement.setAttribute("vref", verticalReferenceAttribute);
      }
      if(longitude.length()!=0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "longitude", longitude);
      }
      if(latitude.length()!=0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "latitude", latitude);
      }
      if(altitude.length()!=0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "altitude", altitude);
      }
      if(heading.length() != 0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "heading", heading);
      }
      if(pitch.length() != 0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "pitch", pitch);
      }
      if(roll.length() != 0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "roll", roll);
      }
      if(altitudeMode.length() != 0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "altitudeMode", altitudeMode);
      }
      if(range.length()!=0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "range", String.valueOf(range));
      }
   }
	public void fromXml(Element el)
	{
		super.fromXml(el);
      verticalReferenceAttribute = el.getAttribute("vref");
      longitude                  = CommonUtil.getTextValue(el, "longitude");
      latitude                   = CommonUtil.getTextValue(el, "latitude");
      altitude                   = CommonUtil.getTextValue(el, "altitude");
      heading                    = CommonUtil.getTextValue(el, "heading");
      pitch                      = CommonUtil.getTextValue(el, "pitch");
      roll                       = CommonUtil.getTextValue(el, "roll");
      altitudeMode               = CommonUtil.getTextValue(el, "altitudeMode");
		range                      = CommonUtil.getTextValue(el, "range");
	}
}
