package org.ossim.planet.message;
import java.util.LinkedList;
import java.util.ListIterator;
import java.net.Socket;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.io.OutputStream;
import java.io.ByteArrayOutputStream;

public class Message extends Object
{
	LinkedList<BaseObject> actionList;
	public Message()
	{
		actionList = new LinkedList<BaseObject>();
	}
	
	public void add(Action action)
	{
		actionList.add(action);
	}
	
	public Action addAction(Action.Type t)
	{
		Action result = new Action(t);
		add(result);
		
		return result;
	}
	
	public LinkedList<BaseObject> getActionList()
	{
		return actionList;
	}
	public void setActionList(LinkedList<BaseObject> actionList)
	{
		this.actionList = actionList;
	}
	public void clear()
	{
		actionList.clear();
	}
	
	public void serialize(OutputStream out)
	{
		XMLEncoder encoder = new XMLEncoder(out);
		ListIterator<BaseObject> iter = actionList.listIterator(0);
		while(iter.hasNext())
		{
			BaseObject obj = iter.next();
			encoder.writeObject(obj);
		}
	}

	public void sendMessage(Socket s)
	{
		try
		{
			serialize(s.getOutputStream());
		}
		catch(Exception e)
	   {
		}
	}
	public void sendMessageToSocket(String ip,
											  int port,
											  boolean streamingFlag,
											  int timeout)
	{
		InetSocketAddress addr = new InetSocketAddress(ip, port);
		if(streamingFlag == true)
		{
			Socket s = new Socket();
			try
			{
				s.connect(addr, timeout);
				sendMessage(s);
				s.close();
			}
			catch(Exception e)
			{
				e.printStackTrace();
			}
		}
		else
		{
			try
			{
				DatagramSocket s = new DatagramSocket();
				ByteArrayOutputStream out = new ByteArrayOutputStream();
				serialize(out);
				if(out.size() > 0)
				{
					byte[] b = out.toByteArray();
					s.connect(addr);
					s.send(new DatagramPacket(b, b.length));
					s.close();
				}
			}
			catch(Exception e)
			{
			}
		}
	}
}