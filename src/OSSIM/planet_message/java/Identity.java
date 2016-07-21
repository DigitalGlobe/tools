package org.ossim.planet.message;
import org.w3c.dom.Element;
import org.w3c.dom.Document;

public class Identity extends AbstractObject
{
   private String userName = "";
   private String password = "";
   private String domain   = "";

   public Identity()
   {
   }
   public Identity(String username)
   {
      userName = username;
   }
   public String getUserName()
   {
      return userName;
   }
   public void setUserName(String value)
   {
      userName = value;
   }
   public String getPassword()
   {
      return password;
   }
   public void setPassword(String value)
   {
      password = value;
   }
   public String getDomain()
   {
      return domain;
   }
   public void setDomain(String value)
   {
      domain = value;
   }
   public String getTagName()
   {
      return new String("Identity");
   }
   public void toXml(Document doc, 
                     Element thisObjectsElement)
   {
      super.toXml(doc, thisObjectsElement);
      if(userName.length() != 0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "username", userName);
      }
      if(password.length() != 0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "password", password);
      }
      if(domain.length() != 0)
      {
         CommonUtil.appendTextElement(doc, thisObjectsElement, "domain", domain);
      }
   }
   public void fromXml(Element el)
   {
      super.fromXml(el);
      userName    = CommonUtil.getTextValue(el, "username");
      password    = CommonUtil.getTextValue(el, "password");
      domain      = CommonUtil.getTextValue(el, "domain");
   }

}