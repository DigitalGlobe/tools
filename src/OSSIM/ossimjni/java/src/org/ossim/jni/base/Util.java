//-----------------------------------------------------------------------------
// File:  Util.java
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class for global utility methods.
//
//-----------------------------------------------------------------------------
// $Id: Util.java 19748 2011-06-12 15:34:56Z dburken $

package org.ossim.jni.base;

import java.io.File;
import java.lang.String;

public class Util
{
   /**
    * @briefMethod to change extension.
    *
    * Given: a file of "/data1/tif/point.tif" and an ext of "his" you get:
    * "/data1/tif/point.his" back.
    *
    * @note No '.' dot in the ext.
    */
   public static File replaceExtension( File f, String ext )
   {
      String s = f.getAbsolutePath();
      int dotPos = s.lastIndexOf(".");
      if ( dotPos > 0 )
      {
         String result = s.substring(0, dotPos+1);
         result = result + ext;
         return new File( result );
      }
      else
      {
         String result = s;
         result = result + "." +ext;
         return new File( result );
      }
   }
}
