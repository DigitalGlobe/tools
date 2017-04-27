//
//  AbstractObject.java
//  
//
//  Created by Garrett Potts on 4/8/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//
package org.ossim.planet.message;
import org.w3c.dom.Element;
import org.w3c.dom.Document;

abstract public class AbstractObject extends BaseObject
{
   private String attributeId;
   private String id;
   private String parentId;
   
   AbstractObject()
   {
      attributeId = new String("");
      id          = new String("");
      parentId    = new String("");
   }
   String getAttributeId()
   {
      return attributeId;
   }
   public void setAttributeId(String value)
   {
      attributeId = value;
   }
   String getId()
   {
      return id;
   }
   public void setId(String value)
   {
      id = value;
   }
   String getParentId()
   {
      return parentId;
   }
   public void setParentId(String value)
   {
      parentId = value;
   }
   public void toXml(Document doc,
                     Element thisObjectsElement)
   {
      if(attributeId.length() != 0)
      {
         thisObjectsElement.setAttribute("id", attributeId);
      }
      if(parentId.length() != 0)
      {
         thisObjectsElement.setAttribute("parentId", parentId);
      }
      if(id.length() != 0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "id", id);
      }
   }
	public void fromXml(Element el)
	{
		attributeId = el.getAttribute("id");
		parentId    = el.getAttribute("parentId");
		id          = CommonUtil.getTextValue(el, "id");
	}
	public abstract String getTagName();
}
