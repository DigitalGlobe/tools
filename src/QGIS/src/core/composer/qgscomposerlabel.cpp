/***************************************************************************
                         qgscomposerlabel.cpp
                             -------------------
    begin                : January 2005
    copyright            : (C) 2005 by Radim Blazek
    email                : blazek@itc.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgscomposerlabel.h"
#include "qgscomposition.h"
#include "qgscomposerutils.h"
#include "qgsexpression.h"
#include "qgsnetworkaccessmanager.h"
#include "qgscomposermodel.h"
#include "qgsvectorlayer.h"
#include "qgsproject.h"
#include "qgsdistancearea.h"
#include "qgsfontutils.h"
#include "qgsexpressioncontext.h"

#include "qgswebview.h"
#include "qgswebframe.h"
#include "qgswebpage.h"

#include <QCoreApplication>
#include <QDate>
#include <QDomElement>
#include <QPainter>
#include <QSettings>
#include <QTimer>
#include <QEventLoop>

QgsComposerLabel::QgsComposerLabel( QgsComposition *composition )
    : QgsComposerItem( composition )
    , mHtmlState( 0 )
    , mHtmlUnitsToMM( 1.0 )
    , mHtmlLoaded( false )
    , mMarginX( 1.0 )
    , mMarginY( 1.0 )
    , mFontColor( QColor( 0, 0, 0 ) )
    , mHAlignment( Qt::AlignLeft )
    , mVAlignment( Qt::AlignTop )
    , mExpressionLayer( nullptr )
    , mDistanceArea( nullptr )
{
  mDistanceArea = new QgsDistanceArea();
  mHtmlUnitsToMM = htmlUnitsToMM();

  //get default composer font from settings
  QSettings settings;
  QString defaultFontString = settings.value( "/Composer/defaultFont" ).toString();
  if ( !defaultFontString.isEmpty() )
  {
    mFont.setFamily( defaultFontString );
  }

  //default to a 10 point font size
  mFont.setPointSizeF( 10 );

  //default to no background
  setBackgroundEnabled( false );

  //a label added while atlas preview is enabled needs to have the expression context set,
  //otherwise fields in the label aren't correctly evaluated until atlas preview feature changes (#9457)
  refreshExpressionContext();

  if ( mComposition )
  {
    //connect to atlas feature changes
    //to update the expression context
    connect( &mComposition->atlasComposition(), SIGNAL( featureChanged( QgsFeature* ) ), this, SLOT( refreshExpressionContext() ) );
  }

  mWebPage = new QgsWebPage( this );
  mWebPage->setIdentifier( tr( "Composer label item" ) );
  mWebPage->setNetworkAccessManager( QgsNetworkAccessManager::instance() );

  //This makes the background transparent. Found on http://blog.qt.digia.com/blog/2009/06/30/transparent-qwebview-or-qwebpage/
  QPalette palette = mWebPage->palette();
  palette.setBrush( QPalette::Base, Qt::transparent );
  mWebPage->setPalette( palette );
  //webPage->setAttribute(Qt::WA_OpaquePaintEvent, false); //this does not compile, why ?

  mWebPage->mainFrame()->setZoomFactor( 10.0 );
  mWebPage->mainFrame()->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
  mWebPage->mainFrame()->setScrollBarPolicy( Qt::Vertical, Qt::ScrollBarAlwaysOff );

  connect( mWebPage, SIGNAL( loadFinished( bool ) ), SLOT( loadingHtmlFinished( bool ) ) );
}

QgsComposerLabel::~QgsComposerLabel()
{
  delete mDistanceArea;
  delete mWebPage;
}

void QgsComposerLabel::paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget )
{
  Q_UNUSED( itemStyle );
  Q_UNUSED( pWidget );
  if ( !painter )
  {
    return;
  }
  if ( !shouldDrawItem() )
  {
    return;
  }

  drawBackground( painter );
  painter->save();

  //antialiasing on
  painter->setRenderHint( QPainter::Antialiasing, true );

  double penWidth = hasFrame() ? ( pen().widthF() / 2.0 ) : 0;
  double xPenAdjust = mMarginX < 0 ? -penWidth : penWidth;
  double yPenAdjust = mMarginY < 0 ? -penWidth : penWidth;
  QRectF painterRect( xPenAdjust + mMarginX, yPenAdjust + mMarginY, rect().width() - 2 * xPenAdjust - 2 * mMarginX, rect().height() - 2 * yPenAdjust - 2 * mMarginY );

  if ( mHtmlState )
  {
    painter->scale( 1.0 / mHtmlUnitsToMM / 10.0, 1.0 / mHtmlUnitsToMM / 10.0 );
    mWebPage->setViewportSize( QSize( painterRect.width() * mHtmlUnitsToMM * 10.0, painterRect.height() * mHtmlUnitsToMM * 10.0 ) );
    mWebPage->settings()->setUserStyleSheetUrl( createStylesheetUrl() );
    mWebPage->mainFrame()->render( painter );
  }
  else
  {
    const QString textToDraw = displayText();
    painter->setFont( mFont );
    //debug
    //painter->setPen( QColor( Qt::red ) );
    //painter->drawRect( painterRect );
    QgsComposerUtils::drawText( painter, painterRect, textToDraw, mFont, mFontColor, mHAlignment, mVAlignment, Qt::TextWordWrap );
  }

  painter->restore();

  drawFrame( painter );
  if ( isSelected() )
  {
    drawSelectionBoxes( painter );
  }
}

void QgsComposerLabel::contentChanged()
{
  if ( mHtmlState )
  {
    const QString textToDraw = displayText();

    //mHtmlLoaded tracks whether the QWebPage has completed loading
    //its html contents, set it initially to false. The loadingHtmlFinished slot will
    //set this to true after html is loaded.
    mHtmlLoaded = false;

    const QUrl baseUrl = QUrl::fromLocalFile( QgsProject::instance()->fileInfo().absoluteFilePath() );
    mWebPage->mainFrame()->setHtml( textToDraw, baseUrl );

    //For very basic html labels with no external assets, the html load will already be
    //complete before we even get a chance to start the QEventLoop. Make sure we check
    //this before starting the loop
    if ( !mHtmlLoaded )
    {
      //Setup event loop and timeout for rendering html
      QEventLoop loop;

      //Connect timeout and webpage loadFinished signals to loop
      connect( mWebPage, SIGNAL( loadFinished( bool ) ), &loop, SLOT( quit() ) );

      // Start a 20 second timeout in case html loading will never complete
      QTimer timeoutTimer;
      timeoutTimer.setSingleShot( true );
      connect( &timeoutTimer, SIGNAL( timeout() ), &loop, SLOT( quit() ) );
      timeoutTimer.start( 20000 );

      // Pause until html is loaded
      loop.exec();
    }
  }
}

/*Track when QWebPage has finished loading its html contents*/
void QgsComposerLabel::loadingHtmlFinished( bool result )
{
  Q_UNUSED( result );
  mHtmlLoaded = true;
}

