//-----------------------------------------------------------------------------
// File:  Mosaic.java
//
// License:  See top level LICENSE.txt file.
//
// Description: 
//
//-----------------------------------------------------------------------------
// $Id: Mosaic.java 19748 2011-06-12 15:34:56Z dburken $

package org.ossim.jni.core;

//******************************************************************************
//**  Mosaic Class
//******************************************************************************
/**
 *   Used to construct and execute an image chain used to generate an ortho
 *   mosaic. 
 *
 ******************************************************************************/

public class Mosaic {


  //**************************************************************************
  //** Constructor
  //**************************************************************************
  /** Creates a new instance of Mosaic.  */

    public Mosaic(){

    }


  //**************************************************************************
  //** addImage
  //**************************************************************************
  /** Used to add an image to the mosaic image chain.
   */
    public void addImage(Image image){
    }


    
  //**************************************************************************
  //** addVector
  //**************************************************************************
  /** Used to add a vector layer to the mosaic image chain.
   */
    public void addVector(Vector vector){
    }



  //**************************************************************************
  //** setMask
  //**************************************************************************
  /** Set the mask source for the mosaic.
   */
    public void setMask(Vector vector){
    }


  //**************************************************************************
  //** getImage
  //**************************************************************************
  /** Creates an orthoimage mosaic using the input layers.
   *  @param bbox The lower left and upper right lat/lon coordinates
   *  @param width The output width, in pixels
   *  @param height The output height, in pixels
   *  @param srs The output projection (e.g. "EPSG:4326")
   *  @param format Output format
   */

    public java.io.InputStream getImage(double[] bbox, int width, int height, String srs, String[] writerProps, String format){
        return null;
    }


  //**************************************************************************
  //** getImage
  //**************************************************************************
  /** Creates an orthoimage mosaic using the input layers.
   *  @param options Orthoigen command line args
   *  @param format Output format
   */

    public java.io.InputStream getImage(String[] options, String format){
        return null;
    }


  //**************************************************************************
  //** destroy
  //**************************************************************************
  /** Used to destroy the image chain and release handles to the input image
   *  and vector data.
   */
    public void destroy(){
    }




}
