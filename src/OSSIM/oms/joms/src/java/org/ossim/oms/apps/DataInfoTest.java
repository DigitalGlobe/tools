//---
// 
// File: DataInfoTest.java
//
// Test app for swig generated DataInfo.java class and associated
// coms c++ class oms::DataInfo.  Basically if this runs your oms/joms wrappers for
// omar should work.
//
// Calls usually go back and forth like this:
// omar<-->joms<-->oms<-->ossim
//
// Requires: joms.jar and libjoms.so(unux's/linux) or libjoms.dylib(OSX)
//
// Use java args, -cp and -Djava.library.path as needed.
// -cp is CLASSPATH to joms.jar
// -Djava.library.path points to directory where libjoms.so or .dylib resides.
//
// Typically:
//
// joms.jar is placed in omar/plugins/omar-oms/lib
// libjoms.so or libjoms.dylib is in LD_LIBRARY_PATH or DYLD_LIBRARY_PATH
// 
// Usages:
//
// Both CLASSPATH and (DY)LD_LIBRARY_PATH set:
// 
// java org.ossim.oms.apps.DataInfoTest <image(s)>
//
// No CLASSPATH, (DY)LD_LIBRARY_PATH set:");
//
// java -cp $OMAR_HOME/../../plugins/omar-oms/lib/joms.jar org.ossim.oms.apps.DataInfoTest <image(s)>
//
// No CLASSPATH, No (DY)LD_LIBRARY_PATH set:");
// Note: /path/to/lib should have libjoms.so or libjoms.dylib in it.");
//
// java -Djava.library.path=/path/to/lib -cp $OMAR_HOME/../../plugins/omar-oms/lib/joms.jar org.ossim.oms.apps.DataInfoTest <image(s)>
// 
//---


package org.ossim.oms.apps;
import joms.oms.Init;
import joms.oms.DataInfo;

public class DataInfoTest
{
   public static void printInfo( DataInfo app, String filename )
   {
      if ( app.open( filename ) )
      {
         System.out.println( app.getInfo() );
         app.close();
      }
      else
      {
         System.out.println("DataInfoTest::printInfo Could not open: " + filename);
      }
   }

   /**
    * @param args
    */
   public static void main( String[] args )
   {
      DataInfo app = new DataInfo();
      String[] newArgs = new String[0];
      
      Init.instance().initialize( newArgs.length, newArgs);
      
      if ( args.length > 0 )
      {
         int idx = 0;
         // this block is for testing an open and close to make sure all resources are freed up
         // we will do this 3000 times.
         //
//      System.out.println("------------------Testing 3000 opens--------------------------------");
//      for(idx = 0; idx < 3000; ++idx)
//      {
//         //app = new DataInfo();
//         app.open(args[0]);
//         if(idx == 2999)
//         {
//            System.out.println("Printing Information");
//            System.out.println( app.getInfo() );
//         }
//         app.close();
//      }
//      System.out.println("------------------Doing a simple printing of data info-------------");
         // this block of coe is for printing a data info
         for ( idx = 0; idx < args.length; ++idx )
         {   
            DataInfoTest.printInfo( app, args[idx] );
         }
      }
      else
      {
         System.out.println("Usage: java org.ossim.oms.apps.DataInfoTest <image(s)>");
      }
   }
}
