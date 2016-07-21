import org.ossim.planet.message.*;
import java.io.IOException;
import java.io.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.net.*;
import java.lang.Thread;
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

public class DomParserExample 
{
   ServerSocket server;
   
	public DomParserExample()
	{
		//create a list to hold the employee objects
	}
   public void runExampleServer(String host, int port)
   {
      try
      {
         InetSocketAddress add = new InetSocketAddress(host, port);
         server= new ServerSocket();
         server.bind(add);
         for(;;)
         {
            Socket s = server.accept();
            InputStream in = s.getInputStream();
            StringBuffer soFar = new StringBuffer();
            byte[] buf = new byte[1024];
            boolean isDone = false;
            int numBytesRead = 0;
            while(numBytesRead>=0)
            {
               numBytesRead = in.read(buf, 0, 1024);
               if(numBytesRead > 0)
               {
                  String tmp = new String(buf, 0, numBytesRead);
                  soFar.append(tmp);
                  while(handleMessage( soFar ))
                  {
                  }
               }
            }
         }
      }
      catch(IOException ioe)
      {
         
      }
   }
   protected BaseObject handleMessage2(InputStream in)
   {
      XMLDecoder decoder = new XMLDecoder(in);
      
      return decoder.readObject();
   }
   protected boolean handleMessage(StringBuffer msg)
   {
      boolean result = false;
      // scan for element root
      //
      if(msg.length()>0)
      {
         int elementIdxStart = 0;
         int elementIdxEnd   = 0;
         int idx = 0;
         boolean found = false;
         int len = msg.length();
         
         // we look for the pattern <Tag>
         for(idx = 0; ((idx <  len)&&(!found));++idx)
         {
            if(msg.charAt(idx) == '<')
            {
               found = true;
            }
         }
         if(!found)
         {
            msg = new StringBuffer("");
            return false;
         }
         elementIdxStart = idx;
         found = false;
         for(idx = elementIdxStart; ((idx <  len)&&(!found));++idx)
         {
            char c = msg.charAt(idx);
            if((c == ' ')||(c=='>'))
            {
               found = true;
               elementIdxEnd = idx;
            }
         }
         if(!found)
         {
            return false;
         }
         String element = msg.substring(elementIdxStart, elementIdxEnd);
         int idxStart = elementIdxStart-1;
         if(idxStart>=0)
         {
            int idxEnd = msg.indexOf("</"+element+">");
            if(idxEnd>=0)
            {
               result = true;
               String sub = msg.substring(idxStart, idxEnd+6);
               msg = msg.replace(idxStart, idxEnd+6, "");
					
					XMLDecoder decoder = new XMLDecoder(sub);
					BaseObject obj = decoder.readObject();
					XMLEncoder encoder = new XMLEncoder(System.out);
					encoder.writeObject(obj);
					System.out.println("");
            }
         }
      }
      return result;
   }
	
	public static void main(String[] args)
   {
      DomParserExample dpe = new DomParserExample();

		if(args.length == 2)
		{
			dpe.runExampleServer(args[0], Integer.valueOf(args[1]));
		}
		else
		{
			dpe.runExampleServer("localhost", 9999);
		}
	}
   
}
