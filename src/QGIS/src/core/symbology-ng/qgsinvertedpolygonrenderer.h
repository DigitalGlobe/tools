/***************************************************************************
    qgsinvertedpolygonrenderer.h
    ---------------------
    begin                : April 2014
    copyright            : (C) 2014 Hugo Mercier / Oslandia
    email                : hugo dot mercier at oslandia dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSINVERTEDPOLYGONRENDERER_H
#define QGSINVERTEDPOLYGONRENDERER_H

#include "qgis.h"
#include "qgsrendererv2.h"
#include "qgssymbolv2.h"
#include "qgsexpression.h"
#include "qgsfeature.h"
#include "qgsgeometry.h"
#include <QScopedPointer>

/** \ingroup core
 * QgsInvertedPolygonRenderer is a polygon-only feature renderer used to
 * display features inverted, where the exterior is turned to an interior
 * and where the exterior theoretically spans the entire plane, allowing
 * to mask the surroundings of some features.
 *
 * It is designed on top of another feature renderer, which is called "embedded"
 * Most of the methods are then only proxies to the embedded renderer.
 *
 * Features are collected to form one "inverted" polygon
 * during renderFeature() and rendered on stopRender().
 *
 * @note added in 2.4
 */
class CORE_EXPORT QgsInvertedPolygonRenderer : public QgsFeatureRendererV2
{
  public:

    /** Constructor
     * @param embeddedRenderer optional embeddedRenderer. If null, a default one will be assigned.
     * Ownership will be transferred.
     */
    QgsInvertedPolygonRenderer( QgsFeatureRendererV2* embeddedRenderer = nullptr );

    virtual ~QgsInvertedPolygonRenderer();

    /** Used to clone this feature renderer.*/
    virtual QgsInvertedPolygonRenderer* clone() const override;

    virtual void startRender( QgsRenderContext& context, const QgsFields& fields ) override;

    /** Renders a given feature.
     * This will here collect features. The actual rendering will be postponed to stopRender()
     * @param feature the feature to render
     * @param context the rendering context
     * @param layer the symbol layer to render, if that makes sense
     * @param selected whether this feature has been selected (this will add decorations)
     * @param drawVertexMarker whether this feature has vertex markers (in edit mode usually)
     * @returns true if the rendering was ok
     */
    virtual bool renderFeature( QgsFeature& feature, QgsRenderContext& context, int layer = -1, bool selected = false, bool drawVertexMarker = false ) override;

    /**
     * The actual rendering will take place here.
     * Features collected during renderFeature() are rendered using the embedded feature renderer
     */
    virtual void stopRender( QgsRenderContext& context ) override;

    /** @returns a textual representation of the renderer */
    virtual QString dump() const override;

    /** Proxy that will call this method on the embedded renderer. */
    virtual QList<QString> usedAttributes() override;
    /** Proxy that will call this method on the embedded renderer. */
    virtual int capabilities() override;
    /** Proxy that will call this method on the embedded renderer.
     * @note available in python bindings as symbol2
     */
    virtual QgsSymbolV2List symbols( QgsRenderContext& context ) override;
    /** Proxy that will call this method on the embedded renderer.
     * @note available in python bindings as symbolForFeature2
     */
    virtual QgsSymbolV2* symbolForFeature( QgsFeature& feature, QgsRenderContext& context ) override;
    /** Proxy that will call this method on the embedded renderer.
     * @note available in python bindings as originalSymbolForFeature2
     */
    virtual QgsSymbolV2* originalSymbolForFeature( QgsFeature& feat, QgsRenderContext& context ) override;
    /** Proxy that will call this method on the embedded renderer.
     * @note available in python bindings as symbolsForFeature
     */
    virtual QgsSymbolV2List symbolsForFeature( QgsFeature& feat, QgsRenderContext& context ) override;
    /** Proxy that will call this method on the embedded renderer.
     * @note available in python bindings as originalSymbolsForFeature2
     */
    virtual QgsSymbolV2List originalSymbolsForFeature( QgsFeature& feat, QgsRenderContext& context ) override;
    /** Proxy that will call this method on the embedded renderer. */
    virtual QgsLegendSymbologyList legendSymbologyItems( QSize iconSize ) override;
    /** Proxy that will call this method on the embedded renderer.
     * @note not available in python bindings
     */
    virtual QgsLegendSymbolList legendSymbolItems( double scaleDenominator = -1, const QString& rule = "" ) override;
    /** Proxy that will call this method on the embedded renderer.
     * @note available in python bindings as willRenderFeature2
     */
    virtual bool willRenderFeature( QgsFeature& feat, QgsRenderContext& context ) override;

