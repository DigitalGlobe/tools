package org.ossim.oms.apps;

import joms.oms.ossimInit;
import joms.oms.ossimImageHandler;
import joms.oms.ossimImageHandlerRegistry;
import joms.oms.ossimImageWriterFactoryRegistry;
import joms.oms.ossimImageSourceFactoryRegistry;
import joms.oms.ossimImageSource;
import joms.oms.ossimString;
import joms.oms.ossimDpt;
import joms.oms.ossimImageFileWriter;
import joms.oms.ossimGptVector;
import joms.oms.SingleImageChain;
import joms.oms.ossimProjection;
import joms.oms.ossimGpt;
import joms.oms.ossimIrect;
import joms.oms.ossimIpt;
import joms.oms.ossimUnitType;
import joms.oms.Util;
import org.ossim.oms.util.ImageUtil;
import org.ossim.oms.image.TwoColorMultiView;
import org.ossim.oms.image.omsRenderedImage;
import org.ossim.oms.image.omsImageSource;

public class ServiceTest
{
	/**
	 * @param args
	 */
	public static void main( String[] args )
	{
		String[] newArgs = new String[0];
		ossimInit.instance().initialize( newArgs.length, newArgs );

		if ( args.length > 0 )
		{
			if ( args[0].equals( "hillShade" ) )
			{
				if ( args.length >= 3 )
				{
					String[] elevationFiles = {
						args[1]
					};
					String[] colorFiles = {
						args[2]
					};

					createHillshade( elevationFiles, colorFiles );
				}
			}
			else if ( args[0].equals( "panSharpen" ) )
			{
				if ( args.length >= 3 )
				{
					String[] singleBandFiles = {
						args[1]
					};
					String[] multiBandFiles = {
						args[2]
					};

					createPanSharpenedImage( singleBandFiles, multiBandFiles );
				}
			}
			else if ( args[0].equals( "twoColorView" ) )
			{
				if ( args.length == 3 )
				{
					String[] newInputFiles = {
						args[1]
					};
					String[] oldInputFiles = {
						args[2]
					};

					createTwoColorViewImage( newInputFiles, oldInputFiles );
				}
			}
			else if ( args[0].equals( "centerCutReproject" ) )
			{
				if ( args.length == 4 )
				{
					// int bandList[] = {2,1,0};

					String filename = args[1];
					int width = Integer.parseInt( args[2] );
					int height = Integer.parseInt( args[3] );

					createCenterCutReprojectImage( filename, width, height );
				}
				else
				{
					System.out
							.println( "Usage: ServiceTest panShapren <pan file> <colorFile> <outputFile>" );
				}
			}
			else if ( args[0].equals( "centerCutRadius" ) )
			{
				if ( args.length == 6 )
				{
					String[] filenames = {
						args[1]
					};
					double lon = Double.parseDouble( args[2] );
					double lat = Double.parseDouble( args[3] );
					double radius = Double.parseDouble( args[4] );
					String units = args[5];

					createCenterCutRadiusImage( filenames, lon, lat, radius, units );
				}
			}
			else if ( args[0].equals( "copy" ) )
			{
				if ( args.length == 3 )
				{
					String inputFile = args[1];
					String outputFile = args[2];

					copyImage( inputFile, outputFile );
				}
			}
		}
		else
		{
			System.out.println( "Usage examples:\n\n" );
			System.out
					.println( "ServiceTest panSharpen <pan_input_file> <color_input_file>" );
			System.out
					.println( "ServiceTest hillShade <elevation_input_file> <color_input_file>" );
			System.out.println( "ServiceTest twoColorView <new_input> <old_input>" );
			System.out.println( "ServiceTest centerCutReproject <input_file>" );
			System.out.println( "ServiceTest centerCutRadius <input_file>" );
			System.out.println( "ServiceTest copy <input_file> <output_file>" );
		}
	}

	public static void copyImage( String inputFile, String outputFile )
	{
		ossimImageHandler newInput = ossimImageHandlerRegistry.instance().open(
				inputFile );

		if ( newInput != null )
		{
			ossimImageFileWriter writer = ossimImageWriterFactoryRegistry.instance()
					.createWriterFromFilename( outputFile );
			if ( writer != null )
			{
				writer.connectMyInputTo( newInput );
				writer.execute();
			}
		}
	}

	public static void createCenterCutRadiusImage( String[] filenames,
			double lon, double lat, double radius, String units )
	{
		ossimImageHandler h = ossimImageHandlerRegistry.instance().open(
				filenames[0] );

		SingleImageChain chain = new SingleImageChain();
		chain.getImageChain().connectMyInputTo( h );
		ossimProjection viewProj = Util.createViewProjection( h,
				"ossimUtmProjection" );
		Util.setAllViewProjections( chain.getImageChain(), viewProj, false );
		if ( viewProj != null )
		{
			ossimGptVector gpts = new ossimGptVector();
			ossimGpt centerGround = new ossimGpt( lat, lon );

			Util.computeGroundPointsFromCenterRadius( gpts, viewProj, centerGround,
					radius, ossimUnitType.OSSIM_METERS );
			chain.setViewCut( gpts );
			Util.updateProjectionToFitOutputDimensions( viewProj, chain
					.getImageChain().getBoundingRect(), 512, 512, true );// set to
			// true to
			// keep the
			// aspect
			// ratio
			Util.setAllViewProjections( chain.getImageChain(), viewProj, false );
			omsRenderedImage renderedImage = new omsRenderedImage(
					new omsImageSource( chain.getImageChain() ) );
			ImageUtil.displayImageModal( "centerCutRadius", renderedImage );
		}
	}

