package org.ossim.planet.message;
import org.w3c.dom.Element;
import org.w3c.dom.Document;
public abstract class AbstractExpireTime extends AbstractObject
{
	public AbstractExpireTime()
   {
	}
   public void toXml(Document doc,
                     Element thisObjectsElement)
   {
		super.toXml(doc, thisObjectsElement);
   }
}
