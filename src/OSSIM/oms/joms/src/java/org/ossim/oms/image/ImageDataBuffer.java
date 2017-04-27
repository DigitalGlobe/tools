package org.ossim.oms.image;

import joms.oms.ossimImageData;
import joms.oms.ossimImageDataRefPtr;
import joms.oms.ossimInterleaveType;
import joms.oms.ossimScalarType;
import java.awt.image.DataBuffer;
import java.awt.image.DataBufferByte;
import java.awt.image.DataBufferShort;
import java.awt.image.DataBufferUShort;
import java.awt.image.DataBufferFloat;
import java.awt.image.DataBufferDouble;
import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.HashMap;
import java.util.Map;

public class ImageDataBuffer
{
	private ossimImageDataRefPtr imageData;
	private DataBuffer dataBuffer;
   private ossimInterleaveType interleaveType = ossimInterleaveType.OSSIM_BSQ;
   
   public ImageDataBuffer(ossimInterleaveType interleaveType)
   {
      this.interleaveType = interleaveType;
   }
	public ImageDataBuffer( ossimImageDataRefPtr imageData )
	{
		setImageData( imageData );
	}
	public ImageDataBuffer( ossimImageDataRefPtr imageData, ossimInterleaveType interleaveType )
	{
      this.interleaveType = interleaveType;
		setImageData( imageData );
	}
   
	public ossimImageDataRefPtr getImageData()
	{
		return imageData;
	}

	public DataBuffer getDataBuffer()
	{
		return dataBuffer;
	}

	public void setImageData( ossimImageDataRefPtr imageData )
	{
		this.imageData = imageData;
		this.dataBuffer = null;
		if ( imageData != null )
		{
			// initialize1();
			initialize();
		}
	}

	private void initialize()
	{
		if(imageData.get() == null)
		{
			this.dataBuffer = null;
			return;
		}
		ossimScalarType scalarType = imageData.getScalarType();
		int width = (int)imageData.getWidth();
		int height = (int)imageData.getHeight();
		int numBands = (int)imageData.getNumberOfBands();
		int sizeInBytes = (int)imageData.getSizeInBytes();
		int numElems = width * height * numBands;

		if ( scalarType.equals( ossimScalarType.OSSIM_UINT8 ) )
		{
			ByteBuffer buffer = unloadTile( sizeInBytes );
			byte[] data = null;

			if ( buffer.hasArray() )
				data = buffer.array();
			else
			{
				data = new byte[numElems];
				buffer.get( data );
			}

			dataBuffer = new DataBufferByte( data, data.length );
		}
		else if ( scalarType.equals( ossimScalarType.OSSIM_SINT16 ) )
		{
			ShortBuffer buffer = unloadTile( sizeInBytes ).asShortBuffer();
			short[] data = null;

			if ( buffer.hasArray() )
				data = buffer.array();
			else
			{
				data = new short[numElems];
				buffer.get( data );
			}

			dataBuffer = new DataBufferShort( data, data.length );
		}
		else if ( scalarType.equals( ossimScalarType.OSSIM_USHORT11 )
				|| scalarType.equals( ossimScalarType.OSSIM_UINT16 ) )
		{
			ShortBuffer buffer = unloadTile( sizeInBytes ).asShortBuffer();
			short[] data = null;

			if ( buffer.hasArray() )
				data = buffer.array();
			else
			{
				data = new short[numElems];
				buffer.get( data );
			}

			dataBuffer = new DataBufferUShort( data, data.length );
		}
		else if ( scalarType.equals( ossimScalarType.OSSIM_FLOAT32 ) )
		{
			FloatBuffer buffer = unloadTile( sizeInBytes ).asFloatBuffer();
			float[] data = null;

			if ( buffer.hasArray() )
				data = buffer.array();
			else
			{
				data = new float[numElems];
				buffer.get( data );
			}

			dataBuffer = new DataBufferFloat( data, data.length );
		}
		else if ( scalarType.equals( ossimScalarType.OSSIM_FLOAT64 ) )
		{
			DoubleBuffer buffer = unloadTile( sizeInBytes ).asDoubleBuffer();
			double[] data = null;

			if ( buffer.hasArray() )
				data = buffer.array();
			else
			{
				data = new double[numElems];
				buffer.get( data );
			}

			dataBuffer = new DataBufferDouble( data, data.length );
		}
	}

	private ByteBuffer unloadTile( int sizeInBytes )
	{
		ByteBuffer buffer = ByteBuffer.allocateDirect( sizeInBytes );

		imageData.unloadTile( buffer, imageData.getImageRectangle(), interleaveType);
		
		return buffer;
	}
}