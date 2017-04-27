package org.ossim.planet.message;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import  org.w3c.dom.bootstrap.DOMImplementationRegistry;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;
import java.io.OutputStream;
import java.io.IOException;
public class XMLEncoder
{
   OutputStream theOutputStream;
   public XMLEncoder(OutputStream out)
   {
      theOutputStream = out;
   }
   public void writeObject(BaseObject obj)
   {
      try
      {
         DOMSerializer s = new DOMSerializer();
         DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
         DocumentBuilder builder = dbf.newDocumentBuilder();
         Document document = builder.newDocument();
         Element root = obj.newRootElement(document);
         document.appendChild(root);
         obj.toXml(document, root);
         s.setLineSeparator("");
         s.setAddHeaderFlag(false);
         s.setIndent("");
         s.serialize(document, theOutputStream);
      }
      catch(ParserConfigurationException e)
      {
         e.printStackTrace();
      }
      catch(IOException e)
      {
         e.printStackTrace();
      }
   }
}