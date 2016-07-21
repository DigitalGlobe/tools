package org.ossim.planet.message;
import java.lang.IllegalArgumentException;
import org.w3c.dom.Element;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import java.util.Vector;
public class Group extends AbstractFeature
{
   public enum Type
   {
      GROUND_TEXTURE,
      FEATURE
	}
	private Vector<AbstractFeature> children = new Vector<AbstractFeature>();
   private Group.Type groupType = Group.Type.GROUND_TEXTURE;
	public Group()
	{
	}
   public Group(Group.Type type)
   {
      setGroupType(type);
   }
	public String getTagName()
	{
		return new String("Group");
	}
	public Vector<AbstractFeature> getChildren()
	{
		return children;
	}
	public void setChildren(Vector<AbstractFeature> children)
	{
		this.children = children;
	}
   public Group.Type getGroupType()
   {
      return groupType;
   }
   public String getGroupTypeAsString()
   {
      switch(groupType)
      {
         case GROUND_TEXTURE:
         {
            return new String("groundTexture");
         }
         case FEATURE:
         {
            return new String("feature");
         }
      }
      return new String("");
   }
   public void setGroupType(Group.Type type)
   {
      groupType = type;
   }
   public void setGroupType(String type)throws IllegalArgumentException
   {
      if(type.equals("groundTexture"))
      {
         groupType = Group.Type.GROUND_TEXTURE;
      }
      else if(type.equals("feature"))
      {
         groupType = Group.Type.FEATURE;
      }
      else
      {
         throw new IllegalArgumentException("Not a valid group type " + type);
      }
   }
	public void add(AbstractFeature feature)throws IllegalArgumentException
	{
      switch(groupType)
      {
         case GROUND_TEXTURE:
         {
            if(!(feature instanceof Image))
            {
               throw new IllegalArgumentException("Can only add Image objects to a groundTexture group");
            }
               
            break;
         }
         case FEATURE:
         {
            if(feature instanceof Image)
            {
               throw new IllegalArgumentException("Can only add non Image object features to a feature group");
            }
            break;
         }
      }  
		children.add(feature);
	}
	public void clear()
	{
		children.clear();
	}
	public void remove(int idx)
	{
		try
		{
			children.remove(idx);
		}
		catch(Exception e)
		{
		}
	}
	public AbstractFeature get(int idx)
	{
		try
		{
			return children.get(idx);
		}
		catch(Exception e)
		{
		}
		
		return null;
	}
	public void toXml(Document doc,
							Element thisObjectsElement)
	{
		super.toXml(doc, thisObjectsElement);
      thisObjectsElement.setAttribute("groupType", getGroupTypeAsString());
		if(children.size() > 0)
		{
			int idx = 0;
			for(idx = 0; idx < children.size();++idx)
			{
				Element element = doc.createElement(children.get(idx).getTagName());
				children.get(idx).toXml(doc, element);
				thisObjectsElement.appendChild(element);
			}
		}
	}
	public void fromXml(Element el)
	{
		children.clear();
		super.fromXml(el);
      try
      {
         setGroupType(el.getAttribute("groupType"));
      }
      catch(Exception e)
      {
         setGroupType(Group.Type.GROUND_TEXTURE);
      }
		NodeList list = el.getChildNodes();
		int n = list.getLength();
		int idx = 0;
		for(idx = 0; idx < n; ++idx)
		{
			Node node = list.item(idx);
			if(node instanceof Element)
			{
				Element childElement = (Element)node;
				BaseObject obj = BaseObjectFactory.instance().createBaseObject(childElement);
				if(obj != null)
				{
					if(obj instanceof AbstractFeature)
					{
                  try
                  {
                     add((AbstractFeature)obj);
                  }
                  catch(Exception e)
                  {
                     
                  }
					}
				}
			}
		}
	}
}
