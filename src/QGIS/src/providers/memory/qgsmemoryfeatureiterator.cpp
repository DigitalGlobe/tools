/***************************************************************************
    qgsmemoryfeatureiterator.cpp
    ---------------------
    begin                : Juli 2012
    copyright            : (C) 2012 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qgsmemoryfeatureiterator.h"
#include "qgsmemoryprovider.h"

#include "qgsgeometry.h"
#include "qgslogger.h"
#include "qgsspatialindex.h"
#include "qgsmessagelog.h"



QgsMemoryFeatureIterator::QgsMemoryFeatureIterator( QgsMemoryFeatureSource* source, bool ownSource, const QgsFeatureRequest& request )
    : QgsAbstractFeatureIteratorFromSource<QgsMemoryFeatureSource>( source, ownSource, request )
    , mSelectRectGeom( nullptr )
    , mSubsetExpression( nullptr )
{
  if ( !mSource->mSubsetString.isEmpty() )
  {
    mSubsetExpression = new QgsExpression( mSource->mSubsetString );
    mSubsetExpression->prepare( &mSource->mExpressionContext );
  }

  if ( !mRequest.filterRect().isNull() && mRequest.flags() & QgsFeatureRequest::ExactIntersect )
  {
    mSelectRectGeom = QgsGeometry::fromRect( request.filterRect() );
  }

  // if there's spatial index, use it!
  // (but don't use it when selection rect is not specified)
  if ( !mRequest.filterRect().isNull() && mSource->mSpatialIndex )
  {
    mUsingFeatureIdList = true;
    mFeatureIdList = mSource->mSpatialIndex->intersects( mRequest.filterRect() );
    QgsDebugMsg( "Features returned by spatial index: " + QString::number( mFeatureIdList.count() ) );
  }
  else if ( mRequest.filterType() == QgsFeatureRequest::FilterFid )
  {
    mUsingFeatureIdList = true;
    QgsFeatureMap::const_iterator it = mSource->mFeatures.constFind( mRequest.filterFid() );
    if ( it != mSource->mFeatures.constEnd() )
      mFeatureIdList.append( mRequest.filterFid() );
  }
  else
  {
    mUsingFeatureIdList = false;
  }

  rewind();
}

QgsMemoryFeatureIterator::~QgsMemoryFeatureIterator()
{
  close();

  delete mSubsetExpression;
}


bool QgsMemoryFeatureIterator::fetchFeature( QgsFeature& feature )
{
  feature.setValid( false );

  if ( mClosed )
    return false;

  if ( mUsingFeatureIdList )
    return nextFeatureUsingList( feature );
  else
    return nextFeatureTraverseAll( feature );
}


bool QgsMemoryFeatureIterator::nextFeatureUsingList( QgsFeature& feature )
{
  bool hasFeature = false;

  // option 1: we have a list of features to traverse
  while ( mFeatureIdListIterator != mFeatureIdList.constEnd() )
  {
    if ( !mRequest.filterRect().isNull() && mRequest.flags() & QgsFeatureRequest::ExactIntersect )
    {
      // do exact check in case we're doing intersection
      if ( mSource->mFeatures.value( *mFeatureIdListIterator ).constGeometry() && mSource->mFeatures.value( *mFeatureIdListIterator ).constGeometry()->intersects( mSelectRectGeom ) )
        hasFeature = true;
    }
    else
      hasFeature = true;

    if ( mSubsetExpression )
    {
      mSource->mExpressionContext.setFeature( mSource->mFeatures.value( *mFeatureIdListIterator ) );
      if ( !mSubsetExpression->evaluate( &mSource->mExpressionContext ).toBool() )
        hasFeature = false;
    }

    if ( hasFeature )
      break;

    ++mFeatureIdListIterator;
  }

  // copy feature
  if ( hasFeature )
  {
    feature = mSource->mFeatures.value( *mFeatureIdListIterator );
    ++mFeatureIdListIterator;
  }
  else
    close();

  if ( hasFeature )
    feature.setFields( mSource->mFields ); // allow name-based attribute lookups

  return hasFeature;
}


bool QgsMemoryFeatureIterator::nextFeatureTraverseAll( QgsFeature& feature )
{
  bool hasFeature = false;

  // option 2: traversing the whole layer
  while ( mSelectIterator != mSource->mFeatures.constEnd() )
  {
    if ( mRequest.filterRect().isNull() )
    {
      // selection rect empty => using all features
      hasFeature = true;
    }
    else
    {
      if ( mRequest.flags() & QgsFeatureRequest::ExactIntersect )
      {
        // using exact test when checking for intersection
        if ( mSelectIterator->constGeometry() && mSelectIterator->constGeometry()->intersects( mSelectRectGeom ) )
          hasFeature = true;
      }
      else
      {
        // check just bounding box against rect when not using intersection
        if ( mSelectIterator->constGeometry() && mSelectIterator->constGeometry()->boundingBox().intersects( mRequest.filterRect() ) )
          hasFeature = true;
      }
    }

    if ( mSubsetExpression )
    {
      mSource->mExpressionContext.setFeature( *mSelectIterator );
      if ( !mSubsetExpression->evaluate( &mSource->mExpressionContext ).toBool() )
        hasFeature = false;
    }

    if ( hasFeature )
      break;

    ++mSelectIterator;
  }

  // copy feature
  if ( hasFeature )
  {
    feature = mSelectIterator.value();
    ++mSelectIterator;
    feature.setValid( true );
    feature.setFields( mSource->mFields ); // allow name-based attribute lookups
  }
  else
    close();

  return hasFeature;
}

bool QgsMemoryFeatureIterator::rewind()
{
  if ( mClosed )
    return false;

  if ( mUsingFeatureIdList )
    mFeatureIdListIterator = mFeatureIdList.constBegin();
  else
    mSelectIterator = mSource->mFeatures.constBegin();

  return true;
}

bool QgsMemoryFeatureIterator::close()
{
  if ( mClosed )
    return false;

  iteratorClosed();

  delete mSelectRectGeom;
  mSelectRectGeom = nullptr;

  mClosed = true;
  return true;
}

// -------------------------

QgsMemoryFeatureSource::QgsMemoryFeatureSource( const QgsMemoryProvider* p )
    : mFields( p->mFields )
    , mFeatures( p->mFeatures )
    , mSpatialIndex( p->mSpatialIndex ? new QgsSpatialIndex( *p->mSpatialIndex ) : nullptr )  // just shallow copy
    , mSubsetString( p->mSubsetString )
{
  mExpressionContext << QgsExpressionContextUtils::globalScope()
  << QgsExpressionContextUtils::projectScope();
  mExpressionContext.setFields( mFields );
}

QgsMemoryFeatureSource::~QgsMemoryFeatureSource()
{
  delete mSpatialIndex;
}

QgsFeatureIterator QgsMemoryFeatureSource::getFeatures( const QgsFeatureRequest& request )
{
  return QgsFeatureIterator( new QgsMemoryFeatureIterator( this, false, request ) );
}
