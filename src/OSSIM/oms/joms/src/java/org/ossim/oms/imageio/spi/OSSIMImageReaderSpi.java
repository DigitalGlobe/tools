//-----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Java Service Provider Interface (SPI) class for
// OSSIMImageReader.  Registered
// with the IIORegistry, which uses them for format recognition and
// presentation of available format readers and writers.
//
// $Id: Info.cpp 11646 2007-08-23 21:04:16Z dburken $
//-----------------------------------------------------------------------------

package org.ossim.oms.imageio.spi;

import java.io.IOException;
import java.io.File;
import java.util.Locale;
import javax.imageio.ImageReader;
import javax.imageio.spi.ImageReaderSpi;

import joms.oms.DataInfo;

import org.ossim.oms.imageio.OSSIMImageReader;

public class OSSIMImageReaderSpi extends ImageReaderSpi
{
    public static String description = "OSSIM/Java ImageIO Plugin";
    
    private static final String[] names = {
	    "nitf", "TIF", "tiff", "TIFF"
    };

    private static final String[] suffixes = {
	    "tif", "tiff", "nitf"
    };

    private static final String[] MIMETypes = {
	"image/tiff"
    };

    private static final String readerClassName = "org.ossim.oms.imageio.OSSIMImageReader";

    private static final String[] writerSpiNames = {
	"org.ossim.oms.imageio.spi.OSSIMImageWriterSpi"
    };

    private static final Class[] inputTypes = new Class[] {
	File.class
    };

    public OSSIMImageReaderSpi()
    {
	super( "OSSIM", "1.8.0", names, suffixes, MIMETypes, readerClassName,
		inputTypes, writerSpiNames, false, null, null, null, null,
		true, null, null, null, null );
    }

    public boolean canDecodeInput( Object input ) throws IOException
    {
	boolean status = false;
	
	if ( input instanceof File  )
	{
	    DataInfo dataInfo = new DataInfo();
	    File file = (File)input;
	    
	    dataInfo.open( file.getAbsolutePath() );
	    
	    String info = dataInfo.getInfo();

	    if ( info != null )
	    {
		status = true;
	    }
	}

	return status; 
    }

    public ImageReader createReaderInstance( Object extension )
	    throws IOException
    {
	return new OSSIMImageReader(this);
    }

    public String getDescription( Locale locale )
    {
	return description;
    }

}
