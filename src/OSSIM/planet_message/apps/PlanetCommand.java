import java.io.*;
import java.net.*;
import java.util.Date;
import org.ossim.planet.message.*;

public class PlanetCommand 
{

   public static void main(String[] args) 
   {
      // need format PlanetCommand <ip> <port> <portType> <Message>
      //
      String ip;
      String port;
      String portType;
      String message;
      
      if(args.length == 4)
      {
         ip = args[0];
         port = args[1];
         portType = args[2];
         message = args[3];
      }
      else if(args.length == 3)
      {
         ip       = args[0];
         port     = args[1];
         portType = "tcp";
         message  = args[2];
      }
      else
      {
         System.out.println("Usage: java PlanetCommand <ip> <port> <portType> <message>");
         System.out.println("or : java PlanetCommand <ip> <port> <message>");
         System.out.println("where:");
         System.out.println("   <portType> can be udp or tcp");
         System.out.println("   <message> Is a single argument enclosed in quotes so it's treated as a single argument");
         System.out.println("the 3 argument form will assume tcp");
         
         return;
      }
      StringMessage m = new StringMessage(message);
      m.sendToSocket(ip, Integer.valueOf(port), portType, 30);
   }
}
