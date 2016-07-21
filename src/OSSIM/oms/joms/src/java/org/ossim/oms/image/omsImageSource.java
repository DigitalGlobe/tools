package org.ossim.oms.image;

import joms.oms.ossimInterleaveType;
import joms.oms.ossimImageSource;
import joms.oms.ossimDataObjectStatus;
import joms.oms.ossimIrect;
import joms.oms.ossimImageDataRefPtr;
import joms.oms.ossimImageDataFactory;
import joms.oms.ossimScalarType;
import joms.oms.ossimImageSource;
import java.awt.Rectangle;
import java.awt.Transparency;
import java.awt.color.ColorSpace;
import java.awt.image.BandedSampleModel;
import java.awt.image.ColorModel;
import java.awt.image.ComponentColorModel;
import java.awt.image.ComponentSampleModel;
import java.awt.image.DataBuffer;
import java.awt.image.DirectColorModel;
import java.awt.image.IndexColorModel;
import java.awt.image.MultiPixelPackedSampleModel;
import java.awt.image.SampleModel;
import java.awt.image.SinglePixelPackedSampleModel;
import java.awt.image.PixelInterleavedSampleModel;

import org.ossim.oms.image.ImageDataBuffer;

import com.sun.imageio.plugins.common.BogusColorSpace;

public class omsImageSource
{
   protected ossimImageSource imageSource;
   protected ossimIrect imageBounds;
   protected int tileWidth;
   protected int tileHeight;
   
   protected int numXTiles;
   protected int numYTiles;
   
   protected int numBands;
   protected ossimScalarType scalarType;
   protected int sampleType;
   protected int colorSpaceType;
   protected ColorModel colorModel;
   protected SampleModel sampleModel;
   protected ossimImageDataRefPtr blankTile;
   protected ossimInterleaveType interleaveType = ossimInterleaveType.OSSIM_BIP;
   public omsImageSource( int width, int height, int tileWidth, int tileHeight, int numBands )
   {
      imageBounds = new ossimIrect();
      imageBounds.setOriginWidthHeight( 0, 0, width, height );
      this.tileWidth = tileWidth;
      this.tileHeight = tileHeight;
      this.numBands = numBands;
      scalarType = ossimScalarType.OSSIM_UINT8;
      setDefaultSampleType();
      setDefaultColorSpaceType();
      setDefaultSampleModel(width, height, numBands);
      // sampleModel = new BandedSampleModel( sampleType, width, height, numBands
      // );
      
      
      // colorModel = new ComponentColorModel( ColorSpace
      // .getInstance( colorSpaceType ), // colorSapce
      // false, // hasAlpha
      // true, // isAlphaPremultiplied
      // Transparency.OPAQUE, // transparency
      // sampleType // transferType
      // );
      
      colorModel = createColorModel( sampleModel );
   }

   public omsImageSource( ossimImageSource src )
   {
      setImageSource( src );
   }
   
   public void setImageSource( ossimImageSource src )
   {
      imageSource = src;
      if ( src != null )
      {
         imageBounds = src.getBoundingRect();
         tileWidth = (int)src.getTileWidth();
         tileHeight = (int)src.getTileHeight();
         numBands = (int)src.getNumberOfOutputBands();
         scalarType = src.getOutputScalarType();
         setDefaultSampleType();
         setDefaultColorSpaceType();
         
         int width = (int)imageBounds.getWidth();
         int height = (int)imageBounds.getHeight();
         
         if ( tileWidth == 0 )
            tileWidth = 64;
         
         if ( tileHeight == 0 )
            tileHeight = 64;
         
         setDefaultSampleModel(width, height, numBands);
         
         colorModel = createColorModel( sampleModel );
         blankTile = ossimImageDataFactory.instance().create( src, src );
         if ( ( blankTile != null ) && ( blankTile.get() != null ) )
         {
            blankTile.initialize();
         }
      }
      else
      {
         tileWidth = 0;
         tileHeight = 0;
         imageBounds = new ossimIrect();
         numBands = 0;
         scalarType = ossimScalarType.OSSIM_SCALAR_UNKNOWN;
         sampleModel = null;
         colorModel = null;
         blankTile = null;
      }
   }
   
   public ColorModel getColorModel()
   {
      return colorModel;
   }
   
   public SampleModel getSampleModel()
   {
      return sampleModel;
   }
   
   public int getWidth()
   {
      return (int)imageBounds.getWidth();
   }
   
   public int getHeight()
   {
      return (int)imageBounds.getHeight();
   }
   
   public int getMinX()
   {
      return imageBounds.ul().getX();
   }
   
   public int getMinY()
   {
      return imageBounds.ul().getY();
   }
   
   public int getTileWidth()
   {
      return tileWidth;
   }
   
   public int getTileHeight()
   {
      return tileHeight;
   }
   
