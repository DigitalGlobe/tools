/***************************************************************************
                         qgscomposerlegend.cpp  -  description
                         ---------------------
    begin                : June 2008
    copyright            : (C) 2008 by Marco Hugentobler
    email                : marco dot hugentobler at karto dot baug dot ethz dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <limits>

#include "qgscomposerlegendstyle.h"
#include "qgscomposerlegend.h"
#include "qgscomposerlegenditem.h"
#include "qgscomposermap.h"
#include "qgscomposition.h"
#include "qgscomposermodel.h"
#include "qgsmaplayerregistry.h"
#include "qgslayertree.h"
#include "qgslayertreemodel.h"
#include "qgslegendrenderer.h"
#include "qgslogger.h"
#include "qgsproject.h"
#include "qgssymbollayerv2utils.h"
#include "qgslayertreeutils.h"
#include <QDomDocument>
#include <QDomElement>
#include <QPainter>

QgsComposerLegend::QgsComposerLegend( QgsComposition* composition )
    : QgsComposerItem( composition )
    , mCustomLayerTree( nullptr )
    , mComposerMap( nullptr )
    , mLegendFilterByMap( false )
    , mFilterOutAtlas( false )
    , mFilterAskedForUpdate( false )
    , mInAtlas( false )
{
  mLegendModel2 = new QgsLegendModelV2( QgsProject::instance()->layerTreeRoot() );

  adjustBoxSize();

  connect( &mLegendModel, SIGNAL( layersChanged() ), this, SLOT( synchronizeWithModel() ) );

  connect( &composition->atlasComposition(), SIGNAL( renderEnded() ), this, SLOT( onAtlasEnded() ) );
  connect( &composition->atlasComposition(), SIGNAL( featureChanged( QgsFeature* ) ), this, SLOT( onAtlasFeature( QgsFeature* ) ) );

  // Connect to the main layertreeroot.
  // It serves in "auto update mode" as a medium between the main app legend and this one
  connect( QgsProject::instance()->layerTreeRoot(), SIGNAL( customPropertyChanged( QgsLayerTreeNode*, QString ) ), this, SLOT( nodeCustomPropertyChanged( QgsLayerTreeNode*, QString ) ) );
}

QgsComposerLegend::QgsComposerLegend()
    : QgsComposerItem( nullptr )
    , mLegendModel2( nullptr )
    , mCustomLayerTree( nullptr )
    , mComposerMap( nullptr )
    , mLegendFilterByMap( false )
    , mLegendFilterByExpression( false )
    , mFilterOutAtlas( false )
    , mFilterAskedForUpdate( false )
    , mInAtlas( false )
{

}

QgsComposerLegend::~QgsComposerLegend()
{
  delete mLegendModel2;
  delete mCustomLayerTree;
}

void QgsComposerLegend::paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget )
{
  Q_UNUSED( itemStyle );
  Q_UNUSED( pWidget );

  if ( !painter )
    return;

  if ( !shouldDrawItem() )
  {
    return;
  }

  if ( mFilterAskedForUpdate )
  {
    mFilterAskedForUpdate = false;
    doUpdateFilterByMap();
  }

  int dpi = painter->device()->logicalDpiX();
  double dotsPerMM = dpi / 25.4;

  if ( mComposition )
  {
    mSettings.setUseAdvancedEffects( mComposition->useAdvancedEffects() );
    mSettings.setDpi( dpi );
  }
  if ( mComposerMap )
  {
    mSettings.setMmPerMapUnit( mComposerMap->mapUnitsToMM() );

    // use a temporary QgsMapSettings to find out real map scale
    QgsMapSettings ms = mComposerMap->composition()->mapSettings();
    ms.setOutputSize( QSizeF( mComposerMap->rect().width() * dotsPerMM, mComposerMap->rect().height() * dotsPerMM ).toSize() );
    ms.setExtent( *mComposerMap->currentMapExtent() );
    ms.setOutputDpi( dpi );
    mSettings.setMapScale( ms.scale() );
  }

  drawBackground( painter );
  painter->save();
  //antialiasing on
  painter->setRenderHint( QPainter::Antialiasing, true );
  painter->setPen( QPen( QColor( 0, 0, 0 ) ) );

  QgsLegendRenderer legendRenderer( mLegendModel2, mSettings );
  legendRenderer.setLegendSize( rect().size() );

  //adjust box if width or height is too small
  QSizeF size = legendRenderer.minimumSize();
  if ( size.height() > rect().height() || size.width() > rect().width() )
  {
    //need to resize box
    QRectF targetRect = QRectF( pos().x(), pos().y(), rect().width(), rect().height() );
    if ( size.height() > targetRect.height() )
      targetRect.setHeight( size.height() );
    if ( size.width() > rect().width() )
      targetRect.setWidth( size.width() );

    //set new rect, respecting position mode and data defined size/position
    setSceneRect( evalItemRect( targetRect, true ) );
  }

  legendRenderer.drawLegend( painter );

  painter->restore();

  //draw frame and selection boxes if necessary
  drawFrame( painter );
  if ( isSelected() )
  {
    drawSelectionBoxes( painter );
  }
}

QSizeF QgsComposerLegend::paintAndDetermineSize( QPainter* painter )
{
  if ( mFilterAskedForUpdate )
  {
    mFilterAskedForUpdate = false;
    doUpdateFilterByMap();
  }

  QgsLegendRenderer legendRenderer( mLegendModel2, mSettings );
  QSizeF size = legendRenderer.minimumSize();
  if ( painter )
    legendRenderer.drawLegend( painter );
  return size;
}


void QgsComposerLegend::adjustBoxSize()
{
  QgsLegendRenderer legendRenderer( mLegendModel2, mSettings );
  QSizeF size = legendRenderer.minimumSize();
  QgsDebugMsg( QString( "width = %1 height = %2" ).arg( size.width() ).arg( size.height() ) );
  if ( size.isValid() )
  {
    QRectF targetRect = QRectF( pos().x(), pos().y(), size.width(), size.height() );
    //set new rect, respecting position mode and data defined size/position
    setSceneRect( evalItemRect( targetRect, true ) );
  }
}


void QgsComposerLegend::setCustomLayerTree( QgsLayerTreeGroup* rootGroup )
{
  mLegendModel2->setRootGroup( rootGroup ? rootGroup : QgsProject::instance()->layerTreeRoot() );

  delete mCustomLayerTree;
  mCustomLayerTree = rootGroup;
}


void QgsComposerLegend::setAutoUpdateModel( bool autoUpdate )
{
  if ( autoUpdate == autoUpdateModel() )
    return;

  setCustomLayerTree( autoUpdate ? nullptr : QgsLayerTree::toGroup( QgsProject::instance()->layerTreeRoot()->clone() ) );
  adjustBoxSize();
  updateItem();
}

void QgsComposerLegend::nodeCustomPropertyChanged( QgsLayerTreeNode*, const QString& )
{
  if ( autoUpdateModel() )
  {
    // in "auto update" mode, some parameters on the main app legend may have been changed (expression filtering)
    // we must then call updateItem to reflect the changes
    updateItem();
  }
}

bool QgsComposerLegend::autoUpdateModel() const
{
  return !mCustomLayerTree;
}

void QgsComposerLegend::setLegendFilterByMapEnabled( bool enabled )
{
  mLegendFilterByMap = enabled;
  updateItem();
}

void QgsComposerLegend::setTitle( const QString& t )
{
  mSettings.setTitle( t );

  if ( mComposition && id().isEmpty() )
  {
    //notify the model that the display name has changed
    mComposition->itemsModel()->updateItemDisplayName( this );
  }
}
QString QgsComposerLegend::title() const { return mSettings.title(); }

Qt::AlignmentFlag QgsComposerLegend::titleAlignment() const { return mSettings.titleAlignment(); }
void QgsComposerLegend::setTitleAlignment( Qt::AlignmentFlag alignment ) { mSettings.setTitleAlignment( alignment ); }

QgsComposerLegendStyle& QgsComposerLegend::rstyle( QgsComposerLegendStyle::Style s ) { return mSettings.rstyle( s ); }
QgsComposerLegendStyle QgsComposerLegend::style( QgsComposerLegendStyle::Style s ) const { return mSettings.style( s ); }
void QgsComposerLegend::setStyle( QgsComposerLegendStyle::Style s, const QgsComposerLegendStyle& style ) { mSettings.setStyle( s, style ); }

QFont QgsComposerLegend::styleFont( QgsComposerLegendStyle::Style s ) const { return mSettings.style( s ).font(); }
void QgsComposerLegend::setStyleFont( QgsComposerLegendStyle::Style s, const QFont& f ) { rstyle( s ).setFont( f ); }

void QgsComposerLegend::setStyleMargin( QgsComposerLegendStyle::Style s, double margin ) { rstyle( s ).setMargin( margin ); }
void QgsComposerLegend::setStyleMargin( QgsComposerLegendStyle::Style s, QgsComposerLegendStyle::Side side, double margin ) { rstyle( s ).setMargin( side, margin ); }

double QgsComposerLegend::boxSpace() const { return mSettings.boxSpace(); }
void QgsComposerLegend::setBoxSpace( double s ) { mSettings.setBoxSpace( s ); }

double QgsComposerLegend::columnSpace() const { return mSettings.columnSpace(); }
void QgsComposerLegend::setColumnSpace( double s ) { mSettings.setColumnSpace( s ); }

QColor QgsComposerLegend::fontColor() const { return mSettings.fontColor(); }
void QgsComposerLegend::setFontColor( const QColor& c ) { mSettings.setFontColor( c ); }

double QgsComposerLegend::symbolWidth() const { return mSettings.symbolSize().width(); }
void QgsComposerLegend::setSymbolWidth( double w ) { mSettings.setSymbolSize( QSizeF( w, mSettings.symbolSize().height() ) ); }

double QgsComposerLegend::symbolHeight() const { return mSettings.symbolSize().height(); }
void QgsComposerLegend::setSymbolHeight( double h ) { mSettings.setSymbolSize( QSizeF( mSettings.symbolSize().width(), h ) ); }

double QgsComposerLegend::wmsLegendWidth() const { return mSettings.wmsLegendSize().width(); }
void QgsComposerLegend::setWmsLegendWidth( double w ) { mSettings.setWmsLegendSize( QSizeF( w, mSettings.wmsLegendSize().height() ) ); }

double QgsComposerLegend::wmsLegendHeight() const {return mSettings.wmsLegendSize().height(); }
void QgsComposerLegend::setWmsLegendHeight( double h ) { mSettings.setWmsLegendSize( QSizeF( mSettings.wmsLegendSize().width(), h ) ); }

void QgsComposerLegend::setWrapChar( const QString& t ) { mSettings.setWrapChar( t ); }
QString QgsComposerLegend::wrapChar() const {return mSettings.wrapChar(); }

int QgsComposerLegend::columnCount() const { return mSettings.columnCount(); }
void QgsComposerLegend::setColumnCount( int c ) { mSettings.setColumnCount( c ); }

bool QgsComposerLegend::splitLayer() const { return mSettings.splitLayer(); }
void QgsComposerLegend::setSplitLayer( bool s ) { mSettings.setSplitLayer( s ); }

bool QgsComposerLegend::equalColumnWidth() const { return mSettings.equalColumnWidth(); }
void QgsComposerLegend::setEqualColumnWidth( bool s ) { mSettings.setEqualColumnWidth( s ); }

bool QgsComposerLegend::drawRasterBorder() const { return mSettings.drawRasterBorder(); }
void QgsComposerLegend::setDrawRasterBorder( bool enabled ) { mSettings.setDrawRasterBorder( enabled ); }

QColor QgsComposerLegend::rasterBorderColor() const { return mSettings.rasterBorderColor(); }
void QgsComposerLegend::setRasterBorderColor( const QColor& color ) { mSettings.setRasterBorderColor( color ); }

double QgsComposerLegend::rasterBorderWidth() const { return mSettings.rasterBorderWidth(); }
void QgsComposerLegend::setRasterBorderWidth( double width ) { mSettings.setRasterBorderWidth( width ); }

void QgsComposerLegend::synchronizeWithModel()
{
  adjustBoxSize();
  updateItem();
}

void QgsComposerLegend::updateLegend()
{
  // take layer list from map renderer (to have legend order)
  mLegendModel.setLayerSet( mComposition ? mComposition->mapSettings().layers() : QStringList() );
  adjustBoxSize();
  updateItem();
}

void QgsComposerLegend::updateItem()
{
  updateFilterByMap( false );
  QgsComposerItem::updateItem();
}

bool QgsComposerLegend::writeXML( QDomElement& elem, QDomDocument & doc ) const
{
  if ( elem.isNull() )
  {
    return false;
  }

  QDomElement composerLegendElem = doc.createElement( "ComposerLegend" );
  elem.appendChild( composerLegendElem );

  //write general properties
  composerLegendElem.setAttribute( "title", mSettings.title() );
  composerLegendElem.setAttribute( "titleAlignment", QString::number( static_cast< int >( mSettings.titleAlignment() ) ) );
  composerLegendElem.setAttribute( "columnCount", QString::number( mSettings.columnCount() ) );
  composerLegendElem.setAttribute( "splitLayer", QString::number( mSettings.splitLayer() ) );
  composerLegendElem.setAttribute( "equalColumnWidth", QString::number( mSettings.equalColumnWidth() ) );

  composerLegendElem.setAttribute( "boxSpace", QString::number( mSettings.boxSpace() ) );
  composerLegendElem.setAttribute( "columnSpace", QString::number( mSettings.columnSpace() ) );

  composerLegendElem.setAttribute( "symbolWidth", QString::number( mSettings.symbolSize().width() ) );
  composerLegendElem.setAttribute( "symbolHeight", QString::number( mSettings.symbolSize().height() ) );

  composerLegendElem.setAttribute( "rasterBorder", mSettings.drawRasterBorder() );
  composerLegendElem.setAttribute( "rasterBorderColor", QgsSymbolLayerV2Utils::encodeColor( mSettings.rasterBorderColor() ) );
  composerLegendElem.setAttribute( "rasterBorderWidth", QString::number( mSettings.rasterBorderWidth() ) );

  composerLegendElem.setAttribute( "wmsLegendWidth", QString::number( mSettings.wmsLegendSize().width() ) );
  composerLegendElem.setAttribute( "wmsLegendHeight", QString::number( mSettings.wmsLegendSize().height() ) );
  composerLegendElem.setAttribute( "wrapChar", mSettings.wrapChar() );
  composerLegendElem.setAttribute( "fontColor", mSettings.fontColor().name() );

  if ( mComposerMap )
  {
    composerLegendElem.setAttribute( "map", mComposerMap->id() );
  }

  QDomElement composerLegendStyles = doc.createElement( "styles" );
  composerLegendElem.appendChild( composerLegendStyles );

  style( QgsComposerLegendStyle::Title ).writeXML( "title", composerLegendStyles, doc );
  style( QgsComposerLegendStyle::Group ).writeXML( "group", composerLegendStyles, doc );
  style( QgsComposerLegendStyle::Subgroup ).writeXML( "subgroup", composerLegendStyles, doc );
  style( QgsComposerLegendStyle::Symbol ).writeXML( "symbol", composerLegendStyles, doc );
  style( QgsComposerLegendStyle::SymbolLabel ).writeXML( "symbolLabel", composerLegendStyles, doc );

  if ( mCustomLayerTree )
  {
    // if not using auto-update - store the custom layer tree
    mCustomLayerTree->writeXML( composerLegendElem );
  }

  if ( mLegendFilterByMap )
  {
    composerLegendElem.setAttribute( "legendFilterByMap", "1" );
  }

  return _writeXML( composerLegendElem, doc );
}

static void _readOldLegendGroup( QDomElement& elem, QgsLayerTreeGroup* parentGroup )
{
  QDomElement itemElem = elem.firstChildElement();

  while ( !itemElem.isNull() )
  {

    if ( itemElem.tagName() == "LayerItem" )
    {
      QString layerId = itemElem.attribute( "layerId" );
      if ( QgsMapLayer* layer = QgsMapLayerRegistry::instance()->mapLayer( layerId ) )
      {
        QgsLayerTreeLayer* nodeLayer = parentGroup->addLayer( layer );
        QString userText = itemElem.attribute( "userText" );
        if ( !userText.isEmpty() )
          nodeLayer->setCustomProperty( "legend/title-label", userText );
        QString style = itemElem.attribute( "style" );
        if ( !style.isEmpty() )
          nodeLayer->setCustomProperty( "legend/title-style", style );
        QString showFeatureCount = itemElem.attribute( "showFeatureCount" );
        if ( showFeatureCount.toInt() )
          nodeLayer->setCustomProperty( "showFeatureCount", 1 );

        // support for individual legend items (user text, order) not implemented yet
      }
    }
    else if ( itemElem.tagName() == "GroupItem" )
    {
      QgsLayerTreeGroup* nodeGroup = parentGroup->addGroup( itemElem.attribute( "userText" ) );
      QString style = itemElem.attribute( "style" );
      if ( !style.isEmpty() )
        nodeGroup->setCustomProperty( "legend/title-style", style );

      _readOldLegendGroup( itemElem, nodeGroup );
    }

    itemElem = itemElem.nextSiblingElement();
  }
}

bool QgsComposerLegend::readXML( const QDomElement& itemElem, const QDomDocument& doc )
{
  if ( itemElem.isNull() )
  {
    return false;
  }

  //read general properties
  mSettings.setTitle( itemElem.attribute( "title" ) );
  if ( !itemElem.attribute( "titleAlignment" ).isEmpty() )
  {
    mSettings.setTitleAlignment( static_cast< Qt::AlignmentFlag >( itemElem.attribute( "titleAlignment" ).toInt() ) );
  }
  int colCount = itemElem.attribute( "columnCount", "1" ).toInt();
  if ( colCount < 1 ) colCount = 1;
  mSettings.setColumnCount( colCount );
  mSettings.setSplitLayer( itemElem.attribute( "splitLayer", "0" ).toInt() == 1 );
  mSettings.setEqualColumnWidth( itemElem.attribute( "equalColumnWidth", "0" ).toInt() == 1 );

  QDomNodeList stylesNodeList = itemElem.elementsByTagName( "styles" );
  if ( !stylesNodeList.isEmpty() )
  {
    QDomNode stylesNode = stylesNodeList.at( 0 );
    for ( int i = 0; i < stylesNode.childNodes().size(); i++ )
    {
      QDomElement styleElem = stylesNode.childNodes().at( i ).toElement();
      QgsComposerLegendStyle style;
      style.readXML( styleElem, doc );
      QString name = styleElem.attribute( "name" );
      QgsComposerLegendStyle::Style s;
      if ( name == "title" ) s = QgsComposerLegendStyle::Title;
      else if ( name == "group" ) s = QgsComposerLegendStyle::Group;
      else if ( name == "subgroup" ) s = QgsComposerLegendStyle::Subgroup;
      else if ( name == "symbol" ) s = QgsComposerLegendStyle::Symbol;
      else if ( name == "symbolLabel" ) s = QgsComposerLegendStyle::SymbolLabel;
      else continue;
      setStyle( s, style );
    }
  }

  //font color
  QColor fontClr;
  fontClr.setNamedColor( itemElem.attribute( "fontColor", "#000000" ) );
  mSettings.setFontColor( fontClr );

  //spaces
  mSettings.setBoxSpace( itemElem.attribute( "boxSpace", "2.0" ).toDouble() );
  mSettings.setColumnSpace( itemElem.attribute( "columnSpace", "2.0" ).toDouble() );

  mSettings.setSymbolSize( QSizeF( itemElem.attribute( "symbolWidth", "7.0" ).toDouble(), itemElem.attribute( "symbolHeight", "14.0" ).toDouble() ) );
  mSettings.setWmsLegendSize( QSizeF( itemElem.attribute( "wmsLegendWidth", "50" ).toDouble(), itemElem.attribute( "wmsLegendHeight", "25" ).toDouble() ) );

  mSettings.setDrawRasterBorder( itemElem.attribute( "rasterBorder", "1" ) != "0" );
  mSettings.setRasterBorderColor( QgsSymbolLayerV2Utils::decodeColor( itemElem.attribute( "rasterBorderColor", "0,0,0" ) ) );
  mSettings.setRasterBorderWidth( itemElem.attribute( "rasterBorderWidth", "0" ).toDouble() );

  mSettings.setWrapChar( itemElem.attribute( "wrapChar" ) );

  //composer map
  mLegendFilterByMap = itemElem.attribute( "legendFilterByMap", "0" ).toInt();
  if ( !itemElem.attribute( "map" ).isEmpty() )
  {
    setComposerMap( mComposition->getComposerMapById( itemElem.attribute( "map" ).toInt() ) );
  }

  QDomElement oldLegendModelElem = itemElem.firstChildElement( "Model" );
  if ( !oldLegendModelElem.isNull() )
  {
    // QGIS <= 2.4
    QgsLayerTreeGroup* nodeRoot = new QgsLayerTreeGroup();
    _readOldLegendGroup( oldLegendModelElem, nodeRoot );
    setCustomLayerTree( nodeRoot );
  }
  else
  {
    // QGIS >= 2.6
    QDomElement layerTreeElem = itemElem.firstChildElement( "layer-tree-group" );
    setCustomLayerTree( QgsLayerTreeGroup::readXML( layerTreeElem ) );
  }

  //restore general composer item properties
  QDomNodeList composerItemList = itemElem.elementsByTagName( "ComposerItem" );
  if ( !composerItemList.isEmpty() )
  {
    QDomElement composerItemElem = composerItemList.at( 0 ).toElement();
    _readXML( composerItemElem, doc );
  }

  // < 2.0 projects backward compatibility >>>>>
  //title font
  QString titleFontString = itemElem.attribute( "titleFont" );
  if ( !titleFontString.isEmpty() )
  {
    rstyle( QgsComposerLegendStyle::Title ).rfont().fromString( titleFontString );
  }
  //group font
  QString groupFontString = itemElem.attribute( "groupFont" );
  if ( !groupFontString.isEmpty() )
  {
    rstyle( QgsComposerLegendStyle::Group ).rfont().fromString( groupFontString );
  }

  //layer font
  QString layerFontString = itemElem.attribute( "layerFont" );
  if ( !layerFontString.isEmpty() )
  {
    rstyle( QgsComposerLegendStyle::Subgroup ).rfont().fromString( layerFontString );
  }
  //item font
  QString itemFontString = itemElem.attribute( "itemFont" );
  if ( !itemFontString.isEmpty() )
  {
    rstyle( QgsComposerLegendStyle::SymbolLabel ).rfont().fromString( itemFontString );
  }

  if ( !itemElem.attribute( "groupSpace" ).isEmpty() )
  {
    rstyle( QgsComposerLegendStyle::Group ).setMargin( QgsComposerLegendStyle::Top, itemElem.attribute( "groupSpace", "3.0" ).toDouble() );
  }
  if ( !itemElem.attribute( "layerSpace" ).isEmpty() )
  {
    rstyle( QgsComposerLegendStyle::Subgroup ).setMargin( QgsComposerLegendStyle::Top, itemElem.attribute( "layerSpace", "3.0" ).toDouble() );
  }
  if ( !itemElem.attribute( "symbolSpace" ).isEmpty() )
  {
    rstyle( QgsComposerLegendStyle::Symbol ).setMargin( QgsComposerLegendStyle::Top, itemElem.attribute( "symbolSpace", "2.0" ).toDouble() );
    rstyle( QgsComposerLegendStyle::SymbolLabel ).setMargin( QgsComposerLegendStyle::Top, itemElem.attribute( "symbolSpace", "2.0" ).toDouble() );
  }
  // <<<<<<< < 2.0 projects backward compatibility

  emit itemChanged();
  return true;
}

QString QgsComposerLegend::displayName() const
{
  if ( !id().isEmpty() )
  {
    return id();
  }

  //if no id, default to portion of title text
  QString text = mSettings.title();
  if ( text.isEmpty() )
  {
    return tr( "<legend>" );
  }
  if ( text.length() > 25 )
  {
    return QString( tr( "%1..." ) ).arg( text.left( 25 ) );
  }
  else
  {
    return text;
  }
}

void QgsComposerLegend::setComposerMap( const QgsComposerMap* map )
{
  if ( mComposerMap )
  {
    disconnect( mComposerMap, SIGNAL( destroyed( QObject* ) ), this, SLOT( invalidateCurrentMap() ) );
    disconnect( mComposerMap, SIGNAL( itemChanged() ), this, SLOT( updateFilterByMap() ) );
    disconnect( mComposerMap, SIGNAL( extentChanged() ), this, SLOT( updateFilterByMap() ) );
    disconnect( mComposerMap, SIGNAL( layerStyleOverridesChanged() ), this, SLOT( mapLayerStyleOverridesChanged() ) );
  }

  mComposerMap = map;

  if ( map )
  {
    QObject::connect( map, SIGNAL( destroyed( QObject* ) ), this, SLOT( invalidateCurrentMap() ) );
    QObject::connect( map, SIGNAL( itemChanged() ), this, SLOT( updateFilterByMap() ) );
    QObject::connect( map, SIGNAL( extentChanged() ), this, SLOT( updateFilterByMap() ) );
    QObject::connect( map, SIGNAL( layerStyleOverridesChanged() ), this, SLOT( mapLayerStyleOverridesChanged() ) );
  }

  updateItem();
}

void QgsComposerLegend::invalidateCurrentMap()
{
  setComposerMap( nullptr );
}

void QgsComposerLegend::mapLayerStyleOverridesChanged()
{
  if ( !mComposerMap )
    return;

  // map's style has been changed, so make sure to update the legend here
  if ( mLegendFilterByMap )
  {
    // legend is being filtered by map, so we need to re run the hit test too
    // as the style overrides may also have affected the visible symbols
    updateFilterByMap( false );
  }
  else
  {
    mLegendModel2->setLayerStyleOverrides( mComposerMap->layerStyleOverrides() );

    Q_FOREACH ( QgsLayerTreeLayer* nodeLayer, mLegendModel2->rootGroup()->findLayers() )
      mLegendModel2->refreshLayerLegend( nodeLayer );
  }

  adjustBoxSize();
  updateItem();
}

void QgsComposerLegend::updateFilterByMap( bool redraw )
{
  if ( isRemoved() )
    return;
  // ask for update
  // the actual update will take place before the redraw.
  // This is to avoid multiple calls to the filter
  mFilterAskedForUpdate = true;

  if ( redraw )
    QgsComposerItem::updateItem();
}

void QgsComposerLegend::doUpdateFilterByMap()
{
  if ( mComposerMap )
    mLegendModel2->setLayerStyleOverrides( mComposerMap->layerStyleOverrides() );
  else
    mLegendModel2->setLayerStyleOverrides( QMap<QString, QString>() );


  bool filterByExpression = QgsLayerTreeUtils::hasLegendFilterExpression( *( mCustomLayerTree ? mCustomLayerTree : QgsProject::instance()->layerTreeRoot() ) );

  if ( mComposerMap && ( mLegendFilterByMap || filterByExpression || mInAtlas ) )
  {
    int dpi = mComposition->printResolution();

    QgsRectangle requestRectangle;
    mComposerMap->requestedExtent( requestRectangle );

    QSizeF theSize( requestRectangle.width(), requestRectangle.height() );
    theSize *= mComposerMap->mapUnitsToMM() * dpi / 25.4;

    QgsMapSettings ms = mComposerMap->mapSettings( requestRectangle, theSize, dpi );

    QgsGeometry filterPolygon;
    if ( mInAtlas )
    {
      filterPolygon = composition()->atlasComposition().currentGeometry( composition()->mapSettings().destinationCrs() );
    }
    mLegendModel2->setLegendFilter( &ms, /* useExtent */ mInAtlas || mLegendFilterByMap, filterPolygon, /* useExpressions */ true );
  }
  else
    mLegendModel2->setLegendFilterByMap( nullptr );
}

