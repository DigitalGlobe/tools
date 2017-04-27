package org.ossim.oms.apps;

import joms.oms.Init;

public class TestGetHeight
{
   static
   {
      System.loadLibrary( "joms" );
   }
   
   /**
    * @param args
    */
   public static void main( String[] args )
   {
      Init.instance().initialize( args.length, args );
      
      /*
      Height hgt = new Height();
      
      double lat = 28.0;
      double lon = -81.0;
      Float64  h = new Float64();
      
      hgt.getGeoidOffset(lat, lon, h);
      System.out.println("lat = " + lat);
      System.out.println("lon = " + lon);
      System.out.println("geoid offset:  " + h.value());
      
      hgt.getHeightAboveMsl(lat, lon, h);
      System.out.println("height above msl:  " + h.value());
      
      hgt.getHeightAboveEllipsoid(lat, lon, h);
      System.out.println("height above ellipsoid:  " + h.value());

      String s = new String();
      s = hgt.getCellName(lat, lon);
      System.out.println("cell name:  " + s);
      */
   }
}
