package org.ossim.oms.image;

import java.awt.image.RenderedImage;

import joms.oms.SingleImageChain;
import joms.oms.Util;
import joms.oms.WmsView;
import joms.oms.ossimFilename;
import joms.oms.ossimImageChain;
import joms.oms.ossimIrect;
import joms.oms.ossimImageHandler;
import joms.oms.ossimImageSource;
import joms.oms.ossimMemoryImageSource;
import joms.oms.ossimImageFileWriter;
import joms.oms.ossimString;
import joms.oms.ossimScalarType;
import joms.oms.ossimDataObjectStatus;
import joms.oms.ossimImageHandlerRegistry;
import joms.oms.ossimImageSourceFactoryRegistry;
import joms.oms.ossimImageWriterFactoryRegistry;
import joms.oms.ossimImageDataRefPtr;

import java.util.ArrayList;
import java.util.List;
import java.awt.image.DataBufferByte;

public class WMSMap
{
   //static private int count = 0;
   private WmsView wmsView;
   private List<ossimImageHandler> inputHandlers = new ArrayList<ossimImageHandler>();
   private List<SingleImageChain> inputChains = new ArrayList<SingleImageChain>();
   private SingleImageChain eightBitThreeBandChain;
   private ossimImageSource mosaic;
   private ossimMemoryImageSource blankSource;
   private ossimMemoryImageSource cacheSource;
   
   // set this to true if you want any call to getMAp to go ahead and
   // retrieve the image and return it as a cached memory object
   //
   private boolean cacheFlag = true;
   private ossimImageHandlerRegistry imageHandlerRegistry;
   
   public WMSMap()
   {
      //incPrintCount();
      mosaic = ossimImageSourceFactoryRegistry.instance().createImageSource(
         new ossimString( "ossimImageMosaic" ) );
      // now setup a 8 bit 3 band chain
      eightBitThreeBandChain = new SingleImageChain();
      
      // make this an image space ony chain and scale to 8 bit 3 band output
      eightBitThreeBandChain.setImageRendererEnableFlag( false );
      eightBitThreeBandChain.getImageChain().connectMyInputTo( 0, mosaic );
      eightBitThreeBandChain.setToThreeBands();
      eightBitThreeBandChain.setScalarRemapType( ossimScalarType.OSSIM_UINT8 );
      blankSource = new ossimMemoryImageSource();
      cacheSource = new ossimMemoryImageSource();
      blankSource.setImage( ossimScalarType.OSSIM_UINT8, 3, 256, 256 );
      imageHandlerRegistry = ossimImageHandlerRegistry.instance();
      wmsView = new WmsView();
      
   }

   public WMSMap( String[] layers )
   {
      this(); // Call zero arg constructor
      //incPrintCount();
      if ( layers.length > 0 )
      {
         int idx = 0;
         // painters algorithm is used for WMS so I will reverse them so the the
         // first one is on the bottom
         //
         for ( idx = layers.length - 1; idx >= 0; --idx )
         {
            addFile( layers[idx] );
         }
      }
   }

   public void addFile( String filename )
   {
      addFile( filename, 0 );
   }
   
   public void addFile( String filename, String entryId )
   {
      addFile( filename, Integer.valueOf( entryId ) );
   }

   private void addFile( String filename, int entryNumber )
   {
      ossimImageHandler input = imageHandlerRegistry.open( new ossimFilename(
                                                              filename ) );

      if ( input != null )
      {
         if ( input.setCurrentEntry( entryNumber ) )
         {
            SingleImageChain reprojectionChain = new SingleImageChain();
            reprojectionChain.setResamplerType( "bilinear" );
            // retain the pointers
            inputHandlers.add( input );
            inputChains.add( reprojectionChain );
            
            // connect the chain to the source
            ossimImageChain chain1 = reprojectionChain.getImageChain();
            
            chain1.connectMyInputTo( input );
            
            // add to the mosaic
            mosaic.connectMyInputTo( chain1 );
            
            ossimImageChain chain2 = eightBitThreeBandChain.getImageChain();
            
            chain2.connectMyInputTo( 0, mosaic );				
            chain2.initialize();
            
            eightBitThreeBandChain.setToThreeBands();
            eightBitThreeBandChain.setScalarRemapType( ossimScalarType.OSSIM_UINT8 );
         }
      }
   }

   /**
    * Stretch mode values can be linear_one_piece, linear_1std_from_mean, linear_2std_from_mean, linear_3std_from_mean, linear_auto_min_max
    */
   public void setStretchMode(String mode)
   {
      int chainCount = inputChains.size();
      for(int idx = 0; idx < chainCount; ++idx)
      {
         SingleImageChain chain = inputChains.get(idx);
         if(chain != null)
         {
            chain.setHistogramFileToDefaultAndMode(mode);
         }
      }
   }

