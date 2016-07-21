package org.ossim.oms.ui;

/*
 * %W% %E% %U%
 *
 * Copyright (c) 2000 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Sun grants you ("Licensee") a non-exclusive, royalty free, license to use,
 * modify and redistribute this software in source and binary code form,
 * provided that i) this copyright notice and license appear on all copies of
 * the software; and ii) Licensee does not utilize the software in a manner
 * which is disparaging to Sun.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY
 * IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE
 * LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS
 * LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT,
 * INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF
 * OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * This software is not designed or intended for use in on-line control of
 * aircraft, air traffic, aircraft navigation or aircraft communications; or in
 * the design, construction, operation or maintenance of any nuclear
 * facility. Licensee represents and warrants that it will not use or
 * redistribute the Software for such purposes.
 */

/**
 *  Example display class Swing Component that is able
 *  to contain an image.  The size of the image
 *  and size of the container can be different.
 *  The image can be positioned within the
 *  container.  This class extends JPanel in order
 *  to support layout management.  Tiling is supported
 *  as of JDK1.3 via drawRenderedImage().
 *
 *  @author Dennis Sigel
 *
 *  @see javax.swing.JPanel
 *  @see javax.swing.JComponent
 *  @see java.awt.image.RenderedImage
 */

import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Dimension;
import java.awt.geom.AffineTransform;
import java.awt.image.RenderedImage;
import javax.swing.*;

public class ImagePanel extends JPanel
{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	protected AffineTransform affineTrans;
	protected JScrollPane sp;

	/** image to display */
	protected RenderedImage source = null;

	/** image origin relative to panel origin */
	protected int originX = 0;
	protected int originY = 0;

	protected double sx, sy;

	protected boolean scale = false;

	/** default constructor */
	public ImagePanel()
	{
		super();
		setLayout( null );
	}

	/** constructor with given image */
	public ImagePanel( RenderedImage image )
	{
		super();
		setLayout( null );
		source = image;
		setPreferredSize( new Dimension( source.getWidth(), source.getHeight() ) );
	}

	public void scale( double scaleFactor )
	{
		scale = true;
		sx = scaleFactor;
		sy = scaleFactor;
		repaint();
		setPreferredSize( new Dimension( (int)( sx * source.getWidth() ),
				(int)( sy * source.getHeight() ) ) );
		sp.revalidate();
	}

	/** move image within it's container */
	public void setOrigin( int x, int y )
	{
		originX = x;
		originY = y;
		repaint();
	}

	/** get the image origin */
	public Point getOrigin()
	{
		return new Point( originX, originY );
	}

	/** use to display a new image */
	public void setImage( RenderedImage im )
	{
		source = im;
		repaint();
	}

	public void setScrollPane( JScrollPane scrollP )
	{
		sp = scrollP;
	}

	/** @returns the Image */
	public RenderedImage getImage()
	{
		return source;
	}

	/** paint routine */
	public synchronized void paintComponent( Graphics g )
	{

		Graphics2D g2d = (Graphics2D)g;

		// empty component (no image)
		if ( source == null )
		{
			g2d.setColor( getBackground() );
			g2d.fillRect( 0, 0, getWidth(), getHeight() );
			return;
		}

		// account for borders
		Insets insets = getInsets();
		int tx = insets.left + originX;
		int ty = insets.top + originY;

		// clear damaged component area
		Rectangle clipBounds = g2d.getClipBounds();
		g2d.setColor( getBackground() );
		g2d.fillRect( clipBounds.x, clipBounds.y, clipBounds.width,
				clipBounds.height );

		/**
		 * Translation moves the entire image within the container
		 */
		affineTrans = new AffineTransform();
		affineTrans.setTransform( AffineTransform.getTranslateInstance( tx, ty ) );
		if ( ( sx != 0 ) && ( sy != 0 ) )
			affineTrans.scale( sx, sy );

		g2d.drawRenderedImage( source, affineTrans );
	}
}
