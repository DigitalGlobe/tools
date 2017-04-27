/***************************************************************************
   qgsmaplayercombobox.h
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

#ifndef QGSMAPLAYERCOMBOBOX_H
#define QGSMAPLAYERCOMBOBOX_H

#include <QComboBox>

#include "qgsmaplayerproxymodel.h"

class QgsMapLayer;
class QgsVectorLayer;

/** \ingroup gui
 * @brief The QgsMapLayerComboBox class is a combo box which displays the list of layers
 * @note added in 2.3
 */
class GUI_EXPORT QgsMapLayerComboBox : public QComboBox
{
    Q_OBJECT
    Q_FLAGS( QgsMapLayerProxyModel::Filters )
    Q_PROPERTY( QgsMapLayerProxyModel::Filters filters READ filters WRITE setFilters )

  public:
    /**
     * @brief QgsMapLayerComboBox creates a combo box to dislpay the list of layers (currently in the registry).
     * The layers can be filtered and/or ordered.
     */
    explicit QgsMapLayerComboBox( QWidget *parent = nullptr );

    //! setFilters allows fitering according to layer type and/or geometry type.
    void setFilters( const QgsMapLayerProxyModel::Filters& filters ) { mProxyModel->setFilters( filters ); }

    //! currently used filter on list layers
    QgsMapLayerProxyModel::Filters filters() const { return mProxyModel->filters(); }

    //! except a list of layers not to be listed
    void setExceptedLayerList( const QList<QgsMapLayer*>& layerList ) { mProxyModel->setExceptedLayerList( layerList );}

    //! returns the list of excepted layers
    QList<QgsMapLayer*> exceptedLayerList() const {return mProxyModel->exceptedLayerList();}

    /** Returns the current layer selected in the combo box.
     * @see layer
     */
    QgsMapLayer* currentLayer() const;

    /** Return the layer currently shown at the specified index within the combo box.
     * @param layerIndex position of layer to return
     * @note added in QGIS 2.10
     * @see currentLayer
     */
    QgsMapLayer* layer( int layerIndex ) const;

  public slots:
    //! setLayer set the current layer selected in the combo
    void setLayer( QgsMapLayer* layer );

  signals:
    //! layerChanged this signal is emitted whenever the currently selected layer changes
    void layerChanged( QgsMapLayer* layer );

  protected slots:
    void indexChanged( int i );
    void rowsChanged();

  private:
    QgsMapLayerProxyModel* mProxyModel;
};

#endif // QGSMAPLAYERCOMBOBOX_H
