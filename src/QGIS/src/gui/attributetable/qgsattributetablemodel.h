/***************************************************************************
  QgsAttributeTableModel.h - Models for attribute table
  -------------------
         date                 : Feb 2009
         copyright            : Vita Cizek
         email                : weetya (at) gmail.com

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSATTRIBUTETABLEMODEL_H
#define QGSATTRIBUTETABLEMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QObject>
#include <QHash>
#include <QQueue>
#include <QMap>

#include "qgsvectorlayer.h" // QgsAttributeList
#include "qgsvectorlayercache.h"
#include "qgsconditionalstyle.h"
#include "qgsattributeeditorcontext.h"

class QgsMapCanvas;
class QgsMapLayerAction;
class QgsEditorWidgetFactory;


/** \ingroup gui
 * A model backed by a {@link QgsVectorLayerCache} which is able to provide
 * feature/attribute information to a QAbstractItemView.
 *
 * @brief
 * Is able to generate editor widgets for its QModelIndexes as well.
 * Is mostly referred to as "master model" within this doc and the source.
 *
 * @see <a href="http://doc.qt.digia.com/qt/model-view-programming.html">Qt Model View Programming</a>
 */
class GUI_EXPORT QgsAttributeTableModel: public QAbstractTableModel
{
    Q_OBJECT

  public:
    enum Role
    {
      SortRole = Qt::UserRole + 1, //!< Role used for sorting
      FeatureIdRole,               //!< Get the feature id of the feature in this row
      FieldIndexRole,              //!< Get the field index of this column
      UserRole                     //!< Start further roles starting from this role
    };

  public:
    /**
     * Constructor
     * @param layerCache  A layer cache to use as backend
     * @param parent      The parent QObject (owner)
     */
    QgsAttributeTableModel( QgsVectorLayerCache *layerCache, QObject *parent = nullptr );

    /**
     * Returns the number of rows
     * @param parent parent index
     */
    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    /**
     * Returns the number of columns
     * @param parent parent index
     */
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

    /**
     * Returns header data
     * @param section required section
     * @param orientation horizontal or vertical orientation
     * @param role data role
     */
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;

    /**
     * Returns data on the given index
     * @param index model index
     * @param role data role
     */
    virtual QVariant data( const QModelIndex &index, int role ) const override;

    /**
     * Updates data on given index
     * @param index model index
     * @param value new data value
     * @param role data role
     */
    virtual bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    /**
     * Returns item flags for the index
     * @param index model index
     */
    Qt::ItemFlags flags( const QModelIndex &index ) const override;

    /**
     * Reloads the model data between indices
     * @param index1 start index
     * @param index2 end index
     */
    void reload( const QModelIndex &index1, const QModelIndex &index2 );

    /**
     * Remove rows
     */
    bool removeRows( int row, int count, const QModelIndex &parent = QModelIndex() ) override;

    /**
     * Resets the model
     *
     * Alias to loadLayer()
     */
    inline void resetModel() { loadLayer(); }

    /**
     * Maps feature id to table row
     * @param id feature id
     */
    int idToRow( QgsFeatureId id ) const;

    QModelIndex idToIndex( QgsFeatureId id ) const;

    QModelIndexList idToIndexList( QgsFeatureId id ) const;

    /**
     * get field index from column
     */
    int fieldIdx( int col ) const;

    /**
     * get column from field index
     */
    int fieldCol( int idx ) const;

    /**
     * Maps row to feature id
     * @param row row number
     */
    QgsFeatureId rowToId( int row ) const;

    /**
     * Swaps two rows
     * @param a first row
     * @param b second row
     */
    void swapRows( QgsFeatureId a, QgsFeatureId b );

    /**
     * Returns the layer this model uses as backend. Retrieved from the layer cache.
     */
    inline QgsVectorLayer* layer() const { return mLayerCache ? mLayerCache->layer() : nullptr; }

    /**
     * Returns the layer cache this model uses as backend.
     */
    inline QgsVectorLayerCache* layerCache() const { return mLayerCache; }

    /**
     * Execute an action
     */
    void executeAction( int action, const QModelIndex &idx ) const;

    /**
     * Execute a QgsMapLayerAction
     */
    void executeMapLayerAction( QgsMapLayerAction* action, const QModelIndex &idx ) const;

    /**
     * Return the feature attributes at given model index
     * @return feature attributes at given model index
     */
    QgsFeature feature( const QModelIndex &idx ) const;

    /**
     * Caches the entire data for one column. This should be called prior to sorting,
     * so the data does not have to be fetched for every single comparison.
     * Specify -1 as column to invalidate the cache
     *
     * @param column The column index of the field to catch
     */
    void prefetchColumnData( int column );