	public static void createCenterCutReprojectImage( String filename, int width,
			int height )
	{
		ossimImageHandler inputImage = ossimImageHandlerRegistry.instance().open(
				filename );

		if ( inputImage != null )
		{
			ossimIrect bounds = inputImage.getBoundingRect();
			ossimIrect rect = new ossimIrect();
			ossimIpt origin = bounds.getMidPoint();
			rect.setOriginWidthHeight( origin.getX() - width / 2, origin.getY()
					- height / 2, width, height );
			SingleImageChain colorChain = new SingleImageChain();

			colorChain.getImageChain().connectMyInputTo( inputImage );
			colorChain.setImageCut( rect );
			colorChain.setHistogramFileAndMode( inputImage
					.createDefaultHistogramFilename().toString(), "linear_auto_min_max" );
			colorChain.setResamplerType( "lanczos" );
			colorChain.setToThreeBands();
			colorChain.setScalarRemapperEnableFlag( true );
			omsRenderedImage renderedImage = new omsRenderedImage(
					new omsImageSource( colorChain.getImageChain()) );
			ImageUtil.displayImageModal( "centerCutReproject", renderedImage );
			// ossimImageFileWriter writer = ossimImageWriterFactoryRegistry
			// .instance().createWriterFromFilename( args[2] );
			// if(writer != null)
			// {
			// writer.connectMyInputTo(colorChain.getImageChain());
			// writer.execute();
			// }
		}
		else
		{
			System.out.println( "Unable to open input file for center cut reproject" );
		}
	}
   
	public static void createTwoColorViewImage( String[] newInputFiles,
			String[] oldInputFiles )
	{
		TwoColorMultiView twoColorMultiView = new TwoColorMultiView( newInputFiles,
				oldInputFiles );
		// and display
		omsRenderedImage renderedImage = twoColorMultiView.createRenderedImage();

		ImageUtil.displayImageModal( "Two Color View", renderedImage );
		// if(writer != null)
		// {
		// writer.connectMyInputTo(twoColorView);
		// writer.execute();
		// }
	}

	public static void createHillshade( String[] elevationFiles,
			String[] colorFiles )
	{
		ossimImageHandler input1 = ossimImageHandlerRegistry.instance().open(
				elevationFiles[0] );
		ossimImageHandler input2 = ossimImageHandlerRegistry.instance().open(
				colorFiles[0] );

		if ( input1 == null || input2 == null )
		{
			return;
		}
		SingleImageChain normalsChain = new SingleImageChain();
		SingleImageChain colorChain = new SingleImageChain();
		if ( ( input1.getNumberOfOutputBands() == 1 )
				&& ( input2.getNumberOfOutputBands() >= 3 ) )
		{
			normalsChain.getImageChain().connectMyInputTo( input1 );
			colorChain.getImageChain().connectMyInputTo( input2 );
		}
		else if ( ( input2.getNumberOfOutputBands() == 1 )
				&& ( input1.getNumberOfOutputBands() >= 3 ) )
		{
			normalsChain.getImageChain().connectMyInputTo( input2 );
			colorChain.getImageChain().connectMyInputTo( input1 );
		}
		else
		{
			return;
		}
		ossimProjection viewProj = Util.createViewProjection( colorChain
				.getImageChain().getInput(), "ossimUtmProjection" );

		colorChain.setToThreeBands();
		colorChain.setScalarRemapperEnableFlag( true );
		normalsChain.setCacheEnableFlag( true );
		normalsChain.setupSurfaceNormalCalculations();
		colorChain.setResamplerType( "lanczos" );
		normalsChain.setResamplerType( "lanczos" );
		Util.setAllViewProjections( colorChain.getImageChain(), viewProj, false );
		Util.setAllViewProjections( normalsChain.getImageChain(), viewProj, false );
		// expects normls for first layer and color for second. If color does
		// not exist it will
		// do a grey shade
		ossimImageSource hillshadeSource = ossimImageSourceFactoryRegistry
				.instance().createImageSource(
						new ossimString( "ossimBumpShadeTileSource" ) );

		if ( hillshadeSource != null )
		{
			ossimIrect colorRect = colorChain.getImageChain().getBoundingRect();
			ossimIrect normalsRect = normalsChain.getImageChain().getBoundingRect();
			ossimIrect clipRect = colorRect.clipToRect( normalsRect );
			colorChain.setViewCut( clipRect );
			normalsChain.setViewCut( clipRect );
			hillshadeSource.connectMyInputTo( normalsChain.getImageChain() );
			hillshadeSource.connectMyInputTo( colorChain.getImageChain() );
			// now display and connect to the 8-bit output and display
			omsRenderedImage renderedImage = new omsRenderedImage(
					new omsImageSource( hillshadeSource ) );
			ImageUtil.displayImageModal( "Hill shade", renderedImage );
		}
	}

