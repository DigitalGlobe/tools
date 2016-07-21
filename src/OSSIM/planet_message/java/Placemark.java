package org.ossim.planet.message;
import org.w3c.dom.Element;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class Placemark extends AbstractFeature
{
	AbstractExpireTime expireTime;
	Geometry geometry;
	public Placemark()
   {
		expireTime = null;
		geometry = null;
	}
	public void setExpireTime(AbstractExpireTime value)
	{
		expireTime = value;
	}
	AbstractExpireTime getExpireTime()
	{
		return expireTime;
	}
	public void setGeometry(Geometry geom)
	{
		this.geometry = geom;
	}
	public Geometry getGeometry()
	{
		return geometry;
	}
	public String getTagName()
	{
		return new String("Placemark");
	}
   public void toXml(Document doc, 
                     Element thisObjectsElement)
   {
		super.toXml(doc, thisObjectsElement);
		
		if(geometry != null)
		{
			Element geomEl = geometry.newRootElement(doc);
			geometry.toXml(doc, geomEl);
			thisObjectsElement.appendChild(geomEl);
		}
		if(expireTime != null)
		{
			Element expireEl = expireTime.newRootElement(doc);
			expireTime.toXml(doc, expireEl);
			thisObjectsElement.appendChild(expireEl);
		}
	}
	public void fromXml(Element el)
	{
		super.fromXml(el);
		NodeList nodes = el.getChildNodes();
		if(nodes.getLength() > 0)
		{
			int n = nodes.getLength();
			for(int idx = 0; idx < n;++idx)
			{
				Node tempNode = nodes.item(idx);
				if(tempNode instanceof Element)
				{
					BaseObject testObj = BaseObjectFactory.instance().createBaseObject((Element)tempNode);
					if(testObj!=null)
					{
						if(testObj instanceof AbstractExpireTime)
						{
							setExpireTime((AbstractExpireTime)testObj);
						}
						else if(testObj instanceof Geometry)
						{
							setGeometry((Geometry)testObj);
						}
					}
				}
			}
		}
	}
}