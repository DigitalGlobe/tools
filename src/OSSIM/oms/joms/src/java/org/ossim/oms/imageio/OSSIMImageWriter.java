//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java image writer interface to OSSIM image writer.
//
// $Id: Info.cpp 11646 2007-08-23 21:04:16Z dburken $
//----------------------------------------------------------------------------

package org.ossim.oms.imageio;

import java.io.IOException;

import javax.imageio.IIOImage;
import javax.imageio.ImageTypeSpecifier;
import javax.imageio.ImageWriteParam;
import javax.imageio.ImageWriter;
import javax.imageio.metadata.IIOMetadata;
import javax.imageio.spi.ImageWriterSpi;

public class OSSIMImageWriter extends ImageWriter
{

   protected OSSIMImageWriter(ImageWriterSpi originatingProvider)
   {
      super(originatingProvider);
      // TODO Auto-generated constructor stub
   }

   public IIOMetadata convertImageMetadata(IIOMetadata inData, ImageTypeSpecifier imageType, ImageWriteParam param)
   {
      // TODO Auto-generated method stub
      return null;
   }

   public IIOMetadata convertStreamMetadata(IIOMetadata inData, ImageWriteParam param)
   {
      // TODO Auto-generated method stub
      return null;
   }

   public IIOMetadata getDefaultImageMetadata(ImageTypeSpecifier imageType, ImageWriteParam param)
   {
      // TODO Auto-generated method stub
      return null;
   }

   public IIOMetadata getDefaultStreamMetadata(ImageWriteParam param)
   {
      // TODO Auto-generated method stub
      return null;
   }

   public void write(IIOMetadata streamMetadata, IIOImage image, ImageWriteParam param) throws IOException
   {
      // TODO Auto-generated method stub

   }
}
