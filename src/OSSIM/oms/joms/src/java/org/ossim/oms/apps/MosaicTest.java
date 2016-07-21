package org.ossim.oms.apps;
import joms.oms.Init;
import joms.oms.Mosaic;

public class MosaicTest
   {
      /**
       * @param args
       */
      public static void main( String[] args )
      {
         String[] newArgs = new String[0];
         
         Init.instance().initialize( newArgs.length, newArgs);
         
         if ( args.length > 0 )
         {
            for ( int i = 0; i < args.length; i++ )
            {
               Mosaic mosaic = new Mosaic();
               
               mosaic.setupOrthoThumbnail(args[i],
                                        "0",
                                        "",
                                        256,
                                        "");
               mosaic.writeImageToFile("/tmp/test.jpg", "");
            }
         }
      }
      
   }