	public static void createPanSharpenedImage( String[] singleBandFiles,
			String[] multiBandFiles )
	{
		// open up the handlers
		ossimImageHandler input1 = ossimImageHandlerRegistry.instance().open(
				singleBandFiles[0] );

		ossimImageHandler input2 = ossimImageHandlerRegistry.instance().open(
				multiBandFiles[0] );

		ossimImageHandler colorHandler;
		if ( ( input1 == null ) || ( input2 == null ) )
		{
			return;

		}

		// now make sure the color chain is 3 band
		// and we find the pan
		ossimProjection panProj = null;
		ossimProjection colorProj = null;
		SingleImageChain panChain = new SingleImageChain();
		SingleImageChain colorChain = new SingleImageChain();

		if ( ( input1.getNumberOfOutputBands() == 1 )
				&& ( input2.getNumberOfOutputBands() >= 3 ) )
		{
			panProj = Util.createProjection( input1 );
			colorProj = Util.createProjection( input2 );
			panChain.getImageChain().connectMyInputTo( input1 );
			colorChain.getImageChain().connectMyInputTo( input2 );
			colorHandler = input2;
		}
		else if ( ( input2.getNumberOfOutputBands() == 1 )
				&& ( input1.getNumberOfOutputBands() >= 3 ) )
		{
			panProj = Util.createProjection( input2 );
			colorProj = Util.createProjection( input1 );
			panChain.getImageChain().connectMyInputTo( input2 );
			colorChain.getImageChain().connectMyInputTo( input1 );
			colorHandler = input1;
		}
		else
		{
			return;
		}
		// we really need to test if the image bands are an image handler with
		// reversed bands
		// where rgb are bgr. For now we will always reverse and set to 3
		// bands
		//
		colorChain.setToThreeBandsReverse();

		// after we reverse the bands we can set the histogram auto stretch
		//
		colorChain.setHistogramFileAndMode( colorHandler
				.createDefaultHistogramFilename().toString(), "linear_auto_min_max" );

		// Let's do a smooth filter that preserves edges
		//
		colorChain.setResamplerType( "lanczos" );
		panChain.setResamplerType( "lanczos" );

		// now let's setup the fusion
		// We need to bring the chains to the full res scale of the pan and
		// use that to enhance
		// the colo chain's detials
		//
		ossimImageSource fusion = ossimImageSourceFactoryRegistry.instance()
				.createImageSource( new ossimString( "ossimSFIMFusion" ) );
		if ( fusion != null )
		{
			// bring color chain to the scale of the pan by using the pan
			// chain's initial geometry and we will output to a UTM
			// projection
			//
			ossimProjection viewProj = Util.createViewProjection( panChain
					.getImageChain().getInput(), "ossimUtmProjection" );

			fusion.connectMyInputTo( panChain.getImageChain() );
			fusion.connectMyInputTo( colorChain.getImageChain() );

			// calculate scale change and use that to set the low pass kernel
			// size. In practice it seems that the low pass kernel width if set
			// to the scale ratio of color to pan produces good results and
			// serves as an automatic way to set the fusion combiner
			//
			ossimDpt colorScale = colorProj.getMetersPerPixel();
			ossimDpt panScale = panProj.getMetersPerPixel();
			fusion.getPropertyInterface().setProperty(
					new ossimString( "lowPassKernelWidth" ),
					new ossimString( Double
							.toString( colorScale.getY() / panScale.getY() ) ) );

			// now let's bring everyone to the same view
			Util.setAllViewProjections( fusion, viewProj, false );

			// no lets clip
			//
			ossimIrect colorRect = colorChain.getImageChain().getBoundingRect();
			ossimIrect panRect = panChain.getImageChain().getBoundingRect();
			ossimIrect clipRect = colorRect.clipToRect( panRect );
			colorChain.setViewCut( clipRect );
			panChain.setViewCut( clipRect );

			// for this example let's produce an 8 bit product. We don't set the
			// input chains scalar remapper since we want to fuse in full
			// pixel depth and not scale down to 8 bit before the fusion.
			//
			ossimImageSource scalarRemapper = ossimImageSourceFactoryRegistry
					.instance().createImageSource(
							new ossimString( "ossimScalarRemapper" ) );
			scalarRemapper.getPropertyInterface().setProperty(
					new ossimString( "enabled" ), new ossimString( "true" ) );
			scalarRemapper.connectMyInputTo( fusion );

			// now display and connect to the 8-bit output and display
			omsRenderedImage renderedImage = new omsRenderedImage(
					new omsImageSource( scalarRemapper ) );
			ImageUtil.displayImageModal( "Pan Sharpen ", renderedImage );
		}
	}
}
