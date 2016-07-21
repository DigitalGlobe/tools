package org.ossim.oms.apps;

import java.awt.Point;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.awt.image.DataBuffer;
import java.awt.image.DataBufferByte;
import java.awt.image.WritableRaster;
import java.io.File;
import java.io.FilenameFilter;
import java.util.Date;
import java.util.Hashtable;

import org.ossim.oms.image.omsImageSource;
import org.ossim.oms.util.ImageUtil;

import joms.oms.Init;
import joms.oms.RasterEngine;

public class RasterEngineTest
{
	public static File[] getTestFiles()
	{
		File[] testFiles = new File( "/data/bmng" ).listFiles( new FilenameFilter() {
			public boolean accept( File dir, String name )
			{
				return name.endsWith( ".tif" );
			}
		} );

		return testFiles;
	}

	/**
	 * @param args
	 */
	public static void main( String[] args )
	{
		Init.instance().initialize( args.length, args );

		test2();
	}

	private static void test2()
	{
		RasterEngine rasterEngine = new RasterEngine();
		File[] inputFiles = getTestFiles();

		for ( int i = 0; i < inputFiles.length; i++ )
		{
			//System.out.println( inputFiles[i] );
			rasterEngine.addFile( inputFiles[i].getAbsolutePath() );
		}

		String srsCode = "epsg:4326";
		double minX = -180.0;
		double minY = -90.0;
		double maxX = 180.0;
		double maxY = 90.0;
		int width = 640;
		int height = 320;
		int numBands = 3;
		byte[] data = new byte[width * height * numBands];

		rasterEngine.getMap( srsCode, minX, minY, maxX, maxY, width, height, data );

		DataBuffer dataBuffer = new DataBufferByte( data, data.length );
		int pixelStride = 3;
		int lineStride = 3 * width;
		
		int[] bandOffsets = new int[] {
				0, 1, 2
		};
		
		Point location = null;

		WritableRaster raster = WritableRaster.createInterleavedRaster( dataBuffer,
				width, height, lineStride, pixelStride, bandOffsets, location );

		ColorModel colorModel = omsImageSource.createColorModel(raster.getSampleModel());

		boolean isRasterPremultiplied = true;
		Hashtable<?,?> properties = null;

		BufferedImage image = new BufferedImage(
			colorModel,
			raster,
			isRasterPremultiplied,
			properties
		);
    
		ImageUtil.displayImage( "test", image );
	}
}
