/***************************************************************************
   qgsmaplayercombobox.cpp
    --------------------------------------
   Date                 : 01.04.2014
   Copyright            : (C) 2014 Denis Rouzaud
   Email                : denis.rouzaud@gmail.com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "qgsmaplayercombobox.h"
#include "qgsmaplayermodel.h"


QgsMapLayerComboBox::QgsMapLayerComboBox( QWidget *parent )
    : QComboBox( parent )
{
  mProxyModel = new QgsMapLayerProxyModel( this );
  setModel( mProxyModel );

  connect( this, SIGNAL( activated( int ) ), this, SLOT( indexChanged( int ) ) );
  connect( mProxyModel, SIGNAL( rowsInserted( QModelIndex, int, int ) ), this, SLOT( rowsChanged() ) );
  connect( mProxyModel, SIGNAL( rowsRemoved( QModelIndex, int, int ) ), this, SLOT( rowsChanged() ) );
}

void QgsMapLayerComboBox::setLayer( QgsMapLayer *layer )
{
  if ( !layer )
  {
    setCurrentIndex( -1 );
    return;
  }

  QModelIndex idx = mProxyModel->sourceLayerModel()->indexFromLayer( layer );
  if ( idx.isValid() )
  {
    QModelIndex proxyIdx = mProxyModel->mapFromSource( idx );
    if ( proxyIdx.isValid() )
    {
      setCurrentIndex( proxyIdx.row() );
      emit layerChanged( currentLayer() );
      return;
    }
  }
  setCurrentIndex( -1 );
  emit layerChanged( currentLayer() );
}

QgsMapLayer* QgsMapLayerComboBox::currentLayer() const
{
  return layer( currentIndex() );
}

QgsMapLayer *QgsMapLayerComboBox::layer( int layerIndex ) const
{
  const QModelIndex proxyIndex = mProxyModel->index( layerIndex, 0 );
  if ( !proxyIndex.isValid() )
  {
    return nullptr;
  }

  const QModelIndex index = mProxyModel->mapToSource( proxyIndex );
  if ( !index.isValid() )
  {
    return nullptr;
  }

  QgsMapLayer* layer = static_cast<QgsMapLayer*>( index.internalPointer() );
  if ( layer )
  {
    return layer;
  }
  return nullptr;
}

void QgsMapLayerComboBox::indexChanged( int i )
{
  Q_UNUSED( i );
  QgsMapLayer* layer = currentLayer();
  emit layerChanged( layer );
}

void QgsMapLayerComboBox::rowsChanged()
{
  if ( count() == 1 )
  {
    //currently selected layer item has changed
    emit layerChanged( currentLayer() );
  }
  else if ( count() == 0 )
  {
    emit layerChanged( nullptr );
  }
}

