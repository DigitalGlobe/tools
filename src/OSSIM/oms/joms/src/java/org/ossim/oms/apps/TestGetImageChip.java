package org.ossim.oms.apps;

public class TestGetImageChip
{
  static
  {
    System.loadLibrary( "joms" );
  }

  /**
   * @param args
   */
  public static void main( String[] args )
  {
    // if ( args.length > 0 )
    // {
    // Init.instance().initialize( args.length, args );
    // }
    //
    // StringVector list = new StringVector();
    //
    // Info.instance().getWriterList( list );
    //
    // // StringVector::const_iterator i = list.begin();
    // // cout << "writer list:" << endl;
    // // while (i != list.end())
    // // {
    // // cout << (*i).c_str() << endl;
    // // ++i;
    // // }
    //
    // RasterEngine re = new RasterEngine();
    // int resLevel = 1;
    //
    // UintVector bandList = new UintVector();
    // String inputImage = "/data/mer1/image_formats/tiff/point.tif";
    // // String writerType = "tiff_tiled_band_separate";
    // String writerType = "gdal_PNG";
    // String outputFile = "/tmp/foo10.png";
    //
    // System.out.println( "Calling RasterEngine::getImageChip..." );
    //
    // Ipt start = new Ipt( 25, 25 );
    // Ipt stop = new Ipt( 400, 400 );
    // Irect requestedRect = new Irect( start, stop );
    //
    // if ( re.getImageChip( inputImage, requestedRect, resLevel, bandList,
    // writerType, outputFile ) == true )
    // {
    // System.out.println( "Wrote file: " + outputFile );
    // }
    // else
    // {
    // System.out.println( "bummer..." );
    // }
    //
  }

  // private static void printFoo( ossimImageHandler handler )
  // {
  // System.out.println( new ToStringBuilder( handler ).append( "tileWidth",
  // handler.getTileWidth() ).append( "tileHeight", handler.getTileHeight() )
  // .append( "width", handler.getBoundingRect().width() ).append( "height",
  // handler.getBoundingRect().height() ).append(
  // "numberOfReducedresSets", handler.getNumberOfReducedResSets() )
  // .append( "numBands", handler.getNumberOfOutputBands() ).append( "open",
  // handler.isOpen() ).toString() );
  // }

  // public static void printFoo( RenderedImage image )
  // {
  // System.out.println( new ToStringBuilder( image ).append( "tileWidth",
  // image.getTileWidth() ).append( "tileHeight", image.getTileHeight() )
  // .append( "width", image.getWidth() ).append( "height",
  // image.getHeight() ).append( "numXTiles", image.getNumXTiles() )
  // .append( "numYTiles", image.getNumYTiles() ) );
  // }
}
