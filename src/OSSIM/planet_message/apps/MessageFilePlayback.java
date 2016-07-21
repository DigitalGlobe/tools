import java.io.*;
import java.net.*;
import java.util.Date;
import org.ossim.planet.message.*;

public class MessageFilePlayback
{
   public static void main(String[] args)
   {
      String ip;
      String port;
      String portType;
      String filename;
      
      if(args.length == 4)
      {
         String line;
         ip = args[0];
         port = args[1];
         portType = args[2];
         filename = args[3];
         try
         {
            BufferedReader input = new BufferedReader(new FileReader(filename));
            try
            {
               while (( line = input.readLine()) != null)
               {
                  StringMessage m = new StringMessage(line);
                  m.sendToSocket(ip, Integer.valueOf(port), portType, 30000);
               }
            }
            finally 
            {
               input.close();
            }
         }
         catch (IOException ex){
            ex.printStackTrace();
         }
      }
      else
      {
         System.out.println("Usage: java PlanetCommand <ip> <port> <portType> <message file>");
         System.out.println("   <ip> ip of the planet you wish to send messages to");
         System.out.println("   <port> port number of the listening planet");
         System.out.println("   <portType> can be udp or tcp");
         System.out.println("   <message file> file that contains serialized message lines.  One line is a complete message");
      }
   }
}
