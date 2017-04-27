//-----------------------------------------------------------------------------
// File:  OssimChipper.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java ossim-chipper app:
//
// Usage examples:
//
// Typical ortho:
// 
// java -Djava.library.path=/work/osgeo/oms/joms -cp /work/osgeo/oms/lib/joms-$OSSIM_VERSION.jar org.ossim.oms.apps.OssimChipper --op ortho <in.ntf> <out.tif>
//
// Generate thumbnail:
// 
// java -Djava.library.path=/work/osgeo/oms/joms -cp /work/osgeo/oms/lib/joms-$OSSIM_VERSION.jar org.ossim.oms.apps.OssimChipper --op ortho --srs "EPSG:4326" --histogram-op auto-minmax --pad-thumbnail true --three-band-out --output-radiometry U8 -t 128 /data1/celtic/003/po_105216_blu_0000000.ntf /data1/celtic/003/outputs/tn-1.jpg
//
//-----------------------------------------------------------------------------
// $Id$

package org.ossim.oms.apps;

import joms.oms.Chipper;
import joms.oms.Init;

public class OssimChipper
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
      // Copy the args with app name for c++ initialize.
      String[] newArgs = new String[args.length + 1];
      newArgs[0] = "joms.oms.apps..OssimChipper";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      int argc = Init.instance().initialize(newArgs.length, newArgs);

      // Note: need to test Init::initialize stripping args.
      if ( argc >= 1 )
      {
         joms.oms.Chipper chipper = new joms.oms.Chipper();

         //---
         // Info::initialize returns true to  indicate the process should continue with execute.
         //---
         if ( chipper.initialize( newArgs.length, newArgs) )
         {
            try
            {
               chipper.execute();
            }
            catch( Exception e )
            {
               System.err.println("Caught Exception: " + e.getMessage());
            }
         }
      }
   }
}

