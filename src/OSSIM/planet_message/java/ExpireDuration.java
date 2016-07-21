package org.ossim.planet.message;

import org.w3c.dom.Element;
import org.w3c.dom.Document;

public class ExpireDuration extends AbstractExpireTime
{
	double value;
	String unit;
	
	public ExpireDuration()
   {
		value = 0.0;
		unit = "seconds";
	}
	public ExpireDuration(double value,
								 String unit)
   {
		this.value = value;
		this.unit = unit;
	}
	public String getTagName()
	{
		return new String("ExpireDuration");
	}
	public void setValue(double value)
	{
		this.value = value;
	}
	public double getValue()
	{
		return value;
	}
	public void setUnit(String value)
	{
		unit = value;
	}
	String unit()
	{
		return unit;
	}
	
   public void toXml(Document doc,
                     Element thisObjectsElement)
   {
		thisObjectsElement.setAttribute("value", Double.toString(value));
      if(unit.length() != 0)
      {
			thisObjectsElement.setAttribute("unit", unit);
      }
   }
	public void fromXml(Element el)
	{
		super.fromXml(el);
		value = CommonUtil.toDouble(el.getAttribute("value"));
		unit  = el.getAttribute("unit");
	}
}