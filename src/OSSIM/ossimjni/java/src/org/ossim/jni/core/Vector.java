//-----------------------------------------------------------------------------
// File:  Vector.java
//
// License:  See top level LICENSE.txt file.
//
// Description: 
//
//-----------------------------------------------------------------------------
// $Id: Vector.java 19748 2011-06-12 15:34:56Z dburken $

package org.ossim.jni.core;

//******************************************************************************
//**  Vector Class
//******************************************************************************
/**
 *   Used to represent an OGR vector layer
 *
 ******************************************************************************/

public class Vector {


  //**************************************************************************
  //** Constructor
  //**************************************************************************
  /** Creates a new instance of a vector
   *  @param url Path to the vector data
   *  @param layer The name of the layer to use
   */
    public Vector(String url, String layer){

    }

  //**************************************************************************
  //** selectFeatures
  //**************************************************************************
  /** Used to subselect features in the layer using a where clause. In ArcGIS
   *  parlance, this is called a definition query.
   */
    public void selectFeatures(String where){
    }


  //**************************************************************************
  //** setLineColor
  //**************************************************************************
  /** Used to set the line color using an RGB value and an optional opacity
   *  filter.
   */
    public void setLineColor(int r, int g, int b, double opacity){
    }


  //**************************************************************************
  //** setLineWidth
  //**************************************************************************
  /** Used to set the line width in pixels.
   */
    public void setLineWidth(int width){
    }

    
  //**************************************************************************
  //** setFillColor
  //**************************************************************************
  /** Used to set the fill color for polygons.
   */
    public void setFillColor(int r, int g, int b, double opacity){
    }

    

}
