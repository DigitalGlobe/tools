import org.ossim.planet.message.*;

class FlyToApp
{
   public FlyToApp()
   {
      
   }
   public static void main(String[] args)
   {
      if(args.length != 4)
      {
         System.out.println("USAGE: java FlyToApp <type> <ip> <port> <target receiver>"); 
         return;
      }
      String type = args[0];
      String ip   = args[1];
      String port = args[2];
      String target = args[3];
      
      Action action    = new Action(Action.Type.FLY_TO);
      
      AbstractObject view = null;
      
      if(type.equals("Camera"))
      {
         view = new Camera(-100, 35, 100000, 0, 0, 0, "relativeToGround");
      }
      else
      {
         view = new LookAt(-100, 35, 100000, 45, 45, 0, "clampToGround", 100000);
      }
      action.add(view);
      action.setTargetAttribute(target);
      Message m = new Message();
      m.add(action);
      System.out.println("Sending Message");
      XMLEncoder encoder = new XMLEncoder(System.out);
      encoder.writeObject(action);
      System.out.println();
     m.sendMessageToSocket(ip, Integer.valueOf(port), true, 45);
   }
}