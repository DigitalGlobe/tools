package org.ossim.planet.message;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
abstract public class BaseObject extends Object
{
	public BaseObject()
	{
	}
	public Element newRootElement(Document doc)
   {
      return doc.createElement(getTagName());
   }
	public abstract String getTagName();
   public abstract void toXml(Document doc,
										Element thisObjectsElement);
	public abstract void fromXml(Element el);	
}