   public int getNumXTiles()
   {
      int numXTiles = 0;
      if ( tileWidth != 0 )
      {
         numXTiles = getWidth() / tileWidth;
         
         if ( (int)imageBounds.getWidth() % tileWidth > 0 )
            numXTiles++;
      }
      
      return numXTiles;
   }
   
   public int getNumYTiles()
   {
      numYTiles = 0;
      
      if ( tileHeight != 0 )
      {
         numYTiles = (int)imageBounds.getHeight() / tileHeight;
         
         if ( (int)imageBounds.getHeight() % tileHeight > 0 )
            ++numYTiles;
      }
      
      return numYTiles;
   }
   
   public int getNumBands()
   {
      return numBands;
   }
   
   public DataBuffer getDataBuffer( Rectangle rect )
   {
      ImageDataBuffer buf = null;
      ossimIrect requestRect = new ossimIrect();
      requestRect.setOriginWidthHeight( rect.x, rect.y, rect.width, rect.height );
      
       try
       {
          ossimImageDataRefPtr data = imageSource.getTile( requestRect );
          if ( data != null )
          {
             if ( ( data.get() != null ) &&
                  ( data.getDataObjectStatus() != ossimDataObjectStatus.OSSIM_NULL ) )
             {
                buf = new ImageDataBuffer( data, interleaveType );
             }

             // Cleanup:
             data.delete();
          }

          
       }
       catch( Exception e )
       {
          System.err.println("Caught Exception: " + e.getMessage());
       }
       
       if ( ( buf == null ) && ( blankTile != null ) && ( blankTile.get() != null ) )
       {
          blankTile.setImageRectangle( requestRect );
          buf = new ImageDataBuffer( blankTile, interleaveType );
       } 
       
       return buf.getDataBuffer();
   }
   
   private void setDefaultSampleModel(int width, int height, int numBands)
   {
      sampleModel = null;
      if(interleaveType == ossimInterleaveType.OSSIM_BIP)
      {
         int[] bandOffsets = new int[numBands];
         for ( int i = 0; i < bandOffsets.length; i++ )
         {
            bandOffsets[i] = i;
         }
         sampleModel = new PixelInterleavedSampleModel(sampleType, width, height, numBands, numBands*width, bandOffsets);
      }
      else if(interleaveType == ossimInterleaveType.OSSIM_BSQ)
      {
         int[] bankOffsets = new int[numBands];
         int[] bandOffsets = new int[numBands];
         for ( int i = 0; i < bandOffsets.length; i++ )
         {
            bandOffsets[i] = i * width * height;
            bankOffsets[i] = 0;
         }
         sampleModel = new BandedSampleModel( sampleType, width, height, width,
                                              bankOffsets, bandOffsets );
      }
      else if(interleaveType == ossimInterleaveType.OSSIM_BIL)
      {
         int[] bandOffsets = new int[numBands];
         for ( int i = 0; i < bandOffsets.length; i++ )
         {
            bandOffsets[i] = i * width;
         }
         sampleModel = new PixelInterleavedSampleModel(sampleType, width, height, 1, numBands*width, bandOffsets);
      }
   }

   private void setDefaultColorSpaceType()
   {
      switch ( numBands )
      {
         case 1:
         {
            if ( scalarType.equals( ossimScalarType.OSSIM_UINT8 ) )
            {
               colorSpaceType = ColorSpace.CS_GRAY;
            }
            else
            {
               colorSpaceType = ColorSpace.TYPE_GRAY;
            }
            break;
         }
         case 2:
         {
            colorSpaceType = ColorSpace.TYPE_2CLR;
            break;
         }
         case 3:
         {
            if ( scalarType.equals( ossimScalarType.OSSIM_UINT8 ) )
            {
               colorSpaceType = ColorSpace.CS_sRGB;
            }
            break;
         }
         case 4:
         {
            colorSpaceType = ColorSpace.TYPE_4CLR;
            break;
         }
         case 5:
         {
            colorSpaceType = ColorSpace.TYPE_5CLR;
            break;
         }
         case 6:
         {
            colorSpaceType = ColorSpace.TYPE_6CLR;
            break;
         }
         case 7:
         {
            colorSpaceType = ColorSpace.TYPE_7CLR;
            break;
         }
         case 8:
         {
            colorSpaceType = ColorSpace.TYPE_8CLR;
            break;
         }
         case 9:
         {
            colorSpaceType = ColorSpace.TYPE_9CLR;
            break;
         }
         case 10:
         {
            colorSpaceType = ColorSpace.TYPE_ACLR;
            break;
         }
         case 11:
         {
            colorSpaceType = ColorSpace.TYPE_BCLR;
            break;
         }
         case 12:
         {
            colorSpaceType = ColorSpace.TYPE_CCLR;
            break;
         }
         case 13:
         {
            colorSpaceType = ColorSpace.TYPE_DCLR;
            break;
         }
         case 14:
         {
            colorSpaceType = ColorSpace.TYPE_ECLR;
            break;
         }
         case 15:
         {
            colorSpaceType = ColorSpace.TYPE_FCLR;
            break;
         }
         default:
         {
            colorSpaceType = ColorSpace.TYPE_GRAY;
            break;
         }
      }
   }

