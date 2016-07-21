//-----------------------------------------------------------------------------
// File:  ImageModelTest.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java test/example app for oms ImageModel class.
//
// Usage:
//
// java -cp ${OSSIM_DEV_HOME}/oms/lib/joms.jar org.ossim.oms.apps.ImageModelTest <image>
//
// Where image sensor model is affected by elevation, e.g. RPC model.
//
//-----------------------------------------------------------------------------
// $Id$

package org.ossim.oms.apps;

import joms.oms.Init;
import joms.oms.ImageModel;
import java.io.File;

public class ImageModelTest
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
      // Initialize ossim stuff:
      Init.instance().initialize();

      if ( args.length == 1 )
      {
         try
         {
            java.io.File file = new java.io.File(args[0]);
            String f = file.getAbsolutePath();

            joms.oms.ImageModel imageSpaceModel = new joms.oms.ImageModel();
            if ( imageSpaceModel.setModelFromFile( f, 0 ) )
            {
               double upIsUp = imageSpaceModel.upIsUpRotation();
               
               System.out.println("Up is up rotation angle: " + upIsUp);
            }
            else
            {
               System.err.println("Error returned from: ImageModel::upIsUpRotation()");
            }
         }
         catch( Exception e )
         {
            System.err.println("Caught Exception: " + e.getMessage());
         }
      }
      else
      {
         System.out.println( "Usage: org.ossim.oms.ImageModelTest <file>");
      }
      
   } // End of main:
   
} // Matches: public class ImageModelTest