double QgsComposerLabel::htmlUnitsToMM()
{
  if ( !mComposition )
  {
    return 1.0;
  }

  //TODO : fix this more precisely so that the label's default text size is the same with or without "display as html"
  return ( mComposition->printResolution() / 72.0 ); //webkit seems to assume a standard dpi of 72
}

void QgsComposerLabel::setText( const QString& text )
{
  mText = text;
  emit itemChanged();

  contentChanged();

  if ( mComposition && id().isEmpty() && !mHtmlState )
  {
    //notify the model that the display name has changed
    mComposition->itemsModel()->updateItemDisplayName( this );
  }
}

void QgsComposerLabel::setHtmlState( int state )
{
  if ( state == mHtmlState )
  {
    return;
  }

  mHtmlState = state;
  contentChanged();

  if ( mComposition && id().isEmpty() )
  {
    //notify the model that the display name has changed
    mComposition->itemsModel()->updateItemDisplayName( this );
  }
}

void QgsComposerLabel::setExpressionContext( QgsFeature *feature, QgsVectorLayer* layer, const QMap<QString, QVariant>& substitutions )
{
  mExpressionFeature.reset( feature ? new QgsFeature( *feature ) : nullptr );
  mExpressionLayer = layer;
  mSubstitutions = substitutions;

  //setup distance area conversion
  if ( layer )
  {
    mDistanceArea->setSourceCrs( layer->crs().srsid() );
  }
  else if ( mComposition )
  {
    //set to composition's mapsettings' crs
    mDistanceArea->setSourceCrs( mComposition->mapSettings().destinationCrs().srsid() );
  }
  if ( mComposition )
  {
    mDistanceArea->setEllipsoidalMode( mComposition->mapSettings().hasCrsTransformEnabled() );
  }
  mDistanceArea->setEllipsoid( QgsProject::instance()->readEntry( "Measure", "/Ellipsoid", GEO_NONE ) );
  contentChanged();

  // Force label to redraw -- fixes label printing for labels with blend modes when used with atlas
  update();
}

