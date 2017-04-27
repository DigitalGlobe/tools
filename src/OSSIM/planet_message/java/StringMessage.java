package org.ossim.planet.message;
import java.net.Socket;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.io.OutputStream;
import java.io.ByteArrayOutputStream;

public class StringMessage extends Object
{
   byte[] message;
   public StringMessage()
   {
      message = null;
   }
   public StringMessage(String s)
   {
      message = s.getBytes();
   }
   public StringMessage(byte[] b)
   {
      message = b;
   }
	public void setMessage(byte[] message)
	{
		this.message = message;
	}
	public byte[] getMessage()
	{
		return message;
	}
	public void sendToSocket(String ip,
                            int port,
                            String  portType, //"udp" or "tcp"
                            int timeout)
	{
		InetSocketAddress addr = new InetSocketAddress(ip, port);
      if(message == null)
      {
         return;
      }
		if(portType.equals("tcp"))
		{
			Socket s = new Socket();
			try
			{
				s.connect(addr, timeout);
            s.getOutputStream().write(message);
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
            s.connect(addr);
            s.send(new DatagramPacket(message, message.length));
            s.close();
			}
			catch(Exception e)
			{
			}
		}
	}
}