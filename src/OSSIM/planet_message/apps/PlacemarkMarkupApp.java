import org.ossim.planet.message.*;

class PlacemarkMarkupApp
{
	public PlacemarkMarkupApp()
	{
	}
	// Exmaple Arguments
	//  "Add,localhost,8000,:annotation,MyId,MyName,My description,-120,30,0,clampToGround" 
	//  "Remove,localhost,8000,:annotation,MyId,,,,,," 
	//
	public static void main(String[] args)
	{
		if(args.length == 0)
		{
			System.out.println("USAGE: java PlacemarkMarkupApp <action>,<ip>,<port>,<target>,<id>,<name>,<description>,<lon>,<lat>,<altitude>,<altitudeMode>"); 
			System.out.println("       This is a single argument comma separated list.  Empty value should have no value in it but all commas need to be present"); 
		}
		else
		{
			String[] argArray = args[0].split(",");
			if(argArray.length == 11)
			{
				String  action       = argArray[0];
				String  ip           = argArray[1];
				String  port         = argArray[2];
				String  target       = argArray[3];
				String  id           = argArray[4];
				String  name         = argArray[5];
				String  description  = argArray[6];
				String  lon          = argArray[7];
				String  lat          = argArray[8];
				String  altitude     = argArray[9];
				String  altitudeMode = argArray[10];
            
				Action a = null;
            if(action.equals("Add"))
            {
               a = new Action(Action.Type.ADD);
            }
            else
            {
               System.out.println("Unsupported action type " + action);
               return;
            }
				Placemark p = new Placemark();
				if((lon.length() !=0)&&
					(lat.length() !=0)&&
					(altitude.length() != 0))
				{
					p.setGeometry(new Point(Double.valueOf(lon),
													Double.valueOf(lat),
													Double.valueOf(altitude),
													altitudeMode));
				}
				p.setName(name);
				p.setDescription(description);
				p.setId(id);
				a.setTargetAttribute(target);
				a.addChild(p);
				Message m = new Message();
				m.add(a);
				m.sendMessageToSocket(ip, Integer.valueOf(port), true, 10);
            m.serialize(System.out);
            System.out.println();
			}
			else
			{
				System.out.println("the split array returned " + argArray.length);
				System.out.println("For input " + args[0]);
			}
		}
	}
}