    /**
     * Prefetches the entire data for one expression. Based on this cached information
     * the sorting can later be done in a performant way.
     *
     * @param expression The expression to cache
     */
    void prefetchSortData( const QString& expression );

    /**
     * The expression which was used to fill the sorting cache
     */
    QString sortCacheExpression() const;

    /**
     * Set a request that will be used to fill this attribute table model.
     * In contrast to a filter, the request will constrain the data shown without the possibility
     * to dynamically adjust it.
     *
     * @param request The request to use to fill this table model.
     */
    void setRequest( const QgsFeatureRequest& request );

    /**
     * Get the the feature request
     */
    // TODO QGIS 3: return copy instead of reference
    const QgsFeatureRequest& request() const;

    /**
     * Sets the context in which this table is shown.
     * Will be forwarded to any editor widget created when editing data on this model.
     *
     * @param context The context
     */
    void setEditorContext( const QgsAttributeEditorContext& context ) { mEditorContext = context; }

    /**
     * Returns the context in which this table is shown.
     * Will be forwarded to any editor widget created when editing data on this model.
     *
     * @return The context
     */
    const QgsAttributeEditorContext& editorContext() const { return mEditorContext; }

    /**
     * Empty extra columns to announce from this model.
     * Any extra columns need to be implemented by proxy models in front of this model.
     */
    int extraColumns() const;

    /**
     * Empty extra columns to announce from this model.
     * Any extra columns need to be implemented by proxy models in front of this model.
     */
    void setExtraColumns( int extraColumns );

  public slots:
    /**
     * Loads the layer into the model
     * Preferably to be called, before using this model as source for any other proxy model
     */
    virtual void loadLayer();

    /** Handles updating the model when the conditional style for a field changes.
     * @param fieldName name of field whose conditional style has changed
     * @note added in QGIS 2.12
     */
    void fieldConditionalStyleChanged( const QString& fieldName );

  signals:
    /**
     * Model has been changed
     */
    void modelChanged();

    //! @note not available in python bindings
    void progress( int i, bool &cancel );
    void finished();

  private slots:
    /**
     * Launched whenever the number of fields has changed
     */
    virtual void updatedFields();

    /**
     * Gets called when an edit command ends
     * This will synchronize all fields which have been changed since the last
     * edit command in one single go
     */
    virtual void editCommandEnded();

    /**
     * Called whenever a column is removed;
     */
    virtual void attributeDeleted( int idx );

  protected slots:
    /**
     * Launched when attribute value has been changed
     * @param fid feature id
     * @param idx attribute index
     * @param value new value
     */
    virtual void attributeValueChanged( QgsFeatureId fid, int idx, const QVariant &value );
    /**
     * Launched when eatures have been deleted
     * @param fids feature ids
     */
    virtual void featuresDeleted( const QgsFeatureIds& fids );
    /**
     * Launched when a feature has been added
     * @param fid feature id
     */
    virtual void featureAdded( QgsFeatureId fid );

    /**
     * Launched when layer has been deleted
     */
    virtual void layerDeleted();

  protected:
    QgsVectorLayerCache *mLayerCache;
    int mFieldCount;

    mutable QgsFeature mFeat;

    QgsAttributeList mAttributes;
    QVector<QgsEditorWidgetFactory*> mWidgetFactories;
    QVector<QVariant> mAttributeWidgetCaches;
    QVector<QgsEditorWidgetConfig> mWidgetConfigs;

    QHash<QgsFeatureId, int> mIdRowMap;
    QHash<int, QgsFeatureId> mRowIdMap;
    mutable QHash<int, QList<QgsConditionalStyle> > mRowStylesMap;

    mutable QgsExpressionContext mExpressionContext;

    /**
      * Gets mFieldCount, mAttributes and mValueMaps
      */
    virtual void loadAttributes();

  private:
    /**
     * Load feature fid into local cache (mFeat)
     *
     * @param  fid     feature id
     *
     * @return feature exists
     */
    virtual bool loadFeatureAtId( QgsFeatureId fid ) const;

    QgsFeatureRequest mFeatureRequest;

    /** The currently cached column */
    QgsExpression mSortCacheExpression;
    QgsAttributeList mSortCacheAttributes;
    /** If it is set, a simple field is used for sorting, if it's -1 it's the mSortCacheExpression*/
    int mSortFieldIndex;
    /** Allows caching of one value per column (used for sorting) */
    QHash<QgsFeatureId, QVariant> mSortCache;

    /**
     * Holds the bounds of changed cells while an update operation is running
     * top    = min row
     * left   = min column
     * bottom = max row
     * right  = max column
     */
    QRect mChangedCellBounds;

    QgsAttributeEditorContext mEditorContext;

    int mExtraColumns;
};


#endif
