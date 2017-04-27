/***************************************************************************
    qgscategorizedsymbolrendererv2.h
    ---------------------
    begin                : November 2009
    copyright            : (C) 2009 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSCATEGORIZEDSYMBOLRENDERERV2_H
#define QGSCATEGORIZEDSYMBOLRENDERERV2_H

#include "qgssymbolv2.h"
#include "qgsrendererv2.h"
#include "qgsexpression.h"

#include <QHash>
#include <QScopedPointer>

class QgsVectorColorRampV2;
class QgsVectorLayer;

/** \ingroup core
 * \brief categorized renderer
*/
class CORE_EXPORT QgsRendererCategoryV2
{
  public:
    QgsRendererCategoryV2();

    //! takes ownership of symbol
    QgsRendererCategoryV2( const QVariant& value, QgsSymbolV2* symbol, const QString& label, bool render = true );

    //! copy constructor
    QgsRendererCategoryV2( const QgsRendererCategoryV2& cat );

    ~QgsRendererCategoryV2() {}

    QgsRendererCategoryV2& operator=( QgsRendererCategoryV2 cat );

    QVariant value() const;
    QgsSymbolV2* symbol() const;
    QString label() const;

    void setValue( const QVariant &value );
    void setSymbol( QgsSymbolV2* s );
    void setLabel( const QString &label );

    // @note added in 2.5
    bool renderState() const;
    void setRenderState( bool render );

    // debugging
    QString dump() const;

    void toSld( QDomDocument& doc, QDomElement &element, QgsStringMap props ) const;

  protected:
    QVariant mValue;
    QScopedPointer<QgsSymbolV2> mSymbol;
    QString mLabel;
    bool mRender;

    void swap( QgsRendererCategoryV2 & other );
};

typedef QList<QgsRendererCategoryV2> QgsCategoryList;

Q_NOWARN_DEPRECATED_PUSH
/** \ingroup core
 * \class QgsCategorizedSymbolRendererV2
 */
class CORE_EXPORT QgsCategorizedSymbolRendererV2 : public QgsFeatureRendererV2
{
  public:

    QgsCategorizedSymbolRendererV2( const QString& attrName = QString(), const QgsCategoryList& categories = QgsCategoryList() );

    virtual ~QgsCategorizedSymbolRendererV2();

    //! @note available in python as symbolForFeature2
    virtual QgsSymbolV2* symbolForFeature( QgsFeature& feature, QgsRenderContext& context ) override;

    //! @note available in python as originalSymbolForFeature2
    virtual QgsSymbolV2* originalSymbolForFeature( QgsFeature& feature, QgsRenderContext& context ) override;

    virtual void startRender( QgsRenderContext& context, const QgsFields& fields ) override;

    virtual void stopRender( QgsRenderContext& context ) override;

    virtual QList<QString> usedAttributes() override;

    virtual QString dump() const override;

    virtual QgsCategorizedSymbolRendererV2* clone() const override;

    virtual void toSld( QDomDocument& doc, QDomElement &element ) const override;

    //! returns bitwise OR-ed capabilities of the renderer
    virtual int capabilities() override { return SymbolLevels | RotationField | Filter; }

    virtual QString filter( const QgsFields& fields = QgsFields() ) override;

    //! @note available in python as symbols2
    virtual QgsSymbolV2List symbols( QgsRenderContext& context ) override;

    /** Update all the symbols but leave categories and colors. This method also sets the source
     * symbol for the renderer.
     * @param sym source symbol to use for categories. Ownership is not transferred.
     * @see setSourceSymbol()
     */
    void updateSymbols( QgsSymbolV2 * sym );

    const QgsCategoryList& categories() const { return mCategories; }

    //! return index of category with specified value (-1 if not found)
    int categoryIndexForValue( const QVariant& val );

    //! return index of category with specified label (-1 if not found or not unique)
    //! @note added in 2.5
    int categoryIndexForLabel( const QString& val );

    bool updateCategoryValue( int catIndex, const QVariant &value );
    bool updateCategorySymbol( int catIndex, QgsSymbolV2* symbol );
    bool updateCategoryLabel( int catIndex, const QString& label );

    //! @note added in 2.5
    bool updateCategoryRenderState( int catIndex, bool render );

    void addCategory( const QgsRendererCategoryV2 &category );
    bool deleteCategory( int catIndex );
    void deleteAllCategories();

    //! Moves the category at index position from to index position to.
    void moveCategory( int from, int to );

    void sortByValue( Qt::SortOrder order = Qt::AscendingOrder );
    void sortByLabel( Qt::SortOrder order = Qt::AscendingOrder );

    QString classAttribute() const { return mAttrName; }
    void setClassAttribute( const QString& attr ) { mAttrName = attr; }

    //! create renderer from XML element
    static QgsFeatureRendererV2* create( QDomElement& element );

