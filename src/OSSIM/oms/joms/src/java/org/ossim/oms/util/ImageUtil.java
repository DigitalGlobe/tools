package org.ossim.oms.util;

import java.awt.Dimension;
import java.awt.image.RenderedImage;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.imageio.ImageIO;
import javax.imageio.ImageReader;
import javax.imageio.stream.ImageInputStream;
import javax.swing.JFrame;
import javax.swing.JDialog;
import javax.swing.JScrollPane;

import joms.oms.ossimImageChain;
import joms.oms.ossimImageHandler;
import joms.oms.ossimImageHandlerRegistry;
import joms.oms.ossimObjectFactory;

import org.ossim.oms.image.omsImageSource;
import org.ossim.oms.image.omsRenderedImage;
import org.ossim.oms.ui.ImagePanel;

public class ImageUtil
{
	public static void printReaderNames()
	{
		String[] readerNames = ImageIO.getReaderFormatNames();

		System.out.println( "readers: " + Arrays.asList( readerNames ) );
	}

	public static void displayImage( String title, RenderedImage image )
	{
		JFrame frame = new JFrame( title );
		ImagePanel display = new ImagePanel( image );
		JScrollPane scrollPane = new JScrollPane( display );

		display.setImage( image );
		scrollPane.setPreferredSize( new Dimension( 512, 512 ) );
		frame.getContentPane().add( scrollPane );
		frame.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
		frame.pack();
		frame.setVisible( true );
	}

	public static void displayImageModal( String title, RenderedImage image )
	{
		JFrame frame = new JFrame( title );
		JDialog dialog = new JDialog( frame, title, true );
		dialog.setDefaultCloseOperation( JDialog.DISPOSE_ON_CLOSE );

		ImagePanel display = new ImagePanel( image );
		JScrollPane scrollPane = new JScrollPane( display );

		display.setImage( image );
		scrollPane.setPreferredSize( new Dimension( 512, 512 ) );
		dialog.getContentPane().add( scrollPane );
		frame.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
		dialog.pack();
		// frame.setVisible( true );
		dialog.setLocationRelativeTo( null );
		dialog.setSize( 512, 512 );
		dialog.setVisible( true );
		frame.setVisible( false );
		frame.dispose();
	}

	public static RenderedImage loadImage( File imageFile ) throws IOException
	{
		Iterator<ImageReader> readers = ImageIO.getImageReaders( imageFile );
		RenderedImage image = null;

		while ( readers.hasNext() && image == null )
		{
			ImageReader reader = readers.next();

			reader.setInput( imageFile );

			image = reader.readAsRenderedImage( 0, reader.getDefaultReadParam() );

			System.out.println( image.getClass().getName() );
		}

		if ( image == null )
			image = loadImage( (Object)imageFile );

		return image;
	}

	public static RenderedImage loadImage( Object input ) throws IOException
	{
		ImageInputStream istream = ImageIO.createImageInputStream( input );
		Iterator<ImageReader> readers = ImageIO.getImageReaders( istream );
		RenderedImage image = null;

		while ( readers.hasNext() && image == null )
		{
			ImageReader reader = readers.next();

			reader.setInput( istream );

			image = reader.readAsRenderedImage( 0, reader.getDefaultReadParam() );
		}

		if ( image == null )
			System.err.println( "No reader for " + input );

		return image;
	}

	public static RenderedImage loadImageWithStdProjChain( Object input )
			throws IOException
	{
		RenderedImage image = null;

		ossimImageChain chain = new ossimImageChain();

		if ( input != null )
		{
			String filename = null;

			if ( input instanceof String )
				filename = (String)input;
			else if ( input instanceof File )
				filename = ( (File)input ).getAbsolutePath();
			else
				throw new IOException( "cannot determine filename" );

			ossimImageHandler handler = ossimImageHandlerRegistry.instance().open(
					filename );

			if ( handler != null )
			{
				chain.addInput( handler );

				if ( handler.getNumberOfInputBands() > 1 )
				{
					chain.addInput( null );
				}
			}
		}
		else
			throw new IOException( "input cannot be null" );

		image = new omsRenderedImage( new omsImageSource( chain ) );

		return image;
	}

	public static void printInfo( RenderedImage image )
	{
		Map<String, Object> info = new HashMap<String, Object>();

		info.put( "className", image.getClass().getName() );
		info.put( "width", image.getWidth() );
		info.put( "height", image.getWidth() );
		info.put( "tileWidth", image.getTileWidth() );
		info.put( "tileHeight", image.getTileHeight() );
		info.put( "numXTiles", image.getNumXTiles() );
		info.put( "numYTiles", image.getNumYTiles() );

		System.out.println( info );
	}
}
