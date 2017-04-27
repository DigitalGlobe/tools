package org.ossim.planet.message;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.InputStream;
import java.io.IOException;
import java.io.ByteArrayInputStream;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;
import javax.xml.parsers.ParserConfigurationException;

public class XMLDecoder
{
   private InputStream theInputStream;
   public XMLDecoder(InputStream in)
   {
		setInputStream(in);
   }
   public XMLDecoder(String in)
	{
		setInputStream(in);
	}
	public void setInputStream(InputStream in)
	{
		theInputStream = in;
	}
	public void setInputStream(String in)
	{
		theInputStream = new ByteArrayInputStream(in.getBytes());
	}
   public BaseObject readObject()
   {
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
      BaseObject result = null;
		try {
			//Using factory get an instance of document builder
			DocumentBuilder db = dbf.newDocumentBuilder();
			
			//parse using builder to get DOM representation of the XML file
			Document document = db.parse(theInputStream);
         result = BaseObjectFactory.instance().createBaseObject(document.getDocumentElement());
		}
      catch(ParserConfigurationException pce) 
      {
			pce.printStackTrace();
		}
      catch(SAXException se) 
      {
			se.printStackTrace();
		}
      catch(IOException ioe) 
      {
			ioe.printStackTrace();
		}
      
      return result;
   }
}