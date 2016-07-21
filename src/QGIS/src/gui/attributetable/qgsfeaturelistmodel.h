/***************************************************************************
    qgsfeaturelistmodel.h
    ---------------------
    begin                : February 2013
    copyright            : (C) 2013 by Matthias Kuhn
    email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSATTRIBUTEEDITORMODEL_H
#define QGSATTRIBUTEEDITORMODEL_H

#include <qgsexpression.h>

#include <QAbstractProxyModel>
#include <QVariant>
#include <QItemSelectionModel>

#include "qgsfeaturemodel.h"
#include "qgsfeature.h" // QgsFeatureId

class QgsAttributeTableFilterModel;
class QgsAttributeTableModel;
class QgsVectorLayerCache;

/** \ingroup gui
 * \class QgsFeatureListModel
 */
class GUI_EXPORT QgsFeatureListModel : public QAbstractProxyModel, public QgsFeatureModel
{
    Q_OBJECT

  public:
    struct FeatureInfo
    {
    public:
      FeatureInfo()
          : isNew( false )
          , isEdited( false )
      {}

      bool isNew;
      bool isEdited;
    };

    enum Role
    {
      FeatureInfoRole = Qt::UserRole,
      FeatureRole
    };

  public:
    explicit QgsFeatureListModel( QgsAttributeTableFilterModel *sourceModel, QObject* parent = nullptr );
    virtual ~QgsFeatureListModel();

    virtual void setSourceModel( QgsAttributeTableFilterModel* sourceModel );
    QgsVectorLayerCache* layerCache();
    virtual QVariant data( const QModelIndex& index, int role ) const override;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const override;

    /**
     * @brief If true is specified, a NULL value will be injected
     * @param injectNull state of null value injection
     * @note added in 2.9
     */
    void setInjectNull( bool injectNull );

    /**
     * @brief Returns the current state of null value injection
     * @return If a NULL value is added
     * @note added in 2.9
     */
    bool injectNull();

    QgsAttributeTableModel* masterModel();

    /**
     *  @param  expression   A {@link QgsExpression} compatible string.
     *  @return true if the expression could be set, false if there was a parse error.
     *          If it fails, the old expression will still be applied. Call {@link parserErrorString()}
     *          for a meaningful error message.
     */
    bool setDisplayExpression( const QString& expression );

    /**
     * @brief Returns a detailed message about errors while parsing a QgsExpression.
     * @return A message containg information about the parser error.
     */
    QString parserErrorString();

    QString displayExpression() const;
    bool featureByIndex( const QModelIndex& index, QgsFeature& feat );
    QgsFeatureId idxToFid( const QModelIndex& index ) const;
    QModelIndex fidToIdx( const QgsFeatureId fid ) const;

    virtual QModelIndex mapToSource( const QModelIndex& proxyIndex ) const override;
    virtual QModelIndex mapFromSource( const QModelIndex& sourceIndex ) const override;

    virtual QModelIndex mapToMaster( const QModelIndex& proxyIndex ) const;
    virtual QModelIndex mapFromMaster( const QModelIndex& sourceIndex ) const;

    virtual QItemSelection mapSelectionFromMaster( const QItemSelection& selection ) const;
    virtual QItemSelection mapSelectionToMaster( const QItemSelection& selection ) const;

    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
    virtual QModelIndex parent( const QModelIndex& child ) const override;
    virtual int columnCount( const QModelIndex&parent = QModelIndex() ) const override;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const override;

    QModelIndex fidToIndex( QgsFeatureId fid ) override;
    QModelIndexList fidToIndexList( QgsFeatureId fid );

  public slots:
    void onBeginRemoveRows( const QModelIndex& parent, int first, int last );
    void onEndRemoveRows( const QModelIndex& parent, int first, int last );
    void onBeginInsertRows( const QModelIndex& parent, int first, int last );
    void onEndInsertRows( const QModelIndex& parent, int first, int last );

  private:
    QgsExpression* mExpression;
    QgsAttributeTableFilterModel* mFilterModel;
    QString mParserErrorString;
    bool mInjectNull;
};

Q_DECLARE_METATYPE( QgsFeatureListModel::FeatureInfo )

#endif // QGSATTRIBUTEEDITORMODEL_H
