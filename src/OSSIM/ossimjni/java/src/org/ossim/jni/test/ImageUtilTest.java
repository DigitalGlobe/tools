//-----------------------------------------------------------------------------
// File:  ImageUtilTest.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java test/example app for ossimjni ImageUtil class.
//
//-----------------------------------------------------------------------------
// $Id: ImageUtilTest.java 21256 2012-07-07 22:38:16Z dburken $

package org.ossim.jni.test;

import org.ossim.jni.ImageUtil;
import org.ossim.jni.Init;
import java.io.File;

public class ImageUtilTest
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
      newArgs[0] = "org.ossim.jni.test.ImageUtilTest";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      int argc = Init.instance().initialize(newArgs.length, newArgs);

      if ( argc == 2 )
      {
         try
         {
            java.io.File file = new java.io.File(args[0]);
            String f = file.getAbsolutePath();

            String s = null;
            
            ImageUtil imageUtil = new org.ossim.jni.ImageUtil();

            // Force overview rebuild.
            imageUtil.setRebuildOverviewsFlag( true );

            // Do a fast (partial tiles) histogram at same time.
            imageUtil.setCreateHistogramFastFlag( true );

            // Make overviews tiff with jpeg compressed tiles (note these are lossy).
            s = "jpeg";
            imageUtil.setCompressionType( s );

            // Compression level, 1 is worst, 100 best...
            s = "75";
            imageUtil.setCompressionQuality( s );

            // Run it:
            imageUtil.processFile( f );
         }
         catch( Exception e )
         {
            System.err.println("Caught Exception: " + e.getMessage());
         }
      }
      else
      {
         System.out.println( "Usage: org.ossim.jni.test.ImageUtilTest <file>");
      }
      
   } // End of main:
   
} // Matches: public class ImageUtilTest
