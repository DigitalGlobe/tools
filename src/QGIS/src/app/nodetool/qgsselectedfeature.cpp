/***************************************************************************
    qgsselectedfeature.cpp  - selected feature of nodetool
    ---------------------
    begin                : April 2009
    copyright            : (C) 2009 by Richard Kostecky
    email                : csf dot kostej at mail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "nodetool/qgsselectedfeature.h"
#include "nodetool/qgsvertexentry.h"

#include "qgspointv2.h"
#include <qgslogger.h>
#include <qgsvertexmarker.h>
#include <qgsgeometryvalidator.h>
#include <qgsvectorlayer.h>
#include <qgsrubberband.h>
#include <qgisapp.h>
#include <qgsmaprenderer.h>
#include <qgslayertreeview.h>
#include <qgsproject.h>

QgsSelectedFeature::QgsSelectedFeature( QgsFeatureId featureId,
                                        QgsVectorLayer *vlayer,
                                        QgsMapCanvas *canvas )
    : mFeatureId( featureId )
    , mGeometry( nullptr )
    , mChangingGeometry( false )
    , mValidator( nullptr )
{
  QgsDebugCall;
  setSelectedFeature( featureId, vlayer, canvas );
}

QgsSelectedFeature::~QgsSelectedFeature()
{
  QgsDebugCall;

  deleteVertexMap();

  while ( !mGeomErrorMarkers.isEmpty() )
  {
    delete mGeomErrorMarkers.takeFirst();
  }

  if ( mValidator )
  {
    mValidator->stop();
    mValidator->wait();
    mValidator->deleteLater();
    mValidator = nullptr;
  }

  delete mGeometry;
}

void QgsSelectedFeature::currentLayerChanged( QgsMapLayer *layer )
{
  QgsDebugCall;
  if ( layer == mVlayer )
    deleteLater();
}

void QgsSelectedFeature::updateGeometry( QgsGeometry *geom )
{
  QgsDebugCall;

  delete mGeometry;

  if ( !geom )
  {
    QgsFeature f;
    mVlayer->getFeatures( QgsFeatureRequest().setFilterFid( mFeatureId ) ).nextFeature( f );
    if ( f.geometry() )
      mGeometry = new QgsGeometry( *f.geometry() );
    else
      mGeometry = new QgsGeometry();
  }
  else
  {
    mGeometry = new QgsGeometry( *geom );
  }
}

void QgsSelectedFeature::setSelectedFeature( QgsFeatureId featureId, QgsVectorLayer* vlayer, QgsMapCanvas* canvas )
{
  mFeatureId = featureId;
  mVlayer = vlayer;
  mCanvas = canvas;

  delete mGeometry;
  mGeometry = nullptr;

  // signal changing of current layer
  connect( QgisApp::instance()->layerTreeView(), SIGNAL( currentLayerChanged( QgsMapLayer* ) ), this, SLOT( currentLayerChanged( QgsMapLayer* ) ) );

  // feature was deleted
  connect( mVlayer, SIGNAL( featureDeleted( QgsFeatureId ) ), this, SLOT( featureDeleted( QgsFeatureId ) ) );

  // rolling back
  connect( mVlayer, SIGNAL( beforeRollBack() ), this, SLOT( beforeRollBack() ) );

  // projection or extents changed
  connect( canvas, SIGNAL( destinationCrsChanged() ), this, SLOT( updateVertexMarkersPosition() ) );
  connect( canvas, SIGNAL( extentsChanged() ), this, SLOT( updateVertexMarkersPosition() ) );

  // geometry was changed
  connect( mVlayer, SIGNAL( geometryChanged( QgsFeatureId, QgsGeometry & ) ), this, SLOT( geometryChanged( QgsFeatureId, QgsGeometry & ) ) );

  replaceVertexMap();
}

void QgsSelectedFeature::beforeRollBack()
{
  QgsDebugCall;
  disconnect( mVlayer, SIGNAL( geometryChanged( QgsFeatureId, QgsGeometry & ) ), this, SLOT( geometryChanged( QgsFeatureId, QgsGeometry & ) ) );
  deleteVertexMap();
}

void QgsSelectedFeature::beginGeometryChange()
{
  Q_ASSERT( !mChangingGeometry );
  mChangingGeometry = true;

  disconnect( mVlayer, SIGNAL( geometryChanged( QgsFeatureId, QgsGeometry & ) ), this, SLOT( geometryChanged( QgsFeatureId, QgsGeometry & ) ) );
}

void QgsSelectedFeature::endGeometryChange()
{
  Q_ASSERT( mChangingGeometry );
  mChangingGeometry = false;

  connect( mVlayer, SIGNAL( geometryChanged( QgsFeatureId, QgsGeometry & ) ), this, SLOT( geometryChanged( QgsFeatureId, QgsGeometry & ) ) );
}

void QgsSelectedFeature::canvasLayersChanged()
{
  currentLayerChanged( mCanvas->currentLayer() );
}

void QgsSelectedFeature::featureDeleted( QgsFeatureId fid )
{
  if ( fid == mFeatureId )
    deleteLater();
}

void QgsSelectedFeature::geometryChanged( QgsFeatureId fid, QgsGeometry &geom )
{
  QgsDebugCall;

  if ( !mVlayer || fid != mFeatureId )
    return;

  updateGeometry( &geom );

  replaceVertexMap();
}

void QgsSelectedFeature::validateGeometry( QgsGeometry *g )
{
  QgsDebugCall;
  QSettings settings;
  if ( settings.value( "/qgis/digitizing/validate_geometries", 1 ).toInt() == 0 )
    return;

  if ( !g )
    g = mGeometry;

  mTip.clear();

  if ( mValidator )
  {
    mValidator->stop();
    mValidator->wait();
    mValidator->deleteLater();
    mValidator = nullptr;
  }

  mGeomErrors.clear();
  while ( !mGeomErrorMarkers.isEmpty() )
  {
    QgsVertexMarker *vm = mGeomErrorMarkers.takeFirst();
    QgsDebugMsg( "deleting " + vm->toolTip() );
    delete vm;
  }

  mValidator = new QgsGeometryValidator( g );
  connect( mValidator, SIGNAL( errorFound( QgsGeometry::Error ) ), this, SLOT( addError( QgsGeometry::Error ) ) );
  connect( mValidator, SIGNAL( finished() ), this, SLOT( validationFinished() ) );
  mValidator->start();

  QStatusBar *sb = QgisApp::instance()->statusBar();
  sb->showMessage( tr( "Validation started." ) );
}

void QgsSelectedFeature::addError( QgsGeometry::Error e )
{
  mGeomErrors << e;
  if ( !mTip.isEmpty() )
    mTip += '\n';
  mTip += e.what();

  if ( e.hasWhere() )
  {
    QgsVertexMarker *marker = new QgsVertexMarker( mCanvas );
    marker->setCenter( mCanvas->mapSettings().layerToMapCoordinates( mVlayer, e.where() ) );
    marker->setIconType( QgsVertexMarker::ICON_X );
    marker->setColor( Qt::green );
    marker->setZValue( marker->zValue() + 1 );
    marker->setPenWidth( 2 );
    marker->setToolTip( e.what() );
    mGeomErrorMarkers << marker;
  }

  QStatusBar *sb = QgisApp::instance()->statusBar();
  sb->showMessage( e.what() );
  sb->setToolTip( mTip );
}

void QgsSelectedFeature::validationFinished()
{
  QStatusBar *sb = QgisApp::instance()->statusBar();
  sb->showMessage( tr( "Validation finished (%n error(s) found).", "number of geometry errors", mGeomErrorMarkers.size() ) );
}

void QgsSelectedFeature::deleteSelectedVertexes()
{
  int nSelected = 0;
  Q_FOREACH ( QgsVertexEntry *entry, mVertexMap )
  {
    if ( entry->isSelected() )
      nSelected++;
  }

  if ( nSelected == 0 )
    return;

  int topologicalEditing = QgsProject::instance()->readNumEntry( "Digitizing", "/TopologicalEditing", 0 );
  QMultiMap<double, QgsSnappingResult> currentResultList;

  mVlayer->beginEditCommand( QObject::tr( "Deleted vertices" ) );

  beginGeometryChange();

  bool success = false;
  QgsVectorLayer::EditResult res = QgsVectorLayer::Success;
  for ( int i = mVertexMap.size() - 1; i > -1 && nSelected > 0; i-- )
  {
    if ( mVertexMap.at( i )->isSelected() )
    {
      if ( topologicalEditing )
      {
        // snap from current vertex
        currentResultList.clear();
        mVlayer->snapWithContext( mVertexMap.at( i )->pointV1(), ZERO_TOLERANCE, currentResultList, QgsSnapper::SnapToVertex );
      }

      // only last update should trigger the geometry update
      // as vertex selection gets lost on the update
      if ( --nSelected == 0 )
        endGeometryChange();

      if ( res != QgsVectorLayer::EmptyGeometry )
        res = mVlayer->deleteVertexV2( mFeatureId, i );

      if ( res != QgsVectorLayer::Success && res != QgsVectorLayer::EmptyGeometry )
      {
        success = false;
        QgsDebugMsg( QString( "Deleting vertex %1 failed - resetting" ).arg( i ) );
        break;
      }

      success = true;

      if ( topologicalEditing )
      {
        QMultiMap<double, QgsSnappingResult>::iterator resultIt =  currentResultList.begin();

        for ( ; resultIt != currentResultList.end(); ++resultIt )
        {
          // move all other
          if ( mFeatureId !=  resultIt.value().snappedAtGeometry )
            mVlayer->deleteVertexV2( resultIt.value().snappedAtGeometry, resultIt.value().snappedVertexNr );
        }
      }

      if ( res == QgsVectorLayer::EmptyGeometry )
      {
        //geometry has been cleared as a result of deleting vertices (eg not enough vertices left to leave a valid geometry),
        //so nothing more to do
        QgsGeometry empty;
        geometryChanged( mFeatureId, empty );
        break;
      }
    }
  }

  if ( nSelected > 0 )
    endGeometryChange();

  if ( success )
  {
    mVlayer->endEditCommand();
  }
  else
  {
    mVlayer->destroyEditCommand();
  }
}

void QgsSelectedFeature::moveSelectedVertexes( QgsVector v )
{
  int nUpdates = 0;
  Q_FOREACH ( QgsVertexEntry *entry, mVertexMap )
  {
    if ( entry->isSelected() )
      nUpdates++;
  }

  if ( nUpdates == 0 )
    return;

  mVlayer->beginEditCommand( QObject::tr( "Moved vertices" ) );
  int topologicalEditing = QgsProject::instance()->readNumEntry( "Digitizing", "/TopologicalEditing", 0 );

  beginGeometryChange();

  QMultiMap<double, QgsSnappingResult> currentResultList;
  for ( int i = mVertexMap.size() - 1; i > -1 && nUpdates > 0; i-- )
  {
    QgsVertexEntry *entry = mVertexMap.value( i, nullptr );
    if ( !entry || !entry->isSelected() )
      continue;

    if ( topologicalEditing )
    {
      // snap from current vertex
      currentResultList.clear();
      mVlayer->snapWithContext( entry->pointV1(), ZERO_TOLERANCE, currentResultList, QgsSnapper::SnapToVertex );
    }

    // only last update should trigger the geometry update
    // as vertex selection gets lost on the update
    if ( --nUpdates == 0 )
      endGeometryChange();

    QgsPointV2 p = entry->point();
    p.setX( p.x() + v.x() );
    p.setY( p.y() + v.y() );
    mVlayer->moveVertex( p, mFeatureId, i );

    if ( topologicalEditing )
    {
      QMultiMap<double, QgsSnappingResult>::iterator resultIt =  currentResultList.begin();

      for ( ; resultIt != currentResultList.end(); ++resultIt )
      {
        // move all other
        if ( mFeatureId !=  resultIt.value().snappedAtGeometry )
          mVlayer->moveVertex( p, resultIt.value().snappedAtGeometry, resultIt.value().snappedVertexNr );
      }
    }
  }

  if ( nUpdates > 0 )
    endGeometryChange();

  mVlayer->endEditCommand();
}

void QgsSelectedFeature::replaceVertexMap()
{
  // delete old map
  deleteVertexMap();

  // create new map
  createVertexMap();

  // validate the geometry
  validateGeometry();

  emit vertexMapChanged();
}

void QgsSelectedFeature::deleteVertexMap()
{
  Q_FOREACH ( QgsVertexEntry *entry, mVertexMap )
  {
    delete entry;
  }

  mVertexMap.clear();
}

bool QgsSelectedFeature::isSelected( int vertexNr )
{
  return mVertexMap.at( vertexNr )->isSelected();
}

QgsGeometry *QgsSelectedFeature::geometry()
{
  Q_ASSERT( mGeometry );
  return mGeometry;
}

void QgsSelectedFeature::createVertexMap()
{

  if ( !mGeometry )
  {
    QgsDebugMsg( "Loading feature" );
    updateGeometry( nullptr );
  }

  if ( !mGeometry )
  {
    return;
  }

  const QgsAbstractGeometryV2* geom = mGeometry->geometry();
  if ( !geom )
  {
    return;
  }

  QgsVertexId vertexId;
  QgsPointV2 pt;
  while ( geom->nextVertex( vertexId, pt ) )
  {
    mVertexMap.append( new QgsVertexEntry( mCanvas, mVlayer, pt, vertexId, tr( "ring %1, vertex %2" ).arg( vertexId.ring ).arg( vertexId.vertex ) ) );
  }
}

void QgsSelectedFeature::selectVertex( int vertexNr )
{
  if ( vertexNr < 0 || vertexNr >= mVertexMap.size() )
    return;

  QgsVertexEntry *entry = mVertexMap.at( vertexNr );
  entry->setSelected();

  emit selectionChanged();
}

void QgsSelectedFeature::deselectVertex( int vertexNr )
{
  if ( vertexNr < 0 || vertexNr >= mVertexMap.size() )
    return;

  QgsVertexEntry *entry = mVertexMap.at( vertexNr );
  entry->setSelected( false );

  emit selectionChanged();
}

void QgsSelectedFeature::deselectAllVertexes()
{
  for ( int i = 0; i < mVertexMap.size(); i++ )
  {
    mVertexMap.at( i )->setSelected( false );
  }
  emit selectionChanged();
}

void QgsSelectedFeature::invertVertexSelection( int vertexNr )
{
  if ( vertexNr < 0 || vertexNr >= mVertexMap.size() )
    return;

  QgsVertexEntry *entry = mVertexMap.at( vertexNr );

  bool selected = !entry->isSelected();

  entry->setSelected( selected );
  emit selectionChanged();
}

void QgsSelectedFeature::invertVertexSelection( QVector<int> vertexIndices )
{
  Q_FOREACH ( int index, vertexIndices )
  {
    if ( index < 0 || index >= mVertexMap.size() )
      continue;

    QgsVertexEntry *entry = mVertexMap.at( index );
    entry->setSelected( !entry->isSelected() );
  }
  emit selectionChanged();
}

void QgsSelectedFeature::updateVertexMarkersPosition()
{
  Q_FOREACH ( QgsVertexEntry* vertexEntry, mVertexMap )
  {
    vertexEntry->placeMarker();
  }
}

QgsFeatureId QgsSelectedFeature::featureId()
{
  return mFeatureId;
}

QList<QgsVertexEntry*> &QgsSelectedFeature::vertexMap()
{
  return mVertexMap;
}

QgsVectorLayer* QgsSelectedFeature::vlayer()
{
  return mVlayer;
}
