/***************************************************************************
    qgsgrassregion.h  -  Edit region
                             -------------------
    begin                : August, 2004
    copyright            : (C) 2004 by Radim Blazek
    email                : blazek@itc.it
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSGRASSREGION_H
#define QGSGRASSREGION_H

#include "ui_qgsgrassregionbase.h"
#include "qgscoordinatereferencesystem.h"
#include "qgscoordinatetransform.h"
#include "qgsmaptool.h"
#include "qgsrubberband.h"
#include "qgspoint.h"

class QgsGrassPlugin;
class QgsGrassRegionEdit;

class QgisInterface;
class QgsMapCanvas;
class QgsRectangle;

class QButtonGroup;

extern "C"
{
#include <grass/gis.h>
}

/** \class QgsGrassRegion
 *  \brief GRASS attributes.
 *
 */
class QgsGrassRegion: public QWidget, private Ui::QgsGrassRegionBase
{
    Q_OBJECT

  public:
    //! Constructor
    QgsGrassRegion( QgisInterface *iface,
                    QWidget * parent = 0, Qt::WindowFlags f = 0 );

    //! Destructor
    ~QgsGrassRegion();

  public slots:
    void buttonClicked( QAbstractButton *button );

    void mapsetChanged();

    void reloadRegion();

    //! Mouse event receiver
    //void mouseEventReceiverMove ( QgsPoint & );

    //! Mouse event receiver
    //void mouseEventReceiverClick ( QgsPoint & );

    //! Calculate region, called if any value is changed
    void adjust( void );

    //! Value in GUI was changed
    void northChanged();
    void southChanged();
    void eastChanged();
    void westChanged();
    void NSResChanged();
    void EWResChanged();
    void rowsChanged();
    void colsChanged();

    void radioChanged( void );

    //! Called when the capture finished to refresh the mWindow values
    void onCaptureFinished();

    void on_mDrawButton_clicked();

    void canvasMapToolSet( QgsMapTool *tool );
  private:
    //! Pointer to plugin
    //QgsGrassPlugin *mPlugin;

    //! Pointer to QGIS interface
    QgisInterface *mInterface;

    //! Pointer to canvas
    QgsMapCanvas *mCanvas;

    QButtonGroup *mRadioGroup;

    //! Current new region
    struct Cell_head mWindow;

    QgsCoordinateReferenceSystem mCrs;

    //! Display current state of new region in XOR mode
    void displayRegion( void );

    void readRegion();


    // Set region values in GUI from mWindow
    void refreshGui();

    //! First corner coordinates
    double mX;
    double mY;

    //! Currently updating GUI, don't run *Changed methods
    bool mUpdatingGui;

    // Format N, S, E, W value
    QString formatExtent( double v );
    QString formatResolution( double v );

    QgsGrassRegionEdit* mRegionEdit;
};

/** Map tool which uses rubber band for changing grass region */
class QgsGrassRegionEdit : public QgsMapTool
{
    Q_OBJECT

  public:
    explicit QgsGrassRegionEdit( QgsMapCanvas* );

    ~QgsGrassRegionEdit();

    //! mouse pressed in map canvas
    void canvasPressEvent( QgsMapMouseEvent * ) override;

    //! mouse movement in map canvas
    void canvasMoveEvent( QgsMapMouseEvent * ) override;

    //! mouse released
    void canvasReleaseEvent( QgsMapMouseEvent * ) override;


    //! called when map tool is about to get inactive
    void deactivate() override;

    //! get the rectangle
    QgsRectangle getRegion();

    //! refresh the rectangle displayed in canvas
    void setRegion( const QgsPoint&, const QgsPoint& );
    void setSrcRegion( const QgsRectangle &rect );

    static void drawRegion( QgsMapCanvas *canvas, QgsRubberBand* rubberBand, const QgsRectangle &rect, QgsCoordinateTransform *coordinateTransform = 0, bool isPolygon = false );
    void calcSrcRegion();
    static void transform( QgsMapCanvas *canvas, QVector<QgsPoint> &points, QgsCoordinateTransform *coordinateTransform, QgsCoordinateTransform::TransformDirection direction = QgsCoordinateTransform::ForwardTransform );

  signals:
    void captureStarted();
    void captureEnded();

  public slots:
    void setTransform();

  private:
    //! Rubber band for selecting grass region
    QgsRubberBand* mRubberBand;
    QgsRubberBand* mSrcRubberBand;

    //! Status of input from canvas
    bool mDraw;

    //! First rectangle point
    QgsPoint mStartPoint;
    //! Last rectangle point
    QgsPoint mEndPoint;

    //! Region rectangle in source CRS
    QgsRectangle mSrcRectangle;

    QgsCoordinateReferenceSystem mCrs;
    QgsCoordinateTransform mCoordinateTransform;
};

#endif // QGSGRASSREGION_H
