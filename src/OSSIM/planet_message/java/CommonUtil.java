package org.ossim.planet.message;
import org.w3c.dom.Element;
import org.w3c.dom.Document;
import org.w3c.dom.Text;
import org.w3c.dom.CDATASection;
import org.w3c.dom.NodeList;

public class CommonUtil
{
   public static Element newTextElement(Document doc,
                                    String nodeName,
                                    String value)
   {
      Element result = doc.createElement(nodeName);
      Text t = doc.createTextNode(value);
      result.appendChild(t);
      
      return result;
	}
	public static Element newCDATAElement(Document doc, 
													  String nodeName,
													  String value)
	{
      Element result = doc.createElement(nodeName);
		CDATASection cdata = doc.createCDATASection(value);
		result.appendChild(cdata);
		
		return result;
	}
	public static void appendTextElement(Document doc,
												Element parent,
												String name,
												String value)
	{
		Element el = newTextElement(doc, name, value);
		parent.appendChild(el);
	}
	public static void appendCDATAElement(Document doc,
													  Element parent,
													  String name,
													  String value)
	{
		Element el = newCDATAElement(doc, name, value);
		parent.appendChild(el);
	}
	public static String getTextValue(Element ele, String tagName) 
   {
		String textVal = new String("");
		try
		{
			NodeList nl = ele.getElementsByTagName(tagName);
			if(nl != null) 
			{
				if(nl.getLength() > 0)
				{
					Element el = (Element)nl.item(0);
					if(el != null)
					{
						textVal = el.getFirstChild().getNodeValue();
					}
				}
			}
		}
		catch(Exception e)
		{
			textVal = new String("");
		}
		return textVal;
	}
	public static double toDouble(String input)
   {
		double result = 0.0;
		
      if(input == null) return result;
		try
		{
			result = Double.valueOf(input);
		}
		catch(NumberFormatException e)
		{
		}
		
		return result;
   }
}