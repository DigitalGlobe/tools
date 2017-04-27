//-----------------------------------------------------------------------------
// File:  ImageTest.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Test app for Image class.
//
// Usage example:
// $ java -Djava.library.path=/work/osgeo/ossimjni/java/build/lib -cp
// /work/osgeo/ossimjni/java/build/lib/ossimjni.jar org.ossim.jni.test.ElevMgrTest
// <latitude> <longitude> 
//
//-----------------------------------------------------------------------------
// $Id: ElevMgrTest.java 22506 2013-12-09 19:40:49Z dburken $

package org.ossim.jni.test;

import org.ossim.jni.ElevMgr;
import org.ossim.jni.Init;
/* import org.ossim.jni.StringVector; */
import java.lang.Double;

public class ElevMgrTest
{
   static
   {
      System.loadLibrary( "ossimjni-swig" );
   }
   
   /**
    * @param args
    */
   public static void main( String[] args )
   {
      // Copy the args with app name for c++ initialize.
      String[] newArgs = new String[args.length + 1];
      newArgs[0] = "org.ossim.oms.apps.ImageTest";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      Init.instance().initialize(newArgs.length, newArgs);

      // Note: need to test Init::initialize stripping args.
      if ( args.length == 2 )
      {
         try
         {
            double lat = Double.parseDouble( args[0] );
            double lon = Double.parseDouble( args[1] );
            double hEllips = ElevMgr.instance().getHeightAboveEllipsoid( lat, lon );
            double hMsl    = ElevMgr.instance().getHeightAboveMSL( lat, lon );            

            System.out.println( "latitude:            " + lat );
            System.out.println( "longitude:           " + lon );
            System.out.println( "Height of ellipsoid: " + hEllips );
            System.out.println( "Height of MSL:       " + hMsl );

            /*
             * Uncomment and edit to test getCellsForBounds call.  
            String connectionString = "/data1/elevation/srtm/3arc";
            double minLat = -42.9407659877087;
            double minLon = 147.165822845586;
            double maxLat = -42.7826589063266;
            double maxLon = 147.353043713048;
            org.ossim.jni.StringVector cells = new org.ossim.jni.StringVector();
            ElevMgr.instance().getCellsForBounds( connectionString,
                                                       minLat,
                                                       minLon,
                                                       maxLat,
                                                       maxLon,
                                                       cells );
            for ( int i = 0; i < cells.size(); ++i )
            {
               System.out.println("cell[" + i + "]: " + cells.get(i));
            }
            */
         }
         catch( Exception e )
         {
            System.err.println("Caught Exception: " + e.getMessage());
         }
      }
      else
      {
         System.out.println( "Usage: org.ossim.oms.test.ElevMgrTest <latitude> <longitude>" );
      }
   } // end of main
}