   private void setDefaultSampleType()
   {
      sampleType = DataBuffer.TYPE_UNDEFINED;
      
      if ( scalarType.equals( ossimScalarType.OSSIM_UINT8 ) )
      {
         sampleType = DataBuffer.TYPE_BYTE;
      }
      else if ( scalarType.equals( ossimScalarType.OSSIM_SINT16 ) )
      {
         sampleType = DataBuffer.TYPE_SHORT;
      }
      else if ( scalarType.equals( ossimScalarType.OSSIM_USHORT11 )
                || scalarType.equals( ossimScalarType.OSSIM_UINT16 ) )
      {
         sampleType = DataBuffer.TYPE_USHORT;
      }
      else if ( scalarType.equals( ossimScalarType.OSSIM_FLOAT32 ) )
      {
         sampleType = DataBuffer.TYPE_FLOAT;
      }
      else if ( scalarType.equals( ossimScalarType.OSSIM_FLOAT64 ) )
      {
         sampleType = DataBuffer.TYPE_DOUBLE;
      }
   }
   
   public static final ColorModel createColorModel( SampleModel sampleModel )
   {
      // Check the parameter.
      if ( sampleModel == null )
      {
         throw new IllegalArgumentException( "sampleModel == null!" );
      }

      // Get the data type.
      int dataType = sampleModel.getDataType();

      // Check the data type
      switch ( dataType )
      {
         case DataBuffer.TYPE_BYTE:
         case DataBuffer.TYPE_USHORT:
         case DataBuffer.TYPE_SHORT:
         case DataBuffer.TYPE_INT:
         case DataBuffer.TYPE_FLOAT:
         case DataBuffer.TYPE_DOUBLE:
            break;
         default:
            // Return null for other types.
            return null;
      }

      // The return variable.
      ColorModel colorModel = null;

      // Get the sample size.
      int[] sampleSize = sampleModel.getSampleSize();

      // Create a Component ColorModel.
      if ( sampleModel instanceof ComponentSampleModel )
      {
         // Get the number of bands.
         int numBands = sampleModel.getNumBands();

         // Determine the color space.
         ColorSpace colorSpace = null;
         if ( numBands <= 2 )
         {
            colorSpace = ColorSpace.getInstance( ColorSpace.CS_GRAY );
         }
         else if ( numBands <= 4 )
         {
            colorSpace = ColorSpace.getInstance( ColorSpace.CS_sRGB );
         }
         else
         {
            colorSpace = new BogusColorSpace( numBands );
         }

         boolean hasAlpha = ( numBands == 2 ) || ( numBands == 4 );
         boolean isAlphaPremultiplied = false;
         int transparency = hasAlpha ? Transparency.TRANSLUCENT
            : Transparency.OPAQUE;

         colorModel = new ComponentColorModel( colorSpace, sampleSize, hasAlpha,
                                               isAlphaPremultiplied, transparency, dataType );
      }
      else if ( sampleModel.getNumBands() <= 4
                && sampleModel instanceof SinglePixelPackedSampleModel )
      {
         SinglePixelPackedSampleModel sppsm = (SinglePixelPackedSampleModel)sampleModel;

         int[] bitMasks = sppsm.getBitMasks();
         int rmask = 0;
         int gmask = 0;
         int bmask = 0;
         int amask = 0;

         int numBands = bitMasks.length;
         if ( numBands <= 2 )
         {
            rmask = gmask = bmask = bitMasks[0];
            if ( numBands == 2 )
            {
               amask = bitMasks[1];
            }
         }
         else
         {
            rmask = bitMasks[0];
            gmask = bitMasks[1];
            bmask = bitMasks[2];
            if ( numBands == 4 )
            {
               amask = bitMasks[3];
            }
         }

         int bits = 0;
         for ( int i = 0; i < sampleSize.length; i++ )
         {
            bits += sampleSize[i];
         }

         return new DirectColorModel( bits, rmask, gmask, bmask, amask );

      }
      else if ( sampleModel instanceof MultiPixelPackedSampleModel )
      {
         // Load the colormap with a ramp.
         int bitsPerSample = sampleSize[0];
         int numEntries = 1 << bitsPerSample;
         byte[] map = new byte[numEntries];
         for ( int i = 0; i < numEntries; i++ )
         {
            map[i] = (byte)( i * 255 / ( numEntries - 1 ) );
         }

         colorModel = new IndexColorModel( bitsPerSample, numEntries, map, map,
                                           map );

      }

      return colorModel;
   }

}
