package org.ossim.oms.util;

import java.awt.image.*;
import java.awt.*;
import java.awt.geom.Rectangle2D;
import javax.imageio.ImageTypeSpecifier;

public class ImageGenerator {

  static Image createErrorImage(int w, int h, String message) {
    BufferedImage errorImage = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
    Graphics2D g = (Graphics2D)errorImage.getGraphics();
    FontMetrics fontMetrics= g.getFontMetrics();

    //float fontHeight = fontMetrics.height;
    String[] stringArray = message.split("\n");
    float originY = 0.0f;
    float originX = 0.0f;
    g.setPaint(Color.white);
    g.setStroke(new BasicStroke(1));

    for ( int i = 0; i < stringArray.length; i++ )
    {
        Rectangle2D bounds = fontMetrics.getStringBounds(stringArray[i], g);
        originY+=bounds.getHeight();
        originX = (float)(w/2.0f - (bounds.getWidth()/2.0f));
        if(originX < 0) originX = 0;
        g.drawString(stringArray[i], originX, originY);
    }
    //g.setPaint(Color.red);
    //g.setStroke(new BasicStroke(4));
    //g.drawLine(0, 0, w, h);
    //g.drawLine(w, 0, 0, h);

    return errorImage;
  }

  public static BufferedImage convertRGBAToIndexed(BufferedImage src)
  {
    BufferedImage dest = new BufferedImage(src.getWidth(), src.getHeight(), BufferedImage.TYPE_BYTE_INDEXED);
    Graphics g = dest.getGraphics();
    g.setColor(new Color(231, 20, 189));
    g.fillRect(0, 0, dest.getWidth(), dest.getHeight()); //fill with a hideous color and make it transparent
    dest = makeTransparent(dest, 0, 0);
    dest.createGraphics().drawImage(src, 0, 0, null);
    return dest;
  }

  public static BufferedImage makeTransparent(BufferedImage image, int x, int y) {
    ColorModel cm = image.getColorModel();
    if (!(cm instanceof IndexColorModel))
      return image; //sorry...
    IndexColorModel icm = (IndexColorModel) cm;
    WritableRaster raster = image.getRaster();
    int pixel = raster.getSample(x, y, 0); //pixel is offset in ICM's palette
    int size = icm.getMapSize();
    byte[] reds = new byte[size];
    byte[] greens = new byte[size];
    byte[] blues = new byte[size];
    icm.getReds(reds);
    icm.getGreens(greens);
    icm.getBlues(blues);
    IndexColorModel icm2 = new IndexColorModel(8, size, reds, greens, blues, pixel);
    return new BufferedImage(icm2, raster, image.isAlphaPremultiplied(), null);
  }


    public static RenderedImage convertToColorIndexModel( DataBuffer dataBuffer, int width, int height, boolean transparentFlag )
    {
        ImageTypeSpecifier isp = ImageTypeSpecifier.createGrayscale( 8, DataBuffer.TYPE_BYTE, false );
        ColorModel colorModel;
        SampleModel sampleModel = isp.getSampleModel( width, height );
        if ( !transparentFlag )
        {
          colorModel = isp.getColorModel();
        }
        else
        {
          int[] lut = new int[256];
          for ( int i = 0; i  < lut.length; i++ ) {
            lut[i] = ( ( 0xff << 24 ) | ( i << 16 ) | ( i << 8 ) | ( i ) );
          }
          lut[0] = 0xff000000;
          colorModel = new IndexColorModel( 8, lut.length, lut, 0, true, 0, DataBuffer.TYPE_BYTE );
        }
        WritableRaster raster = WritableRaster.createWritableRaster( sampleModel, dataBuffer, null );
        return new BufferedImage( colorModel, raster, false, null );
    }
}