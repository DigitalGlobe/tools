package org.ossim.planet.message;
import org.w3c.dom.Element;

public class BaseObjectFactory
{
	static BaseObjectFactory theInstance = null;
	private BaseObjectFactory()
   {
		theInstance = this;
	}
	public static BaseObjectFactory instance()
	{
		if(theInstance == null)
		{
			theInstance = new BaseObjectFactory();
		}
		
		return theInstance;
	}
	public BaseObject createBaseObject(String name)
	{
		if(name.equals("Camera"))
		{
			return new Camera();
		}
		if(name.equals("ExpireDuration"))
		{
			return new ExpireDuration();
		}
		else if(name.equals("Point"))
		{
			return new Point();
		}
		else if(name.equals("Placemark"))
		{
			return new Placemark();
		}
		else if(name.equals("Set"))
		{
			return new Action(Action.Type.SET);
		}
		else if(name.equals("Add"))
		{
			return new Action(Action.Type.ADD);
		}
		else if(name.equals("Remove"))
		{
			return new Action(Action.Type.REMOVE);
		}
		else if(name.equals("Image"))
		{
			return new Image();
		}
		else if(name.equals("Identity"))
		{
			return new Identity();
		}
		else if(name.equals("Group"))
		{
			return new Group();
		}
		return null;
	}
	public BaseObject createBaseObject(Element el)
	{
		if(el == null) return null;
		BaseObject obj = createBaseObject(el.getTagName());
		if(obj !=null)
		{
			obj.fromXml(el);
		}
		
		return obj;
	}
}