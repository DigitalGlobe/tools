package org.ossim.planet.message;
import org.w3c.dom.Element;
import org.w3c.dom.Document;

public class Image extends AbstractFeature
{
	private String location;
	
	public Image()
	{
		location = new String("");
	}
	public Image(String location)
   {
		location = location;
   }
	public String getTagName()
	{
		return new String("Image");
	}
	public void setLocation(String location)
	{
		this.location = location;
	}
	public String getLocation()
	{
		return location;
	}
   public void toXml(Document doc,
			    			Element thisObjectsElement)
	{
		super.toXml(doc, thisObjectsElement);
		CommonUtil.appendTextElement(doc, thisObjectsElement, "filename", location);
	}
	public void fromXml(Element el)
	{
		super.fromXml(el);
		location = CommonUtil.getTextValue(el, "filename");
	}
}