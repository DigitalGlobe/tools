//-----------------------------------------------------------------------------
// File:  Foo.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java foo/test app.  Please do not commit your code.
//
//-----------------------------------------------------------------------------
// $Id: Foo.java 20188 2011-10-23 23:13:30Z dburken $

package org.ossim.jni.test;

// import java.io.File;
// import java.lang.String;
// import org.ossim.jni.base.Util;

public class Foo
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
      try
      {
         // Your code here...
      }
      catch( Exception e )
      {
         System.err.println("Caught Exception: " + e.getMessage());
      }
   }
}