void QgsComposerLegend::setLegendFilterOutAtlas( bool doFilter )
{
  mFilterOutAtlas = doFilter;
}

bool QgsComposerLegend::legendFilterOutAtlas() const
{
  return mFilterOutAtlas;
}

void QgsComposerLegend::onAtlasFeature( QgsFeature* feat )
{
  if ( !feat )
    return;
  mInAtlas = mFilterOutAtlas;
  updateFilterByMap();
}

void QgsComposerLegend::onAtlasEnded()
{
  mInAtlas = false;
  updateFilterByMap();
}

// -------------------------------------------------------------------------
#include "qgslayertreemodellegendnode.h"
#include "qgsvectorlayer.h"

QgsLegendModelV2::QgsLegendModelV2( QgsLayerTreeGroup* rootNode, QObject* parent )
    : QgsLayerTreeModel( rootNode, parent )
{
  setFlag( QgsLayerTreeModel::AllowLegendChangeState, false );
  setFlag( QgsLayerTreeModel::AllowNodeReorder, true );
}

QVariant QgsLegendModelV2::data( const QModelIndex& index, int role ) const
{
  // handle custom layer node labels
  if ( QgsLayerTreeNode* node = index2node( index ) )
  {
    if ( QgsLayerTree::isLayer( node ) && ( role == Qt::DisplayRole || role == Qt::EditRole ) && !node->customProperty( "legend/title-label" ).isNull() )
    {
      QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
      QString name = node->customProperty( "legend/title-label" ).toString();
      if ( nodeLayer->customProperty( "showFeatureCount", 0 ).toInt() && role == Qt::DisplayRole )
      {
        QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer*>( nodeLayer->layer() );
        if ( vlayer && vlayer->featureCount() >= 0 )
          name += QString( " [%1]" ).arg( vlayer->featureCount() );
      }
      return name;
    }
  }

  return QgsLayerTreeModel::data( index, role );
}

Qt::ItemFlags QgsLegendModelV2::flags( const QModelIndex& index ) const
{
  // make the legend nodes selectable even if they are not by default
  if ( index2legendNode( index ) )
    return QgsLayerTreeModel::flags( index ) | Qt::ItemIsSelectable;

  return QgsLayerTreeModel::flags( index );
}
