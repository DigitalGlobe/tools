package org.ossim.planet.message;
import org.w3c.dom.Element;
import org.w3c.dom.Document;

abstract class AbstractFeature extends AbstractObject
{
	private String name;
	private String description;
	
	public AbstractFeature()
	{
		name        = new String("");
		description = new String("");
	}
	public AbstractFeature(String n,   // name 
								  String des) // description
	{
		name = n;
		description = des;
	}
	public void set(String n, // name
						 String des) // descrtiption
	{
		name = n;
		description = des;
	}
	public String getName()
	{
		return name;
	}
	
	public void setName(String name)
	{
		this.name = name;
	}
	public String getDescription()
	{
		return description;
	}
	public void setDescription(String value)
	{
		description = value;
	}
   public void toXml(Document doc, 
                     Element thisObjectsElement)
   {
      super.toXml(doc, thisObjectsElement);
		if(name.length() != 0)
		{
			CommonUtil.appendTextElement(doc, thisObjectsElement, "name", name);
		}
		if(description.length()!=0)
		{
			CommonUtil.appendCDATAElement(doc, thisObjectsElement, "description", description);
		}
   }
	public void fromXml(Element el)
	{
		super.fromXml(el);
		name        = CommonUtil.getTextValue(el, "name");
		description = CommonUtil.getTextValue(el, "description");
	}
}