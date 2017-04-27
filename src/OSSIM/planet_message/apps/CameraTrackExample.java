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

public class CameraTrackExample extends DefaultHandler
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
	public CameraTrackExample()
	{
		//create a list to hold the employee objects
	}
	private Camera getCamera(Action a)
	{
		int idx = 0;
		for(idx = 0; idx < a.getNumberOfChildren();++idx)
		{
			AbstractObject obj = a.getChild(idx);
			if(obj instanceof Camera)
			{
				return (Camera)obj; 
			}
		}
		
		return null;
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
			OutputStream out = null;
			try
			{
				s = server.accept();
				InputStream in = s.getInputStream();
				out = s.getOutputStream();
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
						XMLDecoder decoder = new XMLDecoder(theXmlString);
						BaseObject obj = decoder.readObject();
						if(obj != null)
						{
							// let check to see if it's an Action object
							//
							if(obj instanceof Action)
							{
								Action a = (Action)obj;
                        
								if(a.getActionType() == Action.Type.SET)
								{
									Camera camera = getCamera(a);
									
									if(camera!=null)
									{
										if(out != null)
										{
											Action addAction = new Action(Action.Type.ADD);
											addAction.setTargetAttribute(":idolbridge");
											Placemark p = new Placemark();
                                 ExpireDuration duration = new ExpireDuration(2, "seconds");
                                 p.setExpireTime(duration);
											Point point = new Point(camera.getLongitude(), camera.getLatitude(), 0.0);
											p.setGeometry(point);
											p.setName("X");
											addAction.addChild(p);
											XMLEncoder e = new XMLEncoder(out);
											e.writeObject(addAction);
                                 //e = new XMLEncoder(System.out);
                                 //e.writeObject(addAction);
										}
									}
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
      CameraTrackExample dpe = new CameraTrackExample();

		if(args.length == 2)
		{
			dpe.runExampleServer(args[0], Integer.valueOf(args[1]));
		}
		else
		{
			System.out.println("Usage: java -cp ../classes/OssimPlanetMessage.jar:. CameraTrackExample <ip> <port>");
		}
	}
   
}