void QgsComposerLabel::setSubstitutions( const QMap<QString, QVariant>& substitutions )
{
  mSubstitutions = substitutions;
}

void QgsComposerLabel::refreshExpressionContext()
{
  mExpressionLayer = nullptr;
  mExpressionFeature.reset();

  if ( !mComposition )
    return;

  QgsVectorLayer* layer = nullptr;
  if ( mComposition->atlasComposition().enabled() )
  {
    layer = mComposition->atlasComposition().coverageLayer();
  }

  //setup distance area conversion
  if ( layer )
  {
    mDistanceArea->setSourceCrs( layer->crs().srsid() );
  }
  else
  {
    //set to composition's mapsettings' crs
    mDistanceArea->setSourceCrs( mComposition->mapSettings().destinationCrs().srsid() );
  }
  mDistanceArea->setEllipsoidalMode( mComposition->mapSettings().hasCrsTransformEnabled() );
  mDistanceArea->setEllipsoid( QgsProject::instance()->readEntry( "Measure", "/Ellipsoid", GEO_NONE ) );
  contentChanged();

  update();
}

QString QgsComposerLabel::displayText() const
{
  QString displayText = mText;
  replaceDateText( displayText );
  QMap<QString, QVariant> subs = mSubstitutions;

  QScopedPointer<QgsExpressionContext> context( createExpressionContext() );
  //overwrite layer/feature if they have been set via setExpressionContext
  //TODO remove when setExpressionContext is removed
  if ( mExpressionFeature.data() )
    context->setFeature( *mExpressionFeature.data() );
  if ( mExpressionLayer )
    context->setFields( mExpressionLayer->fields() );

  return QgsExpression::replaceExpressionText( displayText, context.data(), &subs, mDistanceArea );
}

void QgsComposerLabel::replaceDateText( QString& text ) const
{
  QString constant = "$CURRENT_DATE";
  int currentDatePos = text.indexOf( constant );
  if ( currentDatePos != -1 )
  {
    //check if there is a bracket just after $CURRENT_DATE
    QString formatText;
    int openingBracketPos = text.indexOf( '(', currentDatePos );
    int closingBracketPos = text.indexOf( ')', openingBracketPos + 1 );
    if ( openingBracketPos != -1 &&
         closingBracketPos != -1 &&
         ( closingBracketPos - openingBracketPos ) > 1 &&
         openingBracketPos == currentDatePos + constant.size() )
    {
      formatText = text.mid( openingBracketPos + 1, closingBracketPos - openingBracketPos - 1 );
      text.replace( currentDatePos, closingBracketPos - currentDatePos + 1, QDate::currentDate().toString( formatText ) );
    }
    else //no bracket
    {
      text.replace( "$CURRENT_DATE", QDate::currentDate().toString() );
    }
  }
}

void QgsComposerLabel::setFont( const QFont& f )
{
  mFont = f;
}

void QgsComposerLabel::setMargin( const double m )
{
  mMarginX = m;
  mMarginY = m;
  prepareGeometryChange();
}

void QgsComposerLabel::setMarginX( const double margin )
{
  mMarginX = margin;
  prepareGeometryChange();
}

void QgsComposerLabel::setMarginY( const double margin )
{
  mMarginY = margin;
  prepareGeometryChange();
}

void QgsComposerLabel::adjustSizeToText()
{
  double textWidth = QgsComposerUtils::textWidthMM( mFont, displayText() );
  double fontHeight = QgsComposerUtils::fontHeightMM( mFont );

  double penWidth = hasFrame() ? ( pen().widthF() / 2.0 ) : 0;

  double width = textWidth + 2 * mMarginX + 2 * penWidth + 1;
  double height = fontHeight + 2 * mMarginY + 2 * penWidth;

  //keep alignment point constant
  double xShift = 0;
  double yShift = 0;
  itemShiftAdjustSize( width, height, xShift, yShift );

  //update rect for data defined size and position
  QRectF evaluatedRect = evalItemRect( QRectF( pos().x() + xShift, pos().y() + yShift, width, height ) );
  setSceneRect( evaluatedRect );
}

QFont QgsComposerLabel::font() const
{
  return mFont;
}

