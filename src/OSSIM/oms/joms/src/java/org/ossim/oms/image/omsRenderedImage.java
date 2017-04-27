package org.ossim.oms.image;

import java.awt.Point;
import java.awt.Rectangle;
import java.awt.image.DataBuffer;
import java.awt.image.ColorModel;
import java.awt.image.Raster;
import java.awt.image.RenderedImage;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;

import java.util.Map;
import java.util.Vector;

/**
 * Created by IntelliJ IDEA. User: sbortman Date: Jul 10, 2008 Time: 8:45:35 AM
 * To change this template use File | Settings | File Templates.
 */
public class omsRenderedImage implements RenderedImage
{

	protected Vector<RenderedImage> sources;
	protected Map<String, Object> properties;
	protected omsImageSource imageSource;

	public omsRenderedImage( omsImageSource imageSource )
	{
		this.imageSource = imageSource;
	}
	public void setImageSource(omsImageSource imageSource)
	{
		this.imageSource = imageSource;
	}

	public Vector<RenderedImage> getSources()
	{
		return sources;
	}

	public Object getProperty( String name )
	{
		return properties.get( name );
	}

	public String[] getPropertyNames()
	{
		String[] propertyNames = null;

		if ( properties != null )
		{
			propertyNames = new String[properties.size()];
			properties.keySet().toArray( propertyNames );
		}
		else
			propertyNames = new String[0];

		return propertyNames;
	}

	public ColorModel getColorModel()
	{
		return imageSource.getColorModel();
	}

	public SampleModel getSampleModel()
	{
		return imageSource.getSampleModel();
	}

	public int getWidth()
	{
		return imageSource.getWidth();
	}

	public int getHeight()
	{
		return imageSource.getHeight();
	}

	public int getMinX()
	{
		return 0;
	}

	public int getMinY()
	{
		return 0;
	}

	public int getNumXTiles()
	{
		return imageSource.getNumXTiles();
	}

	public int getNumYTiles()
	{
		return imageSource.getNumYTiles();
	}

	public int getMinTileX()
	{
		return 0;
	}

	public int getMinTileY()
	{
		return 0;
	}

	public int getTileWidth()
	{
		return imageSource.getTileWidth();
	}

	public int getTileHeight()
	{
		return imageSource.getTileHeight();
	}

	public int getTileGridXOffset()
	{
		return 0;
	}

	public int getTileGridYOffset()
	{
		return 0;
	}

	public Raster getTile( int tileX, int tileY )
	{
		Rectangle rect = new Rectangle();
		int tileWidth = imageSource.getTileWidth();
		int tileHeight = imageSource.getTileHeight();
		int x1 = tileX * tileWidth;
		int y1 = tileY * tileHeight;
		int x2 = x1 + tileWidth;
		int y2 = y1 + tileWidth;

		rect.setFrameFromDiagonal( x1, y1, x2, y2 );

		return getData( rect );
	}

	public Raster getData()
	{
		Rectangle rect = new Rectangle();
		int width = imageSource.getWidth();
		int height = imageSource.getHeight();

		rect.setFrameFromDiagonal( 0, 0, width, height );

		return getData( rect );
	}

	public Raster getData( Rectangle rect )
	{
		Point location = rect.getLocation();

		SampleModel dataSampleModel = imageSource.getSampleModel()
				.createCompatibleSampleModel( rect.width, rect.height );
      Rectangle requestRect =  new Rectangle(rect.x+imageSource.getMinX(),
                                             rect.y+imageSource.getMinY(),
                                             rect.width,
                                             rect.height);
		DataBuffer dataBuffer = imageSource.getDataBuffer(requestRect);
      //		DataBuffer dataBuffer = imageSource.getDataBuffer(rect);
      
		WritableRaster raster = Raster.createWritableRaster( dataSampleModel,
				dataBuffer, location );
      dataBuffer = null;
		return raster;
	}

	public WritableRaster copyData( WritableRaster raster )
	{
		return null;
	}
}
