import org.ossim.planet.message.*;
import java.lang.*;
import java.net.Socket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.io.OutputStream;

class PlacemarkStressTest
	{
		public PlacemarkStressTest()
		{
		}
		public static void main(String[] args)
		{
         String groupName = "";
			if(args.length < 9)
			{
				System.out.println("USAGE: java PlacemarkStressTest <ip> <port> <target receiver> <number of points> <start id> <minLon> <minLat> <maxLon> <maxLat> [<group>]"); 
			}
			else
			{
				String ip = args[0];
				int port = Integer.valueOf(args[1]);
				String portType = "tcp";
				String target = args[2];
				int numberOfPoints = Integer.valueOf(args[3]);
				int startId        = Integer.valueOf(args[4]);
				double minLon      = Double.valueOf(args[5]);
				double minLat      = Double.valueOf(args[6]);
				double maxLon      = Double.valueOf(args[7]);
				double maxLat      = Double.valueOf(args[8]);
            if(args.length > 9)
            {
               groupName = args[9];
            }
				double lonRange    = maxLon-minLon;
				double latRange    = maxLat-minLat;
				
				Socket socket = new Socket();
				OutputStream socketStream = null;
				try
				{
					InetSocketAddress addr = new InetSocketAddress(ip, port);
					socket.connect(addr, 8000);
					socketStream = socket.getOutputStream();
				}
				catch(Exception e)
				{
					e.printStackTrace();
					return;
				}
            Group group = null;
            if(groupName.length()>0)
            {
               group = new Group(Group.Type.FEATURE);
               group.setId(groupName);
               group.setName(groupName);
            }
				Action addAction    = new Action(Action.Type.ADD);
				Action removeAction = new Action(Action.Type.REMOVE);
				Message m = new Message();
            if(group != null)
            {
               addAction.setTargetAttribute(target);
               addAction.addChild(group);
               m.add(addAction);
               m.serialize(socketStream);
               m = new Message();
            }
				while(true)
				{
					int idx = 0;
					for(;idx < numberOfPoints; ++idx)
					{
						addAction.clearChildren();
						m = new Message();
						Placemark p = new Placemark();
                  p.setParentId(groupName);
						String name = new String("Test") + String.valueOf(idx+startId);
						String id   = name;
						String description = name;
						
						double lat = minLat + latRange*Math.random();
						double lon = minLon + lonRange*Math.random();
						p.setGeometry(new Point(lon,
														lat,
														0.0,
														"clampToGround"));
						
						p.setName(name);
						p.setDescription(description);
						p.setId(id);
						addAction.setTargetAttribute(target);
						addAction.addChild(p);
						m.add(addAction);
						m.serialize(socketStream);
						try
						{
							Thread.sleep(10);
						}
						catch(Exception e)
						{
						}
					}
					
					idx = 0;
					for(;idx < numberOfPoints; ++idx)
					{
						removeAction.clearChildren();
						String id = new String("Test") + String.valueOf(idx+startId);
						
						removeAction.setTargetAttribute(target);
						removeAction.setIdAttribute(id);
						m = new Message();
						m.add(removeAction);
						m.serialize(socketStream);
						try
						{
							Thread.sleep(10);
						}
						catch(Exception e)
						{
						}
					}
				}
			}
		}
	}
