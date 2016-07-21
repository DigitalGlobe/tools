package org.ossim.oms.apps;

import java.awt.image.RenderedImage;
import java.io.File;
import java.io.IOException;

import org.ossim.oms.util.ImageUtil;

import joms.oms.Init;

public class omsImageViewer
{

	/**
	 * @param args
	 * @throws IOException
	 */
	public static void main( String[] args ) throws IOException
	{
		Init.instance().initialize( args.length, args );

		ImageUtil.printReaderNames();

		if ( args.length > 0 )
		{
			for ( int i = 0; i < args.length; i++ )
			{
				File imageFile = new File( args[i] );
				RenderedImage image = ImageUtil.loadImage( imageFile );
				//RenderedImage image = ImageUtil.loadImageWithStdProjChain( imageFile );

				if ( image != null )
				{
					
					ImageUtil.printInfo(image);
					ImageUtil.displayImage( imageFile.getAbsolutePath(), image );
				}
			}
		}
		else
			System.err.println( "Must specify at least one filename" );
	}
}
