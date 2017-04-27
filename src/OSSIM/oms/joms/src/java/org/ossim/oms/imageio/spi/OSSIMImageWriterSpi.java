//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java Service provider class for OSSIMImageWriter.  Registered
// with the IIORegistry, which uses them for format recognition and
// presentation of available format readers and writers.
//
// $Id: Info.cpp 11646 2007-08-23 21:04:16Z dburken $
//----------------------------------------------------------------------------

package org.ossim.oms.imageio.spi;


import java.io.IOException;
import java.util.Locale;
import javax.imageio.ImageTypeSpecifier;
import javax.imageio.ImageWriter;
import javax.imageio.spi.ImageWriterSpi;

public class OSSIMImageWriterSpi extends ImageWriterSpi
{

   public boolean canEncodeImage(ImageTypeSpecifier type)
   {
      // TODO Auto-generated method stub
      return false;
   }

   public ImageWriter createWriterInstance(Object extension) throws IOException
   {
      // TODO Auto-generated method stub
      return null;
   }

   public String getDescription(Locale locale)
   {
      // TODO Auto-generated method stub
      return null;
   }
}

