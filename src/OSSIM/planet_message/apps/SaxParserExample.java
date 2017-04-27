import org.ossim.planet.message.*;
import java.io.IOException;
import java.io.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.net.*;
import java.lang.Thread;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import org.xml.sax.helpers.DefaultHandler;

public class SaxParserExample extends DefaultHandler
{
   public class AdapterStream extends BufferedInputStream
      {
         private boolean theAllowToCloseFlag = false;
         
         public AdapterStream(InputStream in)
         {
            super(in);
         }
         public void setAllowToCloseFlag(boolean flag)
         {
            theAllowToCloseFlag = flag;
         }
         
         public void close() throws IOException
         {
            if(theAllowToCloseFlag)
            {
               super.close();
            }
         }
      }
   ServerSocket server;
   String theStartElement = null;
   boolean theBlockIsGood = false;
   String theXmlString = new String("");
	AdapterStream theAdapter = null;
	public SaxParserExample()
	{
		//create a list to hold the employee objects
	}
   public void runExampleServer(String host, int port)
   {
		InetSocketAddress add = new InetSocketAddress(host, port);
		try
		{
			server= new ServerSocket();
			server.bind(add);
		}
		catch(IOException e)
		{
		}
		SAXParserFactory spf = SAXParserFactory.newInstance();
		spf.setValidating(false);
		for(;;)
		{
			Socket s = null;
			theBlockIsGood = true;
			theStartElement = null;
			try
			{
				s = server.accept();
				InputStream in = s.getInputStream();
				theAdapter = new AdapterStream(in);
			}
			catch(IOException e)
			{
			}
			if(theAdapter != null)
			{
				SAXParser sp = null;
				
				for(;theBlockIsGood == true;)
				{
					try
					{
						sp = spf.newSAXParser();
					}
					catch(Exception e)
					{
						
					}
					theBlockIsGood = false;
					theStartElement = null;
					theXmlString = new String("");
					try 
					{
						sp.parse(theAdapter, this);
					}
					catch(SAXException se) 
					{
					}
					catch (IOException ie) 
					{
					}
					if(theBlockIsGood == true)
					{
                  System.out.println("\n________________________________________________\n");
						XMLDecoder decoder = new XMLDecoder(theXmlString);
						BaseObject obj = decoder.readObject();
						if(obj != null)
						{
							// let check to see if it's an Action object
							//
							if(obj instanceof Action)
							{
								Action a = (Action)obj;
                        XMLEncoder encoder = new XMLEncoder(System.out);
                        
								if(a.getActionType() == Action.Type.SET)
								{
									// do something with the children
									// by accessing a.list()
									//
                           encoder.writeObject(a);
								}
							}
						}
					}
					else
					{
                  theBlockIsGood = true;
						theAdapter.mark(1);
						try
						{
							if(theAdapter.read() == -1)
							{
								theBlockIsGood = false;
								theAdapter.setAllowToCloseFlag(true);
							}
						}
						catch(IOException e)
						{
							theBlockIsGood = false;
						}
						try
						{
							theAdapter.reset();
						}
						catch(IOException e)
						{
						}
					}
				}// for loop theBlockIsGood
				theAdapter.setAllowToCloseFlag(true);
			}
		}
	}
   public void characters(char[] ch, int start, int length) throws SAXException 
   {
		theXmlString = theXmlString + new String(ch,start,length);
	}   
   public void startElement(String uri, String localName, String qName, Attributes attributes)
   {
      theXmlString = theXmlString + "<"+qName;
      if(attributes.getLength() > 0)
      {
         int l = attributes.getLength();
         theXmlString = theXmlString + " ";
         int idx = 0;
         for(idx = 0; idx < l-1;++idx)
         {
            theXmlString = theXmlString + attributes.getQName(idx) + "=\"" + attributes.getValue(idx) + "\" ";
         }
         theXmlString = theXmlString + attributes.getQName(idx) + "=\"" + attributes.getValue(idx) + "\"";
      }
      theXmlString = theXmlString +">";
      if(theStartElement == null)
      {
         theStartElement = qName;
      }
	}
   public void endElement(String uri, String localName, String qName)throws SAXException
   {
      theXmlString = theXmlString + "</"+qName+">";

      if(qName == theStartElement)
      {
			theBlockIsGood = true;
         throw new SAXException();//kick out
      }
	}   
	public static void main(String[] args)
   {
      SaxParserExample dpe = new SaxParserExample();

		if(args.length == 2)
		{
			dpe.runExampleServer(args[0], Integer.valueOf(args[1]));
		}
		else
		{
			System.out.println("Usage: java -cp ../classes/OssimPlanetMessage.jar:. SaxParserExample <ip> <port>");
		}
	}
   
}
