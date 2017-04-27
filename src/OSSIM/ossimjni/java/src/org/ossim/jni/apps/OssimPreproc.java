//-----------------------------------------------------------------------------
// File:  OssimImageUtil.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java ossim-preproc app:
//
//-----------------------------------------------------------------------------
// $Id: OssimPreproc.java 21254 2012-07-07 22:08:10Z dburken $

package org.ossim.jni.apps;

import org.ossim.jni.ImageUtil;
import org.ossim.jni.Init;

public class OssimPreproc
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
      newArgs[0] = "org.ossim.jni.apps.OssimPreproc";
      System.arraycopy(args, 0, newArgs, 1, args.length);

      // Initialize ossim stuff:
      int argc = Init.instance().initialize(newArgs.length, newArgs);

      // Note: need to test Init::initialize stripping args.
      if ( argc >= 1 )
      {
         org.ossim.jni.ImageUtil imageUtil = new org.ossim.jni.ImageUtil();

         //---
         // ImageUtil::initialize returns true to  indicate the process should continue
         // with execute.
         //---
         if ( imageUtil.initialize( newArgs.length, newArgs) )
         {
            try
            {
               imageUtil.execute();
            }
            catch( Exception e )
            {
               System.err.println("Caught Exception: " + e.getMessage());
            }
         }
      }
   }
}