    /** Creates a renderer out of an XML, for loading*/
    static QgsFeatureRendererV2* create( QDomElement& element );

    /** Creates an XML representation of the renderer. Used for saving purpose
     * @param doc the XML document where to create the XML subtree
     * @returns the created XML subtree
     */
    virtual QDomElement save( QDomDocument& doc ) override;

    void setEmbeddedRenderer( QgsFeatureRendererV2* subRenderer ) override;
    const QgsFeatureRendererV2* embeddedRenderer() const override;

    virtual void setLegendSymbolItem( const QString& key, QgsSymbolV2* symbol ) override;

    virtual bool legendSymbolItemsCheckable() const override;
    virtual bool legendSymbolItemChecked( const QString& key ) override;
    virtual void checkLegendSymbolItem( const QString& key, bool state = true ) override;

    /** @returns true if the geometries are to be preprocessed (merged with an union) before rendering.*/
    bool preprocessingEnabled() const { return mPreprocessingEnabled; }
    /**
     * @param enabled enables or disables the preprocessing.
     * When enabled, geometries will be merged with an union before being rendered.
     * It allows fixing some rendering artifacts (when rendering overlapping polygons for instance).
     * This will involve some CPU-demanding computations and is thus disabled by default.
     */
    void setPreprocessingEnabled( bool enabled ) { mPreprocessingEnabled = enabled; }

    /** Creates a QgsInvertedPolygonRenderer by a conversion from an existing renderer.
     * @note added in 2.5
     * @returns a new renderer if the conversion was possible, otherwise 0.
     */
    static QgsInvertedPolygonRenderer* convertFromRenderer( const QgsFeatureRendererV2* renderer );

  private:
    /** Private copy constructor. @see clone() */
    QgsInvertedPolygonRenderer( const QgsInvertedPolygonRenderer& );
    /** Private assignment operator. @see clone() */
    QgsInvertedPolygonRenderer& operator=( const QgsInvertedPolygonRenderer& );

    /** Embedded renderer */
    QScopedPointer<QgsFeatureRendererV2> mSubRenderer;

    /** Structure where the reversed geometry is built during renderFeature */
    struct CombinedFeature
    {
      QList<QgsGeometry*> geometries; //< list of geometries
      QgsFeature feature;             //< one feature (for attriute-based rendering)
    };
    typedef QVector<CombinedFeature> FeatureCategoryVector;
    /** Where features are stored, based on the index of their symbol category @see mSymbolCategories */
    FeatureCategoryVector mFeaturesCategories;

    /** Maps a category to an index */
    QMap<QByteArray, int> mSymbolCategories;

    /** The polygon used as exterior ring that covers the current extent */
    QgsPolygon mExtentPolygon;

    /** The context used for rendering */
    QgsRenderContext mContext;

    /** Fields of each feature*/
    QgsFields mFields;

    /** Class used to represent features that must be rendered
     *  with decorations (selection, vertex markers)
     */
    struct FeatureDecoration
    {
      QgsFeature feature;
      bool selected;
      bool drawMarkers;
      int layer;
      FeatureDecoration( QgsFeature& a_feature, bool a_selected, bool a_drawMarkers, int a_layer )
          : feature( a_feature )
          , selected( a_selected )
          , drawMarkers( a_drawMarkers )
          , layer( a_layer )
      {}
    };
    QList<FeatureDecoration> mFeatureDecorations;

    /** Whether to preprocess (merge) geometries before rendering*/
    bool mPreprocessingEnabled;
};


#endif // QGSMASKRENDERERV2_H