bool QgsComposerLabel::writeXML( QDomElement& elem, QDomDocument & doc ) const
{
  if ( elem.isNull() )
  {
    return false;
  }

  QDomElement composerLabelElem = doc.createElement( "ComposerLabel" );

  composerLabelElem.setAttribute( "htmlState", mHtmlState );

  composerLabelElem.setAttribute( "labelText", mText );
  composerLabelElem.setAttribute( "marginX", QString::number( mMarginX ) );
  composerLabelElem.setAttribute( "marginY", QString::number( mMarginY ) );
  composerLabelElem.setAttribute( "halign", mHAlignment );
  composerLabelElem.setAttribute( "valign", mVAlignment );

  //font
  QDomElement labelFontElem = QgsFontUtils::toXmlElement( mFont, doc, "LabelFont" );
  composerLabelElem.appendChild( labelFontElem );

  //font color
  QDomElement fontColorElem = doc.createElement( "FontColor" );
  fontColorElem.setAttribute( "red", mFontColor.red() );
  fontColorElem.setAttribute( "green", mFontColor.green() );
  fontColorElem.setAttribute( "blue", mFontColor.blue() );
  composerLabelElem.appendChild( fontColorElem );

  elem.appendChild( composerLabelElem );
  return _writeXML( composerLabelElem, doc );
}

bool QgsComposerLabel::readXML( const QDomElement& itemElem, const QDomDocument& doc )
{
  if ( itemElem.isNull() )
  {
    return false;
  }

  //restore label specific properties

  //text
  mText = itemElem.attribute( "labelText" );

  //html state
  mHtmlState = itemElem.attribute( "htmlState" ).toInt();

  //margin
  bool marginXOk = false;
  bool marginYOk = false;
  mMarginX = itemElem.attribute( "marginX" ).toDouble( &marginXOk );
  mMarginY = itemElem.attribute( "marginY" ).toDouble( &marginYOk );
  if ( !marginXOk || !marginYOk )
  {
    //upgrade old projects where margins where stored in a single attribute
    double margin = itemElem.attribute( "margin", "1.0" ).toDouble();
    mMarginX = margin;
    mMarginY = margin;
  }

  //Horizontal alignment
  mHAlignment = static_cast< Qt::AlignmentFlag >( itemElem.attribute( "halign" ).toInt() );

  //Vertical alignment
  mVAlignment = static_cast< Qt::AlignmentFlag >( itemElem.attribute( "valign" ).toInt() );

  //font
  QgsFontUtils::setFromXmlChildNode( mFont, itemElem, "LabelFont" );

  //font color
  QDomNodeList fontColorList = itemElem.elementsByTagName( "FontColor" );
  if ( !fontColorList.isEmpty() )
  {
    QDomElement fontColorElem = fontColorList.at( 0 ).toElement();
    int red = fontColorElem.attribute( "red", "0" ).toInt();
    int green = fontColorElem.attribute( "green", "0" ).toInt();
    int blue = fontColorElem.attribute( "blue", "0" ).toInt();
    mFontColor = QColor( red, green, blue );
  }
  else
  {
    mFontColor = QColor( 0, 0, 0 );
  }

  //restore general composer item properties
  QDomNodeList composerItemList = itemElem.elementsByTagName( "ComposerItem" );
  if ( !composerItemList.isEmpty() )
  {
    QDomElement composerItemElem = composerItemList.at( 0 ).toElement();

    //rotation
    if ( !qgsDoubleNear( composerItemElem.attribute( "rotation", "0" ).toDouble(), 0.0 ) )
    {
      //check for old (pre 2.1) rotation attribute
      setItemRotation( composerItemElem.attribute( "rotation", "0" ).toDouble() );
    }

    _readXML( composerItemElem, doc );
  }
  emit itemChanged();
  contentChanged();
  return true;
}

QString QgsComposerLabel::displayName() const
{
  if ( !id().isEmpty() )
  {
    return id();
  }

  if ( mHtmlState )
  {
    return tr( "<HTML label>" );
  }

  //if no id, default to portion of label text
  QString text = mText;
  if ( text.isEmpty() )
  {
    return tr( "<label>" );
  }
  if ( text.length() > 25 )
  {
    return QString( tr( "%1..." ) ).arg( text.left( 25 ).simplified() );
  }
  else
  {
    return text.simplified();
  }
}

