package org.ossim.oms.image;

import joms.oms.SingleImageChain;
import joms.oms.Util;
import joms.oms.ossimImageHandler;
import joms.oms.ossimImageHandlerRegistry;
import joms.oms.ossimImageSource;
import joms.oms.ossimImageSourceFactoryRegistry;
import joms.oms.ossimIrect;
import joms.oms.ossimProjection;
import joms.oms.ossimString;

public class TwoColorMultiView
{
	private ossimImageHandler newInput;
	private ossimImageHandler oldInput;
	private SingleImageChain newInputChain;
	private SingleImageChain oldInputChain;
	private ossimImageSource twoColorView;
	private ossimProjection viewProj;
	private ossimImageSource cache;

	public TwoColorMultiView( String[] newInputFiles, String[] oldInputFiles )
	{
		newInput = ossimImageHandlerRegistry.instance().open( newInputFiles[0] );

		oldInput = ossimImageHandlerRegistry.instance().open( oldInputFiles[0] );

		if ( newInput == null )
		{
			System.out.println( "Unable to open first input file" + newInputFiles[0] );
			return;
		}
		if ( oldInput == null )
		{
			System.out
					.println( "Unable to open second input file" + oldInputFiles[0] );
			return;
		}
		newInputChain = new SingleImageChain();
		oldInputChain = new SingleImageChain();

		// connect them up
		//
		newInputChain.getImageChain().connectMyInputTo( newInput );
		oldInputChain.getImageChain().connectMyInputTo( oldInput );
		viewProj = Util.createViewProjection( newInput, "ossimUtmProjection" );
		twoColorView = ossimImageSourceFactoryRegistry.instance()
				.createImageSource( new ossimString( "ossimTwoColorView" ) );
		// ossimImageFileWriter writer =
		// ossimImageWriterFactoryRegistry.instance
		// ().createWriterFromFilename( args[3] );
		if ( twoColorView != null )
		{
			twoColorView.connectMyInputTo( newInputChain.getImageChain() );
			twoColorView.connectMyInputTo( oldInputChain.getImageChain() );
			// bring everything to a common view
			Util.setAllViewProjections( twoColorView, viewProj, false );

			// for fun let's clip to the intersection and only display the
			// overlapping region
			//
			ossimIrect newRect = newInputChain.getImageChain().getBoundingRect();
			ossimIrect oldRect = oldInputChain.getImageChain().getBoundingRect();
			ossimIrect clipRect = newRect.clipToRect( oldRect );
			newInputChain.setViewCut( clipRect );
			oldInputChain.setViewCut( clipRect );

			cache = ossimImageSourceFactoryRegistry.instance()
					.createImageSource( new ossimString( "ossimCacheTileSource" ) );

			cache.connectMyInputTo( twoColorView );
			
			// no let's make it fit to the output rect
			// 
			// initiallze the combiner so input bounding rects are recomputed
			twoColorView.initialize();
			// Util.updateProjectionToFitOutputDimensions( viewProj, twoColorView
			// .getBoundingRect(), 512, 512, true );// set to true to keep the
			// aspect ratio
			Util.setAllViewProjections( twoColorView, viewProj, false );
		}
	}

	public omsRenderedImage createRenderedImage()
	{
		omsRenderedImage renderedImage = new omsRenderedImage( new omsImageSource(
				cache ) );
		return renderedImage;
	}
}
