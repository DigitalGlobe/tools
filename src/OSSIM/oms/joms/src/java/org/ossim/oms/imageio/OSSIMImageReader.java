//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java image writer for OSSIM image handler.
//
// $Id: Info.cpp 11646 2007-08-23 21:04:16Z dburken $
//----------------------------------------------------------------------------
package org.ossim.oms.imageio;

import java.awt.image.BufferedImage;
import java.awt.image.RenderedImage;
import java.io.File;
import java.io.IOException;
import java.util.Iterator;

import javax.imageio.ImageReadParam;
import javax.imageio.ImageTypeSpecifier;
import javax.imageio.metadata.IIOMetadata;
import javax.imageio.spi.ImageReaderSpi;

import org.ossim.oms.image.omsImageSource;
import org.ossim.oms.image.omsRenderedImage;

import joms.oms.ossimImageHandler;
import joms.oms.ossimImageHandlerRegistry;
import joms.oms.ossimFilename;

public class OSSIMImageReader extends javax.imageio.ImageReader
{
    private ossimImageHandler imageHandler;
    private omsImageSource imageSource;

    // OMS Image Handler.
    // joms.oms.ImageReader imageReader = null;

    public OSSIMImageReader( ImageReaderSpi originatingProvider )
    {
	super( originatingProvider );
    }

    public void setInput( Object input )
    {
	if ( input != null && input instanceof File )
	{
	    File file = (File) input;

      imageHandler = ossimImageHandlerRegistry.instance().open(new ossimFilename(file.toString()));

	    if ( imageHandler != null)
	    {
          imageSource = new omsImageSource( imageHandler );
	    }
	}
    }

    public int getHeight( int imageIndex ) throws IOException
    {
	int height = 0;

	if ( imageHandler != null )
	{
	    long entry = imageHandler.getCurrentEntry();

	    imageHandler.setCurrentEntry( imageIndex );
	    height = (int)imageHandler.getBoundingRect().getHeight();
	    imageHandler.setCurrentEntry( entry );
	}

	return height;
    }

    public IIOMetadata getImageMetadata( int imageIndex ) throws IOException
    {
	// TODO Auto-generated method stub
	return null;
    }

    public Iterator<ImageTypeSpecifier> getImageTypes( int imageIndex )
	    throws IOException
    {
	// TODO Auto-generated method stub
	return null;
    }

    public int getNumImages( boolean allowSearch ) throws IOException
    {	
	return (int) ( (imageHandler != null) ? imageHandler.getNumberOfEntries() : 0 );
    }

    public IIOMetadata getStreamMetadata() throws IOException
    {
	// TODO Auto-generated method stub
	return null;
    }

    public int getWidth( int imageIndex ) throws IOException
    {
	int width = 0;

	if ( imageHandler != null )
	{
	    long entry = imageHandler.getCurrentEntry();

	    imageHandler.setCurrentEntry( imageIndex );
	    width = (int)imageHandler.getBoundingRect().getWidth();
	    imageHandler.setCurrentEntry( entry );
	}

	return width;    
	}

    public BufferedImage read( int imageIndex, ImageReadParam param )
	    throws IOException
    {
	// TODO Auto-generated method stub
	return null;
    }

    public RenderedImage readAsRenderedImage( int imageIndex,
	    ImageReadParam param ) throws IOException
    {
	
	imageSource = new omsImageSource( imageHandler );

	return new omsRenderedImage( imageSource );
    }

}
