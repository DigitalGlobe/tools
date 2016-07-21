package org.ossim.oms.apps;
import joms.oms.Init;
import joms.oms.DataInfo;
import joms.oms.ossimNmeaMessage;
import joms.oms.ossimNmeaMessageSequencer;
import joms.oms.ossimFilename;
import java.io.FileInputStream;
import java.io.File;

public class NmeaMessageTest
{
   /**
    * @param args
    */
   public static void main( String[] args )
   {
      String[] newArgs = new String[0];
      
      Init.instance().initialize( newArgs.length, newArgs);
      ossimNmeaMessage message = new ossimNmeaMessage();
      if ( args.length > 0 )
      {
         ossimFilename file = new ossimFilename(args[0]);
         
         ossimNmeaMessageSequencer msgSequence = new ossimNmeaMessageSequencer(file);
         boolean done = false;
         do 
         {
            if(msgSequence.next(message))
            {
               if(message.validCheckSum())
               {
                  System.out.println("Valid: " + message.message());
               }
               else
               {
                  System.out.println("INVALID: " + message.message());
               }
            }
            else
            {
               done = !msgSequence.valid();
            }
         } while (!done);
      }
   }
}
