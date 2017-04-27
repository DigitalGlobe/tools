//-----------------------------------------------------------------------------
// File:  InfoTest.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java test/example app for oms Info class.
//
// Example usage:
//
// $ java -Djava.library.path=/work/osgeo/oms/joms -cp /work/osgeo/oms/lib/joms-$OSSIM_VERSION.jar org.ossim.oms.apps.InfoTest /data1/test/data/public/tif/Clinton_IA.tif
//
//-----------------------------------------------------------------------------
// $Id$

package org.ossim.oms.apps;

import joms.oms.Info;
import joms.oms.Init;
import joms.oms.Keywordlist;
import joms.oms.KeywordlistIterator;
import joms.oms.StringPair;

import java.io.File;

public class InfoTest
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
      newArgs[0] = "joms.oms.apps.InfoTest";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      int argc = Init.instance().initialize(newArgs.length, newArgs);

      if ( argc == 2 )
      {
         try
         {
            java.io.File file = new java.io.File(args[0]);
            String f = file.getAbsolutePath();
            
            joms.oms.Info info = new joms.oms.Info();
            joms.oms.Keywordlist kwl = new joms.oms.Keywordlist();
            
            // Equivalent of: ossim-info --dno -i -p <file>
            info.getImageInfo(f,
                              false,  // dump
                              true,   // dump no overviews
                              true,   // geom
                              true,   // image
                              false,  // metadat
                              false,  // palette
                              kwl); 
            
            if ( kwl.size() > 0 )
            {
               // Print out the Keywordlist map:
               joms.oms.KeywordlistIterator iter = new joms.oms.KeywordlistIterator();
               iter = kwl.getIterator();

               if ( iter != null )
               {
                  joms.oms.StringPair pair = new joms.oms.StringPair();
                  
                  while ( !iter.end() )
                  {
                     iter.getKeyValue(pair);
                     System.out.println(pair.getKey() + ":" + pair.getValue());
                     iter.next();  
                  }
               }

               String prefix = new String();
               String key    = new String();
               String value  = new String();

               // Test the interface that takes a prefix and a key.
               prefix = "tiff.image0.";
               key    = "samples_per_pixel";
               value = kwl.findKey( prefix, key );
               
               System.out.println("kwl->findKey test:");
               System.out.println("prefix:       " + prefix);
               System.out.println("key:          " + key);
               System.out.println("return value: " + value);

               
               // Test the interface that takes a key only.
               key    = "tiff.image0.samples_per_pixel";
               value = kwl.findKey( key );
               
               System.out.println("kwl->findKey test:");
               System.out.println("key:          " + key);
               System.out.println("return value: " + value);

               kwl.clearMap();
            }
            else
            {
               System.err.println("Nothing in map for: " + file );
            }


            // Test interface that takes an entry.
            System.out.println("test getImageInfo that takes an entry...");
            
            if ( info.getImageInfo(f,
                                   1,  // Second entry.
                                   kwl) )
            {
               if ( kwl.size() > 0 )
               {
                  // Print out the Keywordlist map:
                  joms.oms.KeywordlistIterator iter = new joms.oms.KeywordlistIterator();
                  iter = kwl.getIterator();
                  
                  if ( iter != null )
                  {
                     joms.oms.StringPair pair = new joms.oms.StringPair();

                     while ( !iter.end() )
                     {
                        iter.getKeyValue(pair);
                        System.out.println(pair.getKey() + ":" + pair.getValue());
                        iter.next();
                        
                     }
                  }
               }
            }
         }
         catch( Exception e )
         {
            System.err.println("Caught Exception: " + e.getMessage());
         }
      }
      else
      {
         System.out.println( "Usage: joms.oms.test.InfoTest <file>");
      }
      
   } // End of main:
   
} // Matches: public class InfoTest