    //! store renderer info to XML element
    virtual QDomElement save( QDomDocument& doc ) override;

    //! return a list of symbology items for the legend
    virtual QgsLegendSymbologyList legendSymbologyItems( QSize iconSize ) override;

    //! return a list of item text / symbol
    //! @note not available in python bindings
    virtual QgsLegendSymbolList legendSymbolItems( double scaleDenominator = -1, const QString& rule = QString() ) override;

    //! @note added in 2.10
    QgsLegendSymbolListV2 legendSymbolItemsV2() const override;

    virtual QSet< QString > legendKeysForFeature( QgsFeature& feature, QgsRenderContext& context ) override;

    /** Returns the renderer's source symbol, which is the base symbol used for the each categories' symbol before applying
     * the categories' color.
     * @see setSourceSymbol()
     * @see sourceColorRamp()
     */
    QgsSymbolV2* sourceSymbol();

    /** Sets the source symbol for the renderer, which is the base symbol used for the each categories' symbol before applying
     * the categories' color.
     * @param sym source symbol, ownership is transferred to the renderer
     * @see sourceSymbol()
     * @see setSourceColorRamp()
     */
    void setSourceSymbol( QgsSymbolV2* sym );

    /** Returns the source color ramp, from which each categories' color is derived.
     * @see setSourceColorRamp()
     * @see sourceSymbol()
     */
    QgsVectorColorRampV2* sourceColorRamp();

    /** Sets the source color ramp.
      * @param ramp color ramp. Ownership is transferred to the renderer
      * @see sourceColorRamp()
      * @see setSourceSymbol()
      */
    void setSourceColorRamp( QgsVectorColorRampV2* ramp );

    //! @note added in 2.1
    bool invertedColorRamp() { return mInvertedColorRamp; }
    void setInvertedColorRamp( bool inverted ) { mInvertedColorRamp = inverted; }

    /** Update the color ramp used and all symbols colors.
      * @param ramp color ramp. Ownership is transferred to the renderer
      * @param inverted set to true to invert ramp colors
      * @note added in 2.5
      */
    void updateColorRamp( QgsVectorColorRampV2* ramp, bool inverted = false );

    Q_DECL_DEPRECATED void setRotationField( const QString& fieldOrExpression ) override;
    Q_DECL_DEPRECATED QString rotationField() const override;

    void setSizeScaleField( const QString& fieldOrExpression );
    QString sizeScaleField() const;

    void setScaleMethod( QgsSymbolV2::ScaleMethod scaleMethod );
    QgsSymbolV2::ScaleMethod scaleMethod() const { return mScaleMethod; }

    //! items of symbology items in legend should be checkable
    //! @note added in 2.5
    virtual bool legendSymbolItemsCheckable() const override;

    //! item in symbology was checked
    // @note added in 2.5
    virtual bool legendSymbolItemChecked( const QString& key ) override;

    virtual void setLegendSymbolItem( const QString& key, QgsSymbolV2* symbol ) override;

    //! item in symbology was checked
    // @note added in 2.5
    virtual void checkLegendSymbolItem( const QString& key, bool state = true ) override;

    //! If supported by the renderer, return classification attribute for the use in legend
    //! @note added in 2.6
    virtual QString legendClassificationAttribute() const override { return classAttribute(); }

    //! creates a QgsCategorizedSymbolRendererV2 from an existing renderer.
    //! @note added in 2.5
    //! @returns a new renderer if the conversion was possible, otherwise 0.
    static QgsCategorizedSymbolRendererV2* convertFromRenderer( const QgsFeatureRendererV2 *renderer );

  protected:
    QString mAttrName;
    QgsCategoryList mCategories;
    QScopedPointer<QgsSymbolV2> mSourceSymbol;
    QScopedPointer<QgsVectorColorRampV2> mSourceColorRamp;
    bool mInvertedColorRamp;
    QScopedPointer<QgsExpression> mRotation;
    QScopedPointer<QgsExpression> mSizeScale;
    QgsSymbolV2::ScaleMethod mScaleMethod;
    QScopedPointer<QgsExpression> mExpression;

    //! attribute index (derived from attribute name in startRender)
    int mAttrNum;

    //! hashtable for faster access to symbols
    QHash<QString, QgsSymbolV2*> mSymbolHash;
    bool mCounting;

    //! temporary symbols, used for data-defined rotation and scaling
    QHash<QgsSymbolV2*, QgsSymbolV2*> mTempSymbols;

    void rebuildHash();

    QgsSymbolV2* skipRender();

    QgsSymbolV2* symbolForValue( const QVariant& value );

  private:

    /** Returns calculated classification value for a feature */
    QVariant valueForFeature( QgsFeature& feature, QgsRenderContext &context ) const;

};
Q_NOWARN_DEPRECATED_POP


#endif // QGSCATEGORIZEDSYMBOLRENDERERV2_H
