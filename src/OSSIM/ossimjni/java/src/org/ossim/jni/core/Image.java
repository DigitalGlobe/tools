//-----------------------------------------------------------------------------
// File:  Image.java
//
// License:  See top level LICENSE.txt file.
//
// Description: 
//
//-----------------------------------------------------------------------------
// $Id: Image.java 19748 2011-06-12 15:34:56Z dburken $

package org.ossim.jni.core;

import org.ossim.jni.UintVector;
import org.ossim.jni.SingleImageChain;

import java.io.File;
import java.lang.String;

/**
 *  Image Class
 *
 *  Used to create an image source for an image chain.
 */
public class Image
{
   private SingleImageChain m_img;
   
   /** @brief default constructor */
   public Image()
   {
      m_img = new SingleImageChain();  
   }

   /**
    * @brief open method that takes an image file.
    *
    * Opens file and creates a simple chain with ossimImageHandler.
    *
    * @param file File to open.
    *
    * @return true on success, false on error.
    *
    * @note This will close previous chain if one was opened.
    */
   public boolean open(java.io.File file)
   {
      return m_img.open( file.getAbsolutePath() );
   }
   
   /**
    * @brief open method.
    * 
    * Creates a new instance of image using a single file source 
    *
    * @param file Image source
    * @param entry Which entry to use. Default is 0.
    */

   public boolean open(java.io.File file, int entry)
   {
      return m_img.open(file.getAbsolutePath(), entry);
   }

   /**
    *  @brief Constructor
    *
    *  Creates a new RGB image using 3 seperate image files. This is equivalent
    *  to a band merge. Input images should have a single band selected.
    *
    *  @param r Image to use as the red band
    *  @param g Image to use as the green band
    *  @param b Image to use as the blue band
    */

   /*
   public Image(Image r, Image g, Image b)
   {
   }
   */

   /**
    * @brief selectBands
    *
    * Used to select which bands to use.
    *
    * @note zero based.
    * 
    * @return true on success, false if not open or bands out of range. 
    */
   public boolean selectBands(int[] bands)
   {
      boolean result = false;
      int bandCount = bands.length;
      if ( bandCount > 0 )
      {
         UintVector bandList = new UintVector();
         for ( int i = 0; i < bandCount; ++i )
         {
            bandList.add(bands[i]);
         }
         result = m_img.selectBands(bandList);
      }
      return result;
   }

   /**
    * @brief getBandSelection
    *
    * @note Zero based.
    *
    * @return A list of bands selected to use in the output product.
    */
   public int[] getBandSelection()
   {
      int[] bands = null;
      
      UintVector bandList = m_img.getBandSelection();

      int bandCount = (int)bandList.size();
      System.out.println("bandCount: " + bandCount);
      if ( bandCount > 0 )
      {
         bands = new int[bandCount];
         for ( int i = 0; i < bandCount; ++i )
         {
            bands[i] = (int)bandList.get(i);
         }
      }
      return bands;
   }

   /**
    * @brief setHistogram
    *
    * Used to identify a histogram file associated with the source image. 
    *  @param his Fully qualified path to a histogram file.
    */
   public boolean setHistogram(java.io.File his)
   {
      String s = new String( his.getAbsolutePath() );
      return m_img.setHistogram( s );
   }

   /** @return The histogram file if open. */
   public java.io.File getHistogramFile()
   {
      String s = m_img.getHistogramFile();
      return new java.io.File(s);
   }

  /**
   * @brief setOverview
   *
   * Used to identify an overview file associated with the source image.
   * 
   * @param ovr Fully qualified path to a overview file.
   * @param mask Mask file associated with the histogram (optional).
   */
   public boolean setOverview(java.io.File ovr) // , java.io.File mask)
   {
      // Mask code currently not wired in:
      String s = new String( ovr.getAbsolutePath() );
      return m_img.setOverview( s );
   }

   /** @return The overview file if open. */
   public java.io.File getOverviewFile()
   {
      String s = m_img.getOverviewFile();
      return new java.io.File(s);
   }
   
   /**
    * @brief Sharpen
    * 
    * Used to sharpen the image using a 3x3 kernal.
    */
   public void sharpen()
   {
      
   }

   /**************************************************************************
    * @brief Stretch
    * 
    * Used to apply a histogram stretch to the image. 
    *  @param op Histogram operation to apply to the image. Options include,
    *  "auto-minmax", "match", "std-stretch", "stretch".
    *  @param params Inputs for the histogram operation.
    */
   public void stretch(String op, String[] params)
   {
      
   }
    
   /**
    * @brief Desaturate
    * 
    * Used to desaturate an image by a specified percentage (expressed as 
    * a double or float). The larger the percentage, the greater the 
    * desaturation and the "grayer" the image. Valid ranges are from 0-100.
    */    
   public void desaturate(double percent)
   {
   }

   /**
    * @brief setOpacity
    *
    * Used to adjust the opacity of the image by a specified percentage. The
    *  smaller the percentage, the less opaque the image. Valid ranges are from
    *  0-100. A value of 0 would result in a completely transparent image. A
    *  value of 100 would result in a completely opaque image (no transparency).
    */
   public void setOpacity(double percent)
   {
      
   }
   
} // End: class org.ossim.jni.core.Image