   public void getMap( String srsCode, double minX, double minY, double maxX,
                       double maxY, int width, int height, byte[] buffer )
   {
      // System.out.println( buffer.length );
      RenderedImage image = getMap( srsCode, minX, minY, maxX, maxY, width,
                                    height );
      
      // System.out.println( image.getWidth() + " " + image.getHeight() );
      
      DataBufferByte byteBuffer = (DataBufferByte)image.getData().getDataBuffer();
      
      // System.out.println( byteBuffer.getSize() );
      
      System.arraycopy( byteBuffer.getData(), 0, buffer, 0, buffer.length );
      
   }

   public RenderedImage getMap( String srsCode, double minX, double minY,
                                double maxX, double maxY, int width, int height )
   {
      if ( setupView( srsCode, minX, minY, maxX, maxY, width, height ) )
      {
         if ( cacheFlag == true )
         {
            try
            {
               ossimImageDataRefPtr imageData = eightBitThreeBandChain.getImageChain()
                  .getTile( new ossimIrect( wmsView.getViewImageRect() ) );
               if ( (imageData != null) && (imageData.get() != null) )
               {
                  if ( imageData.getDataObjectStatus() != ossimDataObjectStatus.OSSIM_NULL )
                  {
                     cacheSource.setImage( imageData );
                     return new omsRenderedImage( new omsImageSource( cacheSource ) );
                  }
               }
            }
            catch( Exception e )
            {
               System.err.println("Caught Exception: " + e.getMessage());

               // Should pass down to blank source return below(last line).
            }
         }
         else
         {
            return new omsRenderedImage( new omsImageSource( eightBitThreeBandChain
                                                             .getImageChain() ) );
         }
      }
      
      blankSource.setImage( ossimScalarType.OSSIM_UINT8, 3, width, height );
      return new omsRenderedImage( new omsImageSource( blankSource ) );
   }
   
   public boolean writeMap( String filename, String format, String srsCode,
                            double minX, double minY, double maxX, double maxY,
                            int width, int height )
   {
      if ( setupView( srsCode, minX, minY, maxX, maxY, width, height ) )
      {
         ossimImageFileWriter writer = ossimImageWriterFactoryRegistry.instance()
            .createWriter( new ossimString( format ) );
         if ( writer != null )
         {
            writer.setFilename( new ossimFilename( filename ) );
            writer.connectMyInputTo( eightBitThreeBandChain.getImageChain() );
            writer.execute();
            return true;
         }
      }
      
      return false;
   }
   
   private boolean setupView( String srsCode, double minX, double minY,
                              double maxX, double maxY, int width, int height )
   {
      // first let's create a reprojection for the epsg code defined
      //
      if ( !wmsView.setProjection( srsCode ) )
      {
         // technically we need to do an WMS XML exception or whatever is
         // defined by OSGC WMS for an exception
         System.err.println( "Unable to create projection for " + srsCode );
         return false;
      }
      
      wmsView.setViewDimensionsAndImageSize( minX, minY, maxX, maxY, width,
                                             height );
      
      Util.setAllViewGeometries( mosaic, wmsView.getImageGeometry().get());
      // enable the image cutter
      //
      eightBitThreeBandChain.setImageCut( wmsView.getViewImageRect() );
      eightBitThreeBandChain.getImageChain().initialize();
      return true;
   }
   /*
     public void finalize()
     {
     if(mosaic != null)
     {
     decPrintCount();
     }
     cleanUp();
     }
   */
/*
  public static synchronized void incPrintCount()
   {
      ++count;
      printCount();
   }
   public static synchronized void decPrintCount()
   {
      --count;
      printCount();
   }
   static public void printCount()
   {
      System.out.println("Still need to finalize " + count);
  }
*/

   public void cleanUp()
   {
      for ( ossimImageHandler handler : this.inputHandlers)
      {
         if ( handler != null )
         {
            handler.delete();
         }
      }
      inputHandlers.clear();
      
      for ( SingleImageChain chain : this.inputChains)
      {
         if ( chain != null )
         {
            chain.delete();
         }
      }
      inputChains.clear();

      if(eightBitThreeBandChain != null)
      {
         eightBitThreeBandChain.delete();
         eightBitThreeBandChain = null;
      }
      
      if(mosaic != null)
      {
         mosaic.delete();
         mosaic = null;
      }
      
      if(blankSource != null)
      {
         blankSource.delete();
         blankSource = null;
      }
      if(cacheSource != null)
      {
         cacheSource.delete();
         cacheSource = null;
      }
      if(wmsView != null)
      {
         wmsView.delete();
         wmsView = null;
      }
      if(imageHandlerRegistry != null)
      {
         imageHandlerRegistry.delete();
         imageHandlerRegistry = null;
      }
   }
}
