package org.ossim.oms.apps;
import joms.oms.Init;
import joms.oms.CoordinateUtility;
import joms.oms.ossimGpt;

public class CoordinateUtilityTest
{
   public static void main(String[] args)
   {
      if(args.length != 3)
      {
         System.out.println("Need to have 3 argument <lat> <lon> precision");
         return;
      }
      CoordinateUtility coord = new CoordinateUtility();
      String[] newArgs = new String[0];
      Init.instance().initialize( newArgs.length, newArgs);
      double initialLat = Double.valueOf(args[0]);
      double initialLon = Double.valueOf(args[1]);
      int    precision  = Integer.valueOf(args[2]);
      
      if(precision > 5 || precision < 0)
      {
         System.out.println("Precision argument must be < 5 and >= 0");
         return;
      }
      String data = coord.convertLatLonToMgrs(initialLat, initialLon, precision);
      System.out.println( "MGRS: " + data );
      
      ossimGpt pt = coord.convertMgrsToLatLon(data);
      System.out.println("Lat, Lon: " + pt.latd() + ", " + pt.lond() );
      
      System.out.println("degrees = " + coord.dmsToDegrees("45 45 34.333"));
      System.out.println("lat dms ddmmss.sss C = " + coord.degreesToDms(initialLat, "ddmmss.sss C", true));
      System.out.println("lon dms ddmmss.sss C = " + coord.degreesToDms(initialLon, "ddmmss.sss C", true));
      System.out.println("lat dms dd C = " + coord.degreesToDms(initialLat, "dd C", true));
      System.out.println("lon dms ddd C = " + coord.degreesToDms(initialLon, "dd C", false));
   }

}