QRectF QgsComposerLabel::boundingRect() const
{
  QRectF rectangle = rect();
  double penWidth = hasFrame() ? ( pen().widthF() / 2.0 ) : 0;
  rectangle.adjust( -penWidth, -penWidth, penWidth, penWidth );

  if ( mMarginX < 0 )
  {
    rectangle.adjust( mMarginX, 0, -mMarginX, 0 );
  }
  if ( mMarginY < 0 )
  {
    rectangle.adjust( 0, mMarginY, 0, -mMarginY );
  }

  return rectangle;
}

void QgsComposerLabel::setFrameEnabled( const bool drawFrame )
{
  QgsComposerItem::setFrameEnabled( drawFrame );
  prepareGeometryChange();
}

void QgsComposerLabel::setFrameOutlineWidth( const double outlineWidth )
{
  QgsComposerItem::setFrameOutlineWidth( outlineWidth );
  prepareGeometryChange();
}

void QgsComposerLabel::itemShiftAdjustSize( double newWidth, double newHeight, double& xShift, double& yShift ) const
{
  //keep alignment point constant
  double currentWidth = rect().width();
  double currentHeight = rect().height();
  xShift = 0;
  yShift = 0;

  if ( mItemRotation >= 0 && mItemRotation < 90 )
  {
    if ( mHAlignment == Qt::AlignHCenter )
    {
      xShift = - ( newWidth - currentWidth ) / 2.0;
    }
    else if ( mHAlignment == Qt::AlignRight )
    {
      xShift = - ( newWidth - currentWidth );
    }
    if ( mVAlignment == Qt::AlignVCenter )
    {
      yShift = -( newHeight - currentHeight ) / 2.0;
    }
    else if ( mVAlignment == Qt::AlignBottom )
    {
      yShift = - ( newHeight - currentHeight );
    }
  }
  if ( mItemRotation >= 90 && mItemRotation < 180 )
  {
    if ( mHAlignment == Qt::AlignHCenter )
    {
      yShift = -( newHeight  - currentHeight ) / 2.0;
    }
    else if ( mHAlignment == Qt::AlignRight )
    {
      yShift = -( newHeight  - currentHeight );
    }
    if ( mVAlignment == Qt::AlignTop )
    {
      xShift = -( newWidth - currentWidth );
    }
    else if ( mVAlignment == Qt::AlignVCenter )
    {
      xShift = -( newWidth - currentWidth / 2.0 );
    }
  }
  else if ( mItemRotation >= 180 && mItemRotation < 270 )
  {
    if ( mHAlignment == Qt::AlignHCenter )
    {
      xShift = -( newWidth - currentWidth ) / 2.0;
    }
    else if ( mHAlignment == Qt::AlignLeft )
    {
      xShift = -( newWidth - currentWidth );
    }
    if ( mVAlignment == Qt::AlignVCenter )
    {
      yShift = ( newHeight - currentHeight ) / 2.0;
    }
    else if ( mVAlignment == Qt::AlignTop )
    {
      yShift = ( newHeight - currentHeight );
    }
  }
  else if ( mItemRotation >= 270 && mItemRotation < 360 )
  {
    if ( mHAlignment == Qt::AlignHCenter )
    {
      yShift = -( newHeight  - currentHeight ) / 2.0;
    }
    else if ( mHAlignment == Qt::AlignLeft )
    {
      yShift = -( newHeight  - currentHeight );
    }
    if ( mVAlignment == Qt::AlignBottom )
    {
      xShift = -( newWidth - currentWidth );
    }
    else if ( mVAlignment == Qt::AlignVCenter )
    {
      xShift = -( newWidth - currentWidth / 2.0 );
    }
  }
}

QUrl QgsComposerLabel::createStylesheetUrl() const
{
  QString stylesheet;
  stylesheet += QString( "body { margin: %1 %2;" ).arg( qMax( mMarginX * mHtmlUnitsToMM, 0.0 ) ).arg( qMax( mMarginY * mHtmlUnitsToMM, 0.0 ) );
  stylesheet += QgsFontUtils::asCSS( mFont, 0.352778 * mHtmlUnitsToMM );
  stylesheet += QString( "color: %1;" ).arg( mFontColor.name() );
  stylesheet += QString( "text-align: %1; }" ).arg( mHAlignment == Qt::AlignLeft ? "left" : mHAlignment == Qt::AlignRight ? "right" : "center" );

  QByteArray ba;
  ba.append( stylesheet.toUtf8() );
  QUrl cssFileURL = QUrl( "data:text/css;charset=utf-8;base64," + ba.toBase64() );

  return cssFileURL;
}
