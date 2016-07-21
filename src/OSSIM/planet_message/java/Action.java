package org.ossim.planet.message;
//
//  AbstractAction.java
//  
//
//  Created by Garrett Potts on 4/8/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//
import java.util.*;
import org.w3c.dom.Element;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;

public class Action extends BaseObject
{
   public enum Type
   {
      ADD,
      REMOVE,
	   SET,
      FLY_TO
	}
	private Action.Type actionType; 
   private String tagName;
   private String targetAttribute;
   private String originAttribute;
   private String nameAttribute;
   private String idAttribute;
   private LinkedList<AbstractObject> children;
   public Action(Action.Type a)
   {
      targetAttribute = new String("");
      originAttribute = new String("");
      nameAttribute = new String("");
      idAttribute = new String("");
      children = new LinkedList<AbstractObject>();
      setActionType(a);
   }
 	public Action.Type getActionType()
	{
		return actionType;
	}
	public void setActionType(Action.Type t)
   {
		actionType = t;
      switch(t)
      {
         case ADD:
         {
            tagName = "Add";
            break;
         }
         case REMOVE:
         {
            tagName = "Remove";
            break;
         }
         case SET:
         {
            tagName = "Set";
            break;
         }
         case FLY_TO:
         {
            tagName = "FlyTo";
            break;
         }
      }
   }
	public void setActionTypeFromString(String type)
	{
		if(type.equals("Add"))
		{
			actionType = Action.Type.ADD;
		}
		else if(type.equals("Remove"))
		{
			actionType = Action.Type.REMOVE;
		}
		else if(type.equals("Set"))
		{
			actionType = Action.Type.SET;
		}
		else if(type.equals("FlyTo"))
		{
			actionType = Action.Type.FLY_TO;
		}
		tagName = type;
	}
   public void setIdAttribute(String value)
   {
      idAttribute = value;
   }
   public String getIdAttribute()
   {
      return idAttribute;
   }
   public void setNameAttribute(String value)
   {
      nameAttribute = value;
   }
   public String getNameAttribute()
   {
      return nameAttribute;
   }
   public String getTargetAttribute()
   {
      return targetAttribute;
   }
   public void setTargetAttribute(String value)
   {
      targetAttribute = value;
   }
   public String getOriginAttribute()
   {
      return originAttribute;
   }
   public void setOriginAttribute(String value)
   {
      originAttribute = value;
   }
   public void addChild(AbstractObject obj)
   {
      children.add(obj);
   }
   public void add(AbstractObject obj)
   {
      children.add(obj);
   }
   public void clearChildren()
   {
      children.clear();
   }
   public int getNumberOfChildren()
   {
      return children.size();
   }
   public AbstractObject getChild(int idx)
   {
      AbstractObject result = null;
      try
      {
         result = children.get(idx);
      }
      catch(Exception e)
      {
         result = null;
      }
      return result;
   }
   public LinkedList<AbstractObject> getChildren()
   {
      return children;
   }
	public String getTagName()
	{
		return tagName;
	}
   public void toXml(Document doc,
                     Element thisObjectsElement)
   {
      if(targetAttribute.length() != 0)
      {
         thisObjectsElement.setAttribute("target", targetAttribute);
      }
      if(originAttribute.length() != 0)
      {
         thisObjectsElement.setAttribute("origin", originAttribute);
      }
      if(idAttribute.length() != 0)
      {
         thisObjectsElement.setAttribute("id", idAttribute);
      }
      if(nameAttribute.length() != 0)
      {
         thisObjectsElement.setAttribute("name", idAttribute);
      }
      for(int idx = 0; idx < children.size(); ++idx)
      {
         AbstractObject currentElement = children.get(idx);
         Element el = currentElement.newRootElement(doc);
         thisObjectsElement.appendChild(el);
         children.get(idx).toXml(doc, el);
      }
   }
	public void fromXml(Element thisObjectsElement)
	{
		setActionTypeFromString(thisObjectsElement.getTagName());
		targetAttribute = thisObjectsElement.getAttribute("target");
		originAttribute = thisObjectsElement.getAttribute("origin");
		idAttribute = thisObjectsElement.getAttribute("id");
		nameAttribute = thisObjectsElement.getAttribute("name");
		NodeList nodes = thisObjectsElement.getChildNodes();
		children.clear();
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
						if(testObj instanceof AbstractObject)
						{
							addChild((AbstractObject)testObj);
						}
					}
				}
			}
		}
	}
}
