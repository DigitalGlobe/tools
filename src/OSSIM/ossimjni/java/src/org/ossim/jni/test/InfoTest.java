//-----------------------------------------------------------------------------
// File:  InfoTest.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java test/example app for ossimjni Info class.
//
// Usage: java -Djava.library.path=<path_to_libossimjni-swig> -cp <path_to_ossimjni.jar> org.ossim.jni.test.InfoTest <image>
//
// Example:
// java -Djava.library.path=/work/osgeo/ossimjni/java/build/lib -cp /work/osgeo/ossimjni/java/build/lib/ossimjni.jar org.ossim.jni.test.InfoTest a.toc
// 
//-----------------------------------------------------------------------------
// $Id: InfoTest.java 23591 2015-10-21 13:14:26Z dburken $

package org.ossim.jni.test;

import org.ossim.jni.Info;
import org.ossim.jni.Init;
import org.ossim.jni.Keywordlist;
import org.ossim.jni.KeywordlistIterator;
import org.ossim.jni.StringPair;

import java.io.File;

public class InfoTest
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
      newArgs[0] = "org.ossim.jni.apps.InfoTest";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      int argc = Init.instance().initialize(newArgs.length, newArgs);

      if ( argc == 2 )
      {
         try
         {
            java.io.File file = new java.io.File(args[0]);
            String f = file.getAbsolutePath();
            
            org.ossim.jni.Info info = new org.ossim.jni.Info();

            String buildDate = new String();
            String version   = new String();
            String revision  = new String();

            buildDate = info.getOssimBuildDate();
            version   = info.getOssimVersion();
            revision  = info.getOssimRevisionNumber();

            System.out.println("buildDate: " + buildDate);
            System.out.println("version:   " + version);
            System.out.println("revision:  " + revision);
            
            org.ossim.jni.Keywordlist kwl = new org.ossim.jni.Keywordlist();
            
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
               org.ossim.jni.KeywordlistIterator iter = new org.ossim.jni.KeywordlistIterator();
               iter = kwl.getIterator();

               if ( iter != null )
               {
                  org.ossim.jni.StringPair pair = new org.ossim.jni.StringPair();
                  
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
                  org.ossim.jni.KeywordlistIterator iter = new org.ossim.jni.KeywordlistIterator();
                  iter = kwl.getIterator();
                  
                  if ( iter != null )
                  {
                     org.ossim.jni.StringPair pair = new org.ossim.jni.StringPair();

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
         System.out.println( "Usage: org.ossim.jni.test.InfoTest <file>");
      }
      
   } // End of main:
   
} // Matches: public class InfoTest
