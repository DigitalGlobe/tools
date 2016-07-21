/***************************************************************************
         qgspointdisplacementrenderer.cpp
         --------------------------------
  begin                : January 26, 2010
  copyright            : (C) 2010 by Marco Hugentobler
  email                : marco at hugis dot net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgspointdisplacementrenderer.h"
#include "qgsgeometry.h"
#include "qgslogger.h"
#include "qgsspatialindex.h"
#include "qgssymbolv2.h"
#include "qgssymbollayerv2utils.h"
#include "qgsvectorlayer.h"
#include "qgssinglesymbolrendererv2.h"
#include "qgspainteffect.h"
#include "qgspainteffectregistry.h"
#include "qgsfontutils.h"
#include "qgsmultipointv2.h"
#include "qgspointv2.h"
#include "qgsunittypes.h"
#include "qgswkbptr.h"

#include <QDomElement>
#include <QPainter>

#include <cmath>

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

QgsPointDisplacementRenderer::QgsPointDisplacementRenderer( const QString& labelAttributeName )
    : QgsFeatureRendererV2( "pointDisplacement" )
    , mLabelAttributeName( labelAttributeName )
    , mLabelIndex( -1 )
    , mTolerance( 3 )
    , mToleranceUnit( QgsSymbolV2::MM )
    , mPlacement( Ring )
    , mCircleWidth( 0.4 )
    , mCircleColor( QColor( 125, 125, 125 ) )
    , mCircleRadiusAddition( 0 )
    , mMaxLabelScaleDenominator( -1 )
    , mSpatialIndex( nullptr )
{
  mRenderer = QgsFeatureRendererV2::defaultRenderer( QGis::Point );
  mCenterSymbol = new QgsMarkerSymbolV2(); //the symbol for the center of a displacement group
  mDrawLabels = true;
}

QgsPointDisplacementRenderer::~QgsPointDisplacementRenderer()
{
  delete mCenterSymbol;
  delete mRenderer;
}

QgsPointDisplacementRenderer* QgsPointDisplacementRenderer::clone() const
{
  QgsPointDisplacementRenderer* r = new QgsPointDisplacementRenderer( mLabelAttributeName );
  if ( mRenderer )
    r->setEmbeddedRenderer( mRenderer->clone() );
  r->setCircleWidth( mCircleWidth );
  r->setCircleColor( mCircleColor );
  r->setLabelFont( mLabelFont );
  r->setLabelColor( mLabelColor );
  r->setPlacement( mPlacement );
  r->setCircleRadiusAddition( mCircleRadiusAddition );
  r->setMaxLabelScaleDenominator( mMaxLabelScaleDenominator );
  r->setTolerance( mTolerance );
  r->setToleranceUnit( mToleranceUnit );
  r->setToleranceMapUnitScale( mToleranceMapUnitScale );
  if ( mCenterSymbol )
  {
    r->setCenterSymbol( mCenterSymbol->clone() );
  }
  copyRendererData( r );
  return r;
}

void QgsPointDisplacementRenderer::toSld( QDomDocument& doc, QDomElement &element ) const
{
  mRenderer->toSld( doc, element );
}


bool QgsPointDisplacementRenderer::renderFeature( QgsFeature& feature, QgsRenderContext& context, int layer, bool selected, bool drawVertexMarker )
{
  Q_UNUSED( drawVertexMarker );
  Q_UNUSED( context );
  Q_UNUSED( layer );

  //check, if there is already a point at that position
  if ( !feature.constGeometry() )
    return false;

  QgsSymbolV2* symbol = firstSymbolForFeature( mRenderer, feature, context );

  //if the feature has no symbol (eg, no matching rule in a rule-based renderer), skip it
  if ( !symbol )
    return false;

  //point position in screen coords
  const QgsGeometry* geom = feature.constGeometry();
  QGis::WkbType geomType = geom->wkbType();
  if ( geomType != QGis::WKBPoint && geomType != QGis::WKBPoint25D )
  {
    //can only render point type
    return false;
  }

  if ( selected )
    mSelectedFeatures.insert( feature.id() );

  double searchDistance = mTolerance * QgsSymbolLayerV2Utils::mapUnitScaleFactor( context, mToleranceUnit, mToleranceMapUnitScale );
  QList<QgsFeatureId> intersectList = mSpatialIndex->intersects( searchRect( feature.constGeometry()->asPoint(), searchDistance ) );
  if ( intersectList.empty() )
  {
    mSpatialIndex->insertFeature( feature );
    // create new group
    DisplacementGroup newGroup;
    newGroup.insert( feature.id(), qMakePair( feature, symbol ) );
    mDisplacementGroups.push_back( newGroup );
    // add to group index
    mGroupIndex.insert( feature.id(), mDisplacementGroups.count() - 1 );
    return true;
  }

  //go through all the displacement group maps and search an entry where the id equals the result of the spatial search
  QgsFeatureId existingEntry = intersectList.at( 0 );

  int groupIdx = mGroupIndex[ existingEntry ];
  DisplacementGroup& group = mDisplacementGroups[groupIdx];

  // add to a group
  group.insert( feature.id(), qMakePair( feature, symbol ) );
  // add to group index
  mGroupIndex.insert( feature.id(), groupIdx );
  return true;
}

void QgsPointDisplacementRenderer::drawGroup( const DisplacementGroup& group, QgsRenderContext& context )
{
  const QgsFeature& feature = group.begin().value().first;
  bool selected = mSelectedFeatures.contains( feature.id() ); // maybe we should highlight individual features instead of the whole group?



  //get list of labels and symbols
  QStringList labelAttributeList;
  QList< QgsMarkerSymbolV2* > symbolList;
  QgsFeatureList featureList;

  QgsMultiPointV2* groupMultiPoint = new QgsMultiPointV2();
  for ( DisplacementGroup::const_iterator attIt = group.constBegin(); attIt != group.constEnd(); ++attIt )
  {
    labelAttributeList << ( mDrawLabels ? getLabel( attIt.value().first ) : QString() );
    symbolList << dynamic_cast<QgsMarkerSymbolV2*>( attIt.value().second );
    featureList << attIt.value().first;
    groupMultiPoint->addGeometry( attIt.value().first.constGeometry()->geometry()->clone() );
  }

  //calculate centroid of all points, this will be center of group
  QgsGeometry groupGeom( groupMultiPoint );
  QgsGeometry* centroid = groupGeom.centroid();
  QPointF pt;
  QgsConstWkbPtr wkbPtr( centroid->asWkb(), centroid->wkbSize() );
  _getPoint( pt, context, wkbPtr );
  delete centroid;

  //calculate max diagonal size from all symbols in group
  double diagonal = 0;
  Q_FOREACH ( QgsMarkerSymbolV2* symbol, symbolList )
  {
    if ( symbol )
    {
      diagonal = qMax( diagonal, QgsSymbolLayerV2Utils::convertToPainterUnits( context,
                       M_SQRT2 * symbol->size(),
                       symbol->sizeUnit(), symbol->sizeMapUnitScale() ) );
    }
  }

  QgsSymbolV2RenderContext symbolContext( context, QgsSymbolV2::MM, 1.0, selected );

  QList<QPointF> symbolPositions;
  QList<QPointF> labelPositions;
  double circleRadius = -1.0;
  calculateSymbolAndLabelPositions( symbolContext, pt, symbolList.size(), diagonal, symbolPositions, labelPositions, circleRadius );

  //draw Circle
  if ( circleRadius > 0 )
    drawCircle( circleRadius, symbolContext, pt, symbolList.size() );

  //draw mid point
  if ( labelAttributeList.size() > 1 )
  {
    if ( mCenterSymbol )
    {
      mCenterSymbol->renderPoint( pt, &feature, context, -1, selected );
    }
    else
    {
      context.painter()->drawRect( QRectF( pt.x() - symbolContext.outputLineWidth( 1 ), pt.y() - symbolContext.outputLineWidth( 1 ), symbolContext.outputLineWidth( 2 ), symbolContext.outputLineWidth( 2 ) ) );
    }
  }

  //draw symbols on the circle
  drawSymbols( featureList, context, symbolList, symbolPositions, selected );
  //and also the labels
  drawLabels( pt, symbolContext, labelPositions, labelAttributeList );
}

void QgsPointDisplacementRenderer::setEmbeddedRenderer( QgsFeatureRendererV2* r )
{
  delete mRenderer;
  mRenderer = r;
}

const QgsFeatureRendererV2* QgsPointDisplacementRenderer::embeddedRenderer() const
{
  return mRenderer;
}

void QgsPointDisplacementRenderer::setLegendSymbolItem( const QString& key, QgsSymbolV2* symbol )
{
  if ( !mRenderer )
    return;

  mRenderer->setLegendSymbolItem( key, symbol );
}

bool QgsPointDisplacementRenderer::legendSymbolItemsCheckable() const
{
  if ( !mRenderer )
    return false;

  return mRenderer->legendSymbolItemsCheckable();
}

bool QgsPointDisplacementRenderer::legendSymbolItemChecked( const QString& key )
{
  if ( !mRenderer )
    return false;

  return mRenderer->legendSymbolItemChecked( key );
}

void QgsPointDisplacementRenderer::checkLegendSymbolItem( const QString& key, bool state )
{
  if ( !mRenderer )
    return;

  return mRenderer->checkLegendSymbolItem( key, state );
}

QList<QString> QgsPointDisplacementRenderer::usedAttributes()
{
  QList<QString> attributeList;
  if ( !mLabelAttributeName.isEmpty() )
  {
    attributeList.push_back( mLabelAttributeName );
  }
  if ( mRenderer )
  {
    attributeList += mRenderer->usedAttributes();
  }
  return attributeList;
}

int QgsPointDisplacementRenderer::capabilities()
{
  if ( !mRenderer )
  {
    return 0;
  }
  return mRenderer->capabilities();
}

QgsSymbolV2List QgsPointDisplacementRenderer::symbols( QgsRenderContext& context )
{
  if ( !mRenderer )
  {
    return QgsSymbolV2List();
  }
  return mRenderer->symbols( context );
}

QgsSymbolV2* QgsPointDisplacementRenderer::symbolForFeature( QgsFeature& feature, QgsRenderContext& context )
{
  if ( !mRenderer )
  {
    return nullptr;
  }
  return mRenderer->symbolForFeature( feature, context );
}

QgsSymbolV2* QgsPointDisplacementRenderer::originalSymbolForFeature( QgsFeature& feat, QgsRenderContext& context )
{
  if ( !mRenderer )
    return nullptr;
  return mRenderer->originalSymbolForFeature( feat, context );
}

QgsSymbolV2List QgsPointDisplacementRenderer::symbolsForFeature( QgsFeature& feature, QgsRenderContext& context )
{
  if ( !mRenderer )
  {
    return QgsSymbolV2List();
  }
  return mRenderer->symbolsForFeature( feature, context );
}

QgsSymbolV2List QgsPointDisplacementRenderer::originalSymbolsForFeature( QgsFeature& feat, QgsRenderContext& context )
{
  if ( !mRenderer )
    return QgsSymbolV2List();
  return mRenderer->originalSymbolsForFeature( feat, context );
}

bool QgsPointDisplacementRenderer::willRenderFeature( QgsFeature& feat, QgsRenderContext& context )
{
  if ( !mRenderer )
  {
    return false;
  }
  return mRenderer->willRenderFeature( feat, context );
}


void QgsPointDisplacementRenderer::startRender( QgsRenderContext& context, const QgsFields& fields )
{
  mRenderer->startRender( context, fields );

  mDisplacementGroups.clear();
  mGroupIndex.clear();
  mSpatialIndex = new QgsSpatialIndex;
  mSelectedFeatures.clear();

  if ( mLabelAttributeName.isEmpty() )
  {
    mLabelIndex = -1;
  }
  else
  {
    mLabelIndex = fields.fieldNameIndex( mLabelAttributeName );
  }

  if ( mMaxLabelScaleDenominator > 0 && context.rendererScale() > mMaxLabelScaleDenominator )
  {
    mDrawLabels = false;
  }
  else
  {
    mDrawLabels = true;
  }

  if ( mCenterSymbol )
  {
    mCenterSymbol->startRender( context, &fields );
  }
  return;
}

void QgsPointDisplacementRenderer::stopRender( QgsRenderContext& context )
{
  QgsDebugMsg( "QgsPointDisplacementRenderer::stopRender" );

  //printInfoDisplacementGroups(); //just for debugging

  Q_FOREACH ( const DisplacementGroup& group, mDisplacementGroups )
  {
    drawGroup( group, context );
  }

  mDisplacementGroups.clear();
  mGroupIndex.clear();
  delete mSpatialIndex;
  mSpatialIndex = nullptr;
  mSelectedFeatures.clear();

  mRenderer->stopRender( context );
  if ( mCenterSymbol )
  {
    mCenterSymbol->stopRender( context );
  }
}

QgsFeatureRendererV2* QgsPointDisplacementRenderer::create( QDomElement& symbologyElem )
{
  QgsPointDisplacementRenderer* r = new QgsPointDisplacementRenderer();
  r->setLabelAttributeName( symbologyElem.attribute( "labelAttributeName" ) );
  QFont labelFont;
  if ( !QgsFontUtils::setFromXmlChildNode( labelFont, symbologyElem, "labelFontProperties" ) )
  {
    labelFont.fromString( symbologyElem.attribute( "labelFont", "" ) );
  }
  r->setLabelFont( labelFont );
  r->setPlacement( static_cast< Placement >( symbologyElem.attribute( "placement", "0" ).toInt() ) );
  r->setCircleWidth( symbologyElem.attribute( "circleWidth", "0.4" ).toDouble() );
  r->setCircleColor( QgsSymbolLayerV2Utils::decodeColor( symbologyElem.attribute( "circleColor", "" ) ) );
  r->setLabelColor( QgsSymbolLayerV2Utils::decodeColor( symbologyElem.attribute( "labelColor", "" ) ) );
  r->setCircleRadiusAddition( symbologyElem.attribute( "circleRadiusAddition", "0.0" ).toDouble() );
  r->setMaxLabelScaleDenominator( symbologyElem.attribute( "maxLabelScaleDenominator", "-1" ).toDouble() );
  r->setTolerance( symbologyElem.attribute( "tolerance", "0.00001" ).toDouble() );
  r->setToleranceUnit( QgsSymbolLayerV2Utils::decodeOutputUnit( symbologyElem.attribute( "toleranceUnit", "MapUnit" ) ) );
  r->setToleranceMapUnitScale( QgsSymbolLayerV2Utils::decodeMapUnitScale( symbologyElem.attribute( "toleranceUnitScale" ) ) );

  //look for an embedded renderer <renderer-v2>
  QDomElement embeddedRendererElem = symbologyElem.firstChildElement( "renderer-v2" );
  if ( !embeddedRendererElem.isNull() )
  {
    r->setEmbeddedRenderer( QgsFeatureRendererV2::load( embeddedRendererElem ) );
  }

  //center symbol
  QDomElement centerSymbolElem = symbologyElem.firstChildElement( "symbol" );
  if ( !centerSymbolElem.isNull() )
  {
    r->setCenterSymbol( QgsSymbolLayerV2Utils::loadSymbol<QgsMarkerSymbolV2>( centerSymbolElem ) );
  }
  return r;
}

QDomElement QgsPointDisplacementRenderer::save( QDomDocument& doc )
{
  QDomElement rendererElement = doc.createElement( RENDERER_TAG_NAME );
  rendererElement.setAttribute( "forceraster", ( mForceRaster ? "1" : "0" ) );
  rendererElement.setAttribute( "type", "pointDisplacement" );
  rendererElement.setAttribute( "labelAttributeName", mLabelAttributeName );
  rendererElement.appendChild( QgsFontUtils::toXmlElement( mLabelFont, doc, "labelFontProperties" ) );
  rendererElement.setAttribute( "circleWidth", QString::number( mCircleWidth ) );
  rendererElement.setAttribute( "circleColor", QgsSymbolLayerV2Utils::encodeColor( mCircleColor ) );
  rendererElement.setAttribute( "labelColor", QgsSymbolLayerV2Utils::encodeColor( mLabelColor ) );
  rendererElement.setAttribute( "circleRadiusAddition", QString::number( mCircleRadiusAddition ) );
  rendererElement.setAttribute( "placement", static_cast< int >( mPlacement ) );
  rendererElement.setAttribute( "maxLabelScaleDenominator", QString::number( mMaxLabelScaleDenominator ) );
  rendererElement.setAttribute( "tolerance", QString::number( mTolerance ) );
  rendererElement.setAttribute( "toleranceUnit", QgsSymbolLayerV2Utils::encodeOutputUnit( mToleranceUnit ) );
  rendererElement.setAttribute( "toleranceUnitScale", QgsSymbolLayerV2Utils::encodeMapUnitScale( mToleranceMapUnitScale ) );

  if ( mRenderer )
  {
    QDomElement embeddedRendererElem = mRenderer->save( doc );
    rendererElement.appendChild( embeddedRendererElem );
  }
  if ( mCenterSymbol )
  {
    QDomElement centerSymbolElem = QgsSymbolLayerV2Utils::saveSymbol( "centerSymbol", mCenterSymbol, doc );
    rendererElement.appendChild( centerSymbolElem );
  }

  if ( mPaintEffect && !QgsPaintEffectRegistry::isDefaultStack( mPaintEffect ) )
    mPaintEffect->saveProperties( doc, rendererElement );

  if ( !mOrderBy.isEmpty() )
  {
    QDomElement orderBy = doc.createElement( "orderby" );
    mOrderBy.save( orderBy );
    rendererElement.appendChild( orderBy );
  }
  rendererElement.setAttribute( "enableorderby", ( mOrderByEnabled ? "1" : "0" ) );

  return rendererElement;
}

QgsLegendSymbologyList QgsPointDisplacementRenderer::legendSymbologyItems( QSize iconSize )
{
  if ( mRenderer )
  {
    return mRenderer->legendSymbologyItems( iconSize );
  }
  return QgsLegendSymbologyList();
}

QgsLegendSymbolList QgsPointDisplacementRenderer::legendSymbolItems( double scaleDenominator, const QString& rule )
{
  if ( mRenderer )
  {
    return mRenderer->legendSymbolItems( scaleDenominator, rule );
  }
  return QgsLegendSymbolList();
}


QgsRectangle QgsPointDisplacementRenderer::searchRect( const QgsPoint& p, double distance ) const
{
  return QgsRectangle( p.x() - distance, p.y() - distance, p.x() + distance, p.y() + distance );
}

void QgsPointDisplacementRenderer::printInfoDisplacementGroups()
{
  int nGroups = mDisplacementGroups.size();
  QgsDebugMsg( "number of displacement groups:" + QString::number( nGroups ) );
  for ( int i = 0; i < nGroups; ++i )
  {
    QgsDebugMsg( "***************displacement group " + QString::number( i ) );
    DisplacementGroup::const_iterator it = mDisplacementGroups.at( i ).constBegin();
    for ( ; it != mDisplacementGroups.at( i ).constEnd(); ++it )
    {
      QgsDebugMsg( FID_TO_STRING( it.key() ) );
    }
  }
}

QString QgsPointDisplacementRenderer::getLabel( const QgsFeature& f )
{
  QString attribute;
  QgsAttributes attrs = f.attributes();
  if ( mLabelIndex >= 0 && mLabelIndex < attrs.count() )
  {
    attribute = attrs.at( mLabelIndex ).toString();
  }
  return attribute;
}

void QgsPointDisplacementRenderer::setCenterSymbol( QgsMarkerSymbolV2* symbol )
{
  delete mCenterSymbol;
  mCenterSymbol = symbol;
}



void QgsPointDisplacementRenderer::calculateSymbolAndLabelPositions( QgsSymbolV2RenderContext& symbolContext, QPointF centerPoint, int nPosition,
    double symbolDiagonal, QList<QPointF>& symbolPositions, QList<QPointF>& labelShifts, double& circleRadius ) const
{
  symbolPositions.clear();
  labelShifts.clear();

  if ( nPosition < 1 )
  {
    return;
  }
  else if ( nPosition == 1 ) //If there is only one feature, draw it exactly at the center position
  {
    symbolPositions.append( centerPoint );
    labelShifts.append( QPointF( symbolDiagonal / 2.0, -symbolDiagonal / 2.0 ) );
    return;
  }

  double circleAdditionPainterUnits = symbolContext.outputLineWidth( mCircleRadiusAddition );

  switch ( mPlacement )
  {
    case Ring:
    {
      double minDiameterToFitSymbols = nPosition * symbolDiagonal / ( 2.0 * M_PI );
      double radius = qMax( symbolDiagonal / 2, minDiameterToFitSymbols ) + circleAdditionPainterUnits;

      double fullPerimeter = 2 * M_PI;
      double angleStep = fullPerimeter / nPosition;
      for ( double currentAngle = 0.0; currentAngle < fullPerimeter; currentAngle += angleStep )
      {
        double sinusCurrentAngle = sin( currentAngle );
        double cosinusCurrentAngle = cos( currentAngle );
        QPointF positionShift( radius * sinusCurrentAngle, radius * cosinusCurrentAngle );
        QPointF labelShift(( radius + symbolDiagonal / 2 ) * sinusCurrentAngle, ( radius + symbolDiagonal / 2 ) * cosinusCurrentAngle );
        symbolPositions.append( centerPoint + positionShift );
        labelShifts.append( labelShift );
      }

      circleRadius = radius;
      break;
    }
    case ConcentricRings:
    {
      double centerDiagonal = QgsSymbolLayerV2Utils::convertToPainterUnits( symbolContext.renderContext(),
                              M_SQRT2 * mCenterSymbol->size(),
                              mCenterSymbol->sizeUnit(), mCenterSymbol->sizeMapUnitScale() );

      int pointsRemaining = nPosition;
      int ringNumber = 1;
      double firstRingRadius = centerDiagonal / 2.0 + symbolDiagonal / 2.0;
      while ( pointsRemaining > 0 )
      {
        double radiusCurrentRing = qMax( firstRingRadius + ( ringNumber - 1 ) * symbolDiagonal + ringNumber * circleAdditionPainterUnits, 0.0 );
        int maxPointsCurrentRing = qMax( floor( 2 * M_PI * radiusCurrentRing / symbolDiagonal ), 1.0 );
        int actualPointsCurrentRing = qMin( maxPointsCurrentRing, pointsRemaining );

        double angleStep = 2 * M_PI / actualPointsCurrentRing;
        double currentAngle = 0.0;
        for ( int i = 0; i < actualPointsCurrentRing; ++i )
        {
          double sinusCurrentAngle = sin( currentAngle );
          double cosinusCurrentAngle = cos( currentAngle );
          QPointF positionShift( radiusCurrentRing * sinusCurrentAngle, radiusCurrentRing * cosinusCurrentAngle );
          QPointF labelShift(( radiusCurrentRing + symbolDiagonal / 2 ) * sinusCurrentAngle, ( radiusCurrentRing + symbolDiagonal / 2 ) * cosinusCurrentAngle );
          symbolPositions.append( centerPoint + positionShift );
          labelShifts.append( labelShift );
          currentAngle += angleStep;
        }

        pointsRemaining -= actualPointsCurrentRing;
        ringNumber++;
        circleRadius = radiusCurrentRing;
      }
      break;
    }
  }
}

void QgsPointDisplacementRenderer::drawCircle( double radiusPainterUnits, QgsSymbolV2RenderContext& context, QPointF centerPoint, int nSymbols )
{
  QPainter* p = context.renderContext().painter();
  if ( nSymbols < 2 || !p ) //draw circle only if multiple features
  {
    return;
  }

  //draw Circle
  QPen circlePen( mCircleColor );
  circlePen.setWidthF( context.outputLineWidth( mCircleWidth ) );
  p->setPen( circlePen );
  p->drawArc( QRectF( centerPoint.x() - radiusPainterUnits, centerPoint.y() - radiusPainterUnits, 2 * radiusPainterUnits, 2 * radiusPainterUnits ), 0, 5760 );
}

void QgsPointDisplacementRenderer::drawSymbols( const QgsFeatureList& features, QgsRenderContext& context,
    const QList< QgsMarkerSymbolV2* >& symbolList, const QList<QPointF>& symbolPositions, bool selected )
{
  QList<QPointF>::const_iterator symbolPosIt = symbolPositions.constBegin();
  QList<QgsMarkerSymbolV2*>::const_iterator symbolIt = symbolList.constBegin();
  QgsFeatureList::const_iterator featIt = features.constBegin();
  for ( ; symbolPosIt != symbolPositions.constEnd() && symbolIt != symbolList.constEnd() && featIt != features.constEnd();
        ++symbolPosIt, ++symbolIt, ++featIt )
  {
    if ( *symbolIt )
    {
      context.expressionContext().setFeature( *featIt );
      ( *symbolIt )->startRender( context );
      ( *symbolIt )->renderPoint( *symbolPosIt, &( *featIt ), context, -1, selected );
      ( *symbolIt )->stopRender( context );
    }
  }
}

void QgsPointDisplacementRenderer::drawLabels( QPointF centerPoint, QgsSymbolV2RenderContext& context, const QList<QPointF>& labelShifts, const QStringList& labelList )
{
  QPainter* p = context.renderContext().painter();
  if ( !p )
  {
    return;
  }

  QPen labelPen( mLabelColor );
  p->setPen( labelPen );

  //scale font (for printing)
  QFont pixelSizeFont = mLabelFont;
  pixelSizeFont.setPixelSize( context.outputLineWidth( mLabelFont.pointSizeF() * 0.3527 ) );
  QFont scaledFont = pixelSizeFont;
  scaledFont.setPixelSize( pixelSizeFont.pixelSize() * context.renderContext().rasterScaleFactor() );
  p->setFont( scaledFont );

  QFontMetricsF fontMetrics( pixelSizeFont );
  QPointF currentLabelShift; //considers the signs to determine the label position

  QList<QPointF>::const_iterator labelPosIt = labelShifts.constBegin();
  QStringList::const_iterator text_it = labelList.constBegin();

  for ( ; labelPosIt != labelShifts.constEnd() && text_it != labelList.constEnd(); ++labelPosIt, ++text_it )
  {
    currentLabelShift = *labelPosIt;
    if ( currentLabelShift.x() < 0 )
    {
      currentLabelShift.setX( currentLabelShift.x() - fontMetrics.width( *text_it ) );
    }
    if ( currentLabelShift.y() > 0 )
    {
      currentLabelShift.setY( currentLabelShift.y() + fontMetrics.ascent() );
    }

    QPointF drawingPoint( centerPoint + currentLabelShift );
    p->save();
    p->translate( drawingPoint.x(), drawingPoint.y() );
    p->scale( 1.0 / context.renderContext().rasterScaleFactor(), 1.0 / context.renderContext().rasterScaleFactor() );
    p->drawText( QPointF( 0, 0 ), *text_it );
    p->restore();
  }
}

QgsSymbolV2* QgsPointDisplacementRenderer::firstSymbolForFeature( QgsFeatureRendererV2* r, QgsFeature& f, QgsRenderContext &context )
{
  if ( !r )
  {
    return nullptr;
  }

  QgsSymbolV2List symbolList = r->symbolsForFeature( f, context );
  if ( symbolList.size() < 1 )
  {
    return nullptr;
  }

  return symbolList.at( 0 );
}

QgsPointDisplacementRenderer* QgsPointDisplacementRenderer::convertFromRenderer( const QgsFeatureRendererV2* renderer )
{
  if ( renderer->type() == "pointDisplacement" )
  {
    return dynamic_cast<QgsPointDisplacementRenderer*>( renderer->clone() );
  }

  if ( renderer->type() == "singleSymbol" ||
       renderer->type() == "categorizedSymbol" ||
       renderer->type() == "graduatedSymbol" ||
       renderer->type() == "RuleRenderer" )
  {
    QgsPointDisplacementRenderer* pointRenderer = new QgsPointDisplacementRenderer();
    pointRenderer->setEmbeddedRenderer( renderer->clone() );
    return pointRenderer;
  }
  return nullptr;
}
