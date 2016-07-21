//-----------------------------------------------------------------------------
// File:  ChipperTest.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java test/example app for ossimjni Info class.
//
// Usage: java -Djava.library.path=<path_to_libossimjni-swig> -cp <path_to_ossimjni.jar> org.ossim.jni.test.ChipperTest <image>
//
// Example:
// java -Djava.library.path=/work/osgeo/ossimjni/java/build/lib -cp /work/osgeo/ossimjni/java/build/lib/ossimjni.jar org.ossim.jni.test.ChipperTest
// 
//-----------------------------------------------------------------------------
// $Id: ChipperTest.java 22320 2013-07-19 15:21:03Z dburken $

package org.ossim.jni.test;

import org.ossim.jni.Chipper;
import org.ossim.jni.ImageData;
import org.ossim.jni.Init;
import org.ossim.jni.Keywordlist;
import org.ossim.jni.KeywordlistIterator;
import org.ossim.jni.StringPair;

import java.io.File;

public class ChipperTest
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
      newArgs[0] = "org.ossim.jni.apps.ChipperTest";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      int argc = Init.instance().initialize(newArgs.length, newArgs);

      if ( argc == 1 )
      {
         try
         {
            org.ossim.jni.Chipper chipper = new org.ossim.jni.Chipper();
            org.ossim.jni.ImageData chip  = new org.ossim.jni.ImageData();
            org.ossim.jni.Keywordlist kwl = new org.ossim.jni.Keywordlist();

            String key    = null;
            String file   = "test1.ras";
            String value  = null;
 
            key = "thumbnail_resolution";
            value = "128";
            kwl.addPair( key, value );

            key = "three_band_out";
            value = "true";
            kwl.addPair( key, value );
            
            key = "hist-op";
            value = "auto-minmax";
            kwl.addPair( key, value );
          
            key = "image0.file";
            value = "/data1/space_coast_metric_private/po_176062_pan_0000000.tif";
            kwl.addPair( key, value );
            
            key = "operation";
            value = "ortho";
            kwl.addPair( key, value );
              
            key = "output_radiometry";
            value = "U8";
            kwl.addPair( key, value );

            /*
            String key    = null;
            String file   = "test1.ras";
            String value  = null;
            key = "cut_center_height";
            value = "512";
            kwl.addPair( key, value );

            key = "cut_center_width";
            value = "512";
            kwl.addPair( key, value );

            key = "cut_center_latitude";
            value = "-42.883986392005788";
            kwl.addPair( key, value );
            
            key = "cut_center_longitude";
            value = "147.331309643650911";
            kwl.addPair( key, value );
            
            key = "hist-op";
            value = "auto-minmax";
            kwl.addPair( key, value );
          
            key = "image0.file";
            value = "/data1/test/data/geoeye1/GE1_Hobart_GeoStereo_NITF-NCD/001508507_01000SP00332258/5V090205P0001912264B220000100282M_001508507/Volume1/5V090205P0001912264B220000100282M_001508507.ntf";
            kwl.addPair( key, value );
            
            key = "operation";
            value = "chip";
            kwl.addPair( key, value );
              
            key = "scale_2_8_bit";
            value = "true";
            kwl.addPair( key, value );

            key = "up_is_up";
            value = "true";
            kwl.addPair( key, value );
            */
            if ( chipper.initialize( kwl ) )
            {
               //---
               // Return status:
               // OSSIM_STATUS_UNKNOWN = 0,
               // OSSIM_NULL           = 1, not initialized
               // OSSIM_EMPTY          = 2, initialized but blank or empty
               // OSSIM_PARTIAL        = 3, contains some null/invalid values
               // OSSIM_FULL           = 4  all valid data
               //---
               int status = chipper.getChip( chip );
               
               if ( ( status == 3 ) || ( status == 4 ) )
               {
                  if ( chip.write( file ) )
                  {
                     System.out.println( "Wrote: " + file );
                  }
                  else
                  {
                     System.err.println("ImageData::write failed for file: " + file);
                  }
               }
               else
               {
                  System.err.println("ImageData::getChip failed!");
                  String statusString = null;
                  if ( status == 0)
                  {
                     statusString = "unknown";
                  }
                  else if ( status == 1 )
                  {
                     statusString = "null";
                  }
                  else if ( status == 2 )
                  {
                     statusString = "empty";
                  }
                  System.err.println("Return status: " + statusString);
               }
            }
            else
            {
               System.err.println("ImageData::initialize failed!");
            }
         }
         catch( Exception e )
         {
            System.err.println("Caught Exception: " + e.getMessage());
         }
      }
      else
      {
         System.out.println( "Usage: org.ossim.jni.test.ChipperTest <file>");
      }
      
   } // End of main:
   
} // Matches: public class ChipperTest
