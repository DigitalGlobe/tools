//-----------------------------------------------------------------------------
// File:  OssimInfo.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java ossim-info app:
//
// Example usage:
//
// java -Djava.library.path=/work/osgeo/oms/joms -cp /work/osgeo/oms/lib/joms-$OSSIM_VERSION.jar org.ossim.oms.apps.OssimInfo /data1/test/data/public/tif/Clinton_IA.tif
//
//-----------------------------------------------------------------------------
// $Id$

package org.ossim.oms.apps;

import joms.oms.Info;
import joms.oms.Init;

public class OssimInfo
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
      newArgs[0] = "joms.oms.apps.OssimInfo";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      int argc = Init.instance().initialize(newArgs.length, newArgs);

      // Note: need to test Init::initialize stripping args.
      if ( argc >= 1 )
      {
         joms.oms.Info info = new joms.oms.Info();

         //---
         // Info::initialize returns true to  indicate the process should continue with execute.
         //---
         if ( info.initialize( newArgs.length, newArgs) )
         {
            try
            {
               info.execute();
            }
            catch( Exception e )
            {
               System.err.println("Caught Exception: " + e.getMessage());
            }
         }
      }
   }
}

