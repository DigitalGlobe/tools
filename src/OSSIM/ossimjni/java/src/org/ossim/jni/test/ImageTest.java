//-----------------------------------------------------------------------------
// File:  ImageTest.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Test app for Image class.
//
//-----------------------------------------------------------------------------
// $Id: ImageTest.java 20188 2011-10-23 23:13:30Z dburken $

package org.ossim.jni.test;

import org.ossim.jni.base.Util;
import org.ossim.jni.Init;
import org.ossim.jni.core.Image;

import java.io.File;

public class ImageTest
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
      if ( args.length > 0 )
      {
         try
         {
            java.io.File imgFile = new java.io.File(args[0]);
            Image img = new Image();
            if ( img.open( imgFile ) )
            {
               System.out.println( "Opened: " + imgFile );
               int[] bands = img.getBandSelection();
               if ( bands != null )
               {
                  for ( int i = 0; i < bands.length; ++i )
                  {
                     System.out.println( "band[" + i + "]: " + bands[i] );
                  }
                  // Reverse for test:
                  System.out.println( "Reversing bands..." );
                  for ( int i = 0; i < bands.length; ++i)
                  {
                     bands[i] = bands.length-(i+1);
                  }
                  img.selectBands(bands);
               }
               else
               {
                  System.out.println( "Image.getBandSelection() returned null...");
               }

               bands = img.getBandSelection();
               if ( bands != null )
               {
                  for ( int i = 0; i < bands.length; ++i )
                  {
                     System.out.println( "band[" + i + "]: " + bands[i] );
                  }
               }
               else
               {
                  System.out.println( "Image.getBandSelection() returned null...");
               }

               // Set the histogram:
               String ext = new String("his");
               File his = Util.replaceExtension( imgFile, ext );
               System.out.println( "Histogram file: " + his );
               if ( img.setHistogram( his ) )
               {
                  java.io.File his2 = img.getHistogramFile();
                  System.out.println( "Image.getHistogramFile() result: " + his2 );
               }
               else
               {
                  System.out.println( "Image.getHistogramFile() returned false!");
               }

               // Set the overview:

               // Ovr in standard place.
               ext = "ovr";
               File ovr = Util.replaceExtension( imgFile, ext );

               // Test for non-standard place:
               // File ovr = new File( "/tmp/point-tif-ovr.ovr");
               System.out.println( "Overview file: " + ovr );
               if ( img.setOverview( ovr ) )
               {
                  java.io.File ovr2 = img.getOverviewFile();
                  System.out.println( "Image.getOverviewFile() result: " + ovr2 );
               }
               else
               {
                  System.out.println( "Image.getOverviewFile() returned false!");
               }
            }
            else
            {
               System.out.println( "Could not open: " + imgFile );
            }
            
         }
         catch( Exception e )
         {
            System.err.println("Caught Exception: " + e.getMessage());
         }
      }
      else
      {
         System.out.println( "Usage: org.ossim.oms.apps.ImageTest <image>" );
      }
   } // end of main
}

