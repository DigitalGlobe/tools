//-----------------------------------------------------------------------------
// File:  OssimChipper.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java ossim-info app:
//
// Usage: java -Djava.library.path=<path_to_libossimjni-swig> -cp <path_to_ossimjni.jar> org.ossim.jni.apps.OssimChipper [options] <args> <...>
//
// Example:
// java -Djava.library.path=/work/osgeo/ossimjni/java/build/lib -cp /work/osgeo/ossimjni/java/build/lib/ossimjni.jar org.ossim.jni.apps.OssimChipper --op ortho <in.ntf> <out.tif>
//-----------------------------------------------------------------------------
// $Id$

package org.ossim.jni.apps;

import org.ossim.jni.Chipper;
import org.ossim.jni.Init;

public class OssimChipper
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
      newArgs[0] = "org.ossim.jni.apps.OssimChipper";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      int argc = Init.instance().initialize(newArgs.length, newArgs);

      // Note: need to test Init::initialize stripping args.
      if ( argc >= 1 )
      {
         org.ossim.jni.Chipper chipper = new org.ossim.jni.Chipper();